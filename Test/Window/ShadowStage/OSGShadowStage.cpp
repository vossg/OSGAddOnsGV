/*---------------------------------------------------------------------------*\
*                                OpenSG                                     *
*                                                                           *
*                                                                           *
*               Copyright (C) 2000-2002 by the OpenSG Forum                 *
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

#define GL_GLEXT_PROTOTYPES

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <OSGConfig.h>
#include <OSGQuaternion.h>
#include <OSGRenderAction.h>
#include <OSGMultiPassMaterial.h>
#include <OSGMatrix.h>
#include <OSGMatrixUtility.h>
#include <OSGBackground.h>
#include <OSGForeground.h>
#include <OSGImage.h>
#include <OSGMaterialGroup.h>
#include <OSGGeometry.h>
#include <OSGLight.h>
#include "OSGShadowStage.h"

#include "OSGTreeHandler.h"

#define OSG_HAS_STDMAP
#define OSG_HAS_PERSPMAP
#define OSG_HAS_DITHERMAP
#define OSG_HAS_PCFMAP
#define OSG_HAS_PCSSMAP
#define OSG_HAS_VARMAP

#ifdef OSG_HAS_STDMAP
#include "OSGStdShadowMapHandler.h"
#endif
#ifdef OSG_HAS_PERSPMAP
#include "OSGPerspectiveShadowMapHandler.h"
#endif
#ifdef OSG_HAS_DITHERMAP
#include "OSGDitherShadowMapHandler.h"
#endif
#ifdef OSG_HAS_PCFMAP
#include "OSGPCFShadowMapHandler.h"
#endif
#ifdef OSG_HAS_PCSSMAP
#include "OSGPCSSShadowMapHandler.h"
#endif
#ifdef OSG_HAS_VARMAP
#include "OSGVarianceShadowMapHandler.h"
#endif

#include "OSGRenderAction.h"
#include <boost/bind.hpp>

//--------------------------------------------------------------------
#ifndef GL_CLAMP_TO_EDGE
#   define GL_CLAMP_TO_EDGE    0x812F
#endif
#ifndef GL_CLAMP_TO_BORDER_ARB
#   define GL_CLAMP_TO_BORDER_ARB  0x812D
#endif
#ifndef GL_ARB_depth_texture
#   define GL_DEPTH_COMPONENT16_ARB    0x81A5
#   define GL_DEPTH_COMPONENT24_ARB    0x81A6
#   define GL_DEPTH_COMPONENT32_ARB    0x81A7
#   define GL_TEXTURE_DEPTH_SIZE_ARB   0x884A
#   define GL_DEPTH_TEXTURE_MODE_ARB   0x884B
#endif
#ifndef GL_ARB_shadow
#   define GL_TEXTURE_COMPARE_MODE_ARB 0x884C
#   define GL_TEXTURE_COMPARE_FUNC_ARB 0x884D
#   define GL_COMPARE_R_TO_TEXTURE_ARB 0x884E
#endif
#ifndef GL_SGIX_depth_texture
#   define GL_DEPTH_COMPONENT16_SGIX   0x81A5
#   define GL_DEPTH_COMPONENT24_SGIX   0x81A6
#   define GL_DEPTH_COMPONENT32_SGIX   0x81A7
#endif
#ifndef GL_SGIX_shadow
#   define GL_TEXTURE_COMPARE_SGIX             0x819A
#   define GL_TEXTURE_COMPARE_OPERATOR_SGIX    0x819B
#   define GL_TEXTURE_LEQUAL_R_SGIX            0x819C
#   define GL_TEXTURE_GEQUAL_R_SGIX            0x819D
#endif
#ifndef GL_DEPTH_COMPONENT_ARB
#   define GL_DEPTH_COMPONENT_ARB            0x1902
#endif

//--------------------------------------------------------------------
OSG_USING_NAMESPACE

/***************************************************************************\
*                            Description                                  *
\***************************************************************************/

/***************************************************************************\
*                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
*                           Class methods                                 *
\***************************************************************************/

void ShadowStage::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
        RenderAction::registerEnterDefault(
            ShadowStage::getClassType(), 
            reinterpret_cast<Action::Callback>(&ShadowStage::renderEnter));
        
        RenderAction::registerLeaveDefault( 
            ShadowStage::getClassType(), 
            reinterpret_cast<Action::Callback>(&ShadowStage::renderLeave));

    }
}


/***************************************************************************\
*                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
   -  private                                                                 -
   \*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

ShadowStage::ShadowStage(void) :
    Inherited(),
    _bRunning(false),
    _GLSLsupported(false),
    _initDone(false),
    _restart(false),
    _treeRenderer(NULL),
    _mapRenderSize(128),
    _mapSizeChanged(false),
    _texGen(NULL),
    _poly(NULL),
    _offset(NULL),
    _dummy(NULL),
    _silentBack(NULL),
    _windowW(0),
    _windowH(0),
    _transparent(),
    _lights(),
    _oldLights(),
    _lightCameras(),
    _lightCamTrans(),
    _lightCamBeacons(),
    _lightStates(),
    _shadowImages(),
    _texChunks(),
    _excludeNodeActive(),
    _realPointLight(),
    _renderSide(),
    _trigger_update(false),
    _transforms(),
    _light_render_transform(NULL),
    _occlusionQuery(0)
{
    _transforms[0] = Matrix(1, 0, 0, 0,
                            0, -1, 0, 0,
                            0, 0, -1, 0,
                            0, 0, 0, 1);

    _transforms[1] = Matrix(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1);

    _transforms[2] = Matrix(1, 0, 0, 0,
                            0, 0, 1, 0,
                            0, -1, 0, 0,
                            0, 0, 0, 1);

    _transforms[3] = Matrix(1, 0, 0, 0,
                            0, 0, -1, 0,
                            0, 1, 0, 0,
                            0, 0, 0, 1);

    _transforms[4] = Matrix(0, 0, 1, 0,
                            0, 1, 0, 0,
                            -1, 0, 0, 0,
                            0, 0, 0, 1);

    _transforms[5] = Matrix(0, 0, -1, 0,
                            0, 1, 0, 0,
                            1, 0, 0, 0,
                            0, 0, 0, 1);
}

ShadowStage::ShadowStage(const ShadowStage &source) :
    Inherited(source),
    _bRunning(false),
    _GLSLsupported(source._GLSLsupported),
    _initDone(source._initDone),
    _restart(source._restart),
    _treeRenderer(source._treeRenderer),
    _mapRenderSize(source._mapRenderSize),
    _mapSizeChanged(source._mapSizeChanged),
    _texGen(source._texGen),
    _poly(source._poly),
    _offset(source._offset),
    _dummy(source._dummy),
    _silentBack(source._silentBack),
    _windowW(source._windowW),
    _windowH(source._windowH),
    _transparent(source._transparent),
    _lights(source._lights),
    _oldLights(source._oldLights),
    _lightCameras(source._lightCameras),
    _lightCamTrans(source._lightCamTrans),
    _lightCamBeacons(source._lightCamBeacons),
    _lightStates(source._lightStates),
    _shadowImages(source._shadowImages),
    _texChunks(source._texChunks),
    _excludeNodeActive(source._excludeNodeActive),
    _realPointLight(source._realPointLight),
    _renderSide(source._renderSide),
    _trigger_update(source._trigger_update),
    _transforms(),
    _light_render_transform(source._light_render_transform),
    _occlusionQuery(source._occlusionQuery)
{
    _treeRenderer = NULL;
    _initDone = false;
    _restart = false;

    _transforms[0] = Matrix(1, 0, 0, 0,
                            0, -1, 0, 0,
                            0, 0, -1, 0,
                            0, 0, 0, 1);

    _transforms[1] = Matrix(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1);

    _transforms[2] = Matrix(1, 0, 0, 0,
                            0, 0, 1, 0,
                            0, -1, 0, 0,
                            0, 0, 0, 1);

    _transforms[3] = Matrix(1, 0, 0, 0,
                            0, 0, -1, 0,
                            0, 1, 0, 0,
                            0, 0, 0, 1);

    _transforms[4] = Matrix(0, 0, 1, 0,
                            0, 1, 0, 0,
                            -1, 0, 0, 0,
                            0, 0, 0, 1);

    _transforms[5] = Matrix(0, 0, -1, 0,
                            0, 1, 0, 0,
                            1, 0, 0, 0,
                            0, 0, 0, 1);

}

ShadowStage::~ShadowStage(void)
{
    _light_render_transform = NULL;
}

/*----------------------------- class specific ----------------------------*/

