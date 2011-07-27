/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2002 by the OpenSG Forum                   *
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

#include <cstdlib>
#include <cstdio>

#include <sstream>
#include <fstream>

#include "OSGConfig.h"

#include "OSGAction.h"
#include "OSGCamera.h"
#include "OSGSceneFileHandler.h"
#include "OSGVolumeDraw.h"

#include "OSGSimpleCudaComputeAlgorithm.h"
#include "OSGDrawEnv.h"

#include "OSGSimpleCompute.cuh"

#ifdef OSG_WITH_CUDA
#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>
#endif

OSG_BEGIN_NAMESPACE

// Documentation for this class is emited in the
// OSGSimpleCudaComputeAlgorithmBase.cpp file.
// To modify it, please change the .fcd file (OSGSimpleCudaComputeAlgorithm.fcd)
// and regenerate the base file.

/*-------------------------------------------------------------------------*/
/*                               Sync                                      */

void SimpleCudaComputeAlgorithm::changed(ConstFieldMaskArg whichField, 
                                         UInt32            origin,
                                         BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

/*-------------------------------------------------------------------------*/
/*                               Dump                                      */

void SimpleCudaComputeAlgorithm::dump(
          UInt32    OSG_CHECK_ARG(uiIndent), 
    const BitVector OSG_CHECK_ARG(bvFlags )) const
{
    SLOG << "Dump SimpleCudaComputeAlgorithm NI" << std::endl;
}

/*-------------------------------------------------------------------------*/
/*                            Constructors                                 */

SimpleCudaComputeAlgorithm::SimpleCudaComputeAlgorithm(void) :
    Inherited()
{
}

SimpleCudaComputeAlgorithm::SimpleCudaComputeAlgorithm(
    const SimpleCudaComputeAlgorithm &source) :

    Inherited(source)
{
}

/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

SimpleCudaComputeAlgorithm::~SimpleCudaComputeAlgorithm(void)
{
}

/*-------------------------------------------------------------------------*/
/*                                Draw                                     */

ActionBase::ResultE SimpleCudaComputeAlgorithm::renderEnter(Action *pAction)
{
    fprintf(stderr, "SimpleCudaComputeAlgorithm::renderEnter\n");

    return Action::Continue;
}

ActionBase::ResultE SimpleCudaComputeAlgorithm::renderLeave(Action *pAction)
{
    fprintf(stderr, "SimpleCudaComputeAlgorithm::renderLeave\n");

    return Action::Continue;
}

void SimpleCudaComputeAlgorithm::execute(HardwareContext *pContext, 
                                         DrawEnv         *pEnv    )
{
#if 0
    fprintf(stderr, "SimpleCudaComputeAlgorithm::execute %p %p\n",
            pContext,
            pEnv);
#endif

    UInt32 uiUpdateMode = _sfMode.getValue();

    if(uiUpdateMode == OSG::SimpleCudaComputeAlgorithm::UpdateOSGAlgo)
    {
        updateGeoOSG(pContext, pEnv);
    }
    else if(uiUpdateMode == 
                           OSG::SimpleCudaComputeAlgorithm::UpdateOSGMappedAlgo)
    {
        updateGeoOSGMapped(pContext, pEnv);
    }
    else if(uiUpdateMode == OSG::SimpleCudaComputeAlgorithm::UpdateOSGCudaAlgo)
    {
        updateGeoOSGCuda(pContext, pEnv);
    }
}

/*-------------------------------------------------------------------------*/
/*                               loading                                   */

void SimpleCudaComputeAlgorithm::incScale(Real32 rVal)
{
    setScale(_sfScale.getValue() - rVal);
}

void SimpleCudaComputeAlgorithm::decScale(Real32 rVal)
{
    setScale(_sfScale.getValue() + rVal);
}

/*-------------------------------------------------------------------------*/
/*                               Init                                      */

void SimpleCudaComputeAlgorithm::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
    }
}

