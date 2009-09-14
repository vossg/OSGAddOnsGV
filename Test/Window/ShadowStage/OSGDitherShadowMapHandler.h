#ifndef _OSGDITHERSHADOWMAPHANDLER_H_
#define _OSGDITHERSHADOWMAPHANDLER_H_
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
#include <OSGSimpleMaterial.h>
#include <OSGPassiveBackground.h>
#include <OSGSolidBackground.h>
#include <OSGChunkMaterial.h>
#include <OSGMaterialChunk.h>
#include <OSGSHLChunk.h>
#include <OSGForeground.h>
#include <OSGPolygonForeground.h>
#include <OSGGrabForeground.h>
#include <OSGTextureGrabForeground.h>
#include <OSGFileGrabForeground.h>
#include <OSGImageForeground.h>
#include <OSGTexGenChunk.h>
#include <OSGTextureTransformChunk.h>
#include <OSGPolygonChunk.h>
#include <OSGBlendChunk.h>
#include <OSGTileCameraDecorator.h>
#include <OSGDepthChunk.h>

#include "OSGTreeHandler.h"

OSG_BEGIN_NAMESPACE

class ShadowStage;
class TreeRenderer;

class OSG_WINDOW_DLLMAPPING DitherShadowMapHandler : public TreeHandler
{
public:

    DitherShadowMapHandler(ShadowStage *source);
    ~DitherShadowMapHandler(void);
    virtual void render(DrawEnv *pEnv,
                        RenderAction *pTmpAction);

  private:

    void createShadowMaps(DrawEnv *pEnv, 
                          RenderAction *pTmpAction);
    void createColorMap(DrawEnv *pEnv,
                        RenderAction *pTmpAction);
    void drawCombineMap(DrawEnv *pEnv,
                        RenderAction *pTmpAction);
    void createShadowFactorMap(DrawEnv *pEnv,
                             RenderAction *pTmpAction);
    void createColorMapFBO(DrawEnv *pEnv,
                           RenderAction *pTmpAction);
    void createShadowFactorMapFBO(DrawEnv *pEnv,
                                  RenderAction *pTmpAction);
    void createShadowMapsFBO(DrawEnv *pEnv,
                             RenderAction *pTmpAction);
    bool initFBO(DrawEnv *pEnv);
    void reInit(DrawEnv *pEnv);
    bool checkFrameBufferStatus(Window *win);
    void initTextures(DrawEnv *pEnv);

    GLuint                       _fb;
    GLuint                       _fb2;
    GLuint                       _rb_depth;
    Matrix                       _transforms[6];
    TileCameraDecoratorUnrecPtr  _tiledeco;
    BlendChunkUnrecPtr           _blender;
    TextureChunkUnrecPtr         _colorMap;
    TextureChunkUnrecPtr         _shadowFactorMap;
    ImageUnrecPtr                _colorMapImage;
    ImageUnrecPtr                _shadowFactorMapImage;
    ChunkMaterialUnrecPtr        _shadowCmat;
    ChunkMaterialUnrecPtr        _combineCmat;
    SHLChunkUnrecPtr             _shadowSHL;
    SHLChunkUnrecPtr             _shadowSHL2;
    SHLChunkUnrecPtr             _shadowSHL3;
    SHLChunkUnrecPtr             _shadowSHL4;
    SHLChunkUnrecPtr             _shadowCubeSHL;
    SHLChunkUnrecPtr             _combineSHL;
    DepthChunkUnrecPtr           _combineDepth;
    PolygonForegroundUnrecPtr    _pf;
    Int32                        _firstRun;
    UInt32                       _width;
    UInt32                       _height;
    UInt32                       _widthHeightPOT;
    UInt32                       _activeFactorMap;
    TextureChunkUnrecPtr         _shadowFactorMap2;
    ImageUnrecPtr                _shadowFactorMapImage2;
    bool                         _initTexturesDone;
};

OSG_END_NAMESPACE

#endif /* _OSGDITHERSHADOWMAPHANDLER_H_ */