void ShadowStage::changed(BitVector whichField, 
                          UInt32    origin,
                          BitVector details)
{
    if(whichField & OffBiasFieldMask ||
       whichField & OffFactorFieldMask)
    {
        FDEBUG(("ShadowStage::changed : ofsset bias/factor changed.\n"));
        // Setting up Polygon-Chunk with Depth-Offset
        {
            _poly->setOffsetBias(getOffBias());
            _poly->setOffsetFactor(getOffFactor());
            _poly->setOffsetFill(true);
        }
    }

    if(whichField & LightNodesFieldMask)
    {
        FDEBUG(("ShadowStage::changed : light nodes changed.\n"));
        _lights.clear();
        for(UInt32 i = 0;i < getMFLightNodes()->size();++i)
            _lights.push_back(
                std::make_pair(
                    getLightNodes(i), 
                    dynamic_cast<Light *>(getLightNodes(i)->getCore())));
    }

    if(whichField & MapAutoUpdateFieldMask)
    {
        _trigger_update = true;
    }

    if(whichField & AutoSearchForLightsFieldMask)
    {
        if(getAutoSearchForLights())
        {
            FNOTICE(("auto light search mode on\n"));
        }
        else
        {
            FNOTICE(("auto light search mode off\n"));
        }
    }

    if(whichField & ShadowSmoothnessFieldMask)
    {
        if(getShadowSmoothness() < 0.0)
        {
            FNOTICE(("No ShadowSmoothness < 0 allowed!\n"));
            setShadowSmoothness(0.0);
        }
        if(getShadowSmoothness() > 1.0)
        {
            FNOTICE(("No ShadowSmoothness > 1.0 allowed!\n"));
            setShadowSmoothness(1.0);
        }
        FNOTICE(("ShadowSmoothness set to %f\n", getShadowSmoothness()));
    }

    if(whichField & ShadowModeFieldMask || whichField & MapSizeFieldMask)
    {
        _restart = true;
    }

    Inherited::changed(whichField, origin, details);
}

void ShadowStage::dump(      UInt32,
                       const BitVector) const
{
    SLOG << "Dump ShadowStage NI" << std::endl;
}


void ShadowStage::onCreate(const ShadowStage *OSG_CHECK_ARG(source))
{
    // if we're in startup this is the prototype ...
    if(OSG::GlobalSystemState == OSG::Startup)
        return;

    _treeRenderer = NULL;

    _mapRenderSize = 128;

    _dummy = makeCoredNode<Group>();

    _texGen = TexGenChunk::create();

    //------Setting up TexGen--------------
    {
        _texGen->setSBeacon(_dummy);
        _texGen->setTBeacon(_dummy);
        _texGen->setRBeacon(_dummy);
        _texGen->setQBeacon(_dummy);

        _texGen->setGenFuncS(GL_EYE_LINEAR);
        _texGen->setGenFuncT(GL_EYE_LINEAR);
        _texGen->setGenFuncR(GL_EYE_LINEAR);
        _texGen->setGenFuncQ(GL_EYE_LINEAR);
    }

    _poly = PolygonChunk::create();

    _offset = PolygonChunk::create();

    _silentBack = PassiveBackground::create();
}

void ShadowStage::onDestroy(void)
{
    clearLights(_lights.size());

    _silentBack = NULL;
    _poly       = NULL;
    _texGen     = NULL;
    _dummy      = NULL;
}

void ShadowStage::postProcess(DrawEnv *pEnv)
{
    RenderAction *ract = dynamic_cast<RenderAction *>(pEnv->getAction());

    ract->disableDefaultPartition();
}

