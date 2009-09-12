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

#ifndef _OSGRTHITTILE_H_
#define _OSGRTHITTILE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTTile.h"
#include "OSGRTHitPacket.h"
#include "OSGRTPrimaryRayTile.h"

OSG_BEGIN_NAMESPACE

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTHitTile : public RTTile
{

    /*==========================  PUBLIC  =================================*/

  public:

    static const UInt32 NumHits = RTTile::NumHPackets * RTTile::NumVPackets;

    typedef RTHitTile<DescT>                Self;

    typedef DescT                           Desc;

    typedef typename Desc::HitPacket        HitPacket;

    typedef typename Desc::RayPacket        RayPacket;
    typedef typename Desc::PrimaryRayTile   PrimaryRayTile;

    typedef std::vector<UInt32   >          ActiveStore;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTHitTile(void);
    RTHitTile(const RTHitTile &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTHitTile(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void setXY(UInt32 uiX, 
               UInt32 uiY);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    UInt32 getX(void);
    UInt32 getY(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    HitPacket &getPacket(UInt32 uiPacketIndex);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void       setRayTile  (PrimaryRayTile *pRayTile     );
    RayPacket &getRayPacket(UInt32          uiPacketIndex);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void setActive(ActiveStore &active       );
    bool hasActive(UInt32       uiPacketIndex);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void reset(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void operator =(const RTHitTile &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef RTTile                 Inherited;
    typedef std::vector<HitPacket> HitStore;

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    HitStore        _vHitStore;
    ActiveStore     _vActiveStore;

    UInt32          _uiX;
    UInt32          _uiY;

    PrimaryRayTile *_pRayTile;

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
};
 
OSG_END_NAMESPACE

#include "OSGRTHitTile.inl"

#endif /* _OSGRTHITTILE_H_ */
