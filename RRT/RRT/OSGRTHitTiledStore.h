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

#ifndef _OSGRTHITTILEDSTORE_H_
#define _OSGRTHITTILEDSTORE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTTileStore.h"

#include "OSGCondVar.h"
#include "OSGLock.h"

OSG_BEGIN_NAMESPACE

class RTTarget;

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<class DescT>
class OSG_CONTRIBRRT_DLLMAPPING RTHitTiledStore : public RTTileStore
{

    /*==========================  PUBLIC  =================================*/

  public:

    static const UInt32 Empty   = UINT_MAX;
    static const UInt32 Waiting = UINT_MAX - 1;

    typedef          DescT         Desc;
    typedef typename Desc::HitTile HitTile;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTHitTiledStore(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void wait     (void);
    void signal   (void);
    void broadcast(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void startFrame(RTTarget &pTarget);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    UInt32   getWriteIndex (void          );
    UInt32   getReadIndex  (void          );
 
    void     pushWriteIndex(UInt32 uiIndex);

    HitTile &getPacket     (UInt32 uiIndex);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef RTTileStore Inherited;

    typedef std::vector<HitTile>  HitStore;
    typedef std::vector<UInt32 >  HitIndexStore;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    UInt32         _uiCurrentHit;
    UInt32         _uiAvailableHits;
    UInt32         _uiServedHits;

    HitStore       _vHits;
    HitIndexStore  _vAvailableHits;
 
    Lock          *_pStoreLock;
    CondVar       *_pCond;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTHitTiledStore(void); 

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
    RTHitTiledStore(const RTHitTiledStore &source);
    void operator =(const RTHitTiledStore &source);
};

OSG_END_NAMESPACE

#include "OSGRTHitTiledStore.inl"

#endif /* _OSGRTHITTILEDSTORE_H_ */
