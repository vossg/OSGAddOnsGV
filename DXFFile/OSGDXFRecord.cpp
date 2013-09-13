/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2003 by the OpenSG Forum                 *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 *   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                License                                    *
 *                                                                           *
 * This library is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Library General Public License as published    *
 * by the Free Software Foundation, version 2.                               *
 *                                                                           *
 * This library is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public         *
 * License along with this library; if not, write to the Free Software       *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Changes                                    *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include <string>

#include "OSGConfig.h"
#include "OSGLog.h"

#include "OSGDXFRecord.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFRecord
    \ingroup GrpSystemFileIO

This class also is responsible for actually fetching data from the
file. It hides whether the input file is binary or ASCII DXF.

\todo Binary DXF (not DWG!) not working yet!

 */

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

//! Set, when init() has been called already.
bool DXFRecord::_isInit = false;

//! Set, when input file is binary DXF.
bool DXFRecord::_isBinary = false;

//! Set, when a record has been unget().
bool DXFRecord::_isUnget = false;

//!The stream DXFRecord is fetching data from.
std::istream *DXFRecord::_inStream = NULL;

//!Current line number in file
UInt32 DXFRecord::_inStreamLineNumber = 0;

//! Used to map group codes to vaue types when a group is read by
//! DXFRecord::fetch(). 
std::vector<DXFValueType> DXFRecord::_valueTypeMap;

//! Internal DXFRecord double buffer
DXFRecord DXFRecord::_records[2];

//! Index to the current one of the DXFRecord::_records
UInt8  DXFRecord::_curRecord = 0;

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/*! Read one line from file. Hides the different line end codes of UNIX,
 *  Windows or Mac.
 *  Returns the control characters defining the line end in \c lineend if this
 *  is not NULL.
 *  \todo
 *  Checks line by line thus might misinterpret files which look like having
 *  mixed line end encodings. Allows a 4th format (the two DOS control
 *  characters reversed). Only chance: set an delimit "mini char array" which
 *  must be matched *exactly* at the end of *each* line
 */

std::istream &DXFRecord::getLine(std::istream  &is, 
                                 std::string   &str, 
                                 Char8        **lineend)
{
    str.erase();
      
    UInt64 extracted = 0;
    UInt64 n = str.max_size();

    bool testDelim = false;
      
    const Char8 DelimCR = '\r';
    const Char8 DelimLF = '\n';
    Char8 c1, c2 = 0;

    c1 = is.get();
    testDelim = (c1 == DelimCR || c1 == DelimLF);
    // read chars until a delim char
    while(extracted <= n && is.good() && !testDelim)
    {   
        str += Char8(c1);
        ++extracted;
        c1 = is.get();
        testDelim = (c1 == DelimCR || c1 == DelimLF);
    }
    // eat up a probable second delim character (DOS), which must be different
    // from the 1st one read!
    if(is.good() && testDelim)
    {
        c2 = is.get();
        testDelim = ((c2 == DelimLF || c2 == DelimCR) && c2 != c1);     
        if(is.good() && !testDelim)
        {
            // last char read belongs to next line, put it back
            is.putback(c2);
            c2 = '\0';
        }
    }

    if(lineend)
    {
        (*lineend)[0]= c1;
        (*lineend)[1]= c2;      
    }
    
//  FDEBUG(("DXFRecord::getLine(): got line %s\n", str.c_str()));
    
    return is;
}

/*! Return the current group code.
 */

Int32 DXFRecord::getGroupCode(void)
{
    return _records[_curRecord]._groupCode;
}

/*! Return the value of the current group as string.
 */

const std::string &DXFRecord::getValueStr(void)
{
    return _records[_curRecord]._valueStr;
}

/*! Return the float value of the current group as long integer. Only valid
 *  when getValueType() corresponds.
 */

Int64 DXFRecord::getValueInt(void)
{
    return _records[_curRecord]._valueInt;
}

/*! Return the float value of the current group as double. Only valid when
 *  getValueType() corresponds.
 */

Real64 DXFRecord::getValueDbl(void)
{
    return _records[_curRecord]._valueDbl;
}

/*! Return type of the value of the current group.
 */

DXFValueType DXFRecord::getValueType(void)
{
    return _records[_curRecord]._valueType;
}

/*! Return line number of group code of this record..
 */

