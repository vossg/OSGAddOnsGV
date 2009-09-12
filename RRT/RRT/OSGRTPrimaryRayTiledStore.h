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

#ifndef _OSGRTPRIMARYRAYTILEDSTORE_H_
#define _OSGRTPRIMARYRAYTILEDSTORE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTStore.h"
#include "OSGVector.h"

#include "OSGCondVar.h"
#include "OSGLock.h"

#include "OSGPerspectiveCamera.h"

#include "OSGRTTile.h"

OSG_BEGIN_NAMESPACE

class Camera;
class RTTarget;
class RTCameraDecorator;

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTPrimaryRayTiledStore;

template<typename DescT, typename MathTag>
struct RTPrimaryRayTiledStoreSetupHelper;


template<typename DescT>
struct RTPrimaryRayTiledStoreSetupHelper<DescT, RTFloatMathTag>
{
    typedef          DescT                Desc;
    typedef typename Desc::PrimaryRayTile PrimaryRayTile ;

    static void setupRays(RTPrimaryRayTiledStore<DescT> *pThis,
                          RTCameraDecorator             &pCam, 
                          RTTarget                      &pTarget);

    static void fillTile (RTPrimaryRayTiledStore<DescT> *pThis,
                          Vec3f                          vCurr, 
                          Vec3f                          vRight, 
                          Vec3f                          vUp,
                          Pnt3f                          vOrigin,
                          UInt32                         uiX,
                          UInt32                         uiY,
                          UInt32                         uiTilesX);
};

template<typename DescT>
struct RTPrimaryRayTiledStoreSetupHelper<DescT, RTSIMDMathTag>
{
    typedef          DescT                Desc;
    typedef typename Desc::PrimaryRayTile PrimaryRayTile;

    static void setupRays(RTPrimaryRayTiledStore<DescT> *pThis,
                          RTCameraDecorator             &pCam, 
                          RTTarget                      &pTarget);
};

template<class DescT>
class RTPrimaryRayTiledStore : public RTStore
{

    /*==========================  PUBLIC  =================================*/

  public:

    static const UInt32 Empty = UINT_MAX;

    typedef          DescT                Desc;
    typedef typename Desc::PrimaryRayTile PrimaryRayTile;
    typedef typename Desc::MathTag        MathTag;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTPrimaryRayTiledStore(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void startFrame(RTCameraDecorator &pCam, RTTarget &pTarget);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    UInt32 getNumRayPackets(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    UInt32          nextIndex   (void              );
    PrimaryRayTile &getRayPacket(UInt32 uiTileIndex);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef RTStore Inherited;

    typedef std::vector<PrimaryRayTile>  RayStore;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    UInt32   _uiCurrentTile;
    RayStore _vTiles;

    Lock    *_pStoreLock;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTPrimaryRayTiledStore(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void fillTile(Vec3f  vCurr, 
                  Vec3f  vRight, 
                  Vec3f  vUp,
                  Pnt3f  vOrigin,
                  UInt32 uiX,
                  UInt32 uiY,
                  UInt32 uiTilesX);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    friend struct RTPrimaryRayTiledStoreSetupHelper<DescT, RTFloatMathTag>;
    friend struct RTPrimaryRayTiledStoreSetupHelper<DescT, RTSIMDMathTag >;

    /*! \}                                                                 */
   /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    RTPrimaryRayTiledStore(const RTPrimaryRayTiledStore &source);
    void operator =(const RTPrimaryRayTiledStore &source);
};
 
OSG_END_NAMESPACE

#include "OSGRTPrimaryRayTiledStore.inl"

#endif /* _OSGRTPRIMARYRAYTILEDSTORE_H_ */
