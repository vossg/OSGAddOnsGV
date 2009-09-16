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

#include <OSGBaseTypes.h>

#include <vector>

//#undef OSG_SIMD_SSE
//#undef OSG_DEBUG_SIMD
//#undef OSG_SIMD_ALTIVEC

#if defined(OSG_SIMD_SSE)
#  define __inline inline
#  include <xmmintrin.h>
#elif defined(OSG_SIMD_ALTIVEC)
#  define __inline inline
#  include <altivec.h>
#endif

#ifndef WIN32
#define OSGRTNU __attribute__((unused))
#else
#define OSGRTNU
#endif

OSG_BEGIN_NAMESPACE

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


#if defined(OSG_SIMD_SSE)
typedef __m128 Float4;
#elif defined(OSG_SIMD_ALTIVEC)
typedef vector float Float4;
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

Float4 osgSIMDSet(const UInt32 uiVal);

Float4 osgSIMDSet(const UInt32 uiVal0,
                  const UInt32 uiVal1,
                  const UInt32 uiVal2,
                  const UInt32 uiVal3);

#ifdef OSG_DEBUG_OLD_C_CASTS

#ifdef __OPTIMIZE__
extern __inline __m128 __attribute__((__gnu_inline__, __always_inline__, __artificial__))
_osg_mm_shuffle_ps (__m128 __A, __m128 __B, int const __mask)
{
  return __m128(__builtin_ia32_shufps (__v4sf(__A), __v4sf(__B), __mask));
}
#else
#define _osg_mm_shuffle_ps(A, B, MASK)                                 \
    (__m128(__builtin_ia32_shufps (__v4sf(__m128((A))),                \
                                   __v4sf(__m128((B))), int(MASK))))
#endif

#define osgSIMDSplat(v, i) \
    _osg_mm_shuffle_ps((v), (v), _MM_SHUFFLE((i), (i), (i), (i)))

#else

#define osgSIMDSplat(v, i) \
    _mm_shuffle_ps((v), (v), _MM_SHUFFLE((i), (i), (i), (i)))

#endif


Float4 osgSIMDUpdate(const Float4 mask, const Float4 v1, const Float4 v2);

void osgSIMDDump(const Float4 v1);

typedef std::numeric_limits<Real32> Real32Limits;

static const Float4 SIMDZero     OSGRTNU = osgSIMDSet( 0.0f);
static const Float4 SIMDHalf     OSGRTNU = osgSIMDSet( 0.5f);
static const Float4 SIMDOne      OSGRTNU = osgSIMDSet( 1.0f);
static const Float4 SIMDMinusOne OSGRTNU = osgSIMDSet(-1.0f);
static const Float4 SIMDThree    OSGRTNU = osgSIMDSet( 3.0f);
static const Float4 SIMDEps      OSGRTNU = osgSIMDSet( 0.00001f);
static const Float4 SIMDInfinity OSGRTNU = osgSIMDSet(Real32Limits::max());

#if 0
static const Float4 SIMDMaskTranslation[16] = 
{
    osgSIMDSet(0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u),
    osgSIMDSet(0x00000000u, 0x00000000u, 0x00000000u, 0xFFFFFFFFu),

    osgSIMDSet(0x00000000u, 0x00000000u, 0xFFFFFFFFu, 0x00000000u),
    osgSIMDSet(0x00000000u, 0x00000000u, 0xFFFFFFFFu, 0xFFFFFFFFu),

    osgSIMDSet(0x00000000u, 0xFFFFFFFFu, 0x00000000u, 0x00000000u),
    osgSIMDSet(0x00000000u, 0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu),
    osgSIMDSet(0x00000000u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u),
    osgSIMDSet(0x00000000u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu),

    osgSIMDSet(0xFFFFFFFFu, 0x00000000u, 0x00000000u, 0x00000000u),
    osgSIMDSet(0xFFFFFFFFu, 0x00000000u, 0x00000000u, 0xFFFFFFFFu),

    osgSIMDSet(0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu, 0x00000000u),
    osgSIMDSet(0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu, 0xFFFFFFFFu),

    osgSIMDSet(0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u, 0x00000000u),
    osgSIMDSet(0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu),
    osgSIMDSet(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u),
    osgSIMDSet(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu)
};
#else
static const Float4 SIMDMaskTranslation[16] OSGRTNU = 
{
    osgSIMDSet(0x00000000u, 0x00000000u, 0x00000000u, 0x00000000u),
    osgSIMDSet(0xFFFFFFFFu, 0x00000000u, 0x00000000u, 0x00000000u),

    osgSIMDSet(0x00000000u, 0xFFFFFFFFu, 0x00000000u, 0x00000000u),
    osgSIMDSet(0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u, 0x00000000u),

    osgSIMDSet(0x00000000u, 0x00000000u, 0xFFFFFFFFu, 0x00000000u),
    osgSIMDSet(0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu, 0x00000000u),
    osgSIMDSet(0x00000000u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u),
    osgSIMDSet(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u),

    osgSIMDSet(0x00000000u, 0x00000000u, 0x00000000u, 0xFFFFFFFFu),
    osgSIMDSet(0xFFFFFFFFu, 0x00000000u, 0x00000000u, 0xFFFFFFFFu),

    osgSIMDSet(0x00000000u, 0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu),
    osgSIMDSet(0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu),

    osgSIMDSet(0x00000000u, 0x00000000u, 0xFFFFFFFFu, 0xFFFFFFFFu),
    osgSIMDSet(0xFFFFFFFFu, 0x00000000u, 0xFFFFFFFFu, 0xFFFFFFFFu),
    osgSIMDSet(0x00000000u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu),
    osgSIMDSet(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu)
};
#endif

OSG_END_NAMESPACE

#include "OSGRTSIMD.inl"

#ifndef OSG_DEBUG_SIMD

#if defined(OSG_SIMD_SSE)
#include "OSGRTSIMD_SSE.inl"
#elif defined(OSG_SIMD_ALTIVEC)
#include "OSGRTSIMD_ALTIVEC.inl"
#else
#include "OSGBaseFunctions.h"
#include "OSGRTSIMD_FPU.inl"
#endif

#else

#include "OSGBaseFunctions.h"

#ifdef OSG_SIMD_SSE
#include "OSGRTSIMD_SSE.inl"
#endif
#include "OSGRTSIMD_FPU.inl"

#endif

#endif
