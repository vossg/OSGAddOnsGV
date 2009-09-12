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
                                      RTHitPacket &oHit)
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

    oHit.set(f, lambda, mue, _uiObjId, _uiTriId);
}

inline
void RTTriAccelBarycentric::intersect(RTRaySIMDPacket &oRay, 
                                      RTHitSIMDPacket &oHit)
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

    oHit.set(f, lambda, mue, _uiObjId, _uiTriId);
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
                                          RTHitPacket &oHit)
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

    oHit.set(t_plane, beta, gamma, _uiObjId, _uiTriId);
}


inline
void RTTriAccelBarycentricVer1::intersect(RTRaySIMDPacket &oRay, 
                                          RTHitSIMDPacket &oHit)
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

    oHit.set(t_plane, beta, gamma, _uiObjId, _uiTriId);
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
                                          RTHitPacket &oHit)
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

    oHit.set(t, b1, b2, _uiObjId, _uiTriId);
}


inline
void RTTriAccelBarycentricVer2::intersect(RTRaySIMDPacket &oRay, 
                                          RTHitSIMDPacket &oHit)
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

    oHit.set(t, b1, b2, _uiObjId, _uiTriId);
}

OSG_END_NAMESPACE
