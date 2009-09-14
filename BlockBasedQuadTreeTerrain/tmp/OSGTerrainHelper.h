/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2007 by the OpenSG Forum                   *
 *                                                                           *
 *                            www.opensg.org                                 *
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
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Notes                                      *
 *                                                                           *
 * Implementation based on the original thesis work by Julien Koenen         *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------

#ifndef _OSGTERRAINTOOLS_H_
#define _OSGTERRAINTOOLS_H_

#include "OSGBaseTypes.h"
#include "OSGBaseFunctions.h"
#include "OSGImage.h"
#include "OSGSimpleSHLChunk.h"
#include "OSGTextureObjChunk.h"
#include "OSGGeoReferenceAttachment.h"

#include <vector>

OSG_BEGIN_NAMESPACE

template<class T> inline
T osgSqr(T val)
{
    return val * val;
};

inline
unsigned int getNextPowerOf2( unsigned int x )
{
    for( unsigned int bit = 0; bit < 31; ++bit )
    {
        if( ( 1U << bit ) >= x )
        {
            return 1 << bit;
        }
    }

    return 0;
}

template<typename T> inline
T clamp(T x, T min, T max)
{
    assert(min <= max);

    if(x < min)
    {
        return min;
    }
    else if(x > max)
    {
        return max;
    }
    else
    {
        return x;
    }
}

inline
int lerp(int a, int b, float t)
{
    int size = b - a;

    int result = int( (float(a)) + ((float(size)) * t) );    

    return result;
}

inline
unsigned int getCeil(float f)
{
    typedef unsigned int gcui;

    return gcui(osgCeil(f));
}


template<class T>
class FixedAllocator
{
  protected:

    typedef std::vector<T  > Storage;
    typedef std::vector<T *> FreeStorage;
 
    Storage     _vStorage;
    FreeStorage _vFreeStorage;

  public:

    typedef typename std::vector<T>::iterator StorageIt;

    void setSize(UInt32 iSize)
    {
        _vStorage.resize(iSize);

        _vFreeStorage.clear();
        _vFreeStorage.reserve(iSize);

        for(UInt32 i = 0; i < iSize; ++i)
        {
            _vFreeStorage.push_back(&(_vStorage[i]));
        }
    }

    StorageIt begin(void)
    {
        return _vStorage.begin();
    }

    StorageIt end  (void)
    {
        return _vStorage.end();
    }

    T    *allocate(void    )
    {
        if(_vFreeStorage.empty() == true)
        {
            return NULL;
        }

        T *returnValue = _vFreeStorage.back();

        _vFreeStorage.pop_back();

        return returnValue;
    }

    void  free    (T *pElem)
    {
        _vFreeStorage.push_back(pElem);
    }

    bool isEmpty(void)
    {
        return _vFreeStorage.empty();
    }
};

enum BbqRenderCacheType
{
    BbqRenderCacheType_HeightData,
    BbqRenderCacheType_TextureData,
    BbqRenderCacheType_Count
};

class ImageBlockAccessorX
{
  protected:
    
    ImageUnrecPtr _pImage;

  public:

    ImageBlockAccessorX(void);
    ~ImageBlockAccessorX(void);

    bool open(const std::string filename);
    bool isOpen(void);

    Vec2i getSize(void);

    Image::Type        getType  (void);
    Image::PixelFormat getFormat(void);

    bool readBlockRGB(Vec2i  vSampleOrigin,
                      int    iTextureSize,
                      UInt8 *pTarget,
                      int    iTargetSizeBytes);

    bool readBlockA16(Vec2i   vSampleOrigin,
                      int     iTextureSize,
                      UInt16 *pTarget,
                      int     iTargetSizeBytes);

    bool readBlockA16(Vec2i   vSampleOrigin,
                      int     iTextureSize,
                      Int16  *pTarget,
                      int     iTargetSizeBytes);
  
    GeoReferenceAttachment *getGeoRef(void);
};

#if 0
class TiledImageBlockAccessor
{
  protected:
    
    std::vector<ImageBlockAccessorPtr> _vImages;

  public:

    TiledImageBlockAccessor(void);
    ~TiledImageBlockAccessor(void);

    bool open(const std::string filename);
    bool isOpen(void);

    Vec2i getSize(void);

    Image::Type        getType  (void);
    Image::PixelFormat getFormat(void);

    bool readBlockRGB(Vec2i  vSampleOrigin,
                      int    iTextureSize,
                      UInt8 *pTarget,
                      int    iTargetSizeBytes);

