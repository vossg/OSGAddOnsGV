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

#ifdef OSG_DEBUG_SIMD
namespace simd_fpu 
{

struct Float4
{
    union 
    {
        Real32 data [4];
        UInt32 idata[4];
    };
};

#endif

inline
Float4 osgSIMDMul(const Float4 v1,  const Float4 v2)
{
    Float4 returnValue;

    returnValue.data[0] = v1.data[0] * v2.data[0];
    returnValue.data[1] = v1.data[1] * v2.data[1];
    returnValue.data[2] = v1.data[2] * v2.data[2];
    returnValue.data[3] = v1.data[3] * v2.data[3];

    return returnValue;
}


inline
Float4 osgSIMDAdd(const Float4 v1,  const Float4 v2)
{
    Float4 returnValue;

    returnValue.data[0] = v1.data[0] + v2.data[0];
    returnValue.data[1] = v1.data[1] + v2.data[1];
    returnValue.data[2] = v1.data[2] + v2.data[2];
    returnValue.data[3] = v1.data[3] + v2.data[3];

    return returnValue;
}

inline
Float4 osgSIMDSub(const Float4 v1,  const Float4 v2)
{
    Float4 returnValue;

    returnValue.data[0] = v1.data[0] - v2.data[0];
    returnValue.data[1] = v1.data[1] - v2.data[1];
    returnValue.data[2] = v1.data[2] - v2.data[2];
    returnValue.data[3] = v1.data[3] - v2.data[3];

    return returnValue;
}

inline
Float4 osgSIMDAnd(const Float4 v1,  const Float4 v2)
{
    Float4 returnValue;

    returnValue.idata[0] = v1.idata[0] & v2.idata[0];
    returnValue.idata[1] = v1.idata[1] & v2.idata[1];
    returnValue.idata[2] = v1.idata[2] & v2.idata[2];
    returnValue.idata[3] = v1.idata[3] & v2.idata[3];

    return returnValue;
}

inline
Float4 osgSIMDOr(const Float4 v1, const Float4 v2)
{
    Float4 returnValue;

    returnValue.idata[0] = v1.idata[0] | v2.idata[0];
    returnValue.idata[1] = v1.idata[1] | v2.idata[1];
    returnValue.idata[2] = v1.idata[2] | v2.idata[2];
    returnValue.idata[3] = v1.idata[3] | v2.idata[3];

    return returnValue;
}

inline
Float4 osgSIMDAndNot(const Float4 v1, const Float4 v2)
{
    Float4 returnValue;

    returnValue.idata[0] = ~v1.idata[0] & v2.idata[0];
    returnValue.idata[1] = ~v1.idata[1] & v2.idata[1];
    returnValue.idata[2] = ~v1.idata[2] & v2.idata[2];
    returnValue.idata[3] = ~v1.idata[3] & v2.idata[3];

    return returnValue;   
}

inline
Float4 osgSIMDMax(const Float4 v1,  const Float4 v2)
{
    Float4 returnValue;

    returnValue.data[0] = v1.data[0] > v2.data[0] ? v1.data[0] : v2.data[0];
    returnValue.data[1] = v1.data[1] > v2.data[1] ? v1.data[1] : v2.data[1];
    returnValue.data[2] = v1.data[2] > v2.data[2] ? v1.data[2] : v2.data[2];
    returnValue.data[3] = v1.data[3] > v2.data[3] ? v1.data[3] : v2.data[3];

    return returnValue;
}

inline
Float4 osgSIMDMin(const Float4 v1,  const Float4 v2)
{
    Float4 returnValue;

    returnValue.data[0] = v1.data[0] < v2.data[0] ? v1.data[0] : v2.data[0];
    returnValue.data[1] = v1.data[1] < v2.data[1] ? v1.data[1] : v2.data[1];
    returnValue.data[2] = v1.data[2] < v2.data[2] ? v1.data[2] : v2.data[2];
    returnValue.data[3] = v1.data[3] < v2.data[3] ? v1.data[3] : v2.data[3];

    return returnValue;
}

inline
Float4 osgSIMDCmpGT(const Float4 v1, const Float4 v2)
{
    Float4 returnValue;

    returnValue.idata[0] = v1.data[0] > v2.data[0] ? 0xffffffff : 0x00000000;
    returnValue.idata[1] = v1.data[1] > v2.data[1] ? 0xffffffff : 0x00000000;
    returnValue.idata[2] = v1.data[2] > v2.data[2] ? 0xffffffff : 0x00000000;
    returnValue.idata[3] = v1.data[3] > v2.data[3] ? 0xffffffff : 0x00000000;

    return returnValue;
}

inline
Float4 osgSIMDCmpGE(const Float4 v1, const Float4 v2)
{
    Float4 returnValue;

    returnValue.idata[0] = v1.data[0] >= v2.data[0] ? 0xffffffff : 0x00000000;
    returnValue.idata[1] = v1.data[1] >= v2.data[1] ? 0xffffffff : 0x00000000;
    returnValue.idata[2] = v1.data[2] >= v2.data[2] ? 0xffffffff : 0x00000000;
    returnValue.idata[3] = v1.data[3] >= v2.data[3] ? 0xffffffff : 0x00000000;

    return returnValue;
}

inline
Float4 osgSIMDCmpLT(const Float4 v1, const Float4 v2)
{
    Float4 returnValue;

    returnValue.idata[0] = v1.data[0] < v2.data[0] ? 0xffffffff : 0x00000000;
    returnValue.idata[1] = v1.data[1] < v2.data[1] ? 0xffffffff : 0x00000000;
    returnValue.idata[2] = v1.data[2] < v2.data[2] ? 0xffffffff : 0x00000000;
    returnValue.idata[3] = v1.data[3] < v2.data[3] ? 0xffffffff : 0x00000000;

    return returnValue;
}


inline
Float4 osgSIMDCmpLE(const Float4 v1, const Float4 v2)
{
    Float4 returnValue;

    returnValue.idata[0] = v1.data[0] <= v2.data[0] ? 0xffffffff : 0x00000000;
    returnValue.idata[1] = v1.data[1] <= v2.data[1] ? 0xffffffff : 0x00000000;
    returnValue.idata[2] = v1.data[2] <= v2.data[2] ? 0xffffffff : 0x00000000;
    returnValue.idata[3] = v1.data[3] <= v2.data[3] ? 0xffffffff : 0x00000000;

    return returnValue;
}

inline
Float4 osgSIMDRSqrtE(const Float4 v)
{
    Float4 returnValue;

    returnValue.data[0] = 1.f / OSG::osgSqrt(v.data[0]);
    returnValue.data[1] = 1.f / OSG::osgSqrt(v.data[1]);
    returnValue.data[2] = 1.f / OSG::osgSqrt(v.data[2]);
    returnValue.data[3] = 1.f / OSG::osgSqrt(v.data[3]);

    return returnValue;
}

inline
Float4 osgSIMDInvert(const Float4 v)
{
    Float4 returnValue;

    returnValue.data[0] = 1.f / v.data[0];
    returnValue.data[1] = 1.f / v.data[1];
    returnValue.data[2] = 1.f / v.data[2];
    returnValue.data[3] = 1.f / v.data[3];

    return returnValue;
}


inline
Int32  osgSIMDMoveMask(const Float4 v)
{
    Int32 returnValue = 
        ((v.idata[0] & 0x80000000) ? 0x1 : 0x0) |
        ((v.idata[1] & 0x80000000) ? 0x2 : 0x0) |
        ((v.idata[2] & 0x80000000) ? 0x4 : 0x0) |
        ((v.idata[3] & 0x80000000) ? 0x8 : 0x0);

    return returnValue;
}

inline
Float4 osgSIMDSet(const Real32 rVal)
{
    Float4 returnValue;

    returnValue.data[0] = rVal;
    returnValue.data[1] = rVal;
    returnValue.data[2] = rVal;
    returnValue.data[3] = rVal;

    return returnValue;
}


inline
Float4 osgSIMDSet(const Real32 rVal0,
                  const Real32 rVal1,
                  const Real32 rVal2,
                  const Real32 rVal3)
{
    Float4 returnValue;

    returnValue.data[3] = rVal0;
    returnValue.data[2] = rVal1;
    returnValue.data[1] = rVal2;
    returnValue.data[0] = rVal3;

    return returnValue;
}

inline
Float4 osgSIMDUpdate(const Float4 mask, const Float4 v1, const Float4 v2)
{
    return osgSIMDOr(osgSIMDAnd   (v1,   mask),
                     osgSIMDAndNot(mask, v2  ));
}

#ifdef OSG_DEBUG_SIMD
}
#endif

OSG_END_NAMESPACE
