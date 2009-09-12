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


#ifndef _OSGSPURTCOMPOSITEINTRINSICS_H_
#define _OSGSPURTCOMPOSITEINTRINSICS_H_
#ifdef __sgi
#pragma once
#endif


#include "spu_intrinsics.h"

static inline int spu_c_movemask_ui(const vector unsigned int v) __attribute__((__always_inline__));

static inline int spu_c_movemask(const vector float v) __attribute__((__always_inline__));

static inline int spu_c_isum(const vector signed int v);

static inline float spu_c_fsum(const vector float v) __attribute__((__always_inline__));

static inline vector float spu_c_invert(const vector float v) __attribute__((__always_inline__));

static inline vector float spu_c_update(const vector float mask, 
					const vector float v1, 
					const vector float v2) __attribute__((__always_inline__));

static inline vector float spu_c_update_ui(const vector unsigned int mask, 
						  const vector float v1, 
						  const vector float v2) __attribute__((__always_inline__));

static inline vector unsigned int spu_c_cmple(vector float v1, 
					      vector float v2) __attribute__((__always_inline__));

static inline vector float spu_c_min(const vector float v1, 
				     const vector float v2) __attribute__((__always_inline__));

static inline vector float spu_c_max(const vector float v1, 
				     const vector float v2) __attribute__((__always_inline__));


// ------------------------------------------------------------------------------------------------

static inline
int spu_c_movemask_ui(const vector unsigned int v)
{
  vector unsigned int cmp = spu_cmpgt(((vector signed int) {0, 0, 0, 0}), (vector signed int) v);
  vector unsigned int res = spu_and(cmp, ((vector unsigned int){1<<0, 1<<1, 1<<2, 1<<3}));
  //return spu_c_isum((vector signed int)res);
  return (int)spu_extract(res,0)+spu_extract(res,1)+spu_extract(res,2)+spu_extract(res,3);
}

static inline
int spu_c_movemask(const vector float v)
{
  vector unsigned int cmp = spu_cmpgt(((vector signed int) {0, 0, 0, 0}), (vector signed int) v);
  vector unsigned int res = spu_and(cmp, ((vector unsigned int){1<<0, 1<<1, 1<<2, 1<<3}));
  //return spu_c_isum((vector signed int)res);
  return (int)spu_extract(res,0)+spu_extract(res,1)+spu_extract(res,2)+spu_extract(res,3);
}

static inline
int spu_c_isum(const vector signed int v) 
{
  return spu_extract(v,0)+spu_extract(v,1)+spu_extract(v,2)+spu_extract(v,3);
}

static inline
float spu_c_fsum(const vector float v)
{
  return spu_extract(v,0)+spu_extract(v,1)+spu_extract(v,2)+spu_extract(v,3);
}

static inline
vector float spu_c_invert(const vector float v)
{
  vector float re = spu_re(v);
  return spu_sub(spu_add(re, re), spu_mul(spu_mul(re, re), v));
}

static inline
vector float spu_c_update(const vector float mask, const vector float v1, const vector float v2)
{
  vector float res = spu_or(spu_and(v1, mask), spu_andc(v2, mask));
  return res;
}

static inline
vector float spu_c_update_ui(const vector unsigned int mask, const vector float v1, const vector float v2)
{
  //  const vector float zero = {0, 0, 0, 0};

  //   vector float tmp1 = spu_sel(zero, v1, mask);
  //   vector float tmp2 = spu_sel(zero, v2, mask);

  //   vector float res = spu_or(tmp1, spu_nor(tmp2, zero));

  vector float* pMask = &mask;
  vector float res = spu_or(spu_and(v1, *pMask), spu_andc(v2, *pMask));
  return res;
}

static inline
vector unsigned int spu_c_cmple(vector float v1, vector float v2)
{
  return spu_or(spu_cmpgt(v2, v1), spu_cmpeq(v1, v2));
}

static inline
vector float spu_c_min(const vector float v1, const vector float v2)
{
  return spu_sel(v1, v2, spu_cmpgt(v1,v2));
}

static inline
vector float spu_c_max(const vector float v1, const vector float v2)
{
  return spu_sel(v2, v1, spu_cmpgt(v1, v2));
}

#endif
