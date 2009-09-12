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

#ifndef _OSGRTRAYSIMD_H_
#define _OSGRTRAYSIMD_H_
#ifdef __sgi
#pragma once
#endif

#include <OSGConfig.h>

//#undef OSG_SIMD_SSE

#ifdef OSG_SIMD_SSE
#  define __inline inline
#  include <xmmintrin.h>
#endif

OSG_BEGIN_NAMESPACE

#ifdef OSG_HAS_POSIXMEMALIGN
template<typename _Tp>
class AlignedAllocator
{
  public:

    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;

    typedef       _Tp *pointer;
    typedef const _Tp *const_pointer;
    typedef       _Tp &reference;
    typedef const _Tp &const_reference;
    typedef       _Tp  value_type;
    
    /*---------------------------------------------------------------------*/

    template<typename _Tp1>
    struct rebind
    { 
        typedef AlignedAllocator<_Tp1> other; 
    };
    
    /*---------------------------------------------------------------------*/

    AlignedAllocator(void) throw();
    
    AlignedAllocator(const AlignedAllocator &) throw();
    
    template<typename _Tp1>
    AlignedAllocator(const AlignedAllocator<_Tp1> &) throw();
    
    ~AlignedAllocator(void) throw();
    
    /*---------------------------------------------------------------------*/

    pointer       address(      reference __x) const;
    const_pointer address(const_reference __x) const;

    /*---------------------------------------------------------------------*/

    pointer allocate  (size_type __n, const void      * = 0);
    void    deallocate(pointer   __p,       size_type      );

    /*---------------------------------------------------------------------*/

    size_type max_size(void) const throw();

    /*---------------------------------------------------------------------*/

    void  construct(pointer __p, const _Tp &__val);
    void  destroy  (pointer __p                  );
};
#endif



#ifdef OSG_SIMD_SSE
typedef __m128 Float4;
#else
struct Float4
{
    union 
    {
        Real32 data [4];
        UInt32 idata[4];
    };
};
#endif

struct SSEVec4 
{
    typedef Real32 Real32_4[4];

    union
    {
        Float4    _data [3];
        Real32_4  _dataA[3];
    };
};

Float4 osgSIMDMadd  (const Float4 vM1, const Float4 vM2, const Float4 vS);
Float4 osgSIMDMul   (const Float4 v1,  const Float4 v2);
Float4 osgSIMDAdd   (const Float4 v1,  const Float4 v2);
Float4 osgSIMDSub   (const Float4 v1,  const Float4 v2);

Float4 osgSIMDAnd   (const Float4 v1,  const Float4 v2);
Float4 osgSIMDOr    (const Float4 v1,  const Float4 v2);

Float4 osgSIMDAndNot(const Float4 v1,  const Float4 v2);

Float4 osgSIMDMax   (const Float4 v1,  const Float4 v2);
Float4 osgSIMDMin   (const Float4 v1,  const Float4 v2);

Float4 osgSIMDCmpGT (const Float4 v1, const Float4 v2);
Float4 osgSIMDCmpGE (const Float4 v1, const Float4 v2);

Float4 osgSIMDCmpLT (const Float4 v1, const Float4 v2);
Float4 osgSIMDCmpLE (const Float4 v1, const Float4 v2);

Float4 osgSIMDRSqrtE(const Float4 v);
Float4 osgSIMDInvert(const Float4 v);

Int32  osgSIMDMoveMask(const Float4 v);

Float4 osgSIMDSet(const Real32 rVal);
Float4 osgSIMDSet(const Real32 rVal0,
                  const Real32 rVal1,
                  const Real32 rVal2,
                  const Real32 rVal3);

Float4 osgSIMDUpdate(const Float4 mask, const Float4 v1, const Float4 v2);

typedef std::numeric_limits<Real32> Real32Limits;

static const Float4 SIMDZero     = osgSIMDSet(0.f);
static const Float4 SIMDOne      = osgSIMDSet(1.f);
static const Float4 SIMDEps      = osgSIMDSet(0.00001);
static const Float4 SIMDInfinity = osgSIMDSet(Real32Limits::max());

OSG_END_NAMESPACE

#include "OSGRTSIMD.inl"

#ifdef OSG_SIMD_SSE
#include "OSGRTSIMD_SSE.inl"
#else
#include "OSGRTSIMD_FPU.inl"
#endif

#endif
