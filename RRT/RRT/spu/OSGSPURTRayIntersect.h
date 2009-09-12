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

#ifndef _OSGSPURTRAYINTERSECT_H_
#define _OSGSPURTRAYINTERSECT_H_
#ifdef __sgi
#pragma once
#endif

#include <stdio.h>
#include <spu_mfcio.h>
#include <assert.h>

#include "OSGCellRT.h"
#include "OSGSPURTConstants.h"

#include "OSGSPURTMain.h"
#include "OSGSPURTHitEnvelope.h"

#ifdef OSG_XCACHEKD
#include "OSGSPURTTraverseXCacheKD.h"
#else
#include "OSGSPURTTraverseCacheKD.h"
#endif

extern volatile control_block_t cb __attribute__ ((aligned (128)));

extern volatile rayEnvelope_t     *rayEnvelope;
extern volatile rayInfoEnvelope_t *rayInfoEnvelope;
extern volatile hitEnvelope_t     *hitEnvelope;
extern volatile triangleData_t* triangleAccel;

extern volatile cacheInfoPack_t* cacheInfoPack;
extern volatile boxVolume_t* boxVolume;

void intersectPrimaryRays(UInt32 tag_id, const UInt32 batchCount);

void intersectPrimaryRaysDynamic(UInt32 tag_id, const UInt32 batchCount);

void initPrimaryRayIntersect(UInt32 tag_id);

void rebaseKDNodeCache(const UInt32 tag_id, const UInt32 uiCache);

void initPrimIndexCache(const UInt32 tag_id, const UInt32 uiCache);

// Funtion should be called after every change/update of KD Node cache
void rebasePrimIndexCache(const UInt32 tag_id, const UInt32 uiCache);

EA_t gEaStIndexArray;

#endif
