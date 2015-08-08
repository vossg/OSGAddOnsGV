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

  private:

    BbqRenderOptions(const BbqRenderOptions &other);
    void operator =(const BbqRenderOptions &rhs);
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

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainRendererBase(void);
    ~BbqTerrainRendererBase(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    const BbqRenderStatistics &getStatistics(void) const;
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    typedef BbqRenderCache< 
              OpenGLGpuBuffer, 
              BbqRenderCacheType_HeightData>  HeightDataRenderCache;

    typedef BbqRenderCache< 
              OpenGLTexture, 
              BbqRenderCacheType_TextureData> TextureDataRenderCache;
    
    BbqDataSourceInformation          _oDatabaseInfo;
    BbqRenderStatistics               _oStatistics;

    // the one and only index buffer for all blocks.
    std::vector<UInt16>               _vStaticIndices;
    OpenGLGpuBuffer                   _oStaticIndexBuffer;

    std::vector<UInt16>               _vStaticVertexData;
    OpenGLGpuBuffer                   _oStaticVertexBuffer;


    OpenGLShader                      _oTerrainShader;


    HeightDataRenderCache             _oHeightDataRenderCache;
    TextureDataRenderCache            _oTextureDataRenderCache;


    std::vector<Real32>               _vHeightBuffer;

    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool initialize(const BbqDataSourceInformation &databaseInfo);
    void shutdown  (                                            );

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

    void setGeoMorphingFactor(const BbqTerrNode         *node  );
    
    void prepareHeightData   (      std::vector<Real32> &target, 
                              const BbqTerrNode         *node  );
    
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

    void renderBoundingBox(const BoxVolume          &box,
                           const BbqRenderOptions   &options     );

    void renderSphere     (const Pnt3f              &center, 
                                 Real32              radius,
                           const BbqRenderOptions   &options     );
    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:
};

//-----------------------------------------------------------------------------

template<class HeightType, class HeightDeltaType, class TextureType>
class BbqTerrainRenderer : public BbqTerrainRendererBase<HeightType,
                                                         HeightDeltaType,
                                                         TextureType    >
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqTerrainNode        <HeightType, 
                                   HeightDeltaType, 
                                   TextureType    > BbqTerrNode;

    typedef BbqTerrainRendererBase<HeightType,
                                   HeightDeltaType,
                                   TextureType    > Inherited;

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
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

    using Inherited::_oDatabaseInfo;

    using Inherited::_vStaticIndices;
    using Inherited::_oStaticIndexBuffer;

    using Inherited::_vStaticVertexData;
    using Inherited::_oStaticVertexBuffer;

    using Inherited::_oHeightDataRenderCache;
    using Inherited::_oTextureDataRenderCache;

    using Inherited::_oTerrainShader;

    using Inherited::_oStatistics;

  private:

    
    std::vector<BbqTerrNode *> traversalStack_;
    
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void renderNodeVbo    (const BbqTerrNode        *node, 
                                 bool                renderSkirts,
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
    
    OpenGLTexture           fallbackTexture_;
    OpenGLTexture           heightColorTexture_;
    
    OpenGLTexture          *texture0;
    OpenGLTexture          *texture1;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    BbqTerrainRenderer(const BbqTerrainRenderer &other);
    void operator =(const BbqTerrainRenderer &rhs);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
};


template<class HeightType, class HeightDeltaType, class TextureType>
class BbqGeoRefdTerrainRenderer : public BbqTerrainRendererBase<HeightType,
                                                                HeightDeltaType,
                                                                TextureType    >
{
    /*==========================  PUBLIC  =================================*/


  public:

    typedef BbqTerrainNode        <HeightType, 
                                   HeightDeltaType, 
                                   TextureType    > BbqTerrNode;

    typedef BbqTerrainRendererBase<HeightType,
                                   HeightDeltaType,
                                   TextureType    > Inherited;

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
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

    using Inherited::_oDatabaseInfo;

    using Inherited::_vStaticIndices;
    using Inherited::_oStaticIndexBuffer;

    using Inherited::_vStaticVertexData;
    using Inherited::_oStaticVertexBuffer;

    using Inherited::_oHeightDataRenderCache;
    using Inherited::_oTextureDataRenderCache;

    using Inherited::_oTerrainShader;

    using Inherited::_oStatistics;

  private:

    std::vector<BbqTerrNode *> traversalStack_;
    
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void renderNodeVbo    (const BbqTerrNode        *node, 
                                 bool                renderSkirts,
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

    
    OpenGLTexture           fallbackTexture_;
    OpenGLTexture           heightColorTexture_;
    
    OpenGLTexture          *texture0;
    OpenGLTexture          *texture1;


    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    BbqGeoRefdTerrainRenderer(const BbqGeoRefdTerrainRenderer &other);
    void operator =(const BbqGeoRefdTerrainRenderer &rhs);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
};


OSG_END_NAMESPACE


#endif // #ifndef BBQTERRAINRENDERER_INCLUDED
