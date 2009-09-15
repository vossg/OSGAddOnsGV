/*---------------------------------------------------------------------------*\
*                                OpenSG                                     *
*                                                                           *
*                                                                           *
*               Copyright (C) 2000-2002 by the OpenSG Forum                 *
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
*                                                                           *
\*---------------------------------------------------------------------------*/

#ifndef _OSGCLIPMAPRENDERER_H_
#define _OSGCLIPMAPRENDERER_H_

#include "OSGFrustumVolume.h"
#include "OSGTextureObjChunkFields.h"
#include "OSGTextureEnvChunkFields.h"
#include "OSGTextureObjChunk.h"
#include "OSGTextureEnvChunk.h"
#include "OSGWindow.h"
#include "OSGTerrainTypes.h"
#include "OSGRectangle.h"

OSG_BEGIN_NAMESPACE

class DrawEnv;

struct WorldTransformation
{
    WorldTransformation();
    
    Pnt2f                       offset;
    float                       sampleDistance;
    float                       heightOffset;
    float                       heightScale;
    
    Pnt2f                       sampleToWorldPos( const Pnt2i& pos ) const;
    Pnt2i                       worldToSamplePos( const Pnt2f& pos ) const;
};

struct ClipmapRenderParameters
{
    ClipmapRenderParameters();
    
    DrawEnv                    *drawEnv;
    Window*                     window;
    FrustumVolume               viewFrustum;
    bool                        showBoundingBoxes;
    bool                        enableFrustumCulling;
    bool                        showTransitionRegions;
    bool                        useVboExtension;        
    TextureObjChunkUnrecPtr     globalTextureObj;
    TextureEnvChunkUnrecPtr     globalTextureEnv;
    TextureObjChunkUnrecPtr     heightColorTexture;
    WorldTransformation         worldTransform;
};


struct ClipmapRenderStatistics
{
    ClipmapRenderStatistics();
    void                        clear();
    
    // stats from last render:
    int                         drawnLevelCount;
    int                         drawnBlockCount;
    int                         drawnTriangleCount;
    int                         transformedVertexCount;
    int                         culledBlockCount;       
};


class GeometryClipmaps;
struct GeometryClipmapLevel;


class ClipmapRenderer
{
  public:

    bool initialize( GeometryClipmaps* geoClipmaps );
    void shutdown();
    
    void updateRenderData( const Pnt3f& viewPosition );
    void render(const ClipmapRenderParameters& renderParameters );
    
    void rebuildIndices( GeometryClipmapLevel& level );
    void buildVertices(GeometryClipmapLevel& level, 
                       const GeometryClipmapLevel* coarserLevel, 
                       const Rectangle2i& sampleRect );
    
    void reloadShader();
    
    const ClipmapRenderStatistics &getLastFrameStatistics() const;
    
  protected:
    
    ClipmapRenderer();
    
    virtual ~ClipmapRenderer();     
    
    virtual bool onInitialize( GeometryClipmaps* terrain ) = 0;
    virtual void onShutdown() = 0;
    
    virtual void onUpdateRenderData( const Pnt3f& viewPosition ) = 0;
    virtual void onRender(const ClipmapRenderParameters& renderParameters) = 0;
    
    virtual void onRebuildIndices( GeometryClipmapLevel& level ) = 0;
    
    virtual void onBuildVertices( GeometryClipmapLevel& level, 
                                  const GeometryClipmapLevel* coarserLevel, 
                                  const Rectangle2i& sampleRect ) = 0;
    
    virtual void onReloadShader() = 0;
    
    virtual void onCreateGpuResources() = 0;
    virtual void onDestroyGpuResources() = 0;
    
    Color3f      getDebugColor( int index ) const;
    
    void         drawSampleRectangle(const Rectangle2i& rect, 
                                     const Color3f& color, 
                                     const WorldTransformation& worldTrans ) const;
    void         drawSamples(const GeometryClipmapLevel& level, 
                             const Color3f& color, 
                             const WorldTransformation& worldTrans ) const;
    


    GeometryClipmaps*               geoClipmaps_;
    
    // values from last frame:
    Pnt3f                           viewerPosition_;
    Pnt2i                           viewerSamplePos_;
    
    Window*                         window_;
    ClipmapRenderStatistics         stats_;
    //bool                          useVertexBuffers_;
};

OSG_END_NAMESPACE

#endif /* _OSGCLIPMAPRENDERER_H_ */
