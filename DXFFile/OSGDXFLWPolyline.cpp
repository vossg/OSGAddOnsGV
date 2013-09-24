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

#include "OSGDXFLWPolyline.h"

#include "OSGFieldContainer.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFLWPolyline
    \ingroup GrpSystemFileIO

  Parses and evaluates the file section between the groups (0,LWPolyline) and
  (0,SEQEND). Exactly speaking any group with code 0 besides (0,VERTEX) ends
  the LWPolyline.

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
DXFLWPolyline *DXFLWPolyline::_the = new DXFLWPolyline();

/* \var VARTYPE DXFLWPolyline::_VARNAME
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


/*! Evaluate records for LWPolyline entities with the following group codes:
 *  - 66 -- Obsolete, ignore if present
 *  - 10, 20 -- always 0
 *  - 30 -- LWPolyline's elevation (in OCS when 2D, WCS when 3D)
 */
DXFResult DXFLWPolyline::evalRecord(void)
{
     DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 10:
			_curVertice[0] = DXFRecord::getValueDbl();
			break;
        case 20:
			_curVertice[1] = DXFRecord::getValueDbl();
			_plyVertices.push_back(_curVertice);
            break;
        case 30:
            _elevation = DXFRecord::getValueDbl();
            break;
        case 66:    // obsolete, ignore if present
            break;
		case 90:
			_numVertices = DXFRecord::getValueInt();
			break;
        default:
            state = DXFStateUnknown;
    }
    if(state == DXFStateUnknown)
        state = Inherited::evalRecord();
    return state;
}

/*!  \todo
 *  check and make sure not to have bad memory leaks here!
 */
DXFResult DXFLWPolyline::beginEntity(void)
{   
     DXFResult state;
    state = Inherited::beginEntity();

    _elevation = 0.0;
	_numVertices = 0;
	_curVertice.setNull();

    beginGeometry();
    
    return state;
}

/*! Create OpenSG LWPolyline or surface geometry from VERTEX data. 
 *
 *  If the LWPolyline entity represents a LWPolyline (not a mesh) and there is a
 *  Geometry with the same color and line settings in the layer, the LWPolyline
 *  will be added to it's core as one GL_LINE_STRIP primitive whereas meshes
 *  always are created as a new Geometry Node (Done in
 *  DXFEntitiesEntry::flushGeometry)
 *
 *  \todo
 *  color and line settings are not checked yet)
 */

DXFResult DXFLWPolyline::endEntity(void)
{
     if(_flags & 1) // 1 for closed
    {
		_pointsP->resize(_plyVertices.size());
		for(int i=0;i<_plyVertices.size();i++)
		{
			_pointsP->setValue(_plyVertices.at(i), i);
		}
		_faceTypeP->push_back(GL_LINE_STRIP);
		_faceLengthP->push_back(_numVertices);
		_plyVertices.clear();
		flushGeometry(true); //only line
		endGeometry();
	}
    else  if(_flags & 128)// 128 for plinegen, currently ignore, do the same as 1
    {
        _pointsP->resize(_plyVertices.size());
		for(int i=0;i<_plyVertices.size();i++)
		{
			_pointsP->setValue(_plyVertices.at(i), i);
		}
		_faceTypeP->push_back(GL_LINE_STRIP);
		_faceLengthP->push_back(_numVertices);
		_plyVertices.clear();
        flushGeometry(true); //only line
		endGeometry();
    }
	else // 0
	{
	}

        
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add ENTITY:LWPolyline entity type to DXF hierarchy as child of
 *  SECTION:ENTITIES (see DXFEntities) and BLOCKSENTRY:BLOCK (see DXFBlock).
 */
DXFLWPolyline::DXFLWPolyline(void) :
    Inherited(),
    _elevation(0.0)
{
    _entityTypeName    = "LWPOLYLINE";

    registerToParentEntityType("SECTION:ENTITIES");
    registerToParentEntityType("BLOCKSENTRY:BLOCK");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFLWPolyline::~DXFLWPolyline(void)
{
}

