//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:20:35
//
//	Description			:		GpuClipmapRenderer.cpp
//
//-------------------------------------------------------------------------------------------------

//#include "PrecompiledHeader.h"

#if 0 

#include "OSGGpuClipmapRenderer.h"
#include "OSGRectangle.h"
#include "OSGDynamicTerrain.h"
#include "OSGDynamicTerrainLevel.h"
#include <algorithm>

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	int				getAlignedInt( int x, int n, bool roundUp );
	Rectangle2i		alignRectangle( const Rectangle2i& rect, const Pnt2i& alignment );
	Color3f			getDebugColor( size_t index );

	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


	GpuClipmapRenderer::GpuClipmapRenderer()
	{
	}


	//-------------------------------------------------------------------------------------------------


	GpuClipmapRenderer::~GpuClipmapRenderer()
	{
		shutdown();
	}


	//-------------------------------------------------------------------------------------------------


	bool GpuClipmapRenderer::onInitialize( TerrainPtr terrain )
	{
		assert( terrain != NullFC );

		terrain_ = terrain;

		shutdown();

		const int levelCount = terrain->getLevel().size();

		levels_.resize( levelCount );

		for( int levelIdx = 0; levelIdx < levelCount; ++levelIdx )
		{
			DynamicTerrainLevelPtr level = terrain->getLevel( levelIdx );
			TerrainLevelRenderData& levelRenderData = levels_[ levelIdx ];

			const int levelSampleCount = level->getSize();

			levelRenderData.elevationData = TextureChunk::create();
		}
		
		// build blocks:
		buildRingBlock( ringBlock_, levelSampleCount );
		buildFixupBlock( ringFixupBlock_[ 0 ], ringFixupBlock_[ 1 ], levelSampleCount );
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::onShutdown()
	{

	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::onUpdateRenderData( const Pnt3f& viewPosition )
	{
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::onRender( const RenderParameters& renderParameters )
	{
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::onRebuildIndices( DynamicTerrainLevelPtr level )
	{
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::onBuildVertices( DynamicTerrainLevelPtr level, const DynamicTerrainLevelPtr coarserLevel, const Rectangle2i& sampleRect )
	{
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::onReloadShader()
	{
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::onCreateGpuResources()
	{
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::onDestroyGpuResources()
	{
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::shutdown()
	{
		terrainShader_.destroy();

		//clipmaps_.shutdown();
		//blockData_.clear();		
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::update( const Pnt3f& viewerPosition, const ImagePtr& heightData )
	{
		Pnt2i viewerSamplePos = worldPosToSamplePos( Pnt2f( viewerPosition[ 0 ], viewerPosition[ 2 ] ) );

		const size_t levelCount = levels_.size();

		// uddate active rect:
		for( size_t i = 0; i < levelCount; ++i )
		{
			ClipLevelData& level = levels_[ i ];

			int levelSize = levelSampleCount_ * level.sampleDistance;

			if( i == 0 )
			{
				// level 0 is the coarsest and encloses the whole terrain
				level.desiredActiveRegion.setOrigin( 0, 0 );
			}
			else
			{
				// the other level are around the viewer

				level.desiredActiveRegion.setOrigin( viewerSamplePos - Pnt2i( levelSize / 2, levelSize / 2 ) );

				// align the region to the sample-distance:
				level.desiredActiveRegion = alignRectangle( level.desiredActiveRegion, level.sampleDistance );
			}

			level.desiredActiveRegion.setSize( levelSize, levelSize );
		}

		// update clip region:
		size_t updatedSampleCount = 0;

		for( size_t i = 0; i < levelCount; ++i )
		{
			ClipLevelData& level = levels_[ i ];

			// we want to go to the desiredActiveRegion..
			Rectangle2i desiredClipRegion = level.desiredActiveRegion;

			// we need at least one sample distance between this clipregion and the parent region
			if( i > 0 )
			{
				const ClipLevelData& parentLevel = levels_[ i - 1 ];

				desiredClipRegion.crop( parentLevel.clipRegion, 1 );
				desiredClipRegion = alignRectangle( desiredClipRegion, level.sampleDistance );
			}

			updatedSampleCount += updateLevel( heightData, level, desiredClipRegion );

			level.clipRegion = desiredClipRegion;

//			if( updatedSampleCount >= sampleBudgetPerFrame_ )
//			{
//				// continue next frame
//				//logMessage( "[terrain] Geometry Clipmaps: reached update Sample Limit!\n" );
//				break;
//			}
		}

		// crop active region:		
		for( size_t i = 0; i < levelCount; ++i )
		{
			ClipLevelData& level = levels_[ i ];

			Rectangle2i croppedActiveRegion = level.desiredActiveRegion;

			if( i > 0 )
			{	
				const ClipLevelData& parentLevel = levels_[ i - 1 ];

				// the cropped region has to fit inside the clipRegion:
				croppedActiveRegion.crop( level.clipRegion, 0 );

				// and inside the active region of the parent:
				croppedActiveRegion.crop( parentLevel.croppedActiveRegion, 2 );

				// adjust the active region so that the active region lies on "even" vertices.
				croppedActiveRegion = alignRectangle( croppedActiveRegion, parentLevel.sampleDistance );
			}

			level.croppedActiveRegion = croppedActiveRegion;
		}

		// update the render region:
		for( size_t i = 0; i < levelCount; ++i )
		{
			const size_t levelIdx = levelCount - i - 1;

			assert( levelIdx >= 0 );
			assert( levelIdx < levelCount );

			ClipLevelData& level = levels_[ levelIdx ];

			if( level.croppedActiveRegion.isEmpty() )
			{
				level.outerRenderBounds.clear();
				level.innerRenderBounds.clear();
				continue;
			}

			level.outerRenderBounds = level.croppedActiveRegion;
			level.innerRenderBounds.clear();

			if( levelIdx < levelCount - 1 )
			{
				const ClipLevelData& nextLevel = levels_[ levelIdx + 1 ];

				level.innerRenderBounds = nextLevel.croppedActiveRegion;
			}

			assert( level.outerRenderBounds.x0 >= level.clipRegion.x0 );
			assert( level.outerRenderBounds.y0 >= level.clipRegion.y0 );
			assert( level.outerRenderBounds.x1 <= level.clipRegion.x1 );
			assert( level.outerRenderBounds.y1 <= level.clipRegion.y1 );

			level.minTransitionSize[ 0 ] = std::min( 
				abs( level.innerRenderBounds.x0 - level.outerRenderBounds.x0 ), 
				abs( level.outerRenderBounds.x1 - level.innerRenderBounds.x0 ) );

			level.minTransitionSize[ 1 ] = std::min( 
				abs( level.innerRenderBounds.y0 - level.outerRenderBounds.y0 ), 
				abs( level.outerRenderBounds.y1 - level.innerRenderBounds.y0 ) );			
		}

	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::render( const FrustumVolume& viewFrustum )
	{		
		if( !terrainShader_.isValid() )
		{
			return;
		}
		glUseProgramObjectARB( terrainShader_.getProgramHandle() );
		
		// render from finest to coarsest:

		const int levelCount = levels_.size();

		for( int i = 0; i < levelCount; ++i )
		{
			int levelIdx = levelCount - i - 1;

			const ClipLevelData& level = levels_[ levelIdx ];
			const ClipLevelData* coarserLevel = 0;

			if( i > 0 )
			{
				coarserLevel = &levels_[ levelIdx + 1 ];
			}

			renderLevel( level, coarserLevel );	
		}

		glUseProgramObjectARB( 0 );
	}


	//-------------------------------------------------------------------------------------------------


//	void GpuClipmapRenderer::drawSampleRectangle( const Rectangle2i& rect, const Color3f& color ) const
//	{
//		Point2f from = samplePosToWorldPos( rect.getTopLeft() );
//		Point2f to = samplePosToWorldPos( rect.getBottomRight() - Point2i( 1, 1 ) );
//
//		glColor3f( color.red(), color.green(), color.blue() );
//
//		glBegin( GL_LINE_STRIP );
//
//		glVertex3f( from[ 0 ], 0, from[ 1 ] );
//		glVertex3f(   to[ 0 ], 0, from[ 1 ] );
//		glVertex3f(   to[ 0 ], 0,   to[ 1 ] );
//		glVertex3f( from[ 0 ], 0,   to[ 1 ] );
//		glVertex3f( from[ 0 ], 0, from[ 1 ] );
//
//		glEnd();	
//	}


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


//	void GpuClipmapRenderer::buildVertices( OpenGLClipmapData& block, const Rectangle2i& rect, const ClipLevelData& clipmapData, const ClipLevelData* coarserData )
//	{
//		Point2i samplePos;
//		Point2f rectWorldOrigin = samplePosToWorldPos( rect.getOrigin() );
//		Point2f rectWorldSpacing = componentMultiply( Point2f( clipmapData.sampleDistance ), worldSpacing_ );
//		Point2f worldPos = rectWorldOrigin;
//
//		ClipmapSampleIterator blockPosIt( clipmapData, rect.getOrigin() );
//
//		if( coarserData )
//		{		
//			Rectangle2i coarserSampleRect = coarserData->getSampleRect();
//
//			for( samplePos[ 1 ] = rect.y0; samplePos[ 1 ] < rect.y1; samplePos[ 1 ] += clipmapData.sampleDistance[ 1 ] )
//			{
//				for( samplePos[ 0 ] = rect.x0; samplePos[ 0 ] < rect.x1; samplePos[ 0 ] += clipmapData.sampleDistance[ 0 ] )
//				{
//					const size_t blockIndex = blockPosIt.getBlockIndex();
//					const size_t sampleIndex = blockPosIt.getSampleIndex();
//					OpenGLTerrainVertex& vertex = block.vertices[ blockIndex ];
//
//					Point2i blendWeights = componentModulo( samplePos, coarserData->sampleDistance );
//					Point2i coarserSamplePos = samplePos - blendWeights;
//
//					vertex.pos[ 0 ] = worldPos[ 0 ];
//					vertex.pos[ 1 ] = clipmapData.getSample( sampleIndex );
//					vertex.pos[ 2 ] = worldPos[ 1 ];
//
//					float coarserSample = vertex.pos[ 1 ];
//
//					if( coarserSampleRect.contains( samplePos ) )
//					{
//						ClipmapSampleIterator coarserBlockPosIt( *coarserData, coarserSamplePos );
//
//						const size_t coarserIndex00 = coarserBlockPosIt.getSampleIndex();
//						const size_t coarserIndex10 = coarserBlockPosIt.getSampleIndex( 1, 0 );
//						const size_t coarserIndex01 = coarserBlockPosIt.getSampleIndex( 0, 1 );
//
//						if( ( blendWeights[ 0 ] != 0 ) && ( blendWeights[ 1 ] != 0 ) )
//						{
//							coarserSample = ( coarserData->getSample( coarserIndex10 ) + coarserData->getSample( coarserIndex01 ) ) / 2.0f;
//						}
//						else if( blendWeights[ 0 ] != 0 )
//						{
//							coarserSample = ( coarserData->getSample( coarserIndex00 ) + coarserData->getSample( coarserIndex10 ) ) / 2.0f;
//						}
//						else if( blendWeights[ 1 ] != 0 )
//						{
//							coarserSample = ( coarserData->getSample( coarserIndex00 ) + coarserData->getSample( coarserIndex01 ) ) / 2.0f;
//						}
//						else
//						{
//							coarserSample = coarserData->getSample( coarserIndex00 );
//						}
//					}
//
//					vertex.pos[ 3 ] = coarserSample;
//
//					blockPosIt.incrementX();
//					worldPos[ 0 ] += rectWorldSpacing[ 0 ];
//				}
//
//				blockPosIt.incrementY();
//				worldPos[ 0 ] = rectWorldOrigin[ 0 ];
//				worldPos[ 1 ] += rectWorldSpacing[ 1 ];
//			}
//		}
//		else
//		{
//			for( samplePos[ 1 ] = rect.y0; samplePos[ 1 ] < rect.y1; samplePos[ 1 ] += clipmapData.sampleDistance[ 1 ] )
//			{
//				for( samplePos[ 0 ] = rect.x0; samplePos[ 0 ] < rect.x1; samplePos[ 0 ] += clipmapData.sampleDistance[ 0 ] )
//				{
//					const size_t blockIndex = blockPosIt.getBlockIndex();
//					const size_t sampleIndex = blockPosIt.getSampleIndex();
//					OpenGLTerrainVertex& vertex = block.vertices[ blockIndex ];
//
//					vertex.pos[ 0 ] = worldPos[ 0 ];
//					vertex.pos[ 1 ] = clipmapData.getSample( sampleIndex );
//					vertex.pos[ 2 ] = worldPos[ 1 ];
//					vertex.pos[ 3 ] = vertex.pos[ 1 ];
//
//					blockPosIt.incrementX();
//					worldPos[ 0 ] += rectWorldSpacing[ 0 ];
//				}
//
//				blockPosIt.incrementY();
//				worldPos[ 0 ] = rectWorldOrigin[ 0 ];
//				worldPos[ 1 ] += rectWorldSpacing[ 1 ];
//			}
//		}		
//	}


	//-------------------------------------------------------------------------------------------------


	//void GpuClipmapRenderer::buildNormals( OpenGLClipmapData& block, const Rectangle2i& rect, const ClipLevelData& clipmapData )
	//{
	//	Point2i samplePos;
	//	Point2f rectWorldOrigin = samplePosToWorldPos( rect.getOrigin() );
	//	Point2f rectWorldSpacing = Point2f( clipmapData.sampleDistance ) * worldSpacing_;
	//	Point2f worldPos = rectWorldOrigin;

	//	ClipmapSampleIterator blockPosIt( clipmapData, rect.getOrigin() );

	//	// use central differencing
	//	for( samplePos[ 1 ] = rect.y0; samplePos[ 1 ] < rect.y1; samplePos[ 1 ] += clipmapData.sampleDistance[ 1 ] )
	//	{
	//		for( samplePos[ 0 ] = rect.x0; samplePos[ 0 ] < rect.x1; samplePos[ 0 ] += clipmapData.sampleDistance[ 0 ] )
	//		{
	//			const size_t blockIndex = blockPosIt.getBlockIndex();
	//			OpenGLTerrainVertex& vertex = block.vertices[ blockIndex ];

	//			const float north = clipmapData.getSample( blockPosIt.getSampleIndex(  0, -1 ) );
	//			const float south = clipmapData.getSample( blockPosIt.getSampleIndex(  0, +1 ) );
	//			const float east  = clipmapData.getSample( blockPosIt.getSampleIndex( -1,  0 ) );
	//			const float west  = clipmapData.getSample( blockPosIt.getSampleIndex( +1,  0 ) );
	//			
	//			vertex.normal.set( ( east - west ) / worldSpacing_[ 0 ], 1.0f, ( north - south ) / worldSpacing_[ 1 ] );
	//			vertex.normal.normalize();

	//			blockPosIt.incrementX();
	//		}
	//	}
	//}



	//-------------------------------------------------------------------------------------------------


//	void GpuClipmapRenderer::buildNormals( OpenGLClipmapData& block, const Rectangle2i& rect, const ClipLevelData& clipmapData )
//	{
//		Point2i samplePos;
//		Point2f rectWorldOrigin = samplePosToWorldPos( rect.getOrigin() );
//		Point2f rectWorldSpacing = componentMultiply( Point2f( clipmapData.sampleDistance ), worldSpacing_ );
//		Point2f worldPos = rectWorldOrigin;
//
//		//ClipmapBlockIterator blockPosIt( clipmapData, rect.getOrigin() );
//
//		//// use central differencing
//		//for( samplePos[ 1 ] = rect.y0; samplePos[ 1 ] < rect.y1; samplePos[ 1 ] += clipmapData.sampleDistance[ 1 ] )
//		//{
//		//	for( samplePos[ 0 ] = rect.x0; samplePos[ 0 ] < rect.x1; samplePos[ 0 ] += clipmapData.sampleDistance[ 0 ] )
//		//	{
//		//		const size_t sampleIndex = blockPosIt.getBlockIndex();
//		//		OpenGLTerrainVertex& vertex = block.vertices[ sampleIndex ];
//
//		//		const float north = clipmapData.getSampleIndex( .samples[ blockPosIt.getClampedBlockIndex(  0, -1 ) ];
//		//		const float south = clipmapData.samples[ blockPosIt.getClampedBlockIndex(  0, +1 ) ];
//		//		const float east  = clipmapData.samples[ blockPosIt.getClampedBlockIndex( -1,  0 ) ];
//		//		const float west  = clipmapData.samples[ blockPosIt.getClampedBlockIndex( +1,  0 ) ];
//		//		
//		//		vertex.normal.set( ( east - west ) / worldSpacing_[ 0 ], 1.0f, ( north - south ) / worldSpacing_[ 1 ] );
//		//		vertex.normal.normalize();
//
//		//		blockPosIt.incrementX();
//		//		worldPos[ 0 ] += rectWorldSpacing[ 0 ];
//		//	}
//
//		//	blockPosIt.incrementY();
//		//	worldPos[ 0 ] = rectWorldOrigin[ 0 ];
//		//	worldPos[ 1 ] += rectWorldSpacing[ 1 ];
//		//}
//	}



	//-------------------------------------------------------------------------------------------------


//	bool GpuClipmapRenderer::buildIndices( OpenGLClipmapData& block, const ClipLevelData& clipmapData, const Frustum& viewFrustum )
//	{
//		// todo: speedup?! (insert optimal tristrips here..)
//		// and frustum culling...		
//
//		Rectangle2i outerBounds = clipmapData.outerRenderBounds;
//		Rectangle2i innerBounds = clipmapData.innerRenderBounds;
//
//		if( outerBounds.isEmpty() )
//		{
//			// nothing to render..
//			return false;
//		}
//
//		outerBounds.x1 -= clipmapData.sampleDistance[ 0 ];
//		outerBounds.y1 -= clipmapData.sampleDistance[ 1 ];
//		innerBounds.x1 -= clipmapData.sampleDistance[ 0 ];
//		innerBounds.y1 -= clipmapData.sampleDistance[ 1 ];
//
//		const Point2i dx( clipmapData.sampleDistance[ 0 ], 0 );
//		const Point2i dy( 0, clipmapData.sampleDistance[ 1 ] );
//		const Point2i dxdy( clipmapData.sampleDistance[ 0 ], clipmapData.sampleDistance[ 1 ] );
//
//		Point2i samplePos;
//
//		block.indexCount = 0;
//		size_t* indexPtr = &block.indices[ 0 ];
//
//		ClipmapSampleIterator blockPosIt( clipmapData, outerBounds.getOrigin() );
//
//		for( samplePos[ 1 ] = outerBounds.y0; samplePos[ 1 ] < outerBounds.y1; samplePos[ 1 ] += clipmapData.sampleDistance[ 1 ] )
//		{
//			for( samplePos[ 0 ] = outerBounds.x0; samplePos[ 0 ] < outerBounds.x1; samplePos[ 0 ] += clipmapData.sampleDistance[ 0 ] )
//			{
//				if( !innerBounds.contains( samplePos ) )
//				{
//					// insert triangle-pair:
//					const size_t i0 = blockPosIt.getBlockIndex();
//					const size_t i1 = blockPosIt.getBlockIndex( 1, 0 );
//					const size_t i2 = blockPosIt.getBlockIndex( 0, 1 );
//					const size_t i3 = blockPosIt.getBlockIndex( 1, 1 );
//
//					*indexPtr++ = i0;
//					*indexPtr++ = i2;
//					*indexPtr++ = i1;
//
//					*indexPtr++ = i1;
//					*indexPtr++ = i2;
//					*indexPtr++ = i3;
//
//					block.indexCount += 6;
//				}
//				blockPosIt.incrementX();
//			}
//
//			blockPosIt.incrementY();
//		}
//
//		return true;
//	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::renderLevel( const ClipLevelData& level, const ClipLevelData* coarserLevel )
	{
		glDisable( GL_LIGHTING );

		Color3f color = getDebugColor( level.sampleSpacing );

		terrainShader_.setUniform( "baseColor", color );

		glEnable( GL_CULL_FACE );
		glCullFace( GL_BACK );
		//glPolygonMode( GL_FRONT, GL_LINE );

		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();

		const int ringBlockSize = ( levelSampleCount_ + 1 ) / 4;
		
		const int ringBlockGridPoints = ringBlockSize - 1;
		
		const int ringBlockOffset0 = 0;
		const int ringBlockOffset1 = ringBlockGridPoints;
		const int ringBlockOffset2 = 2 * ringBlockGridPoints + 2;
		const int ringBlockOffset3 = 3 * ringBlockGridPoints + 2;

		const int ringBlockCount = 16;
		Pnt2i ringBlockOrigins[ ringBlockCount ] = 
		{
			Pnt2i( ringBlockOffset0, ringBlockOffset0 ), Pnt2i( ringBlockOffset1, ringBlockOffset0 ), Pnt2i( ringBlockOffset0, ringBlockOffset1 ),
			Pnt2i( ringBlockOffset2, ringBlockOffset0 ), Pnt2i( ringBlockOffset3, ringBlockOffset0 ), Pnt2i( ringBlockOffset3, ringBlockOffset1 ),
			Pnt2i( ringBlockOffset0, ringBlockOffset2 ), Pnt2i( ringBlockOffset1, ringBlockOffset3 ), Pnt2i( ringBlockOffset0, ringBlockOffset3 ),
			Pnt2i( ringBlockOffset3, ringBlockOffset2 ), Pnt2i( ringBlockOffset3, ringBlockOffset3 ), Pnt2i( ringBlockOffset2, ringBlockOffset3 ),

			Pnt2i( ringBlockOffset1, ringBlockOffset1 ), Pnt2i( ringBlockOffset2, ringBlockOffset1 ), 
			Pnt2i( ringBlockOffset1, ringBlockOffset2 ), Pnt2i( ringBlockOffset2, ringBlockOffset2 ),
		};

		const int drawRingBlockCount = coarserLevel ? 12 : 16;

		for( int i = 0; i < drawRingBlockCount; ++i )
		{
			//const int ringBlockSampleCount = ( ringBlockSize - 1 ) * level.sampleSpacing;

			Pnt4f scaleFactor;

			scaleFactor[ 0 ] = worldSpacing_ * level.sampleSpacing;
			scaleFactor[ 1 ] = worldSpacing_ * level.sampleSpacing;

			scaleFactor[ 2 ] = worldSpacing_ * ( level.origin[ 0 ] + ringBlockOrigins[ i ][ 0 ] * level.sampleSpacing );
			scaleFactor[ 3 ] = worldSpacing_ * ( level.origin[ 1 ] + ringBlockOrigins[ i ][ 1 ] * level.sampleSpacing );

			terrainShader_.setUniform( "scaleFactor", scaleFactor );
			renderBlock( ringBlock_ );
		}

		glPopMatrix();
		glEnable( GL_LIGHTING );

//		Point2i transitionSize;
//
//		transitionSize[ 0 ] = std::min( ( clipmapData.blockSize[ 0 ] / 10 ) * clipmapData.sampleDistance[ 0 ], clipmapData.minTransitionSize[ 0 ] );
//		transitionSize[ 1 ] = std::min( ( clipmapData.blockSize[ 1 ] / 10 ) * clipmapData.sampleDistance[ 1 ], clipmapData.minTransitionSize[ 1 ] );
//
//		Point2f activeRegionMin = samplePosToWorldPos( clipmapData.outerRenderBounds.getTopLeft() );
//		Point2f activeRegionMax = samplePosToWorldPos( clipmapData.outerRenderBounds.getBottomRight() - clipmapData.sampleDistance );
//
//		Point2f worldTransitionSize = componentMultiply( Point2f( transitionSize ), worldSpacing_ );
//
//		//Point2f activeRegionCenter = samplePosToWorldPos( clipmapData.outerRenderBounds.getTopLeft() + clipmapData.outerRenderBounds.getSize() / 2 );		
//		//Point2f localViewerPos( worldViewerPosition_[ 0 ], worldViewerPosition_.z );
//		//Point2i viewerSamplePos = worldPosToSamplePos( localViewerPos );
//		//Point2f baseLocalViewerPos = samplePosToWorldPos( viewerSamplePos );
//
//		//localViewerPos -= baseLocalViewerPos;
//		
//		terrainShader_.setUniform( "transitionWidth", worldTransitionSize );
//		terrainShader_.setUniform( "activeRegionMin", activeRegionMin );
//		terrainShader_.setUniform( "activeRegionMax", activeRegionMax );
//		//terrainShader_.setUniform( "activeRegionCenter", activeRegionCenter );		
//		terrainShader_.setUniform( "localViewerPos", worldViewerPosition_ );
//		terrainShader_.setUniform( "baseColor0", clipmapData.debugColor );
//
//		if( coarserData )
//		{
//			terrainShader_.setUniform( "baseColor1", coarserData->debugColor );
//		//	//terrainShader_.setUniform( "alpha", alpha_ );
//		}
//		else
//		{
//			terrainShader_.setUniform( "baseColor1", clipmapData.debugColor );
//		//	//terrainShader_.setUniform( "alpha", 0.0f );
//		}
//
//		glVertexPointer( 4, GL_FLOAT, sizeof( OpenGLTerrainVertex ), &block.vertices[ 0 ].pos );
//		glEnableClientState( GL_VERTEX_ARRAY );
//
//		//glNormalPointer( GL_FLOAT, sizeof( OpenGLTerrainVertex ), &block.vertices[ 0 ].normal );
//		//glEnableClientState( GL_NORMAL_ARRAY );
//
//		//glPolygonMode( GL_FRONT, GL_LINE );
//		// todo: 16bit indices?!
//		glDrawElements( GL_TRIANGLES, GLsizei( block.indexCount ), GL_UNSIGNED_INT, &block.indices[ 0 ] );
//
//		//glDisableClientState( GL_NORMAL_ARRAY );
//		glDisableClientState( GL_VERTEX_ARRAY );
//
//
//		glUseProgramObjectARB( 0 );
//		glDisable( GL_LIGHTING );
//		
//		
//		//glBegin( GL_LINES );
//
//		//glColor3f( 1, 1, 1 );
//
//		//for( size_t i = 0; i < block.vertices.size(); ++i )
//		//{
//		//	const OpenGLTerrainVertex& vertex = block.vertices[ i ];
//		//	glVertex3f( vertex.pos[ 0 ], vertex.pos[ 1 ], vertex.pos.z );
//		//	//glVertex3f( vertex.pos[ 0 ] + vertex.normal[ 0 ], vertex.pos[ 1 ] + vertex.normal[ 1 ], vertex.pos.z + vertex.normal.z );
//		//	glVertex3f( vertex.pos[ 0 ], vertex.pos.w, vertex.pos.z );
//		//}
//
//		glUseProgramObjectARB( terrainShader_.getProgramHandle() );
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::renderBlock( const Block& block )
	{
		glVertexPointer( 2, GL_SHORT, sizeof( TerrainVertex ), &block.vertices[ 0 ] );
		glEnableClientState( GL_VERTEX_ARRAY );

		int triangleCount = block.indices.size() / 3;
		glDrawElements( GL_TRIANGLES, 3 * triangleCount, GL_UNSIGNED_SHORT, &block.indices[ 0 ] );

		glDisableClientState( GL_VERTEX_ARRAY );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2f GpuClipmapRenderer::samplePosToWorldPos( const Pnt2i& samplePos ) const
	{
		return Pnt2f( float( samplePos[ 0 ] ) * worldSpacing_, float( samplePos[ 1 ] ) * worldSpacing_ );
	}


	//-------------------------------------------------------------------------------------------------
	

	Pnt2i GpuClipmapRenderer::worldPosToSamplePos( const Pnt2f& worldPos ) const
	{
		return Pnt2i( int( worldPos[ 0 ] / worldSpacing_ ), int( worldPos[ 1 ] / worldSpacing_ ) );		
	}


	//-------------------------------------------------------------------------------------------------

	
	void GpuClipmapRenderer::buildRingBlock( Block& target, int levelSampleCount  )
	{
		const int ringBlockSize = ( levelSampleCount + 1 ) / 4;

		buildBlock( target, ringBlockSize, ringBlockSize );
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::buildFixupBlock( Block& horizontalBlock, Block& verticalBlock, int levelSampleCount )
	{
		const int ringBlockSize = ( levelSampleCount + 1 ) / 4;

		buildBlock( horizontalBlock, ringBlockSize, 3 );
		buildBlock( verticalBlock, 3, ringBlockSize );
	}


	//-------------------------------------------------------------------------------------------------


	void GpuClipmapRenderer::buildBlock( Block& target, int vertexCountX, int vertexCountY )
	{
		// we have m * m vertices:
		target.vertices.resize( vertexCountX * vertexCountY );

		// fill vertices:
		TerrainVertex* vertexPtr = &target.vertices[ 0 ];
		for( int y = 0; y < vertexCountY; ++y )
		{
			for( int x = 0; x < vertexCountX; ++x )
			{
				vertexPtr->gridPos.setValues( x, y );
				vertexPtr++;
			}
		}

		// fill indices: (todo: use optimized indices here)
		const int triangleCount = 2 * ( vertexCountX - 1 ) * ( vertexCountY - 1 );

		target.indices.resize( 3 * triangleCount );

		Int16 index0 = 0;
		Int16* indexPtr = &target.indices[ 0 ];

		for( int y = 0; y < vertexCountY - 1; ++y )
		{
			for( int x = 0; x < vertexCountX - 1; ++x )
			{
				index0 = y * vertexCountX + x;

				Int16 index1 = index0 + 1;
				Int16 index2 = index0 + vertexCountX;
				Int16 index3 = index2 + 1;

				// two triangles:
				*indexPtr++ = index0;
				*indexPtr++ = index2;
				*indexPtr++ = index1;

				*indexPtr++ = index1;
				*indexPtr++ = index2;				
				*indexPtr++ = index3;
			}
		}
	}


	//-------------------------------------------------------------------------------------------------


	int GpuClipmapRenderer::updateLevel( const ImagePtr& heightData, ClipLevelData& level, const Rectangle2i& desiredClipRegion )
	{
		assert( desiredClipRegion.getWidth() <= level.sampleRectSize[ 0 ] );
		assert( desiredClipRegion.getHeight() <= level.sampleRectSize[ 1 ] );

		level.changedRects.clear();

		// in general the update-region is L-Shaped:
		const Rectangle2i oldClipRegion( level.sampleOrigin, level.sampleRectSize );

		const Point2i newOrigin = desiredClipRegion.getOrigin();
		const Point2i oldOrigin = oldClipRegion.getOrigin();

		const Rectangle2i newClipRegion( newOrigin, level.sampleRectSize );

		assert( oldClipRegion.getSize() == componentMultiply( level.blockSize - Point2i( 1, 1 ), level.sampleDistance + Point2i( 1, 1 ) ) );
		assert( oldClipRegion.getSize() == newClipRegion.getSize() );

		const Point2i updateSampleCount = newOrigin - oldOrigin;

		const bool initialUpdate = level.samplesDirty || !updateSampleCount.isZero(); //level.samplesDirty;
		const bool fullBlockUpdate = ( abs( updateSampleCount[ 0 ] ) >= level.blockSize[ 0 ] ) || ( abs( updateSampleCount[ 1 ] ) >= level.blockSize[ 1 ] );

		size_t updatedSampleCount = 0;

		if( initialUpdate || fullBlockUpdate )
		{
			// refill complete block..
			level.sampleOrigin = newOrigin;
			level.blockOrigin.setValues( 0, 0 );
			updatedSampleCount += updateLevelRect( terrainData, level, level.getSampleRect() );
			level.samplesDirty = false;
		}
		else if( updateSampleCount[ 0 ] != 0 || updateSampleCount[ 1 ] != 0 )
		{
			// adjust adressing..
			level.blockOrigin = level.blockOrigin + componentModulo( componentDivide( updateSampleCount, level.sampleDistance ) + level.blockSize, level.blockSize );

			assert( level.blockOrigin[ 0 ] >= 0 );
			assert( level.blockOrigin[ 1 ] >= 0 );
			level.sampleOrigin	= newOrigin;			

			// update the dirty Rects:			
			int leftUpdateBorder = oldClipRegion.x0;
			int rightUpdateBorder = oldClipRegion.x1;

			if( updateSampleCount[ 0 ] != 0 )
			{
				Rectangle2i updateRectX;

				if( updateSampleCount[ 0 ] > 0 )
				{
					// we need new samples on the right side..
					updateRectX.x0 = oldClipRegion.x1 - 1;
					updateRectX.y0 = newClipRegion.y0;
					updateRectX.x1 = newClipRegion.x1;
					updateRectX.y1 = newClipRegion.y1;

					leftUpdateBorder	= newClipRegion.x0;
					rightUpdateBorder	= oldClipRegion.x1;					
				}
				else
				{
					// we need new samples on the left side..					
					updateRectX.x0 = newClipRegion.x0;
					updateRectX.y0 = newClipRegion.y0;
					updateRectX.x1 = oldClipRegion.x0 + 1;
					updateRectX.y1 = newClipRegion.y1;

					leftUpdateBorder	= oldClipRegion.x0;
					rightUpdateBorder	= newClipRegion.x1;
				}
				assert( !updateRectX.isEmpty() );
				updatedSampleCount += updateLevelRect( terrainData, level, updateRectX );
			}

			assert( leftUpdateBorder < rightUpdateBorder );

			Rectangle2i updateRectY;

			if( updateSampleCount[ 1 ] != 0 )
			{
				if( updateSampleCount[ 1 ] > 0 )
				{
					// we need new samples on the top side.
					updateRectY.x0 = leftUpdateBorder;
					updateRectY.y0 = oldClipRegion.y1 - 1;
					updateRectY.x1 = rightUpdateBorder;
					updateRectY.y1 = newClipRegion.y1;
				}
				else
				{
					// we need new samples on the bottom side.
					updateRectY.x0 = leftUpdateBorder;
					updateRectY.y0 = newClipRegion.y0;
					updateRectY.x1 = rightUpdateBorder;
					updateRectY.y1 = oldClipRegion.y0 + 1;
				}
				assert( !updateRectY.isEmpty() );
				updatedSampleCount += updateLevelRect( terrainData, level, updateRectY );
			}

			// something has changed: determine the min and max values:
			level.calculateMinMaxSamples();
		}		

		return 0;
	}


	//-------------------------------------------------------------------------------------------------


	int GpuClipmapRenderer::updateLevelRect( const ImagePtr& heightData, ClipLevelData& level, const Rectangle2i& sampleRect )
	{
		Point2i samplePos;

		ClipmapSampleIterator blockPosIt( level, sampleRect.getOrigin() );

		int updatedSampleCount = 0;

		Point2i dx( level.sampleDistance[ 0 ], 0 );
		Point2i dy( 0, level.sampleDistance[ 1 ] );
		Point2i dxy( level.sampleDistance[ 0 ], level.sampleDistance[ 1 ] );

		for( samplePos[ 1 ] = sampleRect.y0; samplePos[ 1 ] < sampleRect.y1; samplePos[ 1 ] += level.sampleDistance[ 1 ] )
		{
			for( samplePos[ 0 ] = sampleRect.x0; samplePos[ 0 ] < sampleRect.x1; samplePos[ 0 ] += level.sampleDistance[ 0 ] )
			{
				const size_t sampleIndex = blockPosIt.getSampleIndex();

				level.setSample( sampleIndex, heightData->getSample( samplePos ) );
				updatedSampleCount++;

				bool updateBorders[ 4 ];

				//updateBorders[ 0 ] = samplePos[ 0 ] == sampleRect.x0;
				//updateBorders[ 1 ] = samplePos[ 1 ] == sampleRect.y0;
				//updateBorders[ 2 ] = samplePos[ 0 ] >= sampleRect.x1 - level.sampleDistance[ 0 ];
				//updateBorders[ 3 ] = samplePos[ 1 ] >= sampleRect.y1 - level.sampleDistance[ 1 ];
				updateBorders[ 0 ] = true;
				updateBorders[ 1 ] = true;
				updateBorders[ 2 ] = true;
				updateBorders[ 3 ] = true;

				if( updateBorders[ 0 ] && updateBorders[ 1 ] )
				{
					// top left:
					level.setSample( blockPosIt.getSampleIndex( -1, -1 ), terrainData->getSample( samplePos - dxy ) );
					updatedSampleCount++;
				}
				if( updateBorders[ 0 ] )
				{
					// left:
					level.setSample( blockPosIt.getSampleIndex( -1, 0 ), terrainData->getSample( samplePos - dx ) );
					updatedSampleCount++;
				}
				if( updateBorders[ 1 ] )
				{
					// top
					level.setSample( blockPosIt.getSampleIndex( 0, -1 ), terrainData->getSample( samplePos - dy ) );
					updatedSampleCount++;
				}

				if( updateBorders[ 2 ] && updateBorders[ 3 ] )
				{
					// bottom right:
					level.setSample( blockPosIt.getSampleIndex( +1, +1 ), terrainData->getSample( samplePos + dxy ) );
					updatedSampleCount++;
				}
				if( updateBorders[ 2 ] )
				{
					// right:
					level.setSample( blockPosIt.getSampleIndex( +1, 0 ), terrainData->getSample( samplePos + dx ) );
					updatedSampleCount++;
				}
				if( updateBorders[ 3 ] )
				{
					// bottom
					level.setSample( blockPosIt.getSampleIndex( 0, +1 ), terrainData->getSample( samplePos + dy ) );
					updatedSampleCount++;
				}

				blockPosIt.incrementX();
			}

			blockPosIt.incrementY();
		}

		//const Point2i sampleCount = level.getSampleCount();

		//// check the boundary samples:
		//if( sampleRect.x0 == level.sampleOrigin[ 0 ] )
		//{
		//	// update left border:
		//	samplePos = sampleRect.getOrigin() - level.sampleDistance;

		//	for( int y = 0; y < sampleCount[ 1 ]; ++y )
		//	{
		//		const int sampleIndex = ( ( y - level.blockOrigin[ 1 ] ) + sampleCount[ 1 ] ) % sampleCount[ 1 ] * sampleCount[ 0 ];

		//		level.setSample( sampleIndex, terrainData->getSample( samplePos ) );

		//		blockPosIt.incrementY();

		//		samplePos[ 1 ] += level.sampleDistance[ 1 ];
		//	}
		//}
		//if( sampleRect.x1 == level.getSampleRect().x1 )
		//{
		//	// update right border:
		//	samplePos = sampleRect.getOrigin() - level.sampleDistance;

		//	for( int y = 0; y < sampleCount[ 1 ]; ++y )
		//	{
		//		const int sampleIndex = ( ( y - level.blockOrigin[ 1 ] ) + sampleCount[ 1 ] ) % sampleCount[ 1 ] * sampleCount[ 0 ];

		//		level.setSample( sampleIndex, terrainData->getSample( samplePos ) );

		//		blockPosIt.incrementY();

		//		samplePos[ 1 ] += level.sampleDistance[ 1 ];
		//	}
		//}



		//size_t updatedSampleCount = 0;

		//Point2i samplePos;

		//for( samplePos[ 1 ] = sampleRect.y0; samplePos[ 1 ] < sampleRect.y1; samplePos[ 1 ] += level.sampleDistance[ 1 ] )
		//{
		//	for( samplePos[ 0 ] = sampleRect.x0; samplePos[ 0 ] < sampleRect.x1; samplePos[ 0 ] += level.sampleDistance[ 0 ] )
		//	{				
		//		const size_t sampleIndex = level.getSampleIndex( samplePos );

		//		level.setSample( sampleIndex, terrainData->getSample( samplePos ) );
		//		
		//	}
		//}

		level.changedRects.push_back( sampleRect );

		return updatedSampleCount;
	}


	//-------------------------------------------------------------------------------------------------


	int getAlignedInt( int x, int n, bool roundUp )
	{
		assert( n > 0 );

		int result = ( x / n ) * n;

		if( roundUp && ( result < x ) )
		{
			return result + n;
		}
		else if( !roundUp && ( result > x ) )
		{
			return result - n;
		}
		else
		{
			return result;
		}
	}


	//-------------------------------------------------------------------------------------------------


	Rectangle2i alignRectangle( const Rectangle2i& rect, const Point2i& alignment )
	{	
		Rectangle2i alignedRect;

		// todo: adjust the active region so that the active region lies on "even" vertices.
		alignedRect.x0 = getAlignedInt( rect.x0, alignment[ 0 ], true );
		alignedRect.y0 = getAlignedInt( rect.y0, alignment[ 1 ], true );
		alignedRect.x1 = getAlignedInt( rect.x1, alignment[ 0 ], false );
		alignedRect.y1 = getAlignedInt( rect.y1, alignment[ 1 ], false );

		if( alignedRect.x1 < rect.x1 )
		{
			alignedRect.x1++;
		}
		else if( alignedRect.x1 == rect.x1 )
		{
			alignedRect.x1--;
		}
		if( alignedRect.y1 < rect.y1 )
		{
			alignedRect.y1++;
		}
		else if( alignedRect.y1 == rect.y1 )
		{
			alignedRect.y1--;
		}

		return alignedRect;
	}


	//-------------------------------------------------------------------------------------------------


#define countof(X) (sizeof(X)/sizeof(X[0]))
	Color3f getDebugColor( size_t index )
	{
		static const Color3f debugColors[] =
		{
			Color3f( 1, 0, 0 ),
				Color3f( 0, 1, 0 ),
				Color3f( 0, 0, 1 ),
				Color3f( 0, 1, 1 ),
				Color3f( 1, 0, 1 ),
				Color3f( 1, 1, 0 ),
		};

		return debugColors[ index % countof( debugColors ) ];
	}
}

//-------------------------------------------------------------------------------------------------

#endif // 0
