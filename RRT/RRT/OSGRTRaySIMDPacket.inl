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
RTRaySIMDPacket::RTRaySIMDPacket(void) :
    _vOrigin  (    ),
    _vDir     (    ),
    _bIsActive(true)
{
}

inline
RTRaySIMDPacket::~RTRaySIMDPacket(void)
{
}

inline 
void RTRaySIMDPacket::operator =(const RTRaySIMDPacket &source)
{
    _vOrigin   = source._vOrigin;
    _vDir      = source._vDir;
    _bIsActive = source._bIsActive;
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
void RTRaySIMDPacket::setDirection(Vec3f  vDir)
{
    _vDir = vDir;
}

inline 
void RTRaySIMDPacket::normalizeDirection(void)
{
    _vDir.normalize();
}

inline 
Pnt3f RTRaySIMDPacket::getOrigin(void)
{
    return _vOrigin;
}

inline 
Vec3f RTRaySIMDPacket::getDir(void)
{
    return _vDir;
}

inline
void RTRaySIMDPacket::setActive(bool bVal)
{
    _bIsActive = bVal;
}

inline
bool RTRaySIMDPacket::isActive(void)
{
    return _bIsActive;
}

OSG_END_NAMESPACE
