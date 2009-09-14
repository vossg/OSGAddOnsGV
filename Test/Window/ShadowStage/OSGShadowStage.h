/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2002 by the OpenSG Forum                 *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 *   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                License                                    *
 *                                                                           *
 * This library is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Library General Public License as published    *
 * by the Free Software Foundation, version 2.                               *
 *                                                                           *
 * This library is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public         *
 * License along with this library; if not, write to the Free Software       *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Changes                                    *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

#ifndef _OSGSHADOWSTAGE_H_
#define _OSGSHADOWSTAGE_H_
#ifdef __sgi
#pragma once
#endif

#include <vector>
#include <utility>

#include "OSGConfig.h"
#include "OSGShadowStageBase.h"


#include <OSGAction.h>
#include <OSGRenderActionBase.h>
#include <OSGSpotLight.h>
#include <OSGDirectionalLight.h>
#include <OSGNode.h>
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
#include "OSGTreeHandler.h"

OSG_BEGIN_NAMESPACE

class TreeHandler;
class StdShadowMapHandler;

class OSG_WINDOW_DLLMAPPING ShadowStage : public ShadowStageBase
{

    /*==========================  PUBLIC  =================================*/

  public:

    enum Mode
    {
        NO_SHADOW=0,
        STD_SHADOW_MAP,
        PERSPECTIVE_SHADOW_MAP,
        DITHER_SHADOW_MAP,
        PCF_SHADOW_MAP,
        PCSS_SHADOW_MAP,
        VARIANCE_SHADOW_MAP
    };

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(BitVector  whichField, 
                         UInt32     origin,
                         BitVector  details);







#if 0
    virtual void setVPSize   (Real32 a,Real32 b, Real32 c, Real32 d);
    virtual void activateSize(void);
#endif
    virtual void activate    (void);
    virtual void deactivate  (void);

    virtual void render      (RenderActionBase* action);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0, 
                      const BitVector  bvFlags  = 0) const;

    void triggerMapUpdate(void);

    void renderLight(RenderActionBase *action, Material *mat, UInt32 index);
    Node *getLightRoot(UInt32 index);

    void checkLightsOcclusion(RenderActionBase *action);
    void setReadBuffer(void);
    void drawOcclusionBB(const Pnt3f &bbmin, const Pnt3f &bbmax);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in ShadowStageBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    ShadowStage(void);
    ShadowStage(const ShadowStage &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~ShadowStage(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Draw                                       */
    /*! \{                                                                 */

    ActionBase::ResultE renderEnter(Action *action);
    ActionBase::ResultE renderLeave(Action *action);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Draw                                       */
    /*! \{                                                                 */

    void onCreate (const ShadowStage *source = NULL);
    void onDestroy(      void                      );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/

    bool _bRunning;

    UInt32                    _mapRenderSize;
    bool                      _mapSizeChanged;

    TexGenChunkUnrecPtr       _texGen;
    PolygonChunkUnrecPtr      _poly;
    PolygonChunkUnrecPtr      _offset;
    NodeUnrecPtr              _dummy;
    PassiveBackgroundUnrecPtr _silentBack;
    UInt32                    _windowW;
    UInt32                    _windowH;

    struct ShadowMapStore
    {
        TextureObjChunkUnrecPtr   pTexO;
        TextureEnvChunkUnrecPtr   pTexE;
        FrameBufferObjectUnrecPtr pFBO;
    };

    std::vector<NodeUnrecPtr>                             _transparent;
    std::vector<std::pair<NodeUnrecPtr, LightUnrecPtr> >  _lights;
    std::vector<std::pair<NodeUnrecPtr, LightUnrecPtr> >  _oldLights;
    std::vector<CameraUnrecPtr>                           _lightCameras;
    std::vector<TransformUnrecPtr>                        _lightCamTrans;
    std::vector<NodeUnrecPtr>                             _lightCamBeacons;
    std::vector<UInt32>                                   _lightStates;

    std::vector<ImageUnrecPtr>                            _shadowImages;
    std::vector<ShadowMapStore>                           _vTexChunks;

    std::vector<bool>                                     _excludeNodeActive;
    std::vector<bool>                                     _realPointLight;
    std::vector<bool*>                                    _renderSide;

    bool                            _trigger_update;
    Matrix                          _transforms[6];

    NodeUnrecPtr                    _light_render_transform;
    GLuint                          _occlusionQuery;

    void postProcess(DrawEnv *pEnv);

    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class ShadowStageBase;
    friend class TreeHandler;
    friend class StdShadowMapHandler;
    friend class PerspectiveShadowMapHandler;
    friend class DitherShadowMapHandler;
    friend class PCFShadowMapHandler;
    friend class VarianceShadowMapHandler;
    friend class PCSSShadowMapHandler;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const ShadowStage &source);



    bool             _GLSLsupported;
    bool             _initDone;
    bool             _restart;
    TreeHandler     *_treeRenderer;

    typedef ShadowStageBase Inherited;

    void checkLights     (RenderActionBase* action);
    void updateLights    (RenderActionBase *action);
    void initializeLights(RenderActionBase *action);
    void clearLights     (UInt32            size  );

    Action::ResultE findLight      (Node * node);
    Action::ResultE findTransparent(Node * node);
    
};

typedef ShadowStage *ShadowStageP;

OSG_END_NAMESPACE

#include "OSGShadowStageBase.inl"
#include "OSGShadowStage.inl"

#endif /* _OSGSHADOWSTAGE_H_ */
