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

#ifndef _OSGRTSHADINGTHREAD_H_
#define _OSGRTSHADINGTHREAD_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTThread.h"

OSG_BEGIN_NAMESPACE

class Barrier;
class RTTarget;

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTShadingThread : public RTThread
{

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTShadingThread<DescT>          Self;

    typedef DescT                           Desc;

    typedef typename Desc::Scene            Scene;

    typedef typename Desc::PrimaryRayTile   PrimaryRayTile;
    typedef typename Desc::HitTile          HitTile;

    typedef typename Desc::HitStore         HitStore;
    typedef typename Desc::HitTiledStore    HitTiledStore;

    typedef typename Desc::SingleHitPacket  SingleHitPacket;
    typedef typename Desc::SingleRayPacket  SingleRayPacket;

    typedef typename Desc::HitPacket        HitPacket;
    typedef typename Desc::RayPacket        RayPacket;

    typedef typename Desc::ColorPacket      ColorPacket;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    static RTShadingThread *get (OSG::Char8 *szName);
    static RTShadingThread *find(OSG::Char8 *szName);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void setup(RTTarget        *pTarget,
               Scene           *pScene,
               HitStore        *pHitStore,
               Barrier         *pSyncBarrier,
               Int32            iID         );

    void setup(RTTarget        *pTarget,
               Scene           *pScene,
               HitTiledStore   *pHitStore,
               Barrier         *pSyncBarrier,
               Int32            iID         );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void setRunning(bool bVal);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef RTThread      Inherited;

    static  MPThreadType _type;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    static OSG::BaseThread *create(const OSG::Char8  *szName, 
                                         OSG::UInt32  uiId  );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTShadingThread(const OSG::Char8 *szName, OSG::UInt32 uiId);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTShadingThread(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void workProc(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    RTTargetUnrecPtr  _pTarget;
    Scene            *_pScene;

    HitStore         *_pHitStore;
    HitTiledStore    *_pHitTiledStore;

    Barrier          *_pSyncBarrier;

    Int32             _iID;
    bool              _bRunning;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    RTShadingThread(const RTShadingThread &source);
    void operator =(const RTShadingThread &source);
};

OSG_END_NAMESPACE

#define OSG_RTSHADINGTHREAD_INST(DESC, NAME)                               \
template<>                                                                 \
OSG::MPThreadType RTShadingThread< DESC >::_type(                          \
    NAME,                                                                  \
    "RTThread",                                                            \
    static_cast<OSG::CreateThreadF>(RTShadingThread::create),              \
    NULL);                                                                 \
                                                                           \
template<> OSG_DLL_EXPORT                                                  \
RTShadingThread< DESC > *RTShadingThread< DESC >::get(OSG::Char8 *szName)  \
{                                                                          \
    OSG::BaseThread *pThread =                                             \
        OSG::ThreadManager::the()->getThread(szName,                       \
                                             NAME);                        \
                                                                           \
    return dynamic_cast<RTShadingThread *>(pThread);                       \
}

#include "OSGRTShadingThread.inl"

#endif /* _OSGRTSHADINGTHREAD_H_ */
