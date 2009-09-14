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

#ifndef _OSGRTCAMERADECORATOR_H_
#define _OSGRTCAMERADECORATOR_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTCameraDecoratorBase.h"

#include "OSGRRTDefinitions.h"
#include "OSGRTRaySIMDPacket.h"

OSG_BEGIN_NAMESPACE

class RTTarget;
class PerspectiveCamera;
class OrthographicCamera;

/*! \brief RTCameraDecorator class. See \ref
           PageContribRRTRTCameraDecorator for a description.
*/

class OSG_CONTRIBRRT_DLLMAPPING RTCameraDecorator : public RTCameraDecoratorBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTCameraDecoratorBase        Inherited;
    typedef RTCameraDecorator            Self;

    typedef RTRaySIMDPacketInfo::RayMode RayMode;
    
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    access                                    */
    /*! \{                                                                 */

    virtual void getProjection           (Matrix &result, 
                                          UInt32  width, 
                                          UInt32  height);

    virtual void getProjectionTranslation(Matrix &result, 
                                          UInt32  width, 
                                          UInt32  height);

    virtual void getViewing              (Matrix &result, 
                                          UInt32  width, 
                                          UInt32  height);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual UInt32 fillRayStores(RRT::SinglePacketDesc::RayStore     &vRays,
                                 RRT::SinglePacketDesc::RayInfoStore &vRayInfos,
                                                        RTTarget     &pTarget );

    virtual UInt32 fillRayStores(RRT::SIMDPacketDesc  ::SIMDRayStore &vRays,
                                 RRT::SIMDPacketDesc  ::RayInfoStore &vRayInfos,
                                                        RTTarget     &pTarget,
                                                        UInt32        uiVTiles,
                                                        UInt32        uiHTiles,
                                                        RayMode      &eRayMode);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in RTCameraDecoratorBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RTCameraDecorator(void);
    RTCameraDecorator(const RTCameraDecorator &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~RTCameraDecorator(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void   fillTile     (                     UInt32               uiWidth,
                                              UInt32               uiHeight,
                                              UInt32               uiX,
                                              UInt32               uiY,
                         RRT::SIMDPacketDesc::SingleRayPacketInfo &rayInfo );

    UInt32 fillRayStores(RRT::SIMDPacketDesc::SIMDRayStore        &vRays,
                         RRT::SIMDPacketDesc::RayInfoStore        &vRayInfos,
                                              RTTarget            &pTarget,
                                              PerspectiveCamera   *pPCam,
                                              UInt32               uiVTiles,
                                              UInt32               uiHTiles);

    UInt32 fillRayStores(RRT::SIMDPacketDesc::SIMDRayStore        &vRays,
                         RRT::SIMDPacketDesc::RayInfoStore        &vRayInfos,
                                              RTTarget            &pTarget,
                                              OrthographicCamera  *pOCam,
                                              UInt32               uiVTiles,
                                              UInt32               uiHTiles);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class RTCameraDecoratorBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const RTCameraDecorator &source);
};

typedef RTCameraDecorator *RTCameraDecoratorP;

OSG_END_NAMESPACE

#include "OSGRTCameraDecoratorBase.inl"
#include "OSGRTCameraDecorator.inl"

#endif /* _OSGRTCAMERADECORATOR_H_ */
