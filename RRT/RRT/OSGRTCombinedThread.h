/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2003 by the OpenSG Forum                   *
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

#ifndef _OSGRTCOMBINEDTHREAD_H_
#define _OSGRTCOMBINEDTHREAD_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTThread.h"
#include "OSGCellRTPPU.h"

OSG_BEGIN_NAMESPACE

class Barrier;
class RTTarget;

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTCombinedThread;

template<typename DescT, typename MathTag>
struct RTCombinedThreadHelper;


template<typename DescT>
struct RTCombinedThreadHelper<DescT, RTFloatMathTag>
{

    static void workProcHelper(RTCombinedThread<DescT> *pThis, 
                               RTTarget                &oTarget);
};

template<typename DescT>
struct RTCombinedThreadHelper<DescT, RTSIMDMathTag>
{
    typedef          DescT                     Desc;

    typedef typename Desc::Scene                Scene;
    typedef typename Desc::RTCache              RTCache;

    typedef typename Desc::PrimaryRayTile       PrimaryRayTile;
    typedef typename Desc::HitTile              HitTile;

    typedef typename Desc::HitStore             HitStore;
    typedef typename Desc::HitTiledStore        HitTiledStore;
    typedef typename Desc::PrimaryRayStore      PrimaryRayStore;
    typedef typename Desc::PrimaryRayTiledStore PrimaryRayTiledStore;
 

    typedef typename Desc::SingleHitPacket      FourHitPacket;
    typedef typename Desc::SingleRayPacket      FourRayPacket;
    typedef typename Desc::SingleRayPacketInfo  FourRayPacketInfo;

    typedef typename Desc::HitPacket            HitPacket;
    typedef typename Desc::RayPacket            RayPacket;

    typedef typename Desc::ColorPacket          ColorPacket;

    typedef typename Desc::RTCacheNode          CacheKDNode;
    typedef typename Desc::TriangleAccel        TriangleAccel;
    typedef typename Desc::MFTriangleAccel      MFTriangleAccel;

    typedef typename Desc::MathTag              MathTag;
 

    static void workProcHelper(RTCombinedThread<DescT> *pThis, 
                               RTTarget                &oTarget);
 
    friend class RTCombinedThread<DescT>;     
};


template<typename DescT>
class RTCombinedThread : public RTThread
{

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTCombinedThread<DescT>             Self;

    typedef DescT                               Desc;

    typedef typename Desc::Scene                Scene;
    typedef typename Desc::RTCache              RTCache;

    typedef typename Desc::PrimaryRayTile       PrimaryRayTile;
    typedef typename Desc::HitTile              HitTile;

    typedef typename Desc::HitStore             HitStore;
    typedef typename Desc::HitTiledStore        HitTiledStore;
    typedef typename Desc::PrimaryRayStore      PrimaryRayStore;
    typedef typename Desc::PrimaryRayTiledStore PrimaryRayTiledStore;

    typedef typename Desc::SingleHitPacket      SingleHitPacket;
    typedef typename Desc::SingleRayPacket      SingleRayPacket;
    typedef typename Desc::SingleRayPacketInfo  SingleRayPacketInfo;

    typedef typename Desc::HitPacket            HitPacket;
    typedef typename Desc::RayPacket            RayPacket;

    typedef typename Desc::ColorPacket          ColorPacket;

    typedef typename Desc::MathTag              MathTag;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    static RTCombinedThread *get (OSG::Char8 *szName);
    static RTCombinedThread *find(OSG::Char8 *szName);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void setup(Scene                  *pScene,
               RTTarget               *pTarget,
               PrimaryRayStore        *pRayStore, 
               HitStore               *pHitStore,
               Barrier                *pSyncBarrier,
               Int32                   iID         );


    void setup(Scene                  *pScene,
               RTTarget               *pTarget,
               PrimaryRayTiledStore   *pRayStore, 
               HitTiledStore          *pHitStore,
               Barrier                *pSyncBarrier,
               Int32                   iID         );

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef OSG::RTThread      Inherited;

    static  OSG::MPThreadType _type;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    static OSG::BaseThread *create(const OSG::Char8  *szName, 
                                         OSG::UInt32  uiId);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTCombinedThread(const OSG::Char8 *szName, OSG::UInt32 uiId);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTCombinedThread(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    friend struct RTCombinedThreadHelper<DescT, RTFloatMathTag>;
    friend struct RTCombinedThreadHelper<DescT, RTSIMDMathTag >;

    void workProc(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    Scene                   *_pScene;
    RTTarget                *_pTarget;
    PrimaryRayStore         *_pRayStore;
    HitStore                *_pHitStore;
    PrimaryRayTiledStore    *_pRayTiledStore;
    HitTiledStore           *_pHitTiledStore;
    Barrier                 *_pSyncBarrier;

    Int32                    _iID;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    RTCombinedThread(const RTCombinedThread &source);
    void operator =(const RTCombinedThread &source);
};
 
OSG_END_NAMESPACE

#define OSG_RTCOMBINEDTHREAD_INST(DESC, NAME)                               \
template<>                                                                  \
OSG::MPThreadType RTCombinedThread< DESC >::_type(                          \
    NAME,                                                                   \
    "RTThread",                                                             \
    (OSG::CreateThreadF) RTCombinedThread::create,                          \
    NULL);                                                                  \
                                                                            \
template<>                                                                  \
RTCombinedThread< DESC > *RTCombinedThread< DESC >::get(OSG::Char8 *szName) \
{                                                                           \
    OSG::BaseThread *pThread =                                              \
        OSG::ThreadManager::the()->getThread(szName,                        \
                                             NAME);                         \
                                                                            \
    return dynamic_cast<RTCombinedThread *>(pThread);                       \
}

#include "OSGRTCombinedThread.inl"

#endif /* _OSGPTCOMBINEDTHREAD_H_ */
