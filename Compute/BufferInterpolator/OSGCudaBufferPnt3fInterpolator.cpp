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


#include "OSGConfig.h"

#include "OSGGL.h"

#ifdef OSG_WITH_CUDA
#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>
#endif

#include "OSGCudaBufferPnt3fInterpolator.h"
#include "OSGInterpolationHelper.h"
#include "OSGTypedGeoVectorProperty.h"
#include "OSGGeoVectorBufferProperty.h"

#include "OSGBufferInterpolator.cuh"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGCudaBufferPnt3fInterpolatorBase.cpp file.
// To modify it, please change the .fcd file
// (OSGCudaBufferPnt3fInterpolator.fcd) and regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

void CudaBufferPnt3fInterpolator::ContextData::releaseResources(
    HardwareContext *pContext)
{
    CallbackTaskRefPtr pShutdownTask = 
        new CallbackTask(CallbackTask::Callback);

    pShutdownTask->setCallback(
        boost::bind(&CudaBufferPnt3fInterpolator::ContextData::shutdownCuda,
                    this));
    
    pContext->submitTaskAndWait(pShutdownTask);
}

void CudaBufferPnt3fInterpolator::ContextData::shutdownCuda(void)
{
#ifdef OSG_WITH_CUDA
    for(UInt32 i = 0; i < this->_vCudaValues.size(); ++i)
    {
        cudaFree(this->_vCudaValues[i]);
    }

    this->_vCudaValues.clear();
#endif
}

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void CudaBufferPnt3fInterpolator::initMethod(InitPhase ePhase)
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

CudaBufferPnt3fInterpolator::CudaBufferPnt3fInterpolator(void) :
     Inherited(    ),
    _pTask    (NULL)
{
}

CudaBufferPnt3fInterpolator::CudaBufferPnt3fInterpolator(
    const CudaBufferPnt3fInterpolator &source) :

     Inherited(source),
    _pTask    (NULL  )
{
}

CudaBufferPnt3fInterpolator::~CudaBufferPnt3fInterpolator(void)
{
}

/*----------------------------- class specific ----------------------------*/

//typedef size_t SizeT;

void CudaBufferPnt3fInterpolator::changed(ConstFieldMaskArg whichField, 
                                          UInt32            origin,
                                          BitVector         details)
{
    if(0x0000 != (whichField & (KeyFieldMask | KeyValueFieldMask)))
    {
        if(_sfOutValue.getValue() != NULL)
        {
            GeoPnt3fProperty *pPnts = 
                dynamic_cast<GeoPnt3fProperty *>(_sfOutValue.getValue());

            GeoVectorBufferProperty *pBuffPnts = 
                dynamic_cast<GeoVectorBufferProperty *>(_sfOutValue.getValue());

            SizeT uiNumNumRes = _mfKeyValue.size() / _mfKey.size();

            fprintf(stderr, "cbi: %d %d %d\n",
                    UInt32(_mfKeyValue.size()),
                    UInt32(_mfKey.size()),
                    UInt32(uiNumNumRes));

            if(pPnts != NULL)
            {
                pPnts->editFieldPtr()->resize(uiNumNumRes);
#if 0
                InterpolationHelper<MFReal32, 
                                    MFPnt3f, 
                                    MFPnt3f>::interpolate(
                                        _sfInValue.getValue(),
                                        _mfKey,
                                        _mfKeyValue,
                                        *(pPnts->editFieldPtr()));
#endif
            }
            else if (pBuffPnts != NULL)
            {
                pBuffPnts->setFormat    (GL_FLOAT   );
                pBuffPnts->setFormatSize(          4);
                pBuffPnts->setDimension (          3);
                pBuffPnts->setStride    (          0);
                pBuffPnts->setUseVBO    (       true);
                pBuffPnts->setSize      (uiNumNumRes);
            }
        }


#if 0
        CallbackDrawTaskRefPtr pInitTask = 
            new CallbackDrawTask(CallbackDrawTask::Callback);

        pInitTask->setCallback(
            boost::bind(&CudaBufferPnt3fInterpolator::setupCuda, 
                        this,
                        _1,
                        _2));

        Window::queueGlobalTask(pInitTask);
#endif
    }
    if(0x0000 != (whichField & ResortIndexFieldMask))
    {
        fprintf(stderr, "resort triggert\n");        

        SizeT uiNumRes = _mfKeyValue.size() / _mfKey.size();

        if(uiNumRes == _mfResortIndex.size())
        {
            MFKeyValueType tmpKeyValues;

            tmpKeyValues.resize(_mfKeyValue.size());

            UInt32 uiGlobalIdx = 0;
        

            for(UInt32 i = 0; i < _mfKey.size(); ++i)
            {
                UInt32 uiGlobalBase = i * uiNumRes;
                
                for(UInt32 j = 0; j < uiNumRes; ++j, ++uiGlobalIdx)
                {
                    tmpKeyValues[uiGlobalIdx] = 
                        _mfKeyValue[uiGlobalBase + _mfResortIndex[j]];
                }
            }

            *(this->editMFKeyValue()) = tmpKeyValues;

                CallbackDrawTaskRefPtr pResortTask = 
                new CallbackDrawTask(CallbackDrawTask::Callback);

            pResortTask->setCallback(
                boost::bind(&CudaBufferPnt3fInterpolator::resubmitCuda, 
                            this,
                            _1,
                            _2));
            
            Window::queueGlobalTask(pResortTask);
        }
        else
        {
            fprintf(stderr, "interpol resort : sizes don't match %zd | %d\n",
                    uiNumRes, _mfResortIndex.size32());
        }
    }
    if(0x0000 != (whichField & InValueFieldMask))
    {
        if(_pTask == NULL)
        {
            _pTask = new CallbackDrawTask(CallbackDrawTask::Callback);
        }

        _pTask->setCallback(
            boost::bind(&CudaBufferPnt3fInterpolator::computeCuda, 
                        this,
                        _1,
                        _2));

        Window::queueGlobalTask(_pTask);

        // hack to signal change
        _sfOutValue.getValue()->editSFUsage();
    }

    BufferPnt3fInterpolatorBase::changed(whichField, origin, details);
}