void SimpleCudaComputeAlgorithm::updateScale(void)
{
    setScale(_sfScale.getValue() + _sfScaleStep.getValue());

    if(_sfScale.getValue() > _sfScaleMax.getValue())
    {
        setScale    (_sfScaleMax .getValue()       );
        setScaleStep(_sfScaleStep.getValue() * -1.f);
    }

    if(_sfScale.getValue() < 1.f)
    {
        setScale    (1.f                           );
        setScaleStep(_sfScaleStep.getValue() * -1.f);
    }
}

void SimpleCudaComputeAlgorithm::updateGeoOSG(OSG::HardwareContext *pContext, 
                                              OSG::DrawEnv         *pEnv    )
{
    if(_sfTarget.getValue() == NULL)
    {
        return;
    }

    GeoPnt3fProperty *pnts  = dynamic_cast<GeoPnt3fProperty *>(
        _sfTarget.getValue()->getProperty(Geometry::PositionsIndex));

    GeoVec3fProperty *norms = dynamic_cast<GeoVec3fProperty *>(
        _sfTarget.getValue()->getProperty(Geometry::NormalsIndex));

    OSG_ASSERT((pnts != NULL) && (norms != NULL));

    OSG::GeoPnt3fProperty::StoredFieldType *p  = pnts ->editFieldPtr();
    OSG::GeoVec3fProperty::StoredFieldType *n  = norms->editFieldPtr();

    setScale(_sfScale.getValue() + _sfScaleStep.getValue());

    if(_sfScale.getValue() > _sfScaleMax.getValue())
    {
        setScale    (_sfScaleMax .getValue()       );
        setScaleStep(_sfScaleStep.getValue() * -1.f);
    }

    if(_sfScale.getValue() < 1.f)
    {
        setScale    (1.f                           );
        setScaleStep(_sfScaleStep.getValue() * -1.f);
    }

    Real32 rIntensity = _sfIntensity.getValue();
    Real32 rScale     = _sfScale    .getValue();

    for(OSG::UInt32 i = 0; i < p->size(); ++i)
    {
        OSG::Vec3f tmpVec = (*p)[i].subZero();

        tmpVec[2] = 0.f;
        
        OSG::Real32 rDist = tmpVec.length();

        OSG::Real32 rBump = rIntensity * OSG::osgCos(rDist * rScale);

        (*p)[i][2] = rBump;

        rBump = rIntensity * (rScale/(rDist)) * -2 * -sin(rDist * rScale);

        tmpVec[0] = rBump * (*p)[i][0];
        tmpVec[1] = rBump * (*p)[i][1];
        tmpVec[2] = 1.f;
       

        tmpVec.normalize();

        (*n)[i] = tmpVec;
    }

    commitChanges();
}

void SimpleCudaComputeAlgorithm::updateGeoOSGMapped(
    OSG::HardwareContext *pContext, 
    OSG::DrawEnv         *pEnv    )
{
    if(_sfTarget.getValue() == NULL)
    {
        return;
    }

    GeoPnt3fProperty *pnts  = dynamic_cast<GeoPnt3fProperty *>(
        _sfTarget.getValue()->getProperty(Geometry::PositionsIndex));

    GeoVec3fProperty *norms = dynamic_cast<GeoVec3fProperty *>(
        _sfTarget.getValue()->getProperty(Geometry::NormalsIndex));


    OSG_ASSERT((pnts != NULL) && (norms != NULL));

    float *p = static_cast<float *>(pnts ->mapBuffer(GL_WRITE_ONLY_ARB, pEnv));
    float *n = static_cast<float *>(norms->mapBuffer(GL_WRITE_ONLY_ARB, pEnv));

#if 0
    rScale += rScaleStep;

    if(rScale > rScaleMax)
    {
        rScale = rScaleMax;
        rScaleStep *= -1;
    }

    if(rScale < 1)
    {
        rScale = 1;
        rScaleStep *= -1;
    }
#endif

    updateScale();

    Real32 rIntensity = _sfIntensity.getValue();
    Real32 rScale     = _sfScale    .getValue();

    for(OSG::UInt32 i = 0; i < pnts->size(); ++i)
    {
        OSG::Vec3f tmpVec(p[i * 3    ],
                          p[i * 3 + 1],
                          p[i * 3 + 2]);
        tmpVec[2] = 0.f;
        
        OSG::Real32 rDist = tmpVec.length();

        OSG::Real32 rBump = rIntensity * OSG::osgCos(rDist * rScale);

        p[i * 3 + 2] = rBump;

        rBump = rIntensity * (rScale/(rDist)) * -2 * -sin(rDist * rScale);

        tmpVec[0] = rBump * p[i * 3    ];
        tmpVec[1] = rBump * p[i * 3 + 1];
        tmpVec[2] = 1.f;
       
        tmpVec.normalize();

        n[i * 3    ] = tmpVec[0];
        n[i * 3 + 1] = tmpVec[1];
        n[i * 3 + 2] = tmpVec[2];
    }

    pnts ->unmapBuffer(pEnv);
    norms->unmapBuffer(pEnv);
}


