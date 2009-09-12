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

OSG_BEGIN_NAMESPACE

template<typename DescT> inline
UInt32 RTPrimaryRayStore<DescT>::nextIndex(void)
{
    UInt32 returnValue = Empty;

    _pStoreLock->acquire();

    if(_uiNumRays != 0)
    {
        _uiNumRays--;

        returnValue = _uiCurrentRay++;
    }

    _pStoreLock->release();

    return returnValue;
}

template<typename DescT> inline
typename RTPrimaryRayStore<DescT>::SingleRayPacket &
    RTPrimaryRayStore<DescT>::getRayPacket(UInt32 uiRayIndex)
{
    return _vRays[uiRayIndex];
}

template<typename DescT> inline
typename RTPrimaryRayStore<DescT>::SingleRayPacketInfo &
    RTPrimaryRayStore<DescT>::getRayPacketInfo(UInt32 uiRayIndex)
{
    return _vRayInfos[uiRayIndex];
}

template<typename DescT> inline
RTPrimaryRayStore<DescT>::RTPrimaryRayStore(void) :
     Inherited   (    ),
    _uiNumRays   (   0),
    _uiCurrentRay(   0),

    _vRays       (    ),
    _vRayInfos   (    ),

    _pStoreLock  (NULL)
{
    _pStoreLock = Lock::get(NULL);

    OSG::addRef(_pStoreLock);
}

template<typename DescT> inline
RTPrimaryRayStore<DescT>::~RTPrimaryRayStore(void)
{
    OSG::subRef(_pStoreLock);
}

template<typename DescT> inline
void RTPrimaryRayStore<DescT>::startFrame(Camera &pCam, RTTarget &pTarget)
{
    RTPrimaryRayStoreSetupHelper<DescT, MathTag>::setupRays(this,
                                                            pCam,
                                                            pTarget);
}

template<typename DescT> inline
void RTPrimaryRayStoreSetupHelper<DescT, RTFloatMathTag>::setupRays(
    RTPrimaryRayStore<DescT> *pThis,
    Camera                   &pCam, 
    RTTarget                 &pTarget)
{
    pThis->_uiNumRays    = pTarget.getWidth() * pTarget.getHeight();
    pThis->_uiCurrentRay = 0;

    pThis->_vRays    .resize(pThis->_uiNumRays);
    pThis->_vRayInfos.resize(pThis->_uiNumRays);

    UInt32 uiWidth  = pTarget.getWidth ();
    UInt32 uiHeight = pTarget.getHeight();

    PerspectiveCamera *pPCam = dynamic_cast<PerspectiveCamera *>(&pCam);

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

    Vec3f vTopLeft = vDir - vRight + vUp;

    vRight *= 2.f / (uiWidth  - 1);
    vUp    *= 2.f / (uiHeight - 1);

    Vec3f vCurrH = vTopLeft;
    Vec3f vCurrV = vTopLeft;

    for(UInt32 i = 0; i < uiHeight; ++i)
    {
        vCurrV = vCurrH;

        for(UInt32 j = 0; j < uiWidth; ++j)
        {
#if 0
            fprintf(stderr, "%d %d\n", j, i);
            fprintf(stderr, "%f %f %f\n", mCam[3][0], mCam[3][1], mCam[3][2]);
            fprintf(stderr, "%f %f %f\n", vCurrV[0], vCurrV[1], vCurrV[2]);
#endif

            SingleRayPacket     &rayPacket = 
                pThis->_vRays    [i * uiWidth + j];

            SingleRayPacketInfo &rayInfo   = 
                pThis->_vRayInfos[i * uiWidth + j];

            rayPacket.setOrigin(mCam[3][0], mCam[3][1], mCam[3][2]);
            rayPacket.setDirection(vCurrV);
            rayPacket.normalizeDirection();

            rayInfo.setXY(j, i);

            vCurrV += vRight;
        }
        
        vCurrH -= vUp;
    }

//  fprintf(stderr, "RS %d\n", pThis->_uiNumRays);
}

