#include <stdlib.h>
#include <stdio.h>
#include <OSGConfig.h>
#include <OSGQuaternion.h>
#include <OSGRenderAction.h>
#include <OSGMatrix.h>
#include <OSGMatrixUtility.h>
#include <OSGBackground.h>
#include <OSGForeground.h>
#include <OSGImage.h>
#include <OSGLight.h>
#include "OSGTreeHandler.h"
#include "OSGShadowStage.h"

#define PI 3.14159265f

//--------------------------------------------------------------------
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
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT 0x8CD8
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
#define GL_ARB_draw_buffers 1



OSG_USING_NAMESPACE

UInt32 TreeHandler::_depth_texture_extension;
UInt32 TreeHandler::_shadow_extension;
UInt32 TreeHandler::_framebuffer_object_extension;
UInt32 TreeHandler::_draw_buffers_extension;
UInt32 TreeHandler::_funcDrawBuffers =                         Window::invalidFunctionID;
UInt32 TreeHandler::_funcBindFramebuffer =                     Window::invalidFunctionID;
UInt32 TreeHandler::_funcBindRenderbuffer =                    Window::invalidFunctionID;
UInt32 TreeHandler::_funcCheckFramebufferStatus =              Window::invalidFunctionID;
UInt32 TreeHandler::_funcDeleteFramebuffers =                  Window::invalidFunctionID;
UInt32 TreeHandler::_funcDeleteRenderbuffers =                 Window::invalidFunctionID;
UInt32 TreeHandler::_funcFramebufferRenderbuffer =             Window::invalidFunctionID;
UInt32 TreeHandler::_funcFramebufferTexture1D =                Window::invalidFunctionID;
UInt32 TreeHandler::_funcFramebufferTexture2D =                Window::invalidFunctionID;
UInt32 TreeHandler::_funcFramebufferTexture3D =                Window::invalidFunctionID;
UInt32 TreeHandler::_funcGenFramebuffers =                     Window::invalidFunctionID;
UInt32 TreeHandler::_funcGenRenderbuffers =                    Window::invalidFunctionID;
UInt32 TreeHandler::_funcGenerateMipmap =                      Window::invalidFunctionID;
UInt32 TreeHandler::_funcGetFramebufferAttachmentParameteriv = Window::invalidFunctionID;
UInt32 TreeHandler::_funcGetRenderbufferParameteriv =          Window::invalidFunctionID;
UInt32 TreeHandler::_funcIsFramebuffer =                       Window::invalidFunctionID;
UInt32 TreeHandler::_funcIsRenderbuffer =                      Window::invalidFunctionID;
UInt32 TreeHandler::_funcRenderbufferStorage =                 Window::invalidFunctionID;

std::string TreeHandler::_shadow_combine_vp =
    "varying vec2 texCoord;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    texCoord = gl_MultiTexCoord0.xy;\n"
    "    gl_Position = ftransform();\n"
    "}\n";

std::string TreeHandler::_shadow_combine_fp =
    "uniform sampler2D colorMap;\n"
    "uniform sampler2D shadowFactorMap;\n"
    "uniform float xFactor;\n"
    "uniform float yFactor;\n"
    "uniform bool hasFactorMap;\n"
    "varying vec2 texCoord;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    vec2 tc = texCoord * vec2(xFactor, yFactor);\n"
    "    vec3 color = texture2D(colorMap, tc).rgb;\n"
    "    color *= hasFactorMap ? (1.0 - texture2D(shadowFactorMap, tc).r) : 1.0;\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

TreeHandler::TreeHandler(ShadowStage *source) :
    _initDone(false),
    _shadowVP(source),
    _maxPLMapSize(0),
    _PLMapSize(1),
    _maxTexSize(0),
    _unlitMat(NULL),

    _width(1),
    _height(1),
    _widthHeightPOT(0),
    _combineSHL(NULL),
    _combineDepth(NULL),
    _combineCmat(NULL),
    _colorMapO(NULL),
    _activeFactorMap(1),
    _shadowFactorMapO(NULL),
    _shadowFactorMap2O(NULL),
    _pFB(NULL),
    _pFB2(NULL)

