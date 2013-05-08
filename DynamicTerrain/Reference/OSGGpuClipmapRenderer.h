//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		GpuClipmapRenderer.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#if 0

#ifndef GPU_CLIPMAP_RENDERER_INCLUDED
#define GPU_CLIPMAP_RENDERER_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGAddOnsConfig.h>
#include <OSGContribDef.h>

#include "ClipmapRenderer.h"
#include "OSGGpuBuffer.h"
#include "OSGGlslShader.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	class DynamicTerrain;
	typedef FCPtr<MaterialDrawablePtr, DynamicTerrain> TerrainPtr;

	//-------------------------------------------------------------------------------------------------


	class OSG_CONTRIBLIB_DLLMAPPING GpuClipmapRenderer : public ClipmapRenderer
	{
	public:		
		GpuClipmapRenderer();
		virtual						~GpuClipmapRenderer();

	protected:
		virtual bool				onInitialize( TerrainPtr terrain );
		virtual void				onShutdown();

		virtual void				onUpdateRenderData( const Pnt3f& viewPosition );
		virtual void				onRender( const ClipmapRenderParameters& renderParameters );

		virtual void				onRebuildIndices( GeometryClipmapLevel& level );
		virtual void				onBuildVertices( GeometryClipmapLevel& level, const GeometryClipmapLevel* coarserLevel, const Rectangle2i& sampleRect );

		virtual void				onReloadShader();

		virtual void				onCreateGpuResources();
		virtual void				onDestroyGpuResources();

	private:		
		struct TerrainVertex
		{
			Vec2s				gridPos;
		};

		typedef std::vector< TerrainVertex >	TerrainVertexList;
		typedef std::vector< Int16 >			IndexList;

		struct TerrainLevelRenderData
		{
			TextureChunkPtr			elevationData;
		};

		struct RenderBlock
		{
			// todo: replace with gpu buffers..
			GpuBuffer				vertexBuffer;
			GpuBuffer				indexBuffer;
			//TerrainVertexList		vertices;
			//IndexList				indices;
		};

		typedef std::vector< TerrainLevelRenderData >	TerrainLevelRenderDataList;

		RenderBlock					ringBlock_;
		RenderBlock					ringFixupBlock_[ 2 ];
		RenderBlock					interiorRimBlock_;
		RenderBlock					degenerateTriangleBlock_;

		TerrainLevelRenderDataList	levels_;		
		GlslShader					terrainShader_;
		Pnt3f						worldViewerPosition_;

		float						worldSpacing_;
		int							worldSampleCount_;
		int							levelSampleCount_;

		void						renderLevel( const TerrainLevelRenderData& level, const TerrainLevelRenderData* coarserLevel );
		void						renderBlock( const RenderBlock& block );

		void						buildRingBlock( RenderBlock& target, int levelSampleCount );
		void						buildFixupBlock( RenderBlock& horizontalBlock, RenderBlock& verticalBlock, int levelSampleCount );
		void						buildBlock( RenderBlock& target, int vertexCountX, int vertexCountY );

		//int							updateLevel( const ImagePtr& heightData, TerrainLevelRenderData& level, const Rectangle2i& desiredClipRegion );
		//int							updateLevelRect( const ImagePtr& heightData, TerrainLevelRenderData& level, const Rectangle2i& sampleRect );
	};


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef GPU_CLIPMAP_RENDERER_INCLUDED

//-------------------------------------------------------------------------------------------------

#endif // 0
