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

#define GL_GLEXT_PROTOTYPES

#include "GL/gl.h"
#include "OSGGLU.h"

#include "OSGBbqTerrainRenderer.h"
#include "OSGBbqTerrainNode.h"

#include "OSGDrawEnv.h"

#include <limits>

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE



// todo: put this function somewhere (terrain-tools):
template<typename IndexType>
void createBlockIndicesPart(Int32      blockSize, 
                            IndexType *ptr, 
                            Int32      partSize, 
                            Int32      partOffsetX, 
                            Int32      partOffsetY)
{
    assert(blockSize >= 2);
    
    for( int y = 0; y < partSize; ++y )
    {
        const int partY = partOffsetY + y;
        
        IndexType* indexPtr = 
            ptr + 6 * ( partY * ( blockSize - 1 ) + partOffsetX );
        
        const IndexType row0 = ( IndexType )( partY * blockSize );
        const IndexType row1 = ( IndexType )( ( partY + 1 ) * blockSize );
        
        bool leftToRight = ( y % 2 ) == 0;
        
        for( int x = 0; x < partSize; ++x )
        {
            int partX = partOffsetX + x;
            
            if( !leftToRight )
            {
                partX = partOffsetX + ( partSize - 1 ) - x;
            }
            
            const IndexType col0 = ( IndexType ) partX;
            const IndexType col1 = ( IndexType )( partX + 1 );
            
            const IndexType idx0 = row0 + col0;
            const IndexType idx1 = row0 + col1;
            const IndexType idx2 = row1 + col0;
            const IndexType idx3 = row1 + col1;
            
            assert( idx0 < std::numeric_limits< IndexType >::max() );
            assert( idx1 < std::numeric_limits< IndexType >::max() );
            assert( idx2 < std::numeric_limits< IndexType >::max() );
            assert( idx3 < std::numeric_limits< IndexType >::max() );
            
            *indexPtr++ = idx0;
            *indexPtr++ = idx2;
            *indexPtr++ = idx1;
            
            *indexPtr++ = idx1;
            *indexPtr++ = idx2;
            *indexPtr++ = idx3;
        }
    }
}

// todo: put this function somewhere (terrain-tools):
template<typename IndexType>
void createBlockIndices(int blockSize, std::vector<IndexType> &indices)
{
    assert( blockSize >= 2 );
    
    // todo: this currently creates very naive indices (not optimized at all)
    
    // we know how much indices we produce:
    indices.resize( 3 * 2 * ( blockSize - 1 ) * ( blockSize - 1 ), 0 );
    
    bool optIndices = true;
    if( optIndices && blockSize == 65 )
    {
        for( int y = 0; y < 4; ++y )
        {   
            for( int x = 0; x < 4; ++x )
            {
                createBlockIndicesPart( blockSize, &indices[ 0 ], 
                                        16, 
                                        x * 16, 
                                        y * 16 );
            }
        }
        return;
    }
    
    IndexType* indexPtr = &indices[ 0 ];
    
    for( int y = 0; y < blockSize - 1; ++y )
    {
        const IndexType row0 = ( IndexType )( y * blockSize );
        const IndexType row1 = ( IndexType )( ( y + 1 ) * blockSize );
        
        for( int x = 0; x < blockSize - 1; ++x )
        {
            const IndexType col0 = ( IndexType ) x;
            const IndexType col1 = ( IndexType )( x + 1 );
            
            const IndexType idx0 = row0 + col0;
            const IndexType idx1 = row0 + col1;
            const IndexType idx2 = row1 + col0;
            const IndexType idx3 = row1 + col1;
            
            assert( idx0 < std::numeric_limits< IndexType >::max() );
            assert( idx1 < std::numeric_limits< IndexType >::max() );
            assert( idx2 < std::numeric_limits< IndexType >::max() );
            assert( idx3 < std::numeric_limits< IndexType >::max() );
            
            *indexPtr++ = idx0;
            *indexPtr++ = idx2;
            *indexPtr++ = idx1;
            
            *indexPtr++ = idx1;
            *indexPtr++ = idx2;
            *indexPtr++ = idx3;
        }
    }
}

#if 0
template< typename IndexType >
void addBlockSkirtIndices( int blockSize, 
                           int skirtId, std::vector< IndexType >& indices )
{
    // add skirt indices:
    int row1 = osgSqr( blockSize ) + skirtId * blockSize;
    
    for( int i = 0; i < blockSize - 1; ++i )
    {
        int row0 = 0;
        
        // two triangles:
        const IndexType col0 = ( IndexType ) i;
        const IndexType col1 = ( IndexType )( i + 1 );
        
        const IndexType idx0 = row0 + col0;
        const IndexType idx1 = row0 + col1;
        const IndexType idx2 = row1 + col0;
        const IndexType idx3 = row1 + col1;
        
        assert( idx0 < std::numeric_limits< IndexType >::max() );
        assert( idx1 < std::numeric_limits< IndexType >::max() );
        assert( idx2 < std::numeric_limits< IndexType >::max() );
        assert( idx3 < std::numeric_limits< IndexType >::max() );
        
        indices.push_back( idx0 );
        indices.push_back( idx2 );
        indices.push_back( idx1 );
        
        indices.push_back( idx1 );
        indices.push_back( idx2 );
        indices.push_back( idx3 );
    }
    
    for( int i = 0; i < databaseInfo_.heightTileSize - 1; ++i )
    {
        IndexType row0 = 0;
        IndexType row1 = ( IndexType )sqr( databaseInfo_.heightTileSize );
        
        // two triangles:
        const IndexType col0 = ( IndexType ) i;
        const IndexType col1 = ( IndexType )( i + 1 );
        
        const IndexType idx0 = row0 + col0;
        const IndexType idx1 = row0 + col1;
        const IndexType idx2 = row1 + col0;
        const IndexType idx3 = row1 + col1;
        
        assert( idx0 < std::numeric_limits< IndexType >::max() );
        assert( idx1 < std::numeric_limits< IndexType >::max() );
        assert( idx2 < std::numeric_limits< IndexType >::max() );
        assert( idx3 < std::numeric_limits< IndexType >::max() );
        
        staticIndices_.push_back( idx0 );
        staticIndices_.push_back( idx1 );
        staticIndices_.push_back( idx2 );
        
        staticIndices_.push_back( idx1 );
        staticIndices_.push_back( idx3 );
    }
    
}
#endif


