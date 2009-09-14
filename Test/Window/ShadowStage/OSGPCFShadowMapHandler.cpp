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
#include <OSGMatrixCameraDecorator.h>
#include <OSGLight.h>
#include <OSGMaterialGroup.h>
#include "OSGPCFShadowMapHandler.h"
#include "OSGShadowStage.h"
#include "OSGTreeHandler.h"
#include "OSGGLU.h"

#include "OSGRenderBuffer.h"
#include "OSGTextureBuffer.h"

//#define USE_FBO_FOR_COLOR_AND_FACTOR_MAP

#if 0
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
#if 1 //GL_ARB_texture_float
#    define    GL_RGBA32F_ARB                      0x8814
#    define    GL_RGB32F_ARB                       0x8815
#    define    GL_ALPHA32F_ARB                     0x8816
#    define    GL_INTENSITY32F_ARB                 0x8817
#    define    GL_LUMINANCE32F_ARB                 0x8818
#    define    GL_LUMINANCE_ALPHA32F_ARB           0x8819
#    define    GL_RGBA16F_ARB                      0x881A
#    define    GL_RGB16F_ARB                       0x881B
#    define    GL_ALPHA16F_ARB                     0x881C
#    define    GL_INTENSITY16F_ARB                 0x881D
#    define    GL_LUMINANCE16F_ARB                 0x881E
#    define    GL_LUMINANCE_ALPHA16F_ARB           0x881F
#endif

/* ----------------------- GL_EXT_framebuffer_object ----------------------- */

#ifndef GL_EXT_framebuffer_object
#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506
#define GL_MAX_RENDERBUFFER_SIZE_EXT 0x84E8
#define GL_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT 0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE_EXT 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT 0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT 0x8CDD
#define GL_FRAMEBUFFER_STATUS_ERROR_EXT 0x8CDE
#define GL_MAX_COLOR_ATTACHMENTS_EXT 0x8CDF
#define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT 0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT 0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT 0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT 0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT 0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT 0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT 0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT 0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT 0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT 0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT 0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT 0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT 0x8CED
#define GL_COLOR_ATTACHMENT14_EXT 0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT 0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT 0x8D00
#define GL_STENCIL_ATTACHMENT_EXT 0x8D20
#define GL_FRAMEBUFFER_EXT 0x8D40
#define GL_RENDERBUFFER_EXT 0x8D41
#define GL_RENDERBUFFER_WIDTH_EXT 0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT 0x8D44
#define GL_STENCIL_INDEX_EXT 0x8D45
#define GL_STENCIL_INDEX1_EXT 0x8D46
#define GL_STENCIL_INDEX4_EXT 0x8D47
#define GL_STENCIL_INDEX8_EXT 0x8D48
#define GL_STENCIL_INDEX16_EXT 0x8D49
#endif

// Some installations have a typo in the name for this one
#ifndef GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
#endif

#ifndef GL_TEXTURE_RECTANGLE_ARB
#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif

#ifndef GL_DEPTH_COMPONENT_ARB
    #define GL_DEPTH_COMPONENT_ARB 0x1902
#endif

#ifndef GL_ARB_depth_texture
#define GL_DEPTH_COMPONENT16_ARB 0x81A5
#define GL_DEPTH_COMPONENT24_ARB 0x81A6
#define GL_DEPTH_COMPONENT32_ARB 0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB 0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB 0x884B
#endif

#ifndef GL_ARB_shadow
#define GL_TEXTURE_COMPARE_MODE_ARB 0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB 0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB 0x884E
#endif
#endif

OSG_BEGIN_NAMESPACE

#include "ShaderCode/OSGPCFShadowMapShaderCode.cinl"

