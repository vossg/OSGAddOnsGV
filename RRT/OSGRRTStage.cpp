/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include <OSGConfig.h>

#include "OSGTextureObjChunk.h"
#include "OSGMaterialChunk.h"
#include "OSGTriangleIterator.h"
#include "OSGPerspectiveCamera.h"

#include "OSGRTTextureTarget.h"

#include "OSGRTHitTile.h"

#include "OSGRTSingleHitPacket.h"
#include "OSGRTFourHitSIMDPacket.h"

#include "OSGRTSingleRayPacket.h"
#include "OSGRTFourRaySIMDPacket.h"

#include "OSGRTPrimaryRayStore.h"
#include "OSGRTPrimaryRayTiledStore.h"

#include "OSGRTHitStore.h"
#include "OSGRTHitTiledStore.h"

#include "OSGRTCombinedThread.h"

#include "OSGRTPrimaryRayThread.h"
#include "OSGRTShadingThread.h"

#include "OSGRTScene.h"
#include "OSGRTLocalPacketManager.h"
#include "OSGRTCacheKDVer1.h"
#include "OSGRTCacheKD.h"
#include "OSGRTTriangleAccel.h"

#include "OSGRRTStage.h"


#include "OSGRTInitAction.h"
#include "OSGRTUpdateAction.h"

#include "OSGRayTracer.h"
#include "OSGRayTracerInst.h"

OSG_BEGIN_NAMESPACE

OSG_RAYTRACERINST_INST(RRTStage::SinglePacketDesc)
OSG_RAYTRACERINST_INST(RRTStage::SIMDPacketDesc)

OSG_RTCACHEATTACHMENT_INST(RRTStage::SinglePacketCacheDesc)
OSG_RTCACHEATTACHMENT_INST(RRTStage::SIMDPacketCacheDesc  )

OSG_RTUPDATEACTION_INST(RRTStage::ActiveRayTracer::RTActionDesc,
                        RRTStageActiveRayTracerRTActionDesc)

OSG_RTINITACTION_INST(RRTStage::ActiveRayTracer::RTActionDesc,
                      RRTStageActiveRayTracerRTActionDesc)

OSG_RTPRIMARYRAYTHREAD_INST(RRTStage::SinglePacketDesc,
                            "RTPrimaryThreadStageSinglePacketDesc")
OSG_RTSHADINGTHREAD_INST(RRTStage::SinglePacketDesc,
                         "RTShadingThreadStageSinglePacketDesc")
OSG_RTCOMBINEDTHREAD_INST(RRTStage::SinglePacketDesc,
                         "RTCombinedThreadStageSinglePacketDesc")

OSG_RTPRIMARYRAYTHREAD_INST(RRTStage::SIMDPacketDesc,
                            "RTPrimaryThreadStageSIMDPacketDesc")
OSG_RTSHADINGTHREAD_INST(RRTStage::SIMDPacketDesc,
                         "RTShadingThreadStageSIMDPacketDesc")
OSG_RTCOMBINEDTHREAD_INST(RRTStage::SIMDPacketDesc,
                         "RTCombinedThreadStageSIMDPacketDesc")

// Documentation for this class is emitted in the
// OSGRRTStageBase.cpp file.
// To modify it, please change the .fcd file (OSGRRTStage.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void RRTStage::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
    }
}


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

RRTStage::RRTStage(void) :
     Inherited   (     ),
    _bInitialized(false),
    _pRayTracer  (NULL )
{
}

RRTStage::RRTStage(const RRTStage &source) :
     Inherited   (source              ),
    _bInitialized(source._bInitialized),
    _pRayTracer  (NULL                )
{
}

RRTStage::~RRTStage(void)
{
}

/*----------------------------- class specific ----------------------------*/

void RRTStage::changed(ConstFieldMaskArg whichField, 
                       UInt32            origin,
                       BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void RRTStage::run(CameraP pCam)
{
    if(_bInitialized == false)
    {
#if 1
        _pRayTracer->setResolution(_sfWidth .getValue(),
                                   _sfHeight.getValue());
#else
        _pRayTracer->setResolution(4, 4);
#endif

        if(_sfTextureTarget.getValue() != NullFC)
        {
            RTTextureTargetP pTexTarget = RTTextureTarget::create();

            pTexTarget->setTexObjChunk(_sfTextureTarget.getValue());

            _pRayTracer->setTarget(pTexTarget);
        }

        _pRayTracer->setRayTracingRoot(_sfRayTracingRoot.getValue());

        _pRayTracer->init(_sfTiled.getValue(), _sfSplitThreads.getValue());
       
       _bInitialized = true;
    }

    _pRayTracer->trace(pCam, _sfTiled.getValue());
}

void RRTStage::postProcess(DrawEnv *pDrawEnv)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static int frameCount = 1;

    _pRayTracer->finalize(pDrawEnv);
}

void RRTStage::dump(      UInt32    ,
                    const BitVector ) const
{
    SLOG << "Dump RRTStage NI" << std::endl;
}

void RRTStage::onCreate(const RRTStage *source)
{
    Inherited::onCreate(source);
}

void RRTStage::onCreateAspect(const RRTStage *createAspect,
                              const RRTStage *source)
{
    Inherited::onCreateAspect(createAspect, source);

    // Don't add the prototype instances to the list
    if(GlobalSystemState != Running)
        return;

    _pRayTracer = ActiveRayTracer::create();

    addRef(_pRayTracer);
}

void RRTStage::onDestroy(UInt32 uiContainerId)
{
    Inherited::onDestroy(uiContainerId);
}

void RRTStage::onDestroyAspect(UInt32    uiContainerId,
                               UInt32    uiAspect     )
{
    subRef(_pRayTracer);

    _pRayTracer = NULL;

    Inherited::onDestroyAspect(uiContainerId, uiAspect);
}

OSG_END_NAMESPACE