//-----------------------------------------------------------------------------

BbqRenderOptions::BbqRenderOptions(void) :
    frustum             (     ),
    viewerpos           (     ),
    fovy                (60.f ),
    screenSize          (0, 0 ),
    enableFrustumCulling(true ),
    useVboExtension     (true ),
    showBoundingBoxes   (false),
    showSwitchDistance  (false),
    showSkirts          (true ),
    screenSpaceError    (5.0f ),
    pDrawEnv            (NULL ),
    geoMorphFactor      (0.f  )
{
}


//-----------------------------------------------------------------------------


BbqRenderStatistics::BbqRenderStatistics(void) :
    triangleCount  (0),
    nodeCount      (0),
    culledNodeCount(0)
{
}

//-----------------------------------------------------------------------------

template<class HeightType, class HeightDeltaType, class TextureType>
BbqTerrainRendererBase<HeightType, 
                       HeightDeltaType,
                       TextureType    >::BbqTerrainRendererBase(void) :
    _oDatabaseInfo          (    ),
    _oStatistics            (    ),
    _vStaticIndices         (    ),
    _oStaticIndexBuffer     (    ),
    _vStaticVertexData      (    ),
    _oStaticVertexBuffer    (    ),
    _oTerrainShader         (    ),
    _oHeightDataRenderCache (    ),
    _oTextureDataRenderCache(    ),
    _vHeightBuffer          (    )
{
}

template<class HeightType, class HeightDeltaType, class TextureType>
BbqTerrainRendererBase<HeightType, 
                       HeightDeltaType,
                       TextureType    >::~BbqTerrainRendererBase(void)
{
}

//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
const BbqRenderStatistics& 
    BbqTerrainRendererBase<HeightType, 
                           HeightDeltaType,
                           TextureType    >::getStatistics(void) const
{
    return _oStatistics;
}

