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
#include "OSGState_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "FragmentProgramChunk.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_FragmentProgramChunk_class(){

    { //::OSG::FragmentProgramChunk
        typedef bp::class_< OSG::FragmentProgramChunk, bp::bases< OSG::FragmentProgramChunkBase >, OSG::FragmentProgramChunk::ObjRecPtr, boost::noncopyable > FragmentProgramChunk_exposer_t;
        FragmentProgramChunk_exposer_t FragmentProgramChunk_exposer = FragmentProgramChunk_exposer_t( "FragmentProgramChunk", bp::no_init );
        bp::scope FragmentProgramChunk_scope( FragmentProgramChunk_exposer );
        { //::OSG::FragmentProgramChunk::changed
        
            typedef void ( ::OSG::FragmentProgramChunk::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            FragmentProgramChunk_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::FragmentProgramChunk::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::FragmentProgramChunk::dump
        
            typedef void ( ::OSG::FragmentProgramChunk::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            FragmentProgramChunk_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::FragmentProgramChunk::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::FragmentProgramChunk::getClass
        
            typedef ::OSG::StateChunkClass const * ( ::OSG::FragmentProgramChunk::*getClass_function_type )(  ) const;
            
            FragmentProgramChunk_exposer.def( 
                "getClass"
                , getClass_function_type( &::OSG::FragmentProgramChunk::getClass )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::FragmentProgramChunk::getStaticClass
        
            typedef ::OSG::StateChunkClass const * ( *getStaticClass_function_type )(  );
            
            FragmentProgramChunk_exposer.def( 
                "getStaticClass"
                , getStaticClass_function_type( &::OSG::FragmentProgramChunk::getStaticClass )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::FragmentProgramChunk::getStaticClassId
        
            typedef ::OSG::UInt32 ( *getStaticClassId_function_type )(  );
            
            FragmentProgramChunk_exposer.def( 
                "getStaticClassId"
                , getStaticClassId_function_type( &::OSG::FragmentProgramChunk::getStaticClassId ) );
        
        }
        FragmentProgramChunk_exposer.staticmethod( "getStaticClass" );
        FragmentProgramChunk_exposer.staticmethod( "getStaticClassId" );
        pyopensg::register_transit< OSG::FragmentProgramChunk >::execute();
        bp::implicitly_convertible< OSG::FragmentProgramChunk::ObjRecPtr, OSG::FragmentProgramChunk* >();
        bp::implicitly_convertible< OSG::FragmentProgramChunk::ObjRecPtr, OSG::FragmentProgramChunk::ObjCPtr >();
        bp::implicitly_convertible< OSG::FragmentProgramChunk::ObjRecPtr, OSG::ProgramChunk* >();
        bp::implicitly_convertible< OSG::FragmentProgramChunk::ObjRecPtr, OSG::ProgramChunk::ObjRecPtr >();
        bp::implicitly_convertible<OSG::FragmentProgramChunk::ObjRecPtr, OSG::ProgramChunk::ObjCPtr>();
    }

}
