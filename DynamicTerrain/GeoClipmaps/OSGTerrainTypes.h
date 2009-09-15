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

#ifndef _OSGTERRAINTYPES_H_
#define _OSGTERRAINTYPES_H_

#include "OSGVector.h"
#include "OSGMatrix.h"
#include "OSGColor.h"
#include "OSGRectangle.h"

OSG_BEGIN_NAMESPACE

#if 0
typedef OSG::PointInterface<OSG::Real32, 
          OSG::VecStorage2< OSG::Real32 > > Pnt2f;  
typedef OSG::PointInterface<OSG::Real32, 
          OSG::VecStorage3< OSG::Real32 > > Pnt3f;  
typedef OSG::PointInterface<OSG::Real32, 
          OSG::VecStorage4< OSG::Real32 > > Pnt4f;
typedef OSG::PointInterface<OSG::Int32, 
          OSG::VecStorage2< OSG::Int32 > >  Pnt2i;

typedef OSG::TransformationMatrix< OSG::Real32 > TransformMatrix;   
#endif

Vec3f           colorToVector( Color3f color );

Pnt2i           componentDivide( const Pnt2i& lhs, const Pnt2i& rhs );

// todo: why does this not work :)
Pnt2i           componentDivide( const Pnt2i& lhs, Int32 rhs );
Pnt2i           componentDivide( const Pnt2i& lhs, Int32 rhs );
Pnt2f           componentDivide( const Pnt2f& lhs, const Pnt2f& rhs );
Pnt3f           componentDivide( const Pnt3f& lhs, const Pnt3f& rhs );
Pnt3f           componentDivide( const Pnt3f& lhs, Real32 rhs );
Pnt2f           componentDivide( const Pnt2f& lhs, Real32 rhs );
Pnt2f           componentMultiply( const Pnt2f& lhs, const Pnt2f& rhs );
Pnt2f           componentMultiply( const Pnt2f& lhs, Real32 rhs );
Pnt2i           componentMultiply( const Pnt2i& lhs, Int32 rhs );
Pnt2i           componentModulo( const Pnt2i& lhs, const Pnt2i& rhs );
Pnt2i           componentModulo( const Pnt2i& lhs, Int32 rhs );
Pnt2i           componentAdd( const Pnt2i& lhs, const Pnt2i& rhs );

Int32           getAlignedInt( Int32 x, Int32 n, bool roundUp );
Pnt2i           alignPoint( const Pnt2i& p, Int32 n, bool roundUp );

template< typename T, UInt32 SIZE > struct BoundingBox
{
    typedef OSG::Point< T, SIZE >    PntType;
    
    PntType min;
    PntType max;
};

typedef BoundingBox<OSG::Real32, 3> BoundingBox3f;

OSG_END_NAMESPACE

#endif /* #ifdef _OSGTERRAINTYPES_H_ */
