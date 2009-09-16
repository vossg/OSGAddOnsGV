//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:20:35
//
//	Description			:		SinusHeightDataSource.cpp
//
//-------------------------------------------------------------------------------------------------

//#include "PrecompiledHeader.h"
#include "OSGSinusHeightDataSource.h"
#include "OSGGeometryClipmaps.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	SinusHeightDataSource::SinusHeightDataSource( const Pnt2i& size )
	{
		heightScale_ = 10.0f;
		frequency_ = 0.1f;
		size_ = size;
	}


	//-------------------------------------------------------------------------------------------------


	SinusHeightDataSource::~SinusHeightDataSource()
	{
	}


	//-------------------------------------------------------------------------------------------------


	void SinusHeightDataSource::setFrequency( float freq )
	{
		frequency_ = freq;
	}


	//-------------------------------------------------------------------------------------------------


	float SinusHeightDataSource::getFrequency() const
	{
		return frequency_;
	}


	//-------------------------------------------------------------------------------------------------


	void SinusHeightDataSource::setHeightScale( float scale )
	{
		assert( scale >= FLT_MIN );
		heightScale_ = scale;
	}

	
	//-------------------------------------------------------------------------------------------------


	float SinusHeightDataSource::getHeightScale() const
	{
		return heightScale_;
	}


	//-------------------------------------------------------------------------------------------------


	const Pnt2i& SinusHeightDataSource::getSampleCount() const
	{
		return size_;
	}


	//-------------------------------------------------------------------------------------------------


	void SinusHeightDataSource::fillHeightData( GeometryClipmapLevel& level, const Rectangle2i& targetRect ) const
	{
		// check validity of the input data:

#if 0
		const int levelSampleCount = level.heightmap.size;
		const int blockOriginX = level.blockOrigin[ 0 ];
		const int blockOriginY = level.blockOrigin[ 1 ];
#endif

		// todo: clip data to the existing data: (saves per pixel clipping)

		// iterate over the target pixels and fill them with the image height data:
		Pnt2i samplePos;
		//SLOG << ". SampleOrigin= " << sampleOrigin << "\n";

		for( int y = targetRect.y0; y < targetRect.y1; ++y )
		{
			float* targetPtr = &level.heightmap.samples[ y * level.heightmap.size ];

			for( int x = targetRect.x0; x < targetRect.x1; ++x )
			{
				// todo: make this incremental:
				samplePos = level.blockPosToSamplePos( Pnt2i( x, y ) );

				targetPtr[ x ] = heightScale_ * getHeightSample( samplePos );

				//SLOG << ". Sample [" << samplePos[ 0 ] << "," << samplePos[ 1 ] << "] updated (BlockPos=" << x << "," << y << ")" << "\n";
			}
		}
	}


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


	float SinusHeightDataSource::getHeightSample( const Pnt2i& samplePos ) const
	{
		// todo: replace this with a higher level clip:
		if( samplePos[ 0 ] < 0 || samplePos[ 1 ] < 0 || samplePos[ 0 ] >= size_[ 0 ] || samplePos[ 1 ] >= size_[ 1 ] )
		{
			return 0.0f;
		}

		const float posX = float(samplePos[ 0 ]) * frequency_;
		const float posY = float(samplePos[ 1 ]) * frequency_;

		return 0.1f * posX;
		return cosf( sinf( posX ) ) * sinf( posY );
	}

	
	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

