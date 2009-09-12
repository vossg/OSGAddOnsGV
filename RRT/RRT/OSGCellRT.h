/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2008 by the OpenSG Forum                   *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 *   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------* \
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

#ifndef _OSGCELLRT_H_
#define _OSGCELLRT_H_
#ifdef __sgi
#pragma once
#endif

#include <stdlib.h>
#include <stdio.h>

#ifdef __PPU__
#include <vec_types.h>
#endif
#ifdef __SPU__
#include <spu_intrinsics.h>
#endif

#ifndef __SPU__
#include "OSGBaseTypes.h"
#endif

//#define NDEBUG  // uncomment to turn  assert statements off

// The SPEs request envelopes from the PPE insted of predefined envelopes//
//#define DYNAMIC_ENV_ASSIGNMENT

#define SPE_THREADS 6 

#if defined(__SPU__) || defined(__PPU__)
//#define OSG_XCACHEKD
#endif

//#define SINGLEFRAME

#define intersectBatchCount 1

#define envelopeTilesY  1
#define envelopeTilesX  1

// ----------------------------------------------------------------------------

#ifdef __SPU__
typedef unsigned short int         UInt16;
typedef unsigned int               UInt32;
typedef unsigned long long         UInt64;
typedef int                        Int32;

typedef float                      Real32;
typedef double                     Real64;
#else
using OSG::UInt16;
using OSG::UInt32;
using OSG::UInt64;
using OSG::Int32;

using OSG::Real32;
using OSG::Real64;
#endif

typedef Real32                     Real32_4[4];

typedef UInt64                     EA_t;
typedef UInt32                     LSA_t;

// ----------------------------------------------------------------------------

static const UInt32 maxNodesInCache = 256;
static const UInt32 maxPrimInNode   = 16; // multiple of 4


#ifdef OSG_XCACHEKD
enum boundingBoxSide{right = 0, left, top, bottom, front, back};
#endif

// ----------------------------------------------------------------------------

typedef struct _control_block
{
    UInt32 speId;
    UInt32 uiCaches;
    UInt32 uiEnvelopes;
    UInt32 uiTargetWidth;
    UInt32 uiTargetHeight;

    EA_t   eaRayBase;
    EA_t   eaRayInfoBase;
    EA_t   eaHitBase;

    EA_t   eaRayEnvelope;
    EA_t   eaRayInfoEnvelope;
    EA_t   eaPrimRaySetupPack;
    EA_t   eaEnvelopeInfo;
    EA_t   eaCacheInfoPack;
    EA_t   eaCacheBoxVolume;
    EA_t   eaHitEnvelope;

    UInt64 pad[3];   // pad to a full cache line (128 bytes) 
} control_block_t;


typedef struct _envelopeInfo
{
    UInt32 id;
    UInt32 topX;
    UInt32 topY;
    UInt32 pad;
} envelopeInfo_t;


#if defined(__PPU__) || defined(__SPU__)

typedef struct _rayTile
{
    union
    {
        vec_float4  fOrigin;
        Real32      vOriginA[4];
    };

    union
    {
        vec_float4 fDir [3];
        Real32_4   vDirA[3];
    };
} rayTile_t;


typedef struct _rayTileEntryPoint
{
    union
    {
        vec_float4 vEntryPoint    [3];
        Real32_4   arrayEntryPoint[3];
    };
} rayTileEntryPoint_t;


typedef struct _ray_envelope_small
{
    rayTile_t tile[envelopeTilesY][envelopeTilesX];
} rayEnvelope_t;

#endif

typedef struct _rayInfoTile
{
    UInt32 uiX;
    UInt32 uiY;
    UInt16 bIsActive[4];
} rayInfoTile_t;


typedef struct _rayInfoEnvelope
{
    rayInfoTile_t tile[envelopeTilesY][envelopeTilesX];
} rayInfoEnvelope_t;


#if defined(__PPU__) || defined(__SPU__)

typedef struct _raySetupBase
{
    vec_float4 vRight;
    vec_float4 vUp;
    vec_float4 vOrigin;
    vec_float4 vMisc;
} raySetupBase_t;


typedef struct _fourHitPacket
{
    UInt32     uiX;
    UInt32     uiY;

    EA_t       eaRayPacket;

    vec_float4 rDist;
    vec_float4 rU;
    vec_float4 rV;
  
    vec_uint4  uiCacheId;
    vec_uint4  uiObjId;
    vec_uint4  uiTriId;

    vec_uint4  pad;

  //  Real32 rDist[4];
  //  Real32 rU[4];
  //  Real32 rV[4];
  
  //  UInt32 uiCacheId[4];
  //  UInt32 uiObjId[4];
  //  UInt32 uiTriId[4];
  //  UInt32 pad[4];
} fourHitPacket_t;


typedef struct _hitEnvelope
{
    fourHitPacket_t tile[envelopeTilesY][envelopeTilesY];
} hitEnvelope_t;

#endif


typedef struct _triangle_data
{
    Real32 nU, nV, nD, bNU, bNV, bD, cNU, cNV, cD;
    UInt32 uiProj, uiTriId, uiObjId; 
} triangleData_t;


typedef struct _boxVolume
{
    Real32 min[3];
    Real32 max[3];
    Real32 pad[2];
} boxVolume_t;


#ifdef OSG_XCACHEKD

typedef struct xCacheKDNode
{
    union 
    {
        UInt32 uiFlags;     // Leave + Interior
        Real32 fSplitPos;   // Interior
        UInt32 uiNumPrims;  // Leaf
    };
    
    union 
    {
        UInt32  uiAboveChild;      // Interior
        UInt32  uiSinglePrimitive; // Not in use, only declared to make 
                                   // swap between both caches simpler
        UInt32  pPrimitiveIdx;     // Leaf
    };

    Int32 ropeOffset[6];
} cacheKDNode_t;


typedef struct _KDCacheInfoPack
{
    UInt32 uiNodes;
    UInt32 uiTriangles;

    EA_t   eaKDNodeCache;
    EA_t   eaPrimIndexArray;  // address to start of array, 
                              // use offset node*MaxPrimInNode 
    EA_t   eaTriCache;
    EA_t   eaBoxCache;

    UInt32 pad[2];
} cacheInfoPack_t;

#else

typedef struct cacheKDNode
{
    union 
    {
        UInt32 uiFlags;     // Leaf + Interior
        Real32 fSplitPos;   // Interior
        UInt32 uiNumPrims;  // Leaf
    };

    union 
    {
        UInt32 uiAboveChild;      // Interior
        UInt32 uiSinglePrimitive;
        UInt32 pPrimitiveIdx;     // Leaf
    };
} cacheKDNode_t;


typedef struct _KDCacheInfoPack
{
    UInt32 uiNodes;
    UInt32 uiTriangles;

    EA_t   eaKDNodeCache;
    EA_t   eaPrimIndexArray;  // address to start of array, 
                              // use offset node*MaxPrimInNode 
    EA_t   eaTriCache;
} cacheInfoPack_t;

#endif

#if defined(__PPU__) || defined(__SPU__)

typedef struct _customVec4 
{
    union
    {
        vec_float4 data [3];
        Real32_4   dataA[3];
    };
} customVec4_t;

#endif

typedef struct _data3f   
{
    Real32 x,y,z;
} data3f_t;

#endif
