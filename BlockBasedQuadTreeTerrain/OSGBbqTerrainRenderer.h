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

#ifndef _OSGBBQTERRAINRENDERER_H_
#define _OSGBBQTERRAINRENDERER_H_

//-----------------------------------------------------------------------------

#include "OSGBbqDataSource.h"
#include "OSGBbqRenderCache.h"

#include "OSGVector.h"
#include "OSGFrustumVolume.h"

#include <vector>

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

class DrawEnv;

//-----------------------------------------------------------------------------

struct BbqRenderOptions
{
    BbqRenderOptions(void);
    
    FrustumVolume  frustum;
    Pnt3f          viewerpos;
    Real32         fovy;
    Vec2i          screenSize;
    bool           enableFrustumCulling;
    bool           useVboExtension;
    bool           showBoundingBoxes;
    bool           showSwitchDistance;
    bool           showSkirts;
    Real32         screenSpaceError;
    DrawEnv       *pDrawEnv;
    Real32         geoMorphFactor;
};

//-----------------------------------------------------------------------------

struct BbqRenderStatistics
{
    BbqRenderStatistics(void);
    
    Int32 triangleCount;
    Int32 nodeCount;
    Int32 culledNodeCount;
};

template<class HeightType, class HeightDeltaType, class TextureType>
class BbqTerrainRendererBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqTerrainNode<HeightType, 
                           HeightDeltaType, 
                           TextureType    > BbqTerrNode;

};

//-----------------------------------------------------------------------------

template<class HeightType, class HeightDeltaType, class TextureType>
class BbqTerrainRenderer
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqTerrainNode<HeightType, 
                           HeightDeltaType, 
                           TextureType    > BbqTerrNode;


    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainRenderer(void);
    ~BbqTerrainRenderer(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool initialize(const BbqDataSourceInformation &databaseInfo);
    void shutdown  (                                            );
    
    void render    (      BbqTerrNode              *rootNode, 
                    const BbqRenderOptions         &options     );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    const BbqRenderStatistics &getStatistics(void) const;
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:

    BbqDataSourceInformation          databaseInfo_;
    
    std::vector<BbqTerrNode *>        traversalStack_;
    
    // the one and only index buffer for all blocks.
    std::vector<UInt16>               staticIndices_;

    OpenGLGpuBuffer                   staticIndexBuffer_;

    std::vector<UInt16>               staticVertexData_;
    OpenGLGpuBuffer                   staticVertxBuffer_;
    
    OpenGLShader                      terrainShader_;
    BbqRenderStatistics               statistics_;
    
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void renderNodeVbo    (const BbqTerrNode        *node, 
                                 bool                renderSkirts,
                           const BbqRenderOptions   &options     );

    void renderBoundingBox(const BoxVolume          &box,
                           const BbqRenderOptions   &options     );

    void renderSphere     (const Pnt3f              &center, 
                                 Real32              radius,
                           const BbqRenderOptions   &options     );
    

    //void renderNodeNonVbo( const BbqTerrainNode* node );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void calculateTextureParameters(const BbqTerrNode *node, 
                                    const BbqTerrNode *parentNode, 
                                          Vec2f       &texCoordOffset, 
                                          Vec2f       &texCoordScale );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    OpenGLTexture *findParentTexture(const BbqTerrNode * node, 
                                     const BbqTerrNode *&textureNode,
                                           DrawEnv     * pEnv);

    void           activateTextures(const BbqTerrNode  * node,
                                          DrawEnv      * pDrawEnv);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool initializeRenderCache(Int32 heightCacheEntryCount, 
                               Int32 textureCacheEntryCount);

    void shutdownRenderCache  (void                        );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    OpenGLTexture   *uploadTexture   (const BbqTerrNode *node,
                                            DrawEnv     *pEnv);
    OpenGLGpuBuffer *uploadHeightData(const BbqTerrNode *node);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void setGeoMorphingFactor(const BbqTerrNode         *node  );
    
    void prepareHeightData   (      std::vector<Real32> &target, 
                              const BbqTerrNode         *node  );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    typedef BbqRenderCache< 
              OpenGLGpuBuffer, 
              BbqRenderCacheType_HeightData>  HeightDataRenderCache;

    typedef BbqRenderCache< 
              OpenGLTexture, 
              BbqRenderCacheType_TextureData> TextureDataRenderCache;
    
    HeightDataRenderCache   heightDataRenderCache_;
    TextureDataRenderCache  textureDataRenderCache_;
    
    OpenGLTexture           fallbackTexture_;
    OpenGLTexture           heightColorTexture_;
    
    OpenGLTexture          *texture0;
    OpenGLTexture          *texture1;

    std::vector<Real32>     heightBuffer_;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
};


