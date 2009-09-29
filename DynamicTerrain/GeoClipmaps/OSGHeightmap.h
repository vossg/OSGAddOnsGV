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

#ifndef _OSGHEIGHTMAP_H_
#define _OSGHEIGHTMAP_H_

#include "OSGTerrainTypes.h"
#include <vector>
#include "OSGLine.h"

OSG_BEGIN_NAMESPACE

class Heightmap
{
  public:       

    Heightmap();
    
    void                        initialize( int size );
    
    inline 
    float               getSample( int x, int y ) const
    {
        return samples[ y * size + x ];
    }
    
    float                       getHeight( float x, float y ) const;        
    
    bool                        findIntersection(const Line   &line, 
                                                       Real32 &t, 
                                                       Vec3f  &normal);
    
    void                        updateMinMaxSamples();
    
    int                         size;
    float                       minSampleValue;
    float                       maxSampleValue;
    std::vector< float >        samples;
};

OSG_END_NAMESPACE

#endif /* #ifdef _OSGHEIGHTMAP_H_ */

