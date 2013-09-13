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

#ifndef _OSGDXFRECORD_H_
#define _OSGDXFRECORD_H_

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <vector>
#include <iostream>

#include <OSGBaseTypes.h>
#include <OSGDXFFileDef.h>

OSG_BEGIN_NAMESPACE

//---------------------------------------------------------------------------
//  Forward References
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Types
//---------------------------------------------------------------------------

/*! Type identifiers for the interpretation of Value union data.
 * \todo 
 * distinction between DXFTypeUnknown and DXFTypeIgnore?!?!? Currently those
 * codes are not used!
 */
enum DXFValueType  // Values must be between -128 and 127 since they may be
                   // returned as Int8
{
    DXFTypeUnknown = -3,  //!< Unknown type, may be an error 
    DXFTypeIgnore  = -2,  //!< Unknown type, but can be ignored
    DXFTypeEOF     = -1,  //!< End of file
    DXFTypeInt8    =  0,
    DXFTypeInt16,
    DXFTypeInt32,
    DXFTypeFloat,
    DXFTypeDouble,
    DXFTypeString,
    DXFTypeHex
};

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

/*! \brief Represents one group/record in a DXF file consisting of group code
    and value.
 */

class OSG_DXFFILE_DLLMAPPING DXFRecord
{
    /*==========================  PUBLIC  =================================*/

  public:
    
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    DXFRecord(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    Destructor                                */
    /*! \{                                                                 */

    virtual ~DXFRecord(void);
  
    /*---------------------------------------------------------------------*/
    /*! \name                    Helper                                    */
    /*! \{                                                                 */

    static std::istream &getLine(std::istream &is, 
                                 std::string  &str,
                                 Char8       **lineend = NULL);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Get                                     */
    /*! \{                                                                 */
    
    static  const Char8        *getClassname(void) { return "DXFRecord"; }

    static        Int32         getGroupCode(void);
 
    static        DXFValueType  getValueType(void);

    static const std::string   &getValueStr(void);
    static       Int64          getValueInt(void);
    static       Real64         getValueDbl(void);

    static       UInt32         getLineNumber(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Set                                     */
    /*! \{                                                                 */

    static Int8 fetch(void);      //!< Get next record from file or the one
                                  //!  which has been unget() just before.
    static bool unget(void);      //!< Put current record back; only one record
                                  //!  can be putback before the next
                                  //!  fetch().

    static bool setStream(std::istream &is);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Initialization                             */
    /*! \{                                                                 */

    static void init(void);   // called by openStream()

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Member                                  */
    /*! \{                                                                 */

    Int32       _groupCode; //!< DXF group code for this record

    DXFValueType _valueType; //!< Type of value of this record.

    std::string _valueStr;  //!< String representation of group value.
    Int64       _valueInt;  //!< Integer group value, maybe invalid.
    Real64      _valueDbl;  //!< Float group value, maybe invald.

    UInt32      _lineNumber; //!< Line number of group code of this record.

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Member                                  */
    /*! \{                                                                 */

    static       bool                       _isInit;               
    static       bool                       _isBinary;             
    static       bool                       _isUnget;          

    static       std::istream              *_inStream;
    static       UInt32                     _inStreamLineNumber;

    //! Max. group code of AutoCAD 2000  
    static const int                         MAXGROUPCODE = 1071;

    static       std::vector<DXFValueType>  _valueTypeMap;
    
    static       DXFRecord                  _records[2];
    static       UInt8                      _curRecord;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    DXFRecord(const DXFRecord &source);
    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const DXFRecord &source);
};

OSG_END_NAMESPACE

#endif /* _OSGDXFRECORD_H_ */
