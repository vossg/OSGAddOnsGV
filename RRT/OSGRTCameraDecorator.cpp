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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include <OSGConfig.h>

#include "OSGRTCameraDecorator.h"

#include "OSGPerspectiveCamera.h"

#include "OSGRTTarget.h"

#include "OSGRTFourRaySIMDPacket.h"
#include "OSGRTSingleRayPacket.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGRTCameraDecoratorBase.cpp file.
// To modify it, please change the .fcd file (OSGRTCameraDecorator.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void RTCameraDecorator::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
    }
}


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

RTCameraDecorator::RTCameraDecorator(void) :
    Inherited()
{
}

RTCameraDecorator::RTCameraDecorator(const RTCameraDecorator &source) :
    Inherited(source)
{
}

RTCameraDecorator::~RTCameraDecorator(void)
{
}

/*----------------------------- class specific ----------------------------*/

void RTCameraDecorator::changed(ConstFieldMaskArg whichField, 
                            UInt32            origin,
                            BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void RTCameraDecorator::getProjection(Matrix &result, 
                                      UInt32  width, 
                                      UInt32  height)
{
    if(getDecoratee() != NULL)
    {
        getDecoratee()->getProjection(result, width, height);
    }
}

void RTCameraDecorator::getProjectionTranslation(Matrix &result, 
                                                 UInt32  width, 
                                                 UInt32  height)
{
    if(getDecoratee() != NULL)
    {
        getDecoratee()->getProjectionTranslation(result, width, height);
    }
}

void RTCameraDecorator::getViewing(Matrix &result, 
                                   UInt32  width, 
                                   UInt32  height)
{
    if(getDecoratee() != NULL)
    {
        getDecoratee()->getViewing(result, width, height);
    }
}


void RTCameraDecorator::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump RTCameraDecorator NI" << std::endl;
}


UInt32 RTCameraDecorator::fillRayStores(
    RRT::SinglePacketDesc::RayStore     &vRays,
    RRT::SinglePacketDesc::RayInfoStore &vRayInfos,
                           RTTarget     &pTarget  )
{
    typedef RRT::SinglePacketDesc::SingleRayPacket     SingleRayPacket;
    typedef RRT::SinglePacketDesc::SingleRayPacketInfo SingleRayPacketInfo;

    UInt32 uiWidth  = pTarget.getWidth ();
    UInt32 uiHeight = pTarget.getHeight();

    PerspectiveCamera *pPCam = 
        dynamic_cast<PerspectiveCamera *>(this->getDecoratee());

    if(pPCam == NULL)
    {
        fprintf(stderr, "RTCamDeco::Unknow Camera\n");
        return 0;
    }

    Matrix mCam;

    pPCam->getBeacon()->getToWorld(mCam);

    Real32 rVOff = atan(pPCam->getFov() / 2.f);
    Real32 rHOff = (rVOff * uiHeight) / uiWidth;

    Vec3f vRight(mCam[0][0], mCam[0][1], mCam[0][2]);
    Vec3f vUp   (mCam[1][0], mCam[1][1], mCam[1][2]);
    Vec3f vDir  (0.f, 0.f, -1.f);

    mCam.mult(vDir);

    vRight *= rVOff;
    vUp    *= rHOff;

    Vec3f vTopLeft = vDir - vRight - vUp;

    vRight *= 2.f / (uiWidth  - 1);
    vUp    *= 2.f / (uiHeight - 1);

    Vec3f vCurrH = vTopLeft;
    Vec3f vCurrV = vTopLeft;

    for(UInt32 i = 0; i < uiHeight; ++i)
    {
        vCurrV = vCurrH;

        for(UInt32 j = 0; j < uiWidth; ++j)
        {
            SingleRayPacket     &rayPacket = 
                vRays    [i * uiWidth + j];

            SingleRayPacketInfo &rayInfo   = 
                vRayInfos[i * uiWidth + j];

            rayPacket.setOrigin(mCam[3][0], mCam[3][1], mCam[3][2]);
            rayPacket.setDirection(vCurrV);
            rayPacket.normalizeDirection();

            rayInfo.setXY(j, i);

            vCurrV += vRight;
        }
        
        vCurrH += vUp;
    }

    return pTarget.getWidth() * pTarget.getHeight();
}

