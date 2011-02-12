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
#include "FullStateChunk.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_FullStateChunk_class(){

    { //::OSG::FullStateChunk
        typedef bp::class_< OSG::FullStateChunk, bp::bases< OSG::FullStateChunkBase >, boost::noncopyable > FullStateChunk_exposer_t;
        FullStateChunk_exposer_t FullStateChunk_exposer = FullStateChunk_exposer_t( "FullStateChunk", bp::no_init );
        bp::scope FullStateChunk_scope( FullStateChunk_exposer );
        { //::OSG::FullStateChunk::changed
        
            typedef void ( ::OSG::FullStateChunk::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            FullStateChunk_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::FullStateChunk::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::FullStateChunk::dump
        
            typedef void ( ::OSG::FullStateChunk::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            FullStateChunk_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::FullStateChunk::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::FullStateChunk::getClass
        
            typedef ::OSG::StateChunkClass const * ( ::OSG::FullStateChunk::*getClass_function_type )(  ) const;
            
            FullStateChunk_exposer.def( 
                "getClass"
                , getClass_function_type( &::OSG::FullStateChunk::getClass )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::FullStateChunk::getStaticClass
        
            typedef ::OSG::StateChunkClass const * ( *getStaticClass_function_type )(  );
            
            FullStateChunk_exposer.def( 
                "getStaticClass"
                , getStaticClass_function_type( &::OSG::FullStateChunk::getStaticClass )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::FullStateChunk::getStaticClassId
        
            typedef ::OSG::UInt32 ( *getStaticClassId_function_type )(  );
            
            FullStateChunk_exposer.def( 
                "getStaticClassId"
                , getStaticClassId_function_type( &::OSG::FullStateChunk::getStaticClassId ) );
        
        }
        { //::OSG::FullStateChunk::switchCost
        
            typedef ::OSG::Real32 ( ::OSG::FullStateChunk::*switchCost_function_type )( ::OSG::StateChunk * ) ;
            
            FullStateChunk_exposer.def( 
                "switchCost"
                , switchCost_function_type( &::OSG::FullStateChunk::switchCost )
                , ( bp::arg("chunk") ) );
        
        }
        FullStateChunk_exposer.staticmethod( "getStaticClass" );
        FullStateChunk_exposer.staticmethod( "getStaticClassId" );
        pyopensg::register_transit< OSG::FullStateChunk >::execute();
        bp::register_ptr_to_python< OSG::FullStateChunk::ObjRecPtr >();
        bp::implicitly_convertible< OSG::FullStateChunk::ObjRecPtr, OSG::FullStateChunk* >();
        bp::implicitly_convertible< OSG::FullStateChunk::ObjRecPtr, OSG::FullStateChunk::ObjCPtr >();
        bp::implicitly_convertible< OSG::FullStateChunk::ObjRecPtr, OSG::StateChunk* >();
        bp::implicitly_convertible< OSG::FullStateChunk::ObjRecPtr, OSG::StateChunk::ObjRecPtr >();
        bp::implicitly_convertible<OSG::FullStateChunk::ObjRecPtr, OSG::StateChunk::ObjCPtr>();
    }

}
