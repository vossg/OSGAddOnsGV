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
#include "OSGRTCameraDecorator.h"

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

//#include "OSGSFieldAdaptor.ins"
//#include "OSGMFieldAdaptor.ins"

OSG_BEGIN_NAMESPACE


OSG_RAYTRACERINST_INST(RRT::SinglePacketDesc)
OSG_RAYTRACERINST_INST(RRT::SIMDPacketDesc)

OSG_RTCACHEATTACHMENT_INST(RRT::SinglePacketCacheDesc)
OSG_RTCACHEATTACHMENT_INST(RRT::SIMDPacketCacheDesc  )

OSG_RTUPDATEACTION_INST(RRTStage::ActiveRayTracer::RTActionDesc,
                        RRTStageActiveRayTracerRTActionDesc)

OSG_RTINITACTION_INST(RRTStage::ActiveRayTracer::RTActionDesc,
                      RRTStageActiveRayTracerRTActionDesc)

OSG_RTPRIMARYRAYTHREAD_INST(RRT::SinglePacketDesc,
                            "RTPrimaryThreadStageSinglePacketDesc")
OSG_RTSHADINGTHREAD_INST(RRT::SinglePacketDesc,
                         "RTShadingThreadStageSinglePacketDesc")
OSG_RTCOMBINEDTHREAD_INST(RRT::SinglePacketDesc,
                         "RTCombinedThreadStageSinglePacketDesc")

OSG_RTPRIMARYRAYTHREAD_INST(RRT::SIMDPacketDesc,
                            "RTPrimaryThreadStageSIMDPacketDesc")
OSG_RTSHADINGTHREAD_INST(RRT::SIMDPacketDesc,
                         "RTShadingThreadStageSIMDPacketDesc")
