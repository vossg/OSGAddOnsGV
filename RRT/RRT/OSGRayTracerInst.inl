/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

OSG_BEGIN_NAMESPACE

OSG_FIELD_BUNDLE_TMPL_NO_TYPE_DEF  (RayTracerInst, DescT)
OSG_RC_GET_STATIC_TYPE_ID_INL_TMPL_DEF(RayTracerInst, DescT)

template<typename DescT> inline
RayTracerInst<DescT>::RayTracerInst(void) :
     Inherited     (    ),
    _pScene        (NULL),
    _pPacketManager(NULL)
{
}

template<typename DescT> inline
RayTracerInst<DescT>::RayTracerInst(const RayTracerInst &source) :
     Inherited     (source),
    _pScene        (NULL  ),
    _pPacketManager(NULL  )
{
}

template<typename DescT> inline
RayTracerInst<DescT>::~RayTracerInst(void)
{
    subRef(_pScene);
    subRef(_pPacketManager);
}

template<typename DescT> inline
void RayTracerInst<DescT>::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);
}

template<typename DescT> inline
void RayTracerInst<DescT>::addCache(RTCache *pCache)
{
    _pScene->addCache(pCache);
}

template<typename DescT> inline
void RayTracerInst<DescT>::init(bool bTiled, bool bSplitThreads)
{
    NodePtr pRoot = _sfRayTracingRoot.getValue();

    if(pRoot != NULL)
    {
        fprintf(stderr, "RayTracer init\n");

//        pRoot->dump();

        setRefd(_pScene, new Scene);

        InitAction *pInitAction = InitAction::create();
        
        pInitAction->apply(pRoot);

        delete pInitAction;

#if 0
        char *outFileName = "/tmp/tri.osg";
        
        OSG::IndentFileOutStream outFileStream( outFileName );
        
        if(!outFileStream )
        {
            std::cerr << "Can not open output stream to file: "
                      << outFileName << std::endl;
        }
        else
        {
            std::cerr << "STARTING PRINTOUT:" << std::endl;
            OSGWriter writer( outFileStream, 4 );
            
            writer.write( pRoot );
            
            outFileStream.close();
        }
#endif


        setRefd(_pPacketManager, new PacketManager);

        _pPacketManager->setTarget(_pTarget);
        _pPacketManager->setScene (_pScene );
        
        if(bSplitThreads == false)
        {
            _pPacketManager->setup(bTiled);
        }
        else
        {
            _pPacketManager->setupSplitThreads(bTiled);
        }

    }
    else
    {
        fprintf(stderr, "RayTracer init root NULL\n");
    }
}

template<typename DescT> inline
void RayTracerInst<DescT>::trace(CameraP pCam, bool bTiled)
{
    if(_pTarget == NULL)
    {
        fprintf(stderr, "No Target\n");
        return;
    }

    NodePtr pRoot = _sfRayTracingRoot.getValue();

    UpdateAction *pUpdateAction = UpdateAction::create();
    
    _pScene->clearCache();

    pUpdateAction->setRayTracer(this);
    
    pUpdateAction->apply(pRoot);
    
    pUpdateAction->setRayTracer(NULL);
    
    delete pUpdateAction;

    if(bTiled == true)
    {
        _pPacketManager->trace(pCam, bTiled);
    }
    else
    {
        _pPacketManager->trace(pCam);
    }
}


template<typename DescT> inline
void RayTracerInst<DescT>::finalize(DrawEnv *pEnv)
{
    if(_pTarget != NULL)
    {
        _pPacketManager->finish();
    
        _pTarget->finalize(pEnv);
    }
}

OSG_END_NAMESPACE
