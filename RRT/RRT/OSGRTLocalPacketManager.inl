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
RTLocalPacketManager<DescT>::RTLocalPacketManager(void) :
     Inherited            (    ),
    _pTarget              (NULL),
    _pScene               (NULL),
    _pPrimaryRayStore     (NULL),
    _pHitStore            (NULL),
    _pPrimaryRayTiledStore(NULL),
    _pHitTiledStore       (NULL),
    _pSyncBarrier         (NULL),
    _vThreads             (    )
{
}

template<typename DescT> inline
RTLocalPacketManager<DescT>::~RTLocalPacketManager(void)
{
    _pTarget = NullFC;

    OSG::subRef(_pScene               );

    OSG::subRef(_pPrimaryRayStore     );
    OSG::subRef(_pHitStore            );

    OSG::subRef(_pPrimaryRayTiledStore);
    OSG::subRef(_pHitTiledStore       );

    OSG::subRef(_pSyncBarrier         );

}


template<typename DescT> inline
void RTLocalPacketManager<DescT>::setTarget(RTTarget *pTarget)
{
    _pTarget = pTarget;
}

template<typename DescT> inline
void RTLocalPacketManager<DescT>::setScene(Scene *pScene)
{
    setRefd(_pScene, pScene);
}


template<typename DescT> inline
void RTLocalPacketManager<DescT>::trace(RTCameraDecorator *pCam, bool bTiled)
{
    if(pCam != NULL && _pTarget != NULL)
    {
        _pTarget->startFrame();

        if(bTiled == true)
        {
            _pScene               ->setCamera ( pCam           );
            _pPrimaryRayTiledStore->startFrame(*pCam, *_pTarget);

            _pHitTiledStore->startFrame(
                *_pTarget, 
                 _pPrimaryRayTiledStore->getNumRayPackets());

        }
        else
        {
            _pScene          ->setCamera ( pCam           );
            _pPrimaryRayStore->startFrame(*pCam, *_pTarget);

            _pHitStore->startFrame(
                *_pTarget, 
                 _pPrimaryRayStore->getNumRayPackets());
        }

        _pSyncBarrier->enter();
    }
}

template<typename DescT> inline
void RTLocalPacketManager<DescT>::finish(void)
{
    _pSyncBarrier->enter();
}

template<typename DescT> inline
void RTLocalPacketManager<DescT>::setup(bool bTiled)
{
    UInt32 uiNumThreads = 1;

//    _pSyncBarrier = Barrier::get("RRT-Shading-DisplaySync");
    _pSyncBarrier = Barrier::get(NULL);

    _pSyncBarrier->setNumWaitFor(uiNumThreads + 1);
    
    OSG::addRef(_pSyncBarrier);
    
    if(bTiled == true)
    {
        _pPrimaryRayTiledStore = new PrimaryRayTiledStore;
        
        OSG::addRef(_pPrimaryRayTiledStore);

        _pHitTiledStore = new HitTiledStore;

        OSG::addRef(_pHitTiledStore);
    }
    else
    {
        _pPrimaryRayStore = new PrimaryRayStore;
        
        OSG::addRef(_pPrimaryRayStore);

        _pHitStore = new HitStore;

        OSG::addRef(_pHitStore);
    }

    for(UInt32 i = 0; i < uiNumThreads; ++i)
    {
        RTCombinedThread<Desc> *prThread;
            
        prThread = RTCombinedThread<Desc>::get(NULL);
  
        _vThreads.push_back(prThread);

        if(bTiled == true)
        {
            prThread->setup(_pScene, 
                            _pTarget,
                            _pPrimaryRayTiledStore, 
                            _pHitTiledStore, 
                            _pSyncBarrier,
                            i);
        }
        else
        {
            prThread->setup(_pScene, 
                            _pTarget,
                            _pPrimaryRayStore, 
                            _pHitStore, 
                            _pSyncBarrier,
                            i);
        }
        
        fprintf(stderr, "Start Thread %d\n", i);
        prThread->run();
    }

    _pSyncBarrier->enter();
}


template<typename DescT> inline
void RTLocalPacketManager<DescT>::setupSplitThreads(bool bTiled)
{
    UInt32 uiNumThreads = 1;

//    _pSyncBarrier = Barrier::get("RRT-Shading-DisplaySync");
    _pSyncBarrier = Barrier::get(NULL);

    _pSyncBarrier->setNumWaitFor(2 * uiNumThreads + 1);
    
    OSG::addRef(_pSyncBarrier);

    if(bTiled == true)
    {
        _pPrimaryRayTiledStore = new PrimaryRayTiledStore;
        
        OSG::addRef(_pPrimaryRayTiledStore);
        
        _pHitTiledStore = new HitTiledStore;
        
        OSG::addRef(_pHitTiledStore);
    }
    else
    {
        _pPrimaryRayStore = new PrimaryRayStore;
        
        OSG::addRef(_pPrimaryRayStore);
        
        _pHitStore = new HitStore;
        
        OSG::addRef(_pHitStore);
    }

    for(UInt32 i = 0; i < uiNumThreads; ++i)
    {
        RTPrimaryRayThread<Desc> *prThread;
            
        prThread = RTPrimaryRayThread<Desc>::get(NULL);
  
        _vThreads.push_back(prThread);

        if(bTiled == true)
        {
            prThread->setup(_pScene, 
                            _pPrimaryRayTiledStore, 
                            _pHitTiledStore, 
                            _pSyncBarrier,
                            i);
        }
        else
        {
            prThread->setup(_pScene, 
                            _pPrimaryRayStore, 
                            _pHitStore, 
                            _pSyncBarrier,
                            i);
        }
        
        prThread->run();


        RTShadingThread<Desc> *shThread;

        shThread = RTShadingThread<Desc>::get(NULL);
  
        _vThreads.push_back(shThread);

        if(bTiled == true)
        {
            shThread->setup(_pTarget, 
                            _pScene,
                            _pHitTiledStore, 
                            _pSyncBarrier, 
                            i);
        }
        else
        {
            shThread->setup(_pTarget, 
                            _pScene,
                            _pHitStore, 
                            _pSyncBarrier, 
                            i);
        }

        shThread->run();
    }

    _pSyncBarrier->enter();
}


OSG_END_NAMESPACE
