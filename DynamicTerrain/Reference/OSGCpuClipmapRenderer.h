//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		CpuClipmapRenderer.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef CPU_CLIPMAP_RENDERER_INCLUDED
#define CPU_CLIPMAP_RENDERER_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGConfig.h>
#include <OSGContribDef.h>

#include "OSGClipmapRenderer.h"
#include "OSGGpuBuffer.h"
#include "OSGStatelessGlslShader.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	class OSG_CONTRIBLIB_DLLMAPPING CpuClipmapRenderer : public ClipmapRenderer
	{
	public:		
		CpuClipmapRenderer();
		virtual						~CpuClipmapRenderer();

		void						setVertexProgram( const char* programText );
		void						setFragmentProgram( const char* programText );

	protected:
		virtual bool				onInitialize( GeometryClipmaps* terrain );
		virtual void				onShutdown();

		virtual void				onUpdateRenderData( const Pnt3f& viewPosition );
		virtual void				onRender( const ClipmapRenderParameters& renderParameters );

		virtual void				onRebuildIndices( GeometryClipmapLevel& level );
		virtual void				onBuildVertices( GeometryClipmapLevel& level, const GeometryClipmapLevel* coarserLevel, const Rectangle2i& sampleRect );

		virtual void				onReloadShader();

	private:
		struct OpenGLTerrainVertex
		{
			Pnt4f					pos;		// x,y,z,coarse_y
			Pnt2f					uv;			
		};

		typedef std::vector< OpenGLTerrainVertex >	OpenGLTerrainVertexList;
		typedef std::vector< UInt16 >				IndexList;

		struct TerrainLevelRenderData
		{
			OpenGLTerrainVertexList	vertices;
			bool					rebuildIndices;
			IndexList				indices;
			GpuBuffer				vertexBuffer;
			TextureChunkPtr			texture;
		};

		typedef std::vector< TerrainLevelRenderData >	TerrainLevelRenderDataList;

		TerrainLevelRenderDataList	levels_;
		GlslShader					terrainShader_;
		GpuBuffer					testBuffer_;
		bool						useVertexBufferObjects_;
		bool						programTextChanged_;
		std::string					vertexProgramText_;
		std::string					fragmentProgramText_;

		virtual void				onCreateGpuResources();
		virtual void				onDestroyGpuResources();

		bool						buildIndices( GeometryClipmapLevel& level, const GeometryClipmapLevel* finerLevel );
		bool						checkVboConsistency( TerrainLevelRenderData& levelRenderData, int levelSampleCount );

		void						drawBlockBoundingBox( const GeometryClipmapLevel& level, const Rectangle2i& blockRect );

		void						createBlockIndices( GeometryClipmapLevel& level, const Rectangle2i& blockRect, int levelSampleCount, IndexList& indices );

		void						renderBlock( const GeometryClipmapLevel& level, const GeometryClipmapLevel* coarserLevel, const ClipmapRenderParameters& renderParameters, const Color3f& debugColor );

		//void						drawVertices( const GeometryClipmapLevel& level, const Color3f& color, const WorldTransformation& worldTransform ) const;
		void						drawBlendLines( const GeometryClipmapLevel& level, const Color3f& color, const WorldTransformation& worldTransform ) const;
	};


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef OSG_CPU_CLIPMAP_RENDERER_INCLUDED

//-------------------------------------------------------------------------------------------------

