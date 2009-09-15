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

//#include "PrecompiledHeader.h"
#include "OSGGeometryClipmapLevel.h"
#include "OSGHeightDataSource.h"
#include "OSGTextureDataSource.h"
#include "OSGTerrainTools.h"
#include <OSGImage.h>
#include "OSGImageFileHandler.h"

OSG_BEGIN_NAMESPACE

void GeometryClipmapLevel::dumpIt(void)
{
    fprintf(stderr, "index %d\n", index);
    fprintf(stderr, "block origin %d %d\n", blockOrigin[0], blockOrigin[1]);
    fprintf(stderr, "sample origin %d %d\n", sampleOrigin[0], sampleOrigin[1]);
    fprintf(stderr, "texdata %p\n", textureData.get());
    fprintf(stderr, "sampleSpacing %d\n", sampleSpacing);
    
    char tmp[256];
    sprintf(tmp, "/tmp/levelTex%d.png", index);

    ImageFileHandler::the()->write(textureData, tmp);    
}

GeometryClipmapLevel::GeometryClipmapLevel()
{
    index			= 0;
    isActive		= false;
    sampleOrigin	= Pnt2i( 0, 0 );
    blockOrigin		= Pnt2i( 0, 0 );
    sampleSpacing	= 0;
    //clipRect		= Rectangle2i( 0, 0, 0, 0 );
    textureData		= NULL;
}


Pnt2i GeometryClipmapLevel::blockPosToSamplePos( const Pnt2i& blockPos ) const
{
    assert( blockPos[ 0 ] < heightmap.size );
    assert( blockPos[ 1 ] < heightmap.size );
    assert( blockPos[ 0 ] >= 0 );
    assert( blockPos[ 1 ] >= 0 );
    
    Pnt2i blockOffset = Pnt2i( blockPos[ 0 ] - blockOrigin[ 0 ], blockPos[ 1 ] - blockOrigin[ 1 ] );
    
    if( blockOffset[ 0 ] < 0 )
    {
        blockOffset[ 0 ] += heightmap.size;
    }
    if( blockOffset[ 1 ] < 0 )
    {
        blockOffset[ 1 ] += heightmap.size;
    }
    
    return componentAdd( sampleOrigin, componentMultiply( blockOffset, sampleSpacing ) );
}



Pnt2i GeometryClipmapLevel::samplePosToBlockPos( const Pnt2i& samplePos ) const
{
    assert( containsSample( samplePos ) );
    
    // for testing:
    const int sampleOriginX = sampleOrigin[ 0 ];
    const int sampleOriginY = sampleOrigin[ 1 ];
    const int blockOriginX = blockOrigin[ 0 ];
    const int blockOriginY = blockOrigin[ 1 ];
    
    Pnt2i blockPos = componentDivide( samplePos - sampleOrigin, sampleSpacing );
    
    blockPos[ 0 ] += blockOrigin[ 0 ];
    blockPos[ 1 ] += blockOrigin[ 1 ];
    
    if( blockPos[ 0 ] >= heightmap.size )
    {
        blockPos[ 0 ] -= heightmap.size;
    }
    if( blockPos[ 1 ] >= heightmap.size )
    {
        blockPos[ 1 ] -= heightmap.size;
    }
	
    assert( blockPos[ 0 ] < heightmap.size );
    assert( blockPos[ 1 ] < heightmap.size );
    assert( blockPos[ 0 ] >= 0 );
    assert( blockPos[ 1 ] >= 0 );
    
    assert( blockPosToSamplePos( blockPos ) == samplePos );
    
    return blockPos;
}


int GeometryClipmapLevel::getSampleCoverage() const
{
    return sampleSpacing * ( heightmap.size - 1 ) + 1;
}



bool GeometryClipmapLevel::findFirstIntersection( const Line& line, Real32& t, Vec3f& normal )
{
    return heightmap.findIntersection( line, t, normal );
}



Rectangle2i GeometryClipmapLevel::getCoveredSampleRect() const
{
    const int sampleCoverage = getSampleCoverage();
    
    return Rectangle2i( sampleOrigin[ 0 ], sampleOrigin[ 1 ], sampleOrigin[ 0 ] + sampleCoverage, sampleOrigin[ 1 ] + sampleCoverage );
}


	
int	GeometryClipmapLevel::updateBlock( HeightDataSource* heightSource, TextureDataSource* textureSource, const Rectangle2i& targetRect )
{
    assert( heightSource );
    
    if( targetRect.isEmpty() )
    {
        return 0;
    }
    
    // get the sample origin of the first point:
    const Pnt2i sampleOrigin = blockPosToSamplePos( targetRect.getOrigin() );
    
    // get the height data from the height data source:
    heightSource->fillHeightData( *this, targetRect );
    
    // get the texture data.. if available:
    if( textureSource )
    {
        textureSource->fillTextureData( *this, targetRect );
        
        // and update the gl texture
    }
	
    // SLOG << "-updateLevelBlock:" << targetRect << " SOrigin=" << sampleOrigin << " BOrigin=" << blockOrigin[ 0 ] << "," << blockOrigin[ 1 ] << " Spacing=" << sampleSpacing << " Count=" << targetRect.getWidth() * targetRect.getHeight() << "\n";
    
    // return the number of updated samples:
    return 0; // targetRect.getWidth() * targetRect.getHeight();
}



float GeometryClipmapLevel::getBlockSample( const Pnt2i& blockPos ) const
{
    assert( blockPos[ 0 ] < heightmap.size );
    assert( blockPos[ 1 ] < heightmap.size );
    assert( blockPos[ 0 ] >= 0 );
    assert( blockPos[ 1 ] >= 0 );
    
    return heightmap.getSample( blockPos[ 0 ], blockPos[ 1 ] );
}



bool GeometryClipmapLevel::containsSample( const Pnt2i& samplePos ) const
{
    const int sampleCoverage = getSampleCoverage();
    
    return ( ( samplePos[ 0 ] >= sampleOrigin[ 0 ] ) &&
             ( samplePos[ 1 ] >= sampleOrigin[ 1 ] ) &&
             ( samplePos[ 0 ] < sampleOrigin[ 0 ] + sampleCoverage ) &&
             ( samplePos[ 1 ] < sampleOrigin[ 1 ] + sampleCoverage ) );
}

OSG_END_NAMESPACE
