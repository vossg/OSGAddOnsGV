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

#define USE_FBO_FOR_COLOR_AND_FACTOR_MAP


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
#	define	GL_RGBA32F_ARB                      0x8814
#	define	GL_RGB32F_ARB                       0x8815
#	define	GL_ALPHA32F_ARB                     0x8816
#	define	GL_INTENSITY32F_ARB                 0x8817
#	define	GL_LUMINANCE32F_ARB                 0x8818
#	define	GL_LUMINANCE_ALPHA32F_ARB           0x8819
#	define	GL_RGBA16F_ARB                      0x881A
#	define	GL_RGB16F_ARB                       0x881B
#	define	GL_ALPHA16F_ARB                     0x881C
#	define	GL_INTENSITY16F_ARB                 0x881D
#	define	GL_LUMINANCE16F_ARB                 0x881E
#	define	GL_LUMINANCE_ALPHA16F_ARB           0x881F
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
#define GL_RENDERBUFFERwidth_EXT 0x8D42
#define GL_RENDERBUFFERheight_EXT 0x8D43
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

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF


OSG_USING_NAMESPACE


static std::string _depth_vp =
    "varying vec3 lightVec;\n"
    "varying vec4 screenPos;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  lightVec = (gl_ModelViewMatrix * gl_Vertex).xyz;\n"
    "  screenPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _depth_fp =
    "uniform float sceneDiagLength;\n"
    "uniform bool isDirLight;\n"
    "varying vec3 lightVec;\n"
    "varying vec4 screenPos;\n"
    "\n"
    "const float dist_fact = 32.0;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	float x;\n"
    "	if(isDirLight) x = gl_FragCoord.z;\n"
    "	else x = ((length(lightVec)/sceneDiagLength)*2.0)-1.0;\n"
    "	vec2 moments = vec2(x, x*x);\n"
    "	vec2 m1,m2;\n"
    "	m2 = fract(moments * dist_fact);\n"
    "	m1 = moments - (m2 / dist_fact);\n"
    "	gl_FragColor = vec4(m1,m2);\n"
    "}\n";

static std::string _variance_vp =
    "uniform float shadowBias;\n"
    "uniform mat4 lightPM;\n"
    "uniform mat4 lightPM2;\n"
    "uniform float texFactor;\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 texPos;\n"
    "varying vec4 realPos;\n"
    "\n"
#ifndef NO_CONST_GLSL_VAR
    "const "
#endif
    "mat4 bias = mat4(0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.0,0.0,0.0,0.5,0.0,0.5,0.5,0.5,1.0);\n""\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "  realPos = gl_ModelViewMatrix * gl_Vertex;\n"
    "  projCoord = lightPM * realPos;\n"
    "  projCoord.x *= texFactor;\n"
    "  projCoord2 = lightPM2 * realPos;\n"
    "  texPos = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "  projCoord = bias * projCoord;\n"
    "  texPos = bias * texPos;\n"
    "  gl_Position = ftransform();\n"
    "}\n";

static std::string _variance_fp =
    "uniform sampler2D shadowMap;\n"
    "uniform sampler2D oldFactorMap;\n"
    "uniform float intensity;\n"
    "uniform int firstRun;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "uniform float sceneDiagLength;\n"
    "uniform float Lod;\n"
    "uniform bool isDirLight;\n"
    "\n"
    "varying vec4 projCoord;\n"
    "varying vec4 projCoord2;\n"
    "varying vec4 texPos;\n"
    "varying vec4 realPos;\n"
    "\n"
    "const float epsilon = 0.00025;\n"
    "const float dist_factor = 32.0;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "	vec4 projectiveBiased = vec4((projCoord.xyz / projCoord.q),1.0);\n"
    "	float shadowed = 0.0;\n"
    "\n"
    "	vec2 moments = texture2DLod(shadowMap,projectiveBiased.xy,Lod).xy;\n"
    "	moments += texture2DLod(shadowMap,projectiveBiased.xy,Lod).zw / dist_factor;\n"
    "	\n"
    "	float distL;\n"
    "	if(isDirLight) distL = projectiveBiased.z;\n"
    "	else distL = ((length(projCoord2.xyz)/sceneDiagLength)*2.0)-1.0;\n"
    "\n"
    "	if(distL  < moments.x) shadowed = 0.0;\n"
    "	else\n"
    "	{\n"
    "		float E_x2 = moments.y;\n"
    "		float Ex_2 = moments.x * moments.x;\n"
    "		float variance = min(max(E_x2 - Ex_2,epsilon), 1.0);\n"
    "		float m_d = moments.x - distL;\n"
    "		float p_max = variance / (variance + m_d * m_d);\n"
    "		shadowed = 1.0 - p_max;\n"
    "	}\n"
    "	shadowed *= intensity;\n"
    "	if(firstRun == 0) shadowed += texture2DProj(oldFactorMap,vec3(texPos.xy * vec2(xFactor,yFactor),texPos.w)).x;\n"
    "	gl_FragColor = vec4(shadowed,0.0,0.0,1.0);\n"
    "}\n";

