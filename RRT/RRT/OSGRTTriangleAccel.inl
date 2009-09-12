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

#define ku aMod[_uiProj + 1]
#define kv aMod[_uiProj + 2]

inline
void RTTriAccelBarycentric::intersect(RTRayPacket &oRay, 
                                      RTHitPacket &oHit,
                                      UInt32       uiCacheId)
{
    static const UInt32 aMod[] = {0, 1, 2, 0, 1};

    const Real32 nd = 1.f / 
        (      oRay.getDir()[_uiProj] + 
         _nU * oRay.getDir()[ku     ] + 
         _nV * oRay.getDir()[kv     ]);

    const Real32 f  = (_nD - 
                             oRay.getOrigin()[_uiProj] - 
                       _nU * oRay.getOrigin()[ku     ] -
                       _nV * oRay.getOrigin()[kv     ]) * nd;


    if(!(oHit.getDist() > f && f > 0.00001))
        return;

    const float hu = (oRay.getOrigin()[ku] + f * oRay.getDir()[ku]);
    const float hv = (oRay.getOrigin()[kv] + f * oRay.getDir()[kv]);

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
             _nU * vRayDirs[i][ku     ] + 
             _nV * vRayDirs[i][kv     ]);

        const Real32 f  = (_nD - 
                                 vOrigin[_uiProj] - 
                           _nU * vOrigin[ku     ] -
                           _nV * vOrigin[kv     ]) * nd;
#else
        const Real32 nd = 1.f / 
            (      oRay.getDirVec(i)[_uiProj] + 
             _nU * oRay.getDirVec(i)[ku     ] + 
             _nV * oRay.getDirVec(i)[kv     ]);

        const Real32 f  = (_nD - 
                                 oRay.getOriginPnt()[_uiProj] - 
                           _nU * oRay.getOriginPnt()[ku     ] -
                           _nV * oRay.getOriginPnt()[kv     ]) * nd;
#endif

        if(!(oHit.getDist(i) > f && f > 0.00001))
            return; //continue;

#ifndef OSG_SIMD_RAYPACKET_DEBUG
        const float hu = (vOrigin[ku] + f * vRayDirs[i][ku]);
        const float hv = (vOrigin[kv] + f * vRayDirs[i][kv]);
#else
        const float hu = (oRay.getOriginPnt()[ku] + f * oRay.getDirVec(i)[ku]);
        const float hv = (oRay.getOriginPnt()[kv] + f * oRay.getDirVec(i)[kv]);
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
        osgSIMDMul(osgSIMDSet(_nU), oRay.getDir(ku)),
        osgSIMDMul(osgSIMDSet(_nV), oRay.getDir(kv)));

#if 0
    const Real32 nd = 1.f / 
        (      oRay.getDir(i)[_uiProj] + 
         _nU * oRay.getDir(i)[ku     ] + 
         _nV * oRay.getDir(i)[kv     ]);
#endif

    const Real32 f  = (_nD - 
                             oRay.getOriginComp(_uiProj) - 
                       _nU * oRay.getOriginComp(ku     ) -
                       _nV * oRay.getOriginComp(kv     ));  // * nd;

    
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

    Float4 mask = osgSIMDAnd(osgSIMDCmpGE(fDist4, f4),
                             osgSIMDCmpGT(f4, SIMDEps));

    if( (osgSIMDMoveMask(mask) & uiActive) == 0) 
        return;

#if 0
    const float hu = (oRay.getOrigin()[ku] + f * oRay.getDir(i)[ku]);
    const float hv = (oRay.getOrigin()[kv] + f * oRay.getDir(i)[kv]);
#endif
    
    const Float4 hu = osgSIMDAdd(osgSIMDSet(oRay.getOriginComp(ku)),
                                 osgSIMDMul(f4, oRay.getDir(ku)));

    const Float4 hv = osgSIMDAdd(osgSIMDSet(oRay.getOriginComp(kv)),
                                 osgSIMDMul(f4, oRay.getDir(kv)));


#if 0
    const float lambda = (hu * _bNU + hv * _bNV + _bD);
    
    if(lambda < 0.)
        continue;
