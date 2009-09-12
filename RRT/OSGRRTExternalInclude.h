/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

#ifndef _OSGRRTEXTERNALINCLUDE_H_
#define _OSGRRTEXTERNALINCLUDE_H_
#ifdef __sgi
#pragma once
#endif

// Just if you want to use some pieces outside this dir use this include.
// As there are a lot of forward declaration it can be hard to guess the
// correct include order from scratch ;-)

#include "OSGTriangleIterator.h"
#include "OSGRTInfoAttachment.h"
#include "OSGOSGWriter.h"
#include "OSGRTTarget.h"
#include "OSGRayTracerInst.h"
#include "OSGRTRaySIMDPacket.h"
#include "OSGRTHitSIMDPacket.h"
#include "OSGRTTriangleAccel.h"
#include "OSGRTCacheKD.h"
#include "OSGRTInitAction.h"
#include "OSGRRTStage.h"
#include "OSGRTCacheAttachmentInst.h"

#endif