VarianceShadowMapHandler::VarianceShadowMapHandler(ShadowStage *source) :
    TreeHandler(source),
    _tiledeco(NULL),
    _colorMapImage(NULL),
    _shadowFactorMapImage(NULL),
    _shadowSHL(NULL),
    _depthCmat(NULL),
    _depthSHL(NULL),
    _shadowCmat(NULL),
    _firstRun(1),
//    _fb(0),
    _fb2(0),
//    _rb_depth(0),
    _rb_depth2(0),
    _texChanged(false),
    _initTexturesDone(false)
{
#if 0
    if(_shadowVP->getParent() != NULL)
    {
        _width = _shadowVP->getPixelWidth();
        _height = _shadowVP->getPixelHeight();
    }

    if(_width == 0)
        _width = 1;
    if(_height == 0)
        _height = 1;

    if(_width > _height)
        _widthHeightPOT = osgNextPower2(_width - 1);
    else
        _widthHeightPOT = osgNextPower2(_height - 1);
#endif

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


#if 0
    if(_useNPOTTextures)
    {
        _colorMapImage->set(GL_RGB, _width, _height);
    }
    else
    {
        _colorMapImage->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);
    }
#endif

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

#if 0
    if(_useNPOTTextures)
    {
        _shadowFactorMapImage->set(GL_RGB, _width, _height);
    }
    else
    {
        _shadowFactorMapImage->set(GL_RGB, _widthHeightPOT, _widthHeightPOT);
    }
#endif

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

    _shadowCmat = ChunkMaterial::create();

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

    _depthCmat = ChunkMaterial::create();

}