PCFShadowMapHandler::PCFShadowMapHandler(ShadowStage *source) :
    TreeHandler(source),
    _tiledeco(NULL),
    _colorMapImage(NULL),
    _shadowFactorMapImage(NULL),
    _shadowFactorMapImage2(NULL),
    _shadowSHL(NULL),
    _shadowSHL2(NULL),
    _shadowSHL3(NULL),
    _shadowSHL4(NULL),
    _shadowCubeSHL(NULL),
    _firstRun(1),
    _transforms(),
    _oldRange(0),
    _initTexturesDone(false)
{
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

    _shadowFactorMap2O = TextureObjChunk::create();

    _shadowFactorMapImage2 = Image::create();

    _shadowFactorMap2O->setImage(_shadowFactorMapImage2);
    _shadowFactorMap2O->setInternalFormat(GL_RGB);
    _shadowFactorMap2O->setExternalFormat(GL_RGB);
    _shadowFactorMap2O->setMinFilter(GL_LINEAR);
    _shadowFactorMap2O->setMagFilter(GL_LINEAR);
    _shadowFactorMap2O->setWrapS(GL_REPEAT);
    _shadowFactorMap2O->setWrapT(GL_REPEAT);
    _shadowFactorMap2O->setTarget(GL_TEXTURE_2D);

    //SHL Chunk 1
    _shadowSHL = SHLChunk::create();
    _shadowSHL2 = SHLChunk::create();
    _shadowSHL3 = SHLChunk::create();
    _shadowSHL4 = SHLChunk::create();

    //SHL Chunk 2
    _combineSHL = SHLChunk::create();

    //SHL Chunk 3
    _shadowCubeSHL = SHLChunk::create();

    //_combineSHL->readVertexProgram("PCF_Shadow_combine.vert");
    //_combineSHL->readFragmentProgram("PCF_Shadow_combine.frag");
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

    _oldRange = _shadowVP->getShadowSmoothness();

    if(_oldRange <= 0.1999)
    {
        //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
        //_shadowSHL->readFragmentProgram("PCF2_Shadow.frag");
        _shadowSHL->setVertexProgram(_pcf_shadow_vp);
        _shadowSHL->setFragmentProgram(_pcf2_shadow_fp);

        //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
        //_shadowSHL2->readFragmentProgram("PCF2_Shadow2.frag");
        _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
        _shadowSHL2->setFragmentProgram(_pcf2_shadow2_fp);

        //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
        //_shadowSHL3->readFragmentProgram("PCF2_Shadow3.frag");
        _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
        _shadowSHL3->setFragmentProgram(_pcf2_shadow3_fp);

        //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
        //_shadowSHL4->readFragmentProgram("PCF2_Shadow4.frag");
        _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
        _shadowSHL4->setFragmentProgram(_pcf2_shadow4_fp);

        //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
        //_shadowCubeSHL->readFragmentProgram("PCF2_CubeShadow.frag");
        _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
        _shadowCubeSHL->setFragmentProgram(_pcf2_shadowCube_fp);
    }
    else if(_oldRange <= 0.3999)
    {
        //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
        //_shadowSHL->readFragmentProgram("PCF3_Shadow.frag");
        _shadowSHL->setVertexProgram(_pcf_shadow_vp);
        _shadowSHL->setFragmentProgram(_pcf3_shadow_fp);

        //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
        //_shadowSHL2->readFragmentProgram("PCF3_Shadow2.frag");
        _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
        _shadowSHL2->setFragmentProgram(_pcf3_shadow2_fp);

        //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
        //_shadowSHL3->readFragmentProgram("PCF3_Shadow3.frag");
        _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
        _shadowSHL3->setFragmentProgram(_pcf3_shadow3_fp);

        //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
        //_shadowSHL4->readFragmentProgram("PCF3_Shadow4.frag");
        _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
        _shadowSHL4->setFragmentProgram(_pcf3_shadow4_fp);

        //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
        //_shadowCubeSHL->readFragmentProgram("PCF3_CubeShadow.frag");
        _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
        _shadowCubeSHL->setFragmentProgram(_pcf3_shadowCube_fp);
    }
    else if(_oldRange <= 0.5999)
    {
        //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
        //_shadowSHL->readFragmentProgram("PCF4_Shadow.frag");
        _shadowSHL->setVertexProgram(_pcf_shadow_vp);
        _shadowSHL->setFragmentProgram(_pcf4_shadow_fp);

        //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
        //_shadowSHL2->readFragmentProgram("PCF4_Shadow2.frag");
        _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
        _shadowSHL2->setFragmentProgram(_pcf4_shadow2_fp);

        //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
        //_shadowSHL3->readFragmentProgram("PCF4_Shadow3.frag");
        _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
        _shadowSHL3->setFragmentProgram(_pcf4_shadow3_fp);

        //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
        //_shadowSHL4->readFragmentProgram("PCF4_Shadow4.frag");
        _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
        _shadowSHL4->setFragmentProgram(_pcf4_shadow4_fp);

        //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
        //_shadowCubeSHL->readFragmentProgram("PCF4_CubeShadow.frag");
        _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
        _shadowCubeSHL->setFragmentProgram(_pcf4_shadowCube_fp);
    }
    else if(_oldRange <= 0.7999)
    {
        //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
        //_shadowSHL->readFragmentProgram("PCF5_Shadow.frag");
        _shadowSHL->setVertexProgram(_pcf_shadow_vp);
        _shadowSHL->setFragmentProgram(_pcf5_shadow_fp);

        //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
        //_shadowSHL2->readFragmentProgram("PCF5_Shadow2.frag");
        _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
        _shadowSHL2->setFragmentProgram(_pcf5_shadow2_fp);

        //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
        //_shadowSHL3->readFragmentProgram("PCF5_Shadow3.frag");
        _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
        _shadowSHL3->setFragmentProgram(_pcf5_shadow3_fp);

        //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
        //_shadowSHL4->readFragmentProgram("PCF5_Shadow4.frag");
        _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
        _shadowSHL4->setFragmentProgram(_pcf5_shadow4_fp);

        //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
        //_shadowCubeSHL->readFragmentProgram("PCF5_CubeShadow.frag");
        _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
        _shadowCubeSHL->setFragmentProgram(_pcf5_shadowCube_fp);
    }
    else
    {
        //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
        //_shadowSHL->readFragmentProgram("PCF6_Shadow.frag");
        _shadowSHL->setVertexProgram(_pcf_shadow_vp);
        _shadowSHL->setFragmentProgram(_pcf6_shadow_fp);

        //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
        //_shadowSHL2->readFragmentProgram("PCF6_Shadow2.frag");
        _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
        _shadowSHL2->setFragmentProgram(_pcf6_shadow2_fp);

        //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
        //_shadowSHL3->readFragmentProgram("PCF6_Shadow3.frag");
        _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
        _shadowSHL3->setFragmentProgram(_pcf6_shadow3_fp);

        //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
        //_shadowSHL4->readFragmentProgram("PCF6_Shadow4.frag");
        _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
        _shadowSHL4->setFragmentProgram(_pcf6_shadow4_fp);

        //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
        //_shadowCubeSHL->readFragmentProgram("PCF6_CubeShadow.frag");
        _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
        _shadowCubeSHL->setFragmentProgram(_pcf6_shadowCube_fp);
    }

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

    PolygonChunkUnrecPtr pPoly = PolygonChunk::create();

    pPoly->setOffsetFill  (true                     );
    pPoly->setOffsetFactor(_shadowVP->getOffFactor());
    pPoly->setOffsetBias  (_shadowVP->getOffBias  ());

    _unlitMat->addChunk(pPoly);
}