UInt32 DXFRecord::getLineNumber(void)
{
    return _records[_curRecord]._lineNumber;
}

/*! Fetch data from the current input stream.
 *
 *  @todo
 *    Distinguish between an error in the stream and an EOF? Make this method
 *    absolutely robust! _isBinary wird noch nicht ausgewertet! Syntaxfehler im
 *    Dateiformat werden noch nicht ausgewertet! Werden Integer im DXF immer
 *    dezimal "%d" abgespeichert? Korrespondieren die size flags "h,l" nach "%"
 *    mit Int8 und Int32? oder muss hier Betriebssystem/Compilerspezifisch
 *    unterschieden werden? evtl. Loesung: alles Int64?
 *
 */

Int8 DXFRecord::fetch(void)
{
    if(_isUnget)
    {
        _isUnget = false;
        // CAUTION: a DXFRecord with _valueType == DXFTypeEOF may have an
        // invalid _groupCode, thus you must NOT access _valueTypeMap for it!
        // TODO testen auf _valueType < 0 oder steht immer nur DXFTypeEOF drin?
        if(_records[_curRecord]._valueType < 0)
            return DXFTypeEOF;
        else
            return _records[_curRecord]._groupCode;
    }

    _curRecord ^= 1;  // toggles between 0 and 1
    _records[_curRecord]._lineNumber = _inStreamLineNumber;
    if(_inStream->eof())
    {
        _records[_curRecord]._valueType = DXFTypeEOF;
        return DXFTypeEOF;
    }

    // Get Group Code (_valueStr is just used intermediately)
    getLine(*_inStream, _records[_curRecord]._valueStr/*, DELIM*/);
    ++ _inStreamLineNumber;
    
    if(_inStreamLineNumber < 5)
    {
        std::cout << _inStreamLineNumber 
                  << ":'" 
                  << _records[_curRecord]._valueStr 
                  << "'" 
                  << std::endl;
    }
    

    if(!_inStream->good())
    {
        _records[_curRecord]._valueType = DXFTypeEOF;
        return DXFTypeEOF;
    }
    
    _records[_curRecord]._groupCode = 
        atoi(_records[_curRecord]._valueStr.c_str());

    if(_records[_curRecord]._groupCode < 0 ||
       _records[_curRecord]._groupCode > MAXGROUPCODE)
    {
        // Eat up next line of broken group record
        getLine(*_inStream, _records[_curRecord]._valueStr/*, DELIM*/);
        ++ _inStreamLineNumber;
        if(!_inStream->good())
        {
            return DXFTypeEOF;
        }
        else
        {   
            // TODO: unterscheiden von DXFTypeEOF?!?!?
            _records[_curRecord]._valueType = DXFTypeUnknown;
            return DXFTypeEOF;
        }
    }

    // Get Group Value
    getLine(*_inStream, _records[_curRecord]._valueStr/*, DELIM*/);
    ++ _inStreamLineNumber;

    _records[_curRecord]._valueType = 
        _valueTypeMap[_records[_curRecord]._groupCode];
    switch(_records[_curRecord]._valueType)
    {
        case DXFTypeUnknown:  //TODO
        case DXFTypeIgnore:  //TODO
        case DXFTypeEOF:   //TODO
            // _records[_curRecord]._valueStr already holds the value
            break;
        case DXFTypeInt8:  //TODO 
        case DXFTypeInt16:
        case DXFTypeInt32:
            _records[_curRecord]._valueInt = 
                atoi(_records[_curRecord]._valueStr.c_str());
            break;
        case DXFTypeFloat:  //TODO
        case DXFTypeDouble:
            _records[_curRecord]._valueDbl = 
                atof(_records[_curRecord]._valueStr.c_str());
            break;
        case DXFTypeString:  //TODO
        case DXFTypeHex:  //TODO  hier koennte _valueStr bereits in eine binaere
                   //Representation gewandelt werden!
            // _records[_curRecord]._valueStr already holds the value
            // _records[_curRecord]._valueHex = _records[_curRecord]._valueStr;
            break;
    }
//  FDEBUG(("DXFRecord::fetch() [%d] %d|%s\n",
//           _records[_curRecord]._lineNumber,
//           _records[_curRecord]._groupCode,
//           _records[_curRecord]._valueStr.c_str()));
    return  _records[_curRecord]._groupCode;
}

