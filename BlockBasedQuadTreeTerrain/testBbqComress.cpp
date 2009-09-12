//-------------------------------------------------------------------------------------------------
//
//  Author              :       Julien Koenen
//  Creation Date       :       10.12.2004 23:16:14
//
//  Description         :       main.cpp
//
//-------------------------------------------------------------------------------------------------

#include "OSGBbqTerrainCompressor.h"
#include <iostream>

//-------------------------------------------------------------------------------------------------

OSG_USING_NAMESPACE

//-------------------------------------------------------------------------------------------------

int main( int argc, char** argv )
{
#if 0
    motor3d::CommandLineOptions cmdOptions;

    cmdOptions.addUsage( "Usage: compressBbqTerrain [OPTIONS]... [BBQ-INPUT] [BBQ-OUTPUT]" );
    cmdOptions.addUsage( "Use -h or --help for a complete list of options" );
    cmdOptions.addUsage( " --quality=[1-1000] [-q] Target Quantization Quality!" );

    cmdOptions.setFlag( "help", 'h' );
    cmdOptions.setOption( "quality", 'q' );

    cmdOptions.processCommandArgs( argc, argv, 3 );

    if( cmdOptions.getFlag( "help" ) || cmdOptions.getFlag( 'h' ) || cmdOptions.getArgc() < 2 )
    {
        cmdOptions.printUsage();
        return 0;
    } 

    std::string bbqSourceFilename = cmdOptions.getArgv( 0 );
    std::string bbqTargetFilename = cmdOptions.getArgv( 1 );
#endif

    std::string bbqSourceFilename = "data/ps.bbq";
    std::string bbqTargetFilename = "data/ps_com.bbq";

    int compressionQuality = 500;

#if 0
    if( cmdOptions.getValue( 'q' ) )
    {
        compressionQuality = clamp( atoi( cmdOptions.getValue( 'q' ) ), 1, 1000 );
    }
#endif

    osgInit(argc, argv);

    std::cout << "Compressing Terrain with Quantization Quality set to " << compressionQuality << " !\n";

    BbqTerrainCompressor terrainCompressor;

    if( !terrainCompressor.start( bbqSourceFilename, bbqTargetFilename, compressionQuality ) )
    {
        std::cout << "Could not produce the .bbq Terrain File!\n";
        return 1;
    }

    while( !terrainCompressor.isFinished() )
    {
        std::cout << "\rProgress = " << int( 100 * terrainCompressor.getProgress() ) << "%                " << std::flush;

        terrainCompressor.step();
    }

    std::cout << "\rFinished!                            \n";

    const BbqTerrainCompressor::Statistics& stats = terrainCompressor.getStatistics();

    std::cout << "Height Compression Factor  = " 
        << float( stats.originalHeightDataSize ) / float( stats.compressedHeightDataSize ) << " " 
        << " [ " << stats.bitsPerHeightSample / 8.0f << " Bytes per height sample ]"
        << std::endl;
    std::cout << "Texture Compression Factor = " << float( stats.originalTextureDataSize ) / float( stats.compressedTextureDataSize ) << " " << std::endl;
    std::cout << "Maximum Error (absolute)   = " << float( stats.maxAbsoluteError ) << " " << std::endl;

    OSG::osgExit();

    return 0;
}

//-------------------------------------------------------------------------------------------------
