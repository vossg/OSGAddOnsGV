//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		OSGTextureDataSource.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef TEXTURE_DATA_SOURCE_INCLUDED
#define TEXTURE_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGAddOnsConfig.h>
#include <OSGContribDef.h>

#include "OSGRectangle.h"
#include "OSGTerrainTypes.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	struct GeometryClipmapLevel;

	//-------------------------------------------------------------------------------------------------

	class OSG_CONTRIBLIB_DLLMAPPING TextureDataSource
	{
	public:		
		virtual						~TextureDataSource() = 0;

		virtual const Pnt2i&		getSampleCount() const = 0;
		virtual	void				fillTextureData( GeometryClipmapLevel& level, const Rectangle2i& targetRect ) const = 0;

		// todo: get residuals instead of absolute height samples

		// todo: function to access normals
	};

	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef TEXTURE_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------

