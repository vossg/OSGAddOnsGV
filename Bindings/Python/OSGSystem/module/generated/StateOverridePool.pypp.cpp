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
#include "OSGSystem_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "StateOverridePool.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_StateOverridePool_class(){

    { //::OSG::SimplePool< OSG::StateOverride, OSG::PoolDefaultTag, OSG::NoRefCountPolicy, OSG::NoLockPolicy >
        typedef bp::class_< OSG::SimplePool< OSG::StateOverride, OSG::PoolDefaultTag, OSG::NoRefCountPolicy, OSG::NoLockPolicy >, boost::noncopyable > StateOverridePool_exposer_t;
        StateOverridePool_exposer_t StateOverridePool_exposer = StateOverridePool_exposer_t( "StateOverridePool", bp::init< >() );
        bp::scope StateOverridePool_scope( StateOverridePool_exposer );
        { //::OSG::SimplePool< OSG::StateOverride, OSG::PoolDefaultTag, OSG::NoRefCountPolicy, OSG::NoLockPolicy >::create
        
            typedef OSG::SimplePool< OSG::StateOverride, OSG::PoolDefaultTag, OSG::NoRefCountPolicy, OSG::NoLockPolicy > exported_class_t;
            typedef ::OSG::StateOverride * ( exported_class_t::*create_function_type )(  ) ;
            
            StateOverridePool_exposer.def( 
                "create"
                , create_function_type( &::OSG::SimplePool< OSG::StateOverride, OSG::PoolDefaultTag, OSG::NoRefCountPolicy, OSG::NoLockPolicy >::create )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimplePool< OSG::StateOverride, OSG::PoolDefaultTag, OSG::NoRefCountPolicy, OSG::NoLockPolicy >::destroyAll
        
            typedef OSG::SimplePool< OSG::StateOverride, OSG::PoolDefaultTag, OSG::NoRefCountPolicy, OSG::NoLockPolicy > exported_class_t;
            typedef void ( exported_class_t::*destroyAll_function_type )(  ) ;
            
            StateOverridePool_exposer.def( 
                "destroyAll"
                , destroyAll_function_type( &::OSG::SimplePool< OSG::StateOverride, OSG::PoolDefaultTag, OSG::NoRefCountPolicy, OSG::NoLockPolicy >::destroyAll ) );
        
        }
    }

}
