//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		GeometryClipmaps.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef GEOMETRY_CLIPMAPS_INCLUDED
#define GEOMETRY_CLIPMAPS_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGAddOnsConfig.h>
#include <OSGContribDef.h>

#include <OSGBaseTypes.h>
#include "OSGTerrainTypes.h"
#include "OSGCpuClipmapRenderer.h"
#include "OSGHeightmap.h"
#include "OSGGeometryClipmapLevel.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	class HeightDataSource;
	class TextureDataSource;

	//-------------------------------------------------------------------------------------------------
	

	class OSG_CONTRIBLIB_DLLMAPPING GeometryClipmaps
	{
	public:
		GeometryClipmaps();
		~GeometryClipmaps();

		bool						initialize( int levelSize, HeightDataSource* heightSource, TextureDataSource* textureSource );
		void						shutdown();

		bool						isValid() const;

		void						update( const Pnt3f& viewerPosition );
		void						render( const ClipmapRenderParameters& renderParams, ClipmapRenderStatistics& renderStats );

		void						adjustVolume( Volume& volume, float heightDataScale );

		// interaction with the renderer:
		void						setCpuVertexProgramText( const std::string& text );
		void						setCpuFragmentProgramText( const std::string& text );

		// Samplecoord <-> Worldcoord
		Pnt2i						getWorldSampleCount() const;

		//Pnt2f						samplePosToWorldPos( const Pnt2i& samplePos ) const;
		//Pnt2i						worldPosToSamplePos( const Pnt2f& worldPos ) const;

		// ray intersection:
		bool						findFirstIntersection( const Line& line, Real32& t, Vec3f& normal );

		int							getLevelCount() const;
		GeometryClipmapLevel&		getLevel( int index );

	protected:
		int							updateLevel( GeometryClipmapLevel& level, GeometryClipmapLevel* parentLevel, const Pnt2i& newOrigin );
		void						checkLevelBlock( GeometryClipmapLevel& level, const Rectangle2i& targetRect );
		int							updateBigLevelBlock( GeometryClipmapLevel& level, const GeometryClipmapLevel* parentLevel, const Rectangle2i& targetRect );
		int							updateLevelBlock( GeometryClipmapLevel& level, const GeometryClipmapLevel* parentLevel, const Rectangle2i& targetRect );

		typedef std::vector< GeometryClipmapLevel > ClipmapLevelList;

		HeightDataSource*			heightSource_;
		TextureDataSource*			textureSource_;
		CpuClipmapRenderer			cpuRenderer_;
		//GpuGeoClipmapRendere		gpuRenderer_;
		int							sampleUpdateBudget_;
		bool						notFinishedLastFrame_;
		ClipmapLevelList			levels_;
	};


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef GEOMETRY_CLIPMAPS_INCLUDED

//-------------------------------------------------------------------------------------------------

