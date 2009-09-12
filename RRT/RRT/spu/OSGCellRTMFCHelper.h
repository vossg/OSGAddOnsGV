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

#ifndef _OSGCELLRTMFCHELPER_H_
#define _OSGCELLRTMFCHELPER_H_
#ifdef __sgi
#pragma once
#endif

//#include <stdio.h>
#include <spu_mfcio.h>

#include "OSGCellRT.h"


void mfc_checkDMAParams(UInt64 lsa, UInt64 ea, UInt32 size);



void mfc_getIn16kBatches(volatile void *lsa, 
			 UInt64 ea, 
			 UInt32 size, 
			 UInt32 tag, 
			 UInt32 tid,
			 UInt32 rid);



void mfc_putIn16kBatches(volatile void *lsa, 
			 UInt64 ea, 
			 UInt32 size, 
			 UInt32 tag, 
			 UInt32 tid, 
			 UInt32 rid);


#endif
