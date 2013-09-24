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

#include "OSGDXFSpline.h"
#include "OSGDXFHeader.h"
#include "OSGFieldContainer.h"
#include "OSGBaseFunctions.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFSpline
    \ingroup GrpSystemFileIO

  Parses and evaluates the file section between the groups (0,POLYLINE) and
  (0,SEQEND). Exactly speaking any group with code 0 besides (0,VERTEX) ends
  the polyline.

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
DXFSpline *DXFSpline::_the = new DXFSpline();

/* \var VARTYPE DXFSpline::_VARNAME
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


/*! Evaluate records for POLYLINE entities with the following group codes:
 *  - 66 -- Obsolete, ignore if present
 *  - 10, 20 -- always 0
 *  - 30 -- Polyline's elevation (in OCS when 2D, WCS when 3D)
 */
DXFResult DXFSpline::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 10:
			_centerPoint[0]= DXFRecord::getValueDbl();
			break;
        case 20:
			_centerPoint[1]= DXFRecord::getValueDbl();
            break;
        case 30:
            _elevation =     DXFRecord::getValueDbl();
            break;
		case 40:
			_radius =        DXFRecord::getValueDbl();
			break;
		case 50:
			_startAngle =    DXFRecord::getValueDbl();
			break;
		case 51:
			_endAngle =      DXFRecord::getValueDbl();
			break;
        case 66:    // obsolete, ignore if present
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
DXFResult DXFSpline::beginEntity(void)
{   
    DXFResult state;
    state = Inherited::beginEntity();

    _centerPoint.setNull();
    beginGeometry();
    
    return state;
}

/*! Create OpenSG polyline or surface geometry from VERTEX data. 
 *
 *  If the POLYLINE entity represents a polyline (not a mesh) and there is a
 *  Geometry with the same color and line settings in the layer, the polyline
 *  will be added to it's core as one GL_LINE_STRIP primitive whereas meshes
 *  always are created as a new Geometry Node (Done in
 *  DXFEntitiesEntry::flushGeometry)
 *
 *  \todo
 *  color and line settings are not checked yet)
 */

DXFResult DXFSpline::endEntity(void)
{
	float angleBase = DXFHeader::getAngBase();
	int dir = DXFHeader::getAngDir();
	double x, y, z;
	if(dir) //clockwise
	{
		_startAngle= 360 - _startAngle;
		_endAngle  =  360 - _endAngle;
	}
	_startAngle += 360;
	_endAngle += 360;
	while(_startAngle > _endAngle)
	{
		_endAngle += 360;
	}
	x= _centerPoint[0] + _radius * cos(osgDegree2Rad(_startAngle));
	y= _centerPoint[1] + _radius * sin(osgDegree2Rad(_startAngle));
	z = _centerPoint[2];
	_pointsP->push_back(OSG::Pnt3f(x,y,z));
	int div = 10;
	int num = abs(_endAngle - _startAngle) / div;

	for(int i=1;i<num-1;i++)
	{
		x= _centerPoint[0] + _radius * cos(osgDegree2Rad(_startAngle + div * i));
		y= _centerPoint[1] + _radius * sin(osgDegree2Rad(_startAngle + div * i));
		z = _centerPoint[2];
		_pointsP->push_back(OSG::Pnt3f(x,y,z));
	}
	x= _centerPoint[0] + _radius * cos(osgDegree2Rad(_endAngle));
	y= _centerPoint[1] + _radius * sin(osgDegree2Rad(_endAngle));
	z = _centerPoint[2];
	_pointsP->push_back(OSG::Pnt3f(x,y,z));
	_faceTypeP->clear();
	_faceLengthP->clear();
    _faceTypeP  ->push_back(GL_LINE_STRIP);
    _faceLengthP->push_back(num);

    flushGeometry(false);
    endGeometry();
        
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add ENTITY:POLYLINE entity type to DXF hierarchy as child of
 *  SECTION:ENTITIES (see DXFEntities) and BLOCKSENTRY:BLOCK (see DXFBlock).
 */
DXFSpline::DXFSpline(void) :
    Inherited(),
    _elevation(0.0),
	_radius(0.0),
	_startAngle(0),
	_endAngle(0),
	_centerPoint(0.0, 0.0, 0.0)
{
    _entityTypeName    = "SPLINE";

    registerToParentEntityType("SECTION:ENTITIES");
    registerToParentEntityType("BLOCKSENTRY:BLOCK");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFSpline::~DXFSpline(void)
{
}

