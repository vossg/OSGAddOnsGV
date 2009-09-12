/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

#ifndef _OSGRRTSTAGE_H_
#define _OSGRRTSTAGE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRRTStageBase.h"
#include "OSGBarrier.h"
#include "OSGCamera.h"
#include "OSGTextureObjChunk.h"
#include "OSGRTCacheAttachmentInst.h"

#include "OSGRTSIMD.h"

OSG_BEGIN_NAMESPACE

class RayTracer;
class RTTriAccelBarycentric;

template<typename DescT>
class RTCacheKD;

template<typename DescT>
class RTCacheKDVer1;

template<typename DescT>
class RayTracerInst;

template<typename DescT>
class RTScene;

template<typename DescT>
class RTLocalPacketManager;

template<typename DescT>
class RTHitStore;

template<typename DescT>
class RTHitTiledStore;

template<class DescT>
class RTPrimaryRayTiledStore;

template<typename DescT>
class RTPrimaryRayStore;

template<typename DescT>
class RTHitTile;

template<typename DescT>
class RTPrimaryRayTile;

class RTTriAccelBarycentric;

class RTHitPacket;
class RTSingleHitPacket;

class RTRayPacket;
class RTSingleRayPacket;

class RTRayPacketInfo;
class RTSingleRayPacketInfo;

class RTRaySIMDPacket;
class RTRaySIMDPacketInfo;
class RTFourRaySIMDPacket;
class RTFourRaySIMDPacketInfo;

class RTHitSIMDPacket;
class RTFourHitSIMDPacket;

class RTColorPacket;
class RTColorSIMDPacket;

/*! \brief RRTStage class. See \ref
           PageContribRRTRRTStage for a description.
*/

class OSG_CONTRIBRRT_DLLMAPPING RRTStage : public RRTStageBase
{
  protected:
    
  public:

    /*==========================  PUBLIC  =================================*/

    struct SinglePacketDescBase
    {
        typedef RTFloatMathTag                         MathTag;

        typedef RTColorPacket                          ColorPacket;

        typedef RTTriAccelBarycentric                  TriangleAccel;
        typedef RTCacheKD<SinglePacketDescBase>        RTCache;

        typedef RTHitPacket                            HitPacket;
        typedef RTSingleHitPacket                      SingleHitPacket;

        typedef RTRayPacket                            RayPacket;
        typedef RTRayPacketInfo                        RayPacketInfo;
        typedef RTSingleRayPacket                      SingleRayPacket;
        typedef RTSingleRayPacketInfo                  SingleRayPacketInfo;

        typedef RTHitTile<SinglePacketDescBase>        HitTile;
        typedef RTPrimaryRayTile<SinglePacketDescBase> PrimaryRayTile;

        typedef std::vector<SingleRayPacket    >       RayStore;
    };
    

    struct SIMDPacketDescBase
    {
        typedef RTSIMDMathTag                        MathTag;

        typedef RTColorSIMDPacket                    ColorPacket;

        typedef RTTriAccelBarycentric                TriangleAccel;
        typedef RTCacheKD<SIMDPacketDescBase>        RTCache;

        typedef RTHitSIMDPacket                      HitPacket;
        typedef RTFourHitSIMDPacket                  SingleHitPacket;

        typedef RTRaySIMDPacket                      RayPacket;
        typedef RTRaySIMDPacketInfo                  RayPacketInfo;
        typedef RTFourRaySIMDPacket                  SingleRayPacket;
        typedef RTFourRaySIMDPacketInfo              SingleRayPacketInfo;

        typedef RTHitTile<SIMDPacketDescBase>        HitTile;
        typedef RTPrimaryRayTile<SIMDPacketDescBase> PrimaryRayTile;

        typedef AlignedAllocator<SingleRayPacket>    SIMDRayAllocator;

        typedef std::vector<SingleRayPacket,
                            SIMDRayAllocator>        RayStore;
    };



    struct SinglePacketCacheDesc : public SinglePacketDescBase
    {
        static const Char8 *getTypeName      (void)
        {
            return "SinglePacketRCacheAttachment";
        }

        static const Char8 *getParentTypeName(void)
        {
            return "RTCacheAttachment";
        }

        static const Char8 *getGroupName     (void)
        {
            return "RTCacheAttachment";
        }
    };

    struct SIMDPacketCacheDesc : public SIMDPacketDescBase
    {
        static const Char8 *getTypeName      (void)
        {
            return "SIMDPacketRCacheAttachment";
        }

        static const Char8 *getParentTypeName(void)
        {
            return "RTCacheAttachment";
        }

        static const Char8 *getGroupName     (void)
        {
            return "RTCacheAttachment";
        }

    };

    typedef RTCacheAttachmentInst<SinglePacketCacheDesc> SPCacheAttachment;
    typedef RTCacheAttachmentInst<SIMDPacketCacheDesc  > SIMDCacheAttachment;



    struct SinglePacketDesc : public SinglePacketDescBase
    {
        static const Char8 *getTypeName      (void)
        {
            return "SinglePacketRayTracer";
        }

        static const Char8 *getParentTypeName(void)
        {
            return "RayTracer";
        }

        static const Char8 *getGroupName     (void)
        {
            return "RayTracer";
        }

        typedef SPCacheAttachment                          CacheAttachment;
        typedef RTScene             <SinglePacketDescBase> Scene;
        typedef RTLocalPacketManager<SinglePacketDesc    > PacketManager;
        typedef RTHitStore          <SinglePacketDescBase> HitStore;
        typedef RTHitTiledStore     <SinglePacketDescBase> HitTiledStore;

        typedef RTPrimaryRayStore   <SinglePacketDescBase> PrimaryRayStore;
        typedef RTPrimaryRayTiledStore<
                  SinglePacketDescBase>                   PrimaryRayTiledStore;
    };

    struct SIMDPacketDesc : public SIMDPacketDescBase
    {
        static const Char8 *getTypeName      (void)
        {
            return "SIMDPacketRayTracer";
        }

        static const Char8 *getParentTypeName(void)
        {
            return "RayTracer";
        }

        static const Char8 *getGroupName     (void)
        {
            return "RayTracer";
        }

        typedef SIMDCacheAttachment                      CacheAttachment;
        typedef RTScene             <SIMDPacketDescBase> Scene;
        typedef RTLocalPacketManager<SIMDPacketDesc    > PacketManager;
        typedef RTHitStore          <SIMDPacketDescBase> HitStore;
        typedef RTHitTiledStore     <SIMDPacketDescBase> HitTiledStore;
        typedef RTPrimaryRayStore   <SIMDPacketDescBase> PrimaryRayStore;
        typedef RTPrimaryRayTiledStore<
                  SIMDPacketDescBase>                    PrimaryRayTiledStore;
    };


    typedef RayTracerInst<SinglePacketDesc> SinglePacketRayTracer;
    typedef RayTracerInst<SIMDPacketDesc  > SIMDPacketRayTracer;
    


  public:


//    typedef SinglePacketRayTracer   ActiveRayTracer;
    typedef SIMDPacketRayTracer   ActiveRayTracer;

    typedef RRTStageBase          Inherited;
    typedef RRTStage              Self;

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details   );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Dump                                  */
    /*! \{                                                                 */

    void run        (CameraP   pCam);
    void postProcess(DrawEnv  *pEnv);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in RRTStageBase.

    bool             _bInitialized;
    ActiveRayTracer *_pRayTracer;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RRTStage(void);
    RRTStage(const RRTStage &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~RRTStage(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                MT Construction                               */
    /*! \{                                                                 */

    void onCreate       (const RRTStage *source = NULL);

    void onCreateAspect (const RRTStage *createAspect,
                         const RRTStage *source = NULL);
    
    void onDestroy      (      UInt32    uiContainerId);

    void onDestroyAspect(      UInt32    uiContainerId,
                               UInt32    uiAspect     );
    
    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class RRTStageBase;
    friend class FieldContainer;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const RRTStage &source);
};

typedef RRTStage *RRTStageP;

OSG_END_NAMESPACE

#include "OSGRRTStageBase.inl"
#include "OSGRRTStage.inl"

#endif /* _OSGRRTSTAGE_H_ */
