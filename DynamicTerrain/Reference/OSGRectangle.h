//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		03.10.2004 18:19:46
//
//	Description			:		Rectangle.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef MATH_RECT_INCLUDED
#define MATH_RECT_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGAddOnsConfig.h>
#include <OSGContribDef.h>

#include <OSGVector.h>
#include <ostream>

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	template< typename T > class Rectangle2
	{
	public:
		typedef OSG::Point< T, 2 > PointType;

		explicit				Rectangle2( T x0 = 0, T y0 = 0, T x1 = 0, T y1 = 0 );
		explicit				Rectangle2( const PointType& p, T width, T height );
		explicit				Rectangle2( const PointType& p, const PointType& s );

		T						getWidth() const;
		T						getHeight() const;

		void					add( T x, T y );
		void					add( const PointType& p );

		bool					contains( T x, T y ) const;
		bool					contains( const PointType& p ) const;
		bool					intersects( const Rectangle2& r ) const;
		
		bool					isEmpty() const;

		void					grow( T x );
		void					shrink( T x );

		void					crop( const Rectangle2& boundary, const T& borderSize = 0 );

		PointType				getTopLeft() const;
		PointType				getTopRight() const;
		PointType				getBottomLeft() const;
		PointType				getBottomRight() const;

		void					clear();

		void					setBounds( T x, T y, T width, T height );
		void					setBounds( const PointType& p, T width, T height );
		void					setBounds( const PointType& p, const PointType& size );

		void					setValues( T x0, T y0, T x1, T y1 );

		void					setOrigin( T x, T y );
		void					setOrigin( const PointType& p );
		PointType				getOrigin() const;
		
		void					setSize( T width, T height );
		void					setSize( const PointType& s );
		PointType				getSize() const;

		bool					operator==( const Rectangle2& rhs ) const;
		bool					operator!=( const Rectangle2& rhs ) const;

	public:
		// the rect encloses all points (x,y) with x in [x0,x1[ and y in [y0,y1[
		T						x0;
		T						y0;
		T						x1;
		T						y1;
	};

	template< typename T > 
		inline std::ostream& operator<<( std::ostream& out, const Rectangle2< T >& rect )
		{
			return out << rect.x0 << "," << rect.y0 << "," << rect.x1 << "," << rect.y1;			
		}

	//-------------------------------------------------------------------------------------------------

	typedef Rectangle2< OSG::Real32 >	Rectangle2f;
	typedef Rectangle2< OSG::Real64 >	Rectangle2d;
	typedef Rectangle2< OSG::Int32 >	Rectangle2i;

	//-------------------------------------------------------------------------------------------------

	template< typename T > Rectangle2< T >	alignRectangle( const Rectangle2< T >& rect, const typename Rectangle2< T >::PointType& alignment );
	template< typename T > Rectangle2< T >	getIntersection( const Rectangle2< T >& lhs, const Rectangle2< T >& rhs );
	template< typename T > Rectangle2< T >	getUnion( const Rectangle2< T >& lhs, const Rectangle2< T >& rhs );

	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef MATH_RECT_INCLUDED

//-------------------------------------------------------------------------------------------------