template<class HeightType, class HeightDeltaType, class TextureType>
bool BbqTerrainRendererBase<HeightType, 
                            HeightDeltaType,
                            TextureType    >::initialize( 
    const BbqDataSourceInformation &databaseInfo)
{
    _oDatabaseInfo = databaseInfo;
    
    if(_oDatabaseInfo.textureType == Image::OSG_INVALID_IMAGEDATATYPE)
    {
        fprintf(stderr, "NO IMAGE TEXTURE\n");
    }

    // build the index buffer:
    createBlockIndices(_oDatabaseInfo.heightTileSize, 
                       _vStaticIndices              );

    // todo: do the other skirts too..
    typedef UInt16 IndexType;

    // todo: put this somewhere..
    
    // north:
    for(Int32 i = 0; i < _oDatabaseInfo.heightTileSize - 1; ++i)
    {
        IndexType row0 = 0;
        IndexType row1 = (IndexType) osgSqr(_oDatabaseInfo.heightTileSize);
        
        // two triangles:
        const IndexType col0 = (IndexType) i;
        const IndexType col1 = (IndexType)(i + 1);
        
        const IndexType idx0 = row0 + col0;
        const IndexType idx1 = row0 + col1;
        const IndexType idx2 = row1 + col0;
        const IndexType idx3 = row1 + col1;
        
        assert(idx0 < std::numeric_limits<IndexType>::max());
        assert(idx1 < std::numeric_limits<IndexType>::max());
        assert(idx2 < std::numeric_limits<IndexType>::max());
        assert(idx3 < std::numeric_limits<IndexType>::max());
        
        _vStaticIndices.push_back(idx0);
        _vStaticIndices.push_back(idx1);
        _vStaticIndices.push_back(idx2);
        
        _vStaticIndices.push_back(idx1);
        _vStaticIndices.push_back(idx3);
        _vStaticIndices.push_back(idx2);
    }
    
    // south:
    for(Int32 i = 0; i < _oDatabaseInfo.heightTileSize - 1; ++i)
    {
        IndexType row0 = 
            (IndexType) osgSqr(_oDatabaseInfo.heightTileSize) - 
            _oDatabaseInfo.heightTileSize;

        IndexType row1 = 
            (IndexType) osgSqr(_oDatabaseInfo.heightTileSize) + 
            _oDatabaseInfo.heightTileSize;
        
        // two triangles:
        const IndexType col0 = (IndexType) i;
        const IndexType col1 = (IndexType)(i + 1);
        
        const IndexType idx0 = row0 + col0;
        const IndexType idx1 = row0 + col1;
        const IndexType idx2 = row1 + col0;
        const IndexType idx3 = row1 + col1;
        
        assert(idx0 < std::numeric_limits<IndexType>::max());
        assert(idx1 < std::numeric_limits<IndexType>::max());
        assert(idx2 < std::numeric_limits<IndexType>::max());
        assert(idx3 < std::numeric_limits<IndexType>::max());
        
        _vStaticIndices.push_back(idx0);
        _vStaticIndices.push_back(idx2);
        _vStaticIndices.push_back(idx1);
        
        _vStaticIndices.push_back(idx1);
        _vStaticIndices.push_back(idx2);
        _vStaticIndices.push_back(idx3);
    }
    
    // west:
    for(Int32 i = 0; i < _oDatabaseInfo.heightTileSize - 1; ++i)
    {
        IndexType row0 = 0;
        IndexType row1 = 
            (IndexType) osgSqr(_oDatabaseInfo.heightTileSize) + 

            2 * _oDatabaseInfo.heightTileSize;

        Int32 increment = _oDatabaseInfo.heightTileSize;

        // two triangles:
        const IndexType col0 = (IndexType)(i    * increment);
        const IndexType col1 = (IndexType)(col0 + increment);
        
        const IndexType idx0 = row0 + col0;
        const IndexType idx1 = row0 + col1;
        const IndexType idx2 = row1 + i;
        const IndexType idx3 = row1 + (i + 1);
        
        assert(idx0 < std::numeric_limits<IndexType>::max());
        assert(idx1 < std::numeric_limits<IndexType>::max());
        assert(idx2 < std::numeric_limits<IndexType>::max());
        assert(idx3 < std::numeric_limits<IndexType>::max());
        
        _vStaticIndices.push_back(idx0);
        _vStaticIndices.push_back(idx2);
        _vStaticIndices.push_back(idx1);
        
        _vStaticIndices.push_back(idx1);
        _vStaticIndices.push_back(idx2);
        _vStaticIndices.push_back(idx3);
    }
    
    // east:
    for(Int32 i = 0; i < _oDatabaseInfo.heightTileSize - 1; ++i)
    {
        IndexType row0 = _oDatabaseInfo.heightTileSize - 1;

        IndexType row1 = 
            (IndexType) osgSqr(_oDatabaseInfo.heightTileSize) + 
            3 * _oDatabaseInfo.heightTileSize;

        Int32 increment = _oDatabaseInfo.heightTileSize;

        // two triangles:
        const IndexType col0 = (IndexType) (i    * increment);
        const IndexType col1 = (IndexType) (col0 + increment);
        
        const IndexType idx0 = row0 + col0;
        const IndexType idx1 = row0 + col1;
        const IndexType idx2 = row1 + i;
        const IndexType idx3 = row1 + (i + 1);
        
        assert(idx0 < std::numeric_limits<IndexType>::max());
        assert(idx1 < std::numeric_limits<IndexType>::max());
        assert(idx2 < std::numeric_limits<IndexType>::max());
        assert(idx3 < std::numeric_limits<IndexType>::max());
        
        _vStaticIndices.push_back(idx0);
        _vStaticIndices.push_back(idx1);
        _vStaticIndices.push_back(idx2);
        
        _vStaticIndices.push_back(idx1);
        _vStaticIndices.push_back(idx3);
        _vStaticIndices.push_back(idx2);
    }

    // store the indices in a gpu buffer:
    _oStaticIndexBuffer.create(GL_ELEMENT_ARRAY_BUFFER, 
                               sizeof(UInt16) * _vStaticIndices.size(), 
                               BufferUsage_Static_WriteOnly           );

    _oStaticIndexBuffer.uploadData(&(_vStaticIndices[0]), 
                                   0, 
                                   sizeof(UInt16) * _vStaticIndices.size());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    // build static vertex data:
    _vStaticVertexData.reserve(osgSqr(_oDatabaseInfo.heightTileSize) + 
                               8 * _oDatabaseInfo.heightTileSize );
    
    for(Int32 y = 0; y < _oDatabaseInfo.heightTileSize; ++y)
    {
        for(Int32 x = 0; x < _oDatabaseInfo.heightTileSize; ++x)
        {
            _vStaticVertexData.push_back((UInt16) x);
            _vStaticVertexData.push_back((UInt16) y);
        }
    }

    // todo: put this into a method..
    for(Int32 i = 0; i < _oDatabaseInfo.heightTileSize; ++i)
    {
        _vStaticVertexData.push_back((UInt16) i);
        _vStaticVertexData.push_back((UInt16) 0);
    }

    for(Int32 i = 0; i < _oDatabaseInfo.heightTileSize; ++i)
    {
        _vStaticVertexData.push_back((UInt16) i);
        _vStaticVertexData.push_back( 
            (UInt16) _oDatabaseInfo.heightTileSize - 1);
    }

    for(Int32 i = 0; i < _oDatabaseInfo.heightTileSize; ++i)
    {
        _vStaticVertexData.push_back((UInt16) 0);
        _vStaticVertexData.push_back((UInt16) i);
    }
    for(Int32 i = 0; i < _oDatabaseInfo.heightTileSize; ++i)
    {
        _vStaticVertexData.push_back( 
            (UInt16) _oDatabaseInfo.heightTileSize - 1);
        _vStaticVertexData.push_back((UInt16) i);
    }

    _oStaticVertexBuffer.create(GL_ARRAY_BUFFER_ARB, 
                                sizeof(UInt16) * _vStaticVertexData.size(), 
                                BufferUsage_Static_WriteOnly );

    _oStaticVertexBuffer.uploadData(&(_vStaticVertexData[0]), 
                                    0, 
                                    sizeof(UInt16) * _vStaticVertexData.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    const int renderCacheSize = 500;
    
    if(!initializeRenderCache(renderCacheSize, renderCacheSize))
    {
        return false;
    }

    return true;
}

template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRendererBase<HeightType, 
                            HeightDeltaType,
                            TextureType    >::shutdown(void)
{
    shutdownRenderCache();
}

//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
bool BbqTerrainRendererBase<HeightType, 
                            HeightDeltaType,
                            TextureType    >::initializeRenderCache( 
    int heightCacheEntryCount, 
    int textureCacheEntryCount )
{       
    // height data cache:
    _oHeightDataRenderCache.setSize(heightCacheEntryCount);
    
    const GLint vboSize = 2 * sizeof( GLfloat ) * 
        osgSqr( _oDatabaseInfo.heightTileSize ) + 8 * 
        sizeof( GLfloat ) * _oDatabaseInfo.heightTileSize;

    //const GLint vboSize = 2 * sizeof( GLfloat ) * sqr(
    //_oDatabaseInfo.heightTileSize ); 
    
    for(int i = 0; i < heightCacheEntryCount; ++i)
    {
        OpenGLGpuBuffer &vbo = _oHeightDataRenderCache.getEntry(i);
        
        if(!vbo.create(GL_ARRAY_BUFFER, 
                       vboSize, 
                       BufferUsage_Static_WriteOnly))
        {
            shutdownRenderCache();
            return false;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

    if( _oDatabaseInfo.textureTileSize > 0 )
    {
        _oTextureDataRenderCache.setSize( textureCacheEntryCount );
        
//        ImageFormat textureFormat = ImageFormat_R8G8B8;
        Image::PixelFormat textureFormat = Image::OSG_RGB_PF;
     
        if( _oDatabaseInfo.textureFormat == BbqFile::Dxt1 )
//        if( _oDatabaseInfo.textureFormat == Image::OSG_RGB_DXT1 )
        {
//            textureFormat = ImageFormat_DXT1;
            textureFormat = Image::OSG_RGB_DXT1;
        }
        
        for( int i = 0; i < textureCacheEntryCount; ++i )
        {
            OpenGLTexture& texture = _oTextureDataRenderCache.getEntry( i );
            
            if( !texture.create2D( textureFormat, 
                                   _oDatabaseInfo.textureTileSize, 
                                   _oDatabaseInfo.textureTileSize, 
                                   1 ) )
            {
                shutdownRenderCache();
                return false;
            }
        }
    }
    else
    {
        _oTextureDataRenderCache.setSize( 0 );
    }
    
    return true;
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRendererBase<HeightType, 
                            HeightDeltaType,
                            TextureType    >::shutdownRenderCache()
{
    for( int i = 0; i < _oHeightDataRenderCache.getSize(); ++i )
    {
        OpenGLGpuBuffer& vbo = _oHeightDataRenderCache.getEntry( i );
        
        vbo.destroy();
    }

    _oHeightDataRenderCache.clear();
    
    for( int i = 0; i < _oTextureDataRenderCache.getSize(); ++i )
    {
        OpenGLTexture& texture = _oTextureDataRenderCache.getEntry( i );
        
        texture.destroy();
    }

    _oTextureDataRenderCache.clear();
}

//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRendererBase<HeightType, 
                            HeightDeltaType,
                            TextureType    >::setGeoMorphingFactor(
    const BbqTerrNode *node)
{
    if(node->parent)
    {
        _oTerrainShader.setUniform("geoMorphFactor", 
                                   node->parent->geoMorphingFactor);
    }
    else
    {
        _oTerrainShader.setUniform("geoMorphFactor", 0.0f);
    }
}

//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRendererBase<HeightType, 
                            HeightDeltaType,
                            TextureType    >::prepareHeightData( 
    std::vector< float >& target, const BbqTerrNode *node )
{
    //todo: we should use the 16bit unsigned short data directly (without
    //conversion) 
    assert( node );

    const HeightType* heightData = &node->data.heightData[ 0 ];

    const int tileSize = _oDatabaseInfo.heightTileSize;
    const int heightSampleCount = ( int )node->data.heightData.size();
    
    target.resize( 2 * heightSampleCount + 8 * tileSize );
    
    const HeightType* sourcePtr = &heightData[ 0 ];

    float* targetPtr = &target[ 0 ];
    
    if( node->parent )
    {
        const HeightType* parentHeightData = 
            &node->parent->data.heightData[ 0 ];

        const Vec2i parentOffset = 
            getParentTileOffset( tileSize, getQuadtreeChildIndex( node->id ) );
        
        // interleave the height data of this node with the delta to the
        // height data of the parent node: 
        // todo: speed this up if necessery 
        int parentY = parentOffset.y();
        const int parentIncX = 1;
        const int parentIncY = tileSize;
        
        for( int y = 0; y < tileSize; ++y )
        {
            const bool yIsEven = y % 2 == 0;
            
            int parentX = parentOffset.x();
            
            for( int x = 0; x < tileSize; ++x )
            {
                const bool xIsEven = x % 2 == 0;
                
                // store the height:
                const HeightType heightSample = *sourcePtr++;
                
                *targetPtr++ = float( heightSample ) / 
                    TypeTraits<HeightType>::getMax();
                
                // todo: use a better interpolation mechanism:
                if( xIsEven && yIsEven )
                {
                    // residual is zero here
                    *targetPtr++ = 0.0f;
                    continue;
                }
                
                // there always has to be a greater sample:
                int parentFirstSampleOffset = parentY * tileSize + parentX;
                int parentSecondSampleOffset = 0;
                
                if( xIsEven )
                {
                    assert( y < tileSize - 1 );
                    
                    // vertical
                    parentSecondSampleOffset = parentIncY;
                }
                else if( yIsEven )
                {
                    assert( x < tileSize - 1 );
                    
                    // horizontal
                    parentSecondSampleOffset = parentIncX;
                }
                else
                {
                    assert( x < tileSize - 1 );
                    assert( y < tileSize - 1 );
                    
                    // diagonal
                    parentSecondSampleOffset = parentIncX + parentIncY;
                }
                
                HeightType parentHeightSample0 = 
                    parentHeightData[ parentFirstSampleOffset ];
                HeightType parentHeightSample1 = 
                    parentHeightData[ parentFirstSampleOffset + 
                                      parentSecondSampleOffset ];
                
                int expectedHeightValue = lerp( ( int )parentHeightSample0, 
                                                ( int )parentHeightSample1,
                                                0.5f );
                int residual = expectedHeightValue - heightSample;
                
                *targetPtr++ = float( residual ) / 
                    TypeTraits<HeightType>::getMax();
                
                if( !xIsEven )
                {
                    parentX++;
                }
            }
            
            if( !yIsEven )
            {
                parentY++;
            }
        }
    }
    else
    {
        for( int i = 0; i < heightSampleCount; ++ i )
        {
            *targetPtr++ = float( node->data.heightData[ i ] ) / 
                TypeTraits<HeightType>::getMax();
            *targetPtr++ = 0.0f;
        }
    }
    
    float skirtSize = 0.01f;
    // add skirt geometry: skirt height is simply the height of the vertex
    // minus a fixed amount 
    
    for( int i = 0; i < 8 * tileSize; ++ i )
    {
        *targetPtr++ = -skirtSize;
    }

/*
    for( int i = 0; i < target.size(); i+=2)
    {
        fprintf(stderr, "%d %f %f\n", 
                i/2, target[i], target[i+1]);
    }
 */
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
OpenGLTexture* BbqTerrainRendererBase<HeightType, 
                                      HeightDeltaType,
                                      TextureType    >::uploadTexture( 
    const BbqTerrNode *node,
    DrawEnv                  *pEnv)
{
    //Profile( uploadTexture );
    // todo: prevent cache trashing (switch from lru to mru if we detect a
    // thrashing situation) 
    
    if( !node )
    {
        return 0;
    }
    
    // if the node does not contain any texture data.. we cant cache anything:
    if( node->data.textureData.empty() )
    {
        return 0;
    }
    
    // cache the node height data into a vbo and the texture into a gl texture 
    OpenGLTexture* texture = _oTextureDataRenderCache.getEntry( node );
    
    if( texture )
    {
        return texture;
    }
    
    // ok.. not cached yet.. get a free texture:
    texture = _oTextureDataRenderCache.getNewEntry( node );
    
    assert( texture );
    
    texture->enable(GL_TEXTURE0, pEnv);

    // upload data
    if( !texture->upload2DRect( 0, 0, 
                                &node->data.textureData[ 0 ], 
                                node->data.textureData.size() ) )
    {
        // todo: report an errors
    }

    texture->disable(GL_TEXTURE0, pEnv);
    
    return texture;
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
OpenGLGpuBuffer* BbqTerrainRendererBase<HeightType, 
                                        HeightDeltaType,
                                        TextureType    >::uploadHeightData( 
    const BbqTerrNode *node )
{
//    Profile( uploadHeightData );
    // todo: prevent cache trashing (switch from lru to mru if we detect a
    // thrashing situation) 
    
    if( !node )
    {
        return 0;
    }
    
    // if the node does not contain any texture data.. we cant cache anything:
    if( node->data.heightData.empty() )
    {
        return 0;
    }
    
    // cache the node height data into a vbo and the texture into a GL texture 
    OpenGLGpuBuffer* heightData = _oHeightDataRenderCache.getEntry( node );
    
    if( heightData )
    {
        return heightData;
    }
    
    // ok.. not cached yet.. get a free texture:
    heightData = _oHeightDataRenderCache.getNewEntry( node );
    
    assert( heightData );
    
    // prepare the data (calculate delta to parent)
    prepareHeightData( _vHeightBuffer, node );
    
    // and upload the data:
    heightData->uploadData( &(_vHeightBuffer[ 0 ]), 
                            0, 
                            sizeof( float ) * _vHeightBuffer.size() );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    return heightData;
}

//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRendererBase<HeightType, 
                            HeightDeltaType,
                            TextureType    >::renderBoundingBox( 
    const BoxVolume& box,
    const BbqRenderOptions& options )
{
//    glUseProgramObjectARB( 0 );
    _oTerrainShader.deactivate(options.pDrawEnv);

    GLint oldPolygonMode[ 2 ];
    glGetIntegerv( GL_POLYGON_MODE, oldPolygonMode );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glDisable( GL_CULL_FACE );
//    OpenGLPrimitives::drawBox( box.minPoint, box.maxPoint );
    glEnable( GL_CULL_FACE );
    glPolygonMode( GL_FRONT, oldPolygonMode[ 0 ] );
    glPolygonMode( GL_BACK, oldPolygonMode[ 1 ] );
//    glUseProgramObjectARB( terrainShader_.getProgramHandle() );
    _oTerrainShader.activate(options.pDrawEnv);
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRendererBase<HeightType, 
                            HeightDeltaType,
                            TextureType    >::renderSphere(
                                const Pnt3f& center, 
                                      float radius,
                                const BbqRenderOptions& options )
{
    static GLUquadric *pQuad;

    if(pQuad == NULL)
    {
        pQuad = gluNewQuadric();
    }

//    glUseProgramObjectARB( 0 );
//    terrainShader_.deactivate(options.pDrawEnv);

    GLint oldPolygonMode[ 2 ];
    glGetIntegerv( GL_POLYGON_MODE, oldPolygonMode );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//    glDisable( GL_CULL_FACE );


    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
//    OpenGLPrimitives::drawSphere( center, radius, 20 );

    gluSphere(pQuad, radius, 20, 20);

    glPopMatrix();

//    glEnable( GL_CULL_FACE );
    glPolygonMode( GL_FRONT, oldPolygonMode[ 0 ] );
    glPolygonMode( GL_BACK, oldPolygonMode[ 1 ] );
//    glUseProgramObjectARB( terrainShader_.getProgramHandle() );
//    terrainShader_.activate(options.pDrawEnv);
}





//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
BbqTerrainRenderer<HeightType, 
                   HeightDeltaType,
                   TextureType    >::BbqTerrainRenderer(void) :
    Inherited              (    ),
    traversalStack_        (    ),
    fallbackTexture_       (    ),
    heightColorTexture_    (    ),
    texture0               (NULL),
    texture1               (NULL)
{
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
BbqTerrainRenderer<HeightType, 
                   HeightDeltaType,
                   TextureType    >::~BbqTerrainRenderer(void)
{
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
bool BbqTerrainRenderer<HeightType, 
                        HeightDeltaType,
                        TextureType    >::initialize( 
    const BbqDataSourceInformation &databaseInfo)
{
    Inherited::initialize(databaseInfo);

    if(!_oTerrainShader.loadVtxFromFile("data/TerrainShaderVtx.glsl") ||
       !_oTerrainShader.loadFrgFromFile("data/TerrainShaderFrg.glsl")  )
    {
        return false;
    }
    

    
    
#if 0
    FileInputStream fallbackTextureFile( 
        "shader/TerrainFallbackTexture.bmp" );
    
    fallbackTexture_.loadFromStream( fallbackTextureFile );
    
    FileInputStream heightColorTextureFile( 
        "E:/Projects/motor3d/bin/bbqterrain/shader/HeightColorTexture.bmp" );
    
    heightColorTexture_.loadFromStream( heightColorTextureFile );
#endif
    
    fallbackTexture_.loadFromFile("shader/TerrainFallbackTexture.bmp");
    
   
    heightColorTexture_.loadFromFile( 
        "E:/Projects/motor3d/bin/bbqterrain/shader/HeightColorTexture.bmp" );

    return true;
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRenderer<HeightType, 
                        HeightDeltaType,
                        TextureType    >::shutdown(void)
{
    Inherited::shutdown();
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRenderer<HeightType, 
                        HeightDeltaType,
                        TextureType    >::render(      

                                        BbqTerrNode      *rootNode, 
                                  const BbqRenderOptions &options )
{
    _oStatistics.nodeCount       = 0;
    _oStatistics.triangleCount   = 0;
    
    traversalStack_.push_back(rootNode);
    
    // activate the shader:
//    glUseProgramObjectARB( terrainShader_.getProgramHandle() );
//    terrainShader_.activate(options.pDrawEnv);
//    fprintf(stderr, "Frame start\n");

    while(!traversalStack_.empty())
    {
        BbqTerrNode *node = traversalStack_.back();

        assert(node);
        
        traversalStack_.pop_back();
        
#ifdef GV_CHECK
        // cull this node->.
        if( options.enableFrustumCulling && 
            !isIntersecting( options.frustum, node->boundingBox ) )
        {
            // not visible at all
            _oStatistics.culledNodeCount++;
            continue;
        }
#endif
        
        const Real32 detailFactor = options.screenSpaceError;
        
        //todo: i need to use the distance to 

        Pnt3f bboxCenter;
        Vec3f dist;

        node->boundingBox.getCenter(bboxCenter);
        
/*
        fprintf(stderr, "%d %f %f %f\n",
                node->id,
                bboxCenter[0],
                bboxCenter[1],
                bboxCenter[2]);
 */

        dist = options.viewerpos - bboxCenter;

        const float distance = osgMax(dist.length(), 0.001f);

//        const float distance = osgMax( 
//            getMagnitude( options.frustum.getPosition() - 
//                          node->boundingBox.getCenter() ), 0.001f );
        
        //// todo: instead of the size of the node, use the maximum error bound

        Vec3f bboxSize;

        node->boundingBox.getSize(bboxSize);

//        const float nodeSize = node->boundingBox.getSize().x;
        const float nodeSize = bboxSize.x();
        
//        const float fovY = options.frustum.getFovY();
        const float fovY = options.fovy;
        const float screenResolution = float( options.screenSize.y() ) / fovY;
        
        const float nodeError = screenResolution * 
            ( ( nodeSize / float( _oDatabaseInfo.heightTileSize ) ) / distance );

        const float objectSpaceHeightError = 
            float( node->maxHeightError ) / 
            65535.0f * _oDatabaseInfo.heightScale + _oDatabaseInfo.heightOffset;

        const float screenSpaceHeightError = 
            ( objectSpaceHeightError / distance ) * screenResolution;

        const float screenFactor = 
            float( options.screenSize.y() ) / ( 2.0f * tanf( fovY / 2.0f ) );

        const float screenSpaceHeightError2 = 
            ( objectSpaceHeightError / distance ) * screenFactor;
        
        const float switchDistance2 =
            ( objectSpaceHeightError / detailFactor ) * screenFactor;

        // todo: geomorphing geht noch nicht richtig...
        
        Vec3f disp = options.viewerpos - bboxCenter;
//            options.frustum.getPosition() - node->boundingBox.getCenter();

//        Vec3f extent = node->boundingBox.getSize();
        Vec3f extent;

        node->boundingBox.getSize(extent);
            
        disp[0] = osgMax( 0.0f, fabsf( disp.x() ) - extent.x() );
        disp[1] = osgMax( 0.0f, fabsf( disp.y() ) - extent.y() );
        disp[2] = osgMax( 0.0f, fabsf( disp.z() ) - extent.z() );
        
        //  disp.y = 0; // for debugging
        
        float d = 0;
//        d = osgSqrt( dot( disp, disp ) );
        d = osgSqrt( disp.dot(disp) );
        
        //  float 
        //      const float tan_half_FOV = tanf(0.5f * horizontal_FOV_degrees
        //      * (float) M_PI / 180.0f); 
        //
        //  const float K = screen_width_pixels / tan_half_FOV;
        
        //  // distance_LODmax is the distance below which we need to be
        //  // at the maximum LOD.  It's used in compute_lod(), which is
        //  // called by the chunks during update().
        //  m_distance_LODmax = ;

        //  return fmax(1, d / (objectSpaceHeightError / detailFactor) * K);
        //}
        
        //float switchDistance2 = ( objectSpaceHeightError / detailFactor ) *
        //screenFactor; 

        //float switchDistance = ( objectSpaceHeightError / detailFactor ) *
        //screenFactor; 

        float switchDistance = 
            ( nodeSize / float( _oDatabaseInfo.heightTileSize ) ) / 
            detailFactor * screenResolution;
        
        float geomorphStartDistance = switchDistance + 145.0f;
        
        //const bool hasEnoughDetail = nodeError < detailFactor;
        

/*
        fprintf(stderr, "%f | %f\n", 
                distance,
                geomorphStartDistance);
 */


        if( node->isLeafNode() || distance > geomorphStartDistance )
        {
            // render the node:
#ifndef GV_TEST
            setGeoMorphingFactor( node );
#else
            _oTerrainShader.setUniform( "geoMorphFactor", 
                                        options.geoMorphFactor );
#endif

            renderNodeVbo( node, options.showSkirts, options );
            
            if( options.showBoundingBoxes )
            {
                glColor3f( 0, 0, 1 );
                renderBoundingBox( node->boundingBox, options );
            }
            
            if( options.showSwitchDistance )
            {
                Pnt3f bboxCenter;

                node->boundingBox.getCenter(bboxCenter);

                glColor3f( 0, 1, 0 );
                Inherited::renderSphere(bboxCenter , switchDistance, options );
            }
        }
        else
        {
            // compute the geomorphing factor:
            //const float innerSwitchDistance = switchDistance -
            //switchDistance / 4.0f; 

            node->geoMorphingFactor = 
                clamp( 1.0f - ( switchDistance - distance ) / 
                       ( geomorphStartDistance - switchDistance ), 0.0f, 1.0f );
            //node->geoMorphingFactor = 1.0f;
            
            // push the child nodes:
            
            // todo: push the nearest child last.. (to get a rough front to
            // back rendering order) 

            static int order[ 4 ] = { 0, 1, 2, 3 };
            //static float dist[ 4 ];
            
            //if( options.sortChildren )
            //{
            //  for( int i = 0; i < 4; ++i )
            //  {
            //      dist[ i ] = getMagnitude( node->children[ i
            //]->boundingBox.getCenter() - options.frustum.getPosition() ); 
            
            //      for( int j = 0; j < i; ++j )
            //      {
            //          if( dist[ i ] <
            //      }
            //  }
            //}
            
            traversalStack_.push_back( node->children[ order[ 0 ] ] );
            traversalStack_.push_back( node->children[ order[ 1 ] ] );
            traversalStack_.push_back( node->children[ order[ 2 ] ] );
            traversalStack_.push_back( node->children[ order[ 3 ] ] );
        }
    }

#if 0
    fprintf(stderr, "%f %f | %f %f\n",
            rootNode->boundingBox.getMin().x(),
            rootNode->boundingBox.getMin().z(),
            rootNode->boundingBox.getMax().x(),
            rootNode->boundingBox.getMax().z());
#endif


//    glUseProgramObjectARB( 0 );
//    terrainShader_.deactivate(options.pDrawEnv);
}






//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRenderer<HeightType, 
                        HeightDeltaType,
                        TextureType    >::renderNodeVbo(
    const BbqTerrNode      *node, 
          bool              renderSkirts,
    const BbqRenderOptions &options  )
{
    Vec3f bboxSize;

    node->boundingBox.getSize(bboxSize);

    const float scale = 
        bboxSize.x() / 
        float( _oDatabaseInfo.heightTileSize - 1 );

    const Vec2f blockScale( scale, scale );
    const Vec2f blockOffset( node->boundingBox.getMin().x(), 
                             node->boundingBox.getMin().z() );
    
    _oTerrainShader.setUniform( "blockScale", blockScale );
    _oTerrainShader.setUniform( "blockOffset", blockOffset );
    
    _oTerrainShader.setUniform( "heightScale", _oDatabaseInfo.heightScale );
    _oTerrainShader.setUniform( "heightOffset", _oDatabaseInfo.heightOffset );

/*
    fprintf(stderr, "bs/bo %f %f %f %f\n",
            blockScale[0], blockScale[1],
            blockOffset[0], blockOffset[1]);
 */

    _oTerrainShader.activate(options.pDrawEnv);

    GLsizei triangleCount = 2 * osgSqr( _oDatabaseInfo.heightTileSize - 1 );
    
    if( renderSkirts )
    {
        triangleCount += 8 * ( _oDatabaseInfo.heightTileSize - 1 );
    }
    
    OpenGLGpuBuffer* heightDataVbo = uploadHeightData( node );
    
    assert( heightDataVbo );
    
    if(_oDatabaseInfo.textureType != Image::OSG_INVALID_IMAGEDATATYPE)
    {
        activateTextures( node, options.pDrawEnv );
    }

    _oTerrainShader.update(options.pDrawEnv);
    
    glBindBuffer( GL_ARRAY_BUFFER, _oStaticVertexBuffer.getBufferId() );
    glVertexPointer( 2, GL_SHORT, 0, ( char* )0 );
    glEnableClientState( GL_VERTEX_ARRAY );
    
    glBindBuffer( GL_ARRAY_BUFFER, heightDataVbo->getBufferId() );
    glTexCoordPointer( 2, GL_FLOAT, 0, ( char* )0 );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _oStaticIndexBuffer.getBufferId() );
    glDrawRangeElements( GL_TRIANGLES, 0, ( GLuint ) _vStaticVertexData.size(), 3 * triangleCount, GL_UNSIGNED_SHORT, 0 );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
    
/*
    OpenGLTexture::disable( GL_TEXTURE2, GL_TEXTURE_2D );
    OpenGLTexture::disable( GL_TEXTURE1, GL_TEXTURE_2D );
    OpenGLTexture::disable( GL_TEXTURE0, GL_TEXTURE_2D );
 */
    if(_oDatabaseInfo.textureType != Image::OSG_INVALID_IMAGEDATATYPE)
    {
        texture0->disable( GL_TEXTURE2, options.pDrawEnv );
        texture1->disable( GL_TEXTURE1, options.pDrawEnv);
        heightColorTexture_.disable( GL_TEXTURE0, options.pDrawEnv );
    }

    _oTerrainShader.deactivate(options.pDrawEnv);

    _oStatistics.nodeCount++;
    _oStatistics.triangleCount += triangleCount;
}






//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRenderer<HeightType, 
                        HeightDeltaType,
                        TextureType    >::calculateTextureParameters( 
    const BbqTerrNode  *node, 
    const BbqTerrNode  *textureNode, 
    Vec2f& texCoordOffset, 
    Vec2f& texCoordScale )
{
    assert( node );
    assert( textureNode );
    
    texCoordScale.setValues( 

        ( node->blockScale / textureNode->blockScale ) / 
        float( _oDatabaseInfo.heightTileSize - 1 ), 

        ( node->blockScale / textureNode->blockScale ) / 
        float( _oDatabaseInfo.heightTileSize - 1 ) );
    
    texCoordOffset.setValues( 

        float(node->sampleRect.x0 - textureNode->sampleRect.x0 ) / 
        float(textureNode->blockScale * ( _oDatabaseInfo.heightTileSize - 1)),

        float(node->sampleRect.y0 - textureNode->sampleRect.y0 ) / 
        float(textureNode->blockScale * ( _oDatabaseInfo.heightTileSize - 1)));

#if 0
    fprintf(stderr, "x %d %d\n", 
            node->sampleRect.x0, textureNode->sampleRect.x0);
    fprintf(stderr, "y %d %d\n", 
            node->sampleRect.y0, textureNode->sampleRect.y0);

    fprintf(stderr, "bo t %f %f\n",
            node->blockOrigin[0], node->blockOrigin[1]);
    fprintf(stderr, "bo tn %f %f\n",
            textureNode->blockOrigin[0], textureNode->blockOrigin[1]);

    fprintf(stderr, "bb t %f %f\n", 
            node->boundingBox.getMin().x(), 
            node->boundingBox.getMin().z() );
    fprintf(stderr, "bb tn %f %f\n", 
            textureNode->boundingBox.getMin().x(), 
            textureNode->boundingBox.getMin().z() );
#endif
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
OpenGLTexture* BbqTerrainRenderer<HeightType, 
                                  HeightDeltaType,
                                  TextureType    >::findParentTexture( 
    const BbqTerrNode * node, 
    const BbqTerrNode *&textureNode,
          DrawEnv     * pEnv)
{
    // find a parent with a texture:
    BbqTerrNode   *currentNode = node->parent;
    OpenGLTexture *texture = 0;
    
    while( currentNode )
    {
        texture = uploadTexture( currentNode, pEnv );
        
        if( texture )
        {
            // we have a winner:
            
            textureNode = currentNode;
            break;
        }
        
        currentNode = currentNode->parent;
    }
    
    return texture;
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqTerrainRenderer<HeightType, 
                        HeightDeltaType,
                        TextureType    >::activateTextures(
                                  const BbqTerrNode *node,
                                  DrawEnv *pEnv)
{
//    Profile( activateTextures );
    
    Vec2f texCoordOffset( 0, 0 );
    Vec2f texCoordScale( 1.0f / float( _oDatabaseInfo.heightTileSize - 1 ), 
                         1.0f / float( _oDatabaseInfo.heightTileSize - 1 ) );

    Vec2f coarserTexCoordOffset( 0, 0 );
    Vec2f coarserTexCoordScale( 
        1.0f / float( _oDatabaseInfo.heightTileSize - 1 ), 
        1.0f / float( _oDatabaseInfo.heightTileSize - 1 ) );

    OpenGLTexture* texture = uploadTexture( node, pEnv );
    OpenGLTexture* coarserTexture = 0;
    
    if( texture )
    {
        if( node->parent )
        {
            coarserTexture = uploadTexture( node->parent, pEnv );
            if( coarserTexture )
            {
                calculateTextureParameters( node, 
                                            node->parent, 
                                            coarserTexCoordOffset, 
                                            coarserTexCoordScale );
            }
        }
        else
        {
            coarserTexture = texture;
        }
    }
    else
    {
        // find a parent with a texture:
        const BbqTerrNode* parentNode = 0;
        texture         = findParentTexture( node, parentNode, pEnv );
        
        if( texture )
        {
            calculateTextureParameters( node, 
                                        parentNode, 
                                        texCoordOffset, 
                                        texCoordScale );
        }
        
        if( parentNode && parentNode->parent )
        {
            const BbqTerrNode* coarserParentNode = 0;
            
            coarserTexture = findParentTexture( parentNode,
                                                coarserParentNode, pEnv );
            
            if( coarserTexture  )
            {
                calculateTextureParameters( node, 
                                            coarserParentNode, 
                                            coarserTexCoordOffset, 
                                            coarserTexCoordScale );
            }
        }           
    }
    
    if( !coarserTexture )
    {
        coarserTexture = texture;
    }
    
    if( texture )
    {
        // todo: put these settings into a OpenGLTextureSampler class:
        texture->enable( GL_TEXTURE0, pEnv );

        texture0 = texture;

        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_WRAP_S, 
                         GL_MIRROR_CLAMP_TO_EDGE_ATI );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_WRAP_T, 
                         GL_MIRROR_CLAMP_TO_EDGE_ATI );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_MIN_FILTER, 
                         GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_MAG_FILTER, 
                         GL_LINEAR );     
        
        coarserTexture->enable( GL_TEXTURE1, pEnv );

        texture1 = coarserTexture;

        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_WRAP_S, 
                         GL_MIRROR_CLAMP_TO_EDGE_ATI );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_WRAP_T, 
                         GL_MIRROR_CLAMP_TO_EDGE_ATI );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_MIN_FILTER, 
                         GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_MAG_FILTER, 
                         GL_LINEAR ); 
    }
    else
    {
        // todo: use fallback texture:          
        fallbackTexture_.enable( GL_TEXTURE0, pEnv );

        texture0 = &fallbackTexture_;

        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_WRAP_S, 
                         GL_MIRROR_CLAMP_TO_EDGE_ATI );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_WRAP_T, 
                         GL_MIRROR_CLAMP_TO_EDGE_ATI );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_MIN_FILTER, 
                         GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_MAG_FILTER, 
                         GL_LINEAR );     
        
        fallbackTexture_.enable( GL_TEXTURE1, pEnv );

        texture1 = &fallbackTexture_;

        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_WRAP_S, 
                         GL_MIRROR_CLAMP_TO_EDGE_ATI );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_WRAP_T, 
                         GL_MIRROR_CLAMP_TO_EDGE_ATI );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_MIN_FILTER, 
                         GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, 
                         GL_TEXTURE_MAG_FILTER, 
                         GL_LINEAR );     
    }
    
    heightColorTexture_.enable( GL_TEXTURE2, pEnv );

    glTexParameteri( GL_TEXTURE_2D, 
                     GL_TEXTURE_WRAP_S, 
                     GL_MIRROR_CLAMP_TO_EDGE_ATI );
    glTexParameteri( GL_TEXTURE_2D, 
                     GL_TEXTURE_WRAP_T, 
                     GL_MIRROR_CLAMP_TO_EDGE_ATI );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ); 
    

