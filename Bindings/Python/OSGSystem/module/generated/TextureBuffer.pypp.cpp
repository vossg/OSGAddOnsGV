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
#include "TextureBuffer.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_TextureBuffer_class(){

    { //::OSG::TextureBuffer
        typedef bp::class_< OSG::TextureBuffer, bp::bases< OSG::TextureBufferBase >, OSG::TextureBuffer::ObjRecPtr, boost::noncopyable > TextureBuffer_exposer_t;
        TextureBuffer_exposer_t TextureBuffer_exposer = TextureBuffer_exposer_t( "TextureBuffer", bp::no_init );
        bp::scope TextureBuffer_scope( TextureBuffer_exposer );
        { //::OSG::TextureBuffer::bind
        
            typedef void ( ::OSG::TextureBuffer::*bind_function_type )( ::OSG::DrawEnv *,::OSG::UInt32 ) ;
            
            TextureBuffer_exposer.def( 
                "bind"
                , bind_function_type( &::OSG::TextureBuffer::bind )
                , ( bp::arg("pEnv"), bp::arg("index")=(::OSG::UInt32)(0) ) );
        
        }
        { //::OSG::TextureBuffer::changed
        
            typedef void ( ::OSG::TextureBuffer::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            TextureBuffer_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::TextureBuffer::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("detail") ) );
        
        }
        { //::OSG::TextureBuffer::dump
        
            typedef void ( ::OSG::TextureBuffer::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            TextureBuffer_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::TextureBuffer::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::TextureBuffer::processPostDeactivate
        
            typedef void ( ::OSG::TextureBuffer::*processPostDeactivate_function_type )( ::OSG::DrawEnv * ) ;
            
            TextureBuffer_exposer.def( 
                "processPostDeactivate"
                , processPostDeactivate_function_type( &::OSG::TextureBuffer::processPostDeactivate )
                , ( bp::arg("pEnv") ) );
        
        }
        { //::OSG::TextureBuffer::processPreDeactivate
        
            typedef void ( ::OSG::TextureBuffer::*processPreDeactivate_function_type )( ::OSG::DrawEnv *,::OSG::UInt32 ) ;
            
            TextureBuffer_exposer.def( 
                "processPreDeactivate"
                , processPreDeactivate_function_type( &::OSG::TextureBuffer::processPreDeactivate )
                , ( bp::arg("pEnv"), bp::arg("index") ) );
        
        }
        { //::OSG::TextureBuffer::validate
        
            typedef void ( ::OSG::TextureBuffer::*validate_function_type )( ::OSG::DrawEnv * ) ;
            
            TextureBuffer_exposer.def( 
                "validate"
                , validate_function_type( &::OSG::TextureBuffer::validate )
                , ( bp::arg("pEnv") ) );
        
        }
        pyopensg::register_transit< OSG::TextureBuffer >::execute();
        bp::implicitly_convertible< OSG::TextureBuffer::ObjRecPtr, OSG::TextureBuffer* >();
        bp::implicitly_convertible< OSG::TextureBuffer::ObjRecPtr, OSG::TextureBuffer::ObjCPtr >();
        bp::implicitly_convertible< OSG::TextureBuffer::ObjRecPtr, OSG::FrameBufferAttachment* >();
        bp::implicitly_convertible< OSG::TextureBuffer::ObjRecPtr, OSG::FrameBufferAttachment::ObjRecPtr >();
        bp::implicitly_convertible<OSG::TextureBuffer::ObjRecPtr, OSG::FrameBufferAttachment::ObjCPtr>();
    }

}