template<class HeightType, class HeightDeltaType, class TextureType>
class BbqGeoRefdTerrainRenderer
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqTerrainNode<HeightType, 
                           HeightDeltaType, 
                           TextureType    > BbqTerrNode;


    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqGeoRefdTerrainRenderer(void);
    ~BbqGeoRefdTerrainRenderer(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool initialize(const BbqDataSourceInformation &databaseInfo);
    void shutdown  (                                            );
    
    void render    (      BbqTerrNode              *rootNode, 
                    const BbqRenderOptions         &options     );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    const BbqRenderStatistics &getStatistics(void) const;
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:

    BbqDataSourceInformation          databaseInfo_;
    
    std::vector<BbqTerrNode *>        traversalStack_;
    
    // the one and only index buffer for all blocks.
    std::vector<UInt16>               staticIndices_;

    OpenGLGpuBuffer                   staticIndexBuffer_;

    std::vector<UInt16>               staticVertexData_;
    OpenGLGpuBuffer                   staticVertxBuffer_;
    
    OpenGLShader                      terrainShader_;
    BbqRenderStatistics               statistics_;
    
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void renderNodeVbo    (const BbqTerrNode        *node, 
                                 bool                renderSkirts,
                           const BbqRenderOptions   &options     );

    void renderBoundingBox(const BoxVolume          &box,
                           const BbqRenderOptions   &options     );

    void renderSphere     (const Pnt3f              &center, 
                                 Real32              radius,
                           const BbqRenderOptions   &options     );
    

    //void renderNodeNonVbo( const BbqTerrainNode* node );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void calculateTextureParameters(const BbqTerrNode *node, 
                                    const BbqTerrNode *parentNode, 
                                          Vec2f       &texCoordOffset, 
                                          Vec2f       &texCoordScale );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    OpenGLTexture *findParentTexture(const BbqTerrNode * node, 
                                     const BbqTerrNode *&textureNode,
                                           DrawEnv     * pEnv);

    void           activateTextures(const BbqTerrNode  * node,
                                          DrawEnv      * pDrawEnv);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool initializeRenderCache(Int32 heightCacheEntryCount, 
                               Int32 textureCacheEntryCount);

    void shutdownRenderCache  (void                        );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    OpenGLTexture   *uploadTexture   (const BbqTerrNode *node,
                                            DrawEnv     *pEnv);
    OpenGLGpuBuffer *uploadHeightData(const BbqTerrNode *node);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void setGeoMorphingFactor(const BbqTerrNode         *node  );
    
    void prepareHeightData   (      std::vector<Real32> &target, 
                              const BbqTerrNode         *node  );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    typedef BbqRenderCache< 
              OpenGLGpuBuffer, 
              BbqRenderCacheType_HeightData>  HeightDataRenderCache;

    typedef BbqRenderCache< 
              OpenGLTexture, 
              BbqRenderCacheType_TextureData> TextureDataRenderCache;
    
    HeightDataRenderCache   heightDataRenderCache_;
    TextureDataRenderCache  textureDataRenderCache_;
    
    OpenGLTexture           fallbackTexture_;
    OpenGLTexture           heightColorTexture_;
    
    OpenGLTexture          *texture0;
    OpenGLTexture          *texture1;

    std::vector<Real32>     heightBuffer_;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
};


OSG_END_NAMESPACE


#endif // #ifndef BBQTERRAINRENDERER_INCLUDED
