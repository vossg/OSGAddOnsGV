#include <stdlib.h>
#include <stdio.h>
#include <OSGConfig.h>
#include <OSGQuaternion.h>
#include <OSGRenderAction.h>
#include <OSGMatrix.h>
#include <OSGMatrixUtility.h>
#include <OSGBackground.h>
#include <OSGForeground.h>
#include <OSGGrabForeground.h>
#include <OSGPolygonForeground.h>
#include <OSGPolygonBackground.h>
#include <OSGTextureGrabForeground.h>
#include <OSGFileGrabForeground.h>
#include <OSGImageForeground.h>
#include <OSGImage.h>
#include <OSGGeometry.h>
#include <OSGSimpleGeometry.h>
#include <OSGLight.h>
#include <OSGMaterialGroup.h>
#include "OSGPCSSShadowMapHandler.h"
#include "OSGShadowStage.h"
#include "OSGTreeHandler.h"
#include "OSGGLU.h"


#include "OSGRenderBuffer.h"
#include "OSGTextureBuffer.h"

OSG_BEGIN_NAMESPACE

#include "ShaderCode/OSGPCSSShadowMapShaderCode.cinl"

PCSSShadowMapHandler::PCSSShadowMapHandler(ShadowStage *source) :
    TreeHandler(source),
    _tiledeco(NULL),
    _colorMapImage(NULL),
    _shadowFactorMapImage(NULL),
    _shadowSHL(NULL),
    _firstRun(1),
    _initTexturesDone(false)
{
    _tiledeco = NULL;

    //Prepare Color Map grabbing
    _colorMapO = TextureObjChunk::create();
    _colorMapImage = Image::create();

    _colorMapO->setImage(_colorMapImage);
    _colorMapO->setInternalFormat(GL_RGB);
    _colorMapO->setExternalFormat(GL_RGB);
    _colorMapO->setMinFilter(GL_NEAREST);
    _colorMapO->setMagFilter(GL_NEAREST);
    _colorMapO->setWrapS(GL_REPEAT);
    _colorMapO->setWrapT(GL_REPEAT);
    _colorMapO->setTarget(GL_TEXTURE_2D);

    //Prepare Shadow Factor Map grabbing
    _shadowFactorMapO = TextureObjChunk::create();
    _shadowFactorMapImage = Image::create();

    _shadowFactorMapO->setImage(_shadowFactorMapImage);
    _shadowFactorMapO->setInternalFormat(GL_RGB);
    _shadowFactorMapO->setExternalFormat(GL_RGB);
    _shadowFactorMapO->setMinFilter(GL_LINEAR);
    _shadowFactorMapO->setMagFilter(GL_LINEAR);
    _shadowFactorMapO->setWrapS(GL_REPEAT);
    _shadowFactorMapO->setWrapT(GL_REPEAT);
    _shadowFactorMapO->setTarget(GL_TEXTURE_2D);


    //SHL Chunk 1

    _shadowSHL = SHLChunk::create();
    //_shadowSHL->readVertexProgram("PCSS_Shadow.vert");
    //_shadowSHL->readFragmentProgram("PCSS_Shadow.frag");
    _shadowSHL->setVertexProgram(_pcss_shadow_vp);
    _shadowSHL->setFragmentProgram(_pcss_shadow_fp);

    //SHL Chunk 2
    _combineSHL = SHLChunk::create();

    //_combineSHL->readVertexProgram("PCSS_Shadow_combine.vert");
    //_combineSHL->readFragmentProgram("PCSS_Shadow_combine.frag");
    _combineSHL->setVertexProgram(_shadow_combine_vp);
    _combineSHL->setFragmentProgram(_shadow_combine_fp);

    _combineDepth = DepthChunk::create();
        _combineDepth->setReadOnly(true);

    //Shadow Shader

    //Combine Shader
    _combineCmat = ChunkMaterial::create();
    _combineCmat->addChunk(_combineSHL);
    _combineCmat->addChunk(_colorMapO);
    _combineCmat->addChunk(_shadowFactorMapO);
    _combineCmat->addChunk(_combineDepth);

    PolygonChunkUnrecPtr pPoly = PolygonChunk::create();

    pPoly->setOffsetFill  (true                     );
    pPoly->setOffsetFactor(_shadowVP->getOffFactor());
    pPoly->setOffsetBias  (_shadowVP->getOffBias  ());

    _unlitMat->addChunk(pPoly);
}

