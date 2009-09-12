//-----------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		10.12.2004 23:16:14
//
//	Description			:		main.cpp
//
//-----------------------------------------------------------------------------

//#include "motor3d/debug/Assert.h"
//#include "motor3d/debug/Log.h"
//#include "motor3d/platform/CommandLineOptions.h"
//#include "motor3d/stream/FileStream.h"

#include "OSGBbqTerrainCreator.h"
//#include "motor3d/math/LinearSolver.h"
#include "OSGBaseInitFunctions.h"

#include <iostream>

//-----------------------------------------------------------------------------

OSG_USING_NAMESPACE

//-----------------------------------------------------------------------------


int main( int argc, char** argv )
{

    osgInit(argc, argv);

#if 0
	motor3d::CommandLineOptions cmdOptions;
    
	cmdOptions.addUsage( "Usage: createBbqTerrain [OPTIONS]... [HEIGHTFIELD-FILE] [TEXTURE-FILE] [BBQ-FILE]" );
	cmdOptions.addUsage( "Use -h or --help for a complete list of options" );
	cmdOptions.addUsage( " --tileSize=tilesize (-t) Supported tileSize values are: 9,17,33,65 and 129!" );
    
	cmdOptions.setFlag( "help", 'h' );
	cmdOptions.setOption( "tileSize", 't' );
    
	cmdOptions.processCommandArgs( argc, argv, 3 );
    
	if( cmdOptions.getFlag( "help" ) || cmdOptions.getFlag( 'h' ) || cmdOptions.getArgc() < 3 )
	{
		cmdOptions.printUsage();
		return 0;
	} 
#endif

	int tileSize = 65;
	int textureSize = 128;

#if 0
	std::string heightFieldFilename = cmdOptions.getArgv( 0 );
	std::string textureFilename     = cmdOptions.getArgv( 1 );
	std::string bbqTerrainFilename  = cmdOptions.getArgv( 2 );
#else
	std::string heightFieldFilename = "data/ps_height_1k.png";
	std::string textureFilename     = "data/ps_texture_1k.png";
	std::string bbqTerrainFilename  = "data/ps.bbq";
#endif



	BbqTerrainCreator *terrainCreator = new BbqTerrainCreator();
    
	if( !terrainCreator->start( heightFieldFilename, 
                                textureFilename, 
                                bbqTerrainFilename, 
                                tileSize, 
                                textureSize ) )
	{
		std::cout << "Could not produce the .bbq Terrain File!\n";
		return 1;
	}
    
	while(!terrainCreator->isFinished() )
	{
		std::cout << "\rProgress = " 
                  << int( 100 * terrainCreator->getProgress() ) 
                  << "%                " 
                  << std::flush;
        
		terrainCreator->step();
	}

	std::cout << "\rFinished!                            \n";
    
    delete terrainCreator;

    OSG::osgExit();

	return 0;
}

