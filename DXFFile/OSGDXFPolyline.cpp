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

#include "OSGDXFPolyline.h"

#include "OSGFieldContainer.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFPolyline
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
DXFPolyline *DXFPolyline::_the = new DXFPolyline();

/* \var VARTYPE DXFPolyline::_VARNAME
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
DXFResult DXFPolyline::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 10:
        case 20:
            break;
        case 30:
            _elevation = DXFRecord::getValueDbl();
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
DXFResult DXFPolyline::beginEntity(void)
{   
    DXFResult state;
    state = Inherited::beginEntity();

    _elevation = 0.0;

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

DXFResult DXFPolyline::endEntity(void)
{
    if(!_flags || _flags & 8)
    {
        // 2D or 3D polyline
        if( osgAbs(_elevation) > Eps)
        {           
            if(!_flags)
            {
                // 2D polyline
                // TODO: elevation in OCS is missing
                FWARNING(("DXF Loader: DXFPolyline currently "
                          "implements '2D polyline' without 'elevation'!\n"));
            }
            else
            {
                // 3D polyline
                _pointsF = _pointsP->editFieldPtr();
                for(MFPnt3d::iterator itr = _pointsF->begin();
                    itr != _pointsF->end();
                    ++itr)
                {
                    (*itr)[2] += _elevation; //TODO: AutoCAD conformant?
                }
            }   
        }
        _faceTypeP  ->push_back(GL_LINE_STRIP);
        _faceLengthP->push_back(_uiNumFacePoints);

        flushGeometry(false);
    }
    else if(_flags & 16)    // implies doNewgeo == true, see above!
    {
        // 3D polygon mesh
        FWARNING(("DXF Loader: "
                  "DXFPolyline doesn't implement '3D polygon mesh' yet!\n"));
        //flushGeometry(true);
    }
    else if(_flags & 64)    // implies doNewgeo == true, see above!
    {
        // polyface mesh
        flushGeometry(true);
    }

    endGeometry();
        
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add ENTITY:POLYLINE entity type to DXF hierarchy as child of
 *  SECTION:ENTITIES (see DXFEntities) and BLOCKSENTRY:BLOCK (see DXFBlock).
 */
DXFPolyline::DXFPolyline(void) :
    Inherited(),
    _elevation(0.0)
{
    _entityTypeName    = "POLYLINE";

    registerToParentEntityType("SECTION:ENTITIES");
    registerToParentEntityType("BLOCKSENTRY:BLOCK");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFPolyline::~DXFPolyline(void)
{
}

