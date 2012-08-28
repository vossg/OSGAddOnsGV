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

#include "OSGConfig.h"

#ifndef GL_GLEXT_PROTOTYPES
# define GL_GLEXT_PROTOTYPES
#endif

#if defined(OSG_GL_INCLUDE_GLEW)
# undef OSG_GL_INCLUDE_GLEW
#endif

#ifdef WIN32
#include <GL/glew.h>
#endif

#include "OSGGL.h"
#include "OSGGLU.h"

#include "OSGBbqTerrainRenderer.h"
#include "OSGBbqTerrainNode.h"

#include "OSGDrawEnv.h"

#include <limits>

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
BbqGeoRefdTerrainRenderer<HeightType, 
                          HeightDeltaType,
                          TextureType    >::BbqGeoRefdTerrainRenderer(void) :
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
BbqGeoRefdTerrainRenderer<HeightType, 
                          HeightDeltaType,
                          TextureType    >::~BbqGeoRefdTerrainRenderer(void)
{
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
bool BbqGeoRefdTerrainRenderer<HeightType, 
                               HeightDeltaType,
                               TextureType    >::initialize( 
    const BbqDataSourceInformation &databaseInfo)
{
    Inherited::initialize(databaseInfo);

    if(!_oTerrainShader.loadVtxFromFile("data/GeoRefTerrainShaderVtx.glsl") ||
       !_oTerrainShader.loadFrgFromFile("data/GeoRefTerrainShaderFrg.glsl")  )
    {
        return false;
    }

//    _oTerrainShader.setUniform("ellipsoidAxis", databaseInfo.vEllipsoidAxis);

   
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
void BbqGeoRefdTerrainRenderer<HeightType, 
                               HeightDeltaType,
                               TextureType    >::shutdown(void)
{
    Inherited::shutdown();
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqGeoRefdTerrainRenderer<HeightType, 
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
        
#if 0 //Unused
        const float nodeError = screenResolution * 
            ( ( nodeSize / float( _oDatabaseInfo.heightTileSize ) ) / distance );

        const float objectSpaceHeightError = 
            float( node->maxHeightError ) / 
            32767.0f * _oDatabaseInfo.heightScale + _oDatabaseInfo.heightOffset;
//            65535.0f * _oDatabaseInfo.heightScale + _oDatabaseInfo.heightOffset;

        const float screenSpaceHeightError = 
            ( objectSpaceHeightError / distance ) * screenResolution;

        const float screenFactor = 
            float( options.screenSize.y() ) / ( 2.0f * tanf( fovY / 2.0f ) );

        const float screenSpaceHeightError2 = 
            ( objectSpaceHeightError / distance ) * screenFactor;
        
        const float switchDistance2 =
            ( objectSpaceHeightError / detailFactor ) * screenFactor;
#endif

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
        
//        float d = 0;
//        d = osgSqrt( dot( disp, disp ) );
//        d = 
              osgSqrt( disp.dot(disp) );
        
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
            Inherited::setGeoMorphingFactor( node );
#else
            _oTerrainShader.setUniform( "geoMorphFactor", 
                                        options.geoMorphFactor );
#endif

            renderNodeVbo( node, options.showSkirts, options );
            
            if( options.showBoundingBoxes )
            {
                glColor3f( 0, 0, 1 );
                this->renderBoundingBox( node->boundingBox, options );
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


    glColor3f(1.f, 0.f, 0.f);
    glBegin(GL_QUADS);
    {
        glVertex3f(rootNode->boundingBox.getMin().x(),
                   0.f,
                   rootNode->boundingBox.getMin().z());
        glVertex3f(rootNode->boundingBox.getMax().x(),
                   0.f,
                   rootNode->boundingBox.getMin().z());
        glVertex3f(rootNode->boundingBox.getMax().x(),
                   0.f,
                   rootNode->boundingBox.getMax().z());
        glVertex3f(rootNode->boundingBox.getMin().x(),
                   0.f,
                   rootNode->boundingBox.getMax().z());
    }
    glEnd();


    static bool dumpBox = false;

    if(dumpBox == false)
    {
        fprintf(stderr, "%f %f | %f %f\n",
                rootNode->boundingBox.getMin().x(),
                rootNode->boundingBox.getMin().z(),
                rootNode->boundingBox.getMax().x(),
                rootNode->boundingBox.getMax().z());

        dumpBox = true;
    }


//    glUseProgramObjectARB( 0 );
//    _oTerrainShader.deactivate(options.pDrawEnv);
}





//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqGeoRefdTerrainRenderer<HeightType, 
                               HeightDeltaType,
                               TextureType    >::renderNodeVbo(
    const BbqTerrNode      *node, 
          bool              renderSkirts,
    const BbqRenderOptions &options  )
{
    Vec3f bboxSize;

    node->boundingBox.getSize(bboxSize);

    float scale = 
        bboxSize.x() / 
        float( _oDatabaseInfo.heightTileSize - 1 );

/*
    const Vec2f blockScale( scale, scale );
    const Vec2f blockOffset( node->boundingBox.getMin().x(), 
                             node->boundingBox.getMin().z() );
 */
    
   
//    _oTerrainShader.setUniform( "heightScale", _oDatabaseInfo.heightScale );
//    _oTerrainShader.setUniform( "heightOffset", _oDatabaseInfo.heightOffset );


/*    fprintf(stderr, "bs/bo %f %f %f %f\n",
            blockScale[0], blockScale[1],
            blockOffset[0], blockOffset[1]);
 */

/*
    fprintf(stderr, "hts: %d %d\n",
            _oDatabaseInfo.heightSampleCount[0],
            _oDatabaseInfo.heightSampleCount[1]);

    fprintf(stderr, "sr: %d %d %d %d | %d %d\n",
            node->sampleRect.x0,
            node->sampleRect.y0,
            node->sampleRect.x1,
            node->sampleRect.y1,
            node->sampleRect.getWidth(),
            node->sampleRect.getHeight());

    fprintf(stderr, "%f %f  | %f %f\n",
            _oDatabaseInfo.vOrigin[0],
            _oDatabaseInfo.vOrigin[1],
            _oDatabaseInfo.vPixelSize[0],
            _oDatabaseInfo.vPixelSize[1]);

    fprintf(stderr, "TS: %d\n", _oDatabaseInfo.heightTileSize);
 */

    scale = 
        Real32 (node->sampleRect.getWidth()) / 
        Real32(_oDatabaseInfo.heightTileSize - 1);


//    fprintf(stderr, "scale %f\n", scale * _oDatabaseInfo.vPixelSize[0]);

    float originX = _oDatabaseInfo.vOrigin[0] + 
        node->sampleRect.x0 * _oDatabaseInfo.vPixelSize[0];

    float originY = _oDatabaseInfo.vOrigin[1] + 
        node->sampleRect.y0 * _oDatabaseInfo.vPixelSize[1];

/*
    fprintf(stderr, "origin %f %f\n", originX, originY);

    fprintf(stderr, "ep %f %f\n",
            originX + 
            (scale * 
             _oDatabaseInfo.vPixelSize[0] * 
             _oDatabaseInfo.heightTileSize - 1),

            originY + 
            (scale * 
             _oDatabaseInfo.vPixelSize[1] * 
             _oDatabaseInfo.heightTileSize - 1));
 */

    const Vec2f blockScale(scale * _oDatabaseInfo.vPixelSize[0], 
                           scale * _oDatabaseInfo.vPixelSize[1]);

    const Vec2f blockOffset(originX, originY); 


    _oTerrainShader.setUniform( "blockScale", blockScale );
    _oTerrainShader.setUniform( "blockOffset", blockOffset );


    _oTerrainShader.activate(options.pDrawEnv);

    GLsizei triangleCount = 2 * osgSqr( _oDatabaseInfo.heightTileSize - 1 );
    
    if( renderSkirts )
    {
        triangleCount += 8 * ( _oDatabaseInfo.heightTileSize - 1 );
    }
    
    OpenGLGpuBuffer* heightDataVbo = this->uploadHeightData( node );
    
    assert( heightDataVbo );
    
    if(_oDatabaseInfo.textureType != Image::OSG_INVALID_IMAGEDATATYPE)
    {
        activateTextures( node, options.pDrawEnv );
    }

    _oTerrainShader.update(options.pDrawEnv);
    
    glBindBuffer( GL_ARRAY_BUFFER, _oStaticVertexBuffer.getBufferId() );
    glVertexPointer( 2, GL_SHORT, 0, NULL );
    glEnableClientState( GL_VERTEX_ARRAY );
    
    glBindBuffer( GL_ARRAY_BUFFER, heightDataVbo->getBufferId() );
    glTexCoordPointer( 2, GL_FLOAT, 0, NULL );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _oStaticIndexBuffer.getBufferId() );
    glDrawRangeElements( GL_TRIANGLES, 0, GLuint(_vStaticVertexData.size()), 
                         3 * triangleCount, GL_UNSIGNED_SHORT, 0 );
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




//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType>
void BbqGeoRefdTerrainRenderer<HeightType, 
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
OpenGLTexture* BbqGeoRefdTerrainRenderer<HeightType, 
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
        texture = this->uploadTexture( currentNode, pEnv );
        
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
void BbqGeoRefdTerrainRenderer<HeightType, 
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

    OpenGLTexture* texture = this->uploadTexture( node, pEnv );
    OpenGLTexture* coarserTexture = 0;
    
    if( texture )
    {
        if( node->parent )
        {
            coarserTexture = this->uploadTexture( node->parent, pEnv );
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



template class BbqGeoRefdTerrainRenderer<UInt16, Int16, UInt8>;
template class BbqGeoRefdTerrainRenderer<Int16,  Int16, UInt8>;

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

