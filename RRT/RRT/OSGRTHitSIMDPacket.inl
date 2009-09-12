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
    _rDist     (),
    _rU        (),
    _rV        (),
    _uiCacheId (),
    _uiObjId   (),
    _uiTriId   ()
{
    for(UInt32 i = 0; i < NumHits; ++i)
    {
        _rDist    [i] = FLT_MAX;
        _rU       [i] = -1.f;
        _rV       [i] = -1.f;
        _uiCacheId[i] = 0;
        _uiObjId  [i] = 0;
        _uiTriId  [i] = 0;
    }
}

inline
RTHitSIMDPacket::~RTHitSIMDPacket(void)
{
}

inline
void RTHitSIMDPacket::reset(void)
{
    for(UInt32 i = 0; i < NumHits; ++i)
    {
        _rDist    [i] = FLT_MAX;
        _rU       [i] = -1.f;
        _rV       [i] = -1.f;
        _uiCacheId[i] = 0;
        _uiObjId  [i] = 0;
        _uiTriId  [i] = 0;
    }
}

inline
void RTHitSIMDPacket::set(UInt32 uiIdx,
                          Real32 rDist, 
                          Real32 rU, 
                          Real32 rV, 
                          UInt32 uiObjId,
                          UInt32 uiTriId,
                          UInt32 uiCacheId)
{
    OSG_ASSERT(uiIdx < NumHits);

    _rDist    [uiIdx] = rDist;
    _rU       [uiIdx] = rU;
    _rV       [uiIdx] = rV;
    _uiObjId  [uiIdx] = uiObjId;
    _uiTriId  [uiIdx] = uiTriId;
    _uiCacheId[uiIdx] = uiCacheId;
}

inline
Real32 RTHitSIMDPacket::getDist(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _rDist[uiIdx];
}

inline
Real32 RTHitSIMDPacket::getU(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _rU[uiIdx];
}

inline
Real32 RTHitSIMDPacket::getV(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _rV[uiIdx];
}


inline
UInt32 RTHitSIMDPacket::getObjId(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _uiObjId[uiIdx];
}

inline
UInt32 RTHitSIMDPacket::getTriId(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _uiTriId[uiIdx];
}

inline
UInt32 RTHitSIMDPacket::getCacheId(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _uiCacheId[uiIdx];
}

inline 
void RTHitSIMDPacket::operator =(const RTHitSIMDPacket &source)
{
    for(UInt32 i = 0; i < NumHits; ++i)
    {
        _rDist    [i]  = source._rDist    [i];
        _rU       [i]  = source._rU       [i];
        _rV       [i]  = source._rV       [i];
        _uiCacheId[i]  = source._uiCacheId[i];
        _uiObjId  [i]  = source._uiObjId  [i];
        _uiTriId  [i]  = source._uiTriId  [i];
    }
}

OSG_END_NAMESPACE