VarianceShadowMapHandler::~VarianceShadowMapHandler(void)
{
    _tiledeco        = NULL;

    _shadowSHL       = NULL;
    _depthSHL        = NULL;
    _combineSHL      = NULL;
    _combineDepth    = NULL;
    _combineCmat     = NULL;
    _shadowCmat      = NULL;
    _depthCmat       = NULL;

#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
#if 0
    if(_fb != 0)
        glDeleteFramebuffersEXT(1, &_fb);
    if(_rb_depth != 0)
        glDeleteRenderbuffersEXT(1, &_rb_depth);
#endif
#endif
    if(_fb2 != 0)
        glDeleteFramebuffersEXT(1, &_fb2);
    if(_rb_depth2 != 0)
        glDeleteRenderbuffersEXT(1, &_rb_depth2);
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
            _shadowVP->_texChunks[i]->setImage(_shadowVP->_shadowImages[i]);
            _shadowVP->_texChunks[i]->setInternalFormat(GL_RGBA16F_ARB);
            _shadowVP->_texChunks[i]->setExternalFormat(GL_RGBA);
            _shadowVP->_texChunks[i]->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
            //_shadowVP->_texChunks[i]->setMinFilter(GL_LINEAR);
            _shadowVP->_texChunks[i]->setMagFilter(GL_LINEAR);
            _shadowVP->_texChunks[i]->setAnisotropy(maximumAnistropy);
            _shadowVP->_texChunks[i]->setWrapS(GL_REPEAT);
            _shadowVP->_texChunks[i]->setWrapT(GL_REPEAT);
            _shadowVP->_texChunks[i]->setTarget(GL_TEXTURE_2D);

            UInt32  mSize = _shadowVP->getMapSize();
            if(mSize > 2048)
                mSize = 2048;
            _shadowVP->_shadowImages[i]->set(Image::OSG_RGBA_PF, mSize, mSize,
                                             1, 1, 1, 0, 0,
                                             Image::OSG_FLOAT16_IMAGEDATA,
                                             false);
        }
        _texChanged = true;
    }

    if(_useFBO)
    {
#if 0
        Int32   width = _shadowVP->getPixelWidth();
        Int32   height = _shadowVP->getPixelHeight();
#endif
        Int32   width  = pEnv->getPixelWidth();
        Int32   height = pEnv->getPixelHeight();

        if(width <= 0 || height <= 0)
            return false;

        if(_fb2 != 0)
            return true;

        Window *win = pEnv->getWindow();

        _width  = pEnv->getPixelWidth();
        _height = pEnv->getPixelHeight();

        _colorMapImage->set(GL_RGB, _width, _height);

        _shadowFactorMapImage->set(GL_RGB, _width, _height);

//        _shadowFactorMapImage2->set(GL_RGB, _width, _height);

        commitChanges();

//        glGenFramebuffersEXT(1, &_fb);
//        glGenRenderbuffersEXT(1, &_rb_depth);

        _pFB = FrameBufferObject::create();
        
        _pFB->setSize(_width, _height);

        RenderBufferUnrecPtr pDepthRB = RenderBuffer::create();
        
        pDepthRB->setInternalFormat(GL_DEPTH_COMPONENT24);

//        win->validateGLObject(_colorMapO->getGLId(), pEnv);
//        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
//        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
//                                  GL_COLOR_ATTACHMENT0_EXT,
//                                  GL_TEXTURE_2D,
//                                  win->getGLObjectId(_colorMapO->getGLId()), 
//                                  0);

        TextureBufferUnrecPtr pTexBuffer = TextureBuffer::create();

        pTexBuffer->setTexture(_colorMapO);

        _pFB->setColorAttachment(pTexBuffer, 0);

//       win->validateGLObject(_shadowFactorMapO->getGLId(), pEnv);
//        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, 
//                                  GL_COLOR_ATTACHMENT1_EXT,
//                                  GL_TEXTURE_2D,
//                                  win->getGLObjectId(
//                                      _shadowFactorMapO->getGLId()), 
//                                  0);

        pTexBuffer = TextureBuffer::create();

        pTexBuffer->setTexture(_shadowFactorMapO);

        _pFB->setColorAttachment(pTexBuffer, 1);

#if 0
        //Initialize Depth Renderbuffer
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rb_depth);
        if(_useNPOTTextures)
#if 0
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
                                     GL_DEPTH_COMPONENT24_ARB,
                                     _shadowVP->getPixelWidth(),
                                     _shadowVP->getPixelHeight());
#endif
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
                                     GL_DEPTH_COMPONENT24_ARB, 
                                     pEnv->getPixelWidth(),
                                     pEnv->getPixelHeight());
        else
            glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, 
                                     GL_DEPTH_COMPONENT24_ARB, 
                                     _widthHeightPOT, 
                                     _widthHeightPOT);

        //Attach Renderbuffer to Framebuffer depth Buffer
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, 
                                     GL_DEPTH_ATTACHMENT_EXT, 
                                     GL_RENDERBUFFER_EXT, _rb_depth);
#endif

        _pFB->setDepthAttachment(pDepthRB);

        commitChanges();
        

#if 0
        win->validateGLObject(_colorMapO->getGLId(), pEnv);
        //setTarget(win, win->getGLObjectId(_colorMap->getGLId()), 0, 
        //GL_TEXTURE_2D);

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(_colorMapO->getGLId()), 0);

        win->validateGLObject(_shadowFactorMapO->getGLId(), pEnv);
        //setTarget(win, win->getGLObjectId(_shadowFactorMap->getGLId()), 1, 
        //GL_TEXTURE_2D);

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                                  GL_TEXTURE_2D,
                                  win->getGLObjectId(_shadowFactorMapO->getGLId
                                                     ()), 0);

        bool    result = checkFrameBufferStatus(win);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
#endif

        //Variance Shadow Map Grabbing
        glGenFramebuffersEXT(1, &_fb2);
        glGenRenderbuffersEXT(1, &_rb_depth2);

        UInt32  mSize = _shadowVP->getMapSize();
        if(mSize > 2048)
            mSize = 2048;

        //Initialize Depth Renderbuffer
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb2);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rb_depth2);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24_ARB,
                                 mSize, mSize);

        //Attach Renderbuffer to Framebuffer depth Buffer
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT
                                     , GL_RENDERBUFFER_EXT, _rb_depth2);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

    }

    return true;
}

void VarianceShadowMapHandler::reInit(DrawEnv *pEnv)
{
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
#if 0
    Int32   width = _shadowVP->getPixelWidth();
    Int32   height = _shadowVP->getPixelHeight();
#endif
    Int32   width  = pEnv->getPixelWidth();
    Int32   height = pEnv->getPixelHeight();

    Window *win = pEnv->getWindow();
#if 0
    win->validateGLObject(_colorMapO->getGLId(), pEnv);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              GL_TEXTURE_2D,
                              win->getGLObjectId(_colorMapO->getGLId()), 0);
    win->validateGLObject(_shadowFactorMapO->getGLId(), pEnv);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
                              GL_TEXTURE_2D,
                              win->getGLObjectId(_shadowFactorMapO->getGLId()),
                              0);

    //Initialize Depth Renderbuffer
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, _rb_depth);
    if(_useNPOTTextures)
#if 0
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24_ARB,
                                 _shadowVP->getPixelWidth(),
                                 _shadowVP->getPixelHeight());
#endif
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24_ARB,
                                 pEnv->getPixelWidth(),
                                 pEnv->getPixelHeight());
    else
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24_ARB,
                                 _widthHeightPOT, _widthHeightPOT);

    //Attach Renderbuffer to Framebuffer depth Buffer
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT
                                 , _rb_depth);
#endif
#endif
}

