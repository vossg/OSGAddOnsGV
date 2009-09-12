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

#ifndef _OSGRTCOLORSIMDPACKET_H_
#define _OSGRTCOLORSIMDPACKET_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTSIMDPacket.h"

OSG_BEGIN_NAMESPACE

/*! \brief RTTarget class. See \ref
           PageContribRRTRTTarget for a description.
*/

class OSG_CONTRIBRRT_DLLMAPPING RTColorSIMDPacket : public RTSIMDPacket
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTSIMDPacket      Inherited;
    typedef RTColorSIMDPacket Self;

    static const UInt32 NumHColors = Inherited::NumHElements;
    static const UInt32 NumVColors = Inherited::NumVElements;

    static const UInt32 NumColors  = Inherited::NumElements;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RTColorSIMDPacket(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    ~RTColorSIMDPacket(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    void reset(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void set(UInt32  uiIdx,
             Real32  rR, 
             Real32  rG, 
             Real32  rB,
             Real32  rA);

    void set(UInt32  uiIdx,
             Color4f oCol);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    Color4f getColor(UInt32 uiIdx);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTColorSIMDPacket &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

    Color4f _aColor[NumColors];

  private:
    
    RTColorSIMDPacket(const RTColorSIMDPacket &source);
};

OSG_END_NAMESPACE

#include "OSGRTColorSIMDPacket.inl"

#endif /* _OSGRTCOLORPACKET_H_ */
