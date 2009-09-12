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
void RTPrimaryRayStore<DescT>::startFrame(RTCameraDecorator &pCam, 
                                          RTTarget          &pTarget)
{
    RTPrimaryRayStoreSetupHelper<DescT, MathTag>::setupRays(this,
                                                            pCam,
                                                            pTarget);
}

#ifdef OSG_CELL

template<typename DescT> inline
void RTPrimaryRayStoreSetupHelper<DescT, RTSIMDMathTag>::setupRays(
    RTPrimaryRayStore<DescT> *pThis,
    RTCameraDecorator        &pCam, 
    RTTarget                 &pTarget)
{
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
}

template<typename DescT> inline
void RTPrimaryRayStore<DescT>::convCellStruct(UInt32         tileIndex,
                                              rayTile_t     *rayTile,
                                              rayInfoTile_t *rayInfoTile)
{
    OSG_ASSERT(tileIndex < _uiHTiles * _uiVTiles);

    SingleRayPacket     &rayPacket     = _vRays    [tileIndex];
    SingleRayPacketInfo &rayInfoPacket = _vRayInfos[tileIndex];

    rayPacket.setOrigin(rayTile->fOrigin);

    rayPacket.setDirX  (rayTile->fDir[0]);
    rayPacket.setDirY  (rayTile->fDir[1]);
    rayPacket.setDirZ  (rayTile->fDir[2]);
  

    rayInfoPacket.setActive(rayInfoTile->bIsActive[0], 0);
    rayInfoPacket.setActive(rayInfoTile->bIsActive[1], 1);
    rayInfoPacket.setActive(rayInfoTile->bIsActive[2], 2);
    rayInfoPacket.setActive(rayInfoTile->bIsActive[3], 3);

    rayInfoPacket.setXY(rayInfoTile->uiX, 
                        rayInfoTile->uiY);
}

#else // OSG_CELL

template<typename DescT> inline
void RTPrimaryRayStoreSetupHelper<DescT, RTFloatMathTag>::setupRays(
    RTPrimaryRayStore<DescT> *pThis,
    RTCameraDecorator        &pCam, 
    RTTarget                 &pTarget)
{
    pThis->_uiNumRays    = pTarget.getWidth() * pTarget.getHeight();
    pThis->_uiCurrentRay = 0;

    pThis->_vRays    .resize(pThis->_uiNumRays);
    pThis->_vRayInfos.resize(pThis->_uiNumRays);

    pCam.fillRayStores(pThis->_vRays, 
                       pThis->_vRayInfos, 
                       pTarget);
}

template<typename DescT> inline
void RTPrimaryRayStoreSetupHelper<DescT, RTSIMDMathTag>::setupRays(
    RTPrimaryRayStore<DescT> *pThis,
    RTCameraDecorator        &pCam, 
    RTTarget                 &pTarget)
{
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

    pCam.fillRayStores(pThis->_vRays, 
                       pThis->_vRayInfos, 
                       pTarget,
                       pThis->_uiVTiles,
                       pThis->_uiHTiles);
}

#endif // OSG_CELL

OSG_END_NAMESPACE
