/*---------------------------------------------------------------------------* \
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

#ifndef _OSGCRTPPU_H_
#define _OSGCRTPPU_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGCellRT.h"

#if defined(__PPU__) || defined(__SPU__)
#include <libspe2.h>
#include <pthread.h>
#include <errno.h>
#include <altivec.h>
#include <malloc_align.h>
#include <free_align.h>

struct ppu_pthread_data 
{
    spe_context_ptr_t  speid;
    pthread_t          pthread;
    void              *argp;
};

typedef struct ppu_pthread_data ppu_pthread_data_t;

// forced to exist 6 controlblocks due to envelopeInfo , fix later
control_block_t cb[SPE_THREADS] __attribute__ ((aligned (128)));


/* this is the pointer to the SPE code, to be used at thread creation time */
extern spe_program_handle_t rrt_spu;

/* these are the handles returned by "spe_context_create"
   and "pthread_create" 
*/

ppu_pthread_data_t datas[SPE_THREADS];

#endif


#endif

