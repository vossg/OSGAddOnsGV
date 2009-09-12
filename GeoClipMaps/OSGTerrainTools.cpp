//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		03.10.2004 18:19:30
//
//	Description			:		OSGTools.cpp
//
//-------------------------------------------------------------------------------------------------

//#include "PrecompiledHeader.h"
#include "OSGTerrainTools.h"
#include <fstream>

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	unsigned int getNextPowerOf2( unsigned int x )
	{
		for( unsigned int bit = 0; bit < 31; ++bit )
		{
			if( ( 1U << bit ) >= x )
			{
				return 1 << bit;
			}
		}
		return 0;
	}
	

	//-------------------------------------------------------------------------------------------------


	void drawBox( const Pnt3f& min, const Pnt3f& max )
	{
		glBegin( GL_TRIANGLE_STRIP );
		glVertex3f( max[ 0 ], max[ 1 ], min[ 2 ] );
		glVertex3f( max[ 0 ], min[ 1 ], min[ 2 ] );

		glVertex3f( max[ 0 ], max[ 1 ], max[ 2 ] );
		glVertex3f( max[ 0 ], min[ 1 ], max[ 2 ] );

		glVertex3f( min[ 0 ], max[ 1 ], max[ 2 ] );
		glVertex3f( min[ 0 ], min[ 1 ], max[ 2 ] );

		glVertex3f( min[ 0 ], max[ 1 ], min[ 2 ] );
		glVertex3f( min[ 0 ], min[ 1 ], min[ 2 ] );
		glEnd();

		glBegin( GL_TRIANGLE_STRIP );
		glVertex3f( min[ 0 ], min[ 1 ], max[ 2 ] );
		glVertex3f( max[ 0 ], min[ 1 ], max[ 2 ] );

		glVertex3f( min[ 0 ], min[ 1 ], min[ 2 ] );
		glVertex3f( max[ 0 ], min[ 1 ], min[ 2 ] );

		glVertex3f( min[ 0 ], max[ 1 ], min[ 2 ] );
		glVertex3f( max[ 0 ], max[ 1 ], min[ 2 ] );

		glVertex3f( min[ 0 ], max[ 1 ], max[ 2 ] );
		glVertex3f( max[ 0 ], max[ 1 ], max[ 2 ] );
		glEnd();
	}

	//-------------------------------------------------------------------------------------------------


	bool loadTextFile( const char* filename, std::string& text )
	{
		if( !filename )
		{
			return false;
		}
		std::ifstream file( filename, std::ios::binary );

		if( !file.is_open() )
		{
			PWARNING << "[DynamicTerrain] GPU Shader Source File '" << filename << "' not found!\n";
			return false;
		}

		std::ifstream::pos_type begin_pos = file.tellg();
		file.seekg(0, std::ios_base::end);		
		const size_t fileSize = file.tellg() - begin_pos;
		file.seekg(0);

		text.resize( fileSize );
		file.read( &text[ 0 ], fileSize );

		return file.good();
	}


	//-------------------------------------------------------------------------------------------------


}

//-------------------------------------------------------------------------------------------------
