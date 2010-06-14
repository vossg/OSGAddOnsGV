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
#include <OSGConfig.h>
#include <OSGContribDef.h>

#include <OSGImage.h>
#include "OSGTextureDataSource.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	class OSG_CONTRIBLIB_DLLMAPPING ImageTextureDataSource : public TextureDataSource
	{
	public:		
		explicit					ImageTextureDataSource( ImagePtr image = NullFC );
		virtual						~ImageTextureDataSource();

		void						setImage( ImagePtr image );

		virtual const Pnt2i&		getSampleCount() const;
		virtual	void				fillTextureData( GeometryClipmapLevel& level, const Rectangle2i& targetRect ) const;

	private:
		ImagePtr					image_;
		Pnt2i						size_;
	};
	
	
	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef OSG_TEXTURE_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------