void VarianceShadowMapHandler::initTextures(DrawEnv *pEnv)
{
    _initTexturesDone = true;

#if 0
    Int32   width = _shadowVP->getPixelWidth();
    Int32   height = _shadowVP->getPixelHeight();
#endif
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

void VarianceShadowMapHandler::createShadowMapsFBO(DrawEnv *pEnv, 
                                              RenderAction *pTmpAction)
{
    UInt32  oldWidth, oldHeight;
#if 0
    oldWidth = _shadowVP->getPixelWidth();
    oldHeight = _shadowVP->getPixelHeight();
#endif
    oldWidth = pEnv->getPixelWidth();
    oldHeight = pEnv->getPixelHeight();

    UInt32  mSize = _shadowVP->getMapSize();
    if(mSize > 2048)
        mSize = 2048;
    //------Setting up Window to fit size of ShadowMap----------------

#if 0
#endif

    Viewport *pVP = pTmpAction->getViewport();

#if 0
    _shadowVP->setVPSize(0, 0, mSize - 1, mSize - 1);
#endif
    pVP->setSize(0, 0, mSize - 1, mSize - 1);
    pVP->activate();

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

    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
            {
                //action->getWindow()->validateGLObject(_shadowVP->_texChunks[i]->getGLId());

                GLenum  *buffers = NULL;
                buffers = new GLenum[1];
                buffers[0] = GL_COLOR_ATTACHMENT0_EXT;

                pEnv->getWindow()->validateGLObject(
                    _shadowVP->_texChunks[i]->getGLId(), pEnv);
                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb2);
                glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT
                                          , GL_TEXTURE_2D,
                                          pEnv->getWindow()->getGLObjectId(
                                          _shadowVP->_texChunks[i]->getGLId()),
                                          0);

                //_shadowVP->_texChunks[i]->activate(action, action->getWindow()->getGLObjectId(_shadowVP->_texChunks[i]->getGLId()));
                glBindTexture(GL_TEXTURE_2D,
                              pEnv->getWindow()->getGLObjectId(
                              _shadowVP->_texChunks[i]->getGLId()));
                glGenerateMipmapEXT(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
                //_shadowVP->_texChunks[i]->deactivate(action, action->getWindow()->getGLObjectId(_shadowVP->_texChunks[i]->getGLId()));

                glDrawBuffer(*buffers);

                glClearColor(1.0, 1.0, 1.0, 1.0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0
                pEnv->getAction()->setCamera(_shadowVP->_lightCameras[i]);
#endif
                pTmpAction->setCamera(_shadowVP->_lightCameras[i]);

                Pnt3f   lPos;
                bool    isDirLight;
                Real32  sceneDiagLength;

                if(_shadowVP->_lights[i].second->getType() == PointLight::getClassType
                   ())
                {
                    PointLight *tmpPoint;
                    tmpPoint = dynamic_cast<PointLight *>(_shadowVP->_lights[i].second.get());

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
                    tmpSpot = dynamic_cast<SpotLight *>(_shadowVP->_lights[i].second.get());
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

                _depthSHL->addUniformVariable("sceneDiagLength",
                                               Real32(sceneDiagLength));
                _depthSHL->addUniformVariable("isDirLight", bool(isDirLight));

                _depthCmat->clearChunks();
                _depthCmat->addChunk(_depthSHL);

#if 0
                _shadowVP->renderLight(pEnv->getAction(), _depthCmat, i);
#endif
                _shadowVP->renderLight(pTmpAction, _depthCmat, i);

                glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

                delete[] buffers;

                glClearColor(0.0, 0.0, 0.0, 1.0);

#if 0
                pEnv->getAction()->setCamera(_shadowVP->getCamera());
#endif
                pTmpAction->setCamera(pEnv->getAction()->getCamera());
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

#if 0
    _shadowVP->setVPSize(0, 0, oldWidth - 1, oldHeight - 1);
    _shadowVP->setVPSize(0, 0, 1, 1);
#endif
    pVP->setSize(0, 0, oldWidth - 1, oldHeight - 1);
    pVP->activate();

    pVP->setSize(0, 0, 1, 1);
    pVP->activate();
}

void VarianceShadowMapHandler::createColorMap(DrawEnv *pEnv,
                                            RenderAction *pTmpAction)
{
#if 0
    _shadowVP->getBackground()->clear(pEnv);
#endif
    pEnv->getAction()->getBackground()->clear(pEnv);

#if 0
    pEnv->getAction()->apply(_shadowVP->getRoot());
#endif
    pTmpAction->apply(_shadowVP->getSceneRoot());

    // disable occluded lights.
#if 0
    _shadowVP->checkLightsOcclusion(pEnv->getAction());
#endif
    _shadowVP->checkLightsOcclusion(pTmpAction);

    pEnv->getWindow()->validateGLObject(_colorMapO->getGLId(), pEnv);

    _shadowVP->setReadBuffer(); // set the right read buffer for the copy texture.
    glBindTexture(GL_TEXTURE_2D,
                  pEnv->getWindow()->getGLObjectId(_colorMapO->getGLId()));
#if 0
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,
                        _shadowVP->getPixelWidth(),
                        _shadowVP->getPixelHeight());
#endif
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,
                        pEnv->getPixelWidth(),
                        pEnv->getPixelHeight());
    glBindTexture(GL_TEXTURE_2D, 0);
}


void VarianceShadowMapHandler::createColorMapFBO(DrawEnv *pEnv,
                                            RenderAction *pTmpAction)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Window  *win = pEnv->getWindow();

    GLenum  *buffers = NULL;
    buffers = new GLenum[1];
    buffers[0] = GL_COLOR_ATTACHMENT0_EXT;

    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
    _pFB->activate(pEnv);

    glDrawBuffer(*buffers);

#if 0
    _shadowVP->getBackground()->clear(pEnv);
#endif
    pEnv->getAction()->getBackground()->clear(pEnv);

#if 0
    pEnv->getAction()->apply(_shadowVP->getRoot());
#endif
    pTmpAction->apply(_shadowVP->getSceneRoot());

    // disable occluded lights.
#if 0
    _shadowVP->checkLightsOcclusion(pEnv->getAction());
#endif
    _shadowVP->checkLightsOcclusion(pTmpAction);

    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    _pFB->deactivate(pEnv);

    delete[] buffers;
}

