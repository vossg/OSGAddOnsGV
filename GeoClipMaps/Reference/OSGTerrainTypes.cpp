//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		03.10.2004 18:19:30
//
//	Description			:		OSGTypes.cpp
//
//-------------------------------------------------------------------------------------------------

#include "PrecompiledHeader.h"
#include "OSGTerrainTypes.h"
#include <algorithm>

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	Vec3f colorToVector( Color3f color )
	{
		return Vec3f( color.red(), color.green(), color.blue() );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2i componentDivide( const Pnt2i& lhs, const Pnt2i& rhs )
	{
		return Pnt2i( lhs[ 0 ] / rhs[ 0 ], lhs[ 1 ] / rhs[ 1 ] );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2i componentModulo( const Pnt2i& lhs, const Pnt2i& rhs )
	{
		return Pnt2i( lhs[ 0 ] % rhs[ 0 ], lhs[ 1 ] % rhs[ 1 ] );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2i componentModulo( const Pnt2i& lhs, Int32 rhs )
	{
		return Pnt2i( lhs[ 0 ] % rhs, lhs[ 1 ] % rhs );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2f componentDivide( const Pnt2f& lhs, const Pnt2f& rhs )
	{
		return Pnt2f( lhs[ 0 ] / rhs[ 0 ], lhs[ 1 ] / rhs[ 1 ] );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt3f componentDivide( const Pnt3f& lhs, const Pnt3f& rhs )
	{
		return Pnt3f( lhs[ 0 ] / rhs[ 0 ], lhs[ 1 ] / rhs[ 1 ], lhs[ 2 ] / rhs[ 2 ] );
	}

	
	//-------------------------------------------------------------------------------------------------


	Pnt3f componentDivide( const Pnt3f& lhs, Real32 rhs )
	{
		return Pnt3f( lhs[ 0 ] / rhs, lhs[ 1 ] / rhs, lhs[ 2 ] / rhs );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2f componentDivide( const Pnt2f& lhs, Real32 rhs )
	{
		return Pnt2f( lhs[ 0 ] / rhs, lhs[ 1 ] / rhs );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2f componentMultiply( const Pnt2f& lhs, const Pnt2f& rhs )
	{
		return Pnt2f( lhs[ 0 ] * rhs[ 0 ], lhs[ 1 ] * rhs[ 1 ] );
	}

	
	//-------------------------------------------------------------------------------------------------


	Pnt2f componentMultiply( const Pnt2f& lhs, Real32 rhs )
	{
		return Pnt2f( lhs[ 0 ] * rhs, lhs[ 1 ] * rhs );
	}
	
	//-------------------------------------------------------------------------------------------------


	Pnt2i componentMultiply( const Pnt2i& lhs, Int32 rhs )
	{
		return Pnt2i( lhs[ 0 ] * rhs, lhs[ 1 ] * rhs );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2i componentDivide( const Pnt2i& lhs, Int32 rhs )
	{
		return Pnt2i( lhs[ 0 ] / rhs, lhs[ 1 ] / rhs );
	}


	//-------------------------------------------------------------------------------------------------


	Pnt2i componentAdd( const Pnt2i& lhs, const Pnt2i& rhs )
	{
		return Pnt2i( lhs[ 0 ] + rhs[ 0 ], lhs[ 1 ] + rhs[ 1 ] );
	}


	//-------------------------------------------------------------------------------------------------


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


	//-------------------------------------------------------------------------------------------------


	Pnt2i alignPoint( const Pnt2i& p, Int32 n, bool roundUp )
	{
		return Pnt2i( getAlignedInt( p[ 0 ], n, roundUp ), 
			getAlignedInt( p[ 1 ], n, roundUp ) );
	}


	//-------------------------------------------------------------------------------------------------


	Rectangle2i alignRectangle( const Rectangle2i& rect, const Pnt2i& alignment )
	{	
		Rectangle2i alignedRect;

		// todo: adjust the active region so that the active region lies on "even" vertices.
		alignedRect.x0 = getAlignedInt( rect.x0, alignment[ 0 ], true );
		alignedRect.y0 = getAlignedInt( rect.y0, alignment[ 1 ], true );
		alignedRect.x1 = getAlignedInt( rect.x1, alignment[ 0 ], false );
		alignedRect.y1 = getAlignedInt( rect.y1, alignment[ 1 ], false );

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


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------