ActionBase::ResultE ShadowStage::renderEnter(Action *action)
{
    ActionBase::ResultE returnValue = ActionBase::Continue;

//    fprintf(stderr, "ShadowStage::renderEnter\n");

    if(_bRunning == true)
        return returnValue;

    RenderAction *ract = dynamic_cast<RenderAction *>(action);


    _bRunning = true;

    if(_restart)
    {
        if(_treeRenderer != NULL)
            delete _treeRenderer;

        _treeRenderer = NULL;

        clearLights(_oldLights.size());

        _mapSizeChanged = true;

        switch(getShadowMode())
        {
            case NO_SHADOW:
            {
                FNOTICE(("No Shadows\n"));
            }
            break;
            
            case STD_SHADOW_MAP:
            {
                FNOTICE(("using standard Shadow Mapping...\n"));
#ifdef OSG_HAS_STDMAP
                _treeRenderer = new StdShadowMapHandler(this);
#endif
            }
            break;
            
            case PERSPECTIVE_SHADOW_MAP:
            {
                FNOTICE(("using Lisp Perspective Shadow Mapping...\n"));
#ifdef OSG_HAS_PERSPMAP
                _treeRenderer = new PerspectiveShadowMapHandler(this);
#endif
            }
            break;
            
            case DITHER_SHADOW_MAP:
            {
                FNOTICE(("using Dither Shadow Mapping...\n"));
#ifdef OSG_HAS_DITHERMAP
                _treeRenderer = new DitherShadowMapHandler(this);
#endif
            }
            break;

            case PCF_SHADOW_MAP:
            {
                FNOTICE(("using PCF Shadow Mapping...\n"));
#ifdef OSG_HAS_PCFMAP
                _treeRenderer = new PCFShadowMapHandler(this);
#endif
            }
            break;
            
            case PCSS_SHADOW_MAP:
            {
                FNOTICE(("using PCSS Shadow Mapping...\n"));
#ifdef OSG_HAS_PCSSMAP
                _treeRenderer = new PCSSShadowMapHandler(this);
#endif
            }
            break;
            
            case VARIANCE_SHADOW_MAP:
            {
                FNOTICE(("using Variance Shadow Mapping...\n"));
#ifdef OSG_HAS_VARMAP
                _treeRenderer = new VarianceShadowMapHandler(this);
#endif
            }
            break;

            default:
                break;
        }

        _restart = false;
    }

    if(_treeRenderer == NULL || getShadowOn() == false)
    {
        _bRunning = false;

        return returnValue;
    }


    if(!_initDone)
    {
        _GLSLsupported = true;

        if(!ract->getWindow()->hasExtension("GL_ARB_shading_language_100") ||
           !ract->getWindow()->hasExtension("GL_ARB_fragment_shader") ||
           !ract->getWindow()->hasExtension("GL_ARB_vertex_shader") ||
           !ract->getWindow()->hasExtension("GL_ARB_shader_objects") )
        {
            _GLSLsupported = false;
        }

        _initDone = true;
    }

    if(getSceneRoot() == NULL)
    {
        setSceneRoot(action->getActNode());
    }

    _excludeNodeActive.clear();
    _realPointLight   .clear();

    //get excludeNode states
    for(UInt32 i = 0;i < getMFExcludeNodes()->size();i++)
    {
        Node *exnode = getExcludeNodes(i);

        if(exnode != NULL)
        {
            _excludeNodeActive.push_back(exnode->getTravMask() != 0);
        }
        else
        {
            _excludeNodeActive.push_back(false);
        }
    }

    RenderAction *tmpAction = RenderAction::create();

    tmpAction->setCamera    ( ract->getCamera() );
    tmpAction->setBackground(_silentBack  );
    tmpAction->setViewport  ( ract->getViewport()  );
    tmpAction->setTravMask  ( ract->getTravMask());
    tmpAction->setWindow    ( ract->getWindow());
    checkLights(ract);

    bool allLightsZero = true;

    if(getGlobalShadowIntensity() != 0.0)
    {
        allLightsZero = false;
    }
    else
    {
        for(UInt32 i = 0;i < _lights.size();i++)
        {
            if(_lights[i].second->getShadowIntensity() != 0.0 && 
               _lightStates[i] != 0)
            {
                allLightsZero = false;
            }
        }
    }
    if(_lights.size() == 0 || allLightsZero)
    {
        delete tmpAction;

        _bRunning = false;

        return returnValue;
    }
    else
    {
        ract->disableDefaultPartition();

        //find transparent nodes
        _transparent.clear();

        if(getAutoExcludeTransparentNodes())
        {
            traverse(getSceneRoot(), 
                     boost::bind(&ShadowStage::findTransparent,
                                 this, 
                                 _1) );
        }

        _windowW = ract->getViewport()->getPixelWidth();
        _windowH = ract->getViewport()->getPixelHeight();

        //check if excludeNodes are disabled
        for(UInt32 i = 0;i < getMFExcludeNodes()->size();++i)
        {
            Node *exnode = getExcludeNodes(i);

            _excludeNodeActive[i] = exnode->getTravMask() != 0;
        }

        //check if all sides of a pointlight are needed
        //TODO: Not implemented yet ...
        _renderSide.clear();

#if 0
        DrawEnv oEnv;
        
        oEnv.setWindow(ract->getWindow());
        oEnv.setAction(ract);
#endif

        // active stereo support.
        activate();

        _treeRenderer->render(&(ract->getActivePartition()->getDrawEnv()), 
                              tmpAction);

        deactivate();

        returnValue = ActionBase::Skip;
    }

    delete tmpAction;

    _bRunning = false;

    return returnValue;
}

ActionBase::ResultE ShadowStage::renderLeave(Action *action)
{
    ActionBase::ResultE returnValue = ActionBase::Continue;

//    fprintf(stderr, "ShadowStage::renderLeave\n");

    return returnValue;
}

















#if 0
void ShadowStage::setVPSize(Real32 a, Real32 b, Real32 c, Real32 d)
{
#ifdef SHADOW_CHECK
    this->setSize(a, b, c, d);

    this->activateSize();
#endif
}
#endif

void ShadowStage::triggerMapUpdate(void)
{
    editSField(MapAutoUpdateFieldMask);
}


#if 0
void ShadowStage::activateSize(void)
{
#ifdef SHADOW_CHECK
    Inherited::activateSize();
#endif
}
#endif

void ShadowStage::activate(void)
{
    glColorMask(getRed(), getGreen(), getBlue(), getAlpha());

#ifdef SHADOW_CHECK
    Inherited::activate();
#endif
}

