//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:20:35
//
//	Description			:		ImageHeightDataSource.cpp
//
//-------------------------------------------------------------------------------------------------

//#include "PrecompiledHeader.h"

#if 0 

#include "OSGImageHeightDataSource.h"
#include "GeometryClipmaps.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	ImageHeightDataSource::ImageHeightDataSource( ImagePtr image )
	{
		setImage( image );
		heightScale_ = 1.0f;
	}


	//-------------------------------------------------------------------------------------------------


	ImageHeightDataSource::~ImageHeightDataSource()
	{
	}


	//-------------------------------------------------------------------------------------------------

	
	void ImageHeightDataSource::setImage( ImagePtr image )
	{
		image_ = image;

		if( image_ != NullFC )
		{
			size_.setValues( image_->getWidth(), image_->getHeight() );			
		}
		else
		{
			size_.setValues( 0, 0 );
		}
	}


	//-------------------------------------------------------------------------------------------------


	void ImageHeightDataSource::setHeightScale( float scale )
	{
		heightScale_ = scale;
	}

	
	//-------------------------------------------------------------------------------------------------


	float ImageHeightDataSource::getHeightScale() const
	{
		return heightScale_;
	}


	//-------------------------------------------------------------------------------------------------


	const Pnt2i& ImageHeightDataSource::getSampleCount() const
	{
		return size_;
	}


	//-------------------------------------------------------------------------------------------------


	void ImageHeightDataSource::fillHeightData( GeometryClipmapLevel& level, const HeightTargetBufferDescription& target, const Pnt2i& sampleOrigin, int sampleDistance ) const
	{
		// check validity of the input data:
		assert( target.format == HeightDataFormat_Real32 || target.format == HeightDataFormat_Int16 );
		assert( target.targetPtr );
		assert( target.targetStride > 0 );
		assert( target.targetRect.getWidth() > 0 );
		assert( target.targetRect.getHeight() > 0 );

		// todo: clip data to the existing data: (saves per pixel clipping)		

		// iterate over the target pixels and fill them with the image height data:
		Pnt2i samplePos;

		if( target.format == HeightDataFormat_Real32 ) 
		{
			for( int y = target.targetRect.y0; y < target.targetRect.y1; ++y )
			{
				float* targetPtr = ( float* )( target.targetPtr + target.targetStride * y );

				for( int x = target.targetRect.x0; x < target.targetRect.x1; ++x )
				{
					// todo: make this incremental:
					samplePos = level.blockPosToSamplePos( Pnt2i( x, y ) );

					targetPtr[ x ] = heightScale_ * getHeightSample( samplePos );					
				}			
			}
		}
		else if( target.format == HeightDataFormat_Int16 )
		{
			// todo..
		}
	}


	//-------------------------------------------------------------------------------------------------


	void ImageHeightDataSource::fillHeightData16dbpp( GeometryClipmapLevel& level, const HeightTargetBufferDescription& target, const Pnt2i& sampleOrigin, int sampleDistance ) const
	{

	}


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


	float ImageHeightDataSource::getHeightSample( const Pnt2i& samplePos ) const
	{
		// todo: replace this with a higher level clip:
		if( samplePos[ 0 ] < 0 || samplePos[ 0 ] >= size_[ 0 ] || 
			samplePos[ 1 ] < 0 || samplePos[ 1 ] >= size_[ 1 ] )
		{
			return 0.0f;
		}

		// todo: replace this calulation with incremental addition
		int index = image_->getBpp() * ( image_->getWidth() * samplePos[ 1 ] + samplePos[ 0 ] );
	
		// todo: move this switch up higher..
		switch( image_->getDataType() )
		{
		default:
		case Image::OSG_INVALID_IMAGEDATATYPE:
		case Image::OSG_UINT8_IMAGEDATA:
			return ( ( UInt8* )image_->getData() )[ index ] / Real32( TypeTraits< UInt8 >::getMax() );

		case Image::OSG_UINT16_IMAGEDATA:
			return ( ( UInt16* )image_->getData())[ index ] / Real32( TypeTraits< UInt16 >::getMax() );

		case Image::OSG_UINT32_IMAGEDATA:
			return ( ( UInt32* )image_->getData() )[ index ] / Real32( TypeTraits< UInt32 >::getMax() );

		case Image::OSG_FLOAT16_IMAGEDATA:
			return ( ( Real16* )image_->getData() )[ index ];

		case Image::OSG_FLOAT32_IMAGEDATA:
			return ( ( Real32* )image_->getData() )[ index ];
		};
		return 0.0f;
	}

	
	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // 0
