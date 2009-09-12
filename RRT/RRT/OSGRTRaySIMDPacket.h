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

    void setDirection      (Vec3f  vDir,
                            UInt32 uiIdx  );

    void normalizeDirection(void          );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    Pnt3f getOrigin(void        );

    Vec3f getDir   (UInt32 uiIdx);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    void setActive(bool   bVal,
                   UInt32 uiIdx);
    bool isActive (UInt32 uiIdx);

    bool hasActive(void        );

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

    Pnt3f  _vOrigin;
    Vec3f  _vDir[NumRays];

    bool   _bIsActive[NumRays];

  private:

};

OSG_END_NAMESPACE

#include "OSGRTRaySIMDPacket.inl"

#endif /* _OSGRTRAYSIMDPACKET_H_ */
