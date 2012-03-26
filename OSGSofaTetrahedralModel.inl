/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                 Copyright (C) 2012 by the OpenSG Forum                    *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 * contact: dirk            'at' opensg.org                                  * 
 *          gerrit.voss     'at' vossg.org                                   *
 *          carsten_neumann 'at' gmx.net                                     *
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

#ifndef OSG2_TETRAHEDRALMODEL_INL_
#define OSG2_TETRAHEDRALMODEL_INL_

#include "OSGSofaTetrahedralModel.h"
#include "OSGSofaShader.h"

#include <sofa/helper/gl/GLSLShader.h>
#include <sofa/defaulttype/BoundingBox.h>
#include <limits>

#include "OSGGeometry.h"
#include "OSGParticles.h"
#include "OSGGeoProperties.h"
#include "OSGSimpleMaterial.h"
#include "OSGPolygonChunk.h"


OSG::Real32 random(OSG::Real32 lower, OSG::Real32 upper)
{
    return lower + OSG::osgRand() * (upper - lower);
}


namespace sofa
{
namespace component
{
namespace visualmodel
{

using namespace sofa::defaulttype;

template<class DataTypes>
OSG2_TetrahedralModel<DataTypes>::OSG2_TetrahedralModel()
: nodes(NULL), topo(NULL)
, depthTest(initData(&depthTest, (bool) true, "depthTest", "Set Depth Test"))
, blending(initData(&blending, (bool) true, "blending", "Set Blending"))

{
}

template<class DataTypes>
OSG2_TetrahedralModel<DataTypes>::~OSG2_TetrahedralModel()
{
}

template<class DataTypes>
void OSG2_TetrahedralModel<DataTypes>::init()
{
    sofa::core::objectmodel::BaseContext* context = this->getContext();
    context->get(topo);
    context->get(nodes);

    if (!nodes)
    {
        serr << "OSG2_TetrahedralModel : Error : no MechanicalState found." << sendl;
        return;
    }

    if (!topo)
    {
        serr << "OSG2_TetrahedralModel : Error : no BaseMeshTopology found." << sendl;
        return;
    }
}

template<class DataTypes>
void OSG2_TetrahedralModel<DataTypes>::initVisual()
{
//    serr << "OSG2_TetrahedralModel : initVisual." << this << sendl;



    core::topology::BaseMeshTopology::SeqTetrahedra::const_iterator it;

#ifdef GL_LINES_ADJACENCY_EXT

    const core::topology::BaseMeshTopology::SeqTetrahedra& vec = topo->getTetrahedra();

    const VecCoord& x = *nodes->getX();
    Coord v;

    OSG::GeoPnt3fPropertyMTRecPtr  _polygonPoints; 

    _polygonPoints =  OSG::GeoPnt3fProperty::create();

    for(it = vec.begin() ; it != vec.end() ; it++)
    {
            for (unsigned int i=0 ; i< 4 ; i++)
            {
                v = x[(*it)[i]];
                //glVertex3f((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2]);
                //std::cerr << "TETRA " << i << ": " <<  v[0] << " " << v[1] << " " << v[2] << std::endl;

                _polygonPoints->addValue(OSG::Pnt3f(v[0],v[1],v[2]));
            }
    }

    OSG::GeoUInt8PropertyRecPtr polyTypes = OSG::GeoUInt8Property::create();
    OSG::GeoUInt32PropertyMTRecPtr polyLens =  OSG::GeoUInt32Property::create();
    OSG::GeoUInt32PropertyMTRecPtr polyIndices =  OSG::GeoUInt32Property::create();
    OSG::GeometryRecPtr geoCore = OSG::Geometry::create();

    //polyTypes->addValue(GL_LINES);
    polyTypes->addValue(GL_LINES_ADJACENCY_EXT);
    //polyTypes->addValue(GL_QUADS);
    polyLens->addValue(4);
    polyIndices->addValue(0);
    polyIndices->addValue(1);
    polyIndices->addValue(2);
    polyIndices->addValue(3);
    
    geoCore->setTypes(polyTypes);
    geoCore->setLengths(polyLens);
    geoCore->setPositions(_polygonPoints);
    geoCore->setIndices(polyIndices);



    OSG::SimpleMaterialRecPtr simpleMaterial = OSG::SimpleMaterial::create();
    simpleMaterial->setLit(false);

    
    sofa::core::objectmodel::BaseContext* context = this->getContext();

    // Find attached shader
    OSG2_Shader* shader = context->core::objectmodel::BaseContext::get<OSG2_Shader>();

    if (shader)
    {
        if (shader->getShaderChunk(0))
        {
                simpleMaterial->addChunk( shader->getShaderChunk(0) );
                simpleMaterial->addChunk( shader->getShaderVariableChunk(0) );
        }

    }
    OSG::PolygonChunkUnrecPtr _polygonChunk = OSG::PolygonChunk::create();
    _polygonChunk->setCullFace(GL_NONE);
    simpleMaterial->addChunk(_polygonChunk);


    geoCore->setMaterial(simpleMaterial);

    OSG::NodeRecPtr geo_node = OSG::makeNodeFor(geoCore);
    _attachNode = geo_node;


#endif
}


template<class DataTypes>
void OSG2_TetrahedralModel<DataTypes>::computeBBox(const core::ExecParams * params)
{
    if (nodes && topo)
    {
        const core::topology::BaseMeshTopology::SeqTetrahedra& vec = topo->getTetrahedra();
        core::topology::BaseMeshTopology::SeqTetrahedra::const_iterator it;
        const VecCoord& x = *nodes->getX();
        Coord v;

        const SReal max_real = std::numeric_limits<SReal>::max();
        const SReal min_real = std::numeric_limits<SReal>::min();

        SReal maxBBox[3] = {min_real,min_real,min_real};
        SReal minBBox[3] = {max_real,max_real,max_real};

        for(it = vec.begin() ; it != vec.end() ; it++)
        {
                for (unsigned int i=0 ; i< 4 ; i++){
                    v = x[(*it)[i]];

                    if (minBBox[0] > v[0]) minBBox[0] = v[0];
                    if (minBBox[1] > v[1]) minBBox[1] = v[1];
                    if (minBBox[2] > v[2]) minBBox[2] = v[2];
                    if (maxBBox[0] < v[0]) maxBBox[0] = v[0];
                    if (maxBBox[1] < v[1]) maxBBox[1] = v[1];
                    if (maxBBox[2] < v[2]) maxBBox[2] = v[2];
                }
        }

         this->f_bbox.setValue(params,sofa::defaulttype::TBoundingBox<SReal>(minBBox,maxBBox));
    }
}

}
}
}

#endif //OGLTETRAHEDRALMODEL_H_
