#ifndef _OSGSTDSHADOWMAPHANDLER_H_
#define _OSGSTDSHADOWMAPHANDLER_H_
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
#include <OSGMaterialGroup.h>
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
#include <OSGSimpleTexturedMaterial.h>
#include <OSGTileCameraDecorator.h>
#include <OSGPolygonForeground.h>

#include "OSGTreeHandler.h"

OSG_BEGIN_NAMESPACE

class ShadowViewport;
class TreeRenderer;
class DrawEnv;
class RenderAction;

class OSG_WINDOW_DLLMAPPING StdShadowMapHandler : public TreeHandler
{
  public:

    StdShadowMapHandler(ShadowStage *source);
    ~StdShadowMapHandler(void);
    virtual void render(DrawEnv      *pEnv,
                        RenderAction *pTmpAction);

  private:

    bool initFBO(DrawEnv *pEnv);
    void reInit(DrawEnv *pEnv);
    
    void initTextures(DrawEnv *pEnv);

    void createColorMapFBO       (DrawEnv *pEnv, RenderAction *pTmpAction);
    void createShadowMapsFBO     (DrawEnv *pEnv, RenderAction *pTmpAction);
    void createShadowFactorMapFBO(DrawEnv *pEnv, RenderAction *pTmpAction);

    Matrix _transforms[6];
    
    TileCameraDecoratorUnrecPtr  _tiledeco;
    BlendChunkUnrecPtr           _blender;
    ImageUnrecPtr                _colorMapImage;
    ImageUnrecPtr                _shadowFactorMapImage;
    ImageUnrecPtr                _shadowFactorMapImage2;
    ChunkMaterialUnrecPtr        _shadowCmat;
    SHLChunkUnrecPtr             _shadowSHL;
    SHLChunkUnrecPtr             _shadowCubeSHL;
    SHLChunkUnrecPtr             _shadowSHL2;
    SHLChunkUnrecPtr             _shadowSHL3;
    SHLChunkUnrecPtr             _shadowSHL4;
    SHLChunkUnrecPtr             _shadowSHL5;
    SHLChunkUnrecPtr             _shadowSHL6;
    SHLChunkUnrecPtr             _shadowSHL7;
    Int32                        _firstRun;
    bool                         _initTexturesDone;
};

OSG_END_NAMESPACE

#endif /* _OSGSTDSHADOWMAPHANDLER_H_ */

