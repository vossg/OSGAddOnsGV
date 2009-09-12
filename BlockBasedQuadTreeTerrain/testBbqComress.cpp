/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2007 by the OpenSG Forum                   *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 *   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                License                                    *
 *                                                                           *
 * This library is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Library General Public License as published    *
 * by the Free Software Foundation, version 2.                               *
 *                                                                           *
 * This library is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public         *
 * License along with this library; if not, write to the Free Software       *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Changes                                    *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Notes                                      *
 *                                                                           *
 * Implementation based on the original thesis work by Julien Koenen         *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

#include "OSGBbqTerrainCompressor.h"
#include <iostream>

//-------------------------------------------------------------------------------------------------

OSG_USING_NAMESPACE

//-------------------------------------------------------------------------------------------------

int main( int argc, char** argv )
{
    std::string bbqSourceFilename = "data/ps.bbq";
    std::string bbqTargetFilename = "data/ps_com.bbq";

    int compressionQuality = 500;

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