PCFShadowMapHandler::~PCFShadowMapHandler(void)
{
    _tiledeco         = NULL;

    _shadowSHL        = NULL;
    _shadowSHL2       = NULL;
    _shadowSHL3       = NULL;
    _shadowSHL4       = NULL;
    _combineSHL       = NULL;
    _combineDepth     = NULL;
    _shadowCubeSHL    = NULL;
    _combineCmat      = NULL;

    _vShadowCmat      .clear();
    _vShadowSHLVar    .clear();
    _vShadowSHLVar2   .clear();
    _vShadowSHLVar3   .clear();
    _vShadowSHLVar4   .clear();
    _vShadowCubeSHLVar.clear();
}


bool PCFShadowMapHandler::initFBO(DrawEnv *pEnv)
{
    if(_useFBO)
    {
        Int32   width = pEnv->getPixelWidth();
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

        _shadowFactorMapImage2->set(GL_RGB, _width, _height);

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


        pTexBuffer = TextureBuffer::create();

        pTexBuffer->setTexture(_shadowFactorMap2O);

        _pFB->setColorAttachment(pTexBuffer, 2);


        _pFB->setDepthAttachment(pDepthRB);

        commitChanges();
    }
    return true;
}

void PCFShadowMapHandler::reInit(DrawEnv *pEnv)
{
}

void PCFShadowMapHandler::initTextures(DrawEnv *pEnv)
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

        _shadowFactorMapImage2->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);
    }
}