void SimpleCudaComputeAlgorithm::updateGeoOSGCuda(
    OSG::HardwareContext *pContext, 
    OSG::DrawEnv         *pEnv    )
{
    if(_sfTarget.getValue() == NULL)
    {
        return;
    }

#ifdef OSG_WITH_CUDA
    GeoPnt3fProperty *pnts  = dynamic_cast<GeoPnt3fProperty *>(
        _sfTarget.getValue()->getProperty(Geometry::PositionsIndex));

    GeoVec3fProperty *norms = dynamic_cast<GeoVec3fProperty *>(
        _sfTarget.getValue()->getProperty(Geometry::NormalsIndex));

    OSG_ASSERT((pnts != NULL) && (norms != NULL));

#if 0
    rScale += rScaleStep;

    if(rScale > rScaleMax)
    {
        rScale = rScaleMax;
        rScaleStep *= -1;
    }

    if(rScale < 1)
    {
        rScale = 1;
        rScaleStep *= -1;
    }
#endif
    
    updateScale();

    Real32 rIntensity = _sfIntensity.getValue();
    Real32 rScale     = _sfScale    .getValue();

    cudaError_t cRes;
    OSG::Window *pWin  = pEnv->getWindow();

    pWin->validateGLObject(pnts->getGLId(), pEnv);

    OSG::UInt32 uiPntsVBOId = pWin->getGLObjectId(pnts->getGLId());

    cRes = cudaGLRegisterBufferObject(uiPntsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "RegisterRes %d\n", OSG::UInt32(cRes));
    }

    void *pMappedPoints = NULL;

    cRes = cudaGLMapBufferObject(&pMappedPoints, uiPntsVBOId);	 

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "MappedPtr, Res : %d\n", 
                OSG::UInt32(cRes));

    }


    pWin->validateGLObject(norms->getGLId(), pEnv);

    OSG::UInt32 uiNormsVBOId = pWin->getGLObjectId(norms->getGLId());

    cRes = cudaGLRegisterBufferObject(uiNormsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "RegisterRes %d\n", OSG::UInt32(cRes));
    }

    void *pMappedNormals = NULL;

    cRes = cudaGLMapBufferObject(&pMappedNormals, uiNormsVBOId);	 

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "MappedPtr, Res : %d\n", 
                OSG::UInt32(cRes));

    }

    OSGCUDA::simpleComputeUpdateGeo(rScale,
                                    rIntensity,
                                    pnts->size(),
                                    pMappedPoints,
                                    pMappedNormals);

    cRes = cudaGLUnmapBufferObject(uiPntsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnmapRes %d\n", OSG::UInt32(cRes));
    }

    cRes = cudaGLUnregisterBufferObject(uiPntsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnRegisterRes %d\n", OSG::UInt32(cRes));
    }


    cRes = cudaGLUnmapBufferObject(uiNormsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnmapRes %d\n", OSG::UInt32(cRes));
    }

    cRes = cudaGLUnregisterBufferObject(uiNormsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnRegisterRes %d\n", OSG::UInt32(cRes));
    }
#endif
}

OSG_END_NAMESPACE