void VarianceShadowMapHandler::createShadowFactorMap(DrawEnv *pEnv,
                                                     RenderAction *pTmpAction,
                                                     UInt32 num)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    if(_firstRun)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                           activeLights);
    else
        shadowIntensity = (_shadowVP->_lights[num].second->getShadowIntensity() /
                           activeLights);
    if(_shadowVP->_lights[num].second->getShadowIntensity() != 0.0 ||
       _shadowVP->getGlobalShadowIntensity() != 0.0)
    {

        Matrix  LVM, LPM, CVM;
#if 0
        _shadowVP->_lightCameras[num]->getViewing(LVM,
                                                  _shadowVP->getPixelWidth(),
                                                  _shadowVP->getPixelHeight());
        _shadowVP->_lightCameras[num]->getProjection(LPM,
                                                     _shadowVP->getPixelWidth
                                                     (),
                                                     _shadowVP->getPixelHeight
                                                     ());
        _shadowVP->getCamera()->getViewing(CVM, _shadowVP->getPixelWidth(),
                                           _shadowVP->getPixelHeight());
#endif
        _shadowVP->_lightCameras[num]->getViewing(LVM,
                                                  pEnv->getPixelWidth(),
                                                  pEnv->getPixelHeight());
        _shadowVP->_lightCameras[num]->getProjection(LPM,
                                                     pEnv->getPixelWidth
                                                     (),
                                                     pEnv->getPixelHeight
                                                     ());
        pEnv->getAction()->getCamera()->getViewing(CVM, 
                                                   pEnv->getPixelWidth(),
                                                   pEnv->getPixelHeight());

        Matrix  iCVM = CVM;
        iCVM.invert();

        Real32  texFactor;
        if(_shadowVP->_lights[num].second->getType() == SpotLight::getClassType() ||
           _shadowVP->_lights[num].second->getType() == PointLight::getClassType())
            texFactor = Real32(_width) / Real32(_height);
        else
            texFactor = 1.0;

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

        if(_shadowVP->_lights[num].second->getType() == PointLight::getClassType())
        {
            PointLight *tmpPoint;
            tmpPoint = dynamic_cast<PointLight *>(_shadowVP->_lights[num].second.get());

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

        else if(_shadowVP->_lights[num].second->getType() == SpotLight::getClassType
                ())
        {
            SpotLight *tmpSpot;
            tmpSpot = dynamic_cast<SpotLight *>(_shadowVP->_lights[num].second.get());
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

        _shadowSHL->addUniformVariable("shadowMap", 0);
        _shadowSHL->addUniformVariable("oldFactorMap", 1);
        _shadowSHL->addUniformVariable("firstRun", _firstRun);
        _shadowSHL->addUniformVariable("intensity", shadowIntensity);
        _shadowSHL->addUniformVariable("texFactor", texFactor);
        //_shadowSHL->addUniformVariable("shadowBias", 0.0075f);
        _shadowSHL->addUniformVariable("lightPM", shadowMatrix);
        _shadowSHL->addUniformVariable("lightPM2", shadowMatrix2);
        //_shadowSHL->addUniformVariable("shadowRange", Real32(shadowRange));
        _shadowSHL->addUniformVariable("xFactor", Real32(xFactor));
        _shadowSHL->addUniformVariable("yFactor", Real32(yFactor));
        _shadowSHL->addUniformVariable("sceneDiagLength",
                                        Real32(sceneDiagLength));
        _shadowSHL->addUniformVariable("Lod", Real32(lod));
        _shadowSHL->addUniformVariable("isDirLight", bool(isDirLight));

        _shadowCmat->clearChunks();
        _shadowCmat->addChunk(_shadowSHL);
        _shadowCmat->addChunk(_shadowVP->_texChunks[num]);
        _shadowCmat->addChunk(_shadowFactorMapO);

#if 0
        _shadowVP->renderLight(pEnv->getAction(), _shadowCmat, num);
#endif
        _shadowVP->renderLight(pTmpAction, _shadowCmat, num);

        pEnv->getWindow()->validateGLObject(_shadowFactorMapO->getGLId(),
                                            pEnv);

        glBindTexture(GL_TEXTURE_2D,
                      pEnv->getWindow()->getGLObjectId(
                      _shadowFactorMapO->getGLId()));
#if 0
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,
                            _shadowVP->getPixelWidth(),
                            _shadowVP->getPixelHeight());
#endif
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0,
                            pEnv->getPixelWidth(),
                            pEnv->getPixelHeight());
        glBindTexture(GL_TEXTURE_2D, 0);

        _firstRun = 0;
    }
}

