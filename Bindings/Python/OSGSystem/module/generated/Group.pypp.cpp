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
#include "Group.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_Group_class(){

    { //::OSG::Group
        typedef bp::class_< OSG::Group, bp::bases< OSG::GroupBase >, OSG::Group::ObjRecPtr, boost::noncopyable > Group_exposer_t;
        Group_exposer_t Group_exposer = Group_exposer_t( "Group", bp::no_init );
        bp::scope Group_scope( Group_exposer );
        { //::OSG::Group::changed
        
            typedef void ( ::OSG::Group::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            Group_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::Group::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("detail") ) );
        
        }
        { //::OSG::Group::dump
        
            typedef void ( ::OSG::Group::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            Group_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::Group::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        pyopensg::register_transit< OSG::Group >::execute();
        bp::implicitly_convertible< OSG::Group::ObjRecPtr, OSG::Group* >();
        bp::implicitly_convertible< OSG::Group::ObjRecPtr, OSG::Group::ObjCPtr >();
        bp::implicitly_convertible< OSG::Group::ObjRecPtr, OSG::NodeCore* >();
        bp::implicitly_convertible< OSG::Group::ObjRecPtr, OSG::NodeCore::ObjRecPtr >();
        bp::implicitly_convertible<OSG::Group::ObjRecPtr, OSG::NodeCore::ObjCPtr>();
    }

}