#if 0
    fprintf(stderr, "T : %f %f | %f %f\n",
            texCoordScale[0], texCoordScale[1],
            texCoordOffset[0], texCoordOffset[1]);

    fprintf(stderr, "CT %f %f | %f %f\n",
            coarserTexCoordScale[0], coarserTexCoordScale[1],
            coarserTexCoordOffset[0], coarserTexCoordOffset[1]);
#endif

    _oTerrainShader.setUniform( "texCoordScale", texCoordScale );
    _oTerrainShader.setUniform( "texCoordOffset", texCoordOffset );
    _oTerrainShader.setUniform( "coarserTexCoordScale", coarserTexCoordScale );
    _oTerrainShader.setUniform( "coarserTexCoordOffset", coarserTexCoordOffset );
    
    _oTerrainShader.setSampler( "texture", 0 );
    _oTerrainShader.setSampler( "coarserTexture", 1 );
    //terrainShader_.setSampler( "heightColorTexture", 2 );
}



template class BbqTerrainRendererBase<UInt16, Int16, UInt8>;
template class BbqTerrainRendererBase<Int16,  Int16, UInt8>;

template class BbqTerrainRenderer<UInt16, Int16, UInt8>;
template class BbqTerrainRenderer<Int16,  Int16, UInt8>;

