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

#include "OSGHeightmap.h"

OSG_BEGIN_NAMESPACE

Heightmap::Heightmap() :
    size          (0  ),
    minSampleValue(0.f),
    maxSampleValue(0.f),
    samples       (   )
{
    size			= 0;
    minSampleValue	= 0.0f;
    maxSampleValue	= 0.0f;
}


void Heightmap::initialize( int initSize )
{
    this->size = initSize;
    samples.resize( initSize * initSize );
    minSampleValue = 0.0f;
    maxSampleValue = 0.0f;
}


float Heightmap::getHeight( float x, float y ) const
{
    // todo: build in triangle interpolation
    
    // find the nearest sample and return it:
    int ix = int(x);
    int iy = int(y);
    
    if( ( ix < 0 || ix >= size ) ||
        ( iy < 0 || iy >= size ) )
    {
        // no height
        return 0.0f;
    }
    
    return samples[ iy * size + ix ];
}

bool Heightmap::findIntersection( const Line& line, Real32& t, Vec3f& normal )
{
    //// ray.origin and dir are in local coordinates already.. 		
    //Vec3f rayPosition = line.getPosition() + t * line.getDirection();
    
    //if( !boundingBox_.containsPoint( rayPosition ) )
    //{
    //	// so we need to do find the first intersection
    //	// of the ray with the bounding box 
    //	float boxT;
    //	if( !getIntersection( ray, boundingBox_, boxT ) )
    //	{
    //		// no intersection.. this means we don't have an intersection at all
    //		return false;
    //	}
    //	else
    //	{
    //		// update the start position:
    //		const float epsilon = 0.0001f;
    //		t = boxT + epsilon;
    //		rayPosition = ray( boxT );
    //	}
    //}
    
    //// start position should be inside now:
    //assert( boundingBox_.containsPoint( rayPosition ) );
    
    //const float epsilon = MathType< float >::epsilon;
    
    //// boundary case: check, if the direction is down or upwards only:
    //if( fabsf( ray.direction.x ) < epsilon && fabsf( ray.direction.y ) < epsilon )
    //{
    //	// special case.. ray directly down/up 
    //	const float height = getHeight( rayPosition.x, rayPosition.y );
    
    //	if( ( rayPosition.z <= height && ray.direction.z > 0.0f ) ||
    //		( rayPosition.z >= height && ray.direction.z < 0.0f ) )
    //	{
    //		// determine the time of impact:
    //		t = ( height - rayPosition.z ) / ray.direction.z;
    
    //		return t >= ray.minT && t <= ray.maxT;
    //	}
    //	else
    //	{
    //		// no impact going to happen..:
    //		return false;
    //	}
    //}
    
    //// and start a bresenham line algorithm from there
    //Vector2i currentPos = Vector2i( rayPosition.x, rayPosition.y );
    //Vector2i targetPos = Vector2i( ray( ray.maxT ) );
    
    //float currentHeight = rayPosition.z;
    ////float deltaHeight = ( 
    
    //if( fabsf( ray.direction.x ) >= fabsf( ray.direction.y ) )
    //{
    //	// x is dominant
    //	if( ray.direction.x > 0.0f )
    //	{
    //		Planef heixelPlane;
    //		float dy = rayPosition.y - currentPos.y;
    
    //		while( currentPos.x < targetPos.x )
    //		{
    //			if( ( currentPos.x >= size_.x ) || 
    //				( currentPos.y < 0 || currentPos.y >= size_.y ) )
    //			{
    //				return false;
    //			}
    //			//float height = 
    
    //			// check, if we need to increment y:
    //			if( dy > 1.0f )
    //			{
    //				currentPos.y++;
    //			}
    //			currentPos.x++;
    //		}
    //	}
    //	else
    //	{
    //		assert( ray.direction.x < epsilon );
    //	}
    //}
    //else
    //{
    //	// y is dominant
    //}
    //// for every passed block we check, if the ray intersects the heightmap at that point
    
    //// get the current position of the ray:		
    
    return false;
}


void Heightmap::updateMinMaxSamples()
{
    const int sampleCount = int(samples.size());
    
    if( sampleCount == 0 )
    {
        minSampleValue = 0.0f;
        maxSampleValue = 0.0f;
        return;
    }
    
    const float* sampleStart = &samples[ 0 ];
    const float* sampleEnd = sampleStart + sampleCount;
    
    minSampleValue = *sampleStart;
    maxSampleValue = *sampleStart;
    
    for( const float *samplePtr  = sampleStart + 1; 
                      samplePtr != sampleEnd; 
                    ++samplePtr                   )
    {
        minSampleValue = std::min( minSampleValue, *samplePtr );
        maxSampleValue = std::max( maxSampleValue, *samplePtr );
    }
}

OSG_END_NAMESPACE
