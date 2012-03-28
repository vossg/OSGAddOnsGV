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

#if __GNUC__ >= 4 || __GNUC_MINOR__ >=3
//#pragma GCC diagnostic warning "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include "OSGSofaVisualVisitor.h"
#include "OSGSofaModel.h"
#include "OSGSofaTetrahedralModel.h"
#include "OSGSofaPointSplatModel.h"
#include "OSGSofaClipPlane.h"

//#include "OSGSofaShader.h"
#include <sofa/component/visualmodel/Light.h>
//#include <sofa/component/visualmodel/OglShadowShader.h>

#include "OSGSpotLight.h"
#include "OSGPointLight.h"
#include "OSGDirectionalLight.h"
#include "OSGChunkOverrideGroup.h"

//#include "OSGSimpleShadowMapEngine.h"
//#include "OSGClipPlaneChunk.h"

#include <boost/algorithm/string.hpp>


BEGIN_SOFA_SIM_NAMESPACE

using namespace sofa::component::visualmodel;
using namespace sofa::core::visual;
using           sofa::defaulttype::Vector3;

void OSGLightVisitor::processVisualModel(simulation  ::Node        *, 
                                         core::visual::VisualModel *pVisModel)
{
    SpotLight *pSpotlight = dynamic_cast<SpotLight *>(pVisModel);

    if(pSpotlight != NULL)
    {
        std::cerr << "Light : " << pSpotlight->getClassName() << std::endl;

        OSG::SpotLightUnrecPtr pSpot = OSG::SpotLight::create();

        pSpot->setSpotExponent(pSpotlight->exponent.getValue());
        pSpot->setSpotCutOff  (OSG::osgDegree2Rad(
                                   pSpotlight->cutoff.getValue()));
        //pSpot->setSpotCutOff(pSpotlight->cutoff.getValue());
        
        //pSpot->setPosition(pos[0], pos[1], pos[2]);
        const Vector3 &color = pSpotlight->color.getValue();
 
        pSpot->setAmbient (OSG::Color4f(color[0], color[1], color[2], 1.f));
        pSpot->setDiffuse (OSG::Color4f(color[0], color[1], color[2], 1.f));
        pSpot->setSpecular(OSG::Color4f(color[0], color[1], color[2], 1.f));

        pSpot->setShadowIntensity(0.7);

              Vector3  dir = pSpotlight->direction.getValue();
        const Vector3 &pos = pSpotlight->position .getValue();

        if(pSpotlight->lookat.getValue()) 
            dir -= pos;

        dir.normalize();

        pSpot->setDirection(OSG::Vec3f(dir[0], dir[1], dir[2]));
        
        pSpot->setLinearAttenuation(pSpotlight->attenuation.getValue());

        OSG::NodeUnrecPtr pSLNode = OSG::makeNodeFor(pSpot);

        // first time a light is encountered
        // later lights will be children of the first light
        //if (!_pAttachNode) _pParent->addChild(sLNode);
        // the lastest node to which the rest of the scene will be attached
        //else _pAttachNode->addChild(sLNode);

        if(_pAttachNode != NULL) 
            _pAttachNode->addChild(pSLNode);

        OSG::Matrix m;

        m.setIdentity();
        m.setTranslate(pos[0], pos[1], pos[2]);

        OSG::TransformUnrecPtr pTrans = OSG::Transform::create();
        pTrans->setMatrix(m);

        OSG::NodeUnrecPtr pSLBeaconNode = OSG::makeNodeFor(pTrans);

        // attach light beacon
        _pParent->addChild(pSLBeaconNode);
        
        pSpot->setBeacon(pSLBeaconNode);

        _pAttachNode = pSLNode;
        
        //if(_pShadowStage != NULL) 
        //    _pShadowStage->editMFLightNodes()->push_back(_pAttachNode);

        return;        
    }

    PositionalLight *pPointlight = dynamic_cast<PositionalLight *>(pVisModel);

    if(pPointlight != NULL)
    {
        std::cerr << "Light : " << pPointlight->getClassName() << std::endl;

        OSG::PointLightUnrecPtr pPoint = OSG::PointLight::create();
        
        //pSpot->setPosition(pos[0], pos[1], pos[2]);
        const Vector3 &color = pPointlight->color.getValue();

        pPoint->setAmbient (OSG::Color4f(color[0], color[1], color[2], 1.f));
        pPoint->setDiffuse (OSG::Color4f(color[0], color[1], color[2], 1.f));
        pPoint->setSpecular(OSG::Color4f(color[0], color[1], color[2], 1.f));

        pPoint->setShadowIntensity(0.7);

        pPoint->setLinearAttenuation(pPointlight->attenuation.getValue());

        const Vector3 &pos = pPointlight->position.getValue();

        OSG::NodeUnrecPtr pLNode = OSG::makeNodeFor(pPoint);

        // first time a light is encountered
        // later lights will be children of the first light
        //if (!_pAttachNode) _pParent->addChild(pLNode);
        // the lastest node to which the rest of the scene will be attached
        //else _pAttachNode->addChild(pLNode);

        if(_pAttachNode != NULL) 
            _pAttachNode->addChild(pLNode);

        OSG::Matrix m;

        m.setIdentity();
        m.setTranslate(pos[0], pos[1], pos[2]);

        OSG::TransformUnrecPtr pTrans = OSG::Transform::create();

        pTrans->setMatrix(m);

        OSG::NodeUnrecPtr pLBeaconNode = OSG::makeNodeFor(pTrans);

        // attach light beacon
        _pParent->addChild(pLBeaconNode);

        pPoint->setBeacon(pLBeaconNode);

        _pAttachNode = pLNode;

        //if(_pShadowStage) 
        //    _pShadowStage->editMFLightNodes()->push_back(_pAttachNode);

        return;        
    }

    DirectionalLight *pDirlight = dynamic_cast<DirectionalLight *>(pVisModel);

    if(pDirlight != NULL)
    {
        std::cerr << "Light : " << pDirlight->getClassName() << std::endl;

        OSG::DirectionalLightUnrecPtr pDir = OSG::DirectionalLight::create();
        
        //spot->setPosition(pos[0], pos[1], pos[2]);
        const Vector3 &color = pDirlight->color.getValue();

        pDir->setAmbient (OSG::Color4f(color[0], color[1], color[2], 1.f));
        pDir->setDiffuse (OSG::Color4f(color[0], color[1], color[2], 1.f));
        pDir->setSpecular(OSG::Color4f(color[0], color[1], color[2], 1.f));

        pDir->setShadowIntensity(0.7);

        Vector3 direct = pDirlight->direction.getValue();

        pDir->setDirection(OSG::Vec3f(direct[0], direct[1], direct[2]));


        OSG::NodeUnrecPtr pLNode = OSG::makeNodeFor(pDir);

        // first time a light is encountered
        // later lights will be children of the first light
        //if (!_pAttachNode) _pParent->addChild(pLNode);
        // the lastest node to which the rest of the scene will be attached
        //else _pAttachNode->addChild(pLNode);

        if(_pAttachNode != NULL)
            _pAttachNode->addChild(pLNode);

        OSG::Matrix m;

        m.setIdentity();
        //m.setTranslate(pos[0], pos[1], pos[2]);

        OSG::TransformUnrecPtr pTrans = OSG::Transform::create();

        pTrans->setMatrix(m);

        OSG::NodeUnrecPtr pLBeaconNode = OSG::makeNodeFor(pTrans);

        // attach light beacon
        _pParent->addChild(pLBeaconNode);

        pDir->setBeacon(pLBeaconNode);

        // test Shadow
        //OSG::SimpleShadowMapEngineRecPtr shadowEngine = 
        //      OSG::SimpleShadowMapEngine::create();
        //if (!shadowEngine) system("pause");
        //shadowEngine->setWidth(1024);
        //shadowEngine->setHeight(1024);
        //shadowEngine->setShadowColor(OSG::Color4f(1.0f, 1.0f, 1.0f, 1.0f));
        //dir->setLightEngine(shadowEngine);

        _pAttachNode = pLNode;

        //if (_shadowStage) 
        //    _shadowStage->editMFLightNodes()->push_back(_pAttachNode);

        return;        
    }
}

