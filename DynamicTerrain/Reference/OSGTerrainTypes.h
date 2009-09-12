//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		03.10.2004 18:19:46
//
//	Description			:		OSGTypes.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef OSG_TYPES_INCLUDED
#define OSG_TYPES_INCLUDED

//-------------------------------------------------------------------------------------------------

#include <OSGVector.h>
#include <OSGMatrix.h>
#include <OSGColor.h>
#include "OSGRectangle.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	typedef OSG::PointInterface< OSG::Real32, OSG::VecStorage2< OSG::Real32 > > Pnt2f;	
	typedef OSG::PointInterface< OSG::Real32, OSG::VecStorage3< OSG::Real32 > > Pnt3f;	
	typedef OSG::PointInterface< OSG::Real32, OSG::VecStorage4< OSG::Real32 > > Pnt4f;
	typedef OSG::PointInterface< OSG::Int32, OSG::VecStorage2< OSG::Int32 > >	Pnt2i;
	typedef OSG::TransformationMatrix< OSG::Real32 >							TransformMatrix;	

	Vec3f			colorToVector( Color3f color );

	Pnt2i			componentDivide( const Pnt2i& lhs, const Pnt2i& rhs );

	// todo: why does this not work :)
	Pnt2i 			componentDivide( const Pnt2i& lhs, Int32 rhs );
	Pnt2i 			componentDivide( const Pnt2i& lhs, Int32 rhs );
	Pnt2f 			componentDivide( const Pnt2f& lhs, const Pnt2f& rhs );
	Pnt3f 			componentDivide( const Pnt3f& lhs, const Pnt3f& rhs );
	Pnt3f 			componentDivide( const Pnt3f& lhs, Real32 rhs );
	Pnt2f 			componentDivide( const Pnt2f& lhs, Real32 rhs );
	Pnt2f 			componentMultiply( const Pnt2f& lhs, const Pnt2f& rhs );
	Pnt2f 			componentMultiply( const Pnt2f& lhs, Real32 rhs );
	Pnt2i 			componentMultiply( const Pnt2i& lhs, Int32 rhs );
	Pnt2i 			componentModulo( const Pnt2i& lhs, const Pnt2i& rhs );
	Pnt2i 			componentModulo( const Pnt2i& lhs, Int32 rhs );
	Pnt2i 			componentAdd( const Pnt2i& lhs, const Pnt2i& rhs );

	Int32			getAlignedInt( Int32 x, Int32 n, bool roundUp );
	Pnt2i			alignPoint( const Pnt2i& p, Int32 n, bool roundUp );

	template< typename T, typename VS > struct BoundingBox
	{
		typedef OSG::PointInterface< T, VS >	PntType;

		PntType	min;
		PntType	max;
	};

	typedef BoundingBox< OSG::Real32, OSG::VecStorage3< OSG::Real32 > > BoundingBox3f;

	//-------------------------------------------------------------------------------------------------
}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef OSG_TYPES_INCLUDED

//-------------------------------------------------------------------------------------------------