#endif

    Float4 lambda = osgSIMDAdd(osgSIMDMul(hu, osgSIMDSet(_bNU)),
                               osgSIMDMul(hv, osgSIMDSet(_bNV)));

    lambda = osgSIMDAdd(lambda, osgSIMDSet(_bD));

    mask = osgSIMDAnd(mask, osgSIMDCmpGT(lambda, SIMDZero));
    
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

    mask = osgSIMDAnd(mask, osgSIMDCmpGT(mue, SIMDZero));

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



//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

inline
RTTriAccelBarycentricVer1::RTTriAccelBarycentricVer1(void) :
    _a      (   ),
    _b      (   ),
    _c      (   ),
    _uiObjId(0  ),
    _uiTriId(0  )
{
}

inline
RTTriAccelBarycentricVer1::RTTriAccelBarycentricVer1(
    const RTTriAccelBarycentricVer1 &source) :

    _a      (source._a      ),
    _b      (source._b      ),
    _c      (source._c      ),
    _uiObjId(source._uiObjId),
    _uiTriId(source._uiTriId)
{
}

inline
RTTriAccelBarycentricVer1::~RTTriAccelBarycentricVer1(void)
{
}

inline
void RTTriAccelBarycentricVer1::operator =(
    const RTTriAccelBarycentricVer1 &source)
{
    _a        = source._a;
    _b        = source._b;
    _c        = source._c;
    _uiObjId  = source._uiObjId;
    _uiTriId  = source._uiTriId;
}


inline
void RTTriAccelBarycentricVer1::setup(Pnt3f  A, 
                                      Pnt3f  B, 
                                      Pnt3f  C, 
                                      UInt32 uiObjId,
                                      UInt32 uiTriId)
{
    _a = A;
    _b = B;
    _c = C;

    _uiObjId = uiObjId;
    _uiTriId = uiTriId;
}


inline
void RTTriAccelBarycentricVer1::intersect(RTRayPacket &oRay, 
                                          RTHitPacket &oHit,
                                          UInt32       uiCacheId)
{
    Vec3f b = _c - _a;
    Vec3f c = _b - _a;
    
    Vec3f d = oRay.getOrigin() - _a;
    Vec3f n = c.cross(b);

    n.normalize();

    Real32 t_plane = - d.dot(n) / oRay.getDir().dot(n);

    if(t_plane < 0.000001 || t_plane > oHit.getDist())
        return;

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

    Real32 H[3];

    H[u] = oRay.getOrigin()[u] + t_plane * oRay.getDir()[u] - _a[u];
    H[v] = oRay.getOrigin()[v] + t_plane * oRay.getDir()[v] - _a[v];

    Real32 beta = (b[u] * H[v] - b[v] * H[u]) / (b[u] * c[v] - b[v] * c[u]);

    if(beta < 0.f)
        return;

    Real32 gamma = (c[v] * H[u] - c[u] * H[v]) / (b[u] * c[v] - b[v] * c[u]);

    if(gamma < 0.f)
        return;

    if(beta + gamma > 1.f)
        return;

    oHit.set(t_plane, beta, gamma, _uiObjId, _uiTriId, uiCacheId);
}


inline
void RTTriAccelBarycentricVer1::intersect(RTRaySIMDPacket &oRay, 
                                          RTHitSIMDPacket &oHit,
                                          UInt32           uiCacheId,
                                          UInt32           uiActive)
{
#if 0
    Vec3f b = _c - _a;
    Vec3f c = _b - _a;
    
    Vec3f d = oRay.getOrigin() - _a;
    Vec3f n = c.cross(b);

    n.normalize();

    for(UInt32 i = 0; i < RTRaySIMDPacket::NumRays; ++i)
    {
        if(uiActive[i] == 0)
            continue;

        Real32 t_plane = - d.dot(n) / oRay.getDir(i).dot(n);

        if(t_plane < 0.000001 || t_plane > oHit.getDist(i))
            continue;

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
        
        Real32 H[3];

        H[u] = oRay.getOrigin()[u] + t_plane * oRay.getDir(i)[u] - _a[u];
        H[v] = oRay.getOrigin()[v] + t_plane * oRay.getDir(i)[v] - _a[v];

        Real32 beta = (b[u] * H[v] - b[v] * H[u]) / 
                      (b[u] * c[v] - b[v] * c[u]);

        if(beta < 0.f)
            continue;
        
        Real32 gamma = (c[v] * H[u] - c[u] * H[v]) / 
                       (b[u] * c[v] - b[v] * c[u]);

        if(gamma < 0.f)
            continue;
        
        if(beta + gamma > 1.f)
            continue;

        oHit.set(i, t_plane, beta, gamma, _uiObjId, _uiTriId, uiCacheId);
    }
#endif
}



