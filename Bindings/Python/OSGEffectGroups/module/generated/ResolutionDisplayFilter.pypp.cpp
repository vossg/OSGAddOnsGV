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
#include "OSGEffectGroups_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "ResolutionDisplayFilter.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_ResolutionDisplayFilter_class(){

    { //::OSG::ResolutionDisplayFilter
        typedef bp::class_< OSG::ResolutionDisplayFilter, bp::bases< OSG::ResolutionDisplayFilterBase >, OSG::ResolutionDisplayFilter::ObjRecPtr, boost::noncopyable > ResolutionDisplayFilter_exposer_t;
        ResolutionDisplayFilter_exposer_t ResolutionDisplayFilter_exposer = ResolutionDisplayFilter_exposer_t( "ResolutionDisplayFilter", bp::no_init );
        bp::scope ResolutionDisplayFilter_scope( ResolutionDisplayFilter_exposer );
        { //::OSG::ResolutionDisplayFilter::changed
        
            typedef void ( ::OSG::ResolutionDisplayFilter::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            ResolutionDisplayFilter_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::ResolutionDisplayFilter::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::ResolutionDisplayFilter::dump
        
            typedef void ( ::OSG::ResolutionDisplayFilter::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            ResolutionDisplayFilter_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::ResolutionDisplayFilter::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::ResolutionDisplayFilter::process
        
            typedef void ( ::OSG::ResolutionDisplayFilter::*process_function_type )( ::OSG::DisplayFilterStageData * ) ;
            
            ResolutionDisplayFilter_exposer.def( 
                "process"
                , process_function_type( &::OSG::ResolutionDisplayFilter::process )
                , ( bp::arg("pData") ) );
        
        }
        pyopensg::register_transit< OSG::ResolutionDisplayFilter >::execute();
        bp::implicitly_convertible< OSG::ResolutionDisplayFilter::ObjRecPtr, OSG::ResolutionDisplayFilter* >();
        bp::implicitly_convertible< OSG::ResolutionDisplayFilter::ObjRecPtr, OSG::ResolutionDisplayFilter::ObjCPtr >();
        bp::implicitly_convertible< OSG::ResolutionDisplayFilter::ObjRecPtr, OSG::DisplayFilter* >();
        bp::implicitly_convertible< OSG::ResolutionDisplayFilter::ObjRecPtr, OSG::DisplayFilter::ObjRecPtr >();
        bp::implicitly_convertible<OSG::ResolutionDisplayFilter::ObjRecPtr, OSG::DisplayFilter::ObjCPtr>();
    }

}
