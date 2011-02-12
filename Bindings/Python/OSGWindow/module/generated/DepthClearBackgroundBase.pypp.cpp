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
#include "OSGWindow_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "DepthClearBackgroundBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

struct DepthClearBackgroundBase_wrapper : OSG::DepthClearBackgroundBase, bp::wrapper< OSG::DepthClearBackgroundBase > {

    virtual void clear( ::OSG::DrawEnv * pEnv ){
        bp::override func_clear = this->get_override( "clear" );
        func_clear( boost::python::ptr(pEnv) );
    }

};

void register_DepthClearBackgroundBase_class(){

    { //::OSG::DepthClearBackgroundBase
        typedef bp::class_< DepthClearBackgroundBase_wrapper, bp::bases< ::OSG::Background >, boost::noncopyable > DepthClearBackgroundBase_exposer_t;
        DepthClearBackgroundBase_exposer_t DepthClearBackgroundBase_exposer = DepthClearBackgroundBase_exposer_t( "DepthClearBackgroundBase", bp::no_init );
        bp::scope DepthClearBackgroundBase_scope( DepthClearBackgroundBase_exposer );
        { //::OSG::DepthClearBackgroundBase::copyFromBin
        
            typedef void ( ::OSG::DepthClearBackgroundBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            DepthClearBackgroundBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::DepthClearBackgroundBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::DepthClearBackgroundBase::copyToBin
        
            typedef void ( ::OSG::DepthClearBackgroundBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            DepthClearBackgroundBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::DepthClearBackgroundBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::DepthClearBackgroundBase::create
        
            typedef ::OSG::DepthClearBackgroundTransitPtr ( *create_function_type )(  );
            
            DepthClearBackgroundBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::DepthClearBackgroundBase::create ) );
        
        }
        { //::OSG::DepthClearBackgroundBase::createDependent
        
            typedef ::OSG::DepthClearBackgroundTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            DepthClearBackgroundBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::DepthClearBackgroundBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::DepthClearBackgroundBase::createLocal
        
            typedef ::OSG::DepthClearBackgroundTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            DepthClearBackgroundBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::DepthClearBackgroundBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::DepthClearBackgroundBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::DepthClearBackgroundBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            DepthClearBackgroundBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::DepthClearBackgroundBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::DepthClearBackgroundBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            DepthClearBackgroundBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::DepthClearBackgroundBase::getClassGroupId ) );
        
        }
        { //::OSG::DepthClearBackgroundBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            DepthClearBackgroundBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::DepthClearBackgroundBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::DepthClearBackgroundBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            DepthClearBackgroundBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::DepthClearBackgroundBase::getClassTypeId ) );
        
        }
        { //::OSG::DepthClearBackgroundBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::DepthClearBackgroundBase::*getContainerSize_function_type )(  ) const;
            
            DepthClearBackgroundBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::DepthClearBackgroundBase::getContainerSize ) );
        
        }
        { //::OSG::DepthClearBackgroundBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::DepthClearBackgroundBase::*getType_function_type )(  ) ;
            
            DepthClearBackgroundBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::DepthClearBackgroundBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::DepthClearBackgroundBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::DepthClearBackgroundBase::*getType_function_type )(  ) const;
            
            DepthClearBackgroundBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::DepthClearBackgroundBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::Background::clear
        
            typedef void ( ::OSG::Background::*clear_function_type )( ::OSG::DrawEnv * ) ;
            
            DepthClearBackgroundBase_exposer.def( 
                "clear"
                , bp::pure_virtual( clear_function_type(&::OSG::Background::clear) )
                , ( bp::arg("pEnv") ) );
        
        }
        DepthClearBackgroundBase_exposer.staticmethod( "create" );
        DepthClearBackgroundBase_exposer.staticmethod( "createDependent" );
        DepthClearBackgroundBase_exposer.staticmethod( "createLocal" );
        DepthClearBackgroundBase_exposer.staticmethod( "getClassGroupId" );
        DepthClearBackgroundBase_exposer.staticmethod( "getClassType" );
        DepthClearBackgroundBase_exposer.staticmethod( "getClassTypeId" );
    }

}
