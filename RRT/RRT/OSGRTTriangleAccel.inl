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

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

inline
RTTriAccelBarycentric::RTTriAccelBarycentric(void) :
    _nU     (0.f),
    _nV     (0.f),
    _nD     (0.f),
    _uiProj (0  ),

    _bNU    (0.f),
    _bNV    (0.f),
    _bD     (0.f),
    _uiObjId(0  ),

    _cNU    (0.f),
    _cNV    (0.f),
    _cD     (0.f),
    _uiTriId(0  )
{
}

inline
RTTriAccelBarycentric::RTTriAccelBarycentric(
    const RTTriAccelBarycentric &source) :

    _nU     (source._nU     ),
    _nV     (source._nV     ),
    _nD     (source._nD     ),
    _uiProj (source._uiProj ),

    _bNU    (source._bNU    ),
    _bNV    (source._bNV    ),
    _bD     (source._bD     ),
    _uiObjId(source._uiObjId),

    _cNU    (source._cNU    ),
    _cNV    (source._cNV    ),
    _cD     (source._cD     ),
    _uiTriId(source._uiTriId)
{
}

inline
RTTriAccelBarycentric::~RTTriAccelBarycentric(void)
{
}

inline
void RTTriAccelBarycentric::operator =(
    const RTTriAccelBarycentric &source)
{
    _nU       = source._nU;
    _nV       = source._nV;
    _nD       = source._nD;
    _uiProj   = source._uiProj;

    _bNU      = source._bNU;
    _bNV      = source._bNV;
    _bD       = source._bD;
    _uiObjId  = source._uiObjId;

    _cNU      = source._cNU;
    _cNV      = source._cNV;
    _cD       = source._cD;
    _uiTriId  = source._uiTriId;
}


inline
bool RTTriAccelBarycentric::operator ==(
    const RTTriAccelBarycentric &source) const
{
    return
        (_nU       == source._nU      &&
         _nV       == source._nV      &&
         _nD       == source._nD      &&
         _uiProj   == source._uiProj  &&
         
         _bNU      == source._bNU     &&
         _bNV      == source._bNV     &&
         _bD       == source._bD      &&
         _uiObjId  == source._uiObjId &&
         
         _cNU      == source._cNU     &&
         _cNV      == source._cNV     &&
         _cD       == source._cD      &&
         _uiTriId  == source._uiTriId  );
}

inline
void RTTriAccelBarycentric::putToStream(OutStream &str) const
{
    typedef TypeTraits<UInt32> UInt32TypeTrait;

    union
    {
        Real32 rVal;
        UInt32 uiVal;
    };

    rVal = _nU;
    UInt32TypeTrait::putToStream(uiVal, str);
    str << " ";

    rVal = _nV;
    UInt32TypeTrait::putToStream(uiVal, str);
    str << " ";

    rVal = _nD;
    UInt32TypeTrait::putToStream(uiVal, str);
    str << " ";

    UInt32TypeTrait::putToStream(_uiProj, str);
    str << " ";
    


    rVal = _bNU;
    UInt32TypeTrait::putToStream(uiVal, str);
    str << " ";

    rVal = _bNV;
    UInt32TypeTrait::putToStream(uiVal, str);
    str << " ";

    rVal = _bD;
    UInt32TypeTrait::putToStream(uiVal, str);
    str << " ";

    UInt32TypeTrait::putToStream(_uiObjId, str);
    str << " ";
    


    rVal = _cNU;
    UInt32TypeTrait::putToStream(uiVal, str);
    str << " ";

    rVal = _cNV;
    UInt32TypeTrait::putToStream(uiVal, str);
    str << " ";

    rVal = _cD;
    UInt32TypeTrait::putToStream(uiVal, str);
    str << " ";

    UInt32TypeTrait::putToStream(_uiTriId, str);
    str << " ";
}

