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
#include "ProgramChunk.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_ProgramChunk_class(){

    { //::OSG::ProgramChunk
        typedef bp::class_< OSG::ProgramChunk, bp::bases< OSG::ProgramChunkBase >, boost::noncopyable > ProgramChunk_exposer_t;
        ProgramChunk_exposer_t ProgramChunk_exposer = ProgramChunk_exposer_t( "ProgramChunk", bp::no_init );
        bp::scope ProgramChunk_scope( ProgramChunk_exposer );
        { //::OSG::ProgramChunk::addParameter
        
            typedef bool ( ::OSG::ProgramChunk::*addParameter_function_type )( char const *,::OSG::Int16 ) ;
            
            ProgramChunk_exposer.def( 
                "addParameter"
                , addParameter_function_type( &::OSG::ProgramChunk::addParameter )
                , ( bp::arg("name"), bp::arg("index") ) );
        
        }
        { //::OSG::ProgramChunk::addParameter
        
            typedef bool ( ::OSG::ProgramChunk::*addParameter_function_type )( char const *,::OSG::Int16,::OSG::Vec4f const & ) ;
            
            ProgramChunk_exposer.def( 
                "addParameter"
                , addParameter_function_type( &::OSG::ProgramChunk::addParameter )
                , ( bp::arg("name"), bp::arg("index"), bp::arg("value") ) );
        
        }
        { //::OSG::ProgramChunk::changed
        
            typedef void ( ::OSG::ProgramChunk::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            ProgramChunk_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::ProgramChunk::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::ProgramChunk::dump
        
            typedef void ( ::OSG::ProgramChunk::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            ProgramChunk_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::ProgramChunk::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::ProgramChunk::findParameter
        
            typedef ::OSG::Int16 ( ::OSG::ProgramChunk::*findParameter_function_type )( char const * ) ;
            
            ProgramChunk_exposer.def( 
                "findParameter"
                , findParameter_function_type( &::OSG::ProgramChunk::findParameter )
                , ( bp::arg("name") ) );
        
        }
        { //::OSG::ProgramChunk::findParameter
        
            typedef ::OSG::Int16 ( ::OSG::ProgramChunk::*findParameter_function_type )( ::std::string const & ) ;
            
            ProgramChunk_exposer.def( 
                "findParameter"
                , findParameter_function_type( &::OSG::ProgramChunk::findParameter )
                , ( bp::arg("name") ) );
        
        }
        { //::OSG::ProgramChunk::getClass
        
            typedef ::OSG::StateChunkClass const * ( ::OSG::ProgramChunk::*getClass_function_type )(  ) const;
            
            ProgramChunk_exposer.def( 
                "getClass"
                , getClass_function_type( &::OSG::ProgramChunk::getClass )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ProgramChunk::getParameter
        
            typedef ::OSG::Vec4f const ( ::OSG::ProgramChunk::*getParameter_function_type )( ::OSG::Int16 ) ;
            
            ProgramChunk_exposer.def( 
                "getParameter"
                , getParameter_function_type( &::OSG::ProgramChunk::getParameter )
                , ( bp::arg("index") ) );
        
        }
        { //::OSG::ProgramChunk::getParameter
        
            typedef ::OSG::Vec4f const ( ::OSG::ProgramChunk::*getParameter_function_type )( char const * ) ;
            
            ProgramChunk_exposer.def( 
                "getParameter"
                , getParameter_function_type( &::OSG::ProgramChunk::getParameter )
                , ( bp::arg("name") ) );
        
        }
        { //::OSG::ProgramChunk::getParameter
        
            typedef ::OSG::Vec4f const ( ::OSG::ProgramChunk::*getParameter_function_type )( ::std::string const & ) ;
            
            ProgramChunk_exposer.def( 
                "getParameter"
                , getParameter_function_type( &::OSG::ProgramChunk::getParameter )
                , ( bp::arg("name") ) );
        
        }
        { //::OSG::ProgramChunk::getStaticClass
        
            typedef ::OSG::StateChunkClass const * ( *getStaticClass_function_type )(  );
            
            ProgramChunk_exposer.def( 
                "getStaticClass"
                , getStaticClass_function_type( &::OSG::ProgramChunk::getStaticClass )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ProgramChunk::getStaticClassId
        
            typedef ::OSG::UInt32 ( *getStaticClassId_function_type )(  );
            
            ProgramChunk_exposer.def( 
                "getStaticClassId"
                , getStaticClassId_function_type( &::OSG::ProgramChunk::getStaticClassId ) );
        
        }
        { //::OSG::ProgramChunk::isTransparent
        
            typedef bool ( ::OSG::ProgramChunk::*isTransparent_function_type )(  ) const;
            
            ProgramChunk_exposer.def( 
                "isTransparent"
                , isTransparent_function_type( &::OSG::ProgramChunk::isTransparent ) );
        
        }
        { //::OSG::ProgramChunk::read
        
            typedef bool ( ::OSG::ProgramChunk::*read_function_type )( char const * ) ;
            
            ProgramChunk_exposer.def( 
                "read"
                , read_function_type( &::OSG::ProgramChunk::read )
                , ( bp::arg("file") ) );
        
        }
        { //::OSG::ProgramChunk::read
        
            typedef bool ( ::OSG::ProgramChunk::*read_function_type )( ::std::istream & ) ;
            
            ProgramChunk_exposer.def( 
                "read"
                , read_function_type( &::OSG::ProgramChunk::read )
                , ( bp::arg("stream") ) );
        
        }
        { //::OSG::ProgramChunk::setParameter
        
            typedef bool ( ::OSG::ProgramChunk::*setParameter_function_type )( ::OSG::Int16,::OSG::Vec4f const & ) ;
            
            ProgramChunk_exposer.def( 
                "setParameter"
                , setParameter_function_type( &::OSG::ProgramChunk::setParameter )
                , ( bp::arg("index"), bp::arg("value") ) );
        
        }
        { //::OSG::ProgramChunk::setParameter
        
            typedef bool ( ::OSG::ProgramChunk::*setParameter_function_type )( char const *,::OSG::Vec4f const & ) ;
            
            ProgramChunk_exposer.def( 
                "setParameter"
                , setParameter_function_type( &::OSG::ProgramChunk::setParameter )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::ProgramChunk::setParameter
        
            typedef bool ( ::OSG::ProgramChunk::*setParameter_function_type )( ::std::string const &,::OSG::Vec4f const & ) ;
            
            ProgramChunk_exposer.def( 
                "setParameter"
                , setParameter_function_type( &::OSG::ProgramChunk::setParameter )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::ProgramChunk::switchCost
        
            typedef ::OSG::Real32 ( ::OSG::ProgramChunk::*switchCost_function_type )( ::OSG::StateChunk * ) ;
            
            ProgramChunk_exposer.def( 
                "switchCost"
                , switchCost_function_type( &::OSG::ProgramChunk::switchCost )
                , ( bp::arg("chunk") ) );
        
        }
        ProgramChunk_exposer.staticmethod( "getStaticClass" );
        ProgramChunk_exposer.staticmethod( "getStaticClassId" );
        pyopensg::register_transit< OSG::ProgramChunk >::execute();
        bp::register_ptr_to_python< OSG::ProgramChunk::ObjRecPtr >();
        bp::implicitly_convertible< OSG::ProgramChunk::ObjRecPtr, OSG::ProgramChunk* >();
        bp::implicitly_convertible< OSG::ProgramChunk::ObjRecPtr, OSG::ProgramChunk::ObjCPtr >();
        bp::implicitly_convertible< OSG::ProgramChunk::ObjRecPtr, OSG::StateChunk* >();
        bp::implicitly_convertible< OSG::ProgramChunk::ObjRecPtr, OSG::StateChunk::ObjRecPtr >();
        bp::implicitly_convertible<OSG::ProgramChunk::ObjRecPtr, OSG::StateChunk::ObjCPtr>();
    }

}
