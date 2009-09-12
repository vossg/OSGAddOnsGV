//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		03.10.2004 18:19:46
//
//	Description			:		OSGTerrainTools.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef OSG_TOOLS_INCLUDED
#define OSG_TOOLS_INCLUDED

//-------------------------------------------------------------------------------------------------

#include <string>
#include <OSGVector.h>
#include <OSGMatrix.h>
#include <OSGColor.h>
#include "OSGTerrainTypes.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	unsigned int		getNextPowerOf2( unsigned int x );
	void				drawBox( const Pnt3f& min, const Pnt3f& max );
	bool				loadTextFile( const char* filename, std::string& text );


	template< typename T > T	clamp( T x, T min, T max )
	{
		assert( min <= max );
		if( x < min )
		{
			return min;
		}
		else if( x > max )
		{
			return max;
		}
		else
		{
			return x;
		}
	}

	//-------------------------------------------------------------------------------------------------
}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef OSG_TOOLS_INCLUDED

//-------------------------------------------------------------------------------------------------
