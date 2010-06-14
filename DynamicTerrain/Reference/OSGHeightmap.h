//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		Heightmap.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef HEIGHTMAP_INCLUDED
#define HEIGHTMAP_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGConfig.h>
#include <OSGContribDef.h>

#include "OSGTerrainTypes.h"
#include <vector>
#include <OSGLine.h>

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	class OSG_CONTRIBLIB_DLLMAPPING Heightmap
	{
	public:		
		Heightmap();

		void						initialize( int size );

		inline float				getSample( int x, int y ) const
		{
			return samples[ y * size + x ];
		}

		float						getHeight( float x, float y ) const;		

		bool						findIntersection( const Line& line, Real32& t, Vec3f& normal );

		void						updateMinMaxSamples();

		int							size;
		float						minSampleValue;
		float						maxSampleValue;
		std::vector< float >		samples;
	};


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef QAEB_RENDERER_INCLUDED

//-------------------------------------------------------------------------------------------------

