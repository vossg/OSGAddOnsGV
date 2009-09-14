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
Float4 osgSIMDMul(const Float4 v1,  const Float4 v2)
{
    return _mm_mul_ps(v1, v2);
}

inline
Float4 osgSIMDAdd(const Float4 v1,  const Float4 v2)
{
    return _mm_add_ps(v1, v2);
}

inline
Float4 osgSIMDSub(const Float4 v1,  const Float4 v2)
{
    return _mm_sub_ps(v1, v2);
}

inline
Float4 osgSIMDAnd(const Float4 v1,  const Float4 v2)
{
    return _mm_and_ps(v1, v2);
}

inline
Float4 osgSIMDMax(const Float4 v1,  const Float4 v2)
{
    return _mm_max_ps(v1, v2);
}

inline
Float4 osgSIMDMin(const Float4 v1,  const Float4 v2)
{
    return _mm_min_ps(v1, v2);
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
    return _mm_rsqrt_ps(v);
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
    return _mm_set_ps(rVal0, rVal1, rVal2, rVal3);
}

inline
Float4 osgSIMDSet(const UInt32 uiVal)
{
    union
    {
        Float4 returnValue;
        UInt32 iData[4];
    };

    iData[0] = iData[1] = iData[2] = iData[3] = uiVal;

    return returnValue;
}


inline
Float4 osgSIMDSet(const UInt32 uiVal0,
                  const UInt32 uiVal1,
                  const UInt32 uiVal2,
                  const UInt32 uiVal3)
{
    union
    {
        Float4 returnValue;
        UInt32 iData[4];
    };

    iData[0] = uiVal0;
    iData[1] = uiVal1;
    iData[2] = uiVal2;
    iData[3] = uiVal3;

    return returnValue;
}

inline
Float4 osgSIMDUpdate(const Float4 mask, const Float4 v1, const Float4 v2)
{
    return _mm_or_ps(_mm_and_ps   (v1,   mask),
                     _mm_andnot_ps(mask, v2  ));
}

inline 
void osgSIMDDump(const Float4 v1)
{
    union 
    {
        Real32 rVal[4];
        Float4 vVal;
    };

    vVal = v1;

    fprintf(stderr, "%f %f %f %f\n",
            rVal[0],
            rVal[1],
            rVal[2],
            rVal[3]);

}

OSG_END_NAMESPACE
