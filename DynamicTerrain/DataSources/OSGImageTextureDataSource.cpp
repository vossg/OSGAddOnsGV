//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:20:35
//
//	Description			:		ImageTextureDataSource.cpp
//
//-------------------------------------------------------------------------------------------------

//#include "PrecompiledHeader.h"
#include "OSGImageTextureDataSource.h"
//#include "OSGDynamicTerrainLevel.h"
#include "OSGGeometryClipmapLevel.h"
#include <OSGImageFunctions.h>

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	ImageTextureDataSource::ImageTextureDataSource( Image *image ) :
        image_(),
        size_ ()
	{
		setImage( image );
	}


	//-------------------------------------------------------------------------------------------------


	ImageTextureDataSource::~ImageTextureDataSource()
	{
	}


	//-------------------------------------------------------------------------------------------------

	
	void ImageTextureDataSource::setImage( Image *image )
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


	const Pnt2i& ImageTextureDataSource::getSampleCount() const
	{
		return size_;
	}


	//-------------------------------------------------------------------------------------------------


	void ImageTextureDataSource::fillTextureData( GeometryClipmapLevel& level, const Rectangle2i& targetRect ) const
	{
		if( level.textureData->getPixelFormat() != Image::OSG_RGBA_PF )
		{
			return;
		}

		// remap block coordinates to image coordinates
		// the last block element maps to the last texel:
		const float factorX = float( level.textureData->getWidth() - 1 ) / float( level.heightmap.size - 1 );
		const float factorY = float( level.textureData->getHeight() - 1 ) / float( level.heightmap.size - 1 );

		Rectangle2i textureRect;

		textureRect._x0 = int( float( targetRect._x0 ) * factorX );
		textureRect._y0 = int( float( targetRect._y0 ) * factorY );
		textureRect._x1 = int( float( targetRect._x1 - 1 ) * factorX ) + 1;
		textureRect._y1 = int( float( targetRect._y1 - 1 ) * factorY ) + 1;

		if( textureRect.isEmpty() )
		{
			// nothing to do:
			return;
		}

		// check validity of the input data:
		UInt8* imageData = level.textureData->editData();
		int imageStride = 4 * level.textureData->getWidth();

		if( !imageData )
		{
			FFATAL(("ImageTextureDataSource::fillTextureData(): Could not get image data pointer\n"));
			return;
		}
		
		for( int y = textureRect._y0; y < textureRect._y1; ++y )
		{
			UInt8* dataPtr = imageData + y * imageStride + textureRect._x0;

			for( int x = textureRect._x0; x < textureRect._x1; ++x )
			{
				// get the nearest block position of this texel:
				// todo: make this incremental
				Pnt2i blockPos( int( float( x ) / factorX ), int( float( y ) / factorY ) );

				float height = level.heightmap.getSample( blockPos[ 0 ], blockPos[ 1 ] );

				// R,G,B,A:
				*dataPtr++ = int( height * 100.0f ) % 256;
				*dataPtr++ = int( height * 100.0f ) % 256;
				*dataPtr++ = int( height * 100.0f ) % 256;
				*dataPtr++ = 0;
			}
		}
	}


	//-------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------


}

//-------------------------------------------------------------------------------------------------

