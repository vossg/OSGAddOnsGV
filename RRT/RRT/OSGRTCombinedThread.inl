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
RTCombinedThread<DescT> *RTCombinedThread<DescT>::find(OSG::Char8 *szName)
{
    OSG::BaseThread *pThread = OSG::ThreadManager::the()->findThread(szName);

    return dynamic_cast<RTCombinedThread *>(pThread);
}

template<class DescT> inline
void RTCombinedThread<DescT>::setup(Scene             *pScene,
                                    RTTarget          *pTarget,
                                    PrimaryRayStore   *pRayStore, 
                                    HitStore          *pHitStore, 
                                    Barrier           *pSyncBarrier,
                                    Int32              iID )
{
    OSG::setRefd(_pScene,       pScene      );
    OSG::setRefd(_pTarget,      pTarget     );
    OSG::setRefd(_pRayStore,    pRayStore   );
    OSG::setRefd(_pHitStore,    pHitStore   );
    OSG::setRefd(_pSyncBarrier, pSyncBarrier);

    _iID      = iID;
}

template<class DescT> inline
void RTCombinedThread<DescT>::setup(Scene                  *pScene,
                                    RTTarget               *pTarget,
                                    PrimaryRayTiledStore   *pRayStore, 
                                    HitTiledStore          *pHitStore, 
                                    Barrier                *pSyncBarrier,
                                    Int32                   iID )
{
    OSG::setRefd(_pScene,            pScene      );
    OSG::setRefd(_pTarget,           pTarget     );
    OSG::setRefd(_pRayTiledStore,    pRayStore   );
    OSG::setRefd(_pHitTiledStore,    pHitStore   );
    OSG::setRefd(_pSyncBarrier,      pSyncBarrier);

    _iID      = iID;
}

template<class DescT> inline
OSG::BaseThread *RTCombinedThread<DescT>::create(const OSG::Char8  *szName, 
                                                       OSG::UInt32  uiId)
{
    return new RTCombinedThread(szName, uiId);
}

template<class DescT> inline
RTCombinedThread<DescT>::RTCombinedThread(const OSG::Char8  *szName, 
                                                OSG::UInt32  uiId  ) :
    Inherited      (szName, 
                    uiId  ),
    _pScene        (NULL  ),
    _pTarget       (NULL  ),
    _pRayStore     (NULL  ),
    _pHitStore     (NULL  ),
    _pRayTiledStore(NULL  ),
    _pHitTiledStore(NULL  ),
    _pSyncBarrier  (NULL  ),
    _iID           (  -1  )
{
}

template<class DescT> inline
RTCombinedThread<DescT>::~RTCombinedThread(void)
{
    OSG::subRef(_pScene        );
    OSG::subRef(_pTarget       );
    OSG::subRef(_pRayStore     );
    OSG::subRef(_pHitStore     );
    OSG::subRef(_pRayTiledStore);
    OSG::subRef(_pHitTiledStore);
    OSG::subRef(_pSyncBarrier  );
}


