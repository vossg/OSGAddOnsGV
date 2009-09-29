//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		OSGTextureDataSource.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef OSG_TEXTURE_DATA_SOURCE_INCLUDED
#define OSG_TEXTURE_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------

#include "OSGImage.h"
#include "OSGTextureDataSource.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	class ImageTextureDataSource : public TextureDataSource
	{
	public:		
		explicit					ImageTextureDataSource(Image *image = NULL);
		virtual						~ImageTextureDataSource();

		void						setImage(Image *image);

		virtual const Pnt2i&		getSampleCount() const;
		virtual	void				fillTextureData( GeometryClipmapLevel& level, const Rectangle2i& targetRect ) const;

	private:
		ImageUnrecPtr				image_;
		Pnt2i						size_;
	};
	
	
	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef OSG_TEXTURE_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------