template<typename DescT> inline
void RTPrimaryRayStoreSetupHelper<DescT, RTSIMDMathTag>::setupRays(
    RTPrimaryRayStore<DescT> *pThis,
    Camera                   &pCam, 
    RTTarget                 &pTarget)
{
#if 0
    pThis->_uiNumRays    = pTarget.getWidth() * pTarget.getHeight();
    pThis->_uiCurrentRay = 0;

    pThis->_vRays.resize(pThis->_uiNumRays);

    UInt32 uiWidth  = pTarget.getWidth ();
    UInt32 uiHeight = pTarget.getHeight();

    PerspectiveCamera *pPCam = dynamic_cast<PerspectiveCamera *>(&pCam);

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

    Vec3f vTopLeft = vDir - vRight + vUp;

    vRight *= 2.f / (uiWidth  - 1);
    vUp    *= 2.f / (uiHeight - 1);

    Vec3f vCurrH = vTopLeft;
    Vec3f vCurrV = vTopLeft;

    for(UInt32 i = 0; i < uiHeight; ++i)
    {
        vCurrV = vCurrH;

        for(UInt32 j = 0; j < uiWidth; ++j)
        {
#if 0
            fprintf(stderr, "%d %d\n", j, i);
            fprintf(stderr, "%f %f %f\n", mCam[3][0], mCam[3][1], mCam[3][2]);
            fprintf(stderr, "%f %f %f\n", vCurrV[0], vCurrV[1], vCurrV[2]);
#endif

            SingleRayPacket &rayPacket = pThis->_vRays[i * uiWidth + j];

            rayPacket.setOrigin(mCam[3][0], mCam[3][1], mCam[3][2]);
            rayPacket.setDirection(vCurrV);
            rayPacket.normalizeDirection();

            rayPacket.setXY(j, i);

            vCurrV += vRight;
        }
        
        vCurrH -= vUp;
    }

//    fprintf(stderr, "RS %d\n", pThis->_uiNumRays);
#endif

    // Hack need full SIMD impl.

    if(pThis->_uiWidth  != pTarget.getWidth() || 
       pThis->_uiHeight != pTarget.getHeight())
    {
        fprintf(stderr, "Update RS SIMD \n==============================\n"); 

        pThis->updateNumTiles(pTarget.getWidth(), 
                              pTarget.getHeight(),
                              FourRayPacket::NumHRays,
                              FourRayPacket::NumVRays);
        
        pThis->_vRays    .resize(pThis->_uiNumTiles);
        pThis->_vRayInfos.resize(pThis->_uiNumTiles);
    }

    pThis->_uiNumRays    = pThis->_uiHTiles * pThis->_uiVTiles;

    pThis->_uiCurrentRay = 0;


    UInt32 uiWidth  = pTarget.getWidth ();
    UInt32 uiHeight = pTarget.getHeight();

    UInt32 uiCurrV  = 0;
    UInt32 uiCurrH  = 0;
    UInt32 uiCountV = 0;
    UInt32 uiCountH = 0;

    PerspectiveCamera *pPCam = dynamic_cast<PerspectiveCamera *>(&pCam);

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

    Vec3f vTopLeft = vDir - vRight + vUp;

    vRight *= 2.f / (uiWidth  - 1);
    vUp    *= 2.f / (uiHeight - 1);

    Vec3f vCurrH = vTopLeft;
    Vec3f vCurrV = vTopLeft;

    Pnt3f vOrigin(mCam[3][0], mCam[3][1], mCam[3][2]);

#if 0
    printf("ray setup vectors:\n");
    printf("vRight %f %f %f ", vRight[0], vRight[1], vRight[2]);
    printf("vUp %f %f %f\n", vUp[0], vUp[1], vUp[2]);
    printf("vOrigin %f %f %f ", vOrigin[0], vOrigin[1], vOrigin[2]);
    printf("vTopLeft %f %f %f ", vTopLeft[0], vTopLeft[1], vTopLeft[2]);
    printf("rVOff rHOff %f %f", rVOff, rHOff);
    printf("\n");
    exit(0);
#endif

    for(UInt32 i = 0; i < pThis->_uiVTiles; ++i)
    {
        vCurrV = vCurrH;

        for(UInt32 j = 0; j < pThis->_uiHTiles; j++)
        {
            fillTile(pThis,
                     vCurrV, 
                     vRight, 
                     vUp, 
                     vOrigin, 
                     j, 
                     i, 
                     pThis->_uiHTiles);

            vCurrV += Real32(FourRayPacket::NumHRays) * vRight;
        }

        vCurrH -= Real32(FourRayPacket::NumVRays) * vUp;
    }

#if 0    // Write binary to file

    FILE* fp;

    fp=fopen(
        "/home/filip/tmp/OpenSG/Standalone.app/primaryraysfull_PPU.bin", "wb+");
	  
    for(UInt32 i = 0; i < pThis->getNumVTiles() ; ++i)
    {
        for(UInt32 j = 0; j < pThis->getNumHTiles() ; ++j)
        {
            UInt32 uiRayIndex = i  * pThis->getNumHTiles() + j;

            FourRayPacket     &oRayTile = 
                pThis->getRayPacket(uiRayIndex);
	
            union
            {
                Float4 dir;
                Real32 arrayDir[4];
            };
		 
            dir = oRayTile.getDir(0);
            fwrite(arrayDir, 
                   sizeof(arrayDir[0]), 
                   sizeof(arrayDir)/sizeof(arrayDir[0]), fp);
		  
            dir = oRayTile.getDir(1);
            fwrite(arrayDir, 
                   sizeof(arrayDir[0]), 
                   sizeof(arrayDir)/sizeof(arrayDir[0]), fp);
	

            dir = oRayTile.getDir(2);
            fwrite(arrayDir, 
                   sizeof(arrayDir[0]), 
                   sizeof(arrayDir)/sizeof(arrayDir[0]), fp);
        }
    }
		
		
    fclose(fp);
	      
    printf("Binary raydata written to file\n");
    exit(0);

#endif
}