void PCFShadowMapHandler::createShadowMapsFBO(DrawEnv *pEnv, 
                                              RenderAction *pTmpAction)
{

    //------Setting up Window to fit size of ShadowMap----------------

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
        if(_shadowVP->_lightStates[i])
        {
            if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
            {
                //------Setting up Window to fit size of ShadowMap------------

                if(_shadowVP->_lights[i].second->getType() != 
                                                 PointLight::getClassType() ||
                   !_shadowVP->_realPointLight[i])
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
                else
                {
                    MatrixCameraDecoratorUnrecPtr    deco =
                        MatrixCameraDecorator::create();

                    for(UInt32 j = 0;j < 6;j++)
                    {
                        //Offset berechnen
                        UInt32  xOffset, yOffset;
                        if(j == 0)
                        {
                            xOffset = 0;
                            yOffset = 0;
                        }
                        else if(j == 1)
                        {
                            xOffset = _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 2)
                        {
                            xOffset = 2 * _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 3)
                        {
                            xOffset = 3 * _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 4)
                        {
                            xOffset = 0;
                            yOffset = _PLMapSize;
                        }
                        else
                        {
                            xOffset = _PLMapSize;
                            yOffset = _PLMapSize;
                        }

                        deco->setDecoratee(_shadowVP->_lightCameras[i]);
                        deco->setPreProjection(_transforms[j]);

                        a->pushPartition();
                        {
                            RenderPartition   *pPart = a->getActivePartition();

                            pPart->setRenderTarget(
                                _shadowVP->_vTexChunks[i].pFBO);

                            pPart->setWindow  (a->getWindow());

                            pPart->calcViewportDimension(
                                xOffset, 
                                yOffset,
                                xOffset + _PLMapSize,
                                yOffset + _PLMapSize,
                                                 
                                _shadowVP->getMapSize(),
                                _shadowVP->getMapSize() );


                            Matrix m, t;
                    
                            // set the projection
                            deco->getProjection          (
                                m, 
                                pPart->getViewportWidth (), 
                                pPart->getViewportHeight());
                            
                            deco->getProjectionTranslation(
                                t, 
                                pPart->getViewportWidth (), 
                                pPart->getViewportHeight());
                            
                            pPart->setupProjection(m, t);
                            
                            deco->getViewing(
                                m, 
                                pPart->getViewportWidth (),
                                pPart->getViewportHeight());
                            
                            pPart->setupViewing(m);
                        
                            pPart->setNear     (
                                deco->getNear());
                            pPart->setFar      (
                                deco->getFar ());
                        
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
        }
    }

    //-------Restoring old states of Window and Viewport----------

    // activate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);

        if(exnode != NULL)
            if(_shadowVP->_excludeNodeActive[i])
                exnode->setTravMask(TypeTraits<UInt32>::BitsSet);
    }

    // enable all lights.
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // restore old states.
        _shadowVP->_lights[i].second->setOn(lightStates[i]);
    }

#ifdef SHADOWCHECK
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
#endif
}

void PCFShadowMapHandler::createColorMapFBO(DrawEnv *pEnv,
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

void PCFShadowMapHandler::createShadowFactorMapFBO(DrawEnv *pEnv,
                                                   RenderAction *pTmpAction)
{
    _activeFactorMap = 0;

    //Finde alle aktiven Lichtquellen
    Real32              activeLights = 0;
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

    bool bCA1Cleared = false;
    bool bCA2Cleared = false;

    RenderAction *a = dynamic_cast<RenderAction *>(pEnv->getAction());

    UInt32 uiPLightCount = 0;

    //Zuerst alle echte Pointlights
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
               _shadowVP->_realPointLight[i])
            {
                Real32      shadowIntensity;
                if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                    shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                       activeLights);
                else
                    shadowIntensity =
                        (_shadowVP->_lights[i].second->getShadowIntensity() /
                         activeLights);

                Matrix      LVM, LPM, CVM;

                _shadowVP->_lightCameras[i]->getViewing(
                    LVM,
                    pEnv->getPixelWidth(),
                    pEnv->getPixelHeight());
                _shadowVP->_lightCameras[i]->getProjection(
                    LPM,
                    pEnv->getPixelWidth(), 
                    pEnv->getPixelHeight());
                
#if 0
                Camera *cam = _shadowVP->getCamera();
                if(cam != NULL)
                    cam->getViewing(CVM, _shadowVP->getPixelWidth(),
                                    _shadowVP->getPixelHeight());
#endif
                Camera *cam = pEnv->getAction()->getCamera();
                if(cam != NULL)
                {
                    cam->getViewing(CVM, 
                                    pEnv->getPixelWidth(),
                                    pEnv->getPixelHeight());
                }
                else
                {
                    SWARNING <<
                        "PCFShadowMap::createShadowFactorMapFBO: no camera "
                        "found!" << std::endl;
                }

                Matrix      iCVM = CVM;
                iCVM.invert();

                Real32      texFactor;
                if(_shadowVP->_lights[i].second->getType() == 
                                                 PointLight::getClassType() ||
                   _shadowVP->_lights[i].second->getType() ==
                                                 SpotLight::getClassType())
                {
                    texFactor = Real32(_width) / Real32(_height);
                }
                else
                {
                    texFactor = 1.0;
                }

                Matrix      shadowMatrix = LPM;
                shadowMatrix.mult(LVM);
                shadowMatrix.mult(iCVM);

                Real32      xFactor = 1.0;
                Real32      yFactor = 1.0;
	
                if(!_useNPOTTextures)
                {
                    xFactor = Real32(_width) / Real32(_widthHeightPOT);
                    yFactor = Real32(_height) / Real32(_widthHeightPOT);
                }

#if 0
                Matrix      m = 
                    pEnv->getAction()->getCamera()->getBeacon()->getToWorld();
#endif
                Matrix      m = 
                    pTmpAction->getCamera()->getBeacon()->getToWorld();

                Matrix      shadowMatrixOP = LVM;
                shadowMatrix.mult(iCVM);

                Matrix      shadowMatrixA = LPM;
                shadowMatrixA.mult(_transforms[0]);
                shadowMatrixA.mult(LVM);
                shadowMatrixA.mult(iCVM);

                Matrix      shadowMatrixB = LPM;
                shadowMatrixB.mult(_transforms[1]);
                shadowMatrixB.mult(LVM);
                shadowMatrixB.mult(iCVM);

                Matrix      shadowMatrixC = LPM;
                shadowMatrixC.mult(_transforms[2]);
                shadowMatrixC.mult(LVM);
                shadowMatrixC.mult(iCVM);

                Matrix      shadowMatrixD = LPM;
                shadowMatrixD.mult(_transforms[3]);
                shadowMatrixD.mult(LVM);
                shadowMatrixD.mult(iCVM);

                Matrix      shadowMatrixE = LPM;
                shadowMatrixE.mult(_transforms[4]);
                shadowMatrixE.mult(LVM);
                shadowMatrixE.mult(iCVM);

                Matrix      shadowMatrixF = LPM;
                shadowMatrixF.mult(_transforms[5]);
                shadowMatrixF.mult(LVM);
                shadowMatrixF.mult(iCVM);

                if(_vShadowCubeSHLVar.size() == uiPLightCount)
                {
                    _vShadowCubeSHLVar.push_back(SHLVariableChunk::create());
                }

                OSG_ASSERT(uiPLightCount < _vShadowCubeSHLVar.size());


                _shadowCubeSHL->addUniformVariable("shadowMap", 0);
                _shadowCubeSHL->addUniformVariable("oldFactorMap", 1);
                _shadowCubeSHL->addUniformVariable("firstRun", _firstRun);
                _shadowCubeSHL->addUniformVariable("intensity",
                                                    shadowIntensity);
                _shadowCubeSHL->addUniformVariable("texFactor", texFactor);
                _shadowCubeSHL->addUniformVariable("lightPMA", shadowMatrixA);
                _shadowCubeSHL->addUniformVariable("lightPMB", shadowMatrixB);
                _shadowCubeSHL->addUniformVariable("lightPMC", shadowMatrixC);
                _shadowCubeSHL->addUniformVariable("lightPMD", shadowMatrixD);
                _shadowCubeSHL->addUniformVariable("lightPME", shadowMatrixE);
                _shadowCubeSHL->addUniformVariable("lightPMF", shadowMatrixF);
                _shadowCubeSHL->addUniformVariable("lightPMOP",
                                                    shadowMatrixOP);
                _shadowCubeSHL->addUniformVariable("KKtoWK", m);
                _shadowCubeSHL->addUniformVariable("mapSize",
                                                    Real32(_PLMapSize));
                _shadowCubeSHL->addUniformVariable("xFactor",
                                                    Real32(xFactor));
                _shadowCubeSHL->addUniformVariable("yFactor",
                                                    Real32(yFactor));

                if(_vShadowCmat.size() == uiPLightCount)
                {
                    _vShadowCmat.push_back(ChunkMaterial::create());
                }

                OSG_ASSERT( uiPLightCount < _vShadowCmat.size());

                _vShadowCmat[uiPLightCount]->clearChunks();
                _vShadowCmat[uiPLightCount]->addChunk(_shadowCubeSHL);

                _vShadowCmat[uiPLightCount]->addChunk(
                    _shadowVP->_vTexChunks[i].pTexO);

                _vShadowCmat[uiPLightCount]->addChunk(
                    _shadowVP->_vTexChunks[i].pTexE);

                if(_activeFactorMap == 0)
                {
                    _vShadowCmat[uiPLightCount]->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _vShadowCmat[uiPLightCount]->addChunk(_shadowFactorMapO);
                }

                GLenum      *buffers = NULL;
                buffers = new GLenum[1];

                if(_activeFactorMap == 0)
                    buffers[0] = GL_COLOR_ATTACHMENT1_EXT;
                else
                    buffers[0] = GL_COLOR_ATTACHMENT2_EXT;

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
                    pPart->setDrawBuffer  (*buffers);

                    Node *light  = _shadowVP->_lights[i].first;
                    Node *parent =  light->getParent();

                    if(parent != NULL)
                    {
                        a->pushMatrix(parent->getToWorld());
                    }

                    if(_activeFactorMap == 0 && bCA1Cleared == false)
                    {
                        pPart->setBackground(_pClearBackground);
                        bCA1Cleared = true;
                    }
                    else if(bCA2Cleared == false)
                    {
                        pPart->setBackground(_pClearBackground);
                        bCA2Cleared = true;
                    }
                       
                    commitChanges();
      
                    a->overrideMaterial(_vShadowCmat[uiPLightCount], 
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

                delete[] buffers;
                _firstRun = 0;
                if(_activeFactorMap == 0)
                    _activeFactorMap = 1;
                else
                    _activeFactorMap = 0;

                ++uiPLightCount;
            }
        }
    }

    std::vector<Real32> shadowIntensityF;
    std::vector<Real32> texFactorF;
    std::vector<Real32> mapFactorF;
    std::vector<Matrix> shadowMatrixF;

    UInt32              lightCounter = 0;

    Real32              xFactor = 1.0;
    Real32              yFactor = 1.0;
	
    if(!_useNPOTTextures)
    {
        xFactor = Real32(_width) / Real32(_widthHeightPOT);
        yFactor = Real32(_height) / Real32(_widthHeightPOT);
    }

    //Jetzt alle normalen Lichtquellen
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0 &&
           ((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
             _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
            !_shadowVP->_realPointLight[i]))
        {

            Real32      shadowIntensity;
            if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                   activeLights);
            else
                shadowIntensity = (_shadowVP->_lights[i].second->getShadowIntensity
                                   () / activeLights);
            shadowIntensityF.push_back(shadowIntensity);
				
            Matrix      LVM, LPM, CVM;

            _shadowVP->_lightCameras[i]->getViewing(LVM,
                                                    pEnv->getPixelWidth(),
                                                    pEnv->getPixelHeight
                                                    ());
            _shadowVP->_lightCameras[i]->getProjection(LPM,
                                                       pEnv->getPixelWidth
                                                       (),
                                                       pEnv->getPixelHeight());