void ShadowStage::deactivate(void)
{
#ifdef SHADOW_CHECK
    Inherited::deactivate();
#endif

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void ShadowStage::render(RenderActionBase *action)
{


#ifdef SHADOW_CHECK
    if(getCamera() == NULL)
    {
        SWARNING << "ShadowStage::render: no camera!" << std::endl;
        return;
    }

    if(getBackground() == NULL)
    {
        SWARNING << "ShadowStage::render: no Background!" << std::endl;
        return;
    }

    if(getRoot() == NULL)
    {
        SWARNING << "ShadowStage::render: no root!" << std::endl;
        return;
    }
#endif






#ifdef SHADOW_CHECK
    action->setCamera    ( getCamera() );
    action->setBackground(_silentBack  );
    action->setViewport  ( this        );
    action->setTravMask  (getTravMask());
#endif


}

Action::ResultE ShadowStage::findLight(Node * const node)
{
    if(node->getCore()->getType().isDerivedFrom(Light::getClassType()))
    {
        _lights.push_back(
            std::make_pair(
                node, 
                dynamic_cast<Light *>(node->getCore())));
    }

    return Action::Continue;
}

Action::ResultE ShadowStage::findTransparent(Node * const node)
{
    if(node->getCore() != NULL)
    {
        if(node->getCore()->getType() == Geometry::getClassType() ||
           node->getCore()->getType() == MaterialGroup::getClassType())
        {
            Material               *mat = NULL;
            MultiPassMaterial      *multiMat = NULL;
            Geometry               *geo = dynamic_cast<Geometry *>(
                node->getCore());
            MaterialGroup          *matGroup = 
                dynamic_cast<MaterialGroup *>(node->getCore());

            if(geo != NULL)
                mat = geo->getMaterial();
            if(matGroup != NULL)
                mat = matGroup->getMaterial();
            if(mat != NULL)
            {
                /* isTransparent in MultiPassMaterial returns false,
                   if one Material is not transparent. Here we need
                   to know if one Material is transparent so we can't
                   use isTransparent for MultiPassMaterials. */
                multiMat = dynamic_cast<MultiPassMaterial *>(mat);
                if(multiMat != NULL)
                {
                    MultiPassMaterial::MFMaterialsType::const_iterator it = 
                        multiMat->getMFMaterials()->begin();

                    MultiPassMaterial::MFMaterialsType::const_iterator matsEnd =
                        multiMat->getMFMaterials()->end();

                    for(;it != matsEnd;++it)
                    {
                        if((*it) == NULL)
                            continue;
                        if((*it)->isTransparent() && node->getTravMask() != 0)
                        {
                            _transparent.push_back(node);
                            break;
                        }
                    }
                }
                else
                {
                    if(mat->isTransparent() && node->getTravMask() != 0)
                        _transparent.push_back(node);
                }
            }

        }

        if(node->getCore()->getType() == Geometry::getClassType())
        {
            Geometry *geo = dynamic_cast<Geometry *>(node->getCore());

            if(geo->getMaterial() != NULL &&
               geo->getMaterial()->isTransparent() &&
               node->getTravMask() != 0)
                _transparent.push_back(node);
        }
        else if(node->getCore()->getType() == MaterialGroup::getClassType())
        {
            MaterialGroup *matGroup = dynamic_cast<MaterialGroup *>(
                node->getCore());

            if(matGroup->getMaterial() != NULL &&
               matGroup->getMaterial()->isTransparent() &&
               node->getTravMask()  != 0)
                _transparent.push_back(node);
        }
    }
    return Action::Continue;
}



void ShadowStage::checkLights(RenderActionBase *action)
{
    if(getAutoSearchForLights())
    {
        //Finding lights by going through whole Scenegraph
        _lights.clear();

        traverse(getSceneRoot(), 
                 boost::bind(&ShadowStage::findLight,
                             this, 
                             _1));
    }

    _lightStates.clear();

    bool changed = false;

    if(_lights.size() > 0 && _lights.size() == _oldLights.size())
    {
        for(UInt32 i = 0;i < _lights.size();++i)
        {
            bool light_state = _lights[i].second->getOn();

            if(_lights[i].second->getShadowMode() == Light::CAST_SHADOW_ON)
                light_state = true;
            else if(_lights[i].second->getShadowMode() == Light::CAST_SHADOW_OFF)
                light_state = false;

            _lightStates.push_back(light_state ? 1 : 0);

            if(_lights[i] != _oldLights[i])
                changed = true;
        }
    }
    else
    {
        changed = true;
    }

    if(!changed)
    {
        if(_mapSizeChanged)
            changed = true;
    }

    if(!changed)
    {
        updateLights(action);
        return;
    }

    _mapSizeChanged = false;

    initializeLights(action);
}

void ShadowStage::updateLights(RenderActionBase *action)
{
    SpotLight        *tmpSpot;
    DirectionalLight *tmpDir;
    PointLight       *tmpPoint;
    Matrix            tmpMatrix;
    bool              isSpot, isDirect;
    Real32            sceneWidth = 0.0;
    Real32            sceneHeight = 0.0;
    Real32            PLangle = 0.0;

    for(UInt32 i = 0;i < _lights.size();++i)
    {
        getLightRoot(i)->updateVolume();
        //Giving new Camera Rotation and Position of the light it belongs to
        {
            Quaternion  q;

            tmpMatrix.setIdentity();

            //Is the Lightsource a Spotlight?
            if(_lights[i].second->getType() == SpotLight::getClassType())
            {
                //Casting to Spotlight
                tmpSpot = dynamic_cast<SpotLight *>(_lights[i].second.get());
                FDEBUG(("Found Spotlight!\n"));
                isSpot = true;
                isDirect = false;

                Pnt3f   lightpos = tmpSpot->getPosition();
                Vec3f   lightdir = tmpSpot->getDirection();

                if(tmpSpot->getBeacon() != NULL)
                {
                    Matrix  m = tmpSpot->getBeacon()->getToWorld();
                    m.mult(lightpos, lightpos);
                    m.mult(lightdir, lightdir);
                }

                //<-- ???
                q.setValue(Vec3f(0, 0, -1), lightdir);
                tmpMatrix.setTransform(Vec3f(lightpos), q);
                _realPointLight.push_back(false);
            }
            else if(_lights[i].second->getType() == DirectionalLight::getClassType())
            {
                Vec3f   diff;
                Pnt3f   center;

                tmpDir = 
                    dynamic_cast<DirectionalLight *>(_lights[i].second.get());

                FDEBUG(("Found Directionallight!\n"));
                isSpot = false;
                isDirect = true;

                diff = (getLightRoot(i)->getVolume().getMax() -
                        getLightRoot(i)->getVolume().getMin());

                sceneWidth = diff.length() * 0.5;
                // Not final values. May get tweaked in the future
                sceneHeight = diff.length() * 0.5;

                getLightRoot(i)->getVolume().getCenter(center);

                Vec3f   lightdir = tmpDir->getDirection();
                if(tmpDir->getBeacon() != NULL)
                {
                    Matrix  m = tmpDir->getBeacon()->getToWorld();
                    m.mult(lightdir, lightdir);
                }

                MatrixLookAt(tmpMatrix, center + lightdir,
                             center, Vec3f(0, 1, 0));
                tmpMatrix.invert();
                _realPointLight.push_back(false);
            }
            else
                // Preparation for PointLight -- In this version just a hack
            {
                Vec3f   dir;
                Pnt3f   center;

                tmpPoint = dynamic_cast<PointLight *>(_lights[i].second.get());
                FDEBUG(("Found PointLight!\n"));
                isSpot = false;
                isDirect = false;

                /* Direction of LightCam is a hack. 
                   It always looks at the center of the scene.
                   If the light is placed INSIDE of your Scene
                   (surrounded by Geometry, like a bulb in a Room)
                   the result of the Shadows may be not what you wanted.
                   So, please use the PointLight only outside of your 
                   Scene/outside the Bounding-Box of the lit geometry
                   */

                Pnt3f   lightpos = tmpPoint->getPosition();
                if(tmpPoint->getBeacon() != NULL)
                {
                    Matrix  m = tmpPoint->getBeacon()->getToWorld();
                    m.mult(lightpos, lightpos);
                }

                if((getShadowMode() == STD_SHADOW_MAP ||
                    getShadowMode() == PERSPECTIVE_SHADOW_MAP ||
                    getShadowMode() == DITHER_SHADOW_MAP ||
                    getShadowMode() == PCF_SHADOW_MAP) && _GLSLsupported)
                {
                    //Lightpos inside Scene BB?
                    Pnt3f   sceneMin = getLightRoot(i)->getVolume().getMin();
                    Pnt3f   sceneMax = getLightRoot(i)->getVolume().getMax();

                    if((lightpos[0] < sceneMin[0] ||
                        lightpos[1] < sceneMin[1] ||
                        lightpos[2] < sceneMin[2]) ||
                       (lightpos[0] > sceneMax[0] ||
                        lightpos[1] > sceneMax[1] ||
                        lightpos[2] > sceneMax[2]))
                    {
                        //check if angle is ok to use one Side
                        Vec3f   dist, diff;
                        Pnt3f   center;

                        getLightRoot(i)->getVolume().getCenter(center);

                        //Scene Bounding Box Points

                        Pnt3f   bb[8];
                        bb[0] = Pnt3f(sceneMin[0], sceneMin[1], sceneMin[2]);
                        bb[1] = Pnt3f(sceneMax[0], sceneMin[1], sceneMin[2]);
                        bb[2] = Pnt3f(sceneMax[0], sceneMax[1], sceneMin[2]);
                        bb[3] = Pnt3f(sceneMin[0], sceneMax[1], sceneMin[2]);
                        bb[4] = Pnt3f(sceneMin[0], sceneMin[1], sceneMax[2]);
                        bb[5] = Pnt3f(sceneMax[0], sceneMin[1], sceneMax[2]);
                        bb[6] = Pnt3f(sceneMax[0], sceneMax[1], sceneMax[2]);
                        bb[7] = Pnt3f(sceneMin[0], sceneMax[1], sceneMax[2]);	

                        PLangle = osgDegree2Rad(0);
                        Pnt3f   maxAnglePnt1, maxAnglePnt2;

                        for(UInt32 j = 0;j < 8;j++)
                        {
                            Vec3f   vector1(lightpos - center);
                            Vec3f   vector2(lightpos - bb[j]);
                            Real32  newAngle = vector1.enclosedAngle(vector2);
                            if(osgAbs(osgRad2Degree(newAngle)) > 180)
                                newAngle = vector2.enclosedAngle(vector1);
                            if(osgRad2Degree(newAngle) > osgRad2Degree(PLangle))
                            {
                                PLangle = newAngle;
                                maxAnglePnt1 = bb[j];
                            }
                        }

                        for(UInt32 j = 0;j < 8;j++)
                        {
                            Vec3f   vector1(lightpos - maxAnglePnt1);
                            Vec3f   vector2(lightpos - bb[j]);
                            Real32  newAngle = vector1.enclosedAngle(vector2);
                            if(osgAbs(osgRad2Degree(newAngle)) > 180)
                                newAngle = vector2.enclosedAngle(vector1);
                            if(osgRad2Degree(newAngle) > osgRad2Degree(PLangle))
                            {
                                PLangle = newAngle;
                                maxAnglePnt2 = bb[j];
                            }
                        }
	
                        if(osgRad2Degree(PLangle) < 120) //Use one Side only
                        {
                            getLightRoot(i)->getVolume().getCenter(center);
                            dir = lightpos - center;
                            dir.normalize();
                            dir.negate();
                            q.setValue(Vec3f(0, 0, -1), dir);
                            tmpMatrix.setTransform(Vec3f(lightpos), q);
                            _realPointLight.push_back(false);
                        }
                        else
                            //use 6 side Pointlight
                        {
                            dir = Vec3f(0.0, 0.0, -1.0);//lightpos - center;
                            dir.negate();
                            q.setValue(Vec3f(0, 0, -1), dir);
                            tmpMatrix.setTransform(Vec3f(lightpos), q);
                            _realPointLight.push_back(true);
                        }
                    }
                    else
                    {
                        dir = Vec3f(0.0, 0.0, -1.0);//lightpos - center;
                        dir.negate();
                        q.setValue(Vec3f(0, 0, -1), dir);
                        tmpMatrix.setTransform(Vec3f(lightpos), q);
                        _realPointLight.push_back(true);
                    }
                }
                else
                {

                    Vec3f   dist, diff;
                    Pnt3f   center;
                    Pnt3f   sceneMin = getLightRoot(i)->getVolume().getMin();
                    Pnt3f   sceneMax = getLightRoot(i)->getVolume().getMax();

                    getLightRoot(i)->getVolume().getCenter(center);

                    Pnt3f   bb[8];
                    bb[0] = Pnt3f(sceneMin[0], sceneMin[1], sceneMin[2]);
                    bb[1] = Pnt3f(sceneMax[0], sceneMin[1], sceneMin[2]);
                    bb[2] = Pnt3f(sceneMax[0], sceneMax[1], sceneMin[2]);
                    bb[3] = Pnt3f(sceneMin[0], sceneMax[1], sceneMin[2]);
                    bb[4] = Pnt3f(sceneMin[0], sceneMin[1], sceneMax[2]);
                    bb[5] = Pnt3f(sceneMax[0], sceneMin[1], sceneMax[2]);
                    bb[6] = Pnt3f(sceneMax[0], sceneMax[1], sceneMax[2]);
                    bb[7] = Pnt3f(sceneMin[0], sceneMax[1], sceneMax[2]);

                    PLangle = osgDegree2Rad(0);
                    Pnt3f   maxAnglePnt1, maxAnglePnt2;

                    for(UInt32 j = 0;j < 8;j++)
                    {
                        Vec3f   vector1(lightpos - center);
                        Vec3f   vector2(lightpos - bb[j]);
                        Real32  newAngle = vector1.enclosedAngle(vector2);
                        if(osgAbs(osgRad2Degree(newAngle)) > 180)
                            newAngle = vector2.enclosedAngle(vector1);
                        if(osgRad2Degree(newAngle) > osgRad2Degree(PLangle))
                        {
                            PLangle = newAngle;
                            maxAnglePnt1 = bb[j];
                        }
                    }

                    for(UInt32 j = 0;j < 8;j++)
                    {
                        Vec3f   vector1(lightpos - maxAnglePnt1);
                        Vec3f   vector2(lightpos - bb[j]);
                        Real32  newAngle = vector1.enclosedAngle(vector2);
                        if(osgAbs(osgRad2Degree(newAngle)) > 180)
                            newAngle = vector2.enclosedAngle(vector1);
                        if(osgRad2Degree(newAngle) > osgRad2Degree(PLangle))
                        {
                            PLangle = newAngle;
                            maxAnglePnt2 = bb[j];
                        }
                    }

                    if(osgRad2Degree(PLangle) > 175) //Use one Side only
                    {
                        PLangle = osgDegree2Rad(175);
                    }

                    dir = lightpos - center;
                    dir.normalize();
                    dir.negate();

                    q.setValue(Vec3f(0, 0, -1), dir);

                    tmpMatrix.setTransform(Vec3f(lightpos), q);
                    _realPointLight.push_back(false);
                }
            }

            _lightCamTrans[i]->setMatrix(tmpMatrix);
        }

        //Feeding new Camera with paramters of original camera
        {
            // Is the Lightsource a Spotlight?
            if(isSpot)
            {
                tmpSpot = dynamic_cast<SpotLight *>(_lights[i].second.get());

                Pnt3f   lightpos = tmpSpot->getPosition();
                if(tmpSpot->getBeacon() != NULL)
                {
                    Matrix  m = tmpSpot->getBeacon()->getToWorld();
                    m.mult(lightpos, lightpos);
                }

                Pnt3f   center;
                getLightRoot(i)->getVolume().getCenter(center);
                
                Vec3f   dir = lightpos - center;
                Real64  dirLength = dir.length();

                Vec3f   diff = (getLightRoot(i)->getVolume().getMax() -
                                getLightRoot(i)->getVolume().getMin());
                Real64  diffLength = diff.length() / 2;
                
                Real64  zNearLimit, zCalcNear = 0;
                Real64  zFar = 1.2 * (dirLength + diffLength);
                Real64  zNear = action->getCamera()->getNear();

                if (diffLength) 
                {
                    // Camera outside of the scene
                    if (dirLength > diffLength)      
                        // Camera outside of the scene
                        zCalcNear = 0.8 * (dirLength - diffLength);
                    // else camera inside of the scene
                    
                    zNear = zCalcNear;
                    zNearLimit = osgMax( 0.01, zFar / 20000 );
                    zNear = osgMax( zNear, zNearLimit );
                }
                
                _lightCameras[i]->setNear( zNear );
                _lightCameras[i]->setFar( zFar );
                
                //Using Spot-angle of Spotlight as FOV for LightCamera
                dynamic_cast<PerspectiveCamera *>(_lightCameras[i].get())->
                    setFov(tmpSpot->getSpotCutOffDeg() * 2);
            }
                // Is the Lightsource a Directional-Light? Setting up MatrixCamera
            else if(isDirect)
            {
                Matrix  proMatrix, modMatrix;

                proMatrix.setIdentity();
                modMatrix.setIdentity();

                MatrixOrthogonal(proMatrix, -sceneWidth,
                                 sceneWidth, -sceneHeight,
                                 sceneHeight, -sceneWidth, sceneWidth);

                // Grabbing ModelView-Matrix from Light-Transformation
                modMatrix = _lightCamTrans[i]->getMatrix();

                dynamic_cast<MatrixCamera *>(_lightCameras[i].get())->setProjectionMatrix(proMatrix);
                dynamic_cast<MatrixCamera *>(_lightCameras[i].get())->setModelviewMatrix(
                    modMatrix);
            }
            else
                // If none of above the Lightsource must be a PointLight
            {
                if((getShadowMode() == STD_SHADOW_MAP ||
                    getShadowMode() == PERSPECTIVE_SHADOW_MAP ||
                    getShadowMode() == DITHER_SHADOW_MAP ||
                    getShadowMode() == PCF_SHADOW_MAP) && _realPointLight[i] &&
                   _GLSLsupported)
                {
                    Vec3f   dist, diff;
                    Pnt3f   center;
                    Real32  angle;

                    getLightRoot(i)->getVolume().getCenter(center);

                    Pnt3f   lightpos = tmpPoint->getPosition();

                    if(tmpPoint->getBeacon() != NULL)
                    {
                        Matrix  m = tmpPoint->getBeacon()->getToWorld();
                        m.mult(lightpos, lightpos);
                    }

                    dist = (lightpos - center);
                    diff = (getLightRoot(i)->getVolume().getMax() -
                            getLightRoot(i)->getVolume().getMin());

                    Real32  distLength = dist.length();
                    Real32  diffLength = diff.length();

                    _lightCameras[i]->setNear(0.01f);
                    _lightCameras[i]->setFar(distLength + diffLength);

                    dynamic_cast<PerspectiveCamera *>(_lightCameras[i].get())->setFov(
                        osgDegree2Rad(91));
                }
                else
                {
                    Vec3f   dist, diff;
                    Pnt3f   center;
                    getLightRoot(i)->getVolume().getCenter(center);

                    Pnt3f   lightpos = tmpPoint->getPosition();

                    if(tmpPoint->getBeacon() != NULL)
                    {
                        Matrix  m = tmpPoint->getBeacon()->getToWorld();
                        m.mult(lightpos, lightpos);
                    }

                    dist = (lightpos - center);
                    diff = (getLightRoot(i)->getVolume().getMax() -
                            getLightRoot(i)->getVolume().getMin());

                    Pnt3f   sceneMin = getLightRoot(i)->getVolume().getMin();
                    Pnt3f   sceneMax = getLightRoot(i)->getVolume().getMax();

                    Real64  distLength = dist.length();
                    Real64  diffLength = diff.length() / 2;
                    
                    Real64  zNearLimit, zCalcNear = 0;
                    Real64  zFar = 1.2 * (distLength + diffLength);
                    Real64  zNear = action->getCamera()->getNear();
                    if (diffLength) 
                    {
                        // Camera outside of the scene
                        if (distLength > diffLength)      
                            // Camera outside of the scene
                            zCalcNear = 0.8 * (distLength - diffLength);
                        // else camera inside of the scene
                        
                        zNear = zCalcNear;
                        zNearLimit = osgMax( 0.01, zFar / 20000 );
                        zNear = osgMax( zNear, zNearLimit );
                    }
                    
                    _lightCameras[i]->setNear( zNear );
                    _lightCameras[i]->setFar( zFar );

                    dynamic_cast<PerspectiveCamera *>(_lightCameras[i].get())->setFov(
                        PLangle);
                }
            }

            _lightCameras[i]->setBeacon(_lightCamBeacons[i]);
        }
    }
}

void ShadowStage::initializeLights(RenderActionBase *action)
{
    clearLights(_oldLights.size());

    FDEBUG(("Initialising lights.\n"));

    _oldLights = _lights;

    //Setting up Light-Cameras, ShadowMaps and TextureChunks
    for(UInt32 i = 0;i < _lights.size();++i)
    {
        bool light_state = _lights[i].second->getOn();
        if(_lights[i].second->getShadowMode() == Light::CAST_SHADOW_ON)
            light_state = true;
        else if(_lights[i].second->getShadowMode() == Light::CAST_SHADOW_OFF)
            light_state = false;

        // Remembering initial state of Lights
        _lightStates.push_back(light_state ? 1 : 0);
        //Fill Transformation-List, so it can be used later on
        _lightCamTrans.push_back(NULL);
        //Creation of Lightcam-Beacon
        _lightCamBeacons.push_back(makeCoredNode<Transform>
                                   (&_lightCamTrans[i]));

        //Giving new Camera Rotation and Position of the light it belongs to
        {
            //Is the Lightsource a Spotlight?
            if(_lights[i].second->getType() == SpotLight::getClassType())
            {
                //Creation of new Perspective-LightCam
                _lightCameras.push_back(PerspectiveCamera::create());
            }
            else if(_lights[i].second->getType() == DirectionalLight::getClassType())
            {
                _lightCameras.push_back(MatrixCamera::create());
            }
            else
                // Preparation for PointLight -- In this version just a hack
            {
                _lightCameras.push_back(PerspectiveCamera::create());
            }
        }

        if(getSceneRoot() == NULL)
        {
            SFATAL << "RootNode not found!" << endLog;
        }
        else
        {
            getSceneRoot()->addChild(_lightCamBeacons[i]);
        }

        //----------Shadowtexture-Images and Texture-Chunks-----------

        if(_lights[i].second->getType() != PointLight::getClassType())
        {
            _shadowImages.push_back(Image::create());

            // creates a image without allocating main memory.
            _shadowImages[i]->set(Image::OSG_L_PF, getMapSize(), getMapSize(),
                                  1, 1, 1, 0, NULL,
                                  Image::OSG_UINT8_IMAGEDATA, false);

            _texChunks.push_back(TextureChunk::create());

            // Preparation of Texture be a Depth-Texture
            {
                _texChunks[i]->setImage(_shadowImages[i]);
                _texChunks[i]->setInternalFormat(GL_DEPTH_COMPONENT_ARB);
                _texChunks[i]->setExternalFormat(GL_DEPTH_COMPONENT_ARB);
                _texChunks[i]->setMinFilter(GL_LINEAR);
                _texChunks[i]->setMagFilter(GL_LINEAR);
                _texChunks[i]->setWrapS(GL_CLAMP_TO_EDGE);
                _texChunks[i]->setWrapT(GL_CLAMP_TO_EDGE);
                _texChunks[i]->setEnvMode(GL_MODULATE);
                _texChunks[i]->setTarget(GL_TEXTURE_2D);
            }
        }
        else
            //Light is a point light
        {
            //TODO: Texturgr�se anpassen, je nach Bedarf
            GLint   max_texture_size;
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

            _shadowImages.push_back(Image::create());

            // creates a image without allocating main memory.
            if((getShadowMode() == STD_SHADOW_MAP ||
                getShadowMode() == PERSPECTIVE_SHADOW_MAP ||
                getShadowMode() == DITHER_SHADOW_MAP ||
                getShadowMode() == PCF_SHADOW_MAP) && _GLSLsupported)
            {
                _shadowImages[i]->set(Image::OSG_L_PF, getMapSize(),
                                      getMapSize(),
                                      1, 1, 1, 0, NULL,
                                      Image::OSG_UINT8_IMAGEDATA, false);
            }
            else
            {
                _shadowImages[i]->set(Image::OSG_L_PF, getMapSize(),
                                      getMapSize(),
                                      1, 1, 1, 0, NULL,
                                      Image::OSG_UINT8_IMAGEDATA, false);
            }

            _texChunks.push_back(TextureChunk::create());

            // Preparation of Texture be a Depth-Texture
            {
                _texChunks[i]->setImage(_shadowImages[i]);
                _texChunks[i]->setInternalFormat(GL_DEPTH_COMPONENT_ARB);
                _texChunks[i]->setExternalFormat(GL_DEPTH_COMPONENT_ARB);
                _texChunks[i]->setMinFilter(GL_LINEAR);
                _texChunks[i]->setMagFilter(GL_LINEAR);
                _texChunks[i]->setWrapS(GL_CLAMP_TO_BORDER_ARB);
                _texChunks[i]->setWrapT(GL_CLAMP_TO_BORDER_ARB);
                _texChunks[i]->setEnvMode(GL_MODULATE);
                _texChunks[i]->setTarget(GL_TEXTURE_2D);
            }
        }

    }

    updateLights(action);
}

void ShadowStage::clearLights(UInt32 size)
{
    if(size > 0)
    {
        FDEBUG(("Clearing Lightcamera-Garbage!\n"));

        for(UInt32 i = 0;i < size;++i)
        {
            if(i < _lightCamBeacons.size())
                getSceneRoot()->subChild(_lightCamBeacons[i]);

            if(i < _lightCameras.size())
                _lightCameras[i] = NULL;

            if(i < _texChunks.size())
                _texChunks[i] = NULL;
        }

        _lightCameras.clear();
        _lightCamTrans.clear();
        _lightCamBeacons.clear();
        _lightStates.clear();
        _texChunks.clear();
        _shadowImages.clear();
    }
}

void ShadowStage::renderLight(RenderActionBase *action, 
                                 Material *mat, 
                                 UInt32 index)
{
    RenderAction *ract = dynamic_cast<RenderAction *>(action);

    if(_light_render_transform == NULL)
    {
        _light_render_transform = Node::create();
        _light_render_transform->setCore(Transform::create());
    }

    NodeUnrecPtr  light = _lights[index].first;
    Node         *parent = light->getParent();

    if(parent != NULL)
    {
        Transform *trans = 
            dynamic_cast<Transform *>(_light_render_transform->getCore());

        trans->setMatrix(parent->getToWorld());

        _light_render_transform->addChild(light);

        // ok we render only one unlit material for the whole scene in this pass.
        ract->setGlobalOverride(mat);

        // disable color mask we only need the depth values!
        action->apply(_light_render_transform);

        parent->addChild(light);
        // reset material.
        ract->setGlobalOverride(NULL);
    }
    else
    {
        // ok we render only one unlit material for the whole scene in this pass.
        ract->setGlobalOverride(mat);

        action->apply(light);
        // reset material.

        ract->setGlobalOverride(NULL);
    }

}

Node *ShadowStage::getLightRoot(UInt32 index)
{
    // return getSceneRoot();
    return _lights[index].first;
}

void ShadowStage::checkLightsOcclusion(RenderActionBase *action)
{
    if ( !getDisableOccludedLights() )
    {
        return;
    }
    
    RenderAction *ract = dynamic_cast<RenderAction *>(action);

    if (ract == NULL)
        return;

    for(UInt32 i = 0;i < _lights.size();i++)
    {
        Node *node = _lights[i].first;

        // now make a occlusion test for all light sources.
        bool occluded = false;

        if(glGenQueriesARB != NULL) // occlusion queries supported?
        {
            Matrix view;
            if(ract->getCamera() != NULL)
            {
                ract->getCamera()->getViewing(
                    view,
                    ract->getViewport()->getPixelWidth (),
                    ract->getViewport()->getPixelHeight());
            }

            BoxVolume     vol = node->getVolume();

            Matrix m = view;
            if(node->getParent() != NULL)
                m.mult(node->getParent()->getToWorld());
            vol.transform(m);

            // ignore objects behind the camera.
            if(vol.getMax()[2] < 0.0f)
            {
                glDepthMask(GL_FALSE);
                glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
    
                if(this->_occlusionQuery == 0)
                    glGenQueriesARB(1, &this->_occlusionQuery);
    
                const BoxVolume    & vol = node->getVolume();

                Pnt3f min,max;
                vol.getBounds(min, max);
    
                glBeginQueryARB(GL_SAMPLES_PASSED_ARB, 
                                this->_occlusionQuery);
                this->drawOcclusionBB(min, max);
                glEndQueryARB(GL_SAMPLES_PASSED_ARB);
    
                glDepthMask(GL_TRUE);
                glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    
                GLuint pixels = 0;
                glGetQueryObjectuivARB(this->_occlusionQuery, 
                                       GL_QUERY_RESULT_ARB, &pixels);
    
                occluded = (pixels == 0);
            }
        }

        if(occluded)
        {
            _lightStates[i] = 0;
        }
        else
        {
            bool light_state = _lights[i].second->getOn();
            if(_lights[i].second->getShadowMode() == Light::CAST_SHADOW_ON)
                light_state = true;
            else if(_lights[i].second->getShadowMode() == Light::CAST_SHADOW_OFF)
                light_state = false;

            _lightStates[i] = (light_state ? 1 : 0);
        }
    }

    //updateLights();
}

void ShadowStage::setReadBuffer(void)
{
#ifdef SHADOW_CHECK
    if(getLeftBuffer())
    {
        if(getRightBuffer())
        {
            glReadBuffer(GL_BACK);
        }
        else
        {
            glReadBuffer(GL_BACK_LEFT);
        }
    }
    else
    {
        if(getRightBuffer())
        {
            glReadBuffer(GL_BACK_RIGHT);
        }
        else
        {
            glReadBuffer(GL_NONE);
        }
    }
#else
    glReadBuffer(GL_BACK);
#endif
}

void ShadowStage::drawOcclusionBB(const Pnt3f &bbmin, const Pnt3f &bbmax)
{
#if 1

#if 0
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f( bbmin[0], bbmin[1], bbmax[2]); // 0
    glVertex3f( bbmax[0], bbmin[1], bbmax[2]); // 1
    glVertex3f( bbmin[0], bbmax[1], bbmax[2]); // 2
    glVertex3f( bbmax[0], bbmax[1], bbmax[2]); // 3
    glVertex3f( bbmin[0], bbmax[1], bbmin[2]); // 4
    glVertex3f( bbmax[0], bbmax[1], bbmin[2]); // 5
    glVertex3f( bbmin[0], bbmin[1], bbmin[2]); // 6
    glVertex3f( bbmax[0], bbmin[1], bbmin[2]); // 7
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    glVertex3f( bbmax[0], bbmax[1], bbmin[2]); // 5
    glVertex3f( bbmax[0], bbmax[1], bbmax[2]); // 3
    glVertex3f( bbmax[0], bbmin[1], bbmin[2]); // 7
    glVertex3f( bbmax[0], bbmin[1], bbmax[2]); // 1
    glVertex3f( bbmin[0], bbmin[1], bbmin[2]); // 6
    glVertex3f( bbmin[0], bbmin[1], bbmax[2]); // 0
    glVertex3f( bbmin[0], bbmax[1], bbmin[2]); // 4
    glVertex3f( bbmin[0], bbmax[1], bbmax[2]); // 2
    glEnd();

#else

    // not sure if this is faster but it reduces the amount of
    // gl commands.
    const GLubyte indices[16] = { 0,1,2,3,4,5,6,7,  5,3,7,1,6,0,4,2 };
    Real32 vertices[24];
    vertices[0]  = bbmin[0]; vertices[1]  = bbmin[1]; vertices[2]  = bbmax[2];
    vertices[3]  = bbmax[0]; vertices[4]  = bbmin[1]; vertices[5]  = bbmax[2];
    vertices[6]  = bbmin[0]; vertices[7]  = bbmax[1]; vertices[8]  = bbmax[2];
    vertices[9]  = bbmax[0]; vertices[10] = bbmax[1]; vertices[11] = bbmax[2];
    vertices[12] = bbmin[0]; vertices[13] = bbmax[1]; vertices[14] = bbmin[2];
    vertices[15] = bbmax[0]; vertices[16] = bbmax[1]; vertices[17] = bbmin[2];
    vertices[18] = bbmin[0]; vertices[19] = bbmin[1]; vertices[20] = bbmin[2];
    vertices[21] = bbmax[0]; vertices[22] = bbmin[1]; vertices[23] = bbmin[2];

    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_BYTE, &indices[0]);
    glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_BYTE, &indices[8]);
    glDisableClientState(GL_VERTEX_ARRAY);