template<class DescT> inline
void RTCombinedThread<DescT>::workProc(void)
{
    _pSyncBarrier->enter();

    UInt32 uiRayStat;
    UInt32 uiHitStat;

    typename RTCache::ElemStack sKDToDoStack;

    sKDToDoStack.reserve(256);

    Color4f oColor;

    if(_pRayStore != NULL)
    {
        while(true)
        {
            fprintf(stderr, "Frame start\n");

            _pSyncBarrier->enter();
            
            UInt32 uiRayIndex = _pRayStore->nextIndex();

            uiRayStat = 0;
            uiHitStat = 0;
            
            while(uiRayIndex != PrimaryRayStore::Empty) 
            {
                UInt32             uiHitIndex = _pHitStore->getWriteIndex();
                
                SingleRayPacket &oRayPacket = 
                    _pRayStore->getRayPacket(uiRayIndex);
                
                SingleHitPacket &oHitPacket = 
                    _pHitStore->getPacket   (uiHitIndex);
                
                oHitPacket.reset();

                oHitPacket.setXY       (oRayPacket.getX(), oRayPacket.getY());
                oHitPacket.setRayPacket(&oRayPacket);

                _pScene->tracePrimaryRays(oRayPacket, 
                                          oHitPacket, 
                                          sKDToDoStack);
                
                _pHitStore->pushWriteIndex(uiHitIndex);

                ++uiRayStat;
                
                uiRayIndex = _pRayStore->nextIndex();
            }
            
            UInt32 uiHitIndex = _pHitStore->getReadIndex();
            
            while(uiHitIndex != HitStore::Empty)
            {
                if(uiHitIndex != HitStore::Waiting)
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

                    ++uiHitStat;
                }
                else
                {
                    osgSleep(1);
                }

                uiHitIndex = _pHitStore->getReadIndex();
            }
            
            fprintf(stderr, "%p : %d %d\n", this, uiRayStat, uiHitStat);

            _pSyncBarrier->enter();
        }
    }
    else
    {
        while(true)
        {
#if 0
            fprintf(stderr, "Frame start\n");
#endif

            _pSyncBarrier->enter();
            
            UInt32 uiTileIndex = _pRayTiledStore->nextIndex();

            uiRayStat = 0;
            uiHitStat = 0;
            
            while(uiTileIndex != PrimaryRayTiledStore::Empty) 
            {
                UInt32             uiHitIndex = 
                    _pHitTiledStore->getWriteIndex();
                
                PrimaryRayTile &oRayTile = 
                    _pRayTiledStore->getRayPacket(uiTileIndex);

                HitTile        &oHitTile = 
                    _pHitTiledStore->getPacket   (uiHitIndex);

                oHitTile.reset     (                );
                oHitTile.setRayTile(&oRayTile       );
                oHitTile.setXY     ( oRayTile.getX(), 
                                     oRayTile.getY());

#if 1
                for(UInt32 i = 0; 
                           i < HitTile::NumVPackets * HitTile::NumHPackets;
                         ++i)
                {
                    RayPacket &oRayPacket = oRayTile.getPacket(i);
                    HitPacket &oHitPacket = oHitTile.getPacket(i);
                
                    if(oRayPacket.isActive() == false)
                        continue;

                    _pScene->tracePrimaryRays(oRayPacket, 
                                              oHitPacket, 
                                              sKDToDoStack);

                    ++uiRayStat;
                }
#else
                for(UInt32 i = 0; i < HitTile::NumVPackets; ++i)
                {
                    for(UInt32 j = 0; 
                               j < HitTile::NumHPackets; 
                             ++j)
                    {
                        UInt32 uiPacketIndex = 
                            i * HitTile::NumHPackets + j;

                        SingleRayPacket &oRayPacket = 
                            oRayTile.getPacket(uiPacketIndex);

                        HitPacket       &oHitPacket = 
                            oHitTile.getPacket(uiPacketIndex);
                        
                        oHitPacket.reset();
                        
                        oHitPacket.setRayPacket(&oRayPacket);

                        if(oRayPacket.isActive() == false)
                            continue;

                        UInt32 uiX = 
                            oHitTile.getX() * HitTile::NumHPackets + j;

                        UInt32 uiY =
                            oHitTile.getY() * HitTile::NumVPackets + i;
                        
                        if(uiX >= _pTarget->getWidth() ||
                           uiY >= _pTarget->getHeight())
                        {
                            fprintf(stderr, "%d %d\n", uiX, uiY);

                            OSG_ASSERT(false);
                        }

                        _pScene->tracePrimaryRays(oRayPacket, 
                                                  oHitPacket, 
                                                  sKDToDoStack);

                        ++uiRayStat;
                    }
                }
#endif
                
                _pHitTiledStore->pushWriteIndex(uiHitIndex);
                
                uiTileIndex = _pRayTiledStore->nextIndex();
            }
            
            
            UInt32 uiHitIndex = _pHitTiledStore->getReadIndex();
            
            while(uiHitIndex != HitTiledStore::Empty)
            {
                if(uiHitIndex != HitStore::Waiting)
                {
                    HitTile &oHitTile = 
                        _pHitTiledStore->getPacket(uiHitIndex);

                    for(UInt32 i = 0; i < HitTile::NumVPackets; ++i)
                    {
                        for(UInt32 j = 0; j < HitTile::NumHPackets; ++j)
                        {
                            UInt32 uiPacketIndex = 
                                i * HitTile::NumHPackets + j;

                            HitPacket &oHitPacket = 
                                oHitTile.getPacket(uiPacketIndex);

                            RayPacket &pRayPacket = 
                                oHitTile.getRayPacket(uiPacketIndex);

                            if(pRayPacket.isActive() == false)
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
                           
                            ++uiHitStat;
                        }
                    }
                }
                else
                {
                    osgSleep(1);
                }

                uiHitIndex = _pHitTiledStore->getReadIndex();
            }

#if 0
            fprintf(stderr, "%p : %d %d\n", this, uiRayStat, uiHitStat);
#endif            
            _pSyncBarrier->enter();
        }
    }
}

OSG_END_NAMESPACE
