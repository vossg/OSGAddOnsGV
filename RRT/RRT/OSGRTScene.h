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

#ifndef _OSGRTSCENE_H_
#define _OSGRTSCENE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGMemoryObject.h"
#include "OSGContribRRTDef.h"

#include "OSGMaterialChunk.h"

#include "OSGRTRayPacket.h"
#include "OSGRTHitPacket.h"
#include "OSGRTColorPacket.h"

#include "OSGRTRaySIMDPacket.h"
#include "OSGRTHitSIMDPacket.h"
#include "OSGRTColorSIMDPacket.h"

OSG_BEGIN_NAMESPACE

class RTCameraDecorator;

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTScene : public MemoryObject
{

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTScene<DescT>                        Self;

    typedef DescT                                 Desc;

    typedef typename Desc::RayPacket              RayPacket;
    typedef typename Desc::HitPacket              HitPacket;
    typedef typename Desc::RTCache                RTCache;
    typedef typename RTCache::ElemStack           ElemStack;

    typedef typename Desc::BasicRayPacket         BasicRayPacket;

    typedef typename Desc::BasicSIMDRayPacket     BasicSIMDRayPacket;

    typedef typename std::vector<RTCache *>       RTCacheStore;

    typedef          RTRaySIMDPacketInfo::RayMode RayMode;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTScene(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    RayMode getRayMode(void         );
    void    setRayMode(RayMode eMode);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

          void          addCache     (RTCache *pCache);
          void          clearCache   (void           );

          UInt32        getNumCaches (void           );

    const RTCacheStore &getCacheArray(void           );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void               setCamera(RTCameraDecorator *pCam);
    RTCameraDecorator *getCamera(void                   );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void tracePrimaryRays    (BasicRayPacket  &oRay, 
                              HitPacket       &oHit,
                              ElemStack       &sKDToDoStack,
                              UInt16          *uiActive     );
    void tracePrimaryRaysSDQO(BasicRayPacket  &oRay, 
                              HitPacket       &oHit,
                              ElemStack       &sKDToDoStack,
                              UInt16          *uiActive     );

    void tracePrimaryRays    (BasicSIMDRayPacket &oRay, 
                              HitPacket          &oHit,
                              ElemStack          &sKDToDoStack,
                              UInt16             *uiActive     );

    void tracePrimaryRaysSDQO(BasicSIMDRayPacket &oRay, 
                              HitPacket          &oHit,
                              ElemStack          &sKDToDoStack,
                              UInt16             *uiActive     );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void shade    (RTHitPacket        &oHit,
                   BasicRayPacket     &oRay,
                   RTColorPacket      &oResult);

    void shadeSDQO(RTHitPacket        &oHit,
                   BasicRayPacket     &oRay,
                   RTColorPacket      &oResult);

    void shade    (RTHitSIMDPacket    &oHit,
                   BasicSIMDRayPacket &oRay,
                   RTColorSIMDPacket  &oResult);

    void shadeSDQO(RTHitSIMDPacket    &oHit,
                   BasicSIMDRayPacket &oRay,
                   RTColorSIMDPacket  &oResult);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void setBackgroundColor(Real32 rR,
                            Real32 rG,
                            Real32 rB,
                            Real32 rA);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    virtual void resolveLinks(void);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef MemoryObject Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    RTRaySIMDPacketInfo::RayMode  _eRayMode;
    Color4f                       _cBackground;

    RTCameraDecorator            *_pCam;
    RTCacheStore                  _vRTCaches;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTScene(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    RTScene(const RTScene &source);
    void operator =(const RTScene &source);
};

OSG_END_NAMESPACE

#include "OSGRTScene.inl"

#endif /* _OSGRTSCENE_H_ */
