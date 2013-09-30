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

#include "OSGDXFEntitiesEntry.h"

#include "OSGGeometry.h"
#include "OSGNameAttachment.h"
#include "OSGGeoOptimization.h"
#include "OSGMaterialGroup.h"
#include "OSGSimpleMaterial.h"
//#include "OSGSimpleAttachments.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFEntitiesEntry
    \ingroup GrpSystemFileIO

  This class implements the evaluation of group codes common for all entries of
  the ENTITIES and BLOCKS sections.

 */

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

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

/*! Evaluate records common for all entities inside ENTITIES section
 *  with the following group codes:
 *  - 8 -- Layer name 
 *  - 6 -- Linetype name
 *  - 62 -- Color number
 *  - 48 -- Linetype scale
 *  - 39 -- Thickness
 *  - 210, 220, 230 -- Extrusion direction
 */
DXFResult DXFEntitiesEntry::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 8:
            _layerName = DXFRecord::getValueStr();
            break;
        case 6:
            _linetypeName = DXFRecord::getValueStr();
            break;
        case 62:
            _colorNumber = DXFRecord::getValueInt();
            break;
        case 48:
            _linetypeScale = DXFRecord::getValueDbl();
            break;
        case 39:
            _thickness = DXFRecord::getValueDbl();
            break;
        case 210:
            _extrusionDirection[0] = DXFRecord::getValueDbl();
            break;
        case 220:
            _extrusionDirection[1] = DXFRecord::getValueDbl();
            break;
        case 230:
            _extrusionDirection[2] = DXFRecord::getValueDbl();
            break;
		case 420:
			_trueColor = DXFRecord::getValueInt();
        default:
            state = DXFStateUnknown;
    }
    if(state == DXFStateUnknown)
        state = Inherited::evalRecord();
    return state;
}

DXFResult DXFEntitiesEntry::beginEntity(void)
{   
    DXFResult state;
    state = Inherited::beginEntity();

    _layerName = "";
    _linetypeName = "";
    _colorNumber = 256;
    _linetypeScale = 1.0;
    _thickness = 0.0;
    _extrusionDirection.setValues(0.0, 0.0, 1.0);

    return state;
}

Node *DXFEntitiesEntry::getParentNode(void)
{
    Node *parentNode = NULL;

    if(std::string("BLOCK") != _parent->getEntityTypeName())
    {
        StringToNodePtrMap::iterator itr = _layersMapP->find(_layerName);
        if(itr == _layersMapP->end())
        {
            // Reference to an unknown LAYER.
            FWARNING(("DXF Loader: in line %d: "
                      "LAYER '%s' not found for new %s\n",
                      DXFRecord::getLineNumber(),
                      _layerName.c_str(),
                      getEntityTypeName()
                     ));
#if 1 // DEBUG
            for( itr =  _layersMapP->begin();
                 itr != _layersMapP->end();
                 ++ itr);
            {
                FDEBUGx(("Known Layer: %s\n",
                        itr->first.c_str()));
            }
#endif
            // TODO: Alternatively add to default layer?
        }
        else
        {
            parentNode  = itr->second;
        }
    }
    else
    {
        parentNode = _parent->getNode();
    }

    return parentNode;
}

/*! Implements the arbitrary axis algorithm of AutoCAD.
 *
 *  See 
 *  http://www.autodesk.com/techpubs/autocad/acad2000/dxf/arbitrary_axis_algorithm_dxf_ab.htm
 */
Matrix DXFEntitiesEntry::arbitraryAxisAlgorithm(const Vec3f &normal)
{
    Vec3f aX, aY, aZ;
    const Vec3f WY(0.0, 1.0, 0.0);
    const Vec3f WZ(0.0, 0.0, 1.0);
    
    const Real32 Bound = 1.0/64.0;

    aZ = normal;
    aZ.normalize();
    
    if(osgAbs(aZ[0]) < Bound && osgAbs(aZ[1]) < Bound)
        aX = WY % aZ;
    else
        aX = WZ % aZ;   
    aX.normalize();

    aY = aZ % aX;
    aY.normalize();

    
    return Matrix(aX, aY, aZ);
}



/*! Set up GeoProperties for derived entity types which fetch geometry data.
 */

