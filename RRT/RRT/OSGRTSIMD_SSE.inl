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
   return _mm_add_ps(_mm_mul_ps(vM1, vM2), vS);
}

inline
Float4 osgSIMDMul (const Float4 v1,  const Float4 v2)
{
    return _mm_mul_ps(v1, v2);
}

inline
Float4 osgSIMDAdd (const Float4 v1,  const Float4 v2)
{
    return _mm_add_ps(v1, v2);
}

inline
Float4 osgSIMDAnd (const Float4 v1,  const Float4 v2)
{
    return _mm_and_ps(v1, v2);
}

inline
Float4 osgSIMDCmpGT(const Float4 v1, const Float4 v2)
{
    return _mm_cmpgt_ps(v1, v2);
}

inline
Float4 osgSIMDCmpGE(const Float4 v1, const Float4 v2)
{
    return _mm_cmpge_ps(v1, v2);
}

inline
Float4 osgSIMDCmpLT(const Float4 v1, const Float4 v2)
{
    return _mm_cmplt_ps(v1, v2);
}

inline
Float4 osgSIMDCmpLE(const Float4 v1, const Float4 v2)
{
    return _mm_cmple_ps(v1, v2);
}

inline
Float4 osgSIMDRSqrtE(const Float4 v)
{
    return _mm_rcp_ps(v);
}

inline
Float4 osgSIMDInvert(const Float4 v)
{
    const Float4 rcp = _mm_rcp_ps(v);

    return _mm_sub_ps(_mm_add_ps(rcp, rcp),
                      _mm_mul_ps(_mm_mul_ps(rcp, rcp),
                                 v));

}

inline
Int32  osgSIMDMoveMask(const Float4 v)
{
    return _mm_movemask_ps(v);
}

inline
Float4 osgSIMDSet(const Real32 rVal)
{
    return _mm_set_ps1(rVal);
}

inline
Float4 osgSIMDSet(const Real32 rVal0,
                  const Real32 rVal1,
                  const Real32 rVal2,
                  const Real32 rVal3)
{
}

OSG_END_NAMESPACE
