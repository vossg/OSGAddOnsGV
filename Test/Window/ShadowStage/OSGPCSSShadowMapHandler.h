#ifndef _OSGPCSSSHADOWMAPHANDLER_H_
#define _OSGPCSSSHADOWMAPHANDLER_H_
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
#include <OSGSimpleMaterial.h>
#include <OSGDepthChunk.h>

#include "OSGTreeHandler.h"
#include "OSGSHLVariableChunk.h"

OSG_BEGIN_NAMESPACE

class ShadowStage;
class TreeRenderer;

class OSG_WINDOW_DLLMAPPING PCSSShadowMapHandler : public TreeHandler
{
  public:

    PCSSShadowMapHandler(ShadowStage *source);
    ~PCSSShadowMapHandler(void);
    virtual void render(DrawEnv *pEnv,
                        RenderAction *pTmpAction);

  private:

    void initTextures(DrawEnv *pEnv);
    void createColorMapFBO(DrawEnv *pEnv,
                           RenderAction *pTmpAction);


    void createShadowFactorMapFBO(DrawEnv      *pEnv,
                                  RenderAction *pTmpAction, 
                                  UInt32        num,
                                  UInt32        uiActiveLightCount);

    void createShadowMapsFBO(DrawEnv *pEnv, 
                             RenderAction *pTmpAction);
    bool initFBO(DrawEnv *pEnv);
    void reInit(DrawEnv *pEnv);
    bool checkFrameBufferStatus(Window *win);

#if 0
    void createShadowMaps(DrawEnv *pEnv, 
                          RenderAction *pTmpAction);
    void createColorMap(DrawEnv *pEnv,
                        RenderAction *pTmpAction);
    void createShadowFactorMap(DrawEnv         *pEnv,
                               RenderAction    *pTmpAction, 
                               UInt32           num               );
#endif

    TileCameraDecoratorUnrecPtr  _tiledeco;
    ImageUnrecPtr                _colorMapImage;
    ImageUnrecPtr                _shadowFactorMapImage;

    std::vector<ChunkMaterialUnrecPtr>    _vShadowCmat;

    SHLChunkUnrecPtr             _shadowSHL;
    Int32                        _firstRun;
//    GLuint                       _fb;
//    GLuint                       _fb2;
//    GLuint                       _rb_depth;
    bool                         _initTexturesDone;

    std::vector<SHLVariableChunkUnrecPtr> _vShadowSHLVar;
};

OSG_END_NAMESPACE

#endif /* _OSGPCSSSHADOWMAPHANDLER_H_ */