inline
bool RTTriAccelBarycentric::getFromCString(const Char8 *inVal)
{
    if(inVal ==  NULL)
        return false;

    union
    {
        Real32 rVal [12];
        UInt32 uiVal[12];
    };


    UInt32 rc = sscanf(inVal, "%u %u %u %u %u %u %u %u %u %u %u %u",
                       &(uiVal[0]),
                       &(uiVal[1]),
                       &(uiVal[2]),
                       &(uiVal[3]),
                       &(uiVal[4]),
                       &(uiVal[5]),
                       &(uiVal[6]),
                       &(uiVal[7]),
                       &(uiVal[8]),
                       &(uiVal[9]),
                       &(uiVal[10]),
                       &(uiVal[11]));

    if(rc != 12)
    {
        return false;
    }

    _nU      = rVal [0 ];
    _nV      = rVal [1 ];
    _nD      = rVal [2 ];
    _uiProj  = uiVal[3 ];

    _bNU     = rVal [4 ];
    _bNV     = rVal [5 ];
    _bD      = rVal [6 ];
    _uiObjId = uiVal[7 ];

    _cNU     = rVal [8 ];
    _cNV     = rVal [9 ];
    _cD      = rVal [10];
    _uiTriId = uiVal[11];

    return true;
}

inline
UInt32 RTTriAccelBarycentric::getBinSize(void) const
{
    return 12 * sizeof(UInt32);
}

inline
void RTTriAccelBarycentric::copyToBin(BinaryDataHandler &pMem) const
{
    pMem.putValue(_nU);
    pMem.putValue(_nV);
    pMem.putValue(_nD);
    pMem.putValue(_uiProj);

    pMem.putValue(_bNU);
    pMem.putValue(_bNV);
    pMem.putValue(_bD);
    pMem.putValue(_uiObjId);

    pMem.putValue(_cNU);
    pMem.putValue(_cNV);
    pMem.putValue(_cD);
    pMem.putValue(_uiTriId);
}

inline
void RTTriAccelBarycentric::copyFromBin(BinaryDataHandler &pMem)
{
    pMem.getValue(_nU);
    pMem.getValue(_nV);
    pMem.getValue(_nD);
    pMem.getValue(_uiProj);

    pMem.getValue(_bNU);
    pMem.getValue(_bNV);
    pMem.getValue(_bD);
    pMem.getValue(_uiObjId);

    pMem.getValue(_cNU);
    pMem.getValue(_cNV);
    pMem.getValue(_cD);
    pMem.getValue(_uiTriId);
}

inline
void RTTriAccelBarycentric::setup(Pnt3f  A, 
                                  Pnt3f  B, 
                                  Pnt3f  C, 
                                  UInt32 uiObjId,
                                  UInt32 uiTriId)
{
    _uiObjId = uiObjId;
    _uiTriId = uiTriId;

    Vec3f c = B - A;
    Vec3f b = C - A;

    Vec3f n = c.cross(b);

    n.normalize();

    UInt32 k, u, v;

    if(osgAbs(n[0]) > osgAbs(n[1])) 
    { 
        if(osgAbs(n[0]) > osgAbs(n[2])) 
            k = 0; 
        else
            k = 2;
    }
    else 
    { 
        if(osgAbs(n[1]) > osgAbs(n[2]))
            k = 1; 
        else
            k = 2;
    }

    u = (k + 1) % 3;
    v = (k + 2) % 3;

    _uiProj = k;

    n /= n[k];

    _nU = n[u];
    _nV = n[v];
    _nD = n.dot(A);

    Real32 rDiv = (b[u] * c[v] - b[v] * c[u]);

    _bNU = -b[v] / rDiv;
    _bNV =  b[u] / rDiv;

    _bD  = (b[v] * A[u] - b[u] * A[v]) / rDiv;

    _cNU =  c[v] / rDiv;
    _cNV = -c[u] / rDiv;

    _cD = (c[u] * A[v] - c[v] * A[u]) / rDiv;
}

#define KU aMod[_uiProj + 1]
#define KV aMod[_uiProj + 2]

