// This file has been generated by Py++.


// PyOpenSG is (C) Copyright 2005-2009 by Allen Bierbaum
//
// This file is part of PyOpenSG.
//
// PyOpenSG is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your option)
// any later version.
//
// PyOpenSG is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#if __GNUC__ >= 4 || __GNUC_MINOR__ >=3
#pragma GCC diagnostic warning "-Wold-style-cast"
#endif

#include "boost/python.hpp"
#include "OSGBase_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "Vec4f.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_Vec4f_class(){

    { //::OSG::Vector< float, 4u >
        typedef bp::class_< OSG::Vector< float, 4u >, bp::bases< OSG::Point< float, 4u > > > Vec4f_exposer_t;
        Vec4f_exposer_t Vec4f_exposer = Vec4f_exposer_t( "Vec4f", bp::init< >() );
        bp::scope Vec4f_scope( Vec4f_exposer );
        Vec4f_exposer.def( bp::init< float const * >(( bp::arg("pVals") )) );
        bp::implicitly_convertible< float const *, OSG::Vector< float, 4u > >();
        Vec4f_exposer.def( bp::init< float * >(( bp::arg("pVals") )) );
        bp::implicitly_convertible< float *, OSG::Vector< float, 4u > >();
        Vec4f_exposer.def( bp::init< OSG::Vector< float, 4u > const & >(( bp::arg("source") )) );
        Vec4f_exposer.def( bp::init< float >(( bp::arg("rVal1") )) );
        bp::implicitly_convertible< float const, OSG::Vector< float, 4u > >();
        Vec4f_exposer.def( bp::init< float, float >(( bp::arg("rVal1"), bp::arg("rVal2") )) );
        Vec4f_exposer.def( bp::init< float, float, float >(( bp::arg("rVal1"), bp::arg("rVal2"), bp::arg("rVal3") )) );
        Vec4f_exposer.def( bp::init< float, float, float, float >(( bp::arg("rVal1"), bp::arg("rVal2"), bp::arg("rVal3"), bp::arg("rVal4") )) );
        { //::OSG::Vector< float, 4u >::addToZero
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Point< float, 4u > const & ( exported_class_t::*addToZero_function_type )(  ) const;
            
            Vec4f_exposer.def( 
                "addToZero"
                , addToZero_function_type( &::OSG::Vector< float, 4u >::addToZero )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::Vector< float, 4u >::addToZero
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Point< float, 4u > & ( exported_class_t::*addToZero_function_type )(  ) ;
            
            Vec4f_exposer.def( 
                "addToZero"
                , addToZero_function_type( &::OSG::Vector< float, 4u >::addToZero )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::Vector< float, 4u >::cross
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Vector< float, 4u > ( exported_class_t::*cross_function_type )( ::OSG::Vector< float, 4u > const & ) const;
            
            Vec4f_exposer.def( 
                "cross"
                , cross_function_type( &::OSG::Vector< float, 4u >::cross )
                , ( bp::arg("vec") ) );
        
        }
        { //::OSG::Vector< float, 4u >::crossThis
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef void ( exported_class_t::*crossThis_function_type )( ::OSG::Vector< float, 4u > const & ) ;
            
            Vec4f_exposer.def( 
                "crossThis"
                , crossThis_function_type( &::OSG::Vector< float, 4u >::crossThis )
                , ( bp::arg("vec") ) );
        
        }
        { //::OSG::Vector< float, 4u >::dot
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef float ( exported_class_t::*dot_function_type )( ::OSG::Vector< float, 4u > const & ) const;
            
            Vec4f_exposer.def( 
                "dot"
                , dot_function_type( &::OSG::Vector< float, 4u >::dot )
                , ( bp::arg("vec") ) );
        
        }
        { //::OSG::Vector< float, 4u >::dot
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef float ( exported_class_t::*dot_function_type )( ::OSG::Point< float, 4u > const & ) const;
            
            Vec4f_exposer.def( 
                "dot"
                , dot_function_type( &::OSG::Vector< float, 4u >::dot )
                , ( bp::arg("pnt") ) );
        
        }
        { //::OSG::Vector< float, 4u >::enclosedAngle
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Real32 ( exported_class_t::*enclosedAngle_function_type )( ::OSG::Vector< float, 4u > const & ) const;
            
            Vec4f_exposer.def( 
                "enclosedAngle"
                , enclosedAngle_function_type( &::OSG::Vector< float, 4u >::enclosedAngle )
                , ( bp::arg("vec") ) );
        
        }
        { //::OSG::Vector< float, 4u >::length
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Real32 ( exported_class_t::*length_function_type )(  ) const;
            
            Vec4f_exposer.def( 
                "length"
                , length_function_type( &::OSG::Vector< float, 4u >::length ) );
        
        }
        { //::OSG::Vector< float, 4u >::normalize
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef void ( exported_class_t::*normalize_function_type )(  ) ;
            
            Vec4f_exposer.def( 
                "normalize"
                , normalize_function_type( &::OSG::Vector< float, 4u >::normalize ) );
        
        }
        Vec4f_exposer.def( bp::self != bp::self );
        Vec4f_exposer.def( bp::self % bp::self );
        Vec4f_exposer.def( bp::self * bp::self );
        Vec4f_exposer.def( bp::self * bp::other< OSG::Point< float, 4u > >() );
        Vec4f_exposer.def( bp::self * bp::other< float >() );
        Vec4f_exposer.def( bp::self + bp::self );
        Vec4f_exposer.def( bp::self - bp::self );
        Vec4f_exposer.def( -bp::self );
        Vec4f_exposer.def( bp::self / bp::other< float >() );
        Vec4f_exposer.def( bp::self < bp::self );
        Vec4f_exposer.def( bp::self == bp::self );
        { //::OSG::Vector< float, 4u >::projectTo
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Real32 ( exported_class_t::*projectTo_function_type )( ::OSG::Vector< float, 4u > const & ) ;
            
            Vec4f_exposer.def( 
                "projectTo"
                , projectTo_function_type( &::OSG::Vector< float, 4u >::projectTo )
                , ( bp::arg("toVec") ) );
        
        }
        { //::OSG::Vector< float, 4u >::reflect
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Vector< float, 4u > ( exported_class_t::*reflect_function_type )( ::OSG::Vector< float, 4u > const & ) const;
            
            Vec4f_exposer.def( 
                "reflect"
                , reflect_function_type( &::OSG::Vector< float, 4u >::reflect )
                , ( bp::arg("Normal") ) );
        
        }
        { //::OSG::Vector< float, 4u >::squareLength
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Real32 ( exported_class_t::*squareLength_function_type )(  ) const;
            
            Vec4f_exposer.def( 
                "squareLength"
                , squareLength_function_type( &::OSG::Vector< float, 4u >::squareLength ) );
        
        }
        { //::OSG::Vector< float, 4u >::subZero
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Vector< float, 4u > & ( exported_class_t::*subZero_function_type )(  ) ;
            
            Vec4f_exposer.def( 
                "subZero"
                , subZero_function_type( &::OSG::Vector< float, 4u >::subZero )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::Vector< float, 4u >::subZero
        
            typedef OSG::Vector< float, 4u > exported_class_t;
            typedef ::OSG::Vector< float, 4u > const & ( exported_class_t::*subZero_function_type )(  ) const;
            
            Vec4f_exposer.def( 
                "subZero"
                , subZero_function_type( &::OSG::Vector< float, 4u >::subZero )
                , bp::return_internal_reference< >() );
        
        }
        Vec4f_exposer.def("__setitem__",osgwrap::setArrayElement<OSG::Vec4f,OSG::Vec4f::ValueType>);
        Vec4f_exposer.def(boost::python::self_ns::str(boost::python::self));
    }

}