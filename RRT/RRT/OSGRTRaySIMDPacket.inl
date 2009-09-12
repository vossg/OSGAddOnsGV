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
UInt32 *RTRaySIMDPacketInfo::getActiveRays(void)
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
    _vOrigin  (        ),
    _vDir     (        )
{
}

RTRaySIMDPacket::RTRaySIMDPacket(const RTRaySIMDPacket &source) :
     Inherited(source         ),
    _vOrigin  (source._vOrigin),
    _vDir     (               )
{
    _vDir[0] = source._vDir[0];
    _vDir[1] = source._vDir[1];
    _vDir[2] = source._vDir[2];
    _vDir[3] = source._vDir[3];
}

inline
RTRaySIMDPacket::~RTRaySIMDPacket(void)
{
}

inline 
void RTRaySIMDPacket::operator =(const RTRaySIMDPacket &source)
{
    Inherited::operator =(source);

    _vOrigin   = source._vOrigin;

    _vDir[0]   = source._vDir[0];
    _vDir[1]   = source._vDir[1];
    _vDir[2]   = source._vDir[2];
    _vDir[3]   = source._vDir[3];
}

inline 
void RTRaySIMDPacket::setOrigin(Real32 oX,
                                Real32 oY,
                                Real32 oZ  )
{
    _vOrigin.setValues(oX, oY, oZ);
}

inline
void RTRaySIMDPacket::setOrigin(Pnt3f vOrigin)
{
    _vOrigin = vOrigin;
}

inline 
void RTRaySIMDPacket::setDirection(Vec3f  vDir,
                                   UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumRays);

    _vDir[uiIdx] = vDir;
}

inline 
void RTRaySIMDPacket::normalizeDirection(void)
{
    for(UInt32 i = 0; i < NumRays; ++i)
    {
        _vDir[i].normalize();
    }
}

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

OSG_END_NAMESPACE
