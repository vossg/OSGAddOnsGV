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
     Inherited(        ),
    _fSingle  (        ),
    _fQuad    (        )
{
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


#if 0
inline 
void RTRaySIMDPacket::setOrigin(Real32 oX,
                                Real32 oY,
                                Real32 oZ  )
{
    _vSingleA[0] = oX;
    _vSingleA[1] = oY;
    _vSingleA[2] = oZ;
    _vSingleA[3] = 0.f;
}

inline
void RTRaySIMDPacket::setOrigin(Pnt3f vOrigin)
{
    _vSingleA[0] = vOrigin[0];
    _vSingleA[1] = vOrigin[1];
    _vSingleA[2] = vOrigin[2];
    _vSingleA[3] = 0.f;
}

inline
void RTRaySIMDPacket::setOrigin(Float4 origin)
{
    _fSingle = origin;
}

inline 
void RTRaySIMDPacket::setDirection(Vec3f  vDir,
                                   UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumRays);

    _vQuadA[X][uiIdx] = vDir[X];
    _vQuadA[Y][uiIdx] = vDir[Y];
    _vQuadA[Z][uiIdx] = vDir[Z];
}

inline
void RTRaySIMDPacket::setDirections(Float4 fDirX,
                                    Float4 fDirY,
                                    Float4 fDirZ  )
{
    _fQuad[0] = fDirX;
    _fQuad[1] = fDirY;
    _fQuad[2] = fDirZ;
}

inline
void RTRaySIMDPacket::setDirX(Float4 fDir)
{
    _fQuad[0] = fDir;
}

inline
void RTRaySIMDPacket::setDirY(Float4 fDir)
{
    _fQuad[1] = fDir;
}

inline
void RTRaySIMDPacket::setDirZ(Float4 fDir)
{
    _fQuad[2] = fDir;
}

inline 
void RTRaySIMDPacket::normalizeDirection(void)
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
#endif

#if 0
inline 
Float4 RTRaySIMDPacket::getOrigin(void)
{
    return _fSingle;
}

inline 
Real32 RTRaySIMDPacket::getOriginComp(const UInt32 uiCoord)
{
    return _vSingleA[uiCoord];
}

inline 
Float4 RTRaySIMDPacket::getDir(const UInt32 uiCoord)
{
    OSG_ASSERT(uiCoord < 3);

    return _fQuad[uiCoord];
}
#endif




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

#if 0
    _fDir[X] = source._fDir[X];
    _fDir[Y] = source._fDir[Y];
    _fDir[Z] = source._fDir[Z];
#endif
    _fDir = source._fDir;
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

#if 0
    _fDir[X]    = source._fDir[X];
    _fDir[Y]    = source._fDir[Y];
    _fDir[Z]    = source._fDir[Z];
#endif

    _fDir = source._fDir;
}

inline
void RTRayFullSIMDPacket::setQuad(Float4 fX,
                                  Float4 fY,
                                  Float4 fZ)
{
    _fOrigin[X] = fX;
    _fOrigin[Y] = fY;
    _fOrigin[Z] = fZ;
}

#if 0
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
#endif

#if 0
inline 
void RTRayFullSIMDPacket::setDirection(Vec3f  vDir,
                                       UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumRays);

#if 0
    _vDirA[X][uiIdx] = vDir[X];
    _vDirA[Y][uiIdx] = vDir[Y];
    _vDirA[Z][uiIdx] = vDir[Z];
#endif

    _vDirA[X] = vDir[X];
    _vDirA[Y] = vDir[Y];
    _vDirA[Z] = vDir[Z];
}
#endif

inline 
void RTRayFullSIMDPacket::setSingle(Vec3f  vDir)
{
#if 0
    _vDirA[X][0] = vDir[X];
    _vDirA[Y][0] = vDir[Y];
    _vDirA[Z][0] = vDir[Z];

    _vDirA[X][1] = vDir[X];
    _vDirA[Y][1] = vDir[Y];
    _vDirA[Z][1] = vDir[Z];

    _vDirA[X][2] = vDir[X];
    _vDirA[Y][2] = vDir[Y];
    _vDirA[Z][2] = vDir[Z];

    _vDirA[X][3] = vDir[X];
    _vDirA[Y][3] = vDir[Y];
    _vDirA[Z][3] = vDir[Z];
#endif

    _vDirA[X] = vDir[X];
    _vDirA[Y] = vDir[Y];
    _vDirA[Z] = vDir[Z];
    _vDirA[3] = 0.f;
}

#if 0
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
#endif

#if 0
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
#endif

inline 
Float4 RTRayFullSIMDPacket::getQuad(const UInt32 uiCoord)
{
    OSG_ASSERT(uiCoord < 3);

    return _fOrigin[uiCoord];
}

#if 0
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
#endif

OSG_END_NAMESPACE
