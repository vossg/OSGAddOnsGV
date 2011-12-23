/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

inline
RTRaySIMDPacketInfo::RTRaySIMDPacketInfo(void) :
    _bIsActive()
{
    for(UInt32 i = 0; i < NumRays; ++i)
    {
        _bIsActive[i] = 1;
    }
}

inline
RTRaySIMDPacketInfo::RTRaySIMDPacketInfo(const RTRaySIMDPacketInfo &source) :
    _bIsActive()
{
    for(UInt32 i = 0; i < NumRays; ++i)
    {
        _bIsActive[i] = source._bIsActive[i];
    }
}

inline
RTRaySIMDPacketInfo::~RTRaySIMDPacketInfo(void)
{
}

inline
void RTRaySIMDPacketInfo::setActive(bool   bVal,
                                    UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumRays);

    _bIsActive[uiIdx] = bVal;
}

inline
bool RTRaySIMDPacketInfo::isActive(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumRays);

    return _bIsActive[uiIdx] != 0;
}

inline
bool RTRaySIMDPacketInfo::hasActive(void)
{
    UInt16 returnValue = false;

    for(UInt32 i = 0; i < NumRays; ++i)
    {
        returnValue |= _bIsActive[i];
    }

    return returnValue != 0;
}

#if 1
inline
UInt16 *RTRaySIMDPacketInfo::getActiveRays(void)
{
    return _bIsActive;
}
#else
inline
UInt16 RTRaySIMDPacketInfo::getActiveRays(void)
{
    return 0; //_bIsActive;
}
#endif

inline 
void RTRaySIMDPacketInfo::operator =(const RTRaySIMDPacketInfo &source)
{
    for(UInt32 i = 0; i < NumRays; ++i)
    {
        _bIsActive[i] = source._bIsActive[i];
    }
}






inline
RTRaySIMDPacket::RTRaySIMDPacket(void) :
     Inherited(        )
{
    _fSingle  = SIMDZero;

    _fQuad[0] = SIMDZero;
    _fQuad[1] = SIMDZero;
    _fQuad[2] = SIMDZero;
}

inline
RTRaySIMDPacket::RTRaySIMDPacket(const RTRaySIMDPacket &source) :
     Inherited(source         ),
    _fSingle  (source._fSingle)
{
    _fQuad[0] = source._fQuad[0];
    _fQuad[1] = source._fQuad[1];
    _fQuad[2] = source._fQuad[2];
}

inline
RTRaySIMDPacket::~RTRaySIMDPacket(void)
{
}

inline 
void RTRaySIMDPacket::operator =(const RTRaySIMDPacket &source)
{
    Inherited::operator =(source);

    _fSingle   = source._fSingle;

    _fQuad[0]   = source._fQuad[0];
    _fQuad[1]   = source._fQuad[1];
    _fQuad[2]   = source._fQuad[2];
}


inline 
void RTRaySIMDPacket::setSingle(Float4 fSingle)
{
    _fSingle = fSingle;
}

inline 
void RTRaySIMDPacket::setSingle(Vec3f vSingle)
{
    _vSingleA[0] = vSingle[0];
    _vSingleA[1] = vSingle[1];
    _vSingleA[2] = vSingle[2];
    _vSingleA[3] = 0.f;
}

inline 
void RTRaySIMDPacket::setSingle(Pnt3f vSingle)
{
    _vSingleA[0] = vSingle[0];
    _vSingleA[1] = vSingle[1];
    _vSingleA[2] = vSingle[2];
    _vSingleA[3] = 0.f;
}

inline 
Real32 RTRaySIMDPacket::getSingleComp(const UInt32 uiCoord)
{
    return _vSingleA[uiCoord];
}

inline 
Float4 RTRaySIMDPacket::getSingle(void)
{
    return _fSingle;
}


inline 
void RTRaySIMDPacket::setQuad(Float4 fX,
                              Float4 fY,
                              Float4 fZ)
{
    _fQuad[X] = fX;
    _fQuad[Y] = fY;
    _fQuad[Z] = fZ;
}

inline 
void RTRaySIMDPacket::normalizeQuad(void)
{
    Float4 v1  = osgSIMDMul(_fQuad[X], _fQuad[X]);
    Float4 v2  = osgSIMDMul(_fQuad[Y], _fQuad[Y]);
    Float4 v3  = osgSIMDMul(_fQuad[Z], _fQuad[Z]);

    Float4 sum = osgSIMDAdd(v1, osgSIMDAdd(v2, v3));

    Float4 nr         = osgSIMDRSqrtE(sum);

    Float4 mulsum     = osgSIMDMul(osgSIMDMul(sum, nr), nr);

    Float4 reciprocal = osgSIMDMul(osgSIMDMul(SIMDHalf,  nr    ), 
                                   osgSIMDSub(SIMDThree, mulsum));

    _fQuad[X] = osgSIMDMul(_fQuad[X], reciprocal );
    _fQuad[Y] = osgSIMDMul(_fQuad[Y], reciprocal );
    _fQuad[Z] = osgSIMDMul(_fQuad[Z], reciprocal );
}

inline 
Float4 RTRaySIMDPacket::getQuad(const UInt32 uiCoord)
{
    OSG_ASSERT(uiCoord < 3);

    return _fQuad[uiCoord];
}


OSG_END_NAMESPACE
