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

#include "OSGConfig.h"

#include "OSGDXFLtype.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFLtype
    \ingroup GrpSystemFileIO

  Parses and evaluates the file subsection inside a LTYPE table between the
  group (0,LTYPE) and any other group with group code 0.

 */

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/*! Pointer to singleton of this class. There is exactly one instance for each
 * derived class which can be instantiated.
 */
DXFLtype *DXFLtype::_the = new DXFLtype();

/* \var VARTYPE DXFLtype::_VARNAME
    variable documentation
 */

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/


/*! Evaluate records for LTYPE table entries with the following group codes:
 * - 3 -- Descriptive text for linetype
 * - 73 -- The number of linetype elements
 * - 40 -- Total pattern length
 * - 49 -- Dash, dot or space length (one entry per element)
 */
DXFResult DXFLtype::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 3:
            _desc = DXFRecord::getValueStr();           
            break;
        case 73:
            _nElements = DXFRecord::getValueInt();
            _elementLen.reserve(_nElements);
            break;
        case 40:
            _patternLen = DXFRecord::getValueDbl();
            break;
        case 49:
            _elementLen.push_back(DXFRecord::getValueDbl());
            break;
        default:
            state = DXFStateUnknown;
    }
    if(state == DXFStateUnknown)
        state = Inherited::evalRecord();
    return state;
}

/*! Clear internal data.
 */
DXFResult DXFLtype::beginEntity(void)
{
    DXFResult state;
    state = Inherited::beginEntity();

    _desc = "";
    _nElements = 0;
    _patternLen = 0;
    _elementLen.clear();

    return state;
}

/*! Create LineChunk from fetched data.
 *  The DXF line pattern description is mapped to a 16 bit stipple pattern
 *  suitable for OpenGL line stippling. This mapping only approximately
 *  resembles the DXF line pattern.
 *
 *  \todo 
 *  Currently it's not checked, whether the fetched data is consistent!
 */
DXFResult DXFLtype::endEntity(void)
{
    if(_linetypeMapP->find(_name) != _linetypeMapP->end())
    {
        FWARNING(("DXF Loader: before line %d: "
                  "LTYPE entity '%s' already exists. "
                  "Overwriting with new one!\n",
                  DXFRecord::getLineNumber(),
                  _name.c_str()
                 ));
    }

    // Do consistency checks
    Real32 checkedPatternLen = 0.0;
    for(std::vector<Real64>::iterator itr = _elementLen.begin();
        itr != _elementLen.end();
        ++ itr)
    {
        checkedPatternLen += osgAbs(*itr);
    }
    
    if(osgAbs(checkedPatternLen - _patternLen) > Eps)
    {
        FWARNING(("DXF Loader: before line %d: "
                  "LTYPE entity defines inconsistent pattern lenght!\n",
                  DXFRecord::getLineNumber()
                 ));
        // we go on, but will use consistent data: checkedPatternLen
    }

    if(SizeT(_nElements) != _elementLen.size())
    {
        FWARNING(("DXF Loader: before line %d: "
                  "LTYPE entity declares %d pattern elements but "
                  "provides %" PRISize "!\n",
                  DXFRecord::getLineNumber(),
                  _nElements,
                  _elementLen.size()
                 ));
        // we go on, but will use consistent data: _elementLen.size()
    }

    UInt16 pattern = 0;
    LineChunkUnrecPtr linetype = LineChunk::create();    
#if 0
    beginEditCP(linetype);
#endif
    {       
//      linetype->setWidth(1);
        if(_elementLen.size() != 0)
        {
            // Build OpenGL 16 bit stipple pattern: for each bit calculate, which
            // pattern element it belongs to and set it if the pattern element is
            // positive, otherwise leave it unset.
            Real32 s = checkedPatternLen/16.0;
            UInt8 j = 0;
            Real32 elementLenSum = osgAbs(_elementLen[j]);
            for(UInt8 i = 0;
                i < 16;
                ++ i)
            {
                if(elementLenSum < (0.5 + i) * s && j < _elementLen.size())
                {
                    ++ j;
                    elementLenSum += osgAbs( _elementLen[j] );
                }
                if( _elementLen[j] > 0.0)
                    pattern |= 1<<i;
            }
            linetype->setStippleRepeat(1);
            linetype->setStipplePattern(pattern);   
        }
        linetype->setSmooth(true);  //TODO: make this configurable by an option?
    }
#if 0
    endEditCP(linetype);
#endif
    
#if 1 // DEBUG TODO: raus?!?!
    std::string pstr("");
    for(UInt8 i=0; i<16; ++i)
        pstr += (pattern & (1<<i)) ? "1" : "0";
    FDEBUG(("DXFLtype::endEntity(): pattern '%s' = %s\n",
            _name.c_str(),
            pstr.c_str()));
#endif // DEBUG
    (*_linetypeMapP)[_name] = linetype;

    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add TABLEENTRY:LTYPE to DXF hierarchy as child of TABLE:LTYPE (See
 *  DXFTable<>).
 */
DXFLtype::DXFLtype(void) :
    Inherited(),
    _desc(),
    _nElements(0),
    _patternLen(0),
    _elementLen()
{
    _entityTypeName  = "LTYPE";

    registerToParentEntityType("TABLE:LTYPE");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFLtype::~DXFLtype(void)
{
}

