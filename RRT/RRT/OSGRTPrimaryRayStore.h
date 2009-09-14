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

#ifndef _OSGRTPRIMARYRAYSTORE_H_
#define _OSGRTPRIMARYRAYSTORE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTStore.h"

#include "OSGCondVar.h"
#include "OSGLock.h"

#include "OSGPerspectiveCamera.h"

// CELL related, move when cell code moving to other location
#include "OSGCellRTPPU.h"

OSG_BEGIN_NAMESPACE

class Camera;
class RTTarget;
class RTCameraDecorator;

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTPrimaryRayStore;

template<typename DescT, typename MathTag>
struct RTPrimaryRayStoreSetupHelper;


template<typename DescT>
struct RTPrimaryRayStoreSetupHelper<DescT, RTFloatMathTag>
{
    typedef          DescT                        Desc;
    typedef typename Desc::SingleRayPacket        SingleRayPacket;
    typedef typename Desc::SingleRayPacketInfo    SingleRayPacketInfo;
    typedef          RTRaySIMDPacketInfo::RayMode RayMode;

    static RayMode setupRays(RTPrimaryRayStore<DescT> *pThis,
                             RTCameraDecorator        &pCam, 
                             RTTarget                 &pTarget);
};

template<typename DescT>
struct RTPrimaryRayStoreSetupHelper<DescT, RTSIMDMathTag>
{
    typedef          DescT                        Desc;
    typedef typename Desc::SingleRayPacket        FourRayPacket;
    typedef typename Desc::SingleRayPacketInfo    FourRayPacketInfo;
    typedef          RTRaySIMDPacketInfo::RayMode RayMode;

    static RayMode setupRays(RTPrimaryRayStore<DescT> *pThis,
                             RTCameraDecorator        &pCam, 
                             RTTarget                 &pTarget);
};

template<typename DescT>
class RTPrimaryRayStore : public RTStore
{

    /*==========================  PUBLIC  =================================*/

  public:


    static const UInt32 Empty = UINT_MAX;

    typedef          DescT                        Desc;
    typedef typename Desc::SingleRayPacket        SingleRayPacket;
    typedef typename Desc::SingleRayPacketInfo    SingleRayPacketInfo;
    typedef typename Desc::RayInfoStore           RayInfoStore;
    typedef typename Desc::RayStore               RayStore;

    typedef typename Desc::MathTag                MathTag;

    typedef          RTRaySIMDPacketInfo::RayMode RayMode;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTPrimaryRayStore(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    RayMode startFrame(RTCameraDecorator &pCam, RTTarget &pTarget);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    UInt32 getNumRayPackets(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

#ifdef OSG_CELL
    void convCellStruct(UInt32         tileIndex,
                        rayTile_t     *rayTile,
                        rayInfoTile_t *rayInfoTile);
#endif

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    UInt32               nextIndex       (void             );

    SingleRayPacket     &getRayPacket    (UInt32 uiRayIndex);
    SingleRayPacketInfo &getRayPacketInfo(UInt32 uiRayIndex);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef RTStore                      Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    UInt32        _uiNumRays;
    UInt32        _uiCurrentRay;
    RayStore      _vRays;
    RayInfoStore  _vRayInfos;
    Lock         *_pStoreLock;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTPrimaryRayStore(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    friend struct RTPrimaryRayStoreSetupHelper<DescT, RTFloatMathTag>;
    friend struct RTPrimaryRayStoreSetupHelper<DescT, RTSIMDMathTag >;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    RTPrimaryRayStore(const RTPrimaryRayStore &source);
    void operator =(const RTPrimaryRayStore &source);
};

OSG_END_NAMESPACE

#include "OSGRTPrimaryRayStore.inl"

#endif /* _OSGRTPRIMARYRAYSTORE_H_ */