inline
void RTTriAccelBarycentric::intersect(RTRayPacket &oRay, 
                                      RTHitPacket &oHit,
                                      UInt32       uiCacheId)
{
    static const UInt32 aMod[] = {0, 1, 2, 0, 1};

    const Real32 nd = 1.f / 
        (      oRay.getDir()[_uiProj] + 
         _nU * oRay.getDir()[KU     ] + 
         _nV * oRay.getDir()[KV     ]);

    const Real32 f  = (_nD - 
                             oRay.getOrigin()[_uiProj] - 
                       _nU * oRay.getOrigin()[KU     ] -
                       _nV * oRay.getOrigin()[KV     ]) * nd;


    if(!(oHit.getDist() > f && f > 0.00001))
        return;

    const float hu = (oRay.getOrigin()[KU] + f * oRay.getDir()[KU]);
    const float hv = (oRay.getOrigin()[KV] + f * oRay.getDir()[KV]);

    const float lambda = (hu * _bNU + hv * _bNV + _bD);

    if(lambda < 0.)
        return;

    const float mue = (hu * _cNU + hv * _cNV + _cD);

    if(mue < 0.)
        return;

    if(lambda + mue > 1.)
        return;

    oHit.set(f, lambda, mue, _uiObjId, _uiTriId, uiCacheId);
}

#ifndef OSG_SIMD_RAYPACKET_DEBUG
inline
void RTTriAccelBarycentric::intersectSingle(Vec3f           *vRayDirs,
                                            Pnt3f           &vOrigin,
                                            RTHitSIMDPacket &oHit,
                                            UInt32           uiCacheId,
                                            UInt32           uiActive  )
#else
inline
void RTTriAccelBarycentric::intersectSingle(RTRaySIMDPacket &oRay, 
                                            RTHitSIMDPacket &oHit,
                                            UInt32           uiCacheId,
                                            UInt32           uiActive)
#endif
{
    static const UInt32 aMod[] = {0, 1, 2, 0, 1};

//    for(UInt32 i = 0; i < 4; ++i)

    UInt32 i = uiActive;
    {
#ifndef OSG_SIMD_RAYPACKET_DEBUG
        const Real32 nd = 1.f / 
            (      vRayDirs[i][_uiProj] + 
             _nU * vRayDirs[i][KU     ] + 
             _nV * vRayDirs[i][KV     ]);

        const Real32 f  = (_nD - 
                                 vOrigin[_uiProj] - 
                           _nU * vOrigin[KU     ] -
                           _nV * vOrigin[KV     ]) * nd;
#else
        const Real32 nd = 1.f / 
            (      oRay.getDirVec(i)[_uiProj] + 
             _nU * oRay.getDirVec(i)[KU     ] + 
             _nV * oRay.getDirVec(i)[KV     ]);

        const Real32 f  = (_nD - 
                                 oRay.getOriginPnt()[_uiProj] - 
                           _nU * oRay.getOriginPnt()[KU     ] -
                           _nV * oRay.getOriginPnt()[KV     ]) * nd;
#endif

        if(!(oHit.getDist(i) > f && f > 0.00001))
            return; //continue;

#ifndef OSG_SIMD_RAYPACKET_DEBUG
        const float hu = (vOrigin[KU] + f * vRayDirs[i][KU]);
        const float hv = (vOrigin[KV] + f * vRayDirs[i][KV]);
#else
        const float hu = (oRay.getOriginPnt()[KU] + f * oRay.getDirVec(i)[KU]);
        const float hv = (oRay.getOriginPnt()[KV] + f * oRay.getDirVec(i)[KV]);
#endif
        const float lambda = (hu * _bNU + hv * _bNV + _bD);
        
        if(lambda < 0.)
            return; //continue;

        const float mue = (hu * _cNU + hv * _cNV + _cD);
        
        if(mue < 0.)
            return; //continue;
        
        if(lambda + mue > 1.)
            return; //continue;
        
        oHit.set(i, f, lambda, mue, _uiObjId, _uiTriId, uiCacheId);
    }
}

