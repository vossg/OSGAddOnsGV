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

#include "OSGDXFLine.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFLine
    \ingroup GrpSystemFileIO

  Parses and evaluates the file subsection inside a LINE between the
  group (0,LINE) and any other group with group code 0.

 */

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

/*! \enum DXFLine::ENUMNAME
  
 */

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/*! Pointer to singleton of this class. There is exactly one instance for each
 * derived class which can be instantiated.
 */
DXFLine *DXFLine::_the = new DXFLine();

/* \var VARTYPE DXFLine::_VARNAME
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

/*! Evalute records for LINE with the following group codes:
 *  - 10, 20, 30 -- Start point (in WCS)
 *  - 11, 21, 31 -- End point (in WCS)
 */
DXFResult DXFLine::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 10:
            _startPoint[0] = DXFRecord::getValueDbl();
            break;
        case 20:
            _startPoint[1] = DXFRecord::getValueDbl();
            break;
        case 30:
            _startPoint[2] = DXFRecord::getValueDbl();
            break;
        case 11:
            _endPoint[0] = DXFRecord::getValueDbl();
            break;
        case 21:
            _endPoint[1] = DXFRecord::getValueDbl();
            break;
        case 31:
            _endPoint[2] = DXFRecord::getValueDbl();
            break;
        default:
            state = DXFStateUnknown;
    }
    if(state == DXFStateUnknown)
        state = Inherited::evalRecord(); // don't remove this!
    return state;
}

/*! Clear internal data.
 */
DXFResult DXFLine::beginEntity(void)
{
    DXFResult state;
    state = Inherited::beginEntity();
    
    // clear own data here
    _startPoint.setNull();
    _endPoint.setNull();

    beginGeometry();

    return state;
}

/*! Create OpenSG line from fetched data.
 */
DXFResult DXFLine::endEntity(void)
{
    _pointsP    ->push_back(_startPoint);
    _pointsP    ->push_back(_endPoint); 
    _faceTypeP  ->push_back(GL_LINE);
    _faceLengthP->push_back(2);

    flushGeometry(false);
    endGeometry();
    
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add ENTITY:LINE entity type to DXF hierarchy as child of SECTION:ENTITIES (see
 *  DXFEntities) and BLOCKSENTRY:BLOCK (see DXFBlock).
 */
DXFLine::DXFLine(void) :
    Inherited(),
    _startPoint(0.0, 0.0, 0.0),
    _endPoint(0.0, 0.0, 0.0)
{
    _entityTypeName    = "LINE";

    registerToParentEntityType("SECTION:ENTITIES");
    registerToParentEntityType("BLOCKSENTRY:BLOCK");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFLine::~DXFLine(void)
{
}