template<typename DescT> inline
void RTPrimaryRayStoreSetupHelper<DescT, RTSIMDMathTag>::fillTile(
    RTPrimaryRayStore<DescT> *pThis,
    Vec3f                     vCurr, 
    Vec3f                     vRight, 
    Vec3f                     vUp,
    Pnt3f                     vOrigin,
    UInt32                    uiX,
    UInt32                    uiY,
    UInt32                    uiTilesX)
{
    Vec3f vCurrH = vCurr;
    Vec3f vCurrV = vCurr;

    FourRayPacket     &rayTile = pThis->_vRays    [uiY * uiTilesX + uiX];
    FourRayPacketInfo &rayInfo = pThis->_vRayInfos[uiY * uiTilesX + uiX];

//    fprintf(stderr, "%d %d\n", uiY * uiTilesX + uiX, _vTiles.size());

    OSG_ASSERT(uiY * uiTilesX + uiX < pThis->_vRays.size());

    for(UInt32 i = 0; i < FourRayPacket::NumVRays; ++i)
    {
        vCurrV = vCurrH;

        UInt32 cY = uiY * FourRayPacket::NumVRays + i;

        if(cY >= pThis->_uiHeight)
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

                if(cX >= pThis->_uiWidth)
                {
                    rayInfo.setActive(false, uiPacketIndex);
                }
                else
                {
                    rayInfo.setActive(true, uiPacketIndex);

                    rayTile.setOrigin(vOrigin);
                    
                    Vec3f vDir = vCurrV;
                    vDir.normalize();
                    
                    rayTile.setDirection(vCurrV, uiPacketIndex);
                    
                    rayInfo.setXY(uiX, uiY);
                    
                    vCurrV += vRight;
                }
            }

            vCurrH -= vUp;
        }
    }

    rayTile.normalizeDirection();
}

OSG_END_NAMESPACE