    bool readBlockA16(Vec2i   vSampleOrigin,
                      int     iTextureSize,
                      UInt16 *pTarget,
                      int     iTargetSizeBytes);

    bool readBlockA16(Vec2i   vSampleOrigin,
                      int     iTextureSize,
                      Int16  *pTarget,
                      int     iTargetSizeBytes);
  
    GeoReferenceAttachment *getGeoRef(void);
};
#endif

enum BufferUsage
{
    BufferUsage_Static_WriteOnly,
    BufferUsage_Static_ReadWrite,
    BufferUsage_Stream_WriteOnly,
    BufferUsage_Stream_ReadWrite,
    BufferUsage_Dynamic_WriteOnly,
    BufferUsage_Dynamic_ReadWrite,
    BufferUsage_Count
};


class OpenGLGpuBuffer
{
  protected:

    GLenum      _eGLType;
    GLenum      _eGLUsage;
    GLuint      _uiBufferId;
    int         _iBufferSize;
    BufferUsage _eBufferUsage;

  public:

    OpenGLGpuBuffer(void);

    bool create(GLenum type, 
                int size, 
                BufferUsage usage );
 

    bool uploadData(const void* sourceData, 
                    int targetOffset, 
                    int size );

    GLuint              getBufferId() const;

    void destroy(void);
};

class OpenGLShader
{
    SimpleSHLChunkUnrecPtr _pShader;

  public:

    OpenGLShader(void);

    bool loadVtxFromFile(const Char8 *szFilename);

    bool loadFrgFromFile(const Char8 *szFilename);

#if 0
    GLhandleARB getProgramHandle(void)
    {
        return 0;
    }
#endif

    void activate  (DrawEnv *pEnv);
    void deactivate(DrawEnv *pEnv);
    void update    (DrawEnv *pEnv);

    void setSampler(const Char8 *szName, Int32 iVal);
    void setUniform(const Char8 *szName, Real32 rVal);
    void setUniform(const Char8 *szName, Vec2f  vVal);
};

class OpenGLTexture
{

    TextureObjChunkUnrecPtr _pTexture;
    bool                    _bCompressed;

  public:

    OpenGLTexture(void);

    bool loadFromFile(const Char8 *szFilename);
    bool create2D(Image::PixelFormat pf, int iWidth, int iHeight, int unknown);

    bool upload2DRect( int iX, 
                       int iY,
                       const void *pData,
                       unsigned int  vSize);

    void disable(GLenum eTexUnit, DrawEnv *pEnv);

    void enable(GLenum eTexUnit, DrawEnv *pEnv);

    void destroy(void);
};

inline
void projectPnt(Pnt3f  &result, 
                Real32  lattitudeDeg, 
                Real32  longitudeDeg, 
                Real32  height)
{
    Real32 rSemiMajorAxis = 6378.137;
    Real32 rSemiMinorAxis = 6356.7523142;


    Real32 rSemiMajorAxisSquare = rSemiMajorAxis * rSemiMajorAxis;


    Real32 e2 = (rSemiMajorAxisSquare - 
                 rSemiMinorAxis * rSemiMinorAxis) / (rSemiMajorAxisSquare);

    Real32 cosTheta = osgCos(osgDegree2Rad(lattitudeDeg));
    Real32 sinTheta = osgSin(osgDegree2Rad(lattitudeDeg));

    float v = rSemiMajorAxis / osgSqrt(1 - (e2 * sinTheta * sinTheta));

    v += height;

    Real32 cosPhi = osgCos(osgDegree2Rad(longitudeDeg));
    Real32 sinPhi = osgSin(osgDegree2Rad(longitudeDeg));

    result[0] = cosTheta * sinPhi * v;
    result[1] = sinTheta          * ((1 - e2) * v);
    result[2] = cosTheta * cosPhi * v;
}

