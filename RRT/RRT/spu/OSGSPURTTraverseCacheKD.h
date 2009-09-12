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

#ifndef _OSGSPURTTRACECACHEKD_H_
#define _OSGSPURTTRACECACHEKD_H_
#ifdef __sgi
#pragma once
#endif

#include "spu_intrinsics.h"
#include <spu_mfcio.h>

#include "OSGCellRT.h"
#include "OSGSPURTConstants.h"
#include "OSGSPURTCompositeIntrinsics.h"
#include "OSGCellRTMFCHelper.h"
#include "OSGSPURTMain.h"




void intersectRayEnvelope(UInt32 envId, 
			  const UInt16 uiCache,
			  const UInt32 mfc_tag_id);

void intersectRayEnvelopeDoubleBuffered(rayEnvelope_t* rayEnvelope,
					rayInfoEnvelope_t* rayInfoEnvelope,
					hitEnvelope_t* hitEnvelope,
					const UInt16 uiCache,
					const UInt32 mfc_tag_id);

void intersectRayTileSIMD(fourHitPacket_t* fourHitPacket,
			  rayTile_t* rayTile,
			  rayInfoTile_t* rayInfoTile,
			  const UInt16 uiCache,
			  const UInt32 mfc_tag_id);

void intersectRayTileScalar(fourHitPacket_t* fourHitPacket,
			    rayTile_t* rayTile,
			    rayInfoTile_t* rayInfoTile,
			    const UInt16 uiCache,
			    const UInt32 mfc_tag_id);

void printCache(void);

typedef struct _KDSIMDStackElem 
{
  union
  {
    vec_float4     tmin4;
    Real32         tminA[4];
  };

  union
  {
    vec_float4     tmax4;
    Real32         tmaxA[4];
  };

  union
  {
    cacheKDNode_t *node;
    UInt32   addr;
  };

  UInt32 uiNodeCacheStIdx;
  UInt32 uiLeafNodeStIdx;

  UInt32 pad[3];

}KDSIMDStackElem_t;


// Not actually a Stack. Fixed size to prevent LS Overflow.
// Traversal will be aborted if attempt to place more than allowed elements in array
#define kDStackSize 16
KDSIMDStackElem_t sKDToDoStack[kDStackSize];


//extern volatile triangleData_t *triangleAccel;

//extern volatile boxVolume_t* boxVolume;

#endif
