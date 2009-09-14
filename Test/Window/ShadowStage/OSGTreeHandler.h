#ifndef _OSGTREEHANDLER_H_
#define _OSGTREEHANDLER_H_
#ifdef __sgi
#pragma once
#endif

#include <vector>
#include <OSGConfig.h>
#include <OSGAction.h>
#include <OSGRenderActionBase.h>
#include <OSGSpotLight.h>
#include <OSGDirectionalLight.h>
#include <OSGPerspectiveCamera.h>
#include <OSGMatrixCamera.h>
#include <OSGTransform.h>
#include <OSGTextureChunk.h>
#include <OSGPassiveBackground.h>
#include <OSGTexGenChunk.h>
#include <OSGTextureTransformChunk.h>
#include <OSGPolygonChunk.h>
#include <OSGBlendChunk.h>
#include <OSGTileCameraDecorator.h>
#include <OSGSimpleMaterial.h>


#include "OSGDepthChunk.h"
#include "OSGSHLChunk.h"
#include "OSGTextureObjChunk.h"
#include "OSGTextureEnvChunk.h"
#include "OSGFrameBufferObject.h"

OSG_BEGIN_NAMESPACE

#ifdef __APPLE__
#define NO_CONST_GLSL_VAR
#endif

class ShadowStage;
class DrawEnv;
class RenderAction;

class OSG_WINDOW_DLLMAPPING TreeHandler
{

/*==========================  PUBLIC  =================================*/
  public:

    TreeHandler(ShadowStage *source);
    
    virtual ~TreeHandler(void);
    
    virtual void render(DrawEnv      *pEnv,
                        RenderAction *pTmpAction)=0;

    Material *getUnlitMaterial(void);

    typedef void (OSG_APIENTRY * OSGGLDRAWBUFFERSARBPROC)
        (GLsizei n, const GLenum* bufs);
    typedef void (OSG_APIENTRY * OSGGLBINDFRAMEBUFFEREXTPROC)
        (GLenum target, GLuint framebuffer);
    typedef void (OSG_APIENTRY * OSGGLBINDRENDERBUFFEREXTPROC)
        (GLenum target, GLuint renderbuffer);
    typedef GLenum (OSG_APIENTRY * OSGGLCHECKFRAMEBUFFERSTATUSEXTPROC)
        (GLenum target);
    typedef void (OSG_APIENTRY * OSGGLDELETEFRAMEBUFFERSEXTPROC)
        (GLsizei n, const GLuint* framebuffers);
    typedef void (OSG_APIENTRY * OSGGLDELETERENDERBUFFERSEXTPROC)
        (GLsizei n, const GLuint* renderbuffers);
    typedef void (OSG_APIENTRY * OSGGLFRAMEBUFFERRENDERBUFFEREXTPROC)
        (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
    typedef void (OSG_APIENTRY * OSGGLFRAMEBUFFERTEXTURE1DEXTPROC)
        (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    typedef void (OSG_APIENTRY * OSGGLFRAMEBUFFERTEXTURE2DEXTPROC)
        (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
    typedef void (OSG_APIENTRY * OSGGLFRAMEBUFFERTEXTURE3DEXTPROC)
        (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
    typedef void (OSG_APIENTRY * OSGGLGENFRAMEBUFFERSEXTPROC)
        (GLsizei n, GLuint* framebuffers);
    typedef void (OSG_APIENTRY * OSGGLGENRENDERBUFFERSEXTPROC)
        (GLsizei n, GLuint* renderbuffers);
    typedef void (OSG_APIENTRY * OSGGLGENERATEMIPMAPEXTPROC)
        (GLenum target);
    typedef void (OSG_APIENTRY * OSGGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)
        (GLenum target, GLenum attachment, GLenum pname, GLint* params);
    typedef void (OSG_APIENTRY * OSGGLGETRENDERBUFFERPARAMETERIVEXTPROC)
        (GLenum target, GLenum pname, GLint* params);
    typedef GLboolean (OSG_APIENTRY * OSGGLISFRAMEBUFFEREXTPROC)
        (GLuint framebuffer);
    typedef GLboolean (OSG_APIENTRY * OSGGLISRENDERBUFFEREXTPROC)
        (GLuint renderbuffer);
    typedef void (OSG_APIENTRY * OSGGLRENDERBUFFERSTORAGEEXTPROC)
        (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

  protected:

    OSGGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
    OSGGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
    OSGGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
    OSGGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
    OSGGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
    OSGGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
    OSGGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT;
    OSGGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
    OSGGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT;
    OSGGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
    OSGGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
    OSGGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;
    OSGGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
    OSGGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
    OSGGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT;
    OSGGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT;
    OSGGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
    OSGGLDRAWBUFFERSARBPROC glDrawBuffersARB;

    static UInt32 _depth_texture_extension;
    static UInt32 _shadow_extension;
    static UInt32 _framebuffer_object_extension;
    static UInt32 _draw_buffers_extension;
    static UInt32 _funcDrawBuffers;
    static UInt32 _funcBindFramebuffer;
    static UInt32 _funcBindRenderbuffer;
    static UInt32 _funcCheckFramebufferStatus;
    static UInt32 _funcDeleteFramebuffers;
    static UInt32 _funcDeleteRenderbuffers;
    static UInt32 _funcFramebufferRenderbuffer;
    static UInt32 _funcFramebufferTexture1D;
    static UInt32 _funcFramebufferTexture2D;
    static UInt32 _funcFramebufferTexture3D;
    static UInt32 _funcGenFramebuffers;
    static UInt32 _funcGenRenderbuffers;
    static UInt32 _funcGenerateMipmap;
    static UInt32 _funcGetFramebufferAttachmentParameteriv;
    static UInt32 _funcGetRenderbufferParameteriv;
    static UInt32 _funcIsFramebuffer;
    static UInt32 _funcIsRenderbuffer;
    static UInt32 _funcRenderbufferStorage;

    static std::string _shadow_combine_vp;
    static std::string _shadow_combine_fp;

    // reference to parent
    ShadowStage *_shadowVP;

    bool _useFBO;
    bool _useNPOTTextures;
    bool _useGLSL;
    bool _useShadowExt;
    bool _useShaderModel3;
    UInt32 _maxPLMapSize;
    UInt32 _PLMapSize;
    UInt32 _maxTexSize;
    SimpleMaterialUnrecPtr _unlitMat;

    void initialize(Window *win);
    bool hasFactorMap(void);

    // New

    UInt32                       _width;
    UInt32                       _height;
    UInt32                       _widthHeightPOT;

    SHLChunkUnrecPtr             _combineSHL;
    DepthChunkUnrecPtr           _combineDepth;
    ChunkMaterialUnrecPtr        _combineCmat;
    TextureObjChunkUnrecPtr      _colorMapO;
    UInt32                       _activeFactorMap;

    TextureObjChunkUnrecPtr      _shadowFactorMapO;
    TextureObjChunkUnrecPtr      _shadowFactorMap2O;

    FrameBufferObjectUnrecPtr    _pFB;
    FrameBufferObjectUnrecPtr    _pFB2;

    void setupDrawCombineMap1(Action  *pAction);
    void doDrawCombineMap1   (DrawEnv *pEnv   );

    void setupDrawCombineMap2(Action  *pAction);
    void doDrawCombineMap2   (DrawEnv *pEnv   );

private:

    bool _initDone;
};

OSG_END_NAMESPACE

#endif /* _OSGTREEHANDLER_H_ */
