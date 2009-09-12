//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		OSGHeightDatasSource.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef HEIGHT_DATA_SOURCE_INCLUDED
#define HEIGHT_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------

#include "OSGRectangle.h"
#include "OSGTerrainTypes.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{
	//-------------------------------------------------------------------------------------------------

	struct GeometryClipmapLevel;

	//-------------------------------------------------------------------------------------------------

	class HeightDataSource
	{
	public:		
		virtual						~HeightDataSource() = 0;

		virtual const Pnt2i&		getSampleCount() const = 0;
		virtual float				getMinHeight() const = 0;
		virtual float				getMaxHeight() const = 0;

		virtual	void				fillHeightData( GeometryClipmapLevel& level, const Rectangle2i& targetRect ) const = 0;

		// todo: get residuals instead of absolute height samples
	};


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef HEIGHT_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------