{
    GLint   max_tex_size = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);

    _maxTexSize = max_tex_size;
    _maxPLMapSize = _maxTexSize / 4;

    _depth_texture_extension = Window::registerExtension(
        "GL_ARB_depth_texture");

    _shadow_extension = Window::registerExtension("GL_ARB_shadow");

    _framebuffer_object_extension = Window::registerExtension(
        "GL_EXT_framebuffer_object");

    _draw_buffers_extension = Window::registerExtension("GL_ARB_draw_buffers");

    _funcDrawBuffers =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glDrawBuffers", GL_ARB_draw_buffers);

    _funcBindFramebuffer =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glBindFramebufferEXT",
                                  _framebuffer_object_extension);

    _funcBindRenderbuffer =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glBindRenderbufferEXT",
                                  _framebuffer_object_extension);

    _funcCheckFramebufferStatus =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glCheckFramebufferStatusEXT",
                                  _framebuffer_object_extension);

    _funcDeleteFramebuffers =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glDeleteFramebuffersEXT",
                                  _framebuffer_object_extension);

    _funcDeleteRenderbuffers =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glDeleteRenderbuffersEXT",
                                  _framebuffer_object_extension);

    _funcFramebufferRenderbuffer =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glFramebufferRenderbufferEXT",
                                  _framebuffer_object_extension);

    _funcFramebufferTexture1D =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glFramebufferTexture1DEXT",
                                  _framebuffer_object_extension);

    _funcFramebufferTexture2D =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glFramebufferTexture2DEXT",
                                  _framebuffer_object_extension);

    _funcFramebufferTexture3D =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glFramebufferTexture3DEXT",
                                  _framebuffer_object_extension);

    _funcGenFramebuffers =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glGenFramebuffersEXT",
                                  _framebuffer_object_extension);

    _funcGenRenderbuffers =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glGenRenderbuffersEXT",
                                  _framebuffer_object_extension);

    _funcGenerateMipmap =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glGenerateMipmapEXT",
                                  _framebuffer_object_extension);

    _funcGetFramebufferAttachmentParameteriv =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glGetFramebufferAttachmentParameterivEXT",
                                  _framebuffer_object_extension);

    _funcGetRenderbufferParameteriv =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glGetRenderbufferParameterivEXT",
                                  _framebuffer_object_extension);

    _funcIsFramebuffer =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glIsFramebufferEXT",
                                  _framebuffer_object_extension);

    _funcIsRenderbuffer =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glIsRenderbufferEXT",
                                  _framebuffer_object_extension);

    _funcRenderbufferStorage =
        Window::registerFunction (OSG_DLSYM_UNDERSCORE"glRenderbufferStorageEXT",
                                  _framebuffer_object_extension);


    _unlitMat = SimpleMaterial::create();

    _unlitMat->setLit(false);

    _pClearBackground = SolidBackground::create();
}


TreeHandler::~TreeHandler(void)
{
    _unlitMat          = NULL;

    _combineSHL        = NULL;
    _combineDepth      = NULL;
    _combineCmat       = NULL;
    _colorMapO         = NULL;
    _shadowFactorMapO  = NULL;
    _shadowFactorMap2O = NULL;
 
    _pFB               = NULL;
    _pFB2              = NULL;

    _pClearBackground  = NULL;
}