OSG_RTCOMBINEDTHREAD_INST(RRT::SIMDPacketDesc,
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
DataType FieldTraits<RTCacheKD<RRT::SinglePacketDescBase> *>::_type(
    "RTCacheKDSinglePacketPtr",
    "RTCacheKDBaseSinglePacketPtr");

template<>
DataType FieldTraits<RTCacheKD<RRT::SIMDPacketDescBase> *>::_type(
    "RTCacheKDSIMDPacketPtr",
    "RTCacheKDBaseSinglePacketPtr");

template<> OSG_DLL_EXPORT
DataType &
    FieldTraits<RTCacheKD<RRT::SinglePacketDescBase> *>::getType(void)
{
    return _type;
}

template<> OSG_DLL_EXPORT
DataType &
    FieldTraits<RTCacheKD<RRT::SIMDPacketDescBase> *>::getType(void)
{
    return _type;
}
#else
template<>
DataType FieldTraits<RTXCacheKD<RRT::SinglePacketDescBase> *>::_type(
    "RTCacheKDSinglePacketPtr",
    "RTCacheKDBaseSinglePacketPtr");

template<>
DataType FieldTraits<RTXCacheKD<RRT::SIMDPacketDescBase> *>::_type(
    "RTCacheKDSIMDPacketPtr",
    "RTCacheKDBaseSinglePacketPtr");

template<>
DataType &
    FieldTraits<RTXCacheKD<RRT::SinglePacketDescBase> *>::getType(void)
{
    return _type;
}

template<>
DataType &
    FieldTraits<RTXCacheKD<RRT::SIMDPacketDescBase> *>::getType(void)
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
DataType FieldTraits<RTCacheBIH<RRT::SinglePacketDescBase> *>::_type(
    "RTCacheBIHSinglePacketPtr",
    "RTCacheBIHBaseSinglePacketPtr");

template<>
DataType FieldTraits<RTCacheBIH<RRT::SIMDPacketDescBase> *>::_type(
    "RTCacheBIHSIMDPacketPtr",
    "RTCacheBIHBaseSinglePacketPtr");

template<>
DataType &
    FieldTraits<RTCacheBIH<RRT::SinglePacketDescBase> *>::getType(void)
{
    return _type;
}

template<>
DataType &
    FieldTraits<RTCacheBIH<RRT::SIMDPacketDescBase> *>::getType(void)
{
    return _type;
}
#endif

//OSG_FIELDTRAITS_GETTYPE (RTCacheKD<RRT::SinglePacketDescBase> *)
//OSG_FIELDTRAITS_GETTYPE (RTCacheKD<RRT::SIMDPacketDescBase  > *)

#ifdef OSG_CACHE_KD
#ifndef OSG_XCACHEKD

OSG_EXPORT_PTR_SFIELD(PointerSField,
                      RTCacheKD<RRT::SinglePacketDescBase> *, 
                      UnrecordedRefCountPolicy,
                      0                                     );

OSG_EXPORT_PTR_MFIELD(PointerMField, 
                      RTCacheKD<RRT::SinglePacketDescBase> *, 
                      UnrecordedRefCountPolicy,
                      0                                     );

OSG_EXPORT_PTR_SFIELD(PointerSField, 
                      RTCacheKD<RRT::SIMDPacketDescBase> *, 
                      UnrecordedRefCountPolicy,
                      0                                   );

OSG_EXPORT_PTR_MFIELD(PointerMField, 
                      RTCacheKD<RRT::SIMDPacketDescBase> *, 
                      UnrecordedRefCountPolicy,
                      0                                   );
#else
OSG_FIELD_DLLEXPORT_DEF2(PointerSField, 
                         RTXCacheKD<RRT::SinglePacketDescBase> *, 
                         UnrecordedRefCountPolicy);

OSG_FIELD_DLLEXPORT_DEF2(PointerMField, 
                         RTXCacheKD<RRT::SinglePacketDescBase> *, 
                         UnrecordedRefCountPolicy);

OSG_FIELD_DLLEXPORT_DEF2(PointerSField, 
                         RTXCacheKD<RRT::SIMDPacketDescBase> *, 
                         UnrecordedRefCountPolicy);

OSG_FIELD_DLLEXPORT_DEF2(PointerMField, 
                         RTXCacheKD<RRT::SIMDPacketDescBase> *, 
                         UnrecordedRefCountPolicy);
#endif
#endif

#ifdef OSG_CACHE_BIH
OSG_EXPORT_PTR_SFIELD(PointerSField, 
                      RTCacheBIH<RRT::SinglePacketDescBase> *, 
                      UnrecordedRefCountPolicy,
                      0);

OSG_EXPORT_PTR_MFIELD(PointerMField, 
                      RTCacheBIH<RRT::SinglePacketDescBase> *, 
                      UnrecordedRefCountPolicy,
                      0);

OSG_EXPORT_PTR_SFIELD(PointerSField, 
                      RTCacheBIH<RRT::SIMDPacketDescBase> *, 
                      UnrecordedRefCountPolicy,
                      0);

OSG_EXPORT_PTR_MFIELD(PointerMField, 
                      RTCacheBIH<RRT::SIMDPacketDescBase> *, 
                      UnrecordedRefCountPolicy,
                      0);
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

OSG_FC_GET_TYPE_INST(RTCacheBase,    RRT::SIMDPacketDescBase)
#ifdef OSG_CACHE_KD
#ifndef OSG_XCACHEKD
OSG_FC_GET_TYPE_INST(RTCacheKD,      RRT::SIMDPacketDescBase)
OSG_FC_GET_TYPE_INST(RTCacheKDBase,  RRT::SIMDPacketDescBase)
#else
OSG_FC_GET_TYPE_INST(RTXCacheKD,     RRT::SIMDPacketDescBase)
OSG_FC_GET_TYPE_INST(RTXCacheKDBase, RRT::SIMDPacketDescBase)
#endif
#endif
#ifdef OSG_CACHE_BIH
OSG_FC_GET_TYPE_INST(RTCacheBIH,     RRT::SIMDPacketDescBase)
OSG_FC_GET_TYPE_INST(RTCacheBIHBase, RRT::SIMDPacketDescBase)
#endif

OSG_FC_GET_TYPE_INST(RTCacheBase  ,  RRT::SinglePacketDescBase)
#ifdef OSG_CACHE_KD
#ifndef OSG_XCACHEKD
OSG_FC_GET_TYPE_INST(RTCacheKD,      RRT::SinglePacketDescBase)
OSG_FC_GET_TYPE_INST(RTCacheKDBase,  RRT::SinglePacketDescBase)
#else
OSG_FC_GET_TYPE_INST(RTXCacheKD,     RRT::SinglePacketDescBase)
OSG_FC_GET_TYPE_INST(RTXCacheKDBase, RRT::SinglePacketDescBase)
#endif
#endif
#ifdef OSG_CACHE_BIH
OSG_FC_GET_TYPE_INST(RTCacheBIH,     RRT::SinglePacketDescBase)
OSG_FC_GET_TYPE_INST(RTCacheBIHBase, RRT::SinglePacketDescBase)
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
    if(0x000 != (whichField & (WidthFieldMask | HeightFieldMask)))
    {
        if(_pRayTracer != NULL)
        {
            _pRayTracer->setResolution(_sfWidth .getValue(),
                                       _sfHeight.getValue());
        }
    }
    else
    {
        Inherited::changed(whichField, origin, details);
    }
}