inline
void RTTriAccelBarycentric::intersect(RTRaySIMDPacket &oRay, 
                                      RTHitSIMDPacket &oHit,
                                      UInt32           uiCacheId,
                                      UInt32           uiActive)
{
    static const UInt32 aMod[] = {0, 1, 2, 0, 1};


    Float4 nd = osgSIMDAdd(
        osgSIMDMul(osgSIMDSet(_nU), oRay.getDir(KU)),
        osgSIMDMul(osgSIMDSet(_nV), oRay.getDir(KV)));

#if 0
    const Real32 nd = 1.f / 
        (      oRay.getDir(i)[_uiProj] + 
         _nU * oRay.getDir(i)[KU     ] + 
         _nV * oRay.getDir(i)[KV     ]);
#endif

    const Real32 f  = (_nD - 
                             oRay.getOriginComp(_uiProj) - 
                       _nU * oRay.getOriginComp(KU     ) -
                       _nV * oRay.getOriginComp(KV     ));  // * nd;

    
    nd = osgSIMDAdd(nd, oRay.getDir(_uiProj));
    
    nd = osgSIMDInvert(nd);

    Float4 f4 = osgSIMDMul(nd, osgSIMDSet(f));

    Float4 fDist4 = osgSIMDSet(oHit.getDist(3), 
                               oHit.getDist(2),
                               oHit.getDist(1),
                               oHit.getDist(0));

#if 0
    if(!(oHit.getDist(i) > f && f > 0.00001))
        continue;
#endif

    Float4 mask = osgSIMDAnd(osgSIMDCmpGT(fDist4, f4),
                             osgSIMDCmpGT(f4, SIMDEps));

    if( (osgSIMDMoveMask(mask) & uiActive) == 0) 
        return;

#if 0
    const float hu = (oRay.getOrigin()[KU] + f * oRay.getDir(i)[KU]);
    const float hv = (oRay.getOrigin()[KV] + f * oRay.getDir(i)[KV]);
#endif
    
    const Float4 hu = osgSIMDAdd(osgSIMDSet(oRay.getOriginComp(KU)),
                                 osgSIMDMul(f4, oRay.getDir(KU)));

    const Float4 hv = osgSIMDAdd(osgSIMDSet(oRay.getOriginComp(KV)),
                                 osgSIMDMul(f4, oRay.getDir(KV)));


#if 0
    const float lambda = (hu * _bNU + hv * _bNV + _bD);
    
    if(lambda < 0.)
        continue;
#endif

    Float4 lambda = osgSIMDAdd(osgSIMDMul(hu, osgSIMDSet(_bNU)),
                               osgSIMDMul(hv, osgSIMDSet(_bNV)));

    lambda = osgSIMDAdd(lambda, osgSIMDSet(_bD));

    mask = osgSIMDAnd(mask, osgSIMDCmpGE(lambda, SIMDZero));
    
    if( (osgSIMDMoveMask(mask) & uiActive) == 0)
        return;
   

#if 0
    const float mue = (hu * _cNU + hv * _cNV + _cD);
    
    if(mue < 0.)
        continue;
#endif

    Float4 mue = osgSIMDAdd(osgSIMDMul(hu, osgSIMDSet(_cNU)),
                            osgSIMDMul(hv, osgSIMDSet(_cNV)));

    mue = osgSIMDAdd(mue, osgSIMDSet(_cD));

    mask = osgSIMDAnd(mask, osgSIMDCmpGE(mue, SIMDZero));

    if( (osgSIMDMoveMask(mask) & uiActive) == 0)
        return;


#if 0
    if(lambda + mue > 1.)
        continue;
#endif

    const Float4 finalMask = osgSIMDAnd(osgSIMDCmpLE(osgSIMDAdd(lambda, mue),
                                                     SIMDOne),
                                        mask);

    const UInt32 intFinalMask = osgSIMDMoveMask(finalMask) & uiActive;

    if(intFinalMask == 0)
        return;

    oHit.set( intFinalMask, 
              f4, 
              lambda, 
              mue, 
             _uiObjId, 
             _uiTriId, 
              uiCacheId);
}



