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

#ifndef _OSGSPURTCONSTANTS_H_
#define _OSGSPURTCONSTANTS_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGCellRT.h"
#include "float.h"

static const vec_float4 SIMDZero     = {0.0f, 0.0f, 0.0f, 0.0f};
static const vec_float4 SIMDHalf     = {0.5f, 0.5f, 0.5f, 0.5f};
static const vec_float4 SIMDOne      = {1.0f, 1.0f, 1.0f, 1.0f};
static const vec_float4 SIMDThree    = {3.0f, 3.0f, 3.0f, 3.0f};
static const vec_float4 SIMDInfinity = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
static const vec_float4 SIMDEps      = {0.00001, 0.00001, 0.00001, 0.00001};

static const enum axis{X = 0, Y, Z} Axis;

static const UInt32 nodeLimitLS = 100;
static const UInt32 triLimitLS = 24;    // multiple of 4


#endif
