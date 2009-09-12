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

#ifndef _OSGGEOMETRYCLIPMAPLEVEL_H_
#define _OSGGEOMETRYCLIPMAPLEVEL_H_

#include "OSGBaseTypes.h"
#include "OSGLine.h"
#include "OSGImage.h"
#include "OSGTerrainTypes.h"
#include "OSGHeightmap.h"

OSG_BEGIN_NAMESPACE

class HeightDataSource;
class TextureDataSource;


struct GeometryClipmapLevel
{
    void dumpIt(void);

    GeometryClipmapLevel();
    
    Pnt2i       blockPosToSamplePos( const Pnt2i& blockPos ) const;
    Pnt2i       samplePosToBlockPos( const Pnt2i& samplePos ) const;
    
    Rectangle2i getCoveredSampleRect() const;
    int         getSampleCoverage() const;
    
    int         updateBlock( HeightDataSource* dataSource, 
                             TextureDataSource* textureSource, 
                             const Rectangle2i& targetRect );
    
    bool        findFirstIntersection( const Line& line, 
                                       Real32& t, 
                                       Vec3f& normal );
    
    float       getBlockSample( const Pnt2i& blockPos ) const;
    
    bool        containsSample( const Pnt2i& samplePos ) const;
    
    int         index;
    bool        isActive;
    Pnt2i       sampleOrigin;
    Pnt2i       blockOrigin;
    //Rectangle2i              clipRect;           // maximal sample region

    int         sampleSpacing;
    Heightmap   heightmap;
    bool        isSampleDataDirty;
    ImagePtr    textureData;
};


OSG_END_NAMESPACE

#endif /* #ifdef _OSGGEOMETRYCLIPMAPLEVEL_H_ */
