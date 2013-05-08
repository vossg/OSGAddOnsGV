//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		GeometryClipmapLevel.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef GEOMETRY_CLIPMAP_LEVEL_INCLUDED
#define GEOMETRY_CLIPMAP_LEVEL_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGAddOnsConfig.h>
#include <OSGContribDef.h>

#include <OSGBaseTypes.h>
#include <OSGLine.h>
#include <OSGImage.h>
#include "OSGTerrainTypes.h"
#include "OSGHeightmap.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	class HeightDataSource;
	class TextureDataSource;

	//-------------------------------------------------------------------------------------------------


	struct OSG_CONTRIBLIB_DLLMAPPING GeometryClipmapLevel
	{
		GeometryClipmapLevel();

		Pnt2i						blockPosToSamplePos( const Pnt2i& blockPos ) const;
		Pnt2i						samplePosToBlockPos( const Pnt2i& samplePos ) const;

		Rectangle2i					getCoveredSampleRect() const;
		int							getSampleCoverage() const;

		int							updateBlock( HeightDataSource* dataSource, TextureDataSource* textureSource, const Rectangle2i& targetRect );

		bool						findFirstIntersection( const Line& line, Real32& t, Vec3f& normal );

		float						getBlockSample( const Pnt2i& blockPos ) const;

		bool						containsSample( const Pnt2i& samplePos ) const;

		int							index;
		bool						isActive;
		Pnt2i						sampleOrigin;
		Pnt2i						blockOrigin;
		//Rectangle2i					clipRect;			// maximal sample region
		int							sampleSpacing;
		Heightmap					heightmap;
		bool						isSampleDataDirty;
		ImagePtr					textureData;
	};


	//-------------------------------------------------------------------------------------------------
	

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef GEOMETRY_CLIPMAPS_INCLUDED

//-------------------------------------------------------------------------------------------------

