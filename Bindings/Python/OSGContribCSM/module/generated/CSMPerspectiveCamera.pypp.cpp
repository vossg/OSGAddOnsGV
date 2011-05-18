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
#include "OSGContribCSM_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "CSMPerspectiveCamera.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_CSMPerspectiveCamera_class(){

    { //::OSG::CSMPerspectiveCamera
        typedef bp::class_< OSG::CSMPerspectiveCamera, bp::bases< OSG::CSMPerspectiveCameraBase >, OSG::CSMPerspectiveCamera::ObjRecPtr, boost::noncopyable > CSMPerspectiveCamera_exposer_t;
        CSMPerspectiveCamera_exposer_t CSMPerspectiveCamera_exposer = CSMPerspectiveCamera_exposer_t( "CSMPerspectiveCamera", bp::no_init );
        bp::scope CSMPerspectiveCamera_scope( CSMPerspectiveCamera_exposer );
        { //::OSG::CSMPerspectiveCamera::changed
        
            typedef void ( ::OSG::CSMPerspectiveCamera::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            CSMPerspectiveCamera_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::CSMPerspectiveCamera::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::CSMPerspectiveCamera::dump
        
            typedef void ( ::OSG::CSMPerspectiveCamera::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            CSMPerspectiveCamera_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::CSMPerspectiveCamera::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        pyopensg::register_transit< OSG::CSMPerspectiveCamera >::execute();
        bp::implicitly_convertible< OSG::CSMPerspectiveCamera::ObjRecPtr, OSG::CSMPerspectiveCamera* >();
        bp::implicitly_convertible< OSG::CSMPerspectiveCamera::ObjRecPtr, OSG::CSMPerspectiveCamera::ObjCPtr >();
        bp::implicitly_convertible< OSG::CSMPerspectiveCamera::ObjRecPtr, OSG::PerspectiveCamera* >();
        bp::implicitly_convertible< OSG::CSMPerspectiveCamera::ObjRecPtr, OSG::PerspectiveCamera::ObjRecPtr >();
        bp::implicitly_convertible<OSG::CSMPerspectiveCamera::ObjRecPtr, OSG::PerspectiveCamera::ObjCPtr>();
    }

}