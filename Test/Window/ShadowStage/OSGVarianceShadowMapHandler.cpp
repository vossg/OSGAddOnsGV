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

#include "OSGVarianceShadowMapHandler.h"
#include "OSGShadowStage.h"
#include "OSGTreeHandler.h"
#include "OSGGLU.h"




#include "OSGRenderBuffer.h"
#include "OSGTextureBuffer.h"

OSG_BEGIN_NAMESPACE

#include "ShaderCode/OSGVarianceShadowMapShaderCode.cinl"

VarianceShadowMapHandler::VarianceShadowMapHandler(ShadowStage *source) :
    TreeHandler(source),
    _tiledeco(NULL),
    _colorMapImage(NULL),
    _shadowFactorMapImage(NULL),
    _shadowSHL(NULL),
    _depthSHL(NULL),
    _firstRun(1),
    _texChanged(false),
    _initTexturesDone(false)
{
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
    //_shadowSHL->readVertexProgram("Variance_Shadow.vert");
    //_shadowSHL->readFragmentProgram("Variance_Shadow.frag");
    _shadowSHL->setVertexProgram(_variance_vp);
    _shadowSHL->setFragmentProgram(_variance_fp);

    //SHL Chunk 2
    _combineSHL = SHLChunk::create();

    //_combineSHL->readVertexProgram("Variance_Shadow_combine.vert");
    //_combineSHL->readFragmentProgram("Variance_Shadow_combine.frag");
    _combineSHL->setVertexProgram(_shadow_combine_vp);
    _combineSHL->setFragmentProgram(_shadow_combine_fp);

    _combineDepth = DepthChunk::create();
        _combineDepth->setReadOnly(true);

    //Combine Shader
    _combineCmat = ChunkMaterial::create();
    _combineCmat->addChunk(_combineSHL);
    _combineCmat->addChunk(_colorMapO);
    _combineCmat->addChunk(_shadowFactorMapO);
    _combineCmat->addChunk(_combineDepth);

    //SHL Depth
    _depthSHL = SHLChunk::create();

    //_depthSHL->readVertexProgram("depth.vert");
    //_depthSHL->readFragmentProgram("depth.frag");
    _depthSHL->setVertexProgram(_depth_vp);
    _depthSHL->setFragmentProgram(_depth_fp);

    _pClearSMapBack = SolidBackground::create();
    
    _pClearSMapBack->setColor(Color3f(1.f, 1.f, 1.f));
}

VarianceShadowMapHandler::~VarianceShadowMapHandler(void)
{
    _tiledeco        = NULL;

    _shadowSHL       = NULL;
    _depthSHL        = NULL;
    _combineSHL      = NULL;
    _combineDepth    = NULL;
    _combineCmat     = NULL;
    

    _vShadowCmat  .clear();
    _vShadowSHLVar.clear();

    _vDepthSHLVar .clear();
    _vDepthCmat   .clear();

    _pClearSMapBack  = NULL;
}

/// Checks if FBO status is ok
bool VarianceShadowMapHandler::checkFrameBufferStatus(Window *win)
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


bool VarianceShadowMapHandler::initFBO(DrawEnv *pEnv)
{
    if(!_texChanged)
    {
        //Set Shadow Map Texture to the needed Format
        Real32  maximumAnistropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnistropy);
        maximumAnistropy = osgMin(maximumAnistropy, Real32(8.0));

        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            _shadowVP->_vTexChunks[i].pTexO->setImage(
                _shadowVP->_shadowImages[i]);

            _shadowVP->_vTexChunks[i].pTexO->setInternalFormat(GL_RGBA16F_ARB);
            _shadowVP->_vTexChunks[i].pTexO->setExternalFormat(GL_RGBA);

            _shadowVP->_vTexChunks[i].pTexO->setMinFilter(
                GL_LINEAR_MIPMAP_LINEAR);

            //_shadowVP->_texChunks[i]->setMinFilter(GL_LINEAR);
            _shadowVP->_vTexChunks[i].pTexO->setMagFilter(GL_LINEAR);
            _shadowVP->_vTexChunks[i].pTexO->setAnisotropy(maximumAnistropy);
            _shadowVP->_vTexChunks[i].pTexO->setWrapS(GL_REPEAT);
            _shadowVP->_vTexChunks[i].pTexO->setWrapT(GL_REPEAT);
            _shadowVP->_vTexChunks[i].pTexO->setTarget(GL_TEXTURE_2D);

            UInt32  mSize = _shadowVP->getMapSize();

            if(mSize > 2048)
                mSize = 2048;

            _shadowVP->_shadowImages[i]->set(Image::OSG_RGBA_PF, mSize, mSize,
                                             1, 1, 1, 0, 0,
                                             Image::OSG_FLOAT16_IMAGEDATA,
                                             false);

            _shadowVP->_vTexChunks[i].pFBO->setColorAttachment(
                _shadowVP->_vTexChunks[i].pFBO->getDepthAttachment(), 0);

//            _shadowVP->_vTexChunks[i].pFBO->setPostProcessOnDeactivate(true);
            
            RenderBufferUnrecPtr pDepthRB = RenderBuffer::create();
            
            pDepthRB->setInternalFormat(GL_DEPTH_COMPONENT24);

            _shadowVP->_vTexChunks[i].pFBO->setDepthAttachment(pDepthRB);

            _shadowVP->_vTexChunks[i].pFBO->setSize(_shadowVP->getMapSize(),
                                                    _shadowVP->getMapSize());
        }
        _texChanged = true;
    }

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