void TreeHandler::initialize(Window *win)
{
    if(!_initDone)
    {
        // without this the registered extensions are not valid yet!
        win->frameInit();

        //check support for ShadowExtension
        if(!win->hasExtension(_depth_texture_extension))
        {
            SWARNING <<
                "No ARB_depth_texture-Extension available! All shadow modes disabled." << endLog;
        }
        else if(!win->hasExtension(_shadow_extension))
        {
            SWARNING <<
                "No ARB_shadow-Extension available! All shadow modes disabled."
                << endLog;
        }


        GLenum  errCode;
        bool    FBOerror = false;

        glBindFramebufferEXT =
            reinterpret_cast<OSGGLBINDFRAMEBUFFEREXTPROC>(
                win->getFunction(_funcBindFramebuffer));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glBindRenderbufferEXT =
            reinterpret_cast<OSGGLBINDRENDERBUFFEREXTPROC>(
                win->getFunction(_funcBindRenderbuffer));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;
        
        glCheckFramebufferStatusEXT =
            reinterpret_cast<OSGGLCHECKFRAMEBUFFERSTATUSEXTPROC>(
                win->getFunction(_funcCheckFramebufferStatus));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glDeleteFramebuffersEXT =
            reinterpret_cast<OSGGLDELETEFRAMEBUFFERSEXTPROC>(
                win->getFunction(_funcDeleteFramebuffers));
        
        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glDeleteRenderbuffersEXT =
            reinterpret_cast<OSGGLDELETERENDERBUFFERSEXTPROC>(
                win->getFunction(_funcDeleteRenderbuffers));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glFramebufferRenderbufferEXT =
            reinterpret_cast<OSGGLFRAMEBUFFERRENDERBUFFEREXTPROC>(
                win->getFunction(_funcFramebufferRenderbuffer));
        
        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;
        
        glFramebufferTexture1DEXT =
            reinterpret_cast<OSGGLFRAMEBUFFERTEXTURE1DEXTPROC>(
                win->getFunction(_funcFramebufferTexture1D));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glFramebufferTexture2DEXT =
            reinterpret_cast<OSGGLFRAMEBUFFERTEXTURE2DEXTPROC>(
                win->getFunction(_funcFramebufferTexture2D));
        
        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;
        
        glFramebufferTexture3DEXT =
            reinterpret_cast<OSGGLFRAMEBUFFERTEXTURE3DEXTPROC>(
                win->getFunction(_funcFramebufferTexture3D));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glGenFramebuffersEXT =
            reinterpret_cast<OSGGLGENFRAMEBUFFERSEXTPROC>(
                win->getFunction(_funcGenFramebuffers));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;
        
        glGenRenderbuffersEXT =
            reinterpret_cast<OSGGLGENRENDERBUFFERSEXTPROC>(
                win->getFunction(_funcGenRenderbuffers));
        
        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glGenerateMipmapEXT =
            reinterpret_cast<OSGGLGENERATEMIPMAPEXTPROC>(
                win->getFunction(_funcGenerateMipmap));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glGetFramebufferAttachmentParameterivEXT =
            reinterpret_cast<
            OSGGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC>(
                win->getFunction(
                    _funcGetFramebufferAttachmentParameteriv));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glGetRenderbufferParameterivEXT =
            reinterpret_cast<OSGGLGETRENDERBUFFERPARAMETERIVEXTPROC>(
                win->getFunction(_funcGetRenderbufferParameteriv));
        
        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;
        
        glIsFramebufferEXT =
            reinterpret_cast<OSGGLISFRAMEBUFFEREXTPROC>(
                win->getFunction(_funcIsFramebuffer));
        
        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;
        
        glIsRenderbufferEXT =
            reinterpret_cast<OSGGLISRENDERBUFFEREXTPROC>(
                win->getFunction(_funcIsRenderbuffer));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glRenderbufferStorageEXT =
            reinterpret_cast<OSGGLRENDERBUFFERSTORAGEEXTPROC>(
                win->getFunction(_funcRenderbufferStorage));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        glDrawBuffersARB =
            reinterpret_cast<OSGGLDRAWBUFFERSARBPROC>(
                win->getFunction(_funcDrawBuffers));

        if((errCode = glGetError()) != GL_NO_ERROR)
            FBOerror = true;

        GLenum  status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        
        if(FBOerror)
        {
            FNOTICE(
                (
                    "Needed FBO functions could not be initialized (error code %d), FBOs disabled. Try new video drivers!\n", errCode));
        }

        switch(status)
        {
            case GL_FRAMEBUFFER_COMPLETE_EXT:
                FINFO(("%x: framebuffer complete!\n", status));
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
                FWARNING(
                    ("%x: framebuffer GL_FRAMEBUFFER_UNSUPPORTED_EXT\n",
                     status));
                break;
            default:
                break;
        }
        _initDone = true;
    }
}

Material *TreeHandler::getUnlitMaterial(void)
{
    return _unlitMat;
}

bool TreeHandler::hasFactorMap(void)
{
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if (_shadowVP->_lightStates[i] != 0 &&
            (_shadowVP->_lights[i].second->getShadowIntensity() != 0.0 ||
			_shadowVP->getGlobalShadowIntensity() != 0.0))
        {
            return true;
        }
    }

    return false;
}











