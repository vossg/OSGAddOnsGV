/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                 Copyright (C) 2003 by the OpenSG Forum                    *
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

#include "OSGSimpleCompute.cuh"

OSGCUDA_BEGIN_NAMESPACE

#if 0

__global__ 
void linearBufferInterpolate(const float *pInLower,
                             const float *pInUpper,
                             const float  t,
                                   int    numElements,
                                   float *pResult    )
{
    const int        tid = blockDim.x * blockIdx.x + threadIdx.x;
    const int numThreads = blockDim.x * gridDim.x;

    float tmp = 0.f;

    for(int pos = tid; pos < numElements; pos += numThreads)
    {
        tmp = pInUpper[pos] - pInLower[pos];

        pResult[pos] = pInLower[pos] + (t * tmp);
    }
}


void lerpFloat(const float *pInLower,
               const float *pInUpper,
               const float  t,
                     int    numElements,
                     float *pResult)

{
    linearBufferInterpolate<<<64, 256>>>(pInLower,
                                         pInUpper,
                                         t,
                                         numElements,
                                         pResult);
}
#endif

struct CudaFloat3 
{
    float x, y, z;
};

__global__ 
void simpleComputeUpdateGeoCuda(float         rScale,
                                float         rIntensity,
                                unsigned int  uiNumPnts,
                                CudaFloat3   *pMappedPoints,
                                CudaFloat3   *pMappedNormals)
{
    const int        tid = blockDim.x * blockIdx.x + threadIdx.x;
    const int numThreads = blockDim.x * gridDim.x;

    for(int pos = tid; pos < uiNumPnts; pos += numThreads)
    {
        float rDist = sqrt((pMappedPoints[pos].x * pMappedPoints[pos].x) + 
                           (pMappedPoints[pos].y * pMappedPoints[pos].y));

        float rBump = rIntensity * cos(rDist * rScale);

        pMappedPoints[pos].z = rBump;


        rBump = rIntensity * (rScale/(rDist)) * -2 * -sin(rDist * rScale);

        float tmp[3];

        tmp[0] = rBump * pMappedPoints[pos].x;
        tmp[1] = rBump * pMappedPoints[pos].y;
        tmp[2] = 1.f;

        rDist = 1.f / sqrt(tmp[0] * tmp[0] + tmp[1] * tmp[1] + tmp[2] * tmp[2]);

        pMappedNormals[pos].x = tmp[0] * rDist;
        pMappedNormals[pos].y = tmp[1] * rDist;
        pMappedNormals[pos].z = tmp[2] * rDist;
    }
}

void simpleComputeUpdateGeo(float         rScale,
                            float         rIntensity,
                            unsigned int  uiNumPnts,
                            void         *pMappedPoints,
                            void         *pMappedNormals)
{
    CudaFloat3 *pPnts  = static_cast<CudaFloat3 *>(pMappedPoints );
    CudaFloat3 *pNorms = static_cast<CudaFloat3 *>(pMappedNormals);

    simpleComputeUpdateGeoCuda<<<64, 256>>>(rScale,
                                            rIntensity,
                                            uiNumPnts,
                                            pPnts,
                                            pNorms);
}

OSGCUDA_END_NAMESPACE
