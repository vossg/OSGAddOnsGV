//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:20:35
//
//	Description			:		OSGCpuClipmapRenderer.cpp
//
//-------------------------------------------------------------------------------------------------

#include "PrecompiledHeader.h"
#include "OSGDynamicTerrain.h"
#include "OSGRectangle.h"
#include "OSGStatelessGlslShader.h"
#include "OSGTerrainTools.h"
#include "OSGHeightDataSource.h"
#include "OSGCpuClipmapRenderer.h"
#include "OSGTerrainTypes.h"
#include <algorithm>
#include <OSGImage.h>
#include <OSGTextureChunk.h>
#include <OSGRenderAction.h>

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	const char* DefaultVertexProgramText = 
		"uniform vec2 transitionWidth;"
		"uniform vec2 activeRegionCenter;"
		"uniform vec2 activeRegionMin;"
		"uniform vec2 activeRegionMax;"
		"uniform vec3 localViewerPos;"
		"uniform vec2 worldOffset;"
		"uniform float sampleDistance;"
		"uniform float heightScale;"
		"uniform float heightOffset;"
		""
		"varying float alpha;"
		"varying float nDotVP;"
		"varying float nDotVPCoarse;"
		"varying vec4 color;"
		""
		"void main( void )"
		"{"
		"	float alphax = clamp( ( abs( gl_Vertex.x - localViewerPos.x ) - ( ( activeRegionMax.x - activeRegionMin.x ) / 2.0 - transitionWidth.x - 1.0 ) ) / transitionWidth.x, 0.0, 1.0 );"
		"	float alphay = clamp( ( abs( gl_Vertex.z - localViewerPos.z ) - ( ( activeRegionMax.y - activeRegionMin.y ) / 2.0 - transitionWidth.y - 1.0 ) ) / transitionWidth.y, 0.0, 1.0 );"
		""
		"	alpha = max( alphax, alphay );"
		""
		"	vec3 viewNormal = gl_NormalMatrix * gl_Normal;"
		""
		"	nDotVP = max( 0.0, dot( viewNormal, vec3( gl_LightSource[ 0 ].position ) ) );"
		""
		"	vec4 pos;"
		"	pos.x = worldOffset.x + sampleDistance * gl_Vertex.x;"
		"	pos.z = worldOffset.y + sampleDistance * gl_Vertex.z;"
		"	pos.y = heightScale * mix( gl_Vertex.y, gl_Vertex.w, alpha ) + heightOffset;"
		"	pos.w = 1.0;"
		""
		"	gl_Position = gl_ModelViewProjectionMatrix * pos;"
		"	gl_TexCoord[0] = gl_MultiTexCoord0;"
		"	color = gl_Color;"
		"}";

	const char* DefaultFragmentProgramText = 
		"uniform vec3 baseColor0;"
		"uniform vec3 baseColor1;"
		"uniform sampler2D texSampler;"
		""
		"varying float alpha;"
		"varying float nDotVP;"
		"varying vec4 color;"
		""
		"void main( void )"
		"{"
		"  vec3 diffuseColor = mix( baseColor0, baseColor1, alpha );"
		"  vec3 texcolor = vec3( texture2D( texSampler, vec2( gl_TexCoord[ 0 ] ) ) );"
		""
		"  gl_FragColor = vec4( texcolor, 1.0 );"
		"}";


	//-------------------------------------------------------------------------------------------------

	Color3f			getDebugColor( int index );	

	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


	CpuClipmapRenderer::CpuClipmapRenderer()
	{
		window_ = 0;
		useVertexBufferObjects_ = false;

		vertexProgramText_		= DefaultVertexProgramText;
		fragmentProgramText_	= DefaultFragmentProgramText;

		programTextChanged_		= true;

		stats_.clear();	
	}


	//-------------------------------------------------------------------------------------------------


	CpuClipmapRenderer::~CpuClipmapRenderer()
	{
		shutdown();
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::setVertexProgram( const char* programText )
	{
		if( !programText || strlen( programText ) < 2 )
		{
			return;
		}
		vertexProgramText_ = programText;
		programTextChanged_ = true;
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::setFragmentProgram( const char* programText )
	{
		if( !programText || strlen( programText ) < 2 )
		{
			return;
		}
		fragmentProgramText_ = programText;
		programTextChanged_ = true;
	}


	//-------------------------------------------------------------------------------------------------


	bool CpuClipmapRenderer::onInitialize( GeometryClipmaps* geoClipmaps )
	{
		const int levelCount = geoClipmaps->getLevelCount();

		levels_.resize( levelCount );

		for( int levelIdx = 0; levelIdx < levelCount; ++levelIdx )
		{
			GeometryClipmapLevel& level = geoClipmaps->getLevel( levelIdx );
			TerrainLevelRenderData& levelRenderData = levels_[ levelIdx ];

			const int levelSampleCount = level.heightmap.size;

			// todo: put the vertex (and index?) data into a gpu buffer:
			levelRenderData.vertices.resize( levelSampleCount * levelSampleCount );
			levelRenderData.indices.resize( 6 * ( levelSampleCount - 1 ) * ( levelSampleCount - 1 ) );

			levelRenderData.rebuildIndices = true;
		}

		return true;
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::onShutdown()
	{
		levels_.clear();
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::onUpdateRenderData( const Pnt3f& viewPosition )
	{
		//todo: check, if the terrain data has changed, and update the vertices:

		viewerPosition_ = viewPosition;
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::onDestroyGpuResources()
	{
		terrainShader_.destroy();

		const int levelCount = levels_.size();

		for( int i = 0; i < levelCount; ++i )
		{
			TerrainLevelRenderData& levelData = levels_[ i ];

			levelData.vertexBuffer.destroy();

			levelData.texture = NullFC;
		}
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::onCreateGpuResources()
	{
		assert( window_ );

		reloadShader();

		const int levelCount = levels_.size();

		// if the extension is supported:
		useVertexBufferObjects_ = true;

		for( int i = 0; i < levelCount; ++i )
		{
			TerrainLevelRenderData& levelData = levels_[ i ];
			GeometryClipmapLevel& level = geoClipmaps_->getLevel( i );

			// create vbo for the vertex data: 
			// todo: create a dynamic vbo for the index data?
			const int vertexCount = levelData.vertices.size();

			// todo: use ReadOnly Vbo (for speed)
			if( !levelData.vertexBuffer.create( window_, GL_ARRAY_BUFFER_ARB, sizeof( OpenGLTerrainVertex ) * vertexCount, BufferUsage_Dynamic_ReadWrite ) )
			{
				useVertexBufferObjects_ = false;
				break;
			}
			levelData.vertexBuffer.uploadData( &levelData.vertices[ 0 ], 0, sizeof( OpenGLTerrainVertex ) * vertexCount );
			levelData.vertexBuffer.deactivate();

			// create the texture:
			levelData.texture = TextureChunk::create();

			beginEditCP( levelData.texture );
			levelData.texture->setImage( level.textureData );
			levelData.texture->setMinFilter( GL_NEAREST );		// TODO: use linear min/mag filters (but no mipmapping!
			levelData.texture->setMagFilter( GL_NEAREST );
			levelData.texture->setWrapS( GL_REPEAT );
			levelData.texture->setWrapT( GL_REPEAT );
			levelData.texture->setEnvMode( GL_MODULATE );
			endEditCP( levelData.texture );
		}
	}

	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::onRender( const ClipmapRenderParameters& renderParameters )
	{
		// activate the gpu program:
		//glDisable( GL_LIGHTING );
		terrainShader_.activate();

		if( programTextChanged_ )
		{
			reloadShader();
			programTextChanged_ = false;
		}

		bool useDependentTextureLookup = false;

		if( renderParameters.globalTexture != NullFC )
		{
			renderParameters.globalTexture->activate( renderParameters.renderAction );
		}
		else if( renderParameters.heightColorTexture != NullFC )
		{
			// todo: use a different pixel shader here
			useDependentTextureLookup = true;
			renderParameters.heightColorTexture->activate( renderParameters.renderAction );
		}

		// render all levels from finest to coarsest:
		const int levelCount = levels_.size();

		//		for( int i = 0; i < levelCount; ++i )
		for( int i = 0; i < levelCount; ++i )
		{
			int levelIdx = levelCount - i - 1;

			//std::cerr << "Rendering Level " << levelIdx << std::endl;

			GeometryClipmapLevel& level = geoClipmaps_->getLevel( levelIdx );
			GeometryClipmapLevel* coarserLevel = 0;
			GeometryClipmapLevel* finerLevel = 0;

			if( levelIdx > 0 )
			{
				coarserLevel = &geoClipmaps_->getLevel( levelIdx - 1 );
			}
			if( levelIdx + 1 < levelCount )
			{
				finerLevel = &geoClipmaps_->getLevel( levelIdx + 1 );
			}

			const Color3f& debugColor = getDebugColor( levelIdx );

			terrainShader_.setUniform( "baseColor0", colorToVector( debugColor ) );
			terrainShader_.setUniform( "sampleDistance", renderParameters.worldTransform.sampleDistance );
			terrainShader_.setUniform( "worldOffset", renderParameters.worldTransform.offset );
			terrainShader_.setUniform( "heightScale", renderParameters.worldTransform.heightScale );
			terrainShader_.setUniform( "heightOffset", renderParameters.worldTransform.heightOffset );

			if( renderParameters.showTransitionRegions )
			{
				// todo: add a sensible debug representation..
				drawSampleRectangle( level.getCoveredSampleRect(), debugColor, renderParameters.worldTransform );
				//drawSamples( level, Color3f( 0, 1, 0 ) );
				//drawVertices( level, debugColor );
				drawBlendLines( level, debugColor, renderParameters.worldTransform );
			}

			if( !level.isActive ) 
			{				
				continue;
			}
			// only use the finer level, if it is active:
			if( finerLevel && !finerLevel->isActive )
			{
				finerLevel = 0;
			}

			// buildIndices dynamically builds the indices for this level and does frustum culling
			// the method returns false if nothing is visible

			// todo: dont rebuild the indices every time.. just rebuild them if something changed (inside the update() call)
			if( buildIndices( level, finerLevel ) )
			{
				// finally: draw the block:
				renderBlock( level, coarserLevel, renderParameters, debugColor );
			}

			stats_.drawnLevelCount++;
		}

		if( renderParameters.globalTexture != NullFC )
		{
			renderParameters.globalTexture->deactivate( renderParameters.renderAction );
		}
		else if( renderParameters.heightColorTexture != NullFC )
		{
			renderParameters.heightColorTexture->deactivate( renderParameters.renderAction );
		}

		terrainShader_.deactivate();
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::onReloadShader()
	{
		assert( window_ );
		std::string errorMsg;

		if( !terrainShader_.create( window_, vertexProgramText_.c_str(), fragmentProgramText_.c_str(), errorMsg ) )
		{
			PWARNING << "[DynamicTerrain] " << "Error compiling the GPU Program: " << errorMsg << "\n";
		}

	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::onRebuildIndices( GeometryClipmapLevel& level )
	{
		TerrainLevelRenderData& levelRenderData = levels_[ level.index ];

		levelRenderData.rebuildIndices = true;
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::onBuildVertices( GeometryClipmapLevel& level, const GeometryClipmapLevel* coarserLevel, const Rectangle2i& blockRect )
	{
		const int levelSampleCount = level.heightmap.size;

		TerrainLevelRenderData& levelRenderData = levels_[ level.index ];

		// loop over the changed vertices:
		Pnt2i samplePos;
		Pnt2f worldPos;

		const Pnt2i worldSampleCount = geoClipmaps_->getWorldSampleCount();

		for( int y = blockRect.y0; y < blockRect.y1; ++y )
		{
			const int index = y * levelSampleCount + blockRect.x0;
			const float* samplePtr = &level.heightmap.samples[ index ];
			OpenGLTerrainVertex* vertexPtr = &levelRenderData.vertices[ index ];

			for( int x = blockRect.x0; x < blockRect.x1; ++x )
			{
				// todo: make this incremental
				samplePos = level.blockPosToSamplePos( Pnt2i( x, y ) );

				vertexPtr->pos[ 0 ] = samplePos[ 0 ];
				vertexPtr->pos[ 1 ] = *samplePtr;
				vertexPtr->pos[ 2 ] = samplePos[ 1 ];
				vertexPtr->pos[ 3 ] = *samplePtr;

				// todo: correct uv coordinates:
				vertexPtr->uv.setValues( ( float )samplePos[ 0 ] / ( float )worldSampleCount[ 0 ], ( float )samplePos[ 1 ] / ( float )worldSampleCount[ 1 ] );
				//vertexPtr->uv.setValues( float( x ) / float( levelSampleCount - 1 ), float( y ) / float( levelSampleCount - 1 ) );

				samplePtr++;
				vertexPtr++;
			}
		}

		if( coarserLevel )
		{
			// todo: fill in the interpolated positions of the coarser level:
			const int coarserLevelSpacing = coarserLevel->sampleSpacing;
			const int coarserLevelSampleCount = coarserLevel->heightmap.size;

			// todo: make this faster
			for( int y = blockRect.y0; y < blockRect.y1; ++y )
			{
				const int index = y * levelSampleCount + blockRect.x0;
				OpenGLTerrainVertex* vertexPtr = &levelRenderData.vertices[ index ];

				for( int x = blockRect.x0; x < blockRect.x1; ++x, ++vertexPtr )
				{
					// todo: make this incremental
					Pnt2i samplePos = level.blockPosToSamplePos( Pnt2i( x, y ) );
					Pnt2i blendWeights = componentModulo( samplePos, coarserLevelSpacing );

					if( blendWeights[ 0 ] == 0 && blendWeights[ 1 ] == 0 )
					{
						continue;
					}

					Pnt2i coarserSamplePos = samplePos - blendWeights;

					Pnt2i coarserSamplePos10 = componentAdd( coarserSamplePos, Pnt2i( coarserLevelSpacing, 0 ) );
					Pnt2i coarserSamplePos01 = componentAdd( coarserSamplePos, Pnt2i( 0, coarserLevelSpacing ) );

					// todo: clip on a higher level (not per pixel)
					if( !coarserLevel->containsSample( coarserSamplePos ) || 
						!coarserLevel->containsSample( coarserSamplePos10 ) || 
						!coarserLevel->containsSample( coarserSamplePos01 ) )
					{
						// no data}
						continue;
					}
					
					Pnt2i coarserBlockPos = coarserLevel->samplePosToBlockPos( coarserSamplePos );

					assert( coarserBlockPos[ 0 ] < coarserLevelSampleCount && coarserBlockPos[ 1 ] < coarserLevelSampleCount );

					Pnt2i coarserBlockPos10 = coarserLevel->samplePosToBlockPos( coarserSamplePos10 );
					Pnt2i coarserBlockPos01 = coarserLevel->samplePosToBlockPos( coarserSamplePos01 );

					if( ( blendWeights[ 0 ] != 0 ) && ( blendWeights[ 1 ] != 0 ) )
					{
						// todo: is this case important?
					}
					else if( blendWeights[ 0 ] != 0 )
					{
						const float coarserSample00 = coarserLevel->getBlockSample( coarserBlockPos );
						const float coarserSample10 = coarserLevel->getBlockSample( coarserBlockPos10 );

						vertexPtr->pos[ 3 ] = ( coarserSample00 + coarserSample10 ) / 2.0f;						
					}
					else if( blendWeights[ 1 ] != 0 )
					{
						const float coarserSample00 = coarserLevel->getBlockSample( coarserBlockPos );
						const float coarserSample01 = coarserLevel->getBlockSample( coarserBlockPos01 );

						vertexPtr->pos[ 3 ] = ( coarserSample00 + coarserSample01 ) / 2.0f;						
					}
				}
			}
		}

		if( levelRenderData.vertexBuffer.isValid() )
		{
			// todo: upload only the changed data rect:
			const int vertexCount = levelRenderData.vertices.size();

			//std::cerr << "Uploading Vbo Data " << blockRect << std::endl;

			const int vboLineLength = sizeof( OpenGLTerrainVertex ) * blockRect.getWidth();
			for( int y = blockRect.y0; y < blockRect.y1; ++y )
			{
				const int index = y * levelSampleCount + blockRect.x0;
				const int vboOffset = index * sizeof( OpenGLTerrainVertex );

				levelRenderData.vertexBuffer.uploadData( &levelRenderData.vertices[ index ], vboOffset, vboLineLength );
			}

			// testing: upload the complete vbo:
			//std::cerr << "Uploading VBO Data" << std::endl;
			//levelRenderData.vertexBuffer.uploadData( &levelRenderData.vertices[ 0 ], 0, sizeof( OpenGLTerrainVertex ) * vertexCount );
			//checkVboConsistency( levelRenderData, levelSampleCount );
			levelRenderData.vertexBuffer.deactivate();
		}
	}


	//-------------------------------------------------------------------------------------------------


	bool CpuClipmapRenderer::checkVboConsistency( TerrainLevelRenderData& levelRenderData, int levelSampleCount )
	{
		if( !levelRenderData.vertexBuffer.isValid() )
		{
			return true;
		}
		std::cerr << "Checking Vbo consistency" << std::endl;

		const int vertexCount = levelRenderData.vertices.size();

		OpenGLTerrainVertex* vboContent = ( OpenGLTerrainVertex* )levelRenderData.vertexBuffer.lock( 0, sizeof( OpenGLTerrainVertex ) * vertexCount, BufferLockOption_ReadOnly );

		if( !vboContent )
		{
			std::cerr << "Could not lock the Vbo for reading!" << std::endl;
			return false;
		}

		const float epsilon = 0.001f;
		bool foundMismatch = false;

		for( int y = 0; y < levelSampleCount; ++y )
		{
			const int index = y * levelSampleCount;
			const OpenGLTerrainVertex* vertexPtr = &levelRenderData.vertices[ index ];
			const OpenGLTerrainVertex* vboVertexPtr = vboContent + index;

			for( int x = 0; x < levelSampleCount; ++x )
			{
				// compare vbo with our data:
				const OpenGLTerrainVertex& originalVertex = *vertexPtr++;
				const OpenGLTerrainVertex& vboVertex = *vboVertexPtr++;

				if( fabsf( originalVertex.pos[ 0 ] - vboVertex.pos[ 0 ] ) > epsilon ||
					fabsf( originalVertex.pos[ 1 ] - vboVertex.pos[ 1 ] ) > epsilon ||
					fabsf( originalVertex.pos[ 2 ] - vboVertex.pos[ 2 ] ) > epsilon ||
					fabsf( originalVertex.pos[ 3 ] - vboVertex.pos[ 3 ] ) > epsilon )
				{
					std::cerr << "Mismatch of VBO Buffer at Vertex (" << x << ", " << y << ")\n";
					foundMismatch = true;
				}
			}
		}
		levelRenderData.vertexBuffer.unlock();
		if( !foundMismatch )
		{
			std::cerr << "Vbo data is correct" << std::endl;
		}


		return true;
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::createBlockIndices( GeometryClipmapLevel& level, const Rectangle2i& blockRect, int levelSampleCount, IndexList& indices )
	{
		// todo: this needs rework..
		if( blockRect.isEmpty() )
		{
			return;
		}
		
		assert( blockRect.x0 >= 0 );
		assert( blockRect.y0 >= 0 );
		assert( blockRect.x1 > blockRect.x0 );
		assert( blockRect.y1 > blockRect.y0 );
		assert( blockRect.x0 < levelSampleCount );
		assert( blockRect.y0 < levelSampleCount );
		assert( blockRect.x1 < levelSampleCount );
		assert( blockRect.y1 < levelSampleCount );

		for( int y = blockRect.y0; y < blockRect.y1; ++y )
		{
			assert( y + 1 < levelSampleCount );
			const int row0 = ( level.blockOrigin[ 1 ] + y ) % levelSampleCount;
			const int row1 = ( level.blockOrigin[ 1 ] + y + 1 ) % levelSampleCount;

			for( int x = blockRect.x0; x < blockRect.x1; ++x )
			{
				assert( x + 1 < levelSampleCount );
				const int col0 = ( level.blockOrigin[ 0 ] + x ) % levelSampleCount;
				const int col1 = ( level.blockOrigin[ 0 ] + x + 1 ) % levelSampleCount;

				const int idx0 = row0 * levelSampleCount + col0;
				const int idx1 = row0 * levelSampleCount + col1;
				const int idx2 = row1 * levelSampleCount + col0;
				const int idx3 = row1 * levelSampleCount + col1;

				assert( idx0 < 65536 );
				assert( idx1 < 65536 );
				assert( idx2 < 65536 );
				assert( idx3 < 65536 );

				indices.push_back( idx0 );
				indices.push_back( idx2 );
				indices.push_back( idx1 );

				indices.push_back( idx1 );
				indices.push_back( idx2 );
				indices.push_back( idx3 );
			}
		}
	}


	//-------------------------------------------------------------------------------------------------


	//void CpuClipmapRenderer::drawBlockBoundingBox( const GeometryClipmapLevel& level, const Rectangle2i& blockRect )
	//{
	//	// calculate the bounding box of this block 
	//	BoundingBox3f bbox;

	//	const Pnt2f minWorldPos = geoClipmaps_->samplePosToWorldPos( level.blockPosToSamplePos( blockRect.getTopLeft() ) );
	//	const Pnt2f maxWorldPos = geoClipmaps_->samplePosToWorldPos( level.blockPosToSamplePos( blockRect.getBottomRight() ) );

	//	bbox.min.setValues( minWorldPos[ 0 ], level.heightmap.minSampleValue, minWorldPos[ 1 ] );
	//	bbox.max.setValues( maxWorldPos[ 0 ], level.heightmap.maxSampleValue, maxWorldPos[ 1 ] );

	//	// and draw it 
	//	drawBox( bbox.min, bbox.max );
	//}


	//-------------------------------------------------------------------------------------------------


	bool CpuClipmapRenderer::buildIndices( GeometryClipmapLevel& level, const GeometryClipmapLevel* finerLevel )
	{		
		// todo: speedup?! (insert optimal tristrips here..)
		// and frustum culling...		
		TerrainLevelRenderData& renderData = levels_[ level.index ];
		const int levelSampleCount = level.heightmap.size;

		if( !renderData.rebuildIndices )
		{
			return true;
		}
		renderData.rebuildIndices = false;

		renderData.indices.resize( 0 );

		const int levelSampleCoverage = level.getSampleCoverage();

		// we need the world extend in sample space for clipping:
		const Pnt2i worldSampleCount = geoClipmaps_->getWorldSampleCount();

		// check if this level is at least partly visible:
		if( level.sampleOrigin[ 0 ] >= worldSampleCount[ 0 ] - level.sampleSpacing ||
			level.sampleOrigin[ 1 ] >= worldSampleCount[ 1 ] - level.sampleSpacing ||
			level.sampleOrigin[ 0 ] + levelSampleCoverage < level.sampleSpacing ||
			level.sampleOrigin[ 1 ] + levelSampleCoverage < level.sampleSpacing )
		{
			// completely invisible:
			return false;
		}	

		// decision: 
		// either it is only one large or 8 small primitives:
		if( !finerLevel )
		{
			// ok: no finer level: only one large primitive
			// todo: possible speedup: frustum culling by dividing the level in smaller parts.. 
			Rectangle2i blockRect;

			blockRect.setBounds( 0, 0, levelSampleCount - 1, levelSampleCount - 1 );

			// clip rectangle to the world bounds:
			if( level.sampleOrigin[ 0 ] < 0 )
			{
				blockRect.x0 = -level.sampleOrigin[ 0 ] / level.sampleSpacing;
			}
			if( level.sampleOrigin[ 1 ] < 0 )
			{
				blockRect.y0 = -level.sampleOrigin[ 1 ] / level.sampleSpacing;
			}
			if( level.sampleOrigin[ 0 ] + levelSampleCoverage >= worldSampleCount[ 0 ] )
			{
				blockRect.x1 = ( worldSampleCount[ 0 ] - level.sampleOrigin[ 0 ] - 1 ) / level.sampleSpacing;
			}
			if( level.sampleOrigin[ 1 ] + levelSampleCoverage >= worldSampleCount[ 1 ] )
			{
				blockRect.y1 = ( worldSampleCount[ 1 ] - level.sampleOrigin[ 1 ] - 1 ) / level.sampleSpacing;
			}
			createBlockIndices( level, blockRect, levelSampleCount, renderData.indices );
		}
		else
		{
			// build 8 primitives for the level:
			// 0 1 2
			// 3   4 
			// 5 6 7

			// todo: beautify this code
			int colEnd[ 3 ];
			int rowEnd[ 3 ];

			int colEndSample[ 3 ];
			int rowEndSample[ 3 ];

			colEndSample[ 0 ] = clamp( finerLevel->sampleOrigin[ 0 ], 0, worldSampleCount[ 0 ] - 1 );
			colEndSample[ 1 ] = clamp( finerLevel->sampleOrigin[ 0 ] + finerLevel->getSampleCoverage(), 0, worldSampleCount[ 0 ] - 1 );
			colEndSample[ 2 ] = clamp( level.sampleOrigin[ 0 ] + levelSampleCoverage, 0, worldSampleCount[ 0 ] - 1 );

			// convert to block space:
			colEnd[ 0 ] = clamp( ( colEndSample[ 0 ] - level.sampleOrigin[ 0 ] ) / level.sampleSpacing, 0, levelSampleCount - 1 );
			colEnd[ 1 ] = clamp( ( colEndSample[ 1 ] - level.sampleOrigin[ 0 ] ) / level.sampleSpacing, 0, levelSampleCount - 1 );
			colEnd[ 2 ] = clamp( ( colEndSample[ 2 ] - level.sampleOrigin[ 0 ] ) / level.sampleSpacing, 0, levelSampleCount - 1 );

			rowEndSample[ 0 ] = clamp( finerLevel->sampleOrigin[ 1 ], 0, worldSampleCount[ 1 ] - 1 );
			rowEndSample[ 1 ] = clamp( finerLevel->sampleOrigin[ 1 ] + finerLevel->getSampleCoverage(), 0, worldSampleCount[ 1 ] - 1 );
			rowEndSample[ 2 ] = clamp( level.sampleOrigin[ 1 ] + levelSampleCoverage, 0, worldSampleCount[ 1 ] - 1 );

			// convert to block space:
			rowEnd[ 0 ] = clamp( ( rowEndSample[ 0 ] - level.sampleOrigin[ 1 ] ) / level.sampleSpacing, 0, levelSampleCount - 1 );
			rowEnd[ 1 ] = clamp( ( rowEndSample[ 1 ] - level.sampleOrigin[ 1 ] ) / level.sampleSpacing, 0, levelSampleCount - 1 );
			rowEnd[ 2 ] = clamp( ( rowEndSample[ 2 ] - level.sampleOrigin[ 1 ] ) / level.sampleSpacing, 0, levelSampleCount - 1 );

			// clip the row start:
			int rowStart = 0;

			if( level.sampleOrigin[ 1 ] < 0 )
			{
				rowStart = -level.sampleOrigin[ 1 ] / level.sampleSpacing;
			}
			
			Rectangle2i blockRect;

			for( int row = 0; row < 3; ++row )
			{
				//int rowStart = 0;
				if( row > 0 )
				{
					rowStart = rowEnd[ row - 1 ];
				}

				int colStart = 0;

				if( level.sampleOrigin[ 0 ] < 0 )
				{
					colStart = -level.sampleOrigin[ 0 ] / level.sampleSpacing;
				}

				for( int col = 0; col < 3; ++col )
				{
					if( row == 1 && col == 1 )
					{
						// this place is taken by the finer level
						continue;
					}

					if( col > 0 )
					{
						colStart = colEnd[ col - 1 ];
					}

					blockRect.setValues( colStart, rowStart, colEnd[ col ], rowEnd[ row ] );

					stats_.drawnBlockCount++;

					// create the indices:
					createBlockIndices( level, blockRect, levelSampleCount, renderData.indices );					
				}
			}
		}

		// return false if this contains no indices -> no need to render
		return renderData.indices.size() > 0;
	}


	//-------------------------------------------------------------------------------------------------


	void CpuClipmapRenderer::renderBlock( const GeometryClipmapLevel& level, const GeometryClipmapLevel* coarserLevel, const ClipmapRenderParameters& renderParameters, const Color3f& debugColor )
	{
		TerrainLevelRenderData& renderData = levels_[ level.index ];

		if( renderData.indices.size() <= 0 )
		{
			return;
		}


		// enable the texture:
		// todo: enable the texture of the next coarser level too, so we can blend between both
		if( renderData.texture != NullFC )
		{
			//renderData.texture->activate( renderParameters.renderAction );
		}
		Pnt2i transitionSize;

		//transitionSize[ 0 ] = std::min( ( levelSampleCount_ / 10 ) * level.sampleDistance[ 0 ], level.minTransitionSize[ 0 ] );
		//transitionSize[ 1 ] = std::min( ( levelSampleCount_ / 10 ) * level.sampleDistance[ 1 ], level.minTransitionSize[ 1 ] );

		const int levelSampleSpacing = level.sampleSpacing;

		transitionSize[ 0 ] = levelSampleSpacing * level.heightmap.size / 10;
		transitionSize[ 1 ] = levelSampleSpacing * level.heightmap.size / 10;

		// min and max are the first,last point inside the renderregion:
		Pnt2f activeRegionMin = Pnt2f( level.sampleOrigin );
		Pnt2f activeRegionMax = Pnt2f( componentAdd( level.sampleOrigin, Pnt2i( level.getSampleCoverage(), level.getSampleCoverage() ) ) );

		Pnt2f worldTransitionSize = Pnt2f( transitionSize );

		//Point2f activeRegionCenter = samplePosToWorldPos( level.outerRenderBounds.getTopLeft() + level.outerRenderBounds.getSize() / 2 );		
		//Point2f localViewerPos( worldViewerPosition_[ 0 ], worldViewerPosition_.z );
		//Pnt2i viewerSamplePos = worldPosToSamplePos( localViewerPos );
		//Point2f baseLocalViewerPos = samplePosToWorldPos( viewerSamplePos );

		//localViewerPos -= baseLocalViewerPos;

		//beginEditCP( terrainShader_ );
		terrainShader_.setUniform( "transitionWidth", Vec2f( worldTransitionSize ) );
		terrainShader_.setUniform( "activeRegionMin", Vec2f( activeRegionMin ) );
		terrainShader_.setUniform( "activeRegionMax", Vec2f( activeRegionMax ) );
		//terrainShader_->setUniform( "activeRegionCenter", activeRegionCenter );		
		terrainShader_.setUniform( "localViewerPos", Vec3f( viewerPosition_ ) );
		terrainShader_.setUniform( "baseColor0", colorToVector( debugColor ) );
		//endEditCP( terrainShader_ );

		if( coarserLevel )
		{
			terrainShader_.setUniform( "baseColor1", colorToVector( getDebugColor( coarserLevel->index ) ) );
		}
		else
		{
			terrainShader_.setUniform( "baseColor1", colorToVector( debugColor ) );
		}

		//terrainShader_->updateParameters( drawAction->getWindow(), terrainShader_->getParameters() );

		if( renderParameters.useVboExtension && useVertexBufferObjects_ )
		{
			renderData.vertexBuffer.activate();

			char* base = 0;

			glVertexPointer( 4, GL_FLOAT, sizeof( OpenGLTerrainVertex ), base );
			glEnableClientState( GL_VERTEX_ARRAY );

			glTexCoordPointer( 2, GL_FLOAT, sizeof( OpenGLTerrainVertex ), base + sizeof( Pnt4f ) );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glDrawElements( GL_TRIANGLES, ( GLsizei ) renderData.indices.size(), GL_UNSIGNED_SHORT, &renderData.indices[ 0 ] );

			renderData.vertexBuffer.deactivate();
		}
		else
		{
			glVertexPointer( 4, GL_FLOAT, sizeof( OpenGLTerrainVertex ), &renderData.vertices[ 0 ].pos );
			glEnableClientState( GL_VERTEX_ARRAY );

			glTexCoordPointer( 2, GL_FLOAT, sizeof( OpenGLTerrainVertex ), &renderData.vertices[ 0 ].uv );
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );

			glDrawElements( GL_TRIANGLES, ( GLsizei ) renderData.indices.size(), GL_UNSIGNED_SHORT, &renderData.indices[ 0 ] );
		}

		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		glDisableClientState( GL_NORMAL_ARRAY );
		glDisableClientState( GL_VERTEX_ARRAY );

		stats_.drawnBlockCount++;
		stats_.drawnTriangleCount		+= renderData.indices.size() / 3;
		stats_.transformedVertexCount	+= renderData.vertices.size();

		if( renderData.texture != NullFC )
		{
			//renderData.texture->deactivate( renderParameters.renderAction );
		}
	}


	//-------------------------------------------------------------------------------------------------	


	//void CpuClipmapRenderer::drawVertices( const GeometryClipmapLevel& level, const Color3f& color ) const
	//{
	//	const TerrainLevelRenderData& renderData = levels_[ level.index ];

	//	const int levelSampleCount = level.heightmap.size;
	//	
	//	glBegin( GL_POINTS );
	//	for( int y = 0; y < levelSampleCount; ++y )
	//	{
	//		for( int x = 0; x < levelSampleCount; ++x )
	//		{
	//			Pnt2i samplePos = level.blockPosToSamplePos( Pnt2i( x, y ) );
	//			Pnt2f sampleWorldPos = geoClipmaps_->samplePosToWorldPos( samplePos );

	//			const OpenGLTerrainVertex& vertex = renderData.vertices[ y * levelSampleCount + x ];

	//			const float sampleValue = level.heightmap.getSample( x, y );

	//			glVertex3f( sampleWorldPos[ 0 ], sampleValue, sampleWorldPos[ 1 ] );
	//		}
	//	}		
	//	glEnd();
	//}


	//-------------------------------------------------------------------------------------------------	


	void CpuClipmapRenderer::drawBlendLines( const GeometryClipmapLevel& level, const Color3f& color, const WorldTransformation& worldTransform ) const
	{
		const TerrainLevelRenderData& renderData = levels_[ level.index ];

		const int levelSampleCount = level.heightmap.size;

		glBegin( GL_LINES );
		for( int y = 0; y < levelSampleCount; ++y )
		{
			for( int x = 0; x < levelSampleCount; ++x )
			{
				Pnt2i samplePos = level.blockPosToSamplePos( Pnt2i( x, y ) );
				Pnt2f sampleWorldPos = worldTransform.sampleToWorldPos( samplePos );

				const OpenGLTerrainVertex& vertex = renderData.vertices[ y * levelSampleCount + x ];

				const float sampleValue = level.heightmap.getSample( x, y );

				if( ( fabsf( sampleWorldPos[ 0 ] - vertex.pos[ 0 ] ) > 0.1f ) ||
					( fabsf( sampleWorldPos[ 1 ] - vertex.pos[ 2 ] ) > 0.1f ) ||
					( fabsf( sampleValue - vertex.pos[ 1 ] ) > 0.1f ) )
				{
					int i = 43;
				}

				glVertex3f( vertex.pos[ 0 ], vertex.pos[ 1 ], vertex.pos[ 2 ] );
				glVertex3f( vertex.pos[ 0 ], vertex.pos[ 3 ], vertex.pos[ 2 ] );
			}
		}		
		glEnd();
	}


	//-------------------------------------------------------------------------------------------------	

}

//-------------------------------------------------------------------------------------------------