PCSSShadowMapHandler::~PCSSShadowMapHandler(void)
{
    _tiledeco        = NULL;

    _shadowSHL       = NULL;
    _combineSHL      = NULL;
    _combineDepth    = NULL;
    _combineCmat     = NULL;

    _vShadowCmat  .clear();
    _vShadowSHLVar.clear();
}

/// Checks if FBO status is ok
bool PCSSShadowMapHandler::checkFrameBufferStatus(Window *win)
{
    GLenum  errCode, status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

    switch(status)
    {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            FINFO(("%x: framebuffer complete!\n", status));
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            FWARNING(("%x: framebuffer GL_FRAMEBUFFER_UNSUPPORTED_EXT\n",
                      status));
            // choose different formats
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            FWARNING(("%x: framebuffer INCOMPLETE_ATTACHMENT\n", status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            FWARNING(("%x: framebuffer FRAMEBUFFER_MISSING_ATTACHMENT\n",
                      status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            FWARNING(("%x: framebuffer FRAMEBUFFER_DIMENSIONS\n", status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            FWARNING(("%x: framebuffer INCOMPLETE_FORMATS\n", status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            FWARNING(("%x: framebuffer INCOMPLETE_DRAW_BUFFER\n", status));
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            FWARNING(("%x: framebuffer INCOMPLETE_READ_BUFFER\n", status));
            break;
        case GL_FRAMEBUFFER_BINDING_EXT:
            FWARNING(("%x: framebuffer BINDING_EXT\n", status));
            break;
        default:
            return false;
    }

    if((errCode = glGetError()) != GL_NO_ERROR)
    {
        const GLubyte   *errString = gluErrorString(errCode);
        FWARNING(("OpenGL Error: %s!\n", errString));
        return false;
    }
    return true;
}


bool PCSSShadowMapHandler::initFBO(DrawEnv *pEnv)
{
    if(_useFBO)
    {
        Int32   width  = pEnv->getPixelWidth();
        Int32   height = pEnv->getPixelHeight();

        if(width <= 0 || height <= 0)
            return false;

        if(_pFB != NULL)
            return true;

        Window *win = pEnv->getWindow();

        _width  = pEnv->getPixelWidth();
        _height = pEnv->getPixelHeight();

        _colorMapImage->set(GL_RGB, _width, _height);

        _shadowFactorMapImage->set(GL_RGB, _width, _height);

        commitChanges();


        _pFB = FrameBufferObject::create();
        
        _pFB->setSize(_width, _height);

        RenderBufferUnrecPtr pDepthRB = RenderBuffer::create();
        
        pDepthRB->setInternalFormat(GL_DEPTH_COMPONENT24);



        TextureBufferUnrecPtr pTexBuffer = TextureBuffer::create();

        pTexBuffer->setTexture(_colorMapO);

        _pFB->setColorAttachment(pTexBuffer, 0);


        pTexBuffer = TextureBuffer::create();

        pTexBuffer->setTexture(_shadowFactorMapO);

        _pFB->setColorAttachment(pTexBuffer, 1);



        _pFB->setDepthAttachment(pDepthRB);

        commitChanges();
    }

    return true;
}

void PCSSShadowMapHandler::reInit(DrawEnv *pEnv)
{
}

void PCSSShadowMapHandler::initTextures(DrawEnv *pEnv)
{
    _initTexturesDone = true;

    Int32   width = pEnv->getPixelWidth();
    Int32   height = pEnv->getPixelHeight();

    //if no NPOTTextures supported, resize images
    if(!_useNPOTTextures)
    {
        if(width > height)
            _widthHeightPOT = osgNextPower2(width - 1);
        else
            _widthHeightPOT = osgNextPower2(height - 1);

        _colorMapImage->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);

        _shadowFactorMapImage->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);
    }
}

void PCSSShadowMapHandler::createShadowMapsFBO(DrawEnv *pEnv, 
                                              RenderAction *pTmpAction)
{
    RenderAction *a = dynamic_cast<RenderAction *>(pEnv->getAction());

#ifdef SHADOWCHECK
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_TRUE);
#endif

    // disable all lights more speed
    std::vector<bool> lightStates;
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // store old states.
        lightStates.push_back(_shadowVP->_lights[i].second->getOn());
        _shadowVP->_lights[i].second->setOn(false);
    }

    // deactivate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            exnode->setTravMask(0);
    }

    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
            {
                a->pushPartition();
                {
                    RenderPartition   *pPart    = a->getActivePartition();
                    
                    pPart->setRenderTarget(_shadowVP->_vTexChunks[i].pFBO);
                    
                    pPart->setWindow  (a->getWindow());
                    
                    pPart->calcViewportDimension(0.f,
                                                 0.f,
                                                 _shadowVP->getMapSize(),
                                                 _shadowVP->getMapSize(),
                                                 
                                                 _shadowVP->getMapSize(),
                                                 _shadowVP->getMapSize() );
                    
                    
                    Matrix m, t;
                    
                    // set the projection
                    _shadowVP->_lightCameras[i]->getProjection          (
                        m, 
                        pPart->getViewportWidth (), 
                        pPart->getViewportHeight());
                    
                    _shadowVP->_lightCameras[i]->getProjectionTranslation(
                        t, 
                        pPart->getViewportWidth (), 
                        pPart->getViewportHeight());
                    
                    pPart->setupProjection(m, t);
                    
                    _shadowVP->_lightCameras[i]->getViewing(
                        m, 
                        pPart->getViewportWidth (),
                        pPart->getViewportHeight());
                    
                    
                    pPart->setupViewing(m);
                    
                    pPart->setNear     (
                        _shadowVP->_lightCameras[i]->getNear());
                    pPart->setFar      (
                        _shadowVP->_lightCameras[i]->getFar ());
                    
                    pPart->calcFrustum();
                    
                    pPart->setBackground(_pClearBackground);
                    
                    Node *light  = _shadowVP->_lights[i].first;
                    Node *parent =  light->getParent();
                    
                    if(parent != NULL)
                    {
                        a->pushMatrix(parent->getToWorld());
                    }
                    
                    
                    a->overrideMaterial(_unlitMat, a->getActNode());
                    a->recurse(light);
                    a->overrideMaterial( NULL,       a->getActNode());
                    
                    if(parent != NULL)
                    {
                        a->popMatrix();
                    }
                }
                a->popPartition();
            }
        }
    }

    //-------Restoring old states of Window and Viewport----------
    // enable all lights.
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // restore old states.
        _shadowVP->_lights[i].second->setOn(lightStates[i]);
    }

    // activate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            if(_shadowVP->_excludeNodeActive[i])
                exnode->setTravMask(TypeTraits<UInt32>::BitsSet);
    }


