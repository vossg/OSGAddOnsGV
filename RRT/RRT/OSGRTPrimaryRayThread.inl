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
RTPrimaryRayThread<DescT> *RTPrimaryRayThread<DescT>::find(OSG::Char8 *szName)
{
    OSG::BaseThread *pThread = OSG::ThreadManager::the()->findThread(szName);

    return dynamic_cast<RTPrimaryRayThread *>(pThread);
}

template<typename DescT> inline
OSG::BaseThread *RTPrimaryRayThread<DescT>::create(const OSG::Char8  *szName, 
                                                         OSG::UInt32  uiId)
{
    return new RTPrimaryRayThread(szName, uiId);
}

template<typename DescT> inline
void RTPrimaryRayThread<DescT>::setup(Scene             *pScene,
                                      PrimaryRayStore   *pRayStore, 
                                      HitStore          *pHitStore,
                                      Barrier           *pBarrier,
                                      Int32              iID )
{
    OSG::setRefd(_pScene,       pScene   );
    OSG::setRefd(_pRayStore,    pRayStore);
    OSG::setRefd(_pHitStore,    pHitStore);
    OSG::setRefd(_pSyncBarrier, pBarrier );

    _iID      = iID;
}

template<typename DescT> inline
void RTPrimaryRayThread<DescT>::setup(Scene                  *pScene,
                                      PrimaryRayTiledStore   *pRayStore, 
                                      HitTiledStore          *pHitStore,
                                      Barrier                *pBarrier,
                                      Int32                   iID )
{
    OSG::setRefd(_pScene,         pScene   );
    OSG::setRefd(_pRayTiledStore, pRayStore);
    OSG::setRefd(_pHitTiledStore, pHitStore);
    OSG::setRefd(_pSyncBarrier,   pBarrier );

    _iID      = iID;
}

template<typename DescT> inline
void RTPrimaryRayThread<DescT>::setRunning(bool bVal)
{
    _bRunning = bVal;
}

template<typename DescT> inline
RTPrimaryRayThread<DescT>::RTPrimaryRayThread(const OSG::Char8  *szName, 
                                                    OSG::UInt32  uiId  ) :
     Inherited     (szName, 
                    uiId  ),
    _pScene        (NULL  ),
    _pRayStore     (NULL  ),
    _pHitStore     (NULL  ),
    _pRayTiledStore(NULL  ),
    _pHitTiledStore(NULL  ),
    _pSyncBarrier  (NULL  ),
    _iID           (   -1 ),
    _bRunning      (false )
{
}

template<typename DescT> inline
RTPrimaryRayThread<DescT>::~RTPrimaryRayThread(void)
{
    OSG::subRef(_pScene        );
    OSG::subRef(_pRayStore     );
    OSG::subRef(_pHitStore     );
    OSG::subRef(_pRayTiledStore);
    OSG::subRef(_pHitTiledStore);
    OSG::subRef(_pSyncBarrier  );
}


template<typename DescT> inline
void RTPrimaryRayThread<DescT>::workProc(void)
{
    _pSyncBarrier->enter();

    typename RTCache::ElemStack sKDToDoStack;

    sKDToDoStack.reserve(256);

    if(_pRayStore != NULL)
    {
        _bRunning = true;

        while(true)
        {
            _pSyncBarrier->enter();
            
            if(_bRunning == false)
            {
                break;
            }

            UInt32 uiRayIndex = _pRayStore->nextIndex();
            
            while(uiRayIndex != PrimaryRayStore::Empty)
            {
                UInt32             uiHitIndex = _pHitStore->getWriteIndex();
                
                SingleRayPacket     &oRayPacket = 
                    _pRayStore->getRayPacket(uiRayIndex);

                SingleRayPacketInfo &oRayPacketInfo = 
                    _pRayStore->getRayPacketInfo(uiRayIndex);
                
                SingleHitPacket &oHitPacket = 
                    _pHitStore->getPacket   (uiHitIndex);
                
                oHitPacket.reset();
                oHitPacket.setXY(oRayPacketInfo.getX(), 
                                 oRayPacketInfo.getY());
                
                oHitPacket.setRayPacket(&oRayPacket);

                _pScene->tracePrimaryRays(oRayPacket, 
                                          oHitPacket, 
                                          sKDToDoStack,
                                          oRayPacketInfo.getActiveRays());
                
                _pHitStore->pushWriteIndex(uiHitIndex);
                
                uiRayIndex = _pRayStore->nextIndex();
            }
            
            
            _pHitStore->signal();
            
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

            UInt32 uiTileIndex = _pRayTiledStore->nextIndex();
            
            while(uiTileIndex != PrimaryRayTiledStore::Empty)
            {
                UInt32             uiHitIndex = 
                    _pHitTiledStore->getWriteIndex();
                
                PrimaryRayTile &oRayTile = 
                    _pRayTiledStore->getRayPacket(uiTileIndex);

                
                HitTile        &oHitTile = 
                    _pHitTiledStore->getPacket   (uiHitIndex);
                
                oHitTile.reset();

                oHitTile.setRayTile(&oRayTile);
                oHitTile.setXY     ( oRayTile.getX(), 
                                     oRayTile.getY()     );

                oHitTile.setActive ( oRayTile.getActive());

                for(UInt32 i = 0; 
                           i < HitTile::NumVPackets * HitTile::NumHPackets;
                         ++i)
                {
                    RayPacket &oRayPacket = oRayTile.getPacket(i);
                    HitPacket &oHitPacket = oHitTile.getPacket(i);

                    if(oRayTile.hasActive(i) == false)
                        continue;

                    _pScene->tracePrimaryRays(oRayPacket, 
                                              oHitPacket, 
                                              sKDToDoStack,
                                              oRayTile.getActiveRays());
                }

                _pHitTiledStore->pushWriteIndex(uiHitIndex);
                
                uiTileIndex = _pRayTiledStore->nextIndex();
            }
            
            
            _pHitTiledStore->signal();
            
            _pSyncBarrier->enter();
        }
    }
}

OSG_END_NAMESPACE
