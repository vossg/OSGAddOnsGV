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
#include "OSGSolidBackground.h"

#include "OSGRTTextureTarget.h"
#include "OSGRTImageTarget.h"

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
#ifdef OSG_CACHE_KD
#ifndef OSG_XCACHEKD
#include "OSGRTCacheKD.h"
#else
#include "OSGRTXCacheKD.h"
#endif
#endif
#ifdef OSG_CACHE_BIH
#include "OSGRTCacheBIH.h"
#endif

#include "OSGRTTriangleAccel.h"
#include "OSGRTTriangleAccelFields.h"

#include "OSGRRTStage.h"


#include "OSGRTInitAction.h"
#include "OSGRTUpdateAction.h"

#include "OSGRayTracer.h"
#include "OSGRayTracerInst.h"

#include "OSGRTCacheAttachmentInst.h"

#include "OSGRTCacheBase.ins"

#ifdef OSG_CACHE_KD
#ifndef OSG_XCACHEKD
#include "OSGRTCacheKDBase.ins"
#include "OSGRTCacheKD.ins"
#else
#include "OSGRTXCacheKDBase.ins"
#include "OSGRTXCacheKD.ins"
#endif
#endif
#ifdef OSG_CACHE_BIH
#include "OSGRTCacheBIHBase.ins"
#include "OSGRTCacheBIH.ins"
#endif

#include "OSGSFieldAdaptor.ins"
#include "OSGMFieldAdaptor.ins"

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

static DataType SingleCacheBaseType(
    "RTCacheBaseSinglePacketPtr",
    "FieldContainerPtr");

static DataType SIMDCacheBaseType(
    "RTCacheBaseSIMDPacketPtr",
    "FieldContainerPtr");

#ifdef OSG_CACHE_KD
static DataType SingleCacheKDBaseType(
    "RTCacheKDBaseSinglePacketPtr",
    "RTCacheBaseSinglePacketPtr");

static DataType SIMDCacheKDBaseType(
    "RTCacheKDBaseSIMDPacketPtr",
    "RTCacheBaseSIMDPacketPtr");

#ifndef OSG_XCACHEKD
template<>
DataType FieldTraits<RTCacheKD<RRTStage::SinglePacketDescBase> *>::_type(
    "RTCacheKDSinglePacketPtr",
    "RTCacheKDBaseSinglePacketPtr");

template<>
DataType FieldTraits<RTCacheKD<RRTStage::SIMDPacketDescBase> *>::_type(
    "RTCacheKDSIMDPacketPtr",
    "RTCacheKDBaseSinglePacketPtr");

template<>
DataType &
    FieldTraits<RTCacheKD<RRTStage::SinglePacketDescBase> *>::getType(void)
{
    return _type;
}

template<>
DataType &
    FieldTraits<RTCacheKD<RRTStage::SIMDPacketDescBase> *>::getType(void)
{
    return _type;
}
#else
template<>
DataType FieldTraits<RTXCacheKD<RRTStage::SinglePacketDescBase> *>::_type(
    "RTCacheKDSinglePacketPtr",
    "RTCacheKDBaseSinglePacketPtr");

template<>
DataType FieldTraits<RTXCacheKD<RRTStage::SIMDPacketDescBase> *>::_type(
    "RTCacheKDSIMDPacketPtr",
    "RTCacheKDBaseSinglePacketPtr");

template<>
DataType &
    FieldTraits<RTXCacheKD<RRTStage::SinglePacketDescBase> *>::getType(void)
{
    return _type;
}

template<>
DataType &
    FieldTraits<RTXCacheKD<RRTStage::SIMDPacketDescBase> *>::getType(void)
{
    return _type;
}
#endif
#endif

#ifdef OSG_CACHE_BIH
static DataType SingleCacheBIHBaseType(
    "RTCacheBIHBaseSinglePacketPtr",
    "RTCacheBaseSinglePacketPtr");

static DataType SIMDCacheBIHBaseType(
    "RTCacheBIHBaseSIMDPacketPtr",
    "RTCacheBaseSIMDPacketPtr");

template<>
DataType FieldTraits<RTCacheBIH<RRTStage::SinglePacketDescBase> *>::_type(
    "RTCacheBIHSinglePacketPtr",
    "RTCacheBIHBaseSinglePacketPtr");

template<>
DataType FieldTraits<RTCacheBIH<RRTStage::SIMDPacketDescBase> *>::_type(
    "RTCacheBIHSIMDPacketPtr",
    "RTCacheBIHBaseSinglePacketPtr");

template<>
DataType &
    FieldTraits<RTCacheBIH<RRTStage::SinglePacketDescBase> *>::getType(void)
{
    return _type;
}

template<>
DataType &
    FieldTraits<RTCacheBIH<RRTStage::SIMDPacketDescBase> *>::getType(void)
{
    return _type;
}
#endif

//OSG_FIELDTRAITS_GETTYPE (RTCacheKD<RRTStage::SinglePacketDescBase> *)
//OSG_FIELDTRAITS_GETTYPE (RTCacheKD<RRTStage::SIMDPacketDescBase  > *)

#ifdef OSG_CACHE_KD
#ifndef OSG_XCACHEKD
OSG_FIELD_DLLEXPORT_DEF2(SFieldAdaptor, 
                         RTCacheKD<RRTStage::SinglePacketDescBase> *, 
                         SFFieldContainerPtr);

OSG_FIELD_DLLEXPORT_DEF2(MFieldAdaptor, 
                         RTCacheKD<RRTStage::SinglePacketDescBase> *, 
                         MFFieldContainerPtr);