//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

inline
RTTriAccelBarycentricVer2::RTTriAccelBarycentricVer2(void) :
    _a      (   ),
    _b      (   ),
    _c      (   ),
    _uiObjId(0  ),
    _uiTriId(0  )
{
}

inline
RTTriAccelBarycentricVer2::RTTriAccelBarycentricVer2(
    const RTTriAccelBarycentricVer2 &source) :

    _a      (source._a      ),
    _b      (source._b      ),
    _c      (source._c      ),
    _uiObjId(source._uiObjId),
    _uiTriId(source._uiTriId)
{
}

inline
RTTriAccelBarycentricVer2::~RTTriAccelBarycentricVer2(void)
{
}

inline
void RTTriAccelBarycentricVer2::operator =(
    const RTTriAccelBarycentricVer2 &source)
{
    _a        = source._a;
    _b        = source._b;
    _c        = source._c;
    _uiObjId  = source._uiObjId;
    _uiTriId  = source._uiTriId;
}


inline
void RTTriAccelBarycentricVer2::setup(Pnt3f  A, 
                                      Pnt3f  B, 
                                      Pnt3f  C, 
                                      UInt32 uiObjId,
                                      UInt32 uiTriId)
{
    _a = A;
    _b = B;
    _c = C;

    _uiObjId = uiObjId;
    _uiTriId = uiTriId;
}

inline
void RTTriAccelBarycentricVer2::intersect(RTRayPacket &oRay, 
                                          RTHitPacket &oHit,
                                          UInt32       uiCacheId)
{
	Vec3f e1 = _b - _a;
	Vec3f e2 = _c - _a;

	Vec3f s1 = oRay.getDir().cross(e2);

	float divisor = s1.dot(e1);

	if(divisor == 0.)
		return;

	float invDivisor = 1.f / divisor;

	Vec3f d = oRay.getOrigin() - _a;

	float b1 = d.dot(s1) * invDivisor;

	if(b1 < 0. || b1 > 1.)
		return;

	Vec3f s2 = d.cross(e1);

	float b2 = oRay.getDir().dot(s2) * invDivisor;

	if (b2 < 0. || b1 + b2 > 1.)
		return;

	float t = e2.dot(s2) * invDivisor;

	if (t < 0.0001 || t > oHit.getDist())
		return;

    oHit.set(t, b1, b2, _uiObjId, _uiTriId, uiCacheId);
}


inline
void RTTriAccelBarycentricVer2::intersect(RTRaySIMDPacket &oRay, 
                                          RTHitSIMDPacket &oHit,
                                          UInt32           uiCacheId,
                                          UInt32           uiActive  )
{
#if 0
	Vec3f e1 = _b - _a;
	Vec3f e2 = _c - _a;

    for(UInt32 i = 0; i < RTRaySIMDPacket::NumRays; ++i)
    {
        if(uiActive[i] == 0)
            continue;

        Vec3f s1 = oRay.getDir(i).cross(e2);

        float divisor = s1.dot(e1);
        
        if(divisor == 0.)
            continue;

        float invDivisor = 1.f / divisor;
    
        Vec3f d = oRay.getOrigin() - _a;

        float b1 = d.dot(s1) * invDivisor;

        if(b1 < 0. || b1 > 1.)
            continue;

        Vec3f s2 = d.cross(e1);

        float b2 = oRay.getDir(i).dot(s2) * invDivisor;

        if (b2 < 0. || b1 + b2 > 1.)
            continue;

        float t = e2.dot(s2) * invDivisor;

        if (t < 0.0001 || t > oHit.getDist(i))
            continue;

        oHit.set(i, t, b1, b2, _uiObjId, _uiTriId, uiCacheId);
    }
#endif
}

OSG_END_NAMESPACE