void VarianceShadowMapHandler::reInit(DrawEnv *pEnv)
{
}

void VarianceShadowMapHandler::initTextures(DrawEnv *pEnv)
{
    _initTexturesDone = true;

    Int32   width  = pEnv->getPixelWidth();
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


void VarianceShadowMapHandler::createShadowMapsFBO(DrawEnv      *pEnv, 
                                                   RenderAction *pTmpAction)
{
    UInt32  mSize = _shadowVP->getMapSize();

    if(mSize > 2048)
        mSize = 2048;

    //------Setting up Window to fit size of ShadowMap----------------


    // disable all lights more speed
    std::vector<bool> lightStates;
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // store old states.
        lightStates.push_back(_shadowVP->_lights[i].second->getOn());
        _shadowVP->_lights[i].second->setOn(false);
    }

    // activate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            if(_shadowVP->_excludeNodeActive[i])
                exnode->setTravMask(0);
    }

    RenderAction *a = dynamic_cast<RenderAction *>(pEnv->getAction());

    UInt32 uiActiveLightCount = 0;

    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
            {

                GLenum  *buffers = NULL;
                buffers = new GLenum[1];
                buffers[0] = GL_COLOR_ATTACHMENT0_EXT;

                Pnt3f   lPos;
                bool    isDirLight;
                Real32  sceneDiagLength;

                if(_shadowVP->_lights[i].second->getType() == 
                                                 PointLight::getClassType())
                {
                    PointLight *tmpPoint;
                    tmpPoint = dynamic_cast<PointLight *>(
                        _shadowVP->_lights[i].second.get());

                    lPos = tmpPoint->getPosition();

                    if(tmpPoint->getBeacon() != NULL)
                    {
                        Matrix  m = tmpPoint->getBeacon()->getToWorld();
                        m.mult(lPos, lPos);
                    }
                    isDirLight = false;

                    Pnt3f           center;
                    _shadowVP->getLightRoot(i)->getVolume().getCenter(center);

                    Vec3f           dir = lPos - center;
                    Real32          dirLength = dir.length();

                    Vec3f           diff =
                        (_shadowVP->getLightRoot(i)->getVolume().getMax() -
                         center);
                    Real32          diffLength = diff.length();

                    sceneDiagLength = dirLength + diffLength;
                }

                else if(_shadowVP->_lights[i].second->getType() ==
                                                    SpotLight::getClassType())
                {
                    SpotLight *tmpSpot;
                    tmpSpot = dynamic_cast<SpotLight *>(
                        _shadowVP->_lights[i].second.get());

                    lPos = tmpSpot->getPosition();
                    if(tmpSpot->getBeacon() != NULL)
                    {
                        Matrix  m = tmpSpot->getBeacon()->getToWorld();
                        m.mult(lPos, lPos);
                    }
                    isDirLight = false;
                    Pnt3f           center;
                    _shadowVP->getLightRoot(i)->getVolume().getCenter(center);

                    Vec3f           dir = lPos - center;
                    Real32          dirLength = dir.length();

                    Vec3f           diff =
                        (_shadowVP->getLightRoot(i)->getVolume().getMax() -
                         center);
                    Real32          diffLength = diff.length();

                    sceneDiagLength = dirLength + diffLength;
                }

                else
                {
                    isDirLight = true;
                    sceneDiagLength = 1.0;
                }

                if(_vDepthCmat.size() == uiActiveLightCount)
                {
                    _vDepthCmat.push_back(ChunkMaterial::create());
                }
        
                OSG_ASSERT(uiActiveLightCount < _vDepthCmat.size());

                if(_vDepthSHLVar.size() == uiActiveLightCount)
                {
                    _vDepthSHLVar.push_back(SHLVariableChunk::create());
                    
                    _vDepthSHLVar[uiActiveLightCount]->setSHLChunk(_depthSHL);
                }

                OSG_ASSERT(uiActiveLightCount < _vDepthSHLVar.size());

                _vDepthSHLVar[uiActiveLightCount]->addUniformVariable(
                    "sceneDiagLength",
                    Real32(sceneDiagLength));

                _vDepthSHLVar[uiActiveLightCount]->addUniformVariable(
                    "isDirLight", bool(isDirLight));

                
                _vDepthCmat[uiActiveLightCount]->clearChunks();
                _vDepthCmat[uiActiveLightCount]->addChunk(_depthSHL);
                _vDepthCmat[uiActiveLightCount]->addChunk(
                    _vDepthSHLVar[uiActiveLightCount]);

                commitChanges();

                a->pushPartition();
                {
                    RenderPartition   *pPart    = a->getActivePartition();
                    
                    pPart->setRenderTarget(_shadowVP->_vTexChunks[i].pFBO);
                    pPart->setDrawBuffer  (*buffers                      );

                    pPart->setWindow  (a->getWindow());

                    pPart->calcViewportDimension(0.f,
                                                 0.f,
                                                 mSize,
                                                 mSize,
                                                 
                                                 mSize,
                                                 mSize);


                    RenderFunctor f = 
                        boost::bind(&VarianceShadowMapHandler::genMipMapCB,
                                    this,
                                    _1,
                                    i);

                    pPart->addPreRenderCallback(f);

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
                    
                    pPart->setBackground(_pClearSMapBack);
                    
                    Node *light  = _shadowVP->_lights[i].first;
                    Node *parent =  light->getParent();
                    
                    if(parent != NULL)
                    {
                        a->pushMatrix(parent->getToWorld());
                    }
                    
                    
                    a->overrideMaterial(_vDepthCmat[uiActiveLightCount], 
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

                ++uiActiveLightCount;
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
}

void VarianceShadowMapHandler::genMipMapCB(DrawEnv *pEnv,
                                           UInt32   uiLightIdx)
{
    glBindTexture(GL_TEXTURE_2D,
                  pEnv->getWindow()->getGLObjectId(
                      _shadowVP->_vTexChunks[uiLightIdx].pTexO->getGLId()));

    glGenerateMipmapEXT(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void VarianceShadowMapHandler::createColorMapFBO(DrawEnv *pEnv,
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


void VarianceShadowMapHandler::createShadowFactorMapFBO(
    DrawEnv      *pEnv,
    RenderAction *pTmpAction,
    UInt32        num,
    UInt32        uiActiveLightCount)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);

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
        _shadowVP->_lightCameras[num]->getViewing(LVM,
                                                  pEnv->getPixelWidth(),
                                                  pEnv->getPixelHeight());

        _shadowVP->_lightCameras[num]->getProjection(LPM,
                                                     pEnv->getPixelWidth(),
                                                     pEnv->getPixelHeight());

#if 0
        _shadowVP->getCamera()->getViewing(CVM, 
                                           _shadowVP->getPixelWidth(),
                                           _shadowVP->getPixelHeight());
#endif

        pEnv->getAction()->getCamera()->getViewing(CVM,
                                                   pEnv->getPixelWidth(),
                                                   pEnv->getPixelHeight());
        Matrix  iCVM = CVM;
        iCVM.invert();

        Real32  texFactor;
        if(_shadowVP->_lights[num].second->getType() == 
                                               SpotLight::getClassType() ||
           _shadowVP->_lights[num].second->getType() == 
                                               PointLight::getClassType())
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

        Matrix  shadowMatrix2 = LVM;
        shadowMatrix2.mult(iCVM);


        Real32  xFactor = 1.0;
        Real32  yFactor = 1.0;

        if(!_useNPOTTextures)
        {
            xFactor = Real32(_width) / Real32(_widthHeightPOT);
            yFactor = Real32(_height) / Real32(_widthHeightPOT);
        }

        Pnt3f   lPos;
        bool    isDirLight;
        Real32  sceneDiagLength;

        if(_shadowVP->_lights[num].second->getType() ==
                                                  PointLight::getClassType())
        {
            PointLight *tmpPoint;

            tmpPoint = dynamic_cast<PointLight *>(
                _shadowVP->_lights[num].second.get());

            lPos = tmpPoint->getPosition();

            if(tmpPoint->getBeacon() != NULL)
            {
                Matrix  m = tmpPoint->getBeacon()->getToWorld();
                m.mult(lPos, lPos);
            }

            isDirLight = false;
            Pnt3f           center;

            _shadowVP->getLightRoot(num)->getVolume().getCenter(center);

            Vec3f           dir = lPos - center;
            Real32          dirLength = dir.length();

            Vec3f           diff = (_shadowVP->getLightRoot(num)->getVolume
                                    ().getMax() - center);
            Real32          diffLength = diff.length();

            sceneDiagLength = dirLength + diffLength;
        }

        else if(_shadowVP->_lights[num].second->getType() == 
                                                     SpotLight::getClassType())
        {
            SpotLight *tmpSpot;
            tmpSpot = dynamic_cast<SpotLight *>(
                _shadowVP->_lights[num].second.get());

            lPos = tmpSpot->getPosition();

            if(tmpSpot->getBeacon() != NULL)
            {
                Matrix  m = tmpSpot->getBeacon()->getToWorld();
                m.mult(lPos, lPos);
            }

            isDirLight = false;
            Pnt3f           center;
            _shadowVP->getLightRoot(num)->getVolume().getCenter(center);

            Vec3f           dir = lPos - center;
            Real32          dirLength = dir.length();

            Vec3f           diff = (_shadowVP->getLightRoot(num)->getVolume
                                    ().getMax() - center);
            Real32          diffLength = diff.length();

            sceneDiagLength = dirLength + diffLength;
        }

        else
        {
            isDirLight = true;
            sceneDiagLength = 1.0;
        }


        Real32  lod;

        if(_shadowVP->getShadowSmoothness() <= 0.1999)
            lod = 0.5;
        else if(_shadowVP->getShadowSmoothness() <= 0.3999)
            lod = 1.5;
        else if(_shadowVP->getShadowSmoothness() <= 0.5999)
            lod = 2.5;
        else if(_shadowVP->getShadowSmoothness() <= 0.7999)
            lod = 3.5;
        else
            lod = 4.5;

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
        
        OSG_ASSERT(uiActiveLightCount < _vShadowSHLVar.size());

        _shadowSHL->addUniformVariable("shadowMap",    0);
        _shadowSHL->addUniformVariable("oldFactorMap", 1);

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "firstRun",
            (uiActiveLightCount == 0) ? Int32(1) : Int32(0));

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "intensity", shadowIntensity);

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "texFactor", texFactor);
        //_shadowSHL->addUniformVariable("shadowBias", 0.0075f);

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "lightPM", shadowMatrix);

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "lightPM2", shadowMatrix2);

        //_shadowSHL->addUniformVariable("shadowRange", Real32(shadowRange));
        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "xFactor", Real32(xFactor));

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "yFactor", Real32(yFactor));

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "sceneDiagLength", Real32(sceneDiagLength));

        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "Lod", Real32(lod));
        
        _vShadowSHLVar[uiActiveLightCount]->addUniformVariable(
            "isDirLight", bool(isDirLight));



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


