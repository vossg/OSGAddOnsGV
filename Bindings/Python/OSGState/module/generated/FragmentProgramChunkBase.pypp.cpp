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
#include "FragmentProgramChunkBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_FragmentProgramChunkBase_class(){

    { //::OSG::FragmentProgramChunkBase
        typedef bp::class_< OSG::FragmentProgramChunkBase, bp::bases< OSG::ProgramChunk >, boost::noncopyable > FragmentProgramChunkBase_exposer_t;
        FragmentProgramChunkBase_exposer_t FragmentProgramChunkBase_exposer = FragmentProgramChunkBase_exposer_t( "FragmentProgramChunkBase", bp::no_init );
        bp::scope FragmentProgramChunkBase_scope( FragmentProgramChunkBase_exposer );
        { //::OSG::FragmentProgramChunkBase::copyFromBin
        
            typedef void ( ::OSG::FragmentProgramChunkBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            FragmentProgramChunkBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::FragmentProgramChunkBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::FragmentProgramChunkBase::copyToBin
        
            typedef void ( ::OSG::FragmentProgramChunkBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            FragmentProgramChunkBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::FragmentProgramChunkBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::FragmentProgramChunkBase::create
        
            typedef ::OSG::FragmentProgramChunkTransitPtr ( *create_function_type )(  );
            
            FragmentProgramChunkBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::FragmentProgramChunkBase::create ) );
        
        }
        { //::OSG::FragmentProgramChunkBase::createDependent
        
            typedef ::OSG::FragmentProgramChunkTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            FragmentProgramChunkBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::FragmentProgramChunkBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::FragmentProgramChunkBase::createLocal
        
            typedef ::OSG::FragmentProgramChunkTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            FragmentProgramChunkBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::FragmentProgramChunkBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::FragmentProgramChunkBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::FragmentProgramChunkBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            FragmentProgramChunkBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::FragmentProgramChunkBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::FragmentProgramChunkBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            FragmentProgramChunkBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::FragmentProgramChunkBase::getClassGroupId ) );
        
        }
        { //::OSG::FragmentProgramChunkBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            FragmentProgramChunkBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::FragmentProgramChunkBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::FragmentProgramChunkBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            FragmentProgramChunkBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::FragmentProgramChunkBase::getClassTypeId ) );
        
        }
        { //::OSG::FragmentProgramChunkBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::FragmentProgramChunkBase::*getContainerSize_function_type )(  ) const;
            
            FragmentProgramChunkBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::FragmentProgramChunkBase::getContainerSize ) );
        
        }
        { //::OSG::FragmentProgramChunkBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::FragmentProgramChunkBase::*getType_function_type )(  ) ;
            
            FragmentProgramChunkBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::FragmentProgramChunkBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::FragmentProgramChunkBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::FragmentProgramChunkBase::*getType_function_type )(  ) const;
            
            FragmentProgramChunkBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::FragmentProgramChunkBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        FragmentProgramChunkBase_exposer.staticmethod( "create" );
        FragmentProgramChunkBase_exposer.staticmethod( "createDependent" );
        FragmentProgramChunkBase_exposer.staticmethod( "createLocal" );
        FragmentProgramChunkBase_exposer.staticmethod( "getClassGroupId" );
        FragmentProgramChunkBase_exposer.staticmethod( "getClassType" );
        FragmentProgramChunkBase_exposer.staticmethod( "getClassTypeId" );
    }

}
