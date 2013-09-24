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

#include "OSGDXFVertex.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFVertex
    \ingroup GrpSystemFileIO

Parses and evaluates the file section between group (0,VERTEX) and any other
group with code 0.

 */

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/*! Pointer to singleton of this class.
 */
DXFVertex *DXFVertex::_the = new DXFVertex();

/*! \var const Int8 DXFVertex::PFACEVMAX 
  AutoCAD constant: maximum number of vertices per face.
 */

/*! \var Pnt3f DXFVertex::_locationPoint
  3D point coordinates
 */

/*! \var Int16 DXFVertex::_vertexIndex[PFACEVMAX]
  Indices for indexed faces (polymeshes); \c _vertexIndex[i]==-1 means
  unused. The indices are in DXF sematics: the first vertex has index 1 (not 0
  as in OpenSG!)
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


/*! Evaluate records for VERTEX entities with the following group codes:
 *  - 10, 20, 30 -- Location point (DXFVertex::_locationPoint)
 *  - 71, 72, 73, 74 -- Vertex indices (DXFVertex::_vertexIndex)
 */
DXFResult DXFVertex::evalRecord(void)  // only for leaves
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 10:
            _locationPoint[0] = DXFRecord::getValueDbl();
            break;
        case 20:
            _locationPoint[1] = DXFRecord::getValueDbl();
            break;
        case 30:
            _locationPoint[2] = DXFRecord::getValueDbl();
            break;
        case 71:
        case 72:
        case 73:
        case 74:
            _vertexIndex[DXFRecord::getGroupCode()-71] 
                = DXFRecord::getValueInt();
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
DXFResult DXFVertex::beginEntity(void)
{
    _locationPoint.setNull();
    for(Int8 i = 0; i <  PFACEVMAX; ++ i)
        _vertexIndex[i] = -1;
    return DXFStateContinue;
}

/*! Push either point coordinates or indices and corresponding face types to
 * GeoProperties.
 *
 *  \todo Perform integrity checks on the fetched data!
 *
 * \attention
 * In DXF counting (for indices) starts at 1, NOT at 0!
 */

DXFResult DXFVertex::endEntity(void)
{
    if ( _vertexIndex[0] != -1 )
    {
        Int32 faceType = 0;
        // VERTEX holds face indices, not coordinates
        // check, whether 3 or 4 vertices are combined as a face ( 1 ore 2 are
        // considered a bug!)
        // TODO: integrity check: groups 71,72,73 and optionally 74 read? Any
        // double indices?
#if 1   // currently only checked for 73/74
        if(_vertexIndex[PFACEVMAX-1] == _vertexIndex[PFACEVMAX-2])
            _vertexIndex[PFACEVMAX-1] = -1; // Skip double index
#else   // Simple algo for removing double indices, works only, when the doubles
        // follow each other sequentially. May be too time consuming though!
        for(int j=PFACEVMAX-1; j>0; --j)
            if(_vertexIndex[i] == _vertexIndex[i-1]
               _vertexIndex[i] = -1;
#endif


        int nIndices = 0;
        for(int i = 0; i < PFACEVMAX; ++i)
        {
            if(_vertexIndex[i] != -1)
            {
                //CAUTION: DXF starts counting at 1, OpenSG at 0, thus '- 1'!
                _pointIndicesP->push_back(_vertexIndex[i] - 1);
                ++ nIndices;
            }
        }
        switch(nIndices)
        {
            case 0:
                // Error in DXF File! TODO: Error message?
                break;
            case 1:
                // TODO: is this an error?
                faceType = GL_POINT;
                break;
            case 2:
                // TODO: is this an error?
                faceType = GL_LINE;
                break;
            case 3:
                faceType = GL_TRIANGLES;
                break;
            case 4:
                faceType = GL_QUADS;
                break;
            default:    // can't happen for current DXF file format
                faceType = GL_POLYGON;              
        }

        if(faceType == _uiCurrentFaceType)
        {
            _faceLengthP->editField().back() += nIndices;
        }
        else
        {
            _uiCurrentFaceType = faceType;
            _faceTypeP  ->push_back(faceType);
            _faceLengthP->push_back(nIndices);
        }
    }
    else
    {
        // TODO: integrity check: 3 coordinates read?
        _pointsP->push_back(Pnt3f(_locationPoint[0],
                                  _locationPoint[1],
                                  _locationPoint[2]));
        ++ _uiNumFacePoints;
    }
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add VERTEX entity to DXF hierarchy as child of ENTITY:POLYLINE
 * (see DXFPolyline).
 */
DXFVertex::DXFVertex(void) :
    Inherited(),
    _locationPoint(0.0, 0.0, 0.0)
{
    _entityTypeName = "VERTEX"; 

    registerToParentEntityType("ENTITY:POLYLINE");
    registerToParentEntityType("ENTITY:LWPOLYLINE");
    // TODO: are VERTEX entities used in any other entity than POLYLINE?
//  registerToParentEntityType("ENTITY:<OTHER>");

    for(Int8 i = 0; i <  PFACEVMAX; ++ i)
        _vertexIndex[i] = -1;
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFVertex::~DXFVertex(void)
{
}