UInt32 RTCameraDecorator::fillRayStores(
    RRT::SIMDPacketDesc::SIMDRayStore     &vRays,
    RRT::SIMDPacketDesc::RayInfoStore &vRayInfos,
                         RTTarget     &pTarget,
                         UInt32        uiVTiles,
                         UInt32        uiHTiles)
{
    typedef RRT::SIMDPacketDesc::StoredSIMDPacket    FourRayPacket;
    typedef RRT::SIMDPacketDesc::SingleRayPacketInfo FourRayPacketInfo;

    UInt32 uiWidth  = pTarget.getWidth ();
    UInt32 uiHeight = pTarget.getHeight();

    UInt32 uiCurrV  = 0;
    UInt32 uiCurrH  = 0;
    UInt32 uiCountV = 0;
    UInt32 uiCountH = 0;

    PerspectiveCamera *pPCam = 
        dynamic_cast<PerspectiveCamera *>(this->getDecoratee());

    if(pPCam == NULL)
    {
        fprintf(stderr, "RTCamDeco::Unknow Camera\n");

        return 0;
    }

    Matrix mCam;

    pPCam->getBeacon()->getToWorld(mCam);

    Real32 rVOff = atan(pPCam->getFov() / 2.f);
    Real32 rHOff = (rVOff * uiHeight) / uiWidth;

    Vec3f vRight(mCam[0][0], mCam[0][1], mCam[0][2]);
    Vec3f vUp   (mCam[1][0], mCam[1][1], mCam[1][2]);
    Vec3f vDir  (0.f, 0.f, -1.f);

    mCam.mult(vDir);

    vRight *= rVOff;
    vUp    *= rHOff;

    Vec3f vTopLeft = vDir - vRight - vUp;

    vRight *= 2.f / (uiWidth  - 1);
    vUp    *= 2.f / (uiHeight - 1);

    Vec3f vCurrH = vTopLeft;
    Vec3f vCurrV = vTopLeft;

    Float4 vfCurrH[3];
    Float4 vfCurrV[3];

    vfCurrH[0] = osgSIMDSet(vTopLeft[0] + vRight[0] + vUp[0],
                            vTopLeft[0]             + vUp[0],
                            vTopLeft[0] + vRight[0],
                            vTopLeft[0]                     );

    vfCurrH[1] = osgSIMDSet(vTopLeft[1] + vRight[1] + vUp[1],
                            vTopLeft[1]             + vUp[1],
                            vTopLeft[1] + vRight[1],
                            vTopLeft[1]                     );

    vfCurrH[2] = osgSIMDSet(vTopLeft[2] + vRight[2] + vUp[2],
                            vTopLeft[2]             + vUp[2],
                            vTopLeft[2] + vRight[2],
                            vTopLeft[2]                    );

    Float4 vfRight = osgSIMDSet(
        0.f,
        vRight[0] * Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumHRays),
        vRight[1] * Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumHRays),
        vRight[2] * Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumHRays));

    Float4 vfUp = osgSIMDSet(
        0.f,
        vUp[0] * Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumVRays),
        vUp[1] * Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumVRays),
        vUp[2] * Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumVRays));

    Pnt3f vOrigin(mCam[3][0], mCam[3][1], mCam[3][2]);

    for(UInt32 i = 0; i < uiVTiles; ++i)
    {
        vCurrV = vCurrH;

        vfCurrV[0] = vfCurrH[0];
        vfCurrV[1] = vfCurrH[1];
        vfCurrV[2] = vfCurrH[2];

        for(UInt32 j = 0; j < uiHTiles; j++)
        {
            FourRayPacket     &rayTile = vRays    [i * uiHTiles + j];
            FourRayPacketInfo &rayInfo = vRayInfos[i * uiHTiles + j];

            OSG_ASSERT(i * uiHTiles + j < vRays.size());


            fillTile(pTarget.getWidth (),
                     pTarget.getHeight(),
                     j, 
                     i, 
                     rayInfo            );

            rayTile.setOrigin(vOrigin);
            rayInfo.setXY(j, i);
            rayTile.setDirections(vfCurrV[0],
                                  vfCurrV[1],
                                  vfCurrV[2]);

            rayTile.normalizeDirection();

            vfCurrV[0] = osgSIMDAdd(osgSIMDSplat(vfRight, 2), vfCurrV[0]);
            vfCurrV[1] = osgSIMDAdd(osgSIMDSplat(vfRight, 1), vfCurrV[1]);
            vfCurrV[2] = osgSIMDAdd(osgSIMDSplat(vfRight, 0), vfCurrV[2]);


            vCurrV += 
                Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumHRays) * vRight;
        }

        vfCurrH[0] = osgSIMDAdd(osgSIMDSplat(vfUp, 2), vfCurrH[0]);
        vfCurrH[1] = osgSIMDAdd(osgSIMDSplat(vfUp, 1), vfCurrH[1]);
        vfCurrH[2] = osgSIMDAdd(osgSIMDSplat(vfUp, 0), vfCurrH[2]);

        vCurrH += Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumVRays) * vUp;
    }

    return (uiVTiles * uiHTiles);
}