/*! Put current record back for to be retrieved again by the next call of
 * fetch(). Only one record can put back until the next fetch().
 */

bool DXFRecord::unget(void)
{
    if(_isUnget)
    {
        FFATAL(("BUG!: DXFRecord::unget() called too often!\n"));
        return false;
    }
    _isUnget = true;
    
    return true;
}

/*! Set the instream. The stream has to be opened already, but no data should
 * have been read (TODO: rewind?). It's immediately checked, whether the stream
 * contains binary DXF or not.
 *
 *  @todo Test binary DXF!
 */

bool DXFRecord::setStream(std::istream &is)
{
    FDEBUG(("DXFRecord::setStream()\n"));
    if(! _isInit)
        init();

    bool ok = false;
    _inStream = &is;
    _inStreamLineNumber = 0;
    
    ok = _inStream->good();
#if 1
    if(ok)
    {
        std::string line;
        Char8 lineend[2]="";
        Char8 *l = lineend;
        getLine(*_inStream, line, &l);
        ++ _inStreamLineNumber;

        // Check for binary!
        // Magic line for binary DXF: "AutoCAD Binary DXF<CR><LF><SUB><NULL>"
        //                                               \015\012\032 \000
        if(line == "AutoCAD Binary DXF")
        {
            _isBinary=true;
            FDEBUG(("Binary DXF File\n"));
            if(_inStream->good())
            {
                getLine(*_inStream, line);  // just eat up line after <CR><LF>
                ++ _inStreamLineNumber;
            }
        }
        else    
        {
            _isBinary = false;
            FDEBUG(("ASCII DXF File\n"));
            if(_inStream->good())
            {           
//              _inStream->seekg(0);  // rewind to the beginnig
                // not implemented in zipstream, this we have to do it by
                // hand...
                if(lineend[1] != '\0')
                    _inStream->putback(lineend[1]);             
                _inStream->putback(lineend[0]);
                
                for(int i = line.length()-1; i >= 0; -- i)
                {
                    std::cout << "'" << line[i] << "' ";
                    _inStream->putback(line[i]);
                }
                std::cout << "--- len: " << line.length() << std::endl;
                _inStreamLineNumber = 0;
            }
        }
    }
#endif
    return _inStream->good();
}

/*=============================== PROTECTED ===============================*/

/*! Initialize global data structures.
 *  The following internal data is initialized:
 *  - DXFRecord::_valueTypeMap
 */

