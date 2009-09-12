/*---------------------------------------------------------------------------*\
*                                OpenSG                                     *
*                                                                           *
*                                                                           *
*               Copyright (C) 2000-2002 by the OpenSG Forum                 *
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
*                                                                           *
\*---------------------------------------------------------------------------*/

#include "OSGTerrainTools.h"
#include <fstream>

OSG_BEGIN_NAMESPACE

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

bool loadTextFile( const char* filename, std::string& text )
{
    if( !filename )
    {
        return false;
    }
    std::ifstream file( filename, std::ios::binary );
    
    if( !file.is_open() )
    {
        PWARNING << "[DynamicTerrain] GPU Shader Source File '" 
                 << filename << "' not found!\n";
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

OSG_END_NAMESPACE