#if 0
            Camera *cam = _shadowVP->getCamera();
            if(cam != NULL)
                cam->getViewing(CVM, _shadowVP->getPixelWidth(),
                                _shadowVP->getPixelHeight());
#endif
            Camera *cam = pTmpAction->getCamera();
            if(cam != NULL)
            {
                cam->getViewing(CVM, 
                                pEnv->getPixelWidth(),
                                pEnv->getPixelHeight());
            }
            else
            {
                SWARNING <<
                    "PCFShadowMap::createShadowFactorMapFBO: no camera found!"
                    << std::endl;
            }

            Matrix      iCVM = CVM;
            iCVM.invert();

            Real32      texFactor;
            if(_shadowVP->_lights[i].second->getType() == 
                                                 PointLight::getClassType() ||
               _shadowVP->_lights[i].second->getType() ==
                                                 SpotLight::getClassType())
            {
                texFactor = Real32(_width) / Real32(_height);
            }
            else
            {
                texFactor = 1.0;
            }

            texFactorF.push_back(texFactor);

            Matrix      shadowMatrix = LPM;
            shadowMatrix.mult(LVM);
            shadowMatrix.mult(iCVM);

            shadowMatrixF.push_back(shadowMatrix);

            Real32      mapFactor;
            mapFactor = Real32(_shadowVP->getMapSize()) /
                Real32(_shadowVP->_shadowImages[i]->getWidth());
            mapFactorF.push_back(mapFactor);
            lightCounter++;
        }
    }

    if(lightCounter != 0)
    {
        UInt32 renderTimes = 1;
        UInt32 uiMatCount  = uiPLightCount;

        UInt32 uiVarCount  = 0;
        UInt32 uiVarCount2 = 0;
        UInt32 uiVarCount3 = 0;
        UInt32 uiVarCount4 = 0;

        if(lightCounter > 4)
            renderTimes = UInt32(ceil(Real32(lightCounter) / 4.0f));

        for(UInt32 i = 0;i < renderTimes;i++)
        {

            GLenum  *buffers = NULL;
            buffers = new GLenum[1];
            if(_activeFactorMap == 0)
                buffers[0] = GL_COLOR_ATTACHMENT1_EXT;
            else
                buffers[0] = GL_COLOR_ATTACHMENT2_EXT;

            UInt32  lightOffset = lightCounter - (i * 4);
		
            if(_vShadowCmat.size() == uiMatCount)
            {
                _vShadowCmat.push_back(ChunkMaterial::create());
            }

            OSG_ASSERT( uiMatCount < _vShadowCmat.size());

            //clear chunk and add Textures
            _vShadowCmat[uiMatCount]->clearChunks();
	
            UInt32  lightNum = 0;
            for(UInt32 j = 0;j < _shadowVP->_lights.size();j++)
            {
                if(_shadowVP->_lightStates[j] != 0)
                {
                    if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                        _shadowVP->_lights[j].second->getShadowIntensity() != 0.0) &&
                       !_shadowVP->_realPointLight[j])
                    {
                        if(lightNum >= (i * 4) && lightNum < ((i + 1) * 4))
                        {
                            _vShadowCmat[uiMatCount]->addChunk(
                                _shadowVP->_vTexChunks[j].pTexO);
                            _vShadowCmat[uiMatCount]->addChunk(
                                _shadowVP->_vTexChunks[j].pTexE);
                        }
                        lightNum++;
                    }
                }
            }

            if(lightOffset == 1)
            {
                _vShadowCmat[uiMatCount]->addChunk(_shadowSHL);

                if(_activeFactorMap == 0)
                {
                    _vShadowCmat[uiMatCount]->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _vShadowCmat[uiMatCount]->addChunk(_shadowFactorMapO);
                }

                if(_vShadowSHLVar.size() == uiVarCount)
                {
                    _vShadowSHLVar.push_back(SHLVariableChunk::create());
                }

                OSG_ASSERT(uiVarCount < _vShadowSHLVar.size());

                _shadowSHL->addUniformVariable("oldFactorMap", 1);
                _shadowSHL->addUniformVariable("shadowMap", 0);
                _shadowSHL->addUniformVariable("firstRun", _firstRun);
                _shadowSHL->addUniformVariable("intensity",
                                                shadowIntensityF[0 + (i * 4)]);
                _shadowSHL->addUniformVariable("texFactor", texFactorF[0 +
                                                (i * 4)]);
                _shadowSHL->addUniformVariable("lightPM", shadowMatrixF[0 +
                                                (i * 4)]);
                _shadowSHL->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL->addUniformVariable("mapSize",
                                                Real32(_shadowVP->getMapSize
                                                       ()));
                _shadowSHL->addUniformVariable("mapFactor",
                                                Real32(mapFactorF[0 +
                                                       (i * 4)]));
                ++uiVarCount;
            }	

            else if(lightOffset == 2)
            {
                _vShadowCmat[uiMatCount]->addChunk(_shadowSHL2);

                if(_activeFactorMap == 0)
                {
                    _vShadowCmat[uiMatCount]->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _vShadowCmat[uiMatCount]->addChunk(_shadowFactorMapO);
                }

                if(_vShadowSHLVar2.size() == uiVarCount2)
                {
                    _vShadowSHLVar2.push_back(SHLVariableChunk::create());
                }

                OSG_ASSERT(uiVarCount2 < _vShadowSHLVar2.size());

                _shadowSHL2->addUniformVariable("oldFactorMap", 2);
                _shadowSHL2->addUniformVariable("shadowMap1", 0);
                _shadowSHL2->addUniformVariable("shadowMap2", 1);
                _shadowSHL2->addUniformVariable("firstRun", _firstRun);
                _shadowSHL2->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 4)]);
                _shadowSHL2->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 4)]);
                _shadowSHL2->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 4)]);
                _shadowSHL2->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 4)]);
                _shadowSHL2->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 4)]);
                _shadowSHL2->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 4)]);
                _shadowSHL2->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL2->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL2->addUniformVariable("mapSize",
                                                 Real32(
                                                 _shadowVP->getMapSize()));
                _shadowSHL2->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        4)]));
                _shadowSHL2->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        4)]));

                ++uiVarCount2;
            }

            else if(lightOffset == 3)
            {
                _vShadowCmat[uiMatCount]->addChunk(_shadowSHL3);

                if(_activeFactorMap == 0)
                {
                    _vShadowCmat[uiMatCount]->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _vShadowCmat[uiMatCount]->addChunk(_shadowFactorMapO);
                }

                if(_vShadowSHLVar3.size() == uiVarCount3)
                {
                    _vShadowSHLVar3.push_back(SHLVariableChunk::create());
                }

                OSG_ASSERT(uiVarCount3 < _vShadowSHLVar3.size());

                _shadowSHL3->addUniformVariable("oldFactorMap", 3);
                _shadowSHL3->addUniformVariable("shadowMap1", 0);
                _shadowSHL3->addUniformVariable("shadowMap2", 1);
                _shadowSHL3->addUniformVariable("shadowMap3", 2);
                _shadowSHL3->addUniformVariable("firstRun", _firstRun);
                _shadowSHL3->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 4)]);
                _shadowSHL3->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 4)]);
                _shadowSHL3->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 4)]);
                _shadowSHL3->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 4)]);
                _shadowSHL3->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 4)]);
                _shadowSHL3->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 4)]);
                _shadowSHL3->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 4)]);
                _shadowSHL3->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 4)]);
                _shadowSHL3->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 4)]);
                _shadowSHL3->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL3->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL3->addUniformVariable("mapSize",
                                                 Real32(
                                                 _shadowVP->getMapSize()));
                _shadowSHL3->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        4)]));
                _shadowSHL3->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        4)]));
                _shadowSHL3->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        4)]));

                ++uiVarCount3;
            }

            else
                //if(lightCounter == 4)
            {
                _vShadowCmat[uiMatCount]->addChunk(_shadowSHL4);

                if(_activeFactorMap == 0)
                {
                    _vShadowCmat[uiMatCount]->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _vShadowCmat[uiMatCount]->addChunk(_shadowFactorMapO);
                }

                if(_vShadowSHLVar4.size() == uiVarCount4)
                {
                    _vShadowSHLVar4.push_back(SHLVariableChunk::create());
                }

                OSG_ASSERT(uiVarCount4 < _vShadowSHLVar4.size());

                _shadowSHL4->addUniformVariable("oldFactorMap", 4);
                _shadowSHL4->addUniformVariable("shadowMap1", 0);
                _shadowSHL4->addUniformVariable("shadowMap2", 1);
                _shadowSHL4->addUniformVariable("shadowMap3", 2);
                _shadowSHL4->addUniformVariable("shadowMap4", 3);
                _shadowSHL4->addUniformVariable("firstRun", _firstRun);
                _shadowSHL4->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("texFactor1", texFactorF[0 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("texFactor2", texFactorF[1 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("texFactor3", texFactorF[2 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("texFactor4", texFactorF[3 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 4)]);
                _shadowSHL4->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL4->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL4->addUniformVariable("mapSize",
                                                 Real32(
                                                 _shadowVP->getMapSize()));
                _shadowSHL4->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        4)]));
                _shadowSHL4->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        4)]));
                _shadowSHL4->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        4)]));
                _shadowSHL4->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        4)]));

                ++uiVarCount4;
            }


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
                pPart->setDrawBuffer  (*buffers);
                
                Node *light  = _shadowVP->_lights[i].first;
                Node *parent =  light->getParent();
                
                if(parent != NULL)
                {
                    a->pushMatrix(parent->getToWorld());
                }
                
                if(_activeFactorMap == 0 && bCA1Cleared == false)
                {
                    pPart->setBackground(_pClearBackground);
                    bCA1Cleared = true;
                }
                else if(bCA2Cleared == false)
                {
                    pPart->setBackground(_pClearBackground);
                    bCA2Cleared = true;
                }

                commitChanges();

                a->overrideMaterial(_vShadowCmat[uiMatCount], a->getActNode());
                a->recurse(light);
                a->overrideMaterial( NULL,                    a->getActNode());

                if(parent != NULL)
                {
                    a->popMatrix();
                }
            }
            a->popPartition();

            _firstRun = 0;
            if(_activeFactorMap == 0)
                _activeFactorMap = 1;
            else
                _activeFactorMap = 0;
            delete[] buffers;

            ++uiMatCount;
        }
    }
    _firstRun = 0;
    shadowIntensityF.clear();
    texFactorF.clear();
    mapFactorF.clear();
    shadowMatrixF.clear();
}


