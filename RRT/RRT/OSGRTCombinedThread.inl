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

#ifdef OSG_CELL

void *ppu_pthread_function(void *arg) 
{
    ppu_pthread_data_t *datap = (ppu_pthread_data_t *)arg;
    int rc;
    unsigned int entry = SPE_DEFAULT_ENTRY;

    if((rc = spe_context_run(datap->speid, 
                             &entry, 
                             0, 
                             datap->argp, 
                             NULL, 
                             NULL)) < 0) 
    {
        fprintf (stderr, 
                 "Failed spe_context_run(rc=%d, errno=%d, strerror=%s)\n", 
                 rc, 
                 errno, 
                 strerror(errno));
        exit (1);
    }

    pthread_exit(NULL);
}

template<class DescT> inline
void RTCombinedThreadHelper<DescT, RTFloatMathTag>::workProcHelper(
    RTCombinedThread<DescT> *pThis,
    RTTarget                &oTarget)
{
    OSG_ASSERT(false);
}

template<class DescT> inline
void RTCombinedThreadHelper<DescT, RTSIMDMathTag>::workProcHelper(
    RTCombinedThread<DescT> *pThis,
    RTTarget                &oTarget)
{
    pThis->_pSyncBarrier->enter(); 

    if(pThis->_pRayStore != NULL)
    {
        UInt32 uiFrame=1;

        while(true)
        {
            UInt16 rc;
            for (UInt8 i = 0; i < SPE_THREADS; i++) 
            {
                datas[i].speid = spe_context_create (0, NULL);
                datas[i].argp  = (UInt64 *) &cb[i];

                rc = spe_program_load (datas[i].speid, &rrt_spu);

                rc = pthread_create (&datas[i].pthread, 
                                     NULL, 
                                     &ppu_pthread_function, 
                                     &datas[i]);
            }
            
            if(SPE_THREADS > spe_cpu_info_get(SPE_COUNT_PHYSICAL_SPES, -1)) 
            {
                fprintf(stderr, "System doesn't have %d working SPE(s).\n", 
                        SPE_THREADS);

                exit (-1);
            }

            // printf("SPE Threads %u\n", SPE_THREADS);  

            pThis->_pSyncBarrier->enter();

            printf("frame %u start\n", uiFrame);

            PerspectiveCamera *pPCam = 
                dynamic_cast<PerspectiveCamera *>(pThis->_pScene->getCamera());

            const UInt32 uiTargetWidth  = oTarget.getWidth ();
            const UInt32 uiTargetHeight = oTarget.getHeight();

            const UInt32 uiNumVEnvelopes = 
                pThis->_pRayStore->getNumVTiles() / envelopeTilesY;

            const UInt32 uiNumHEnvelopes = 
                pThis->_pRayStore->getNumHTiles() / envelopeTilesX;

            const UInt32 uiEnvelopes = uiNumVEnvelopes*uiNumHEnvelopes;
            const UInt32 uiNumCaches = pThis->_pScene->getNumCaches();

            Matrix mCam;

            pPCam->getBeacon()->getToWorld(mCam);

            Real32 rVOff = atan(pPCam->getFov() / 2.f);
            Real32 rHOff = (rVOff * uiTargetHeight) / uiTargetWidth;

            raySetupBase_t raySetupBase;

            raySetupBase.vRight  = (vec_float4){mCam[0][0], 
                                                mCam[0][1], 
                                                mCam[0][2], 
                                                0.0};
            raySetupBase.vUp     = (vec_float4){mCam[1][0], 
                                                mCam[1][1], 
                                                mCam[1][2], 
                                                0.0};

            Vec3f vDir(0.f, 0.f, -1.f);

            mCam.mult(vDir);

            raySetupBase.vOrigin = (vec_float4){mCam[3][0], 
                                                mCam[3][1], 
                                                mCam[3][2], 
                                                0.0};

            raySetupBase.vMisc   = (vec_float4){rVOff, rVOff, rHOff, rHOff};
     
            rayEnvelope_t *rayEnvelope = (rayEnvelope_t*)
                _malloc_align(sizeof(rayEnvelope_t)*uiEnvelopes, 7);

            envelopeInfo_t *envelopeInfo = (envelopeInfo_t*)
                _malloc_align(sizeof(envelopeInfo_t)*uiEnvelopes,7);

            rayInfoEnvelope_t *rayInfoEnvelope = (rayInfoEnvelope_t*)
                _malloc_align(sizeof(rayInfoEnvelope_t)*uiEnvelopes,7);

            hitEnvelope_t *hitEnvelope = (hitEnvelope_t*)
                _malloc_align(sizeof(hitEnvelope_t)*uiEnvelopes,7);

            boxVolume_t *rootBoxVolume = (boxVolume_t*)
                _malloc_align(sizeof(boxVolume_t)*uiNumCaches, 5);

            cacheInfoPack_t *cacheInfoPack = (cacheInfoPack_t*)
                _malloc_align(sizeof(cacheInfoPack_t)*uiNumCaches, 5);


#ifdef SINGLEFRAME

            printf("===================\n");
            printf("PPU: %d bytes, %d  Ray Envelopes\n", 
                   sizeof(rayEnvelope_t)*uiEnvelopes,uiEnvelopes);
            printf("PPU: %d bytes, %d  Ray Info Envelopes\n", 
                   sizeof(rayInfoEnvelope_t)*uiEnvelopes, uiEnvelopes);
            printf("PPU: %d bytes %d Envelope Info structs\n", 
                   sizeof(envelopeInfo_t)*uiEnvelopes, uiEnvelopes);
            printf("PPU: %d bytes, %d  Hit Envelopes\n", 
                   sizeof(hitEnvelope_t)*uiEnvelopes, uiEnvelopes);
            printf("PPU: %d bytes, %d CacheInfoPack\n", 
                   sizeof(cacheInfoPack_t)*uiNumCaches, uiNumCaches);

#endif

            UInt32 envelopesSPE[SPE_THREADS];
            UInt32 uiSplit = (UInt32) (uiEnvelopes / SPE_THREADS)+1;

            UInt32 uiEnvUnAssigned = uiEnvelopes;

            for (int i = 0; i < SPE_THREADS; i++) 
            {
                if(uiSplit >= uiEnvUnAssigned)
                {
                    envelopesSPE[i] = uiEnvUnAssigned;
                }
                else
                {
                    envelopesSPE[i]= uiSplit;
                }

                uiEnvUnAssigned -= envelopesSPE[i];
            }

            UInt32 uiX = 0;
            UInt32 uiY = 0;
            UInt32 e = 0;

            for(UInt32 spe = 0 ; spe < SPE_THREADS ; ++spe)
            {
                for(UInt32 i = 0 ; i < envelopesSPE[spe]; ++i)
                {
                    envelopeInfo[e].id = e;

                    if(uiX >= uiTargetWidth)
                    {
                        uiX = 0;
                        uiY = uiY + envelopeTilesY * FourRayPacket::NumVRays;
                    }

                    assert(uiX < uiTargetWidth);
                    assert(uiY < uiTargetHeight);

                    envelopeInfo[e].topX = uiX;
                    envelopeInfo[e].topY = uiY;

                    uiX = uiX + envelopeTilesX * FourRayPacket::NumHRays;

                    e++;
                }
            }

            e=0;

            for (int i = 0; i < SPE_THREADS; i++) 
            {
                cb[i].speId              = i;
                cb[i].uiTargetWidth      = uiTargetWidth;
                cb[i].uiTargetHeight     = uiTargetHeight;
                cb[i].eaPrimRaySetupPack = (EA_t) &raySetupBase;
                cb[i].uiEnvelopes        = envelopesSPE[i];
                cb[i].uiCaches           = uiNumCaches;
                cb[i].eaCacheBoxVolume   = (EA_t) &rootBoxVolume[0];
                cb[i].eaCacheInfoPack    = (EA_t) &cacheInfoPack[0];

                cb[i].eaEnvelopeInfo     = (EA_t) &envelopeInfo[e];
                cb[i].eaRayEnvelope      = (EA_t) &rayEnvelope[e];
                cb[i].eaRayInfoEnvelope  = (EA_t) &rayInfoEnvelope[e];
                cb[i].eaHitEnvelope      = (EA_t) &hitEnvelope[e];
                cb[i].eaRayBase          = (EA_t) &rayEnvelope[0];
                cb[i].eaRayInfoBase      = (EA_t) &rayInfoEnvelope[0];
                cb[i].eaHitBase          = (EA_t) &hitEnvelope[0];

                e += envelopesSPE[i];
            }

            // Send signal to all SPE's to start
            for (UInt8 i=0; i<SPE_THREADS; ++i)  
            {
                spe_in_mbox_write(datas[i].speid, 
                                  (UInt32*)&cb[i].speId, 
                                  1, 
                                  SPE_MBOX_ALL_BLOCKING);
            }
    
            UInt32         *primIndexArray;
            triangleData_t *triangleArray;

#ifdef OSG_XCACHEKD

            UInt32 uiTotNodes     = 0;
            UInt32 uiTotTriangles = 0;
            UInt32 uiTotLeafs     = 0;
            UInt32 uiNodesInCache = 0;

            const std::vector<RTCache *> &vRTCaches = 
                pThis->_pScene->getCacheArray();

            for(int i =0 ; i < uiNumCaches ; ++i)
            {
                if(vRTCaches.at(i)->getNumNodes() == 1)
                    cacheInfoPack[i].uiNodes == 2;
                else
                    cacheInfoPack[i].uiNodes = 
                        vRTCaches.at(i)->getNumNodes();

                uiTotNodes += cacheInfoPack[i].uiNodes;

                cacheInfoPack[i].uiTriangles = 
                    vRTCaches.at(i)->getNumTriangles();

                uiTotTriangles += cacheInfoPack[i].uiTriangles;
         
                rootBoxVolume[i] = 
                    vRTCaches.at(i)->getRootBoxVolume();

         

                cacheInfoPack[i].eaKDNodeCache = (EA_t)
                    vRTCaches.at(i)->getXCacheKDPointer();

                cacheInfoPack[i].eaBoxCache    = (EA_t)
                    vRTCaches.at(i)->getBoxCachePointer();
            }

            // make it big enough
/*
            UInt32* primIndexArray = (UInt32*)
                _malloc_align(sizeof(UInt32)*
                              uiNumCaches*
                              uiTotNodes*maxPrimInNode,4);
*/

            UInt32 uiSizePrimIndexArray = 
                50+sizeof(UInt32)*maxPrimInNode*(uiTotNodes+1)/2;

            primIndexArray = (UInt32*)_malloc_align(uiSizePrimIndexArray, 4);

            triangleArray = (triangleData_t*)
                _malloc_align(sizeof(triangleData_t)*uiTotTriangles, 5);

#ifdef SINGLEFRAME

            printf("PPU: %d bytes, %d triangles\n",
                   sizeof(triangleData_t)*uiTotTriangles,uiTotTriangles);
            printf("PPU: %d bytes primIndexArray\n",uiSizePrimIndexArray);

#endif
      
            cacheInfoPack[0].eaPrimIndexArray = (EA_t)&primIndexArray[0];

            EA_t eaTriCache=0x0;

            UInt32 leafIdx = 0;

            for(int i =0 ; i < uiNumCaches ; ++i)
            {
                uiTotLeafs += cacheInfoPack[i].uiNodes/2;

                UInt32 prevCacheTri = 0;
                if(i>0)
                {
                    eaTriCache += (EA_t)
                        &triangleArray[cacheInfoPack[i-1].uiTriangles];

                    prevCacheTri = cacheInfoPack[i-1].uiTriangles;

                    cacheInfoPack[i].eaPrimIndexArray = (EA_t)
                        &primIndexArray[uiTotLeafs];
                }
                else
                {
                    eaTriCache += (EA_t)&triangleArray[0];
                }

                cacheInfoPack[i].eaTriCache = eaTriCache;

                const MFTriangleAccel &mfTriangleAcc = 
                    vRTCaches.at(i)->getTrianglAccel();
        
                TriangleAccel *tmpTri = (TriangleAccel *) &(mfTriangleAcc[0]);

//                    pThis->_pScene->_vRTCaches.at(i)->getTriangleCache();

                for(UInt32 t = 0 ; t < cacheInfoPack[i].uiTriangles ; ++t)
                {
                    triangleArray[i*prevCacheTri+t].nU      = tmpTri->_nU;
                    triangleArray[i*prevCacheTri+t].nV      = tmpTri->_nV;
                    triangleArray[i*prevCacheTri+t].nD      = tmpTri->_nD;
                    triangleArray[i*prevCacheTri+t].bNU     = tmpTri->_bNU;
                    triangleArray[i*prevCacheTri+t].bNV     = tmpTri->_bNV;
                    triangleArray[i*prevCacheTri+t].bD      = tmpTri->_bD; 
                    triangleArray[i*prevCacheTri+t].cNU     = tmpTri->_cNU;
                    triangleArray[i*prevCacheTri+t].cNV     = tmpTri->_cNV;
                    triangleArray[i*prevCacheTri+t].cD      = tmpTri->_cD;
                    triangleArray[i*prevCacheTri+t].uiProj  = tmpTri->_uiProj;
                    triangleArray[i*prevCacheTri+t].uiTriId = tmpTri->_uiTriId;
                    triangleArray[i*prevCacheTri+t].uiObjId = tmpTri->_uiObjId;
                    
                    tmpTri++;
                }
   
                UInt32 nArray = vRTCaches.at(i)->getPrimitiveListSize();

                for(UInt32 k = 0 ; k < nArray ; ++k)
                {
                    ++leafIdx;
                    
                    const std::vector<UInt32> &prims = 
                        vRTCaches.at(i)->getPrimitiveIndexList(k);

                    for(UInt32 l =0 ; l < prims.size() ; ++l)
                    {
                        primIndexArray[leafIdx*maxPrimInNode] = prims[l];
                    }
                }
            }
#endif

#ifndef OSG_XCACHEKD
            cacheKDNode_t* nodeArray;

            UInt32 uiNodesInCache[uiNumCaches];
            UInt32 uiTotNodes =0;
            UInt32 uiTotTriangles =0;

            const std::vector<RTCache *> &vRTCaches = 
                pThis->_pScene->getCacheArray();

            for(int i =0 ; i < uiNumCaches ; ++i)
            {
                uiNodesInCache[i] = vRTCaches.at(i)->getNumNodes();

                uiTotTriangles += vRTCaches.at(i)->getNumTriangles();
                
                if(uiNodesInCache[i] == 1)
                    uiNodesInCache[i] = 2;
                
                uiTotNodes += uiNodesInCache[i];
                
                rootBoxVolume[i] = vRTCaches.at(i)->getBoxVolume();
            }

            // make it big enough
/*
            UInt32* primIndexArray =(UInt32*)
                _malloc_align(sizeof(UInt32)*
                              uiNumCaches*
                              uiTotNodes*
                              maxPrimInNode,
                              4);
*/

            UInt32 uiSizePrimIndexArray = 
                50+sizeof(UInt32)*maxPrimInNode*(uiTotNodes+1)/2;

            primIndexArray =(UInt32*)_malloc_align(uiSizePrimIndexArray,4);
      
            nodeArray=(cacheKDNode_t*)
                _malloc_align(sizeof(cacheKDNode_t)*uiTotNodes,3);

            triangleArray=(triangleData_t*)
                _malloc_align(sizeof(triangleData_t)*uiTotTriangles, 5);
      
#ifdef SINGLEFRAME

            printf("PPU: %d bytes, %d nodes\n", 
                   sizeof(cacheKDNode_t)*uiTotNodes, uiTotNodes);
            printf("PPU: %d bytes, %d triangles\n",
                   sizeof(triangleData_t)*uiTotTriangles,uiTotTriangles);
            printf("PPU: %d bytes primIndexArray\n",uiSizePrimIndexArray);

#endif
     
            EA_t eaTriCache=0x0;

            UInt32 leafCount[uiNumCaches];
            UInt32 uiTotLeafs =0;

            for(int i =0 ; i < uiNumCaches ; ++i)
            {
                leafCount[i]=0;
                
                for(int j = 1 ; j < uiNodesInCache[i] ; ++j)
                {
                    CacheKDNode* tmpNode = vRTCaches.at(i)->getNode(j);
                    
                    nodeArray[j].uiFlags = tmpNode->_uiFlags;
                    nodeArray[j].uiAboveChild = tmpNode->_uiAboveChild;
                    
                    if(tmpNode->isLeaf())
                        leafCount[i]++;
                }
                
                cacheInfoPack[i].eaKDNodeCache = 
                    (UInt64)&nodeArray[i*maxNodesInCache];

                cacheInfoPack[i].uiNodes = 18 << leafCount[i];
                cacheInfoPack[i].uiNodes += uiNodesInCache[i];

                cacheInfoPack[i].uiTriangles = 
                    vRTCaches.at(i)->getNumTriangles();

                cacheInfoPack[i].eaPrimIndexArray = 
                    (UInt64)&primIndexArray[uiTotLeafs*maxPrimInNode];
         
                uiTotLeafs += leafCount[i];

                UInt32 prevCacheTri = 0;
                if(i>0)
                {
                    eaTriCache += 
                        (EA_t)&triangleArray[cacheInfoPack[i-1].uiTriangles];

                    prevCacheTri = cacheInfoPack[i-1].uiTriangles;
                }
                else
                {
                    eaTriCache += (EA_t)&triangleArray[0];
                }
          
                cacheInfoPack[i].eaTriCache = eaTriCache;
        
                const MFTriangleAccel &mfTriangleAcc = 
                    vRTCaches.at(i)->getTrianglAccel();

                //Prepare triangles for SPE's
                for(UInt32 t = 0 ; t < cacheInfoPack[i].uiTriangles ; ++t)
                {
                    const TriangleAccel &tmpTri = mfTriangleAcc[t];  
          
                    triangleArray[i*prevCacheTri+t].nU      = tmpTri._nU;
                    triangleArray[i*prevCacheTri+t].nV      = tmpTri._nV;
                    triangleArray[i*prevCacheTri+t].nD      = tmpTri._nD;
                    triangleArray[i*prevCacheTri+t].bNU     = tmpTri._bNU;
                    triangleArray[i*prevCacheTri+t].bNV     = tmpTri._bNV;
                    triangleArray[i*prevCacheTri+t].bD      = tmpTri._bD; 
                    triangleArray[i*prevCacheTri+t].cNU     = tmpTri._cNU;
                    triangleArray[i*prevCacheTri+t].cNV     = tmpTri._cNV;
                    triangleArray[i*prevCacheTri+t].cD      = tmpTri._cD;
                    triangleArray[i*prevCacheTri+t].uiProj  = tmpTri._uiProj;
                    triangleArray[i*prevCacheTri+t].uiTriId = tmpTri._uiTriId;
                    triangleArray[i*prevCacheTri+t].uiObjId = tmpTri._uiObjId;
                }
        
                UInt32 leafIdx = 0;
                UInt32 numIndexLists = vRTCaches.at(i)->getPrimitiveListSize();

                for(UInt32 k = 0 ; k < numIndexLists ; ++k)
                {
                    ++leafIdx;

                    const std::vector<UInt32> &prims = 
                        vRTCaches.at(i)->getPrimitiveIndexList(k);

                    for(UInt32 l =0 ; l < prims.size() ; ++l)
                    {
                        primIndexArray[leafIdx*maxPrimInNode] = prims[l];
                    }
                }
            }
    
      
#endif

            // Wait for SPE Ray Setup completion
            for (UInt8 i=0; i<SPE_THREADS; ++i) 
            { 
                unsigned int foo;                     

                while(spe_out_mbox_status(datas[i].speid) < 1);

                spe_out_mbox_read(datas[i].speid, &foo, 1);
            }

            _free_align(envelopeInfo);



            // Issue a sync, to be safe that all SPEs are done writing 
            // to main memory.

            __asm__ __volatile__ ("lwsync" : : : "memory");


            // Copy Rays to RTPrimaryRayStore
            //
            UInt16 eCurr;

            for(UInt16 eY = 0 ; eY < uiNumVEnvelopes ; eY++)
            {
                for(UInt16 eX = 0 ; eX < uiNumHEnvelopes ; eX++)
                {
                    eCurr = eY*uiNumVEnvelopes + eX;

                    for(UInt16 i = 0 ; i < envelopeTilesY ; i++)
                    {
                        for(UInt16 j = 0 ; j < envelopeTilesX ; j++)
                        {
                            pThis->_pRayStore->convCellStruct(
                                eY*
                                envelopeTilesY* 
                                pThis->_pRayStore->getNumHTiles()
                                + i *
                                pThis->_pRayStore->getNumHTiles()+
                                (j+eX*envelopeTilesX),
                                &rayEnvelope[eCurr].tile[i][j],
                                &rayInfoEnvelope[eCurr].tile[i][j]);
                        }
                    }
                } 
            }


#if 1    // Write binary to file
            FILE* fp;
            fp=fopen("/home/filip/tmp/OpenSG/Standalone.app/primaryraysfullDBSIMD1x1_cell.bin", "wb+");
      
            for(UInt32 i = 0; i < pThis->_pRayStore->getNumVTiles() ; ++i)
            {
                for(UInt32 j = 0; j < pThis->_pRayStore->getNumHTiles() ; ++j)
                {
                    UInt32 uiRayIndex = 
                        i * pThis->_pRayStore->getNumHTiles() + j;

                    FourRayPacket     &oRayTile = 
                        pThis->_pRayStore->getRayPacket(uiRayIndex);
    
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
#endif


#if 0 
            // Prints a diagonal set of raytiles to console or raytiles in
            //leftmost column of stage 
            //for(UInt32 i = 0; i < pThis->_pRayStore->getNumTiles() ;
            //i=i+pThis->_pRayStore->getNumHTiles()+1) 

            for(UInt32 i = 0; 
                       i < pThis->_pRayStore->getNumTiles() ; 
                       i=i+pThis->_pRayStore->getNumHTiles())
                
            {
                FourRayPacket &oRayTile = pThis->_pRayStore->getRayPacket(i);

                union
                {
                    Float4 dir;
                    Real32 arrayDir[4];
                };          

                dir = oRayTile.getDir(0);

                printf("%d\n", i);
                printf("x: %f %f %f %f\n", 
                       arrayDir[0], 
                       arrayDir[1], 
                       arrayDir[2], 
                       arrayDir[3]);

                dir = oRayTile.getDir(1);

                printf("y: %f %f %f %f\n", 
                       arrayDir[0], 
                       arrayDir[1], 
                       arrayDir[2], 
                       arrayDir[3]);

                dir = oRayTile.getDir(2);

                printf("z: %f %f %f %f\n", 
                       arrayDir[0], 
                       arrayDir[1], 
                       arrayDir[2], 
                       arrayDir[3]);
            }
    
          
#endif

#ifdef SINGLEFRAME
            fprintf(stderr, "Primary Ray setup completed!\n");
#endif
      
            for (UInt8 i=0; i<SPE_THREADS; ++i) 
            { // Send signal to all SPE's to start
                spe_in_mbox_write(datas[i].speid, 
                                  (UInt32*)&cb[i].speId, 
                                  1, 
                                  SPE_MBOX_ALL_BLOCKING);
            }


#ifdef DYNAMIC_ENV_ASSIGNMENT

            for(UInt32 c = 0 ; c < uiNumCaches ; ++c)
            {
                UInt32 uiCurrEnvelope = 0;
          
                while(uiCurrEnvelope < uiEnvelopes)
                {
                    for(UInt32 i=0; i<SPE_THREADS; ++i) 
                    { 
                        if(spe_in_mbox_status(datas[i].speid) == 4 && 
                           uiCurrEnvelope < uiEnvelopes)
                        {
                            spe_in_mbox_write(
                                datas[i].speid, 
                                (unsigned int *)(&uiCurrEnvelope), 
                                1, 
                                SPE_MBOX_ALL_BLOCKING);

                            uiCurrEnvelope += intersectBatchCount;
                        }
                    }
                }
                UInt32 syncedSPEs = 0;
                bool speSynced[SPE_THREADS];
                Int32 nextCache = -1;
          
                while(syncedSPEs < SPE_THREADS)
                {
                    for (UInt32 i=0; i<SPE_THREADS; ++i) 
                    { 
                        if(spe_in_mbox_status(datas[i].speid) == 4 && 
                           !speSynced[i]); // poll mailbox
                        {
                            spe_in_mbox_write(datas[i].speid,
                                              (unsigned int*)&(nextCache),
                                              1,
                                              SPE_MBOX_ALL_BLOCKING);
                            syncedSPEs++;
                            speSynced[i]= true;
                        }
                    }
                }
            }

#endif

            for (UInt32 i=0; i<SPE_THREADS; ++i) 
            { 
                unsigned int foo;                     
                while (spe_out_mbox_status(datas[i].speid) < 1);
                spe_out_mbox_read(datas[i].speid, &foo, 1);
            }

            // Issue a sync, to be safe that all SPEs are done writing to main memory.
            __asm__ __volatile__ ("lwsync" : : : "memory");

            for(UInt16 eY = 0 ; eY < uiNumVEnvelopes  ; eY++)
            {
                for(UInt16 eX = 0 ; eX < uiNumHEnvelopes ; eX++)
                {
                    for(UInt16 i = 0 ; i < envelopeTilesY ; i++)
                    {
                        for(UInt16 j = 0 ; j < envelopeTilesX ; j++)
                        {
                            UInt32  uiHitIndex = 
                                pThis->_pHitStore->getWriteIndex();

                            FourHitPacket &oHitPacket = 
                                pThis->_pHitStore->getPacket   (uiHitIndex);
              
                            UInt32 rayPackIndex =
                                eY*
                                envelopeTilesY* 
                                pThis->_pRayStore->getNumHTiles() + i *
                                pThis->_pRayStore->getNumHTiles()+
                                (j+eX*envelopeTilesX);

                            // oHitPacket.setRayPacket((FourRayPacket*)
                            //              hitEnvelope[eY*uiNumHEnvelopes+eX].tile[i][j].pRayPacket);

                            oHitPacket.setRayPacket((FourRayPacket*)&
                                pThis->_pRayStore->getRayPacket(rayPackIndex));
              
                            oHitPacket.setXY(
                                hitEnvelope[eY*
                                            uiNumHEnvelopes+
                                            eX].tile[i][j].uiX,

                                hitEnvelope[eY*
                                            uiNumHEnvelopes+
                                            eX].tile[i][j].uiY);


                            oHitPacket.setAll(
                                hitEnvelope[eY*
                                            uiNumHEnvelopes+
                                            eX].tile[i][j].rDist,
                                hitEnvelope[eY*
                                            uiNumHEnvelopes+
                                            eX].tile[i][j].rU,

                                hitEnvelope[eY*
                                            uiNumHEnvelopes+
                                            eX].tile[i][j].rV,

                                hitEnvelope[eY*
                                            uiNumHEnvelopes+
                                            eX].tile[i][j].uiObjId,

                                hitEnvelope[eY*
                                            uiNumHEnvelopes+
                                            eX].tile[i][j].uiTriId,

                                hitEnvelope[eY*
                                            uiNumHEnvelopes+
                                            eX].tile[i][j].uiCacheId);

                            // printf("hitindex %u\n", uiHitIndex);
                            
                            pThis->_pHitStore->pushWriteIndex(uiHitIndex);
                        }
                    }
                } 
            }

#if 0    // Write binary to file

            FILE* fp;
            fp=fopen("/home/filip/tmp/OpenSG/Standalone.app/HitPacketsCell_box.bin", "wb+");
      
            UInt32 hits = 0;
      
            for(UInt32 i = 0; i < 4096 ; ++i)
            {
                FourHitPacket     &oHitPacket = 
                    pThis->_pHitStore->getPacket(i);
    
                Real32 rDist[4], rU[4], rV[4];
                UInt32 objId[4], triId[4], cacheId[4];
        
                for(UInt32 i = 0 ; i < 4 ; ++i)
                {

                    rDist[i] = oHitPacket.getDist(i);
                    rU[i] = oHitPacket.getU(i);
                    rV[i] = oHitPacket.getV(i);
                    objId[i] = oHitPacket.getObjId(i);
                    triId[i] = oHitPacket.getTriId(i);
                    cacheId[i] = oHitPacket.getCacheId(i);
          
                    if(triId[i] != 0)
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
            printf("Hits: %u\n", hits);
            printf("Binary hitdata written to file\n");
#endif

#ifdef SINGLEFRAME
            fprintf(stderr, "Primary Ray Intersection completed\n");
#endif

            _free_align(hitEnvelope);
            _free_align(rayEnvelope);
            _free_align(rayInfoEnvelope);
            _free_align(rootBoxVolume);
            _free_align(triangleArray);
            _free_align(cacheInfoPack);
            _free_align(primIndexArray);

#ifndef OSG_XCACHEKD
            _free_align(nodeArray);
#endif
       
            UInt32 uiHitIndex = pThis->_pHitStore->getReadIndex();
            UInt32 uiHitStat = 0;

            ColorPacket oColor;
      
            while(uiHitIndex != HitStore::Empty)
            {
                if(uiHitIndex != HitStore::Waiting)
                {     
                    FourHitPacket &oHitPacket = 
                        pThis->_pHitStore->getPacket(uiHitIndex);

                    FourRayPacket *pRayPacket = 
                        oHitPacket.getRayPacket();

                    pThis->_pScene->shade(oHitPacket, *pRayPacket, oColor);

                    pThis->_pTarget->setPixel(oHitPacket.getX(), 
                                              oHitPacket.getY(),
                                              oColor           );
  
                    ++uiHitStat;
                }
                else
                {
                    osgSleep(1);
                }

                uiHitIndex = pThis->_pHitStore->getReadIndex();
            }
            printf("frame %u end\n", uiFrame);
            uiFrame++;

#ifdef SINGLEFRAME
//            exit(-1); //terminate PPE thread
#endif

            for (UInt8 t = 0 ; t < SPE_THREADS ; ++t) 
            {
                if ((rc = pthread_join ( datas[t].pthread, NULL)) != 0)
                {
                    fprintf(stderr,
                            "Failed pthread_join(rc=%d errno=%d,strerror=%s)\n",
                            rc,
                            errno,
                            strerror(errno)); 

                    exit (-1);
                }

                if((rc = spe_context_destroy (datas[t].speid)) != 0)
                {
                    fprintf(stderr,
                            "Failed spe_context_destroy(rc=%d, errno=%d"
                            "strerror=%s)\n",
                            rc,
                            errno,
                            strerror(errno));
                    exit (-1);
                }
            }

            pThis->_pSyncBarrier->enter();
            //================================================================
        }
    }
    else
    {
        OSG_ASSERT(false);  // Cell code does not support RTPrimaryRayTiledStore
    }  
}

#else // OSG_CELL

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
                
                FourRayPacket     &oRayPacket = 
                    pThis->_pRayStore->getRayPacket(uiRayIndex);

                FourRayPacketInfo &oRayPacketInfo = 
                    pThis->_pRayStore->getRayPacketInfo(uiRayIndex);
                
                FourHitPacket &oHitPacket = 
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
                FourRayPacket &oRayTile = pThis->_pRayStore->getRayPacket(i);
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
                FourHitPacket     &oHitPacket = 
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
                    FourHitPacket &oHitPacket = 
                        pThis->_pHitStore->getPacket(uiHitIndex);

                    FourRayPacket *pRayPacket = 
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

                        FourRayPacket &oRayPacket = 
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

#endif

OSG_END_NAMESPACE
