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

#include "OSGBufferInterpolator.cuh"

OSGCUDA_BEGIN_NAMESPACE

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

OSGCUDA_END_NAMESPACE