#ifdef SHADOWCHECK
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
#endif
}


void PCSSShadowMapHandler::createColorMapFBO(DrawEnv *pEnv,
                                             RenderAction *pTmpAction)
{
    RenderAction *a = dynamic_cast<RenderAction *>(pEnv->getAction());

    a->pushPartition((RenderPartition::CopyWindow      |
                      RenderPartition::CopyViewing     |
                      RenderPartition::CopyProjection  |
                      RenderPartition::CopyFrustum     |
                      RenderPartition::CopyNearFar     |
                      RenderPartition::CopyViewportSize),
                     RenderPartition::StateSorting);
    {
        RenderPartition *pPart = a->getActivePartition();

        pPart->setRenderTarget(_pFB);
        pPart->setDrawBuffer  (GL_COLOR_ATTACHMENT0_EXT);

        Node *parent = _shadowVP->getSceneRoot()->getParent();

        if(parent != NULL)
        {
            a->pushMatrix(parent->getToWorld());
        }
        
        pPart->setBackground(a->getBackground());

        a->recurse(_shadowVP->getSceneRoot());

        if(parent != NULL)
        {
            a->popMatrix();
        }
    }
    a->popPartition();


}

void PCSSShadowMapHandler::createShadowFactorMapFBO(
    DrawEnv      *pEnv,
    RenderAction *pTmpAction,
    UInt32        num,
    UInt32        uiActiveLightCount)
{
    //Finde alle aktiven Lichtquellen

    Real32  activeLights = 0;

    if(_shadowVP->getGlobalShadowIntensity() != 0.0)
    {
        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            if(_shadowVP->_lightStates[i] != 0)
                activeLights++;
        }
    }
    else
    {
        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            if(_shadowVP->_lightStates[i] != 0 &&
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
                activeLights++;
        }
    }

    Real32  shadowIntensity;

    if(_shadowVP->getGlobalShadowIntensity() != 0.0)
    {
        shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                           activeLights);
    }
    else
    {
        shadowIntensity = 
            (_shadowVP->_lights[num].second->getShadowIntensity() /
             activeLights);
    }

    if(_shadowVP->_lights[num].second->getShadowIntensity() != 0.0 ||
       _shadowVP->getGlobalShadowIntensity() != 0.0)
    {

        Matrix  LVM, LPM, CVM;

        _shadowVP->_lightCameras[num]->getViewing(
            LVM,
            pEnv->getPixelWidth(),
            pEnv->getPixelHeight());

        _shadowVP->_lightCameras[num]->getProjection(
            LPM,
            pEnv->getPixelWidth(),
            pEnv->getPixelHeight());

#if 0
        _shadowVP->getCamera()->getViewing(
            CVM, 
            _shadowVP->getPixelWidth(),
            _shadowVP->getPixelHeight());
#endif

        pEnv->getAction()->getCamera()->getViewing(
            CVM, 
            pEnv->getPixelWidth(),
            pEnv->getPixelHeight());

        Matrix  iCVM = CVM;
        iCVM.invert();

        Real32  texFactor;

        if(_shadowVP->_lights[num].second->getType() == 
                                                 SpotLight::getClassType () ||
           _shadowVP->_lights[num].second->getType() == 
                                                 PointLight::getClassType()  )
        {
            texFactor = Real32(_width) / Real32(_height);
        }
        else
        {
            texFactor = 1.0;
        }

        Matrix  shadowMatrix = LPM;
        shadowMatrix.mult(LVM);
        shadowMatrix.mult(iCVM);

        Real32  xFactor = 1.0;
        Real32  yFactor = 1.0;

        if(!_useNPOTTextures)
        {
            xFactor = Real32(_width) / Real32(_widthHeightPOT);
            yFactor = Real32(_height) / Real32(_widthHeightPOT);
        }

        Real32  lightSize;
        if(_shadowVP->_lights[num].second->getType() != 
                                             DirectionalLight::getClassType())
        {
            lightSize = _shadowVP->getShadowSmoothness() * 10.0;
        }
        else
        {
            lightSize = _shadowVP->getShadowSmoothness() / 25.0;
        }

        if(_vShadowCmat.size() == uiActiveLightCount)
        {
            _vShadowCmat.push_back(ChunkMaterial::create());
        }
        
        OSG_ASSERT( uiActiveLightCount < _vShadowCmat.size());

        if(_vShadowSHLVar.size() == uiActiveLightCount)
        {
            _vShadowSHLVar.push_back(SHLVariableChunk::create());

            _vShadowSHLVar[uiActiveLightCount]->setSHLChunk(_shadowSHL);
        }


        _shadowSHL->addUniformVariable("shadowMap",    0);
        _shadowSHL->addUniformVariable("oldFactorMap", 1);

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "firstRun", (uiActiveLightCount == 0) ? Int32(1) : Int32(0));

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "intensity", shadowIntensity);

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "texFactor", texFactor);

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "lightPM", shadowMatrix);

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "mapSize",
            Real32(_shadowVP->getMapSize()));

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "lightSize", Real32(lightSize));

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "xFactor", Real32(xFactor));

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "yFactor", Real32(yFactor));



        _vShadowCmat[uiActiveLightCount]->clearChunks();

        _vShadowCmat[uiActiveLightCount]->addChunk(
            _shadowSHL);

        _vShadowCmat[uiActiveLightCount]->addChunk(
            _vShadowSHLVar[uiActiveLightCount]);

        _vShadowCmat[uiActiveLightCount]->addChunk(
            _shadowVP->_vTexChunks[num].pTexO);

        _vShadowCmat[uiActiveLightCount]->addChunk(
            _shadowVP->_vTexChunks[num].pTexE);

        _vShadowCmat[uiActiveLightCount]->addChunk(
            _shadowFactorMapO);

        RenderAction *a = dynamic_cast<RenderAction *>(pEnv->getAction());

        a->pushPartition((RenderPartition::CopyWindow      |
                          RenderPartition::CopyViewing     |
                          RenderPartition::CopyProjection  |
                          RenderPartition::CopyFrustum     |
                          RenderPartition::CopyNearFar     |
                          RenderPartition::CopyViewportSize),
                         RenderPartition::StateSorting);
        {
            RenderPartition *pPart = a->getActivePartition();

            pPart->setRenderTarget(_pFB);
            pPart->setDrawBuffer  (GL_COLOR_ATTACHMENT1_EXT);
            
            Node *light  = _shadowVP->_lights[num].first;
            Node *parent =  light->getParent();
            
            if(parent != NULL)
            {
                a->pushMatrix(parent->getToWorld());
            }

            if(uiActiveLightCount == 0)
            {
                pPart->setBackground(_pClearBackground);
            }
                           
            commitChanges();

            a->overrideMaterial(_vShadowCmat[uiActiveLightCount], 
                                 a->getActNode());
            a->recurse(light);
            a->overrideMaterial( NULL,                    
                                 a->getActNode());
            
            if(parent != NULL)
            {
                a->popMatrix();
            }
        }
        a->popPartition();

        _firstRun = 0;
    }
}


