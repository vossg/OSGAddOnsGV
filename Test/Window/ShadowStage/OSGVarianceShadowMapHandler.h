#ifndef _OSGVARIANCESHADOWMAPHANDLER_H_
#define _OSGVARIANCESHADOWMAPHANDLER_H_
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

class OSG_WINDOW_DLLMAPPING VarianceShadowMapHandler : public TreeHandler
{

    /*==========================  PUBLIC  =================================*/
  public:

    VarianceShadowMapHandler(ShadowStage *source);
    ~VarianceShadowMapHandler(void);
    virtual void render(DrawEnv *pEnv,
                        RenderAction *pTmpAction);

private:

    void initTextures(DrawEnv *pEnv);
    void drawCombineMap(DrawEnv *pEnv, 
                        RenderAction *pTmpAction);
    void createColorMap(DrawEnv *pEnv,
                        RenderAction *pTmpAction);
    void createColorMapFBO(DrawEnv *pEnv,
                           RenderAction *pTmpAction);
    void createShadowFactorMap(DrawEnv *pEnv,
                               RenderAction *pTmpAction, UInt32 num);
    void createShadowFactorMapFBO(DrawEnv *pEnv,
                                  RenderAction *pTmpAction, UInt32 num);
    void createShadowMapsFBO(DrawEnv *pEnv, 
                                              RenderAction *pTmpAction);
    bool initFBO(DrawEnv *pEnv);
    void reInit(DrawEnv *pEnv);
    bool checkFrameBufferStatus(Window *win);

    TileCameraDecoratorUnrecPtr  _tiledeco;
    TextureChunkUnrecPtr         _colorMap;
    TextureChunkUnrecPtr         _shadowFactorMap;
    ImageUnrecPtr                _colorMapImage;
    ImageUnrecPtr                _shadowFactorMapImage;
    ChunkMaterialUnrecPtr        _shadowCmat;
    SHLChunkUnrecPtr             _shadowSHL;
    SHLChunkUnrecPtr             _combineSHL;
    DepthChunkUnrecPtr           _combineDepth;
    ChunkMaterialUnrecPtr        _depthCmat;
    SHLChunkUnrecPtr             _depthSHL;
    ChunkMaterialUnrecPtr        _combineCmat;
    PolygonForegroundUnrecPtr    _pf;
    Int32                        _firstRun;
    UInt32                       _width;
    UInt32                       _height;

    GLuint                       _fb;
    GLuint                       _fb2;
    GLuint                       _rb_depth;
    GLuint                       _rb_depth2;
    bool                         _texChanged;
    UInt32                       _widthHeightPOT;
    bool                         _initTexturesDone;
};

OSG_END_NAMESPACE

#endif /* _OSGVARIANCESHADOWMAPHANDLER_H_ */