void CudaBufferPnt3fInterpolator::dump(      UInt32    ,
                                       const BitVector ) const
{
    SLOG << "Dump CudaBufferPnt3fInterpolator NI" << std::endl;
}

void CudaBufferPnt3fInterpolator::setupCuda(HardwareContext *pContext, 
                                            DrawEnv         *pEnv    )
{
}

void CudaBufferPnt3fInterpolator::computeCuda(HardwareContext *pContext, 
                                              DrawEnv         *pEnv    )
{
#ifdef OSG_WITH_CUDA
//    fprintf(stderr, "computeCuda\n");

    cudaError_t cRes;

    if(_mfKey.size() == 0 || _mfKeyValue.size() == 0)
    {
        return;
    }

    SizeT uiNumRes = _mfKeyValue.size() / _mfKey.size();

    ContextData *pData = pContext->getData<ContextData *>(this->_iDataSlotId);

    if(pData == NULL)
    {
        pData = new ContextData;

        this->setData(pData, this->_iDataSlotId, pContext);

        pData->_vCudaValues.resize(_mfKey.size(), NULL);
        
        for(UInt32 i = 0; i < _mfKey.size(); ++i)
        {
            float *pCVals = NULL;

            cudaMalloc(reinterpret_cast<void **>(&pCVals), 
                       uiNumRes * 3 * 4);

            cudaMemcpy( pCVals, 
                       _mfKeyValue[i * uiNumRes].getValues(),
                        uiNumRes * 3 * 4, 
                        cudaMemcpyHostToDevice);

            pData->_vCudaValues[i] = pCVals;
        }

        pContext->setCudaInit();
    }

    Window            *pWin  = pEnv->getWindow();

    GeoVectorProperty *pProp = _sfOutValue.getValue();

    pWin->validateGLObject(pProp->getGLId(), pEnv);

    UInt32 uiPropVBOId = pWin->getGLObjectId(pProp->getGLId());


    cRes = cudaGLRegisterBufferObject(uiPropVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "RegisterRes %d\n", UInt32(cRes));
    }

    void *pMappedBuff = NULL;

    cRes = cudaGLMapBufferObject(&pMappedBuff, uiPropVBOId);	 

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "MappedPtr, Res : %d\n", 
                UInt32(cRes));

    }


    MFKeyType::const_iterator keyIt;

    keyIt = lower_bound(_mfKey.begin(), 
                        _mfKey.end  (),
                        _sfInValue.getValue());

    if(keyIt != _mfKey.end())
    {
        if(keyIt == _mfKey.begin())
        {
            cudaMemcpy(pMappedBuff,
                       pData->_vCudaValues[0],
                       uiNumRes * 3 * 4,
                       cudaMemcpyDeviceToDevice);
        }
        else
        {
            UInt32 uiStopIndex  = keyIt - _mfKey.begin();
            UInt32 uiStartIndex = uiStopIndex - 1;

            Real32 t =
                (_sfInValue.getValue() - _mfKey[uiStartIndex]) /
                (_mfKey[uiStopIndex]   - _mfKey[uiStartIndex]);

            OSGCUDA::lerpFloat(pData->_vCudaValues[uiStartIndex],
                               pData->_vCudaValues[uiStopIndex ],
                               t,
                               uiNumRes * 3,
                               static_cast<float *>(pMappedBuff));
        }
    }
    else
    {
        cudaMemcpy(pMappedBuff,
                   pData->_vCudaValues[_mfKey.size() - 1],
                   uiNumRes * 3 * 4,
                   cudaMemcpyDeviceToDevice);
    }

    cRes = cudaGLUnmapBufferObject(uiPropVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnmapRes %d\n", UInt32(cRes));
    }

    cRes = cudaGLUnregisterBufferObject(uiPropVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnRegisterRes %d\n", UInt32(cRes));
    }
#endif
}

void CudaBufferPnt3fInterpolator::resubmitCuda(HardwareContext *pContext, 
                                               DrawEnv         *pEnv    )
{
#ifdef OSG_WITH_CUDA
    fprintf(stderr, "resubmitCuda\n");

    ContextData *pData = pContext->getData<ContextData *>(this->_iDataSlotId);

    if(pData != NULL)
    {
        SizeT uiNumRes = _mfKeyValue.size() / _mfKey.size();

        for(UInt32 i = 0; i < _mfKey.size(); ++i)
        {
            float *pCVals = pData->_vCudaValues[i];

            cudaMemcpy( pCVals, 
                       _mfKeyValue[i * uiNumRes].getValues(),
                        uiNumRes * 3 * 4, 
                        cudaMemcpyHostToDevice);

        }

        computeCuda(pContext, pEnv);
    }
#endif
}

void CudaBufferPnt3fInterpolator::resolveLinks(void)
{
    _pTask = NULL;

    Inherited::resolveLinks();
}

OSG_END_NAMESPACE