void RTCameraDecorator::fillTile(
                         UInt32               uiWidth,
                         UInt32               uiHeight,
                         UInt32               uiX,
                         UInt32               uiY,
    RRT::SIMDPacketDesc::SingleRayPacketInfo &rayInfo )
{
    typedef RRT::SIMDPacketDesc::StoredSIMDPacket    FourRayPacket;
    typedef RRT::SIMDPacketDesc::SingleRayPacketInfo FourRayPacketInfo;

    for(UInt32 i = 0; i < FourRayPacket::NumVRays; ++i)
    {
        UInt32 cY = uiY * FourRayPacket::NumVRays + i;

        if(cY >= uiHeight)
        {
            for(UInt32 j = 0; j < FourRayPacket::NumHRays; ++j)
            {
                UInt32 uiPacketIndex = i * FourRayPacket::NumHRays + j;

                rayInfo.setActive(false, uiPacketIndex);
            }
        }
        else
        {
            for(UInt32 j = 0; j < FourRayPacket::NumHRays; ++j)
            {
                UInt32 uiPacketIndex = i * FourRayPacket::NumHRays + j;

                UInt32 cX = uiX * FourRayPacket::NumHRays + j;

                if(cX >= uiWidth)
                {
                    rayInfo.setActive(false, uiPacketIndex);
                }
                else
                {
                    rayInfo.setActive(true, uiPacketIndex);
                }
            }
        }
    }
}