Visitor::Result OSGLightVisitor::processNodeTopDown(simulation::Node *pNode)
{
    if(_pParent == NULL) 
        return RESULT_PRUNE;

    for_each( this, 
              pNode, 
              pNode->visualModel, 
             &OSGLightVisitor::processVisualModel);

    return RESULT_CONTINUE;
}




void OSGVisualUpdateVisitor::processVisualModel(
    simulation  ::Node        *pNode, 
    core::visual::VisualModel *pVisModel)
{
//    pVisModel->updateVisual();

    std::cerr << "CLASS_NAME : " << pVisModel->getClassName() << std::endl;

    OSGSofaModel *osg2Model = dynamic_cast<OSGSofaModel *>(pVisModel);

    if(osg2Model != NULL)
    {
        if(_pParent != NULL) 
        {
            if(osg2Model->getAttachNode() != NULL) 
            {
                _pParent->addChild(osg2Model->getAttachNode());

            }
        }

        return;
    }

    if(boost::iequals(pVisModel->getClassName(), "OSGClipPlane"))
    {
        if( _pChunkOverrideGroupNode == NULL)
            return;

        OSG::ClipPlaneChunkUnrecPtr pClip = OSG::ClipPlaneChunk::create();

        OSGClipPlane *pOSGClip = dynamic_cast<OSGClipPlane *>(pVisModel);

        Vector3 position = pOSGClip->position.getValue();
        Vector3 normal   = pOSGClip->normal  .getValue();
        
        pClip->setEquation(OSG::Vec4f(-normal[0], 
                                      -normal[1], 
                                      -normal[2],  
                                       position * normal));
        
        pClip->setEnable(pOSGClip->active.getValue());

        pClip->setBeacon(_pChunkOverrideGroupNode);

        OSG::NodeCore           *pNC  = _pChunkOverrideGroupNode->getCore();

        OSG::ChunkOverrideGroup *pCOG = 
            dynamic_cast<OSG::ChunkOverrideGroup *>(pNC);

        if(pCOG != NULL)
            pCOG->addChunk(pClip);

        pOSGClip->setClipPlane(pClip);

        return;        
    }
    //any better way??
    else if(boost::iequals(pVisModel->getClassName(),
                           "OSGTetrahedralModel"     ) &&   
            boost::iequals(pVisModel->getTemplateName(), 
                           "vec3d"                     )   )
    {
//        std::cerr << "CLASS_NAME : " 
//                  << pVisModel->getClassName() 
//                  << std::endl;

        std::cerr << "TEMPLATE_NAME : " 
                  << pVisModel->getTemplateName() 
                  << std::endl;

        OSGTetrahedralModel<Vec3dTypes> *pOSG2TModel = 
            dynamic_cast<OSGTetrahedralModel<Vec3dTypes> *>(pVisModel);

        if(pOSG2TModel == NULL) 
            return;

        if(_pParent != NULL) 
        {
            if(pOSG2TModel->getAttachNode() != NULL) 
            {
                _pParent->addChild(pOSG2TModel->getAttachNode());
            }
        }

        return;
    }
    else if(boost::iequals(pVisModel->getClassName(),
                           "OSGTetrahedralModel"     ) &&   
            boost::iequals(pVisModel->getTemplateName(),
                           "vec3f"                     )   )
    {
//        std::cerr << "CLASS_NAME : " 
//                  << pVisModel->getClassName() 
//                  << std::endl;

        std::cerr << "TEMPLATE_NAME : " 
                  << pVisModel->getTemplateName() 
                  << std::endl;

        OSGTetrahedralModel<Vec3fTypes> *pOSG2TModel = 
            dynamic_cast<OSGTetrahedralModel<Vec3fTypes> *>(pVisModel);

        if(pOSG2TModel == NULL) 
            return;

        if(_pParent != NULL) 
        {
            if(pOSG2TModel->getAttachNode() != NULL) 
            {
                _pParent->addChild(pOSG2TModel->getAttachNode());
            }
        }

        return;
    }
    else if(boost::iequals(pVisModel->getClassName(), "OSGPointSplatModel"))
    {
        OSGPointSplatModel *pPointModel = 
            dynamic_cast<OSGPointSplatModel *>(pVisModel);

        if(pPointModel == NULL)
            return;

        if(_pParent != NULL) 
        {
            if(pPointModel->getAttachNode() != NULL) 
            {
                _pParent->addChild(pPointModel->getAttachNode());
            }
        }

        return;
    }
}

