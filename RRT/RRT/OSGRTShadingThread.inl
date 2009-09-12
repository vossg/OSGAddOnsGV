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
RTShadingThread<DescT> *RTShadingThread<DescT>::find(OSG::Char8 *szName)
{
    OSG::BaseThread *pThread = OSG::ThreadManager::the()->findThread(szName);

    return dynamic_cast<RTShadingThread *>(pThread);
}

template<typename DescT> inline
void RTShadingThread<DescT>::setup(RTTarget   *pTarget,
                                   Scene      *pScene,
                                   HitStore   *pHitStore,
                                   Barrier    *pSyncBarrier,
                                   Int32       iID)
{
    _pTarget = pTarget;

    setRefd(_pScene,       pScene      );
    setRefd(_pHitStore,    pHitStore   );
    setRefd(_pSyncBarrier, pSyncBarrier);

    _iID      = iID;
}

template<typename DescT> inline
void RTShadingThread<DescT>::setup(RTTarget        *pTarget,
                                   Scene           *pScene,
                                   HitTiledStore   *pHitStore,
                                   Barrier         *pSyncBarrier,
                                   Int32            iID)
{
    _pTarget = pTarget;

    setRefd(_pScene,         pScene      );
    setRefd(_pHitTiledStore, pHitStore   );
    setRefd(_pSyncBarrier,   pSyncBarrier);

    _iID      = iID;
}

template<typename DescT> inline
void RTShadingThread<DescT>::setRunning(bool bVal)
{
    _bRunning = bVal;
}

template<typename DescT> inline
OSG::BaseThread *RTShadingThread<DescT>::create(const OSG::Char8  *szName, 
                                                      OSG::UInt32  uiId)
{
    return new RTShadingThread(szName, uiId);
}

template<typename DescT> inline
RTShadingThread<DescT>::RTShadingThread(const OSG::Char8  *szName, 
                                              OSG::UInt32  uiId  ) :
    Inherited     (szName, 
                   uiId  ),
   _pTarget       (NULL  ),
   _pScene        (NULL  ),
   _pHitStore     (NULL  ),
   _pHitTiledStore(NULL  ),
   _pSyncBarrier  (NULL  ),
   _iID           (-1    ),
   _bRunning      (false )
{
}

template<typename DescT> inline
RTShadingThread<DescT>::~RTShadingThread(void)
{
    _pTarget = NullFC;

    OSG::subRef(_pScene        );
    OSG::subRef(_pHitStore     );
    OSG::subRef(_pHitTiledStore);
    OSG::subRef(_pSyncBarrier  );
}

template<typename DescT> inline
void RTShadingThread<DescT>::workProc(void)
{
    _pSyncBarrier->enter();

    ColorPacket oColor;

    if(_pHitStore != NULL)
    {
        _bRunning = true;

        while(true)
        {
            _pSyncBarrier->enter();
            
            if(_bRunning == false)
            {
                break;
            }

            UInt32 uiHitIndex = _pHitStore->getReadIndex();
            
            while(uiHitIndex != HitStore::Empty)
            {
                if(uiHitIndex == HitStore::Waiting)
                {
                    _pHitStore->wait();
                }
                else
                {
                    SingleHitPacket &oHitPacket = 
                        _pHitStore->getPacket(uiHitIndex);

                    SingleRayPacket *pRayPacket = 
                        oHitPacket.getRayPacket();
                   
                    _pScene->shade(oHitPacket, *pRayPacket, oColor);

                    _pTarget->setPixel(oHitPacket.getX(), 
                                       oHitPacket.getY(),
                                       oColor           );

#if 0
                    if(oHitPacket.getU() > -0.5)
                    {
                        _pTarget->markPixelHit(oHitPacket.getX(), 
                                               oHitPacket.getY());
                    }
                    else
                    {
                        _pTarget->markPixelNotHit(oHitPacket.getX(), 
                                                  oHitPacket.getY());
                    }
#endif
                }
                
                uiHitIndex = _pHitStore->getReadIndex();
            }
            
            _pHitStore->broadcast();
            
            _pSyncBarrier->enter();
        }
    }
    else
    {
        _bRunning = true;

        while(true)
        {
            _pSyncBarrier->enter();
            
            if(_bRunning == false)
            {
                break;
            }

            UInt32 uiHitIndex = _pHitTiledStore->getReadIndex();
            
            while(uiHitIndex != HitTiledStore::Empty)
            {
                if(uiHitIndex == HitTiledStore::Waiting)
                {
                    _pHitTiledStore->wait();
                }
                else
                {
                    HitTile &oHitTile = 
                        _pHitTiledStore->getPacket(uiHitIndex);
                    
                    for(UInt32 i = 0; i < HitTile::NumVPackets; ++i)
                    {
                        for(UInt32 j = 0; 
                                   j < HitTile::NumHPackets; 
                                 ++j)
                        {
                            UInt32 uiPacketIndex = 
                                i * HitTile::NumHPackets + j;

                            HitPacket &oHitPacket = 
                                oHitTile.getPacket(uiPacketIndex);
                            
                            RayPacket &pRayPacket = 
                                oHitTile.getRayPacket(uiPacketIndex);

                            if(oHitTile.hasActive(uiPacketIndex) == false)
                                continue;

                            UInt32 uiX = 
                                oHitTile.getX() * HitTile::NumHPackets + j;

                            UInt32 uiY =
                                oHitTile.getY() * HitTile::NumVPackets + i;

                            _pScene->shade(oHitPacket, pRayPacket, oColor);

                            _pTarget->setPixel(uiX, uiY, oColor);

#if 0
                            if(oHitPacket.getU() > -0.5)
                            {
                                _pTarget->markPixelHit(uiX, uiY);
                            }
                            else
                            {
                                _pTarget->markPixelNotHit(uiX, uiY);
                            }
#endif
                        }
                    }
                }
                
                uiHitIndex = _pHitTiledStore->getReadIndex();
            }
            
            _pHitTiledStore->broadcast();
            
            _pSyncBarrier->enter();
        }
    }
}

OSG_END_NAMESPACE
