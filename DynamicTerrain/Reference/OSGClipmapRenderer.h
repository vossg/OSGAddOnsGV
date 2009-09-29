//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		ClipmapRenderer.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef CLIPMAP_RENDERER_INCLUDED
#define CLIPMAP_RENDERER_INCLUDED

//-------------------------------------------------------------------------------------------------

#include "OSGFrustumVolume.h"
#include "OSGTextureChunkFields.h"
#include "OSGWindow.h"
#include "OSGTerrainTypes.h"
#include "OSGRectangle.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	class RenderAction;

	struct WorldTransformation
	{
		WorldTransformation();

		Pnt2f						offset;
		float						sampleDistance;
		float						heightOffset;
		float						heightScale;

		Pnt2f						sampleToWorldPos( const Pnt2i& pos ) const;
		Pnt2i						worldToSamplePos( const Pnt2f& pos ) const;
	};

	//-------------------------------------------------------------------------------------------------

	struct ClipmapRenderParameters
	{
		ClipmapRenderParameters();

		RenderAction*				renderAction;
		Window*						window;
		FrustumVolume				viewFrustum;
		bool						showBoundingBoxes;
		bool						enableFrustumCulling;
		bool						showTransitionRegions;
		bool						useVboExtension;		
		TextureChunkPtr				globalTexture;
		TextureChunkPtr				heightColorTexture;
		WorldTransformation			worldTransform;
	};

	//-------------------------------------------------------------------------------------------------

	struct ClipmapRenderStatistics
	{
		ClipmapRenderStatistics();
		void						clear();

		// stats from last render:
		int							drawnLevelCount;
		int							drawnBlockCount;
		int							drawnTriangleCount;
		int							transformedVertexCount;
		int							culledBlockCount;		
	};

	//-------------------------------------------------------------------------------------------------

	class GeometryClipmaps;
	struct GeometryClipmapLevel;

	//-------------------------------------------------------------------------------------------------

	class ClipmapRenderer
	{
	public:
		bool							initialize( GeometryClipmaps* geoClipmaps );
		void							shutdown();

		void							updateRenderData( const Pnt3f& viewPosition );
		void							render( const ClipmapRenderParameters& renderParameters );

		void							rebuildIndices( GeometryClipmapLevel& level );
		void							buildVertices( GeometryClipmapLevel& level, const GeometryClipmapLevel* coarserLevel, const Rectangle2i& sampleRect );

		void							reloadShader();

		const ClipmapRenderStatistics&	getLastFrameStatistics() const;

	protected:
		ClipmapRenderer();
		virtual							~ClipmapRenderer();		
		
		virtual bool					onInitialize( GeometryClipmaps* terrain ) = 0;
		virtual void					onShutdown() = 0;

		virtual void					onUpdateRenderData( const Pnt3f& viewPosition ) = 0;
		virtual void					onRender( const ClipmapRenderParameters& renderParameters ) = 0;

		virtual void					onRebuildIndices( GeometryClipmapLevel& level ) = 0;

		virtual void					onBuildVertices( GeometryClipmapLevel& level, const GeometryClipmapLevel* coarserLevel, const Rectangle2i& sampleRect ) = 0;

		virtual void					onReloadShader() = 0;

		virtual void					onCreateGpuResources() = 0;
		virtual void					onDestroyGpuResources() = 0;

		Color3f							getDebugColor( int index ) const;

		void							drawSampleRectangle( const Rectangle2i& rect, const Color3f& color, const WorldTransformation& worldTrans ) const;
		void							drawSamples( const GeometryClipmapLevel& level, const Color3f& color, const WorldTransformation& worldTrans ) const;

		GeometryClipmaps*				geoClipmaps_;

		// values from last frame:
		Pnt3f							viewerPosition_;
		Pnt2i							viewerSamplePos_;

		Window*							window_;
		ClipmapRenderStatistics			stats_;
		//bool							useVertexBuffers_;
	};


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef OSG_CLIPMAP_RENDERER_INCLUDED

//-------------------------------------------------------------------------------------------------