void PCFShadowMapHandler::render(DrawEnv      *pEnv, 
                                 RenderAction *pTmpAction)
{
    Window  *win = pEnv->getWindow();
    initialize(win);

    if(_useGLSL && _useShadowExt)
    {

        glPushAttrib(GL_ENABLE_BIT);

        if(!_initTexturesDone)
            initTextures(pEnv);

        if(_shadowVP->getMapSize() / 4 > _maxPLMapSize)
            _PLMapSize = _maxPLMapSize;
        else
            _PLMapSize = _shadowVP->getMapSize() / 4;

        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            glBindTexture(GL_TEXTURE_2D,
                          pEnv->getWindow()->getGLObjectId(
                          _shadowVP->_vTexChunks[i].pTexO->getGLId()));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                            GL_COMPARE_R_TO_TEXTURE_ARB);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,
                            GL_LEQUAL);
            glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB,
                            GL_LUMINANCE);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        if(_useFBO)
        {
            if(!initFBO(pEnv))
                printf("ERROR with FBOBJECT\n");
        }

        GLfloat globalAmbient[4];
        glGetFloatv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
        GLfloat newGlobalAmbient[] =
        {
            0.0, 0.0, 0.0, 1.0
        };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, newGlobalAmbient);

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
            _width  = pEnv->getPixelWidth();
            _height = pEnv->getPixelHeight();

            if(_useNPOTTextures)
            {
                _colorMapImage->set(GL_RGB, _width, _height);

                _shadowFactorMapImage->set(GL_RGB, _width, _height);

                _shadowFactorMapImage2->set(GL_RGB, _width, _height);

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

                _shadowFactorMapImage2->set(GL_RGB, _widthHeightPOT,
                                            _widthHeightPOT);
            }
        }

        if(_oldRange != _shadowVP->getShadowSmoothness())
        {
            _oldRange = _shadowVP->getShadowSmoothness();
            if(_oldRange <= 0.1999)
            {
                //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
                //_shadowSHL->readFragmentProgram("PCF2_Shadow.frag");
                _shadowSHL->setVertexProgram(_pcf_shadow_vp);
                _shadowSHL->setFragmentProgram(_pcf2_shadow_fp);
		
                //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
                //_shadowSHL2->readFragmentProgram("PCF2_Shadow2.frag");
                _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
                _shadowSHL2->setFragmentProgram(_pcf2_shadow2_fp);

                //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
                //_shadowSHL3->readFragmentProgram("PCF2_Shadow3.frag");
                _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
                _shadowSHL3->setFragmentProgram(_pcf2_shadow3_fp);

                //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
                //_shadowSHL4->readFragmentProgram("PCF2_Shadow4.frag");
                _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
                _shadowSHL4->setFragmentProgram(_pcf2_shadow4_fp);

                //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
                //_shadowCubeSHL->readFragmentProgram("PCF2_CubeShadow.frag");
                _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
                _shadowCubeSHL->setFragmentProgram(_pcf2_shadowCube_fp);
            }
            else if(_oldRange <= 0.3999)
            {
                //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
                //_shadowSHL->readFragmentProgram("PCF3_Shadow.frag");
                _shadowSHL->setVertexProgram(_pcf_shadow_vp);
                _shadowSHL->setFragmentProgram(_pcf3_shadow_fp);

                //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
                //_shadowSHL2->readFragmentProgram("PCF3_Shadow2.frag");
                _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
                _shadowSHL2->setFragmentProgram(_pcf3_shadow2_fp);

                //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
                //_shadowSHL3->readFragmentProgram("PCF3_Shadow3.frag");
                _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
                _shadowSHL3->setFragmentProgram(_pcf3_shadow3_fp);

                //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
                //_shadowSHL4->readFragmentProgram("PCF3_Shadow4.frag");
                _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
                _shadowSHL4->setFragmentProgram(_pcf3_shadow4_fp);

                //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
                //_shadowCubeSHL->readFragmentProgram("PCF3_CubeShadow.frag");
                _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
                _shadowCubeSHL->setFragmentProgram(_pcf3_shadowCube_fp);
            }
            else if(_oldRange <= 0.5999)
            {
                //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
                //_shadowSHL->readFragmentProgram("PCF4_Shadow.frag");
                _shadowSHL->setVertexProgram(_pcf_shadow_vp);
                _shadowSHL->setFragmentProgram(_pcf4_shadow_fp);

                //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
                //_shadowSHL2->readFragmentProgram("PCF4_Shadow2.frag");
                _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
                _shadowSHL2->setFragmentProgram(_pcf4_shadow2_fp);

                //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
                //_shadowSHL3->readFragmentProgram("PCF4_Shadow3.frag");
                _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
                _shadowSHL3->setFragmentProgram(_pcf4_shadow3_fp);

                //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
                //_shadowSHL4->readFragmentProgram("PCF4_Shadow4.frag");
                _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
                _shadowSHL4->setFragmentProgram(_pcf4_shadow4_fp);

                //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
                //_shadowCubeSHL->readFragmentProgram("PCF4_CubeShadow.frag");
                _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
                _shadowCubeSHL->setFragmentProgram(_pcf4_shadowCube_fp);
            }
            else if(_oldRange <= 0.7999)
            {
                //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
                //_shadowSHL->readFragmentProgram("PCF5_Shadow.frag");
                _shadowSHL->setVertexProgram(_pcf_shadow_vp);
                _shadowSHL->setFragmentProgram(_pcf5_shadow_fp);

                //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
                //_shadowSHL2->readFragmentProgram("PCF5_Shadow2.frag");
                _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
                _shadowSHL2->setFragmentProgram(_pcf5_shadow2_fp);

                //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
                //_shadowSHL3->readFragmentProgram("PCF5_Shadow3.frag");
                _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
                _shadowSHL3->setFragmentProgram(_pcf5_shadow3_fp);

                //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
                //_shadowSHL4->readFragmentProgram("PCF5_Shadow4.frag");
                _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
                _shadowSHL4->setFragmentProgram(_pcf5_shadow4_fp);

                //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
                //_shadowCubeSHL->readFragmentProgram("PCF5_CubeShadow.frag");
                _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
                _shadowCubeSHL->setFragmentProgram(_pcf5_shadowCube_fp);
            }
            else
            {
                //_shadowSHL->readVertexProgram("PCF_Shadow.vert");
                //_shadowSHL->readFragmentProgram("PCF6_Shadow.frag");
                _shadowSHL->setVertexProgram(_pcf_shadow_vp);
                _shadowSHL->setFragmentProgram(_pcf6_shadow_fp);

                //_shadowSHL2->readVertexProgram("PCF_Shadow2.vert");
                //_shadowSHL2->readFragmentProgram("PCF6_Shadow2.frag");
                _shadowSHL2->setVertexProgram(_pcf_shadow2_vp);
                _shadowSHL2->setFragmentProgram(_pcf6_shadow2_fp);
		
                //_shadowSHL3->readVertexProgram("PCF_Shadow3.vert");
                //_shadowSHL3->readFragmentProgram("PCF6_Shadow3.frag");
                _shadowSHL3->setVertexProgram(_pcf_shadow3_vp);
                _shadowSHL3->setFragmentProgram(_pcf6_shadow3_fp);

                //_shadowSHL4->readVertexProgram("PCF_Shadow4.vert");
                //_shadowSHL4->readFragmentProgram("PCF6_Shadow4.frag");
                _shadowSHL4->setVertexProgram(_pcf_shadow4_vp);
                _shadowSHL4->setFragmentProgram(_pcf6_shadow4_fp);

                //_shadowCubeSHL->readVertexProgram("PCF_CubeShadow.vert");
                //_shadowCubeSHL->readFragmentProgram("PCF6_CubeShadow.frag");
                _shadowCubeSHL->setVertexProgram(_pcf_shadowCube_vp);
                _shadowCubeSHL->setFragmentProgram(_pcf6_shadowCube_fp);
            }
        }

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


            if(_useFBO && _useNPOTTextures)
            {
                createShadowFactorMapFBO(pEnv, pTmpAction);
            }

            _shadowVP->_trigger_update = false;
        }

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

        setupDrawCombineMap2(pEnv->getAction());

        glPopAttrib();
    }
}

OSG_END_NAMESPACE
