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

#ifndef _OSGSOFAPOINTSPLATMODEL_H_
#define _OSGSOFAPOINTSPLATMODEL_H_

#include <sofa/core/visual/VisualModel.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/component/topology/TopologyData.h>
#include <sofa/component/component.h>

#include "OSGContribSOFADef.h"
#include "OSGGeometry.h"
#include "OSGGeoProperties.h"
#include "OSGParticles.h"
#include "OSGSimpleMaterial.h"
#include "OSGTextureEnvChunk.h"
#include "OSGTextureObjChunk.h"
#include "OSGBlendChunk.h"


//Work in Progress - Visualization not working

BEGIN_SOFA_CORE_TOPO_NAMESPACE
class BaseMeshTopology;
END_SOFA_CORE_TOPO_NAMESPACE

BEGIN_SOFA_CORE_BEHAVIOR_NAMESPACE
class BaseMechanicalState;
END_SOFA_CORE_BEHAVIOR_NAMESPACE

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

class OSG_CONTRIBSOFA_DLLMAPPING OSGPointSplatModel : public core::visual::VisualModel
{
  public:
    SOFA_CLASS(OSGPointSplatModel,core::visual::VisualModel);
  protected:
    OSGPointSplatModel();
    virtual ~OSGPointSplatModel();
  public:
    virtual void init();

    virtual void reinit();

    virtual void initVisual();
    virtual void updateVisual();

    virtual bool isTransparent() {return true;}

    virtual void drawTransparent(const core::visual::VisualParams* vparams);

    // OpenSG
    OSG::Node* getAttachNode() const { return _attachNode; }

  private:
    void setColor(float r, float g, float b, float a);
    void setColor(std::string color);

  private:
    Data<float>     radius;
    Data<int>       textureSize;
    Data<float>     alpha;
    Data<std::string>   color;

    core::topology::BaseMeshTopology*   _topology;
    core::behavior::BaseMechanicalState* _mstate;

    unsigned char *texture_data;
    float r,g,b,a;
    component::topology::PointData<sofa::helper::vector<unsigned char> >        pointData;

    typedef defaulttype::ExtVec3fTypes::Coord Coord;
    typedef defaulttype::ExtVec3fTypes::VecCoord VecCoord;
    typedef defaulttype::ExtVec3fTypes::Real Real;


    OSG::ParticlesMTRecPtr _particles;
    OSG::GeometryMTRecPtr _geocore;

    OSG::GeoPnt3fPropertyMTRecPtr  _particlePoints;
    OSG::GeoColor3fPropertyMTRecPtr  _particleColors;
    OSG::GeoVec3fPropertyMTRecPtr  _particleSizes;
    OSG::TextureObjChunkUnrecPtr _texture;
    OSG::SimpleMaterialUnrecPtr _simpleTexMaterial;

    OSG::NodeUnrecPtr _attachNode;
};

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFAPOINTSPLATMODEL_H_
