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

#include "OSGGeometryClipmaps.h"
#include "OSGHeightDataSource.h"
#include "OSGTextureDataSource.h"
#include "OSGTerrainTools.h"
#include <OSGImage.h>

OSG_BEGIN_NAMESPACE

void GeometryClipmaps::dumpIt(void)
{
    fprintf(stderr, "Got %zd levels\n", levels_.size());

    for(UInt32 i = 0; i < levels_.size(); ++i)
    {
        fprintf(stderr, "Level %d\n", i);

        levels_[i].dumpIt();
    }
}

GeometryClipmaps::GeometryClipmaps()
{
    heightSource_ = 0;
    textureSource_ = 0;
}

GeometryClipmaps::~GeometryClipmaps()
{
}


bool GeometryClipmaps::initialize( int requestedLevelSize, 
                                   HeightDataSource* heightSource, 
                                   TextureDataSource* textureSource )
{
    if( requestedLevelSize < 3 )
    {
        // todo: report an error...
        return false;
    }
    if( !heightSource )
    {
        return false;
    }
    
    // fix the levelsize to be 2**x - 1:
    const int levelSampleCount = int(getNextPowerOf2( requestedLevelSize ) - 1);
    const int worldSampleCount = int(heightSource->getSampleCount().maxValue());
    
    // Calculate needed level count 
    int levelCount = 1;
    int levelSize = levelSampleCount;
    
    while( levelSize < worldSampleCount )
    {
        levelCount++;
        levelSize = ( 1 << ( levelCount - 1 ) ) * ( levelSampleCount - 1 ) + 1;
    }
    
    // debug: use only 2 levels:
    const int maxLevelCount = 20;
    levelCount = std::min( maxLevelCount, levelCount );
    
    // todo: make this a parameter:
    const int TextureSize = 256;
    
    levels_.resize( levelCount );
    
    // initialize clipmap-level:        
    for( int i = 0; i < levelCount; ++i )
    {
        // Level 0 is the coarsest one: so reverse the index:
        const int levelIndex = levelCount - i - 1;
        
        GeometryClipmapLevel& level = levels_[ i ];
        
        level.index             = i;
        level.isSampleDataDirty = true;
        level.sampleSpacing     = 1 << levelIndex;
        
        level.heightmap.initialize( levelSampleCount );
        
        level.textureData = Image::create();
        
        level.textureData->set( Image::OSG_RGBA_PF, TextureSize, TextureSize );
    }           
    
    // one full levelupdate is allowed..
    sampleUpdateBudget_ = ( levelSampleCount * levelSampleCount );      
    
    // initialize the render data:
    if( !cpuRenderer_.initialize( this ) )
    {
        levels_.clear();
        return false;
    }
    
    heightSource_   = heightSource;
    textureSource_  = textureSource;

    return true;
}



void GeometryClipmaps::adjustVolume( Volume& volume, float heightDataScale )
{
    volume.extendBy( Pnt3f( 0, 0, 0 ) );
    
    if( heightSource_ )
    {
        Pnt2i sampleCount = heightSource_->getSampleCount();
        
        const float minHeight = 0.0f;
        const float maxHeight = heightDataScale;
        
        volume.extendBy( Pnt3f( 0, minHeight, sampleCount[ 1 ] ) );
        volume.extendBy( Pnt3f( sampleCount[ 0 ], maxHeight, sampleCount[ 1 ] ) );
        volume.dump();
    }
}


void GeometryClipmaps::shutdown()
{
    levels_.clear();
    
    cpuRenderer_.shutdown();
    heightSource_           = 0;
    textureSource_          = 0;
    sampleUpdateBudget_     = 0;
    notFinishedLastFrame_   = false;
}


bool GeometryClipmaps::isValid() const
{
    return heightSource_ != 0;
}



void GeometryClipmaps::update( const Pnt3f& localViewerPos )
{
    assert( heightSource_ );
    
    // todo: transform world to modelposition (inverse transform)
    Pnt2i viewerSamplePos = Pnt2i( int(localViewerPos[0]),
                                   int(localViewerPos[2]) );
    
    // for debugging:
    static Pnt2i oldViewerPos;
    
    if( viewerSamplePos != oldViewerPos && !notFinishedLastFrame_ )
    {
        oldViewerPos = viewerSamplePos;
    }
    
    notFinishedLastFrame_ = false;
    
    cpuRenderer_.updateRenderData( localViewerPos );
    
    const int levelCount = levels_.size();
    
    // update clip rect:
    int updatedSampleCount = 0;
    
    // from coarse to fine:
    for( int levelIdx = 0; levelIdx < levelCount; ++levelIdx )
    {
        GeometryClipmapLevel& level = levels_[ levelIdx ];
        GeometryClipmapLevel* parentLevel = 0;
        
        if( levelIdx > 0 )
        {
            parentLevel = &levels_[ levelIdx - 1 ];
        }
        
        const int halfSampleCoverage = level.getSampleCoverage() / 2;
        
        // set the desired origin of the level: (aligned to the the sample spacing of this level )
        Pnt2i newOrigin = alignPoint( viewerSamplePos - Pnt2i( halfSampleCoverage, halfSampleCoverage ), level.sampleSpacing, false );
        
        if( parentLevel )
        {
            // align to parent sample spacing:
            newOrigin = alignPoint( newOrigin, parentLevel->sampleSpacing, false );
        }
        else
        {
            // todo: Level 0 needs to cover the whole terrain: so it needs to
            // start at 0,0
            newOrigin = Pnt2i( 0, 0 );
        }
        
        // do the update:
        updatedSampleCount += updateLevel( level, parentLevel, newOrigin );
        
        if( updatedSampleCount >= sampleUpdateBudget_ )
        {
            // continue next frame
            SLOG << "Geometry Clipmaps: update sample limit reached!\n";
            
            for( int i = levelIdx + 1; i < levelCount; ++i )
            {
                GeometryClipmapLevel& level = getLevel( i );
                
                level.isActive = false;
            }
            // and stop updating here.
            notFinishedLastFrame_ = true;
            break;
        }           
    }       
    
    //todo: update statistics/profiler
    //lastUpdateSampleCount_ = updatedSampleCount;
}

void GeometryClipmaps::render( const ClipmapRenderParameters& renderParams, ClipmapRenderStatistics& renderStats )
{
    cpuRenderer_.render( renderParams );
    
    renderStats = cpuRenderer_.getLastFrameStatistics();
}



void GeometryClipmaps::setCpuVertexProgramText( const std::string& text )
{
    cpuRenderer_.setVertexProgram( text.c_str() );
}



void GeometryClipmaps::setCpuFragmentProgramText( const std::string& text )
{
    cpuRenderer_.setFragmentProgram( text.c_str() );
}



bool GeometryClipmaps::findFirstIntersection( const Line& line, 
                                              Real32& t, 
                                              Vec3f& normal )
{
    //todo: implement this
    return false;
}


int GeometryClipmaps::getLevelCount() const
{
    return levels_.size();
}


GeometryClipmapLevel& GeometryClipmaps::getLevel( int index )
{
    assert( index >= 0 );
    assert( index < getLevelCount() );
    return levels_[ index ];
}


Pnt2i GeometryClipmaps::getWorldSampleCount() const
{
    if( heightSource_ )
    {
        return heightSource_->getSampleCount();
    }
    else
    {
        return Pnt2i( 1, 1 );
    }
}


int GeometryClipmaps::updateLevel( GeometryClipmapLevel& level, 
                                   GeometryClipmapLevel* parentLevel, 
                                   const Pnt2i& newOrigin )
{
    //todo: move this into the GeometryClipmapLevel class??
    // how much samples do we have to update?
    const Pnt2i oldOrigin = level.sampleOrigin;
    const Pnt2i updateSampleCount = newOrigin - oldOrigin;
    
    const bool forceFullUpdate = level.isSampleDataDirty;
    
    if( !forceFullUpdate && oldOrigin == newOrigin )
    {
        // nothing to do:
        return 0;
    }
    
    //SLOG << "Level " << level.index << " moved by " << updateSampleCount << " oldOrigin = " << oldOrigin << " newOrigin = " << newOrigin << "\n";
    
    // whatever we need to do: the origin is now here:
    level.sampleOrigin = newOrigin;
    
    const int levelSampleCount = level.heightmap.size;
    const int levelSampleSpacing = level.sampleSpacing;
    
    const bool fullBlockUpdate = ( abs( updateSampleCount[ 0 ] ) >= levelSampleCount ) || 
        ( abs( updateSampleCount[ 1 ] ) >= levelSampleCount );
    
    int updatedSampleCount = 0;
    
    if( fullBlockUpdate || forceFullUpdate )
    {
        // full block update:
        // reset the block origin:
        level.blockOrigin.setValues( 0, 0 );
        updatedSampleCount += updateLevelBlock( level, 
                                                parentLevel, 
                                                Rectangle2i( 0, 0, levelSampleCount, levelSampleCount ) );
    }
    else if( updateSampleCount[ 0 ] != 0 || updateSampleCount[ 1 ] != 0 )
    {
        Pnt2i levelSampleCount2d( levelSampleCount, levelSampleCount );
        
        // adjust adressing..
        Pnt2i blockDelta = componentDivide( updateSampleCount, levelSampleSpacing );
        
        Pnt2i oldBlockOrigin( level.blockOrigin[ 0 ], level.blockOrigin[ 1 ] );
        
        // update block origin:
        Pnt2i newBlockOrigin = componentModulo( componentAdd( componentAdd( oldBlockOrigin, blockDelta ), levelSampleCount2d ), levelSampleCount2d );
        
        assert( newBlockOrigin[ 0 ] >= 0 );
        assert( newBlockOrigin[ 1 ] >= 0 );
        assert( newBlockOrigin[ 0 ] < levelSampleCount );
        assert( newBlockOrigin[ 1 ] < levelSampleCount );
        
        level.blockOrigin = newBlockOrigin;
        
        // update the dirty Rects:
        // in general the update region is ? shaped: (we do 1 or 2 rectangular updates)
        Rectangle2i region1( 0, 0, levelSampleCount, levelSampleCount );
        Rectangle2i region2( 0, 0, levelSampleCount, levelSampleCount );
        Rectangle2i region3( 0, 0, levelSampleCount, levelSampleCount );
        
        if( blockDelta[ 0 ] > 0 )
        {
            region1.x0 = oldBlockOrigin[ 0 ];
            region1.x1 = region1.x0 + blockDelta[ 0 ];
        }
        else if( blockDelta[ 0 ] < 0 )
        {
            region1.x0 = newBlockOrigin[ 0 ];
            region1.x1 = region1.x0 - blockDelta[ 0 ];
        }
        else
        {
            // region 1 is empty:
            region1.setValues( 0, 0, 0, 0 );
        }
        if( blockDelta[ 1 ] > 0 )
        {
            region2.y0 = oldBlockOrigin[ 1 ];
            region2.y1 = region2.y0 + blockDelta[ 1 ];
        }
        else if( blockDelta[ 1 ] < 0 )
        {
            region2.y0 = newBlockOrigin[ 1 ];
            region2.y1 = region2.y0 - blockDelta[ 1 ];
        }
        else
        {
            // region 1 is empty:
            region2.setValues( 0, 0, 0, 0 );
        }
        
        updatedSampleCount += updateBigLevelBlock( level, parentLevel, region1 );
        updatedSampleCount += updateBigLevelBlock( level, parentLevel, region2 );
    }
    
    // for debugging:
    //checkLevelBlock( level, Rectangle2i( 0, 0, levelSampleCount, levelSampleCount ) );
    
    cpuRenderer_.rebuildIndices( level );       
    
    if( parentLevel )
    {
        // todo: is this really needed every frame?
        cpuRenderer_.rebuildIndices( *parentLevel );
    }
    
    // determine min/max sample:
    level.heightmap.updateMinMaxSamples();
    
    // activate this level:
    level.isActive = true;
    
    // we just updated the data:
    level.isSampleDataDirty = false;
    
    // todo: make this work:
    return updatedSampleCount;
    //return 0;
}



//void GeometryClipmaps::checkLevelBlock( GeometryClipmapLevel& level, const Rectangle2i& targetRect )
//{
//  // send a heightdata request to the data provider:
//  HeightTargetBufferDescription targetDesc;

//  std::vector< Real32 > testData( targetRect.getWidth() * targetRect.getHeight() );

//  // we want floats:
//  targetDesc.format       = HeightDataFormat_Real32;
//  targetDesc.targetPtr    = ( unsigned char* )&testData[ 0 ];
//  targetDesc.targetStride = sizeof( Real32 ) * level.heightmap.size;
//  targetDesc.targetRect   = targetRect;
//
//  // get the sample origin of the first point:
//  const Pnt2i sampleOrigin = level.blockPosToSamplePos( targetRect.getOrigin() );

//  // get the height data from the height data source:
//  heightSource_->fillHeightData( level, targetDesc, sampleOrigin, level.sampleSpacing );

//  const int levelSampleCount = level.heightmap.size;

//  // compare the test data with the current data (should match)
//  for( int y = targetRect.y0; y < targetRect.y1; ++y )
//  {
//      for( int x = targetRect.x0; x < targetRect.x1; ++x )
//      {
//          const float testSample = testData[ y * levelSampleCount + x ];
//          const float currentSample = level.heightmap.getSample( x, y );
//          
//          if( fabsf( currentSample - testSample ) > 0.1f )
//          {
//              Pnt2i samplePos = level.blockPosToSamplePos( Pnt2i( x, y ) );

//              PLOG << " Sample mismatch at Sample [" << samplePos << "] at BlockPos [" << Pnt2i( x, y ) << "]\n";
//          }
//      }
//  }
//}



int GeometryClipmaps::updateBigLevelBlock( GeometryClipmapLevel& level, const GeometryClipmapLevel* parentLevel, const Rectangle2i& targetRect )
{
    if( targetRect.isEmpty() )
    {
        return 0;
    }
    
    const int levelSampleCount = level.heightmap.size;
    
    // split the rect into up to 4 subrects and update them individually:
    Rectangle2i subRects[ 4 ];
    int subRectCount = 0;
    
#if 0
    int midY = 0;
    int maxY = 0;
    int midX = 0;
    int maxX = 0;
#endif
    
    subRects[ 0 ] = targetRect;
    subRectCount++;
    
    if( targetRect.x1 < targetRect.x0 )
    {
        // split in X:              
        subRects[ 0 ].x1 = levelSampleCount;
        subRects[ 1 ] = targetRect;
        subRects[ 1 ].x0 = 0;
        subRectCount++;
    }
    else if( targetRect.x1 > levelSampleCount )
    {
        // split in X:              
        subRects[ 0 ].x1 = levelSampleCount;
        subRects[ 1 ] = targetRect;
        subRects[ 1 ].x0 = 0;
        subRects[ 1 ].x1 -= levelSampleCount;
        subRectCount++;
    }
    
    // ok.. we have 1 or 2 subRects already: split them on the Y axis:
    if( targetRect.y1 < targetRect.y0 )
    {
        // patch the existing ones:
        for( int i = 0; i < subRectCount; ++i )
        {
            subRects[ subRectCount + i ] = subRects[ i ];
            
            subRects[ subRectCount + i ].y0 = 0;
            subRects[ i ].y1 = levelSampleCount;
        }
        subRectCount += subRectCount;
    }
    else if( targetRect.y1 > levelSampleCount )
    {
        // patch the existing ones:
        for( int i = 0; i < subRectCount; ++i )
        {
            subRects[ subRectCount + i ] = subRects[ i ];
            
            subRects[ subRectCount + i ].y0 = 0;
            subRects[ subRectCount + i ].y1 -= levelSampleCount;
            subRects[ i ].y1 = levelSampleCount;
        }
        subRectCount += subRectCount;
    }
    
    int sampleUpdateCount = 0;
    
    for( int i = 0; i < subRectCount; ++i )
    {
        sampleUpdateCount += updateLevelBlock( level, parentLevel, subRects[ i ] );
    }
    return sampleUpdateCount;
}



int GeometryClipmaps::updateLevelBlock( GeometryClipmapLevel& level, const GeometryClipmapLevel* parentLevel, const Rectangle2i& targetRect )
{
    const int result = level.updateBlock( heightSource_, textureSource_, targetRect );
    
    // and rebuild the changed vertices:
    cpuRenderer_.buildVertices( level, parentLevel, targetRect );       
    
    // return the number of updated samples:
    return result;
}

OSG_END_NAMESPACE

