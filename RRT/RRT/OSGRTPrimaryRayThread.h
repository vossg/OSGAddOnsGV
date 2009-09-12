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

#ifndef _OSGRTPRIMARYRAYTHREAD_H_
#define _OSGRTPRIMARYRAYTHREAD_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTThread.h"

OSG_BEGIN_NAMESPACE

class Barrier;

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTPrimaryRayThread : public RTThread
{

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTPrimaryRayThread<DescT>           Self;

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

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    static RTPrimaryRayThread *get (OSG::Char8 *szName);
    static RTPrimaryRayThread *find(OSG::Char8 *szName);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void setup(Scene             *pScene,
               PrimaryRayStore   *pRayStore, 
               HitStore          *pHitStore, 
               Barrier           *pSyncBarrier,
               Int32              iID      );

    void setup(Scene                  *pScene,
               PrimaryRayTiledStore   *pRayStore, 
               HitTiledStore          *pHitStore, 
               Barrier                *pSyncBarrier,
               Int32                   iID      );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void setRunning(bool bVal);

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
 
    RTPrimaryRayThread(const OSG::Char8 *szName, OSG::UInt32 uiId);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTPrimaryRayThread(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void workProc(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    Scene                   *_pScene;
    PrimaryRayStore         *_pRayStore;
    HitStore                *_pHitStore;
    PrimaryRayTiledStore    *_pRayTiledStore;
    HitTiledStore           *_pHitTiledStore;
    Barrier                 *_pSyncBarrier;

    Int32                    _iID;
    bool                     _bRunning;

    /*! \}                                                                 */
   /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    RTPrimaryRayThread(const RTPrimaryRayThread &source);
    void operator =(const RTPrimaryRayThread &source);
};
 
OSG_END_NAMESPACE

#define OSG_RTPRIMARYRAYTHREAD_INST(DESC, NAME)                        \
template <>                                                            \
OSG::MPThreadType RTPrimaryRayThread< DESC >::_type(                   \
    NAME,                                                              \
    "RTThread",                                                        \
    (OSG::CreateThreadF) RTPrimaryRayThread::create,                   \
    NULL);                                                             \
                                                                       \
template<>                                                             \
RTPrimaryRayThread< DESC > *                                           \
    RTPrimaryRayThread< DESC >::get(OSG::Char8 *szName)                \
{                                                                      \
    OSG::BaseThread *pThread =                                         \
        OSG::ThreadManager::the()->getThread(szName,                   \
                                             NAME);                    \
                                                                       \
    return dynamic_cast<RTPrimaryRayThread *>(pThread);                \
}

#include "OSGRTPrimaryRayThread.inl"

#endif /* _OSGPTRIMARYRAYTHREAD_H_ */
