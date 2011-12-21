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
#include "PostShaderStageBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_PostShaderStageBase_class(){

    { //::OSG::PostShaderStageBase
        typedef bp::class_< OSG::PostShaderStageBase, bp::bases< ::OSG::Stage >, boost::noncopyable > PostShaderStageBase_exposer_t;
        PostShaderStageBase_exposer_t PostShaderStageBase_exposer = PostShaderStageBase_exposer_t( "PostShaderStageBase", bp::no_init );
        bp::scope PostShaderStageBase_scope( PostShaderStageBase_exposer );
        bp::scope().attr("ColorBufferFormatFieldId") = (int)OSG::PostShaderStageBase::ColorBufferFormatFieldId;
        bp::scope().attr("UseColorTextureBufferFieldId") = (int)OSG::PostShaderStageBase::UseColorTextureBufferFieldId;
        bp::scope().attr("UseDepthTextureBufferFieldId") = (int)OSG::PostShaderStageBase::UseDepthTextureBufferFieldId;
        bp::scope().attr("VertexShadersFieldId") = (int)OSG::PostShaderStageBase::VertexShadersFieldId;
        bp::scope().attr("FragmentShadersFieldId") = (int)OSG::PostShaderStageBase::FragmentShadersFieldId;
        bp::scope().attr("PassSizesFieldId") = (int)OSG::PostShaderStageBase::PassSizesFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::PostShaderStageBase::NextFieldId;
        { //::OSG::PostShaderStageBase::copyFromBin
        
            typedef void ( ::OSG::PostShaderStageBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            PostShaderStageBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::PostShaderStageBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::PostShaderStageBase::copyToBin
        
            typedef void ( ::OSG::PostShaderStageBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            PostShaderStageBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::PostShaderStageBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::PostShaderStageBase::create
        
            typedef ::OSG::PostShaderStageTransitPtr ( *create_function_type )(  );
            
            PostShaderStageBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::PostShaderStageBase::create ) );
        
        }
        { //::OSG::PostShaderStageBase::createDependent
        
            typedef ::OSG::PostShaderStageTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            PostShaderStageBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::PostShaderStageBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::PostShaderStageBase::createLocal
        
            typedef ::OSG::PostShaderStageTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            PostShaderStageBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::PostShaderStageBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::PostShaderStageBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::PostShaderStageBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            PostShaderStageBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::PostShaderStageBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::PostShaderStageBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            PostShaderStageBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::PostShaderStageBase::getClassGroupId ) );
        
        }
        { //::OSG::PostShaderStageBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            PostShaderStageBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::PostShaderStageBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::PostShaderStageBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            PostShaderStageBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::PostShaderStageBase::getClassTypeId ) );
        
        }
        { //::OSG::PostShaderStageBase::getColorBufferFormat
        
            typedef ::GLenum const & ( ::OSG::PostShaderStageBase::*getColorBufferFormat_function_type )(  ) const;
            
            PostShaderStageBase_exposer.def( 
                "getColorBufferFormat"
                , getColorBufferFormat_function_type( &::OSG::PostShaderStageBase::getColorBufferFormat )
                , bp::return_value_policy< bp::return_by_value >() );
        
        }
        { //::OSG::PostShaderStageBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::PostShaderStageBase::*getContainerSize_function_type )(  ) const;
            
            PostShaderStageBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::PostShaderStageBase::getContainerSize ) );
        
        }
        { //::OSG::PostShaderStageBase::getSFColorBufferFormat
        
            typedef ::OSG::SFGLenum const * ( ::OSG::PostShaderStageBase::*getSFColorBufferFormat_function_type )(  ) const;
            
            PostShaderStageBase_exposer.def( 
                "getSFColorBufferFormat"
                , getSFColorBufferFormat_function_type( &::OSG::PostShaderStageBase::getSFColorBufferFormat )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PostShaderStageBase::getSFUseColorTextureBuffer
        
            typedef ::OSG::SFBool const * ( ::OSG::PostShaderStageBase::*getSFUseColorTextureBuffer_function_type )(  ) const;
            
            PostShaderStageBase_exposer.def( 
                "getSFUseColorTextureBuffer"
                , getSFUseColorTextureBuffer_function_type( &::OSG::PostShaderStageBase::getSFUseColorTextureBuffer )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PostShaderStageBase::getSFUseDepthTextureBuffer
        
            typedef ::OSG::SFBool const * ( ::OSG::PostShaderStageBase::*getSFUseDepthTextureBuffer_function_type )(  ) const;
            
            PostShaderStageBase_exposer.def( 
                "getSFUseDepthTextureBuffer"
                , getSFUseDepthTextureBuffer_function_type( &::OSG::PostShaderStageBase::getSFUseDepthTextureBuffer )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PostShaderStageBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::PostShaderStageBase::*getType_function_type )(  ) ;
            
            PostShaderStageBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::PostShaderStageBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PostShaderStageBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::PostShaderStageBase::*getType_function_type )(  ) const;
            
            PostShaderStageBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::PostShaderStageBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PostShaderStageBase::getUseColorTextureBuffer
        
            typedef bool ( ::OSG::PostShaderStageBase::*getUseColorTextureBuffer_function_type )(  ) const;
            
            PostShaderStageBase_exposer.def( 
                "getUseColorTextureBuffer"
                , getUseColorTextureBuffer_function_type( &::OSG::PostShaderStageBase::getUseColorTextureBuffer ) );
        
        }
        { //::OSG::PostShaderStageBase::getUseDepthTextureBuffer
        
            typedef bool ( ::OSG::PostShaderStageBase::*getUseDepthTextureBuffer_function_type )(  ) const;
            
            PostShaderStageBase_exposer.def( 
                "getUseDepthTextureBuffer"
                , getUseDepthTextureBuffer_function_type( &::OSG::PostShaderStageBase::getUseDepthTextureBuffer ) );
        
        }
        { //::OSG::PostShaderStageBase::setColorBufferFormat
        
            typedef void ( ::OSG::PostShaderStageBase::*setColorBufferFormat_function_type )( ::GLenum const & ) ;
            
            PostShaderStageBase_exposer.def( 
                "setColorBufferFormat"
                , setColorBufferFormat_function_type( &::OSG::PostShaderStageBase::setColorBufferFormat )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::PostShaderStageBase::setUseColorTextureBuffer
        
            typedef void ( ::OSG::PostShaderStageBase::*setUseColorTextureBuffer_function_type )( bool const ) ;
            
            PostShaderStageBase_exposer.def( 
                "setUseColorTextureBuffer"
                , setUseColorTextureBuffer_function_type( &::OSG::PostShaderStageBase::setUseColorTextureBuffer )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::PostShaderStageBase::setUseDepthTextureBuffer
        
            typedef void ( ::OSG::PostShaderStageBase::*setUseDepthTextureBuffer_function_type )( bool const ) ;
            
            PostShaderStageBase_exposer.def( 
                "setUseDepthTextureBuffer"
                , setUseDepthTextureBuffer_function_type( &::OSG::PostShaderStageBase::setUseDepthTextureBuffer )
                , ( bp::arg("value") ) );
        
        }
        PostShaderStageBase_exposer.staticmethod( "create" );
        PostShaderStageBase_exposer.staticmethod( "createDependent" );
        PostShaderStageBase_exposer.staticmethod( "createLocal" );
        PostShaderStageBase_exposer.staticmethod( "getClassGroupId" );
        PostShaderStageBase_exposer.staticmethod( "getClassType" );
        PostShaderStageBase_exposer.staticmethod( "getClassTypeId" );
    }

}
