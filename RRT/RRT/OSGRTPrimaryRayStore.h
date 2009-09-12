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

OSG_BEGIN_NAMESPACE

class Camera;
class RTTarget;

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
    typedef          DescT                 Desc;
    typedef typename Desc::SingleRayPacket SingleRayPacket;

    static void setupRays(RTPrimaryRayStore<DescT> *pThis,
                          Camera                   &pCam, 
                          RTTarget                 &pTarget);
};

template<typename DescT>
struct RTPrimaryRayStoreSetupHelper<DescT, RTSIMDMathTag>
{
    typedef          DescT                 Desc;
    typedef typename Desc::SingleRayPacket SingleRayPacket;

    static void setupRays(RTPrimaryRayStore<DescT> *pThis,
                          Camera                   &pCam, 
                          RTTarget                 &pTarget);

    static void fillTile (RTPrimaryRayStore<DescT> *pThis,
                          Vec3f                     vCurr, 
                          Vec3f                     vRight, 
                          Vec3f                     vUp,
                          Pnt3f                     vOrigin,
                          UInt32                    uiX,
                          UInt32                    uiY,
                          UInt32                    uiTilesX);
};

template<typename DescT>
class RTPrimaryRayStore : public RTStore
{

    /*==========================  PUBLIC  =================================*/

  public:

    static const UInt32 Empty = UINT_MAX;

    typedef          DescT                 Desc;
    typedef typename Desc::SingleRayPacket SingleRayPacket;
    typedef typename Desc::MathTag         MathTag;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTPrimaryRayStore(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void startFrame(Camera &pCam, RTTarget &pTarget);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    UInt32           nextIndex   (void             );
    SingleRayPacket &getRayPacket(UInt32 uiRayIndex);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef RTStore                      Inherited;

    typedef std::vector<SingleRayPacket> RayStore;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    UInt32   _uiNumRays;
    UInt32   _uiCurrentRay;
    RayStore _vRays;

    Lock    *_pStoreLock;

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