OSG_END_NAMESPACE



//-----------------------------------------------------------------------------

// todo: make this work again (as a fallback)
//void BbqOpenGLTerrainRenderer::renderNodeNonVbo( const BbqTerrainNode* node )
//{
//  const float scale = node->boundingBox.getSize().x / float( _oDatabaseInfo.heightTileSize - 1 );
//  const Vector2f blockScale( scale, scale );
//  const Vector2f blockOffset( node->boundingBox.minPoint.x, node->boundingBox.minPoint.z );

//  terrainShader_.setUniform( "blockScale", blockScale );
//  terrainShader_.setUniform( "blockOffset", blockOffset );

//  terrainShader_.setUniform( "heightScale", _oDatabaseInfo.heightScale );
//  terrainShader_.setUniform( "heightOffset", _oDatabaseInfo.heightOffset );

//  const GLsizei triangleCount = 2 * sqr( _oDatabaseInfo.heightTileSize - 1 );

//  activateTextures( node );

//  glVertexPointer( 2, GL_SHORT, 0, &staticVertexData_[ 0 ] );
//  glEnableClientState( GL_VERTEX_ARRAY );

//  glTexCoordPointer( 1, GL_SHORT, 0, &node->data.heightData[ 0 ] );
//  glEnableClientState( GL_TEXTURE_COORD_ARRAY );

//  //todo: hack!!! clear this..
//  terrainShader_.setUniform( "heightScale", _oDatabaseInfo.heightScale / 65535.0f );

//  glDrawElements( GL_TRIANGLES, 3 * triangleCount, GL_UNSIGNED_SHORT, &_vStaticIndices[ 0 ] );

//  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
//  glDisableClientState( GL_VERTEX_ARRAY );
//}

