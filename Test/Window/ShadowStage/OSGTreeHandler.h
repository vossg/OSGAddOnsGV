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

#ifndef _OSGTREEHANDLER_H_
#define _OSGTREEHANDLER_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGWindowDef.h"
#include "OSGMemoryObject.h"

#include "OSGTextureObjChunk.h"
#include "OSGImage.h"
#include "OSGSolidBackground.h"
#include "OSGFrameBufferObject.h"
#include "OSGDepthChunk.h"
#include "OSGSHLChunk.h"
#include "OSGSimpleMaterial.h"

OSG_BEGIN_NAMESPACE

#ifdef __APPLE__
#define NO_CONST_GLSL_VAR
#endif

class ShadowStage;
class ShadowStageData;
class DrawEnv;

class OSG_WINDOW_DLLMAPPING TreeHandler : public MemoryObject
{

    /*==========================  PUBLIC  =================================*/

  public:

    typedef MemoryObject Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    TreeHandler(ShadowStage     *pSource,
                ShadowStageData *pData  );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~TreeHandler(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Draw                                       */
    /*! \{                                                                 */

    virtual void render(DrawEnv *pEnv) = 0;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Draw                                       */
    /*! \{                                                                 */

    UInt32 getMode(void);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    static std::string         _shadow_combine_vp;
    static std::string         _shadow_combine_fp;


    UInt32                     _uiMode;
    UInt32                     _uiMapSize;
    bool                       _bShadowMapsConfigured;
    UInt32                     _activeFactorMap;

    UInt32                     _width;
    UInt32                     _height;

    UInt32                     _maxPLMapSize;
    UInt32                     _PLMapSize;
    UInt32                     _maxTexSize;


    ShadowStage               *_pStage;
    ShadowStageData           *_pStageData;

    TextureObjChunkUnrecPtr    _colorMapO;
    TextureObjChunkUnrecPtr    _shadowFactorMapO;
    TextureObjChunkUnrecPtr    _shadowFactorMap2O;

    ImageUnrecPtr              _colorMapImage;
    ImageUnrecPtr              _shadowFactorMapImage;
    ImageUnrecPtr              _shadowFactorMapImage2;

    FrameBufferObjectUnrecPtr  _pSceneFBO;

    SolidBackgroundUnrecPtr    _pClearBackground;

    SimpleMaterialUnrecPtr     _unlitMat;

    SHLChunkUnrecPtr           _combineSHL;
    DepthChunkUnrecPtr         _combineDepth;
    ChunkMaterialUnrecPtr      _combineCmat;

    Matrix                     _aCubeTrans[6];

    /*---------------------------------------------------------------------*/
    /*! \name                       Edit                                   */
    /*! \{                                                                 */

    bool initSceneFBO      (DrawEnv *pEnv, bool bHaveTwoFactorMaps);
    void updateSceneFBOSize(DrawEnv *pEnv, bool bHaveTwoFactorMaps);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Draw                                       */
    /*! \{                                                                 */

    void initShadowMaps     (void);
    void updateShadowMapSize(void);
    void configureShadowMaps(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Draw                                       */
    /*! \{                                                                 */

    void setupDrawCombineMap1(Action  *pAction);
    void doDrawCombineMap1   (DrawEnv *pEnv   );

    void setupDrawCombineMap2(Action  *pAction);
    void doDrawCombineMap2   (DrawEnv *pEnv   );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Draw                                       */
    /*! \{                                                                 */

    bool hasFactorMap(void);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    /*---------------------------------------------------------------------*/

    // prohibit default functions (move to 'public' if you need one)
    TreeHandler(const TreeHandler &source);
    void operator =(const TreeHandler &source);
};

typedef RefCountPtr<TreeHandler, MemObjRefCountPolicy> TreeHandlerRefPtr;

OSG_END_NAMESPACE

#include "OSGTreeHandler.inl"

#endif /* _OSGTREEHANDLER_H_ */
