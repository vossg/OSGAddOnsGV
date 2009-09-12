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

#ifndef _OSGSPURTRAYSETUP_H_
#define _OSGSPURTRAYSETUP_H_
#ifdef __sgi
#pragma once
#endif

#include <stdio.h>
#include <spu_mfcio.h>
#include <assert.h>

#include "OSGCellRT.h"
#include "OSGSPURTConstants.h"
#include "OSGSPURTMain.h"

//extern volatile control_block_t cb __attribute__ ((aligned (128)));

// extern volatile envelopeInfo_t *envelopeInfo;
// extern volatile rayEnvelope_t     *rayEnvelope;
// extern volatile rayInfoEnvelope_t *rayInfoEnvelope;

// extern volatile raySetupBase_t *raySetupBase;

void setupPrimaryRays(UInt32 tag_id, const UInt32 batchCount);
void setupPrimaryRaysDB(UInt32 tag_id, const UInt32 batchCount);

vec_float4 initPrimaryRaySetup(UInt32 tag_id);


void setupSingleEnvelope(rayEnvelope_t* rayEnvelope,
			 rayInfoEnvelope_t* rayInfoEnvelope,
			 UInt32 topX, 
			 UInt32 topY, 
			 vec_float4 vTopLeft);

void setupSingleEnvelopeSIMD(rayEnvelope_t* rayEnvelope,
			     rayInfoEnvelope_t* rayInfoEnvelope,
			     UInt32 topX, 
			     UInt32 topY, 
			     vec_float4 vTopLeft);

#endif