UInt32 RTCameraDecorator::fillRayStores(
        RRT::SIMDPacketDesc  ::FullSIMDRayStore &vRays,
        RRT::SIMDPacketDesc  ::RayInfoStore     &vRayInfos,
                               RTTarget         &pTarget,
                               UInt32            uiVTiles,
                               UInt32            uiHTiles )
{
    UInt32 uiWidth  = pTarget.getWidth ();
    UInt32 uiHeight = pTarget.getHeight();

    UInt32 uiCurrV  = 0;
    UInt32 uiCurrH  = 0;
    UInt32 uiCountV = 0;
    UInt32 uiCountH = 0;

    PerspectiveCamera *pPCam = 
        dynamic_cast<PerspectiveCamera *>(this->getDecoratee());

    if(pPCam == NULL)
    {
        fprintf(stderr, "RTCamDeco::Unknow Camera\n");

        return 0;
    }

    Matrix mCam;

    pPCam->getBeacon()->getToWorld(mCam);

    Real32 rVOff = atan(pPCam->getFov() / 2.f);
    Real32 rHOff = (rVOff * uiHeight) / uiWidth;

    Vec3f vRight(mCam[0][0], mCam[0][1], mCam[0][2]);
    Vec3f vUp   (mCam[1][0], mCam[1][1], mCam[1][2]);
    Vec3f vDir  (0.f, 0.f, -1.f);

    mCam.mult(vDir);

    vRight *= rVOff;
    vUp    *= rHOff;

//    vRight.setValues(0.f, 0.f, 0.f);
//    vUp.setValues   (0.f, 0.f, 0.f);

    Vec3f vTopLeft = vDir - vRight - vUp;

    vRight *= 2.f / (uiWidth  - 1);
    vUp    *= 2.f / (uiHeight - 1);

    Vec3f vCurrH = vTopLeft;
    Vec3f vCurrV = vTopLeft;

    Pnt3f vOrigin(mCam[3][0], mCam[3][1], mCam[3][2]);

    for(UInt32 i = 0; i < uiVTiles; ++i)
    {
        vCurrV = vCurrH;

        for(UInt32 j = 0; j < uiHTiles; j++)
        {
            fillTile(vRays,
                     vRayInfos,
                     pTarget.getWidth(),
                     pTarget.getHeight(),
                     vCurrV, 
                     vRight, 
                     vUp, 
                     vOrigin, 
                     j, 
                     i, 
                     uiHTiles);

            vCurrV += 
                Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumHRays) * vRight;
        }

        vCurrH += Real32(RRT::SIMDPacketDesc::SingleRayPacket::NumVRays) * vUp;
    }

    return (uiVTiles * uiHTiles);
}


void RTCameraDecorator::fillTile(
    RRT::SIMDPacketDesc::FullSIMDRayStore &vRays,
    RRT::SIMDPacketDesc::RayInfoStore     &vRayInfos,
                         UInt32            uiWidth,
                         UInt32            uiHeight,
                         Vec3f             vCurr, 
                         Vec3f             vRight, 
                         Vec3f             vUp,
                         Pnt3f             vOrigin,
                         UInt32            uiX,
                         UInt32            uiY,
                         UInt32            uiTilesX )
{
    typedef RRT::SIMDPacketDesc::StoredFullSIMDPacket FourRayPacket;
    typedef RRT::SIMDPacketDesc::SingleRayPacketInfo  FourRayPacketInfo;

    Vec3f vCurrH = vCurr;
    Vec3f vCurrV = vCurr;

    FourRayPacket     &rayTile = vRays    [uiY * uiTilesX + uiX];
    FourRayPacketInfo &rayInfo = vRayInfos[uiY * uiTilesX + uiX];

//    fprintf(stderr, "%d %d\n", uiY * uiTilesX + uiX, _vTiles.size());

    OSG_ASSERT(uiY * uiTilesX + uiX < vRays.size());

    for(UInt32 i = 0; i < FourRayPacket::NumVRays; ++i)
    {
        vCurrV = vCurrH;

        UInt32 cY = uiY * FourRayPacket::NumVRays + i;

        if(cY >= uiHeight)
        {
            for(UInt32 j = 0; j < FourRayPacket::NumHRays; ++j)
            {
                UInt32 uiPacketIndex = i * FourRayPacket::NumHRays + j;

                rayInfo.setActive(false, uiPacketIndex);
            }
        }
        else
        {
            for(UInt32 j = 0; j < FourRayPacket::NumHRays; ++j)
            {
                UInt32 uiPacketIndex = i * FourRayPacket::NumHRays + j;

                UInt32 cX = uiX * FourRayPacket::NumHRays + j;

                if(cX >= uiWidth)
                {
                    rayInfo.setActive(false, uiPacketIndex);
                }
                else
                {
                    rayInfo.setActive(true, uiPacketIndex);

                    rayTile.setOrigin(vOrigin, uiPacketIndex);
                    
                    rayTile.setDirection(vCurrV, uiPacketIndex);
                    
                    rayInfo.setXY(uiX, uiY);
                    
                    vCurrV += vRight;
                }
            }

            vCurrH += vUp;
        }
    }

    rayTile.normalizeDirection();
}

OSG_END_NAMESPACE
