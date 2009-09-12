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

OSG_BEGIN_NAMESPACE


inline
Float4 osgSIMDMadd(const Float4 vM1, const Float4 vM2, const Float4 vS)
{
    return vec_madd(vM1, vM2, vS);
}

inline
Float4 osgSIMDMul(const Float4 v1,  const Float4 v2)
{
    return vec_madd(v1, v2, SIMDZero);
}

inline
Float4 osgSIMDAdd(const Float4 v1,  const Float4 v2)
{
    return vec_add(v1, v2);
}

inline
Float4 osgSIMDSub(const Float4 v1,  const Float4 v2)
{
    return vec_sub(v1, v2);
}

inline
Float4 osgSIMDAnd(const Float4 v1,  const Float4 v2)
{
    return vec_and(v1, v2);
}

inline
Float4 osgSIMDOr(const Float4 v1, const Float4 v2)
{
    return vec_or(v1, v2);
}

inline
Float4 osgSIMDAndNot(const Float4 v1, const Float4 v2)
{
    return vec_andc(v2, v1);
}

inline
Float4 osgSIMDMax(const Float4 v1,  const Float4 v2)
{
    return vec_max(v1, v2);
}

inline
Float4 osgSIMDMin(const Float4 v1,  const Float4 v2)
{
    return vec_min(v1, v2);
}

inline
Float4 osgSIMDCmpGT(const Float4 v1, const Float4 v2)
{
    //vector bool int cmp = vec_cmpgt(v1, v2);
    return (Float4) vec_cmpgt(v1, v2);
}

inline
Float4 osgSIMDCmpGE(const Float4 v1, const Float4 v2)
{
    // vector bool int cmp = vec_cmpgt(v1, v2);
    return (Float4) vec_cmpge(v1, v2);
}

inline
Float4 osgSIMDCmpLT(const Float4 v1, const Float4 v2)
{
    //vector bool int cmp = vec_cmplt(v1, v2);
    return (Float4) vec_cmplt(v1, v2);
}

inline
Float4 osgSIMDCmpLE(const Float4 v1, const Float4 v2)
{
    //vector bool int cmp = vec_cmple(v1, v2);
    return (Float4) vec_cmple(v1, v2);
}

inline
Float4 osgSIMDRSqrtE(const Float4 v)
{
    return vec_rsqrte(v);
}

inline
Float4 osgSIMDInvert(const Float4 v)
{
    const Float4 re = vec_re(v);

    return vec_sub(vec_add(re, re),
                   vec_madd(vec_madd(re, re, SIMDZero),
                            v, SIMDZero));
}

inline
Int32  osgSIMDMoveMask(const Float4 v)
{
    vector bool int cmp = vec_cmplt((vector signed int) v, 
                                    (vector signed int) SIMDZero);

    vector unsigned int resi = vec_and((vector unsigned int)cmp,
                                       (vector unsigned int) {1<<0,
                                               1<<1,
                                               1<<2,
                                               1<<3} );

    vector signed int res = vec_sums((vector signed int)resi, 
                                     (vector signed int)SIMDZero);

    return vec_extract(res, 3);
}

inline
Float4 osgSIMDSet(const Real32 rVal)
{
    return (vector float) {rVal, rVal, rVal, rVal};
}

inline
Float4 osgSIMDSet(const Real32 rVal0,
                  const Real32 rVal1,
                  const Real32 rVal2,
                  const Real32 rVal3)
{
//  return (vector float){rVal0, rVal1, rVal2, rVal3};
    return (vector float){rVal3, rVal2, rVal1, rVal0};
}

inline
Float4 osgSIMDUpdate(const Float4 mask, const Float4 v1, const Float4 v2)
{
    return vec_or(vec_and(v1, mask), vec_andc(v2, mask));
}

OSG_END_NAMESPACE
