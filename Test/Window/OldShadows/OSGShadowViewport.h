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

#ifndef _OSGSHADOWVIEWPORT_H_
#define _OSGSHADOWVIEWPORT_H_
#ifdef __sgi
#pragma once
#endif

#include <vector>
#include <utility>

#include "OSGAddOnsConfig.h"
#include "OSGAction.h"
#include "OSGRenderActionBase.h"
#include "OSGSpotLight.h"
#include "OSGDirectionalLight.h"
#include "OSGNode.h"
#include "OSGShadowViewportBase.h"
#include "OSGPerspectiveCamera.h"
#include "OSGMatrixCamera.h"
#include "OSGTransform.h"
#include "OSGTextureChunk.h"
#include "OSGPassiveBackground.h"
#include "OSGTexGenChunk.h"
#include "OSGTextureTransformChunk.h"
#include "OSGPolygonChunk.h"
#include "OSGBlendChunk.h"
#include "OSGTileCameraDecorator.h"
#include "OSGTreeRenderer.h"

OSG_BEGIN_NAMESPACE

class OSG_WINDOW_DLLMAPPING ShadowViewport : public ShadowViewportBase
{
  private:

    friend class FieldContainer;
    friend class ShadowViewportBase;

    static void initMethod(InitPhase ePhase);

    // prohibit default functions (move to 'public' if you need one)

    void operator =(const ShadowViewport &source);

    bool             _GLSLsupported;
    bool             _initDone;
    bool             _restart;
    TreeRenderer    *_treeRenderer;

    typedef ShadowViewportBase Inherited;

    void checkLights     (RenderActionBase* action);
    void updateLights    (void                    );
    void initializeLights(RenderActionBase *action);
    void clearLights     (UInt32            size  );

    Action::ResultE findLight      (Node * node);
    Action::ResultE findTransparent(Node * node);

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

    UInt32                    _mapRenderSize;
    bool                      _mapSizeChanged;

    TexGenChunkUnrecPtr       _texGen;
    PolygonChunkUnrecPtr      _poly;
    PolygonChunkUnrecPtr      _offset;
    NodeUnrecPtr              _dummy;
    PassiveBackgroundUnrecPtr _silentBack;
    UInt32                    _windowW;
    UInt32                    _windowH;

    std::vector<NodeUnrecPtr>                             _transparent;
    std::vector<std::pair<NodeUnrecPtr, LightUnrecPtr> >  _lights;
    std::vector<std::pair<NodeUnrecPtr, LightUnrecPtr> >  _oldLights;
    std::vector<CameraUnrecPtr>                           _lightCameras;
    std::vector<TransformUnrecPtr>                        _lightCamTrans;
    std::vector<NodeUnrecPtr>                             _lightCamBeacons;
    std::vector<UInt32>                                   _lightStates;
    std::vector<ImageUnrecPtr>                            _shadowImages;
    std::vector<TextureChunkUnrecPtr>                     _texChunks;
    std::vector<bool>                                     _excludeNodeActive;
    std::vector<bool>                                     _realPointLight;
    std::vector<bool*>                                    _renderSide;

    bool                            _trigger_update;
    Matrix                          _transforms[6];

    NodeUnrecPtr                    _light_render_transform;
    GLuint                          _occlusionQuery;

    virtual void setVPSize   (Real32 a,Real32 b, Real32 c, Real32 d);
    virtual void activateSize(void);
    virtual void activate    (void);
    virtual void deactivate  (void);
    virtual void render      (RenderActionBase* action);

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(BitVector  whichField, 
                         UInt32     origin,
                         BitVector  details);

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


    // Variables should all be in ShadowViewportBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    ShadowViewport(void);
    ShadowViewport(const ShadowViewport &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~ShadowViewport(void); 

    void onCreate(const ShadowViewport *source = NULL);
    void onDestroy(void);


    /*! \}                                                                 */
    
};

typedef ShadowViewport *ShadowViewportP;

OSG_END_NAMESPACE

#include "OSGShadowViewportBase.inl"
#include "OSGShadowViewport.inl"

#endif /* _OSGSHADOWVIEWPORT_H_ */
