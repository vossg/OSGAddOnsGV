#ifndef _OSGPERSPECTIVESHADOWMAPHANDLER_H_
#define _OSGPERSPECTIVESHADOWMAPHANDLER_H_
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
#include <OSGSimpleTexturedMaterial.h>
#include <OSGTileCameraDecorator.h>
#include <OSGPolygonForeground.h>
#include <OSGMatrixCameraDecorator.h>
#include <OSGDepthChunk.h>

#include "OSGTreeHandler.h"

OSG_BEGIN_NAMESPACE

class ShadowStage;
class TreeRenderer;

class OSG_WINDOW_DLLMAPPING PerspectiveShadowMapHandler : public TreeHandler
{
public:

    PerspectiveShadowMapHandler(ShadowStage *source);
    ~PerspectiveShadowMapHandler(void);
    virtual void render(DrawEnv *pEnv,
                        RenderAction *pTmpAction);

private:

    bool initFBO(DrawEnv *pEnv);
    void reInit(DrawEnv *pEnv);

    bool checkFrameBufferStatus(Window *win);
    void createShadowMapsFBO(DrawEnv *pEnv, 
                             RenderAction *pTmpAction);
    void initTextures(DrawEnv *pEnv);

    void createShadowMaps(DrawEnv *pEnv, 
                          RenderAction *pTmpAction);
    void createShadowMapsNOGLSL(DrawEnv *pEnv);
    void projectShadowMaps(DrawEnv *pEnv, 
                           RenderAction *pTmpAction);
    void createColorMapFBO(DrawEnv *pEnv,
                           RenderAction *pTmpAction);
    void createShadowFactorMapFBO(DrawEnv *pEnv,
                                  RenderAction *pTmpAction);
    void createShadowFactorMap(DrawEnv *pEnv,
                               RenderAction *pTmpAction);
    void createColorMap(DrawEnv *pEnv,
                        RenderAction *pTmpAction);
    void drawCombineMap(DrawEnv      *pEnv,
                        RenderAction *pTmpAction);

    void calcPerspectiveSpot(Matrix &_LPM, Matrix &_LVM, UInt32 num,
                                                      DrawEnv *pEnv);
    bool bbInsideFrustum(Pnt3f sceneMin, Pnt3f sceneMax, Matrix LPVM);
    void calcHull2(std::vector<Pnt3f> *points, Matrix invEyeProjMatrix, Pnt3f sceneMin, Pnt3f sceneMax, Matrix LPVM);
    void calcUpVec2(Vec3f &up99,Vec3f viewDir,Vec3f lightDir);
    void calcCubicHull2(Pnt3f &min99, Pnt3f &max99, std::vector<Pnt3f> *points);
    void scaleTranslateToFit2(Matrix &mat, const Pnt3f vMin, const Pnt3f vMax); 
    void calcBodyVec(Vec3f &dir, Pnt3f eyePos, std::vector<Pnt3f> *points);
    bool pntInFrontOf(Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p);

    void printMatrix(Matrix m);
    void printPoint(Pnt3f p);

    GLuint _fb;
    GLuint _fb2;
    GLuint _rb_depth;
    UInt32 _widthHeightPOT;
    UInt32 _activeFactorMap;
    Matrix _transforms[6];

    std::vector<Matrix>          _perspectiveLPM;
    std::vector<Matrix>          _perspectiveLVM;

    TileCameraDecoratorUnrecPtr  _tiledeco;
    BlendChunkUnrecPtr           _blender;
    MatrixCameraUnrecPtr         _matrixCam2;
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

    UInt32                       _width;
    UInt32                       _height;
    Int32                        _firstRun;
    bool                         _initTexturesDone;
};

OSG_END_NAMESPACE

#endif /* _OSGPERSPECTIVESHADOWMAPHANDLER_H_ */

