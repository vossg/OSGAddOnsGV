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
    RTCombinedThreadHelper<DescT, MathTag>::workProcHelper(this, *_pTarget);
}

template<class DescT> inline
void RTCombinedThreadHelper<DescT, RTFloatMathTag>::workProcHelper(
    RTCombinedThread<DescT> *pThis,
    RTTarget                &oTarget)
{
    OSG_ASSERT(false);
    // for SingleRay on PowerPC use the same code as below
}

template<class DescT> inline
void RTCombinedThreadHelper<DescT, RTSIMDMathTag>::workProcHelper(
    RTCombinedThread<DescT> *pThis,
    RTTarget                &oTarget)
{
    pThis->_pSyncBarrier->enter();

    UInt32 uiRayStat;
    UInt32 uiHitStat;

    typename RTCache::ElemStack sKDToDoStack;

    sKDToDoStack.reserve(256);

    ColorPacket oColor;

    if(pThis->_pRayStore != NULL)
    {
        while(true)
        {
            pThis->_pSyncBarrier->enter();
            
            UInt32 uiRayIndex = pThis->_pRayStore->nextIndex();

            uiRayStat = 0;
            uiHitStat = 0;
            
            while(uiRayIndex != PrimaryRayStore::Empty) 
            {
                UInt32               uiHitIndex = 
                    pThis->_pHitStore->getWriteIndex();
                
                SingleRayPacket     &oRayPacket = 
                    pThis->_pRayStore->getRayPacket(uiRayIndex);

                SingleRayPacketInfo &oRayPacketInfo = 
                    pThis->_pRayStore->getRayPacketInfo(uiRayIndex);
                
                SingleHitPacket &oHitPacket = 
                    pThis->_pHitStore->getPacket   (uiHitIndex);

                oHitPacket.reset();

                oHitPacket.setXY       (oRayPacketInfo.getX(), 
                                        oRayPacketInfo.getY());

                oHitPacket.setRayPacket(&oRayPacket);

                pThis->_pScene->tracePrimaryRays(
                    oRayPacket, 
                    oHitPacket, 
                    sKDToDoStack,
                    oRayPacketInfo.getActiveRays());

                pThis->_pHitStore->pushWriteIndex(uiHitIndex);

                ++uiRayStat;
                
                uiRayIndex = pThis->_pRayStore->nextIndex();
            }
            
            UInt32 uiHitIndex = pThis->_pHitStore->getReadIndex();

#if 0  
            // Prints a diagonal set of raytiles to console or raytiles in
            // leftmost column of stage
            //for(UInt32 i = 0; i < pThis->_pRayStore->getNumTiles() ; 
            // i=i+pThis->_pRayStore->getNumHTiles()+1)

            for(UInt32 i = 0; 
                       i < pThis->_pRayStore->getNumTiles() ; 
                       i = i + pThis->_pRayStore->getNumHTiles())
            {
                SingleRayPacket &oRayTile = pThis->_pRayStore->getRayPacket(i);
                union
                {
                    Float4 dir;
                    Real32 arrayDir[4];
                };        

                dir = oRayTile.getDir(0);

                printf("%d\n", i);
                printf("x: %f %f %f %f\n", 
                       arrayDir[0], arrayDir[1], arrayDir[2], arrayDir[3]);

                dir = oRayTile.getDir(1);

                printf("y: %f %f %f %f\n", 
                       arrayDir[0], arrayDir[1], arrayDir[2], arrayDir[3]);

                dir = oRayTile.getDir(2);

                printf("z: %f %f %f %f\n", 
                       arrayDir[0], arrayDir[1], arrayDir[2], arrayDir[3]);
            }

            exit(-1);
#endif


#if 0   // Write binary to file

            FILE* fp;
            fp=fopen(
                "/home/filip/tmp/OpenSG/Standalone.app/HitPacketsPPU_box.bin", 
                "wb+");

            UInt32 hits = 0;

            for(UInt32 i = 0; i < 4096 ; ++i)
            {
                SingleHitPacket     &oHitPacket = 
                    pThis->_pHitStore->getPacket(i);
    
                Real32 rDist[4], rU[4], rV[4];
                UInt32 objId[4], triId[4], cacheId[4];

                for(UInt32 j = 0 ; j < 4 ; ++j)
                {
                    rDist[j] = oHitPacket.getDist(j);
                    rU[j] = oHitPacket.getU(j);
                    rV[j] = oHitPacket.getV(j);
                    objId[j] = oHitPacket.getObjId(j);
                    triId[j] = oHitPacket.getTriId(j);
                    cacheId[j] = oHitPacket.getCacheId(j); 

                    if(rDist[j] < FLT_MAX)
                        hits++;
                }
          
                fwrite(&rDist, 4, 4, fp);
                fwrite(&rU, 4, 4, fp);
                fwrite(&rV, 4, 4, fp);
                fwrite(&objId, 4, 4, fp);
                fwrite(&triId, 4, 4, fp);
                fwrite(&cacheId, 4, 4, fp);
            }
        
            fclose(fp);

            printf("Hits %u\n", hits);
            printf("Binary hitdata written to file\n");
            exit(-1);
#endif

            while(uiHitIndex != HitStore::Empty)
            {
                if(uiHitIndex != HitStore::Waiting)
                {
                    SingleHitPacket &oHitPacket = 
                        pThis->_pHitStore->getPacket(uiHitIndex);

                    SingleRayPacket *pRayPacket = 
                        oHitPacket.getRayPacket();

                    pThis->_pScene->shade(oHitPacket, *pRayPacket, oColor);

                    pThis->_pTarget->setPixel(oHitPacket.getX(), 
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

                uiHitIndex = pThis->_pHitStore->getReadIndex();
            }
            
#if 0
            fprintf(stderr, "%p : %d %d\n", this, uiRayStat, uiHitStat);
#endif

            pThis->_pSyncBarrier->enter();
        }
    }
    else
    {
        while(true)
        {
#if 0
            fprintf(stderr, "Frame start\n");
#endif

            pThis->_pSyncBarrier->enter();
            
            UInt32 uiTileIndex = pThis->_pRayTiledStore->nextIndex();

            uiRayStat = 0;
            uiHitStat = 0;
            
            while(uiTileIndex != PrimaryRayTiledStore::Empty) 
            {
                UInt32             uiHitIndex = 
                    pThis->_pHitTiledStore->getWriteIndex();
                
                PrimaryRayTile &oRayTile = 
                    pThis->_pRayTiledStore->getRayPacket(uiTileIndex);

                HitTile        &oHitTile = 
                    pThis->_pHitTiledStore->getPacket   (uiHitIndex);

                oHitTile.reset     (                     );
                oHitTile.setRayTile(&oRayTile            );
                oHitTile.setXY     ( oRayTile.getX     (), 
                                     oRayTile.getY     ());

                oHitTile.setActive ( oRayTile.getActive());

#if 1
                for(UInt32 i = 0; 
                           i < HitTile::NumVPackets * HitTile::NumHPackets;
                         ++i)
                {
                    RayPacket &oRayPacket = oRayTile.getPacket(i);
                    HitPacket &oHitPacket = oHitTile.getPacket(i);
                
                    if(oRayTile.hasActive(i) == false)
                        continue;

                    pThis->_pScene->tracePrimaryRays(oRayPacket, 
                                                     oHitPacket, 
                                                     sKDToDoStack,
                                                     oRayTile.getActiveRays());

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

                        if(oRayPacket.hasActive() == false)
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
                
                pThis->_pHitTiledStore->pushWriteIndex(uiHitIndex);
                
                uiTileIndex = pThis->_pRayTiledStore->nextIndex();
            }
            
            
            UInt32 uiHitIndex = pThis->_pHitTiledStore->getReadIndex();
            
            while(uiHitIndex != HitTiledStore::Empty)
            {
                if(uiHitIndex != HitStore::Waiting)
                {
                    HitTile &oHitTile = 
                        pThis->_pHitTiledStore->getPacket(uiHitIndex);

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

                            if(oHitTile.hasActive(uiPacketIndex) == false)
                                continue;
                            
                            UInt32 uiX = 
                                oHitTile.getX() * HitTile::NumHPackets + j;

                            UInt32 uiY =
                                oHitTile.getY() * HitTile::NumVPackets + i;
                                
                            pThis->_pScene->shade(oHitPacket, pRayPacket, oColor);
                                
                            pThis->_pTarget->setPixel(uiX, uiY, oColor);

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

                uiHitIndex = pThis->_pHitTiledStore->getReadIndex();
            }

#if 0
            fprintf(stderr, "%p : %d %d\n", this, uiRayStat, uiHitStat);
#endif            
            pThis->_pSyncBarrier->enter();
        }
    }
}

OSG_END_NAMESPACE