#endif

#else

    if(_occ_bb_dl == 0)
    {
        // we create a display list for rendering the occlusion
        // bounding box.
        // is this faster ??? need to check it amz.
        Pnt3f min(-0.5f, -0.5f, -0.5f);
        Pnt3f max(0.5f, 0.5f, 0.5f);
        _occ_bb_dl = glGenLists(1);

        const GLubyte indices[16] = { 0,1,2,3,4,5,6,7,  5,3,7,1,6,0,4,2 };
        Real32 vertices[24];
        vertices[0]  = min[0]; vertices[1]  = min[1]; vertices[2]  = max[2];
        vertices[3]  = max[0]; vertices[4]  = min[1]; vertices[5]  = max[2];
        vertices[6]  = min[0]; vertices[7]  = max[1]; vertices[8]  = max[2];
        vertices[9]  = max[0]; vertices[10] = max[1]; vertices[11] = max[2];
        vertices[12] = min[0]; vertices[13] = max[1]; vertices[14] = min[2];
        vertices[15] = max[0]; vertices[16] = max[1]; vertices[17] = min[2];
        vertices[18] = min[0]; vertices[19] = min[1]; vertices[20] = min[2];
        vertices[21] = max[0]; vertices[22] = min[1]; vertices[23] = min[2];

        glNewList(_occ_bb_dl, GL_COMPILE);
            glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
            glEnableClientState(GL_VERTEX_ARRAY);
            glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_BYTE, &indices[0]);
            glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_BYTE, &indices[8]);
            glDisableClientState(GL_VERTEX_ARRAY);
        glEndList();
    }

    Vec3f s = bbmax - bbmin;
    Vec3f t = bbmin + bbmax;
    t *= 0.5f;
    glPushMatrix();
        glTranslatef(t[0], t[1], t[2]);
        glScalef(s[0], s[1], s[2]);
        glCallList(_occ_bb_dl);
    glPopMatrix();

#endif
}