void VarianceShadowMapHandler::render(DrawEnv *pEnv, 
                                      RenderAction *pTmpAction)
{
    Window  *win = pEnv->getWindow();
    initialize(win);

    if(_useGLSL && _useShadowExt && _useFBO)
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
        //Used for ShadowFactorMap
        _firstRun = 1;

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

        if(_shadowVP->getMapAutoUpdate() == true ||
           _shadowVP->_trigger_update    == true  )
        {
            if(_useNPOTTextures)
            {
                createColorMapFBO(pEnv, pTmpAction);
            }


            //deactivate transparent Nodes
            for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
            {
                _shadowVP->_transparent[t]->setTravMask(0);
            }


            createShadowMapsFBO(pEnv, pTmpAction);


            // switch on all transparent geos
            for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
            {
                _shadowVP->_transparent[t]->setTravMask(
                    TypeTraits<UInt32>::BitsSet);
            }


            //filterShadowMaps(pEnv);

            bool   bClear             = true;
            UInt32 uiActiveLightCount = 0;

            for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
            {
                if(_shadowVP->_lightStates[i] != 0)
                {
                    if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                       _shadowVP->_lights[i].second->getShadowIntensity() != 
                                                                           0.0)
                    {
                        if(_useNPOTTextures)
                        {
                            createShadowFactorMapFBO(pEnv, 
                                                     pTmpAction, 
                                                     i,
                                                     uiActiveLightCount);
                        }

                        bClear = false;
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