void TreeHandler::setupDrawCombineMap2(Action  *pAction)
{
    RenderAction *a = dynamic_cast<RenderAction *>(pAction);
    
    a->pushPartition((RenderPartition::CopyWindow      |
                      RenderPartition::CopyViewportSize),
                     RenderPartition::SimpleCallback);
    {
        RenderPartition *pPart  = a->getActivePartition();
        Matrix m, t;
        
        m.setIdentity();
        t.setIdentity();
        
        MatrixOrthogonal( m,
                          0.f, 1.f,
                          0.f, 1.f,
                         -1.f, 1.f);
        
        pPart->setupProjection(m, t);
        
        RenderPartition::SimpleDrawCallback f;
        
        f = boost::bind(&TreeHandler::doDrawCombineMap2, this, _1);
        
        pPart->dropFunctor(f);
    }
    a->popPartition();
}

void TreeHandler::doDrawCombineMap2(DrawEnv *pEnv)
{
    Real32  xFactor = 1.0f;
    Real32  yFactor = 1.0f;

    _combineCmat->clearChunks();

    _combineCmat->addChunk(_combineSHL);
    _combineCmat->addChunk(_colorMapO);

    if(_activeFactorMap == 0)
    {
        _combineCmat->addChunk(_shadowFactorMap2O);
    }
    else
    {
        _combineCmat->addChunk(_shadowFactorMapO);
    }

    _combineCmat->addChunk(_combineDepth);

    _combineSHL->addUniformVariable("colorMap",        0);
    _combineSHL->addUniformVariable("shadowFactorMap", 1);
    _combineSHL->addUniformVariable("xFactor",         Real32(xFactor));
    _combineSHL->addUniformVariable("yFactor",         Real32(yFactor));
    _combineSHL->addUniformVariable("hasFactorMap",    hasFactorMap());

    commitChanges();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();

    State *pCombineState = _combineCmat->getState();

    pEnv->activateState(pCombineState, NULL);

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.00, 0.00);
        glVertex2f  (0.00, 0.00);
        
        glTexCoord2f(1.00, 0.00);
        glVertex2f  (1.00, 0.00);
        
        glTexCoord2f(1.00, 1.00);
        glVertex2f  (1.00, 1.00);
        
        glTexCoord2f(0.00, 1.00);
        glVertex2f  (0.00, 1.00);
    }
    glEnd();

    pEnv->deactivateState();

    glPopMatrix();
}


void TreeHandler::setupDrawCombineMap1(Action  *pAction)
{
    RenderAction *a = dynamic_cast<RenderAction *>(pAction);
    
    a->pushPartition((RenderPartition::CopyWindow      |
                      RenderPartition::CopyViewportSize),
                     RenderPartition::SimpleCallback);
    {
        RenderPartition *pPart  = a->getActivePartition();
        Matrix m, t;
        
        m.setIdentity();
        t.setIdentity();
        
        MatrixOrthogonal( m,
                          0.f, 1.f,
                          0.f, 1.f,
                         -1.f, 1.f);
        
        pPart->setupProjection(m, t);
        
        RenderPartition::SimpleDrawCallback f;
        
        f = boost::bind(&TreeHandler::doDrawCombineMap1, this, _1);
        
        pPart->dropFunctor(f);
    }
    a->popPartition();
}

void TreeHandler::doDrawCombineMap1(DrawEnv *pEnv)
{
    Real32  xFactor = 1.0f;
    Real32  yFactor = 1.0f;

    _combineSHL->addUniformVariable("colorMap",        0);
    _combineSHL->addUniformVariable("shadowFactorMap", 1);
    _combineSHL->addUniformVariable("xFactor",         Real32(xFactor));
    _combineSHL->addUniformVariable("yFactor",         Real32(yFactor));
    _combineSHL->addUniformVariable("hasFactorMap",    hasFactorMap());

    commitChanges();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();

    State *pCombineState = _combineCmat->getState();

    pEnv->activateState(pCombineState, NULL);

    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.00, 0.00);
        glVertex2f  (0.00, 0.00);
        
        glTexCoord2f(1.00, 0.00);
        glVertex2f  (1.00, 0.00);
        
        glTexCoord2f(1.00, 1.00);
        glVertex2f  (1.00, 1.00);
        
        glTexCoord2f(0.00, 1.00);
        glVertex2f  (0.00, 1.00);
    }
    glEnd();

    pEnv->deactivateState();

    glPopMatrix();
}
