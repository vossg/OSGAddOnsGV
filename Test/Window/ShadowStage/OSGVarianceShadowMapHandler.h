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
#include "OSGSHLVariableChunk.h"

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
    void createColorMap(DrawEnv *pEnv,
                        RenderAction *pTmpAction);
    void createColorMapFBO(DrawEnv *pEnv,
                           RenderAction *pTmpAction);

    void createShadowFactorMap   (DrawEnv      *pEnv,
                                  RenderAction *pTmpAction, 
                                  UInt32        num,
                                  bool          bClear    );
    void createShadowFactorMapFBO(DrawEnv      *pEnv,
                                  RenderAction *pTmpAction, 
                                  UInt32        num,
                                  UInt32        uiActiveLightCount);

    void createShadowMapsFBO(DrawEnv *pEnv, 
                                              RenderAction *pTmpAction);
    bool initFBO(DrawEnv *pEnv);
    void reInit(DrawEnv *pEnv);
    bool checkFrameBufferStatus(Window *win);

    TileCameraDecoratorUnrecPtr  _tiledeco;
    ImageUnrecPtr                _colorMapImage;
    ImageUnrecPtr                _shadowFactorMapImage;

    std::vector<ChunkMaterialUnrecPtr>    _vShadowCmat;

    SHLChunkUnrecPtr             _shadowSHL;
    std::vector<ChunkMaterialUnrecPtr>        _vDepthCmat;
    SHLChunkUnrecPtr             _depthSHL;
    Int32                        _firstRun;

//    GLuint                       _fb;
//    GLuint                       _fb2;
//    GLuint                       _rb_depth;
//    GLuint                       _rb_depth2;
    bool                         _texChanged;
    bool                         _initTexturesDone;
 
    std::vector<SHLVariableChunkUnrecPtr> _vShadowSHLVar;
    std::vector<SHLVariableChunkUnrecPtr> _vDepthSHLVar;

    SolidBackgroundUnrecPtr _pClearSMapBack;
 };

OSG_END_NAMESPACE

#endif /* _OSGVARIANCESHADOWMAPHANDLER_H_ */
