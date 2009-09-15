//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:20:35
//
//	Description			:		ImageHeightDataSource.cpp
//
//-------------------------------------------------------------------------------------------------

//#include "PrecompiledHeader.h"
#include "OSGImageHeightDataSource.h"
#include "OSGGeometryClipmaps.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	ImageHeightDataSource::ImageHeightDataSource( Image *image )
	{
		setImage( image );
		heightScale_ = 1.0f;
	}


	//-------------------------------------------------------------------------------------------------


	ImageHeightDataSource::~ImageHeightDataSource()
	{
	}


	//-------------------------------------------------------------------------------------------------

	
	void ImageHeightDataSource::setImage( Image *image )
	{
		image_ = image;

		if( image_ != NULL )
		{
			size_.setValues( image_->getWidth(), image_->getHeight() );			
		}
		else
		{
			size_.setValues( 0, 0 );
		}
	}

	//-------------------------------------------------------------------------------------------------


	float ImageHeightDataSource::getMinHeight() const
	{
		return 0.0f;
	}


	//-------------------------------------------------------------------------------------------------


	float ImageHeightDataSource::getMaxHeight() const
	{
		return 1.0f;
	}


	//-------------------------------------------------------------------------------------------------


	const Pnt2i& ImageHeightDataSource::getSampleCount() const
	{
		return size_;
	}


	//-------------------------------------------------------------------------------------------------


	void ImageHeightDataSource::fillHeightData( GeometryClipmapLevel& level, const Rectangle2i& targetRect ) const
	{
		// check validity of the input data:
		assert( targetRect.getWidth() > 0 );
		assert( targetRect.getHeight() > 0 );

		// todo: clip data to the existing data: (saves per pixel clipping)		

		// iterate over the target pixels and fill them with the image height data:
		Pnt2i samplePos;

		for( int y = targetRect.y0; y < targetRect.y1; ++y )
		{
			float* targetPtr = &level.heightmap.samples[ y * level.heightmap.size ];

			for( int x = targetRect.x0; x < targetRect.x1; ++x )
			{
				// todo: make this incremental:
				samplePos = level.blockPosToSamplePos( Pnt2i( x, y ) );

				targetPtr[ x ] = heightScale_ * getHeightSample( samplePos );					
			}			
		}
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
		int index = ( image_->getWidth() * samplePos[ 1 ] + samplePos[ 0 ] );
	
		// todo: move this switch up higher.. (possibly into a template accessor??)
		switch( image_->getDataType() )
		{
		default:
		case Image::OSG_INVALID_IMAGEDATATYPE:
		case Image::OSG_UINT8_IMAGEDATA:
			return ( static_cast<const UInt8*>(image_->getData() ))[ index ] / Real32( TypeTraits< UInt8 >::getMax() );

		case Image::OSG_UINT16_IMAGEDATA:
			return ( reinterpret_cast<const UInt16*>(image_->getData()))[ index ] / Real32( TypeTraits< UInt16 >::getMax() );

		case Image::OSG_UINT32_IMAGEDATA:
			return ( reinterpret_cast<const UInt32*>(image_->getData() ))[ index ] / Real32( TypeTraits< UInt32 >::getMax() );

		case Image::OSG_FLOAT16_IMAGEDATA:
			return ( reinterpret_cast<const Real16* >(image_->getData()) )[ index ];

		case Image::OSG_FLOAT32_IMAGEDATA:
			return ( reinterpret_cast<const Real32* >(image_->getData()) )[ index ];
		};
		return 0.0f;
	}

	
	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