#if 1
inline
void backProjectPnt(Vec3f &result, 
                    Vec3f &currpos)
{
#if 0
    Real32 rSemiMajorAxis = 6378.137;
    Real32 rSemiMinorAxis = 6356.7523142;


    Real32 rSemiMajorAxisSquare = rSemiMajorAxis * rSemiMajorAxis;


    Real32 e2 = (rSemiMajorAxisSquare - 
                 rSemiMinorAxis * rSemiMinorAxis) / (rSemiMajorAxisSquare);

    Real32 cosTheta = osgCos(osgDegree2Rad(lattitudeDeg));
    Real32 sinTheta = osgSin(osgDegree2Rad(lattitudeDeg));

    float v = rSemiMajorAxis / osgSqrt(1 - (e2 * sinTheta * sinTheta));

    v += height;

    Real32 cosPhi = osgCos(osgDegree2Rad(longitudeDeg));
    Real32 sinPhi = osgSin(osgDegree2Rad(longitudeDeg));

    result[0] = cosTheta * sinPhi * v;
    result[1] = sinTheta          * ((1 - e2) * v);
    result[2] = cosTheta * cosPhi * v;
#endif

    double W;        /* distance from Z axis */
    double W2;       /* square of distance from Z axis */
    double T0;       /* initial estimate of vertical component */
    double T1;       /* corrected estimate of vertical component */
    double S0;       /* initial estimate of horizontal component */
    double S1;       /* corrected estimate of horizontal component */
    double Sin_B0;   /* sin(B0), B0 is estimate of Bowring aux variable */
    double Sin3_B0;  /* cube of sin(B0) */
    double Cos_B0;   /* cos(B0) */
    double Sin_p1;   /* sin(phi1), phi1 is estimated latitude */
    double Cos_p1;   /* cos(phi1) */
    double Rn;       /* Earth radius at location */
    double Sum;      /* numerator of cos(phi1) */
    int At_Pole;     /* indicates location is in polar region */

#define X currpos[2]
#define Y currpos[0]
#define Z currpos[1]

#define Longitude (&(result[0]))
#define Latitude  (&(result[1]))
#define Height    (&(result[2]))
#define PI_OVER_2  (PI / 2.0e0)
#define COS_67P5   0.38268343236508977 
#define PI           3.14159265358979323846
#define AD_C       1.0026000

    static const int FALSE = 0;
    static const int TRUE = 0;

    Real32 a = 6378.137;
    Real32 b = 6356.7523142;

    double Geocent_a = a;
    double Geocent_b = b;
    double Geocent_a2 = a * a;
    double Geocent_b2 = b * b;
    double Geocent_e2 = (Geocent_a2 - Geocent_b2) / Geocent_a2;
    double Geocent_ep2 = (Geocent_a2 - Geocent_b2) / Geocent_b2;


    At_Pole = FALSE;
    if (X != 0.0)
    {
        *Longitude = atan2(Y,X);
    }
    else
    {
        if (Y > 0)
        {
            *Longitude = PI_OVER_2;
        }
        else if (Y < 0)
        {
            *Longitude = -PI_OVER_2;
        }
        else
        {
            At_Pole = TRUE;
            *Longitude = 0.0;
            if (Z > 0.0)
            {  /* north pole */
                *Latitude = PI_OVER_2;
            }
            else if (Z < 0.0)
            {  /* south pole */
                *Latitude = -PI_OVER_2;
            }
            else
            {  /* center of earth */
                *Latitude = PI_OVER_2;
                *Height = -Geocent_b;
                return;
            } 
        }
    }
    W2 = X*X + Y*Y;
    W = sqrt(W2);
    T0 = Z * AD_C;
    S0 = sqrt(T0 * T0 + W2);
    Sin_B0 = T0 / S0;
    Cos_B0 = W / S0;
    Sin3_B0 = Sin_B0 * Sin_B0 * Sin_B0;
    T1 = Z + Geocent_b * Geocent_ep2 * Sin3_B0;
    Sum = W - Geocent_a * Geocent_e2 * Cos_B0 * Cos_B0 * Cos_B0;
    S1 = sqrt(T1*T1 + Sum * Sum);
    Sin_p1 = T1 / S1;
    Cos_p1 = Sum / S1;
    Rn = Geocent_a / sqrt(1.0 - Geocent_e2 * Sin_p1 * Sin_p1);
    if (Cos_p1 >= COS_67P5)
    {
        *Height = W / Cos_p1 - Rn;
    }
    else if (Cos_p1 <= -COS_67P5)
    {
        *Height = W / -Cos_p1 - Rn;
    }
    else
    {
        *Height = Z / Sin_p1 + Rn * (Geocent_e2 - 1.0);
    }
    if (At_Pole == FALSE)
    {
        *Latitude = atan(Sin_p1 / Cos_p1);
    }

#undef X
#undef Y
#undef Z

#undef Longitude
#undef Latitude
#undef Height   

}
#endif


OSG_END_NAMESPACE

#endif // #ifdef _OSGTERRAINTOOLS_H_
