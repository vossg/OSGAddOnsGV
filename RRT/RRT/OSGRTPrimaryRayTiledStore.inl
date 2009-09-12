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

template<class DescT> inline
UInt32 RTPrimaryRayTiledStore<DescT>::nextIndex(void)
{
    UInt32 returnValue = Empty;

    _pStoreLock->acquire();

    if(_uiNumTiles != 0)
    {
        _uiNumTiles--;

        returnValue = _uiCurrentTile++;
    }

    _pStoreLock->release();

    return returnValue;
}

template<class DescT> inline
typename RTPrimaryRayTiledStore<DescT>::PrimaryRayTile &
    RTPrimaryRayTiledStore<DescT>::getRayPacket(UInt32 uiTileIndex)
{
    return _vTiles[uiTileIndex];
}


template<class DescT> inline
RTPrimaryRayTiledStore<DescT>::RTPrimaryRayTiledStore(void) :
     Inherited    (    ),
    _uiCurrentTile(   0),

    _vTiles       (    ),
    _pStoreLock   (NULL)
{
    _pStoreLock = Lock::get(NULL);

    OSG::addRef(_pStoreLock);
}

template<class DescT> inline
RTPrimaryRayTiledStore<DescT>::~RTPrimaryRayTiledStore(void)
{
    OSG::subRef(_pStoreLock);
}

template<class DescT> inline
void RTPrimaryRayTiledStore<DescT>::startFrame(Camera &pCam, RTTarget &pTarget)
{
    RTPrimaryRayTiledStoreSetupHelper<DescT, MathTag>::setupRays(this,
                                                                 pCam,
                                                                 pTarget);
}

template<typename DescT> inline
void RTPrimaryRayTiledStoreSetupHelper<DescT, RTFloatMathTag>::setupRays(
    RTPrimaryRayTiledStore<DescT> *pThis,
    Camera                        &pCam, 
    RTTarget                      &pTarget)
{
    if(pThis->_uiWidth  != pTarget.getWidth() || 
       pThis->_uiHeight != pTarget.getHeight())
    {
        fprintf(stderr, "Update RTS \n================================\n"); 

        pThis->updateNumTiles(pTarget.getWidth(), 
                              pTarget.getHeight(),
                              PrimaryRayTile::NumHPackets,
                              PrimaryRayTile::NumVPackets);
        
        pThis->_vTiles.resize(pThis->_uiNumTiles);
    }

    pThis->_uiNumTiles = pThis->_uiHTiles * pThis->_uiVTiles;

    pThis->_uiCurrentTile = 0;


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

            vCurrV += Real32(PrimaryRayTile::NumHPackets) * vRight;
        }

        vCurrH -= Real32(PrimaryRayTile::NumVPackets) * vUp;
    }

#if 0
    for(UInt32 i = 0; i < uiHeight; ++i)
    {
        vCurrV = vCurrH;

        for(UInt32 j = 0; j < uiWidth; ++j)
        {
            PrimaryRayTile &rayPacket = pThis->_vRays[i * uiWidth + j];

            rayPacket.setOrigin(mCam[3][0], mCam[3][1], mCam[3][2]);
            rayPacket.setDirection(vCurrV);
            rayPacket.normalizeDirection();

            rayPacket.setXY(j, i);

            vCurrV += vRight;
        }
        
        vCurrH -= vUp;
    }
#endif
}

template<typename DescT> inline
void RTPrimaryRayTiledStoreSetupHelper<DescT, RTFloatMathTag>::fillTile(
    RTPrimaryRayTiledStore<DescT> *pThis,
    Vec3f  vCurr, 
    Vec3f  vRight, 
    Vec3f  vUp,
    Pnt3f  vOrigin,
    UInt32 uiX,
    UInt32 uiY,
    UInt32 uiTilesX)
{
    Vec3f vCurrH = vCurr;
    Vec3f vCurrV = vCurr;

    PrimaryRayTile &rayTile = pThis->_vTiles[uiY * uiTilesX + uiX];

//    fprintf(stderr, "%d %d\n", uiY * uiTilesX + uiX, _vTiles.size());

    OSG_ASSERT(uiY * uiTilesX + uiX < pThis->_vTiles.size());

    for(UInt32 i = 0; i < PrimaryRayTile::NumVPackets; ++i)
    {
        vCurrV = vCurrH;

        UInt32 cY = uiY * RTTile::NumVPackets + i;

        if(cY >= pThis->_uiHeight)
        {
            for(UInt32 j = 0; j < PrimaryRayTile::NumHPackets; ++j)
            {
                UInt32 uiPacketIndex = i * PrimaryRayTile::NumHPackets + j;

                rayTile.setActive(uiPacketIndex, false);
            }
        }
        else
        {
            for(UInt32 j = 0; j < PrimaryRayTile::NumHPackets; ++j)
            {
                UInt32 uiPacketIndex = i * PrimaryRayTile::NumHPackets + j;

                UInt32 cX = uiX * RTTile::NumHPackets + j;

                if(cX >= pThis->_uiWidth)
                {
                    rayTile.setActive(uiPacketIndex, false);
                }
                else
                {
                    rayTile.setActive(uiPacketIndex, true);

                    rayTile.setOrigin(uiPacketIndex,
                                      vOrigin);
                    
                    rayTile.setDirection(uiPacketIndex, vCurrV);
                    
                    rayTile.setXY(uiX, uiY);

                    vCurrV += vRight;
                }
            }

            vCurrH -= vUp;
        }
    }

    rayTile.normalizeDirection();
}

template<typename DescT> inline
void RTPrimaryRayTiledStoreSetupHelper<DescT, RTSIMDMathTag>::setupRays(
    RTPrimaryRayTiledStore<DescT> *pThis,
    Camera                        &pCam, 
    RTTarget                      &pTarget)
{
    OSG_ASSERT(false);
}

OSG_END_NAMESPACE