DXFResult DXFEntitiesEntry::beginGeometry(void)
{
    // prepare geometry properties 
    // TODO: moeglicherweise können diese Pointer gleich auf was gescheites
    // gesetzt werden: Geometrieknoten des Layers!

    _pointsP       = GeoPnt3dProperty ::create();
    _pointIndicesP = GeoUInt32Property::create();  // for meshes
    _faceLengthP   = GeoUInt32Property::create();
    _faceTypeP     = GeoUInt8Property ::create();

#if 0
    addRefCP(_pointsP);
    addRefCP(_pointIndicesP);   
    addRefCP(_faceLengthP);
    addRefCP(_faceTypeP);

    beginEditCP(_pointsP);
    beginEditCP(_pointIndicesP);
    beginEditCP(_faceLengthP);
    beginEditCP(_faceTypeP);
#endif

    _uiNumFacePoints = 0;
    _uiCurrentFaceType = -1;    // no current GL primitive

    return DXFStateContinue;
}


/*! Clear GeoProperties.
 */

DXFResult DXFEntitiesEntry::endGeometry(void)
{
    // The corresponding beginEditCPs are performed in beginEntity()    
#if 0
    endEditCP(_faceTypeP);
    endEditCP(_faceLengthP);
    endEditCP(_pointIndicesP);
    endEditCP(_pointsP);
#endif

    // Clean up: perform subRefCP() for all FCPtrs there was an addRefCP in
    // beginEntity() or this method before. Unused FCs will be destroyed then,
    // the others will survive!
#if 0
    subRefCP(_pointsP);
    subRefCP(_pointIndicesP);
    subRefCP(_faceTypeP);
    subRefCP(_faceLengthP);
#endif

    _pointsP       = NULL;
    _pointIndicesP = NULL;
    _faceTypeP     = NULL;
    _faceLengthP   = NULL;

    return DXFStateContinue;
}


/*! Creates new geometry node and core and sets the properties from the fetched
 *  data (this is enforced by setting \c forceNewNode to \c true) or appends
 *  fetched geometry data to existing Geometry Node. The distinction is made as
 *  follows: if there is a GeoIndices property set, a new Node is
 *  created. Otherwise the data is appended.
 *  
 */
