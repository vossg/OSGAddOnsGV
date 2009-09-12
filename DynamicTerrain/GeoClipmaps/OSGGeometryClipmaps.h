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

#ifndef _OSGGEOMETRYCLIPMAPS_H_
#define _OSGGEOMETRYCLIPMAPS_H_

#include "OSGBaseTypes.h"
#include "OSGTerrainTypes.h"
#include "OSGCpuClipmapRenderer.h"
#include "OSGHeightmap.h"
#include "OSGGeometryClipmapLevel.h"

OSG_BEGIN_NAMESPACE

class HeightDataSource;
class TextureDataSource;


class GeometryClipmaps
{
  public:

    GeometryClipmaps();
    ~GeometryClipmaps();
    
    bool                  initialize(int levelSize, 
                                     HeightDataSource* heightSource, 
                                     TextureDataSource* textureSource );

    void                  shutdown();
    
    bool                  isValid() const;

    void                  update( const Pnt3f& viewerPosition );
    void                  render( const ClipmapRenderParameters& renderParams, 
                                  ClipmapRenderStatistics& renderStats );
    
    void                  adjustVolume( Volume& volume, float heightDataScale);
    
    // interaction with the renderer:
    void                  setCpuVertexProgramText( const std::string& text );
    void                  setCpuFragmentProgramText( const std::string& text );
    
    // Samplecoord <-> Worldcoord
    Pnt2i                 getWorldSampleCount() const;
    
    //Pnt2f               samplePosToWorldPos( const Pnt2i& samplePos ) const;
    //Pnt2i               worldPosToSamplePos( const Pnt2f& worldPos ) const;
    
    // ray intersection:
    bool                  findFirstIntersection(const Line& line, 
                                                Real32& t, 
                                                Vec3f& normal );
    
    int                   getLevelCount() const;
    GeometryClipmapLevel &getLevel( int index );
    
  protected:

    int  updateLevel( GeometryClipmapLevel& level, 
                      GeometryClipmapLevel* parentLevel, 
                      const Pnt2i& newOrigin );

    void checkLevelBlock(GeometryClipmapLevel& level, 
                         const Rectangle2i& targetRect );

    int updateBigLevelBlock( GeometryClipmapLevel& level, 
                             const GeometryClipmapLevel* parentLevel, 
                             const Rectangle2i& targetRect );

    int updateLevelBlock( GeometryClipmapLevel& level, 
                          const GeometryClipmapLevel* parentLevel, 
                          const Rectangle2i& targetRect );
    

    typedef std::vector< GeometryClipmapLevel > ClipmapLevelList;
    
    HeightDataSource*           heightSource_;
    TextureDataSource*          textureSource_;
    CpuClipmapRenderer          cpuRenderer_;
    //GpuGeoClipmapRendere      gpuRenderer_;
    int                         sampleUpdateBudget_;
    bool                        notFinishedLastFrame_;
    ClipmapLevelList            levels_;
};


OSG_END_NAMESPACE

#endif /* #ifdef _OSGGEOMETRYCLIPMAPS_H_ */


