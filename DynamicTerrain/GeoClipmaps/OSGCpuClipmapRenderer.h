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

#ifndef _OSGCPUCLIPMAPRENDERER_H_
#define _OSGCPUCLIPMAPRENDERER_H_

#include "OSGClipmapRenderer.h"
#include "OSGGpuBuffer.h"
#ifdef OLD_GEOCLIP
#include "OSGStatelessGlslShader.h"
#else
#include "OSGSimpleSHLChunk.h"
#endif

OSG_BEGIN_NAMESPACE


class CpuClipmapRenderer : public ClipmapRenderer
{
  public:       

    CpuClipmapRenderer();
    virtual                     ~CpuClipmapRenderer();
    
    void                        setVertexProgram( const char* programText );
    void                        setFragmentProgram( const char* programText );
    
  protected:
    virtual bool                onInitialize( GeometryClipmaps* terrain );
    virtual void                onShutdown();
    
    virtual void                onUpdateRenderData( const Pnt3f& viewPosition );
    virtual void                onRender( const ClipmapRenderParameters& renderParameters );
    
    virtual void                onRebuildIndices( GeometryClipmapLevel& level );
    virtual void                onBuildVertices( GeometryClipmapLevel& level, const GeometryClipmapLevel* coarserLevel, const Rectangle2i& sampleRect );
    
    virtual void                onReloadShader();
    
  private:
    struct OpenGLTerrainVertex
    {
        Pnt4f                   pos;        // x,y,z,coarse_y
        Pnt2f                   uv;  

        OpenGLTerrainVertex(void) :
            pos(),
            uv () {}
    };
    
    typedef std::vector< OpenGLTerrainVertex >  OpenGLTerrainVertexList;
    typedef std::vector< UInt16 >               IndexList;
    
    struct TerrainLevelRenderData
    {
        OpenGLTerrainVertexList vertices;
        bool                    rebuildIndices;
        IndexList               indices;
        GpuBuffer               vertexBuffer;
        TextureObjChunkUnrecPtr texture;

        TerrainLevelRenderData(void) :
            vertices      (    ),
            rebuildIndices(true),
            indices       (    ),
            vertexBuffer  (    ),
            texture       (NULL) {}
    };
    
    typedef std::vector< TerrainLevelRenderData >   TerrainLevelRenderDataList;
    
    TerrainLevelRenderDataList  levels_;
#ifdef OLD_GEOCLIP
    GlslShader                  terrainShader_;
#else
    SimpleSHLChunkUnrecPtr     _pTerrainShader;
#endif
    GpuBuffer                   testBuffer_;
    bool                        useVertexBufferObjects_;
    bool                        programTextChanged_;
    std::string                 vertexProgramText_;
    std::string                 fragmentProgramText_;
    
    virtual void onCreateGpuResources();
    virtual void onDestroyGpuResources();
    
    bool         buildIndices( GeometryClipmapLevel& level, 
                               const GeometryClipmapLevel* finerLevel );
    bool         checkVboConsistency( TerrainLevelRenderData& levelRenderData,
                                      int levelSampleCount );
    
    void         drawBlockBoundingBox( const GeometryClipmapLevel& level, 
                                       const Rectangle2i& blockRect );
    
    void         createBlockIndices( GeometryClipmapLevel& level, 
                                     const Rectangle2i& blockRect, 
                                     int levelSampleCount, 
                                     IndexList& indices );
    
    void         renderBlock( const GeometryClipmapLevel& level, 
                              const GeometryClipmapLevel* coarserLevel, 
                              const ClipmapRenderParameters& renderParameters, 
                              const Color3f& debugColor );
    
    //void      drawVertices( const GeometryClipmapLevel& level, const Color3f& color, const WorldTransformation& worldTransform ) const;

    void         drawBlendLines(
        const GeometryClipmapLevel& level, 
        const Color3f& color, 
        const WorldTransformation& worldTransform) const;
};

OSG_END_NAMESPACE

#endif /* #ifdef _OSGCPUCLIPMAPRENDERER_H_ */
