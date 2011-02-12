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
#include "OSGGroup_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "PointLightBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

struct PointLightBase_wrapper : OSG::PointLightBase, bp::wrapper< OSG::PointLightBase > {

    virtual void callLightEngineEnter( ::OSG::RenderAction * ract ){
        bp::override func_callLightEngineEnter = this->get_override( "callLightEngineEnter" );
        func_callLightEngineEnter( boost::python::ptr(ract) );
    }

    virtual void callLightEngineLeave( ::OSG::RenderAction * ract ){
        bp::override func_callLightEngineLeave = this->get_override( "callLightEngineLeave" );
        func_callLightEngineLeave( boost::python::ptr(ract) );
    }

};

void register_PointLightBase_class(){

    { //::OSG::PointLightBase
        typedef bp::class_< PointLightBase_wrapper, bp::bases< ::OSG::Light >, boost::noncopyable > PointLightBase_exposer_t;
        PointLightBase_exposer_t PointLightBase_exposer = PointLightBase_exposer_t( "PointLightBase", bp::no_init );
        bp::scope PointLightBase_scope( PointLightBase_exposer );
        bp::scope().attr("PositionFieldId") = (int)OSG::PointLightBase::PositionFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::PointLightBase::NextFieldId;
        { //::OSG::PointLightBase::copyFromBin
        
            typedef void ( ::OSG::PointLightBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            PointLightBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::PointLightBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::PointLightBase::copyToBin
        
            typedef void ( ::OSG::PointLightBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            PointLightBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::PointLightBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::PointLightBase::create
        
            typedef ::OSG::PointLightTransitPtr ( *create_function_type )(  );
            
            PointLightBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::PointLightBase::create ) );
        
        }
        { //::OSG::PointLightBase::createDependent
        
            typedef ::OSG::PointLightTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            PointLightBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::PointLightBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::PointLightBase::createLocal
        
            typedef ::OSG::PointLightTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            PointLightBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::PointLightBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::PointLightBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::PointLightBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            PointLightBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::PointLightBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::PointLightBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            PointLightBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::PointLightBase::getClassGroupId ) );
        
        }
        { //::OSG::PointLightBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            PointLightBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::PointLightBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::PointLightBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            PointLightBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::PointLightBase::getClassTypeId ) );
        
        }
        { //::OSG::PointLightBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::PointLightBase::*getContainerSize_function_type )(  ) const;
            
            PointLightBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::PointLightBase::getContainerSize ) );
        
        }
        { //::OSG::PointLightBase::getPosition
        
            typedef ::OSG::Pnt3f const & ( ::OSG::PointLightBase::*getPosition_function_type )(  ) const;
            
            PointLightBase_exposer.def( 
                "getPosition"
                , getPosition_function_type( &::OSG::PointLightBase::getPosition )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::PointLightBase::getSFPosition
        
            typedef ::OSG::SFPnt3f const * ( ::OSG::PointLightBase::*getSFPosition_function_type )(  ) const;
            
            PointLightBase_exposer.def( 
                "getSFPosition"
                , getSFPosition_function_type( &::OSG::PointLightBase::getSFPosition )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PointLightBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::PointLightBase::*getType_function_type )(  ) ;
            
            PointLightBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::PointLightBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PointLightBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::PointLightBase::*getType_function_type )(  ) const;
            
            PointLightBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::PointLightBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PointLightBase::setPosition
        
            typedef void ( ::OSG::PointLightBase::*setPosition_function_type )( ::OSG::Pnt3f const & ) ;
            
            PointLightBase_exposer.def( 
                "setPosition"
                , setPosition_function_type( &::OSG::PointLightBase::setPosition )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::Light::callLightEngineEnter
        
            typedef void ( ::OSG::Light::*callLightEngineEnter_function_type )( ::OSG::RenderAction * ) ;
            
            PointLightBase_exposer.def( 
                "callLightEngineEnter"
                , bp::pure_virtual( callLightEngineEnter_function_type(&::OSG::Light::callLightEngineEnter) )
                , ( bp::arg("ract") ) );
        
        }
        { //::OSG::Light::callLightEngineLeave
        
            typedef void ( ::OSG::Light::*callLightEngineLeave_function_type )( ::OSG::RenderAction * ) ;
            
            PointLightBase_exposer.def( 
                "callLightEngineLeave"
                , bp::pure_virtual( callLightEngineLeave_function_type(&::OSG::Light::callLightEngineLeave) )
                , ( bp::arg("ract") ) );
        
        }
        PointLightBase_exposer.staticmethod( "create" );
        PointLightBase_exposer.staticmethod( "createDependent" );
        PointLightBase_exposer.staticmethod( "createLocal" );
        PointLightBase_exposer.staticmethod( "getClassGroupId" );
        PointLightBase_exposer.staticmethod( "getClassType" );
        PointLightBase_exposer.staticmethod( "getClassTypeId" );
    }

}
