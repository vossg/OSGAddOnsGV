#ifndef _OSGSTDSHADOWMAP_H_
#define _OSGSTDSHADOWMAP_H_
#ifdef __sgi
#pragma once
#endif

#include <vector>
#include "OSGConfig.h"
#include "OSGAction.h"
#include "OSGRenderActionBase.h"
#include "OSGSpotLight.h"
#include "OSGDirectionalLight.h"
#include "OSGPerspectiveCamera.h"
#include "OSGMatrixCamera.h"
#include "OSGTransform.h"
#include "OSGTextureChunk.h"
#include "OSGSimpleMaterial.h"
#include "OSGPassiveBackground.h"
#include "OSGSolidBackground.h"
#include "OSGChunkMaterial.h"
#include "OSGMaterialChunk.h"
#include "OSGMaterialGroup.h"
#include "OSGSHLChunk.h"
#include "OSGForeground.h"
#include "OSGPolygonForeground.h"
#include "OSGGrabForeground.h"
#include "OSGTextureGrabForeground.h"
#include "OSGFileGrabForeground.h"
#include "OSGImageForeground.h"
#include "OSGTexGenChunk.h"
#include "OSGTextureTransformChunk.h"
#include "OSGPolygonChunk.h"
#include "OSGBlendChunk.h"
#include "OSGSimpleTexturedMaterial.h"
#include "OSGTileCameraDecorator.h"
#include "OSGPolygonForeground.h"
#include "OSGDepthChunk.h"

#include "OSGTreeRenderer.h"

OSG_BEGIN_NAMESPACE

class ShadowViewport;
class TreeRenderer;
class DrawEnv;

class OSG_WINDOW_DLLMAPPING StdShadowMap : public TreeRenderer
{
  public:

    StdShadowMap(ShadowViewport *source);
    ~StdShadowMap(void);
    virtual void render(DrawEnv *pEnv);

  private:

    bool initFBO(DrawEnv *pEnv);
    void reInit(DrawEnv *pEnv);
    
    void initTextures(Window *win);
    bool checkFrameBufferStatus(Window *win);

    void createShadowMapsFBO(DrawEnv *pEnv);
    void createShadowMaps(DrawEnv *pEnv);
    void createShadowMapsNOGLSL(DrawEnv *pEnv);
    void projectShadowMaps(DrawEnv *pEnv);
    void createColorMapFBO(DrawEnv *pEnv);
    void createShadowFactorMapFBO(DrawEnv *pEnv);
    void createShadowFactorMap(DrawEnv *pEnv);
    void createColorMap(DrawEnv *pEnv);
    void drawCombineMap(DrawEnv *pEnv);

    GLuint _fb;
    GLuint _fb2;
    GLuint _rb_depth;
    UInt32 _activeFactorMap;
    UInt32 _widthHeightPOT;
    Matrix _transforms[6];
    
    TileCameraDecoratorUnrecPtr  _tiledeco;
    BlendChunkUnrecPtr           _blender;
    TextureChunkUnrecPtr         _colorMap;
    TextureChunkUnrecPtr         _shadowFactorMap;
    ImageUnrecPtr                _colorMapImage;
    ImageUnrecPtr                _shadowFactorMapImage;
    TextureChunkUnrecPtr         _shadowFactorMap2;
    ImageUnrecPtr                _shadowFactorMapImage2;
    ChunkMaterialUnrecPtr        _shadowCmat;
    ChunkMaterialUnrecPtr        _combineCmat;
    SHLChunkUnrecPtr             _shadowSHL;
    SHLChunkUnrecPtr             _shadowCubeSHL;
    SHLChunkUnrecPtr             _shadowSHL2;
    SHLChunkUnrecPtr             _shadowSHL3;
    SHLChunkUnrecPtr             _shadowSHL4;
    SHLChunkUnrecPtr             _shadowSHL5;
    SHLChunkUnrecPtr             _shadowSHL6;
    SHLChunkUnrecPtr             _shadowSHL7;
    SHLChunkUnrecPtr             _combineSHL;
    DepthChunkUnrecPtr           _combineDepth;
    PolygonForegroundUnrecPtr    _pf;
    Int32                        _firstRun;
    UInt32                       _width;
    UInt32                       _height;
    bool                         _initTexturesDone;
};

OSG_END_NAMESPACE

#endif /* _OSGSTDSHADOWMAP_H_ */

