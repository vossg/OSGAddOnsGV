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

#ifndef _OSGRTRAYSIMDPACKET_H_
#define _OSGRTRAYSIMDPACKET_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTSIMDPacket.h"
#include "OSGVector.h"

OSG_BEGIN_NAMESPACE

class OSG_CONTRIBRRT_DLLMAPPING RTRaySIMDPacketInfo
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTRaySIMDPacketInfo Self;

    static const UInt32 NumHRays = RTSIMDPacket::NumHElements;
    static const UInt32 NumVRays = RTSIMDPacket::NumVElements;

    static const UInt32 NumRays  = RTSIMDPacket::NumElements;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RTRaySIMDPacketInfo(void);
    RTRaySIMDPacketInfo(const RTRaySIMDPacketInfo &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    ~RTRaySIMDPacketInfo(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    void    setActive    (bool   bVal,
                          UInt32 uiIdx);
    bool    isActive     (UInt32 uiIdx);

    bool    hasActive    (void        );

    UInt16 *getActiveRays(void        );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTRaySIMDPacketInfo &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    UInt16 _bIsActive[NumRays];
};

/*! \brief RTTarget class. See \ref
           PageContribRRTRTTarget for a description.
*/

class OSG_CONTRIBRRT_DLLMAPPING RTRaySIMDPacket : public RTSIMDPacket
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:
    
    typedef RTSIMDPacket    Inherited;
    typedef RTRaySIMDPacket Self;

    static const UInt32 NumHRays = Inherited::NumHElements;
    static const UInt32 NumVRays = Inherited::NumVElements;

    static const UInt32 NumRays  = Inherited::NumElements;

    static const UInt32 X        = 0;
    static const UInt32 Y        = 1;
    static const UInt32 Z        = 2;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RTRaySIMDPacket(void);
    RTRaySIMDPacket(const RTRaySIMDPacket &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    ~RTRaySIMDPacket(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    void setOrigin         (Real32 oX,
                            Real32 oY,
                            Real32 oZ     );

    void setOrigin         (Pnt3f  vOrigin);

    void setOrigin         (Float4 origin );


    void setDirection      (Vec3f  vDir,
                            UInt32 uiIdx  );

    void setDirX           (Float4 fDir   );
    void setDirY           (Float4 fDir   );
    void setDirZ           (Float4 fDir   );

    void normalizeDirection(void          );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

#ifdef OSG_SIMD_RAYPACKET_DEBUG
    Pnt3f getOriginPnt         (      void          );

    Vec3f getDirVec            (const UInt32 uiCoord);
#endif

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    Real32 getOriginComp(const UInt32 uiCoord);
    Float4 getOrigin    (      void          );

    Float4 getDir       (const UInt32 uiCoord);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTRaySIMDPacket &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

    typedef Real32 Real32_4[4];

#ifdef OSG_SIMD_RAYPACKET_DEBUG
    Pnt3f  _vOrigin;
    Real32 _pad0;

    Vec3f  _vDir[4];
#endif

    union
    {
        Float4  _fOrigin;
        Real32  _vOriginA[4];
    };

    union
    {
        Float4    _fDir [3];
        Real32_4  _vDirA[3];
    };

  private:
};

OSG_END_NAMESPACE

#include "OSGRTRaySIMDPacket.inl"

#endif /* _OSGRTRAYSIMDPACKET_H_ */
