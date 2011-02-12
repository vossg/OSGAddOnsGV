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
#include "OSGDynamics_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "VRMLScalarInterpolator.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_VRMLScalarInterpolator_class(){

    { //::OSG::VRMLScalarInterpolator
        typedef bp::class_< OSG::VRMLScalarInterpolator, bp::bases< OSG::VRMLScalarInterpolatorBase >, OSG::VRMLScalarInterpolator::ObjRecPtr, boost::noncopyable > VRMLScalarInterpolator_exposer_t;
        VRMLScalarInterpolator_exposer_t VRMLScalarInterpolator_exposer = VRMLScalarInterpolator_exposer_t( "VRMLScalarInterpolator", bp::no_init );
        bp::scope VRMLScalarInterpolator_scope( VRMLScalarInterpolator_exposer );
        { //::OSG::VRMLScalarInterpolator::changed
        
            typedef void ( ::OSG::VRMLScalarInterpolator::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            VRMLScalarInterpolator_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::VRMLScalarInterpolator::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::VRMLScalarInterpolator::dump
        
            typedef void ( ::OSG::VRMLScalarInterpolator::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            VRMLScalarInterpolator_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::VRMLScalarInterpolator::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        pyopensg::register_transit< OSG::VRMLScalarInterpolator >::execute();
        bp::implicitly_convertible< OSG::VRMLScalarInterpolator::ObjRecPtr, OSG::VRMLScalarInterpolator* >();
        bp::implicitly_convertible< OSG::VRMLScalarInterpolator::ObjRecPtr, OSG::VRMLScalarInterpolator::ObjCPtr >();
        bp::implicitly_convertible< OSG::VRMLScalarInterpolator::ObjRecPtr, OSG::VRMLInterpolator* >();
        bp::implicitly_convertible< OSG::VRMLScalarInterpolator::ObjRecPtr, OSG::VRMLInterpolator::ObjRecPtr >();
        bp::implicitly_convertible<OSG::VRMLScalarInterpolator::ObjRecPtr, OSG::VRMLInterpolator::ObjCPtr>();
    }

}