void VarianceShadowMapHandler::createShadowFactorMapFBO(DrawEnv *pEnv,
                                                        RenderAction *pTmpAction,
                                                        UInt32 num)
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
        shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                           activeLights);
    else
        shadowIntensity = (_shadowVP->_lights[num].second->getShadowIntensity() /
                           activeLights);
    if(_shadowVP->_lights[num].second->getShadowIntensity() != 0.0 ||
       _shadowVP->getGlobalShadowIntensity() != 0.0)
    {

        Matrix  LVM, LPM, CVM;
#if 0
        _shadowVP->_lightCameras[num]->getViewing(LVM,
                                                  _shadowVP->getPixelWidth(),
                                                  _shadowVP->getPixelHeight());
        _shadowVP->_lightCameras[num]->getProjection(LPM,
                                                     _shadowVP->getPixelWidth
                                                     (),
                                                     _shadowVP->getPixelHeight
                                                     ());
        _shadowVP->getCamera()->getViewing(CVM, _shadowVP->getPixelWidth(),
                                           _shadowVP->getPixelHeight());
#endif
        _shadowVP->_lightCameras[num]->getViewing(LVM,
                                                  pEnv->getPixelWidth(),
                                                  pEnv->getPixelHeight());
        _shadowVP->_lightCameras[num]->getProjection(LPM,
                                                     pEnv->getPixelWidth
                                                     (),
                                                     pEnv->getPixelHeight
                                                     ());
        pEnv->getAction()->getCamera()->getViewing(CVM,
                                                   pEnv->getPixelWidth(),
                                                   pEnv->getPixelHeight());
        Matrix  iCVM = CVM;
        iCVM.invert();

        Real32  texFactor;
        if(_shadowVP->_lights[num].second->getType() == SpotLight::getClassType() ||
           _shadowVP->_lights[num].second->getType() == PointLight::getClassType())
            texFactor = Real32(_width) / Real32(_height);
        else
            texFactor = 1.0;

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

        if(_shadowVP->_lights[num].second->getType() == PointLight::getClassType())
        {
            PointLight *tmpPoint;
            tmpPoint = dynamic_cast<PointLight *>(_shadowVP->_lights[num].second.get());

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

        else if(_shadowVP->_lights[num].second->getType() == SpotLight::getClassType
                ())
        {
            SpotLight *tmpSpot;
            tmpSpot = dynamic_cast<SpotLight *>(_shadowVP->_lights[num].second.get());
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

        _shadowSHL->addUniformVariable("shadowMap", 0);
        _shadowSHL->addUniformVariable("oldFactorMap", 1);
        _shadowSHL->addUniformVariable("firstRun", _firstRun);
        _shadowSHL->addUniformVariable("intensity", shadowIntensity);
        _shadowSHL->addUniformVariable("texFactor", texFactor);
        //_shadowSHL->addUniformVariable("shadowBias", 0.0075f);
        _shadowSHL->addUniformVariable("lightPM", shadowMatrix);
        _shadowSHL->addUniformVariable("lightPM2", shadowMatrix2);
        //_shadowSHL->addUniformVariable("shadowRange", Real32(shadowRange));
        _shadowSHL->addUniformVariable("xFactor", Real32(xFactor));
        _shadowSHL->addUniformVariable("yFactor", Real32(yFactor));
        _shadowSHL->addUniformVariable("sceneDiagLength",
                                        Real32(sceneDiagLength));
        _shadowSHL->addUniformVariable("Lod", Real32(lod));
        _shadowSHL->addUniformVariable("isDirLight", bool(isDirLight));

        _shadowCmat->clearChunks();
        _shadowCmat->addChunk(_shadowSHL);
        _shadowCmat->addChunk(_shadowVP->_texChunks[num]);
        _shadowCmat->addChunk(_shadowFactorMapO);

        GLenum  *buffers = NULL;
        buffers = new GLenum[1];
        buffers[0] = GL_COLOR_ATTACHMENT1_EXT;

        //Setup FBO
        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, _fb);
        _pFB->activate(pEnv);

        glDrawBuffer(*buffers);

        //draw the Scene
        if(_firstRun)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 0
        _shadowVP->renderLight(pEnv->getAction(), _shadowCmat, num);
#endif
        _shadowVP->renderLight(pTmpAction, _shadowCmat, num);

        //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        _pFB->deactivate(pEnv);

        delete[] buffers;
        _firstRun = 0;
    }
}


