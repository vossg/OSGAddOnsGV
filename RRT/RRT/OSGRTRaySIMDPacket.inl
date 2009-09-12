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

    return _bIsActive[uiIdx];
}

inline
bool RTRaySIMDPacketInfo::hasActive(void)
{
    bool returnValue = false;

    for(UInt32 i = 0; i < NumRays; ++i)
    {
        returnValue |= _bIsActive[i];
    }

    return returnValue;
}

inline
UInt16 *RTRaySIMDPacketInfo::getActiveRays(void)
{
    return _bIsActive;
}

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
     Inherited(        ),
#ifdef OSG_SIMD_RAYPACKET_DEBUG
    _vOrigin  (        ),
    _vDir     (        ),
#endif
    _fOrigin  (        ),
    _fDir     (        )
{
}

inline
RTRaySIMDPacket::RTRaySIMDPacket(const RTRaySIMDPacket &source) :
     Inherited(source         ),
#ifdef OSG_SIMD_RAYPACKET_DEBUG
    _vOrigin  (source._vOrigin),
    _vDir     (               ),
#endif
    _fOrigin  (source._fOrigin)
{
    _fDir[0] = source._fDir[0];
    _fDir[1] = source._fDir[1];
    _fDir[2] = source._fDir[2];
    _fDir[3] = source._fDir[3];

#ifdef OSG_SIMD_RAYPACKET_DEBUG
    _vDir[0] = source._vDir[0];
    _vDir[1] = source._vDir[1];
    _vDir[2] = source._vDir[2];
    _vDir[3] = source._vDir[3];
#endif
}

inline
RTRaySIMDPacket::~RTRaySIMDPacket(void)
{
}

inline 
void RTRaySIMDPacket::operator =(const RTRaySIMDPacket &source)
{
    Inherited::operator =(source);

    _fOrigin   = source._fOrigin;

    _fDir[0]   = source._fDir[0];
    _fDir[1]   = source._fDir[1];
    _fDir[2]   = source._fDir[2];
    _fDir[3]   = source._fDir[3];

#ifdef OSG_SIMD_RAYPACKET_DEBUG
    _vOrigin   = source._vOrigin;

    _vDir[0]   = source._vDir[0];
    _vDir[1]   = source._vDir[1];
    _vDir[2]   = source._vDir[2];
    _vDir[3]   = source._vDir[3];
#endif
}

inline 
void RTRaySIMDPacket::setOrigin(Real32 oX,
                                Real32 oY,
                                Real32 oZ  )
{
    _vOriginA[0] = oX;
    _vOriginA[1] = oY;
    _vOriginA[2] = oZ;
    _vOriginA[3] = 0.f;

#ifdef OSG_SIMD_RAYPACKET_DEBUG
    _vOrigin.setValues(oX, oY, oZ);
#endif
}

inline
void RTRaySIMDPacket::setOrigin(Pnt3f vOrigin)
{
    _vOriginA[0] = vOrigin[0];
    _vOriginA[1] = vOrigin[1];
    _vOriginA[2] = vOrigin[2];
    _vOriginA[3] = 0.f;

#ifdef OSG_SIMD_RAYPACKET_DEBUG
    _vOrigin = vOrigin;
#endif
}

inline 
void RTRaySIMDPacket::setDirection(Vec3f  vDir,
                                   UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumRays);

    _vDirA[X][uiIdx] = vDir[X];
    _vDirA[Y][uiIdx] = vDir[Y];
    _vDirA[Z][uiIdx] = vDir[Z];

#ifdef OSG_SIMD_RAYPACKET_DEBUG
    _vDir[uiIdx] = vDir;
#endif
}

inline 
void RTRaySIMDPacket::normalizeDirection(void)
{
#ifdef OSG_SIMD_RAYPACKET_DEBUG
    for(UInt32 i = 0; i < NumRays; ++i)
    {
        _vDir[i].normalize();
    }
#endif

    Float4 v1  = osgSIMDMul(_fDir[X], _fDir[X]);
    Float4 v2  = osgSIMDMul(_fDir[Y], _fDir[Y]);
    Float4 v3  = osgSIMDMul(_fDir[Z], _fDir[Z]);

    Float4 sum = osgSIMDAdd(v1, osgSIMDAdd(v2, v3));

    Float4 nr         = osgSIMDRSqrtE(sum);

    Float4 mulsum     = osgSIMDMul(osgSIMDMul(sum, nr), nr);

    Float4 reciprocal = osgSIMDMul(osgSIMDMul(SIMDHalf,  nr    ), 
                                   osgSIMDSub(SIMDThree, mulsum));

    _fDir[X] = osgSIMDMul(_fDir[X], reciprocal );
    _fDir[Y] = osgSIMDMul(_fDir[Y], reciprocal );
    _fDir[Z] = osgSIMDMul(_fDir[Z], reciprocal );
}

#ifdef OSG_SIMD_RAYPACKET_DEBUG
inline 
Pnt3f RTRaySIMDPacket::getOriginPnt(void)
{
    return _vOrigin;
}

inline 
Vec3f RTRaySIMDPacket::getDirVec(const UInt32 uiCoord)
{
    OSG_ASSERT(uiCoord < 4);

    return _vDir[uiCoord];
}
#endif

inline 
Float4 RTRaySIMDPacket::getOrigin(void)
{
    return _fOrigin;
}

inline 
Real32 RTRaySIMDPacket::getOriginComp(const UInt32 uiCoord)
{
    return _vOriginA[uiCoord];
}

inline 
Float4 RTRaySIMDPacket::getDir(const UInt32 uiCoord)
{
    OSG_ASSERT(uiCoord < 3);

    return _fDir[uiCoord];
}

OSG_END_NAMESPACE