void PCSSShadowMapHandler::render(DrawEnv *pEnv,
                                  RenderAction *pTmpAction)
{
    Window  *win = pEnv->getWindow();
    initialize(win);

    if(_useGLSL && _useShadowExt)
    {
        glPushAttrib(GL_ENABLE_BIT);

        if(!_initTexturesDone)
            initTextures(pEnv);

        if(_useFBO)
        {
            if(!initFBO(pEnv))
                printf("ERROR with FBOBJECT\n");
        }

        GLfloat globalAmbient[] =
        {
            0.0, 0.0, 0.0, 1.0
        };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
        _firstRun = 1;

        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            {
                _shadowVP->_vTexChunks[i].pTexO->setMinFilter(GL_NEAREST);
                _shadowVP->_vTexChunks[i].pTexO->setMagFilter(GL_NEAREST);
            }
        }

#if 0
        if(_shadowVP->getPixelWidth() != _width ||
           _shadowVP->getPixelHeight() != _height)
#endif
        if(pEnv->getPixelWidth() != _width ||
           pEnv->getPixelHeight() != _height)
        {
#if 0
            _width = _shadowVP->getPixelWidth();
            _height = _shadowVP->getPixelHeight();
#endif
            _width = pEnv->getPixelWidth();
            _height = pEnv->getPixelHeight();

            if(_useNPOTTextures)
            {
                _colorMapImage->set(GL_RGB, _width, _height);

                _shadowFactorMapImage->set(GL_RGB, _width, _height);

                reInit(pEnv);
            }
            else
            {
                if(_width > _height)
                    _widthHeightPOT = osgNextPower2(_width - 1);
                else
                    _widthHeightPOT = osgNextPower2(_height - 1);

                _colorMapImage->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);

                _shadowFactorMapImage->set(GL_RGB, _widthHeightPOT,
                                           _widthHeightPOT);
            }
        }

        commitChanges();

        if(_shadowVP->getMapAutoUpdate() == true ||
           _shadowVP->_trigger_update    == true  )
        {
            if(_useFBO && _useNPOTTextures)
            {
                createColorMapFBO(pEnv, pTmpAction);
            }


            //deactivate transparent Nodes
            for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
            {
                _shadowVP->_transparent[t]->setTravMask(0);
            }


            if(_useFBO)
            {
                createShadowMapsFBO(pEnv, pTmpAction);
            }


            // switch on all transparent geos
            for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
            {
                _shadowVP->_transparent[t]->setTravMask(
                    TypeTraits<UInt32>::BitsSet);
            }


            UInt32 uiActiveLightCount = 0;

            for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
            {
                if(_shadowVP->_lightStates[i] != 0)
                {
                    if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                       _shadowVP->_lights[i].second->getShadowIntensity() != 
                                                                           0.0)
                    {
                        if(_useFBO && _useNPOTTextures)
                        {
                            createShadowFactorMapFBO(pEnv, 
                                                     pTmpAction, 
                                                     i,
                                                     uiActiveLightCount);
                        }

                        ++uiActiveLightCount;
                    }
                }
            }
            
            _shadowVP->_trigger_update = false;
        }

        setupDrawCombineMap1(pEnv->getAction());
            
        glPopAttrib();

    }
}

OSG_END_NAMESPACE