inline
void RTTriAccelBarycentric::intersect(RTRayFullSIMDPacket &oRay, 
                                      RTHitSIMDPacket     &oHit,
                                      UInt32               uiCacheId,
                                      UInt32               uiActive )
{
    static const UInt32 aMod[] = {0, 1, 2, 0, 1};


    Float4 nd = osgSIMDAdd(
        osgSIMDMul(osgSIMDSet(_nU), oRay.getDir(KU)),
        osgSIMDMul(osgSIMDSet(_nV), oRay.getDir(KV)));

#if 0
    const Real32 nd = 1.f / 
        (      oRay.getDir(i)[_uiProj] + 
         _nU * oRay.getDir(i)[KU     ] + 
         _nV * oRay.getDir(i)[KV     ]);
#endif

    const Real32 f  = (_nD - 
                             oRay.getOriginComp(_uiProj) - 
                       _nU * oRay.getOriginComp(KU     ) -
                       _nV * oRay.getOriginComp(KV     ));  // * nd;

    
    nd = osgSIMDAdd(nd, oRay.getDir(_uiProj));
    
    nd = osgSIMDInvert(nd);

    Float4 f4 = osgSIMDMul(nd, osgSIMDSet(f));

    Float4 fDist4 = osgSIMDSet(oHit.getDist(3), 
                               oHit.getDist(2),
                               oHit.getDist(1),
                               oHit.getDist(0));

#if 0
    if(!(oHit.getDist(i) > f && f > 0.00001))
        continue;
#endif

    Float4 mask = osgSIMDAnd(osgSIMDCmpGT(fDist4, f4),
                             osgSIMDCmpGT(f4, SIMDEps));

    if( (osgSIMDMoveMask(mask) & uiActive) == 0) 
        return;

#if 0
    const float hu = (oRay.getOrigin()[KU] + f * oRay.getDir(i)[KU]);
    const float hv = (oRay.getOrigin()[KV] + f * oRay.getDir(i)[KV]);
#endif
    
    const Float4 hu = osgSIMDAdd(osgSIMDSet(oRay.getOriginComp(KU)),
                                 osgSIMDMul(f4, oRay.getDir(KU)));

    const Float4 hv = osgSIMDAdd(osgSIMDSet(oRay.getOriginComp(KV)),
                                 osgSIMDMul(f4, oRay.getDir(KV)));


#if 0
    const float lambda = (hu * _bNU + hv * _bNV + _bD);
    
    if(lambda < 0.)
        continue;
#endif

    Float4 lambda = osgSIMDAdd(osgSIMDMul(hu, osgSIMDSet(_bNU)),
                               osgSIMDMul(hv, osgSIMDSet(_bNV)));

    lambda = osgSIMDAdd(lambda, osgSIMDSet(_bD));

    mask = osgSIMDAnd(mask, osgSIMDCmpGE(lambda, SIMDZero));
    
    if( (osgSIMDMoveMask(mask) & uiActive) == 0)
        return;
   

#if 0
    const float mue = (hu * _cNU + hv * _cNV + _cD);
    
    if(mue < 0.)
        continue;
#endif

    Float4 mue = osgSIMDAdd(osgSIMDMul(hu, osgSIMDSet(_cNU)),
                            osgSIMDMul(hv, osgSIMDSet(_cNV)));

    mue = osgSIMDAdd(mue, osgSIMDSet(_cD));

    mask = osgSIMDAnd(mask, osgSIMDCmpGE(mue, SIMDZero));

    if( (osgSIMDMoveMask(mask) & uiActive) == 0)
        return;


#if 0
    if(lambda + mue > 1.)
        continue;
#endif

    const Float4 finalMask = osgSIMDAnd(osgSIMDCmpLE(osgSIMDAdd(lambda, mue),
                                                     SIMDOne),
                                        mask);

    const UInt32 intFinalMask = osgSIMDMoveMask(finalMask) & uiActive;

    if(intFinalMask == 0)
        return;

    oHit.set( intFinalMask, 
              f4, 
              lambda, 
              mue, 
             _uiObjId, 
             _uiTriId, 
              uiCacheId);
}




OSG_END_NAMESPACE