void DXFRecord::init(void)
{
    int i;

    if(_isInit)
    {
//      FWARNING(("Class DXFRecord already initialized, ignoring!")); // TODO
        return;
    }
    
    //--- initialize value type map ----------------------------
    _valueTypeMap.resize(MAXGROUPCODE + 1);  // +1 for the '0' ;-)
    
    //-------------------------------------------------------------------------
    //   0 -   9 : String
    for(i = 0; i <= 9; ++i)
    {
        _valueTypeMap[i] = DXFTypeString;
    }
    //-------------------------------------------------------------------------
    //  10 -  59 : Double precision 3D point
    for(i = 10; i <= 59; ++i)
    {
        _valueTypeMap[i] = DXFTypeDouble;
    }
    //-------------------------------------------------------------------------
    //  60 -  79 : 16-bit integer value
    for(i = 60; i <= 79; ++i)
    {
        _valueTypeMap[i] = DXFTypeInt16;
    }
    //-------------------------------------------------------------------------
    //  90 -  99 : 32-bit integer value  
    for(i = 90; i <= 99; ++i)
    {
        _valueTypeMap[i] = DXFTypeInt32;
    }
    //-------------------------------------------------------------------------
    //  100, 102 : String (255-character maximum; less for Unicode strings)
    //  105      :  String representing hexadecimal (hex) handle value
    {
        _valueTypeMap[100] = DXFTypeString;
        _valueTypeMap[102] = DXFTypeString;
        _valueTypeMap[105] = DXFTypeHex;
    }
    //-------------------------------------------------------------------------
    // 140 - 147 : Double precision scalar floating-point value  
    for(i = 140; i <= 147; ++i)
    {
        _valueTypeMap[i] = DXFTypeDouble;
    }
    //-------------------------------------------------------------------------
    // 170 - 175 : 16-bit integer value  
    for(i = 170; i <= 175; ++i)
    {
        _valueTypeMap[i] = DXFTypeInt16;
    }
    //-------------------------------------------------------------------------
    // 176 - 179 : unknown type (DIMSTYLE codes, not documented in "Group Code
    //                           Value Types") TODO
    for(i = 176; i <= 179; ++i)
    {
        _valueTypeMap[i] = DXFTypeUnknown;
    }
    //-------------------------------------------------------------------------
    // 210 - 239 : Float (not documented in "Group Code Value Types")
    for(i = 210; i <= 239; ++i)
    {
        _valueTypeMap[i] = DXFTypeFloat;
    }
    //-------------------------------------------------------------------------
    // 270 - 279 : unknown type (DIMSTYLE codes, not documented in "Group Code
    //                           Value Types") TODO
    for(i = 270; i <= 279; ++i)
    {
        _valueTypeMap[i] = DXFTypeUnknown;
    }
    //-------------------------------------------------------------------------
    // 280 - 289 : 8-bit integer value  
    for(i = 280; i <= 289; ++i)
    {
        _valueTypeMap[i] = DXFTypeInt8;
    }
    //-------------------------------------------------------------------------
    //  300 - 309 : Arbitrary text string
    for(i = 300; i <= 309; ++i)
    {
        _valueTypeMap[i] = DXFTypeString;
    }
    //-------------------------------------------------------------------------
    //  310 - 319 : String representing hex value of binary chunk
    for(i = 310; i <= 319; ++i)
    {
        _valueTypeMap[i] = DXFTypeHex;
    }
    //-------------------------------------------------------------------------
    //  320 - 329 : String representing hex handle value
    for(i = 320; i <= 329; ++i)
    {
        _valueTypeMap[i] = DXFTypeHex;
    }
    //-------------------------------------------------------------------------
    //  330 - 369 : String representing hex object IDs
    for(i = 330; i <= 369; ++i)
    {
        _valueTypeMap[i] = DXFTypeHex;
    }
    //-------------------------------------------------------------------------
    // 370 - 379 : 8-bit integer value  
    for(i = 370; i <= 379; ++i)
    {
        _valueTypeMap[i] = DXFTypeInt8;
    }
    //-------------------------------------------------------------------------
    // 380 - 389 : 8-bit integer value  
    for(i = 380; i <= 389; ++i)
    {
        _valueTypeMap[i] = DXFTypeInt8;
    }
    //-------------------------------------------------------------------------
    //  390 - 399 : String representing hex object IDs
    for(i = 390; i <= 399; ++i)
    {
        _valueTypeMap[i] = DXFTypeHex;
    }
    //-------------------------------------------------------------------------
    // 400 - 409 : 16-bit integer value  
    for(i = 400; i <= 409; ++i)
    {
        _valueTypeMap[i] = DXFTypeInt16;
    }
    //-------------------------------------------------------------------------
    //  410 - 419 : String
    for(i = 410; i <= 419; ++i)
    {
        _valueTypeMap[i] = DXFTypeString;
    }
    //-------------------------------------------------------------------------
    //       999 : Comment (string)
    {
        _valueTypeMap[999] = DXFTypeIgnore;
    }
    //-------------------------------------------------------------------------
    // 1000-1009 : String
    for(i = 1000; i <= 1009; ++i)
    {
        _valueTypeMap[i] = DXFTypeString;
    }
    //-------------------------------------------------------------------------
    // 1010-1059 : Floating-point value
    for(i = 1010; i <= 1059; ++i)
    {
        _valueTypeMap[i] = DXFTypeFloat;
    }

    //-------------------------------------------------------------------------
    // 1060-1070 : 16-bit integer value
    for(i = 1060; i <= 1070; ++i)
    {
        _valueTypeMap[i] = DXFTypeInt16;
    }
    //-------------------------------------------------------------------------
    // 1071      : 32-bit integer value
    {
        _valueTypeMap[1071] = DXFTypeInt32;
    }


    //-------------------------------------------------------------------------
    _isInit = true;
} // init()

/*================================ PRIVATE ================================*/


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Member initialization.
 */

DXFRecord::DXFRecord(void) :
    _groupCode (-1            ),
    _valueType (DXFTypeUnknown),
    _valueStr  (""            ),
    _valueInt  (0             ),
    _valueDbl  (0.0           ),
    _lineNumber(0             )
{
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */

DXFRecord::~DXFRecord(void)
{
}


