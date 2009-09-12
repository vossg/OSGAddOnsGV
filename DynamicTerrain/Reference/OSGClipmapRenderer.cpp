//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:20:35
//
//	Description			:		OSGClipmapRenderer.cpp
//
//-------------------------------------------------------------------------------------------------

#include "PrecompiledHeader.h"
#include "OSGClipmapRenderer.h"
#include "OSGGeometryClipmaps.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------

	
	WorldTransformation::WorldTransformation()
	{
		sampleDistance	= 1.0f;
		heightOffset	= 0.0f;
		heightScale		= 1.0f;
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2f WorldTransformation::sampleToWorldPos( const Pnt2i& pos ) const
	{
		return Pnt2f( float( pos[ 0 ] ) * sampleDistance + offset[ 0 ], float( pos[ 1 ] ) * sampleDistance + offset[ 1 ] );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2i WorldTransformation::worldToSamplePos( const Pnt2f& pos ) const
	{
		return Pnt2i( int( ( pos[ 0 ] - offset[ 0 ] ) / sampleDistance ), int( ( pos[ 1 ] - offset[ 1 ] ) / sampleDistance ) );
	}


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


	ClipmapRenderParameters::ClipmapRenderParameters()
	{
		window					= 0;
		showBoundingBoxes		= false;
		enableFrustumCulling	= false;
		useVboExtension			= true;
		showTransitionRegions	= false;
		renderAction			= 0;
		globalTexture			= NullFC;		
	}


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


	ClipmapRenderStatistics::ClipmapRenderStatistics()
	{
		clear();
	}


	//-------------------------------------------------------------------------------------------------


	void ClipmapRenderStatistics::clear()
	{
		drawnLevelCount			= 0;
		drawnBlockCount			= 0;
		drawnTriangleCount		= 0;
		culledBlockCount		= 0;
		transformedVertexCount	= 0;
	}


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


	bool ClipmapRenderer::initialize( GeometryClipmaps* geoClipmaps )
	{
		if( !geoClipmaps )
		{
			return false;
		}
		shutdown();

		geoClipmaps_ = geoClipmaps;

		return onInitialize( geoClipmaps );
	}


	//-------------------------------------------------------------------------------------------------


	void ClipmapRenderer::shutdown()
	{
		onDestroyGpuResources();
		onShutdown();
		window_ = 0;
		geoClipmaps_ = 0;
	}


	//-------------------------------------------------------------------------------------------------


	void ClipmapRenderer::updateRenderData( const Pnt3f& viewPosition )
	{
		onUpdateRenderData( viewPosition );
	}


	//-------------------------------------------------------------------------------------------------


	void ClipmapRenderer::render( const ClipmapRenderParameters& renderParameters )
	{
		// todo: save/restore all used opengl states
		stats_.clear();

		if( !renderParameters.window || !geoClipmaps_ )
		{
			return;
		}

		// if the window has changed: reload the shader (and recreate the gpu buffers)
		if( window_ != renderParameters.window )
		{			
			if( window_ )
			{
				// destroy all gpu stuff:
				onDestroyGpuResources();				
			}

			// load shader from glsl file:			
			window_ = renderParameters.window;

			onCreateGpuResources();			
		}
		
		// and now: render the terrain:
		onRender( renderParameters );
	}


	//-------------------------------------------------------------------------------------------------


	void ClipmapRenderer::rebuildIndices( GeometryClipmapLevel& level )
	{
		onRebuildIndices( level );
	}


	//-------------------------------------------------------------------------------------------------


	void ClipmapRenderer::buildVertices( GeometryClipmapLevel& level, const GeometryClipmapLevel* coarserLevel, const Rectangle2i& sampleRect )
	{
		onBuildVertices( level, coarserLevel, sampleRect );
	}


	//-------------------------------------------------------------------------------------------------


	void ClipmapRenderer::reloadShader()
	{
		onReloadShader();
	}


	//-------------------------------------------------------------------------------------------------


	const ClipmapRenderStatistics& ClipmapRenderer::getLastFrameStatistics() const
	{
		return stats_;
	}


	//-------------------------------------------------------------------------------------------------


	ClipmapRenderer::ClipmapRenderer()
	{
		window_ = 0;
		viewerPosition_.setValues( 0, 0, 0 );
		viewerSamplePos_.setValues( 0, 0 );
	}


	//-------------------------------------------------------------------------------------------------


	ClipmapRenderer::~ClipmapRenderer()
	{
	}


	//-------------------------------------------------------------------------------------------------


#	define countof(A) (sizeof(A)/sizeof(A[0]))

	Color3f ClipmapRenderer::getDebugColor( int index ) const
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
#	undef countof
	

	//-------------------------------------------------------------------------------------------------


	void ClipmapRenderer::drawSampleRectangle( const Rectangle2i& rect, const Color3f& color, const WorldTransformation& worldTrans ) const
	{
		Pnt2f from = worldTrans.sampleToWorldPos( rect.getTopLeft() );
		Pnt2f to = worldTrans.sampleToWorldPos( rect.getBottomRight() - Pnt2i( 1, 1 ) );

		glColor3f( color[ 0 ], color[ 1 ], color[ 2 ] );

		glBegin( GL_LINE_STRIP );

		glVertex3f( from[ 0 ], 0, from[ 1 ] );
		glVertex3f(   to[ 0 ], 0, from[ 1 ] );
		glVertex3f(   to[ 0 ], 0,   to[ 1 ] );
		glVertex3f( from[ 0 ], 0,   to[ 1 ] );
		glVertex3f( from[ 0 ], 0, from[ 1 ] );

		glEnd();	
	}


	//-------------------------------------------------------------------------------------------------


	void ClipmapRenderer::drawSamples( const GeometryClipmapLevel& level, const Color3f& color, const WorldTransformation& worldTrans ) const
	{
		const int levelSampleCount = level.heightmap.size;
		
		for( int y = 0; y < levelSampleCount; ++y )
		{
			for( int x = 0; x < levelSampleCount; ++x )
			{
				// draw the sample:
				if( x == 0 && y == 0 )
				{
					glColor3f( 1, 0, 0 );
				}
				else
				{
					glColor3f( 0, 1, 0 );					
				}

				Pnt2i samplePos = level.blockPosToSamplePos( Pnt2i( x, y ) );
				Pnt2f sampleWorldPos = worldTrans.sampleToWorldPos( samplePos );

				const float sampleValue = level.heightmap.getSample( x, y );

				glBegin( GL_POINTS );
				glVertex3f( sampleWorldPos[ 0 ], sampleValue, sampleWorldPos[ 1 ] );
				glEnd();
			}
		}		
	}


	//-------------------------------------------------------------------------------------------------	



}

//-------------------------------------------------------------------------------------------------

