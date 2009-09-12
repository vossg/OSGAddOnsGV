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
RTHitSIMDPacket::RTHitSIMDPacket(void) :
    _rDist     (FLT_MAX),
    _rU        (-1.f   ),
    _rV        (-1.f   ),
    _uiCacheId (0      ),
    _uiObjId   (0      ),
    _uiTriId   (0      )
{
}

inline
RTHitSIMDPacket::~RTHitSIMDPacket(void)
{
}

inline
void RTHitSIMDPacket::reset(void)
{
    _rDist      = FLT_MAX;
    _rU         = -1.f;
    _rV         = -1.f;
    _uiCacheId  = 0;
    _uiObjId    = 0;
    _uiTriId    = 0;
}

inline
void RTHitSIMDPacket::set(Real32 rDist, 
                          Real32 rU, 
                          Real32 rV, 
                          UInt32 uiObjId,
                          UInt32 uiTriId)
{
    _rDist   = rDist;
    _rU      = rU;
    _rV      = rV;
    _uiObjId = uiObjId;
    _uiTriId = uiTriId;
}

inline
Real32 RTHitSIMDPacket::getDist(void)
{
    return _rDist;
}

inline
Real32 RTHitSIMDPacket::getU(void)
{
    return _rU;
}

inline
Real32 RTHitSIMDPacket::getV(void)
{
    return _rV;
}


inline
UInt32 RTHitSIMDPacket::getObjId(void)
{
    return _uiObjId;
}

inline
UInt32 RTHitSIMDPacket::getTriId(void)
{
    return _uiTriId;
}

inline
void RTHitSIMDPacket::setCacheId(UInt32 uiCacheId)
{
    _uiCacheId = uiCacheId;
}


inline
UInt32 RTHitSIMDPacket::getCacheId(void)
{
    return _uiCacheId;
}

inline 
void RTHitSIMDPacket::operator =(const RTHitSIMDPacket &source)
{
    _rDist      = source._rDist;
    _rU         = source._rU;
    _rV         = source._rV;
    _uiCacheId  = source._uiCacheId;
    _uiObjId    = source._uiObjId;
    _uiTriId    = source._uiTriId;
}

OSG_END_NAMESPACE