#define RT_DEBUG_IMAGE

void RRTStage::run(CameraP     pCam,
                   Background *pBackground,
                   Node       *pRoot)
{
    if(_bInitialized == false)
    {
#if 1
        _pRayTracer->setResolution(_sfWidth .getValue(),
                                   _sfHeight.getValue());
#else
        _pRayTracer->setResolution(4, 4);
#endif

        if(_sfTextureTarget.getValue() != NULL)
        {
            RTTextureTargetUnrecPtr pTexTarget = RTTextureTarget::create();

            pTexTarget->setTexObjChunk(_sfTextureTarget.getValue());

            _pRayTracer->setTarget(pTexTarget);
        }
#ifdef RT_DEBUG_IMAGE
        else
        {
            RTImageTargetUnrecPtr pImgTarget = RTImageTarget::create();

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
        SolidBackground *pBack = 
            dynamic_cast<SolidBackground *>(pBackground);
        
        if(pBack != NULL)
        {
            _pRayTracer->setBackgroundColor(pBack->getColor()[0],
                                            pBack->getColor()[1],
                                            pBack->getColor()[2],
                                            pBack->getAlpha()   );

        }
    }

    RTCameraDecorator *pRTCam = dynamic_cast<RTCameraDecorator *>(pCam);

    if(pRTCam == NULL)
    {
        if(getRTCamera() == NULL)
        {
            RTCameraDecoratorUnrecPtr pRTCamDeco = 
                RTCameraDecorator::create();

            setRTCamera(pRTCamDeco);
        }

        pRTCam = getRTCamera();

        pRTCam->setDecoratee(pCam);
    }
    
    _pRayTracer->trace(pRTCam, _sfTiled.getValue());
}

void RRTStage::postProcess(DrawEnv *pDrawEnv)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static int frameCount = 1;

    _pRayTracer->finalize(pDrawEnv);
}

void RRTStage::setSize(UInt32 uiWidth, UInt32 uiHeight)
{
    setWidth (uiWidth );
    setHeight(uiHeight);
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

    _pRayTracer = ActiveRayTracer::createLocal();
}

void RRTStage::onDestroy(UInt32 uiContainerId)
{
    Inherited::onDestroy(uiContainerId);
}

void RRTStage::onDestroyAspect(UInt32    uiContainerId,
                               UInt32    uiAspect     )
{
    _pRayTracer = NULL;

    Inherited::onDestroyAspect(uiContainerId, uiAspect);
}

OSG_END_NAMESPACE
