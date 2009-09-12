/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

#ifndef _OSGRTHITSIMDPACKET_H_
#define _OSGRTHITSIMDPACKET_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTSIMDPacket.h"

OSG_BEGIN_NAMESPACE

/*! \brief RTTarget class. See \ref
           PageContribRRTRTTarget for a description.
*/

class OSG_CONTRIBRRT_DLLMAPPING RTHitSIMDPacket : public RTSIMDPacket
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTSIMDPacket    Inherited;
    typedef RTHitSIMDPacket Self;

    static const UInt32 NumHHits = Inherited::NumHElements;
    static const UInt32 NumVHits = Inherited::NumVElements;

    static const UInt32 NumHits  = Inherited::NumElements;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RTHitSIMDPacket(void);
    RTHitSIMDPacket(const RTHitSIMDPacket &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    ~RTHitSIMDPacket(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    void reset(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void set       (UInt32 uiIdx,
                    Real32 rDist, 
                    Real32 rU, 
                    Real32 rV, 
                    UInt32 uiObjId,
                    UInt32 uiTriId,
                    UInt32 uiCacheId);

#if 0
    void set       (const UInt32 mask,
                    const Float4 rDist, 
                    const Float4 rU, 
                    const Float4 rV, 
                    const UInt32 uiObjId,
                    const UInt32 uiTriId,
                    const UInt32 uiCacheId);
#endif

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    Real32 getDist   (UInt32 uiIdx);
    Real32 getU      (UInt32 uiIdx);
    Real32 getV      (UInt32 uiIdx);

    UInt32 getObjId  (UInt32 uiIdx);
    UInt32 getTriId  (UInt32 uiIdx);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    UInt32 getCacheId(UInt32 uiIdx);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTHitSIMDPacket &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

    Real32 _rDist[4];
    Real32 _rU[4];
    Real32 _rV[4];
    
    UInt32 _uiCacheId[4];
    UInt32 _uiObjId[4];
    UInt32 _uiTriId[4];

  private:
};

OSG_END_NAMESPACE

#include "OSGRTHitSIMDPacket.inl"

#endif /* _OSGRTHITPACKET_H_ */
