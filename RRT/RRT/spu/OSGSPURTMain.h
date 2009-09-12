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

#ifndef _OSGSPURTNEWMAIN_H_
#define _OSGSPURTNEWMAIN_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGCellRT.h"

#include <malloc_align.h>
#include <calloc_align.h>
#include <realloc_align.h>
#include <free_align.h>


#define DOUBLEBUFFER

#define TRUE   1
#define FALSE  0


volatile envelopeInfo_t *envelopeInfo;

volatile rayEnvelope_t     *rayEnvelope;

volatile rayInfoEnvelope_t *rayInfoEnvelope;

volatile hitEnvelope_t     *hitEnvelope;

volatile triangleData_t* triangleAccel;

volatile raySetupBase_t *raySetupBase;

volatile cacheKDNode_t* cacheKDNode;

volatile control_block_t cb __attribute__ ((aligned (128)));

volatile cacheInfoPack_t* cacheInfoPack;

volatile boxVolume_t* rootBoxVolume;

volatile boxVolume_t* boxVolumeCache;

volatile UInt32* primIndexArray;

UInt32 gUiNodeLS;    // number of nodes in LS
UInt32 gUiNodeCacheStIdx;  // main memory element index of NodeCache for first node LS  

UInt32 gUiLeafNodeStIdx; // main memory element index for first leafNode in LS


#endif