void DXFEntitiesEntry::flushGeometry(bool forceNewNode)
{
    bool             doNewNode = true;
    NodeUnrecPtr     geoNode   = NULL;
    GeometryUnrecPtr geoCore   = NULL;

    NodeUnrecPtr     parentNode = getParentNode();

    if(! forceNewNode)
    {
        for(MFUnrecChildNodePtr::const_iterator itr = 
                parentNode->getMFChildren()->begin();
            itr != parentNode->getMFChildren()->end();
            ++ itr)
        {
            geoNode = *itr;  // can't be NullFC (see Node::addChild())
            geoCore = dynamic_cast<Geometry *>(geoNode->getCore());
            if(geoCore == NULL)
                continue;
                
            // Look for a node with non indexed geometry TODO: here goes the
            // additional check for matching line/color attibutes
            if(geoCore->getIndices() == NULL)
            {
                doNewNode = false;
                break;
            }
        }
    }

    if(doNewNode)
    {
        geoNode = Node::create();
        geoCore = Geometry::create();
    
#if 0
        beginEditCP(geoNode);
#endif
        {
            geoNode->setCore(geoCore);
            setName(geoNode, _entityTypeName +"="+ _layerName +"-"+ _handle);
            FDEBUGx(("DXFEntitiesEntry::flushGeometry(): "
                     "%s attached name '%s'\n",
                     getEntityTypeName(),
                     getName(geoNode)
                    ));
        }
#if 0
        endEditCP(geoNode);
        // put the geometry core together
        beginEditCP(geoCore);
#endif
        {
            geoCore->setPositions(_pointsP);
            geoCore->setLengths  (_faceLengthP);
            geoCore->setTypes    (_faceTypeP);
			StringToNodePtrMap::iterator itr = _layersMapP->find(this->_layerName);
			if(itr!= _layersMapP->end())
			{
				OSG::NodeRefPtr layerNodePtr = itr->second;
				OSG::GroupRefPtr mgrp = dynamic_cast<OSG::Group*>(layerNodePtr->getCore());
				OSG::SimpleMaterialRefPtr mat = dynamic_cast<
                          OSG::SimpleMaterial*>(
                              mgrp->findAttachment(
                                  OSG::SimpleMaterial::getClassType()));

#if 0
				OSG::MaterialGroupRefPtr mgrp = dynamic_cast<OSG::MaterialGroup*>(layerNodePtr->getCore());
				OSG::SimpleMaterialRefPtr mat = dynamic_cast<OSG::SimpleMaterial*>(mgrp->getMaterial());
#endif

				if(this->_trueColor >=0)
				{	
                    OSG::SimpleMaterialRefPtr newMat = OSG::SimpleMaterial::create();

                    OSG::Color3f newColor;
					Int32 b = (this->_trueColor & 0xff);
					newColor[2] = (float(b) / float(256.0));
					Int32 g = (this->_trueColor >> 8) & 0xff;
					newColor[1] = (float(g) / float(256.0));
					Int32 r = (this->_trueColor >> 16) & 0xff;
					newColor[0] = (float(r)/ float(256.0));
					newMat->setDiffuse(newColor);
					newMat->setLit(false);
					OSG::setName(newMat, "testMat");

                    geoCore->setMaterial(newMat);
				}
				else
				{
//					newMat->setDiffuse(mat->getDiffuse());
                    geoCore->setMaterial(mat);
				}
			}
            if(_pointIndicesP->size() > 0)
                geoCore->setIndices(_pointIndicesP);    
        }
#if 0
        endEditCP(geoCore);

        beginEditCP(parentNode, Node::ChildrenFieldMask);
#endif
        {
            parentNode->addChild(geoNode);
        }
#if 0
        endEditCP(parentNode, Node::ChildrenFieldMask); 
#endif
    }
    else
    {
        // Append new vertex data to existing geometry core
        // Here we don't have indexed geometry, thus we only have to
        // copy the positions, lengths and types
        
        GeoPnt3dProperty *pointsP =  
            dynamic_cast<GeoPnt3dProperty *>(geoCore->getPositions());

        MFPnt3d *pointsF =  pointsP->editFieldPtr();
        _pointsF         = _pointsP->editFieldPtr();
        
        GeoUInt32Property *faceLengthP =  
            dynamic_cast<GeoUInt32Property *>(geoCore->getLengths());
        MFUInt32 *faceLengthF =  faceLengthP->editFieldPtr();
        _faceLengthF          = _faceLengthP->editFieldPtr();

        GeoUInt8Property *faceTypeP =  
            dynamic_cast<GeoUInt8Property *>(geoCore->getTypes());
        MFUInt8 *faceTypeF =  faceTypeP->editFieldPtr();
        _faceTypeF         = _faceTypeP->editFieldPtr();
        
        
#if 0
        beginEditCP(geoCore);
#endif
        {
#if 1 // TODO: remove when OSGMField.h/inl defines a range insert
            for(MFPnt3d::iterator itr = _pointsF->begin();
                itr != _pointsF->end();
                ++itr)
                pointsF->push_back(*itr);
        
            for(MFUInt32::iterator itr = _faceLengthF->begin();
                itr != _faceLengthF->end();
                ++itr)
                faceLengthF->push_back(*itr);
            
            for(MFUInt8::iterator itr = _faceTypeF->begin();
                itr != _faceTypeF->end();
                ++itr)
                faceTypeF->push_back(*itr);
            
#else // TODO: use, when OSGMField.h/inl defines a range insert
            pointsF->insert(pointsF->end(),
                            _pointsF->begin(), 
                            _pointsF->end());
			colorsF->insert(colorsF->end(),
                            _colorsF->begin(), 
                            _colorsF->end());
            faceLengthF->insert(faceLengthF->end(),
                                _faceLengthF->begin(),
                                _faceLengthF->end());
            faceTypeF->insert(faceTypeF->end(),
                              _faceTypeF->begin(),
                              _faceTypeF->end());
#endif
        }
#if 0
        endEditCP(geoCore);
#endif
    }
	/*makeSingleIndexed(geoCore);
	makeIndexedTriangles(geoCore, true);
      */

    makeIndexedTrianglesConcave(geoCore, true);
}


/*================================ PRIVATE ================================*/

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Define derived entity types as of class ENTITY. Sets default
 *  values to common entity entries data.
 *
 * \see  beginEntity()
 * \todo
 * do registerToParentEntityType() here? Problem: can VERTEX be a direct
 * "child" of DXFEntities? How do we deal with prepare the geometry data? Will
 * this be done in beginEntity for all ENTITIES entries?
 */
DXFEntitiesEntry::DXFEntitiesEntry(void) :
    Inherited(),
    _layerName(),
    _linetypeName(),
    _colorNumber(256),
    _linetypeScale(1.0),
    _thickness(0.0),
    _extrusionDirection(0.0, 0.0, 1.0),
	_trueColor(-1)
{
    _entityClassName = "ENTITY";

//  registerToParentEntityType("SECTION:ENTITIES");
//  registerToParentEntityType("BLOCKSENTRY:BLOCK");
}

/*-------------------------- destructor -----------------------------------*/

/*! Destructor documentation
 */
DXFEntitiesEntry::~DXFEntitiesEntry(void)
{
}
