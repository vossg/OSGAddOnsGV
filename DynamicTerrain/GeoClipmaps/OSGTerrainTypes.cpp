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

#include "OSGTerrainTypes.h"
#include <algorithm>

OSG_BEGIN_NAMESPACE

Vec3f colorToVector( Color3f color )
{
    return Vec3f( color.red(), color.green(), color.blue() );
}

Pnt2i componentDivide( const Pnt2i& lhs, const Pnt2i& rhs )
{
    return Pnt2i( lhs[ 0 ] / rhs[ 0 ], lhs[ 1 ] / rhs[ 1 ] );
}



Pnt2i componentModulo( const Pnt2i& lhs, const Pnt2i& rhs )
{
    return Pnt2i( lhs[ 0 ] % rhs[ 0 ], lhs[ 1 ] % rhs[ 1 ] );
}


Pnt2i componentModulo( const Pnt2i& lhs, Int32 rhs )
{
    return Pnt2i( lhs[ 0 ] % rhs, lhs[ 1 ] % rhs );
}



Pnt2f componentDivide( const Pnt2f& lhs, const Pnt2f& rhs )
{
    return Pnt2f( lhs[ 0 ] / rhs[ 0 ], lhs[ 1 ] / rhs[ 1 ] );
}


Pnt3f componentDivide( const Pnt3f& lhs, const Pnt3f& rhs )
{
    return Pnt3f( lhs[ 0 ] / rhs[ 0 ], lhs[ 1 ] / rhs[ 1 ], lhs[ 2 ] / rhs[ 2 ] );
}

	

Pnt3f componentDivide( const Pnt3f& lhs, Real32 rhs )
{
    return Pnt3f( lhs[ 0 ] / rhs, lhs[ 1 ] / rhs, lhs[ 2 ] / rhs );
}



Pnt2f componentDivide( const Pnt2f& lhs, Real32 rhs )
{
    return Pnt2f( lhs[ 0 ] / rhs, lhs[ 1 ] / rhs );
}



Pnt2f componentMultiply( const Pnt2f& lhs, const Pnt2f& rhs )
{
    return Pnt2f( lhs[ 0 ] * rhs[ 0 ], lhs[ 1 ] * rhs[ 1 ] );
}

	

Pnt2f componentMultiply( const Pnt2f& lhs, Real32 rhs )
{
    return Pnt2f( lhs[ 0 ] * rhs, lhs[ 1 ] * rhs );
}


Pnt2i componentMultiply( const Pnt2i& lhs, Int32 rhs )
{
    return Pnt2i( lhs[ 0 ] * rhs, lhs[ 1 ] * rhs );
}



Pnt2i componentDivide( const Pnt2i& lhs, Int32 rhs )
{
    return Pnt2i( lhs[ 0 ] / rhs, lhs[ 1 ] / rhs );
}



Pnt2i componentAdd( const Pnt2i& lhs, const Pnt2i& rhs )
{
    return Pnt2i( lhs[ 0 ] + rhs[ 0 ], lhs[ 1 ] + rhs[ 1 ] );
}



Int32 getAlignedInt( Int32 x, Int32 n, bool roundUp )
{
    assert( n > 0 );
    
    Int32 result = ( x / n ) * n;
    
    if( roundUp && ( result < x ) )
    {
        return result + n;
    }
    else if( !roundUp && ( result > x ) )
    {
        return result - n;
    }
    else
    {
        return result;
    }
}


Pnt2i alignPoint( const Pnt2i& p, Int32 n, bool roundUp )
{
    return Pnt2i( getAlignedInt( p[ 0 ], n, roundUp ), 
                  getAlignedInt( p[ 1 ], n, roundUp ) );
}



Rectangle2i alignRectangle( const Rectangle2i& rect, const Pnt2i& alignment )
{	
    Rectangle2i alignedRect;
    
    // todo: adjust the active region so that the active region lies on "even" vertices.
    alignedRect._x0 = getAlignedInt( rect._x0, alignment[ 0 ], true );
    alignedRect._y0 = getAlignedInt( rect._y0, alignment[ 1 ], true );
    alignedRect._x1 = getAlignedInt( rect._x1, alignment[ 0 ], false );
    alignedRect._y1 = getAlignedInt( rect._y1, alignment[ 1 ], false );
    
    //if( alignedRect.x1 < rect.x1 )
    //{
    //	alignedRect.x1++;
    //}
    //else if( alignedRect.x1 == rect.x1 )
    //{
    //	alignedRect.x1--;
    //}
    //if( alignedRect.y1 < rect.y1 )
    //{
    //	alignedRect.y1++;
    //}
    //else if( alignedRect.y1 == rect.y1 )
    //{
    //	alignedRect.y1--;
    //}
    
    return alignedRect;
}

OSG_END_NAMESPACE
