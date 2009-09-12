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

#ifndef _OSGSPURTTRACEXCACHEKD_H_
#define _OSGSPURTTRACEXCACHEKD_H_
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


void intersectXRayEnvelope(UInt32 envId, 
			  const UInt16 uiCache,
			  const UInt32 mfc_tag_id);

void intersectXRayEnvelopeDoubleBuffered(rayEnvelope_t* rayEnvelope,
					rayInfoEnvelope_t* rayInfoEnvelope,
					hitEnvelope_t* hitEnvelope,
					const UInt16 uiCache,
					const UInt32 mfc_tag_id);

void intersectXRayTileSIMD(fourHitPacket_t* fourHitPacket,
			  rayTile_t* rayTile,
			  rayInfoTile_t* rayInfoTile,
			  const UInt16 uiCache,
			  const UInt32 mfc_tag_id);

void intersectXRayTileScalar(fourHitPacket_t* fourHitPacket,
			    rayTile_t* rayTile,
			    rayInfoTile_t* rayInfoTile,
			    const UInt16 uiCache,
			    const UInt32 mfc_tag_id);


void updateXKDNodeCache(const UInt32 tag_id, const UInt32 uiCache);


vec_float4 updateEntryParams(rayTile_t* rayTile,
			     vec_uint4 currNodeLSA,
			     cacheKDNode_t* travNode);

// Funtions should be called after every change/update of KD Node cache
void updateBoxVolumeCache(UInt32 tag_id, UInt32 cacheId);

void printXCache(UInt32 cacheId);

//extern volatile triangleData_t *triangleAccel;

//extern volatile boxVolume_t* boxVolume;

#endif
