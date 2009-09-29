//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		OSGHeightDataSource.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef OSG_HEIGHT_DATA_SOURCE_INCLUDED
#define OSG_HEIGHT_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------

#include "OSGImage.h"
#include "OSGHeightDataSource.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	struct GeometryClipmapLevel;

	class ImageHeightDataSource : public HeightDataSource
	{
	public:		
		explicit					ImageHeightDataSource( ImagePtr image = NullFC );
		virtual						~ImageHeightDataSource();

		void						setImage( ImagePtr image );

		virtual float				getMinHeight() const;
		virtual float				getMaxHeight() const;

		virtual const Pnt2i&		getSampleCount() const;
		virtual	void				fillHeightData( GeometryClipmapLevel& level, const Rectangle2i& targetRect ) const;

	private:
		ImagePtr					image_;
		Pnt2i						size_;
		float						heightScale_;
		float						hegihtOffset_;

		float						getHeightSample( const Pnt2i& samplePos ) const;
		//void						fillHeightData16dbpp( GeometryClipmapLevel& level, const HeightTargetBufferDescription& target, const Pnt2i& sampleOrigin, int sampleDistance ) const;
	};
	
	
	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef OSG_HEIGHT_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------

