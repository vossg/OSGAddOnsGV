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
void RTRaySIMDPacket::setOrigin(Float4 origin)
{
    _fOrigin = origin;
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
void RTRaySIMDPacket::setDirX(Float4 fDir)
{
    _fDir[0] = fDir;
}

inline
void RTRaySIMDPacket::setDirY(Float4 fDir)
{
    _fDir[1] = fDir;
}

inline
void RTRaySIMDPacket::setDirZ(Float4 fDir)
{
    _fDir[2] = fDir;
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


inline
RTRayFullSIMDPacket::RTRayFullSIMDPacket(void) :
     Inherited(        ),
    _fOrigin  (        ),
    _fDir     (        )
{
}

inline
RTRayFullSIMDPacket::RTRayFullSIMDPacket(const RTRayFullSIMDPacket &source) :
     Inherited(source)
{
    _fOrigin[X] = source._fOrigin[X];
    _fOrigin[Y] = source._fOrigin[Y];
    _fOrigin[Z] = source._fOrigin[Z];

    _fDir[X] = source._fDir[X];
    _fDir[Y] = source._fDir[Y];
    _fDir[Z] = source._fDir[Z];
}


inline
RTRayFullSIMDPacket::~RTRayFullSIMDPacket(void)
{
}

inline 
void RTRayFullSIMDPacket::operator =(const RTRayFullSIMDPacket &source)
{
    Inherited::operator =(source);

    _fOrigin[X] = source._fOrigin[X];
    _fOrigin[Y] = source._fOrigin[Y];
    _fOrigin[Z] = source._fOrigin[Z];

    _fDir[X]    = source._fDir[X];
    _fDir[Y]    = source._fDir[Y];
    _fDir[Z]    = source._fDir[Z];
}

inline
void RTRayFullSIMDPacket::setOrigin(Pnt3f vOrigin)
{
    _vOriginA[X][0] = vOrigin[X];
    _vOriginA[Y][0] = vOrigin[Y];
    _vOriginA[Z][0] = vOrigin[Z];

    _vOriginA[X][1] = vOrigin[X];
    _vOriginA[Y][1] = vOrigin[Y];
    _vOriginA[Z][1] = vOrigin[Z];

    _vOriginA[X][2] = vOrigin[X];
    _vOriginA[Y][2] = vOrigin[Y];
    _vOriginA[Z][2] = vOrigin[Z];

    _vOriginA[X][3] = vOrigin[X];
    _vOriginA[Y][3] = vOrigin[Y];
    _vOriginA[Z][3] = vOrigin[Z];
}

inline 
void RTRayFullSIMDPacket::setOrigin(Real32 oX,
                                    Real32 oY,
                                    Real32 oZ,
                                    UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumRays);

    _vOriginA[X][uiIdx] = oX;
    _vOriginA[Y][uiIdx] = oY;
    _vOriginA[Z][uiIdx] = oZ;
}

inline
void RTRayFullSIMDPacket::setOrigin(Pnt3f  vOrigin,
                                    UInt32 uiIdx  )
{
    OSG_ASSERT(uiIdx < NumRays);

    _vOriginA[X][uiIdx] = vOrigin[X];
    _vOriginA[Y][uiIdx] = vOrigin[Y];
    _vOriginA[Z][uiIdx] = vOrigin[Z];
}

inline 
void RTRayFullSIMDPacket::setDirection(Vec3f  vDir,
                                       UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumRays);

    _vDirA[X][uiIdx] = vDir[X];
    _vDirA[Y][uiIdx] = vDir[Y];
    _vDirA[Z][uiIdx] = vDir[Z];
}

inline
void RTRayFullSIMDPacket::setDirX(Float4 fDir)
{
    _fDir[X] = fDir;
}

inline
void RTRayFullSIMDPacket::setDirY(Float4 fDir)
{
    _fDir[Y] = fDir;
}

inline
void RTRayFullSIMDPacket::setDirZ(Float4 fDir)
{
    _fDir[Z] = fDir;
}

inline 
void RTRayFullSIMDPacket::normalizeDirection(void)
{
    Float4 v1  = osgSIMDMul(_fDir[X], _fDir[X]);
    Float4 v2  = osgSIMDMul(_fDir[Y], _fDir[Y]);
    Float4 v3  = osgSIMDMul(_fDir[Z], _fDir[Z]);

    Float4 sum = osgSIMDAdd(v1, osgSIMDAdd(v2, v3));

    Float4 nr         = osgSIMDRSqrtE(sum);

    Float4 mulsum     = osgSIMDMul(osgSIMDMul(sum, nr), nr);

    Float4 reciprocal = osgSIMDMul(osgSIMDMul(SIMDHalf,  nr    ), 
                                   osgSIMDSub(SIMDThree, mulsum));

    _fDir[X] = osgSIMDMul(_fDir[X], reciprocal);
    _fDir[Y] = osgSIMDMul(_fDir[Y], reciprocal);
    _fDir[Z] = osgSIMDMul(_fDir[Z], reciprocal);
}

inline 
Float4 RTRayFullSIMDPacket::getOrigin(const UInt32 uiCoord)
{
    OSG_ASSERT(uiCoord < 3);

    return _fOrigin[uiCoord];
}

inline 
Float4 RTRayFullSIMDPacket::getDir(const UInt32 uiCoord)
{
    OSG_ASSERT(uiCoord < 3);

    return _fDir[uiCoord];
}

inline 
Real32 RTRayFullSIMDPacket::getOriginComp(const UInt32 uiIndex)
{
    return _vOriginA[uiIndex][0];
}

OSG_END_NAMESPACE
