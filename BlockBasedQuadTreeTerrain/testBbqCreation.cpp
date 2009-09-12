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

#include "OSGBbqTerrainCreator.h"
#include "OSGBaseInitFunctions.h"

#include <iostream>

//-----------------------------------------------------------------------------

OSG_USING_NAMESPACE

//-----------------------------------------------------------------------------


int main( int argc, char** argv )
{

    osgInit(argc, argv);

    int tileSize    = 65;
    int textureSize = 128;

    std::string heightFieldFilename = "data/ps_height_1k.png";
//  std::string heightFieldFilename = "data/Z_70_22.TIF";
    std::string textureFilename     = "data/ps_texture_1k.png";
    std::string bbqTerrainFilename  = "data/ps.bbq";


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

