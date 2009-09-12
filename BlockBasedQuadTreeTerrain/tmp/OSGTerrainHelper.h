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
#include "OSGSHLChunk.h"
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

    int result = int( ((float)a) + (((float)size) * t) );    

    return result;
}

inline
unsigned int getCeil(float f)
{
    return (unsigned int) osgCeil(f);
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

class ImageBlockAccessor
{
  protected:
    
    ImagePtr _pImage;

  public:

    ImageBlockAccessor(void);
    ~ImageBlockAccessor(void);

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
  
    GeoReferenceAttachmentPtr getGeoRef(void);
};

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
    SHLChunkPtr _pShader;

  public:

    OpenGLShader(void);

    bool loadVtxFromFile(Char8 *szFilename);

    bool loadFrgFromFile(Char8 *szFilename);

#if 0
    GLhandleARB getProgramHandle(void)
    {
        return 0;
    }
#endif

    void activate  (DrawEnv *pEnv);
    void deactivate(DrawEnv *pEnv);
    void update    (DrawEnv *pEnv);

    void setSampler(Char8 *szName, Int32 iVal);
    void setUniform(Char8 *szName, Real32 rVal);
    void setUniform(Char8 *szName, Vec2f  vVal);
};

class OpenGLTexture
{

    TextureObjChunkPtr _pTexture;
    bool               _bCompressed;

  public:

    OpenGLTexture(void);

    bool loadFromFile(Char8 *szFilename);
    bool create2D(Image::PixelFormat pf, int iWidth, int iHeight, int unknown);

    bool upload2DRect( int iX, 
                       int iY,
                       const void *pData,
                       unsigned int  vSize);

    void disable(GLenum eTexUnit, DrawEnv *pEnv);

    void enable(GLenum eTexUnit, DrawEnv *pEnv);

    void destroy(void);
};

OSG_END_NAMESPACE

#endif // #ifdef _OSGTERRAINTOOLS_H_
