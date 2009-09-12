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

#ifndef _OSGRTLOCALPACKETMANAGER_H_
#define _OSGRTLOCALPACKETMANAGER_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGMemoryObject.h"
#include "OSGContribRRTDef.h"

OSG_BEGIN_NAMESPACE

class Camera;

class RTTarget;
class RTThread;

class Barrier;

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTLocalPacketManager : public MemoryObject
{

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTLocalPacketManager<DescT>         Self;

    typedef DescT                               Desc;

    typedef typename Desc::Scene                Scene;
    typedef typename Desc::HitStore             HitStore;
    typedef typename Desc::HitTiledStore        HitTiledStore;
    typedef typename Desc::PrimaryRayStore      PrimaryRayStore;
    typedef typename Desc::PrimaryRayTiledStore PrimaryRayTiledStore;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTLocalPacketManager(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void setTarget(RTTarget *pTarget);
    void setScene (Scene    *pScene );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void setup                 (bool bTiled = false);
    void setupSplitThreads     (bool bTiled = false);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void trace     (Camera *pCam, bool bTiled = false);
    void finish    (void        );

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef MemoryObject            Inherited;

    typedef std::vector<RTThread *> ThreadStore;


    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    RTTarget               *_pTarget;
    Scene                  *_pScene;

    PrimaryRayStore        *_pPrimaryRayStore;
    HitStore               *_pHitStore;

    PrimaryRayTiledStore   *_pPrimaryRayTiledStore;
    HitTiledStore          *_pHitTiledStore;

    Barrier                *_pSyncBarrier;

    ThreadStore             _vThreads;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTLocalPacketManager(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    /*! \}                                                                 */
   /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    RTLocalPacketManager(const RTLocalPacketManager &source);
    void operator =(const RTLocalPacketManager &source);
};

OSG_END_NAMESPACE

#include "OSGRTLocalPacketManager.inl"

#endif /* _OSGRTLOCALPACKETMANAGER_H_ */