OSG_FIELD_DLLEXPORT_DEF2(SFieldAdaptor, 
                         RTCacheKD<RRTStage::SIMDPacketDescBase> *, 
                         SFFieldContainerPtr);

OSG_FIELD_DLLEXPORT_DEF2(MFieldAdaptor, 
                         RTCacheKD<RRTStage::SIMDPacketDescBase> *, 
                         MFFieldContainerPtr);
#else
OSG_FIELD_DLLEXPORT_DEF2(SFieldAdaptor, 
                         RTXCacheKD<RRTStage::SinglePacketDescBase> *, 
                         SFFieldContainerPtr);

OSG_FIELD_DLLEXPORT_DEF2(MFieldAdaptor, 
                         RTXCacheKD<RRTStage::SinglePacketDescBase> *, 
                         MFFieldContainerPtr);

OSG_FIELD_DLLEXPORT_DEF2(SFieldAdaptor, 
                         RTXCacheKD<RRTStage::SIMDPacketDescBase> *, 
                         SFFieldContainerPtr);

OSG_FIELD_DLLEXPORT_DEF2(MFieldAdaptor, 
                         RTXCacheKD<RRTStage::SIMDPacketDescBase> *, 
                         MFFieldContainerPtr);
#endif
#endif

#ifdef OSG_CACHE_BIH
OSG_FIELD_DLLEXPORT_DEF2(SFieldAdaptor, 
                         RTCacheBIH<RRTStage::SinglePacketDescBase> *, 
                         SFFieldContainerPtr);

OSG_FIELD_DLLEXPORT_DEF2(MFieldAdaptor, 
                         RTCacheBIH<RRTStage::SinglePacketDescBase> *, 
                         MFFieldContainerPtr);

OSG_FIELD_DLLEXPORT_DEF2(SFieldAdaptor, 
                         RTCacheBIH<RRTStage::SIMDPacketDescBase> *, 
                         SFFieldContainerPtr);

OSG_FIELD_DLLEXPORT_DEF2(MFieldAdaptor, 
                         RTCacheBIH<RRTStage::SIMDPacketDescBase> *, 
                         MFFieldContainerPtr);
#endif

#define OSG_FC_GET_TYPE_INST(CLASS, T) \
                                       \
template <>                            \
CLASS < T >::TypeObject &              \
    CLASS < T >::getType(void)         \
{                                      \
    return _type;                      \
}                                      \
                                       \
template <>                            \
const CLASS < T >::TypeObject &        \
    CLASS < T >::getType(void) const   \
{                                      \
    return _type;                      \
}                                      \
                                       \
template <>                            \
CLASS < T >::TypeObject &              \
    CLASS < T >::getClassType(void)    \
{                                      \
    return _type;                      \
}

OSG_FC_GET_TYPE_INST(RTCacheBase,    RRTStage::SIMDPacketDescBase)
#ifdef OSG_CACHE_KD
#ifndef OSG_XCACHEKD
OSG_FC_GET_TYPE_INST(RTCacheKD,      RRTStage::SIMDPacketDescBase)
OSG_FC_GET_TYPE_INST(RTCacheKDBase,  RRTStage::SIMDPacketDescBase)
#else
OSG_FC_GET_TYPE_INST(RTXCacheKD,     RRTStage::SIMDPacketDescBase)
OSG_FC_GET_TYPE_INST(RTXCacheKDBase, RRTStage::SIMDPacketDescBase)
#endif
#endif
#ifdef OSG_CACHE_BIH
OSG_FC_GET_TYPE_INST(RTCacheBIH,     RRTStage::SIMDPacketDescBase)
OSG_FC_GET_TYPE_INST(RTCacheBIHBase, RRTStage::SIMDPacketDescBase)
#endif

OSG_FC_GET_TYPE_INST(RTCacheBase  ,  RRTStage::SinglePacketDescBase)
#ifdef OSG_CACHE_KD
#ifndef OSG_XCACHEKD
OSG_FC_GET_TYPE_INST(RTCacheKD,      RRTStage::SinglePacketDescBase)
OSG_FC_GET_TYPE_INST(RTCacheKDBase,  RRTStage::SinglePacketDescBase)
#else
OSG_FC_GET_TYPE_INST(RTXCacheKD,     RRTStage::SinglePacketDescBase)
OSG_FC_GET_TYPE_INST(RTXCacheKDBase, RRTStage::SinglePacketDescBase)
#endif
#endif
#ifdef OSG_CACHE_BIH
OSG_FC_GET_TYPE_INST(RTCacheBIH,     RRTStage::SinglePacketDescBase)
OSG_FC_GET_TYPE_INST(RTCacheBIHBase, RRTStage::SinglePacketDescBase)
#endif

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

#define RT_DEBUG_IMAGE

void RRTStage::run(CameraP     pCam,
                   Background *pBackground,
                   NodePtr     pRoot)
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
#ifdef RT_DEBUG_IMAGE
        else
        {
            RTImageTargetP pImgTarget = RTImageTarget::create();

            _pRayTracer->setTarget(pImgTarget);
        }
#endif

//        _pRayTracer->setRayTracingRoot(_sfRayTracingRoot.getValue());

        _pRayTracer->setRayTracingRoot(pRoot);

        _pRayTracer->init(_sfTiled.getValue(), _sfSplitThreads.getValue());
       
       _bInitialized = true;
    }

    if(pBackground != NULL)
    {
        SolidBackgroundPtr pBack = 
            dynamic_cast<SolidBackgroundPtr>(pBackground);
        
        if(pBack != NULL)
        {
            _pRayTracer->setBackgroundColor(pBack->getColor()[0],
                                            pBack->getColor()[1],
                                            pBack->getColor()[2],
                                            pBack->getAlpha()   );

        }
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
