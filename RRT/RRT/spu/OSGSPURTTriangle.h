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

#ifndef _OSGSPURTTRIANGLE_H_
#define _OSGSPURTTRIANGLE_H_
#ifdef __sgi
#pragma once
#endif

#include "spu_intrinsics.h"
#include <spu_mfcio.h>

#include "OSGSPURTCompositeIntrinsics.h"
#include "OSGSPURTConstants.h"
#include "OSGCellRT.h"
#include "OSGSPURTMain.h"

#define KU aMod[triangle->uiProj + 1]
#define KV aMod[triangle->uiProj + 2]



void intersectTriangleSIMD(fourHitPacket_t* fourHitPacket,
			   rayTile_t* rayTile,
			   UInt32 uiCacheId,
			   UInt16 uiActive,
			   UInt32 nPrimitives,
			   UInt32* prims,
			   UInt32 mfcTagId,
			   EA_t eaTriCache);

void intersectSingleTriangleSIMD(fourHitPacket_t* fourHitPacket,
				 rayTile_t* rayTile,
				 UInt32 uiCacheId,
				 UInt16 uiActive,
				 triangleData_t* triangle);


void intersectTriangleScalar(fourHitPacket_t* fourHitPacket,
			     rayTile_t* rayTile,
			     UInt32 uiCacheId,
			     UInt16 uiRay,
			     UInt32 nPrimitives,
			     UInt32* prims,
			     UInt32 mfcTagId,
			     EA_t eaTriCache);

void intersectSingleTriangleScalar(fourHitPacket_t* fourHitPacket,
				   rayTile_t* rayTile,
				   UInt32 uiCacheId,
				   UInt16 uiRay,
				   triangleData_t* triangle);


void updateTriangleCache(UInt32 nPrimitives, 
			 UInt32* prims, 
			 UInt32 mfcTagId,
			 EA_t eaTriCache); 

//NOT COMPLETED
void updateTriangleCacheSIMD(UInt32 nPrimitives, 
			 UInt32* prims, 
			 UInt32 mfcTagId,
			 EA_t eaTriCache); 
		       
void resetTriangleCache(void);

void resetTriangleCacheSIMD(void);

#endif