void VarianceShadowMapHandler::render(DrawEnv *pEnv, 
                                      RenderAction *pTmpAction)
{
    Window  *win = pEnv->getWindow();
    initialize(win);

    if(!_useGLSL || !_useShadowExt || !_useFBO)
    {
#if 0
        _shadowVP->Viewport::render(pEnv->getAction());
#endif
    }
    else
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

        if(_shadowVP->getMapAutoUpdate())
        {
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
            if(_useNPOTTextures)
                createColorMapFBO(pEnv, pTmpAction);
            else
#endif
                createColorMap(pEnv, pTmpAction);

            //deactivate transparent Nodes
            for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                _shadowVP->_transparent[t]->setTravMask(0);

            createShadowMapsFBO(pEnv, pTmpAction);

            // switch on all transparent geos
            for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                _shadowVP->_transparent[t]->setTravMask(TypeTraits<UInt32>::BitsSet);
            //filterShadowMaps(pEnv);

            for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
            {
                if(_shadowVP->_lightStates[i] != 0)
                {
                    if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                       _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
                    {
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
                        if(_useNPOTTextures)
                            createShadowFactorMapFBO(pEnv, pTmpAction, i);
                        else
#endif
                            createShadowFactorMap(pEnv, pTmpAction, i);
                    }
                }
            }
        }
        else
        {
            if(_shadowVP->_trigger_update)
            {
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
                if(_useNPOTTextures)
                    createColorMapFBO(pEnv, pTmpAction);
                else
#endif
                    createColorMap(pEnv, pTmpAction);

                //deactivate transparent Nodes
                for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                    _shadowVP->_transparent[t]->setTravMask(0);

                createShadowMapsFBO(pEnv, pTmpAction);

                // switch on all transparent geos
                for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
                    _shadowVP->_transparent[t]->setTravMask(TypeTraits<UInt32>::BitsSet);
                //filterShadowMaps(pEnv);

                for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
                {
                    if(_shadowVP->_lightStates[i] != 0)
                    {
                        if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                           _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
                        {
#ifdef USE_FBO_FOR_COLOR_AND_FACTOR_MAP
                            if(_useNPOTTextures)
                                createShadowFactorMapFBO(pEnv, pTmpAction, i);
                            else
#endif
                                createShadowFactorMap(pEnv, pTmpAction, i);
                        }
                    }
                }
                _shadowVP->_trigger_update = false;
            }
        }

        setupDrawCombineMap1(pEnv->getAction());
            
        glPopAttrib();

#if 0
        // render the foregrounds.
        for(UInt16 i = 0;i < _shadowVP->getMFForegrounds()->size();++i)
        {
            _shadowVP->getForegrounds(i)->draw(pEnv, _shadowVP);
        }
#endif
    }
}