Visitor::Result OSGVisualUpdateVisitor::processNodeTopDown(
    simulation::Node *pNode)
{
    for_each( this, 
              pNode, 
              pNode->visualModel, 
             &OSGVisualUpdateVisitor::processVisualModel);

    return RESULT_CONTINUE;
}




void OSGVisualInitVisitor::processVisualModel(
    simulation  ::Node        *pNode, 
    core::visual::VisualModel *pVisModel)
{
    if(boost::iequals(pVisModel->getClassName(), "LightManager")) 
    {
        return;
    }
    else if(boost::iequals(pVisModel->getClassName(), "SpotLight")) 
    {
        return;
    }
    else if(boost::iequals(pVisModel->getClassName(), "DirectionalLight")) 
    {
        return;
    }
    else if(boost::iequals(pVisModel->getClassName(), "PositionalLight")) 
    {
        return;
    }

    std::cerr << "INIT CLASS_NAME : " 
              << pVisModel->getClassName() 
              << std::endl;

    pVisModel->initVisual();
}


Visitor::Result OSGVisualInitVisitor::processNodeTopDown(
    simulation::Node *pNode)
{
    for_each( this, 
              pNode, 
              pNode->visualModel, 
             &OSGVisualInitVisitor::processVisualModel);

    return RESULT_CONTINUE;
}

END_SOFA_SIM_NAMESPACE

