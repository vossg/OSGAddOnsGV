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
     Inherited (            ),
    _rDist     (SIMDInfinity),
    _rU        (SIMDMinusOne),
    _rV        (SIMDMinusOne),
    _uiCacheId (SIMDZero    ),
    _uiObjId   (SIMDZero    ),
    _uiTriId   (SIMDZero    )
{
}
 
inline
RTHitSIMDPacket::RTHitSIMDPacket(const RTHitSIMDPacket &source) :
     Inherited (),
    _rDist     (source._rDist    ),
    _rU        (source._rU       ),
    _rV        (source._rV       ),
    _uiCacheId (source._uiCacheId),
    _uiObjId   (source._uiObjId  ),
    _uiTriId   (source._uiTriId  )
{
}

inline
RTHitSIMDPacket::~RTHitSIMDPacket(void)
{
}

inline
void RTHitSIMDPacket::reset(void)
{
    _rDist     = SIMDInfinity;
    _rU        = SIMDMinusOne;
    _rV        = SIMDMinusOne;
    _uiCacheId = SIMDZero;
    _uiObjId   = SIMDZero;
    _uiTriId   = SIMDZero;
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

    _rDistA    [uiIdx] = rDist;
    _rUA       [uiIdx] = rU;
    _rVA       [uiIdx] = rV;
    _uiObjIdA  [uiIdx] = uiObjId;
    _uiTriIdA  [uiIdx] = uiTriId;
    _uiCacheIdA[uiIdx] = uiCacheId;
}

inline
void RTHitSIMDPacket::set(const UInt32 mask,
                          const Float4 rDist, 
                          const Float4 rU, 
                          const Float4 rV, 
                          const UInt32 uiObjId,
                          const UInt32 uiTriId,
                          const UInt32 uiCacheId)
{
    Float4 simdMask = SIMDMaskTranslation[mask];

    _rDist     = osgSIMDUpdate(simdMask,            rDist,      _rDist    );
    _rU        = osgSIMDUpdate(simdMask,            rU,         _rU       );
    _rV        = osgSIMDUpdate(simdMask,            rV,         _rV       );
    _uiObjId   = osgSIMDUpdate(simdMask, osgSIMDSet(uiObjId  ), _uiObjId  );
    _uiTriId   = osgSIMDUpdate(simdMask, osgSIMDSet(uiTriId  ), _uiTriId  );
    _uiCacheId = osgSIMDUpdate(simdMask, osgSIMDSet(uiCacheId), _uiCacheId);
}

#if defined(OSG_SIMD_ALTIVEC)
inline
void RTHitSIMDPacket::setAll(const Float4 rDist,
                             const Float4 rU,
                             const Float4 rV,
                             const vector unsigned int uiObjId,
                             const vector unsigned int uiTriId,
                             const vector unsigned int uiCacheId)
{
    union
    {
        vector unsigned int sseObjId;
        UInt32 arrayObjId[4];
    };  
    union
    {
        vector unsigned int sseTriId;
        UInt32 arrayTriId[4];
    };  
    union
    {
        vector unsigned int sseCacheId;
        UInt32 arrayCacheId[4];
    }; 
    union
    {
        Float4 sseRDist;
        Real32 arrayRDist[4];
    }; 
    union
    {
        Float4 sseRU;
        Real32 arrayRU[4];
    }; 
    union
    {
        Float4 sseRV;
        Real32 arrayRV[4];
    };

    sseObjId   = uiObjId;
    sseTriId   = uiTriId;
    sseCacheId = uiCacheId;
    sseRDist   = rDist;
    sseRU      = rU;
    sseRV      = rV;

    for(UInt32 i = 0; i < 4; ++i)
    {
        _uiObjId  [i] = arrayObjId  [i];
        _uiTriId  [i] = arrayTriId  [i];
        _uiCacheId[i] = arrayCacheId[i];
        _rDist    [i] = arrayRDist  [i];
        _rU       [i] = arrayRU     [i];
        _rV       [i] = arrayRV     [i];
    }
}
#endif			 

inline
Real32 RTHitSIMDPacket::getDist(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _rDistA[uiIdx];
}

inline
Real32 RTHitSIMDPacket::getU(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _rUA[uiIdx];
}

inline
Real32 RTHitSIMDPacket::getV(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _rVA[uiIdx];
}


inline
UInt32 RTHitSIMDPacket::getObjId(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _uiObjIdA[uiIdx];
}

inline
UInt32 RTHitSIMDPacket::getTriId(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _uiTriIdA[uiIdx];
}

inline
UInt32 RTHitSIMDPacket::getCacheId(UInt32 uiIdx)
{
    OSG_ASSERT(uiIdx < NumHits);

    return _uiCacheIdA[uiIdx];
}

inline 
void RTHitSIMDPacket::operator =(const RTHitSIMDPacket &source)
{
    Inherited::operator =(source);

    _rDist     = source._rDist;
    _rU        = source._rU;
    _rV        = source._rV;
    _uiCacheId = source._uiCacheId;
    _uiObjId   = source._uiObjId;
    _uiTriId   = source._uiTriId;
}

OSG_END_NAMESPACE
