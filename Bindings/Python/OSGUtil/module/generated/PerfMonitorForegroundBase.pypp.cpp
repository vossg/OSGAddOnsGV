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
#include "OSGUtil_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "PerfMonitorForegroundBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

struct PerfMonitorForegroundBase_wrapper : OSG::PerfMonitorForegroundBase, bp::wrapper< OSG::PerfMonitorForegroundBase > {



};

void register_PerfMonitorForegroundBase_class(){

    { //::OSG::PerfMonitorForegroundBase
        typedef bp::class_< PerfMonitorForegroundBase_wrapper, bp::bases< ::OSG::Foreground >, boost::noncopyable > PerfMonitorForegroundBase_exposer_t;
        PerfMonitorForegroundBase_exposer_t PerfMonitorForegroundBase_exposer = PerfMonitorForegroundBase_exposer_t( "PerfMonitorForegroundBase", bp::no_init );
        bp::scope PerfMonitorForegroundBase_scope( PerfMonitorForegroundBase_exposer );
        bp::scope().attr("ModeFieldId") = (int)OSG::PerfMonitorForegroundBase::ModeFieldId;
        bp::scope().attr("MaxSizeFieldId") = (int)OSG::PerfMonitorForegroundBase::MaxSizeFieldId;
        bp::scope().attr("ColorFieldId") = (int)OSG::PerfMonitorForegroundBase::ColorFieldId;
        bp::scope().attr("BgColorFieldId") = (int)OSG::PerfMonitorForegroundBase::BgColorFieldId;
        bp::scope().attr("FamilyFieldId") = (int)OSG::PerfMonitorForegroundBase::FamilyFieldId;
        bp::scope().attr("BorderColorFieldId") = (int)OSG::PerfMonitorForegroundBase::BorderColorFieldId;
        bp::scope().attr("BorderOffsetFieldId") = (int)OSG::PerfMonitorForegroundBase::BorderOffsetFieldId;
        bp::scope().attr("TextMarginFieldId") = (int)OSG::PerfMonitorForegroundBase::TextMarginFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::PerfMonitorForegroundBase::NextFieldId;
        { //::OSG::PerfMonitorForegroundBase::copyFromBin
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::PerfMonitorForegroundBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::copyToBin
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::PerfMonitorForegroundBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::create
        
            typedef ::OSG::PerfMonitorForegroundTransitPtr ( *create_function_type )(  );
            
            PerfMonitorForegroundBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::PerfMonitorForegroundBase::create ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::createDependent
        
            typedef ::OSG::PerfMonitorForegroundTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            PerfMonitorForegroundBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::PerfMonitorForegroundBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::createLocal
        
            typedef ::OSG::PerfMonitorForegroundTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            PerfMonitorForegroundBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::PerfMonitorForegroundBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getBgColor
        
            typedef ::OSG::Color4f const & ( ::OSG::PerfMonitorForegroundBase::*getBgColor_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getBgColor"
                , getBgColor_function_type( &::OSG::PerfMonitorForegroundBase::getBgColor )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::PerfMonitorForegroundBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::PerfMonitorForegroundBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getBorderColor
        
            typedef ::OSG::Color4f const & ( ::OSG::PerfMonitorForegroundBase::*getBorderColor_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getBorderColor"
                , getBorderColor_function_type( &::OSG::PerfMonitorForegroundBase::getBorderColor )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getBorderOffset
        
            typedef ::OSG::Vec2f const & ( ::OSG::PerfMonitorForegroundBase::*getBorderOffset_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getBorderOffset"
                , getBorderOffset_function_type( &::OSG::PerfMonitorForegroundBase::getBorderOffset )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            PerfMonitorForegroundBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::PerfMonitorForegroundBase::getClassGroupId ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            PerfMonitorForegroundBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::PerfMonitorForegroundBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            PerfMonitorForegroundBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::PerfMonitorForegroundBase::getClassTypeId ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getColor
        
            typedef ::OSG::Color4f const & ( ::OSG::PerfMonitorForegroundBase::*getColor_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getColor"
                , getColor_function_type( &::OSG::PerfMonitorForegroundBase::getColor )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::PerfMonitorForegroundBase::*getContainerSize_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::PerfMonitorForegroundBase::getContainerSize ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getFamily
        
            typedef ::std::string const & ( ::OSG::PerfMonitorForegroundBase::*getFamily_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getFamily"
                , getFamily_function_type( &::OSG::PerfMonitorForegroundBase::getFamily )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getMaxSize
        
            typedef ::OSG::Real32 ( ::OSG::PerfMonitorForegroundBase::*getMaxSize_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getMaxSize"
                , getMaxSize_function_type( &::OSG::PerfMonitorForegroundBase::getMaxSize ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getMode
        
            typedef ::OSG::UInt32 ( ::OSG::PerfMonitorForegroundBase::*getMode_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getMode"
                , getMode_function_type( &::OSG::PerfMonitorForegroundBase::getMode ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getSFBgColor
        
            typedef ::OSG::SFColor4f const * ( ::OSG::PerfMonitorForegroundBase::*getSFBgColor_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getSFBgColor"
                , getSFBgColor_function_type( &::OSG::PerfMonitorForegroundBase::getSFBgColor )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getSFBorderColor
        
            typedef ::OSG::SFColor4f const * ( ::OSG::PerfMonitorForegroundBase::*getSFBorderColor_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getSFBorderColor"
                , getSFBorderColor_function_type( &::OSG::PerfMonitorForegroundBase::getSFBorderColor )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getSFBorderOffset
        
            typedef ::OSG::SFVec2f const * ( ::OSG::PerfMonitorForegroundBase::*getSFBorderOffset_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getSFBorderOffset"
                , getSFBorderOffset_function_type( &::OSG::PerfMonitorForegroundBase::getSFBorderOffset )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getSFColor
        
            typedef ::OSG::SFColor4f const * ( ::OSG::PerfMonitorForegroundBase::*getSFColor_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getSFColor"
                , getSFColor_function_type( &::OSG::PerfMonitorForegroundBase::getSFColor )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getSFFamily
        
            typedef ::OSG::SFString const * ( ::OSG::PerfMonitorForegroundBase::*getSFFamily_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getSFFamily"
                , getSFFamily_function_type( &::OSG::PerfMonitorForegroundBase::getSFFamily )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getSFMaxSize
        
            typedef ::OSG::SFReal32 const * ( ::OSG::PerfMonitorForegroundBase::*getSFMaxSize_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getSFMaxSize"
                , getSFMaxSize_function_type( &::OSG::PerfMonitorForegroundBase::getSFMaxSize )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getSFMode
        
            typedef ::OSG::SFUInt32 const * ( ::OSG::PerfMonitorForegroundBase::*getSFMode_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getSFMode"
                , getSFMode_function_type( &::OSG::PerfMonitorForegroundBase::getSFMode )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getSFTextMargin
        
            typedef ::OSG::SFVec2f const * ( ::OSG::PerfMonitorForegroundBase::*getSFTextMargin_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getSFTextMargin"
                , getSFTextMargin_function_type( &::OSG::PerfMonitorForegroundBase::getSFTextMargin )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getTextMargin
        
            typedef ::OSG::Vec2f const & ( ::OSG::PerfMonitorForegroundBase::*getTextMargin_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getTextMargin"
                , getTextMargin_function_type( &::OSG::PerfMonitorForegroundBase::getTextMargin )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::PerfMonitorForegroundBase::*getType_function_type )(  ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::PerfMonitorForegroundBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::PerfMonitorForegroundBase::*getType_function_type )(  ) const;
            
            PerfMonitorForegroundBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::PerfMonitorForegroundBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::PerfMonitorForegroundBase::setBgColor
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*setBgColor_function_type )( ::OSG::Color4f const & ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "setBgColor"
                , setBgColor_function_type( &::OSG::PerfMonitorForegroundBase::setBgColor )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::setBorderColor
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*setBorderColor_function_type )( ::OSG::Color4f const & ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "setBorderColor"
                , setBorderColor_function_type( &::OSG::PerfMonitorForegroundBase::setBorderColor )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::setBorderOffset
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*setBorderOffset_function_type )( ::OSG::Vec2f const & ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "setBorderOffset"
                , setBorderOffset_function_type( &::OSG::PerfMonitorForegroundBase::setBorderOffset )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::setColor
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*setColor_function_type )( ::OSG::Color4f const & ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "setColor"
                , setColor_function_type( &::OSG::PerfMonitorForegroundBase::setColor )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::setFamily
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*setFamily_function_type )( ::std::string const & ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "setFamily"
                , setFamily_function_type( &::OSG::PerfMonitorForegroundBase::setFamily )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::setMaxSize
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*setMaxSize_function_type )( ::OSG::Real32 const ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "setMaxSize"
                , setMaxSize_function_type( &::OSG::PerfMonitorForegroundBase::setMaxSize )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::setMode
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*setMode_function_type )( ::OSG::UInt32 const ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "setMode"
                , setMode_function_type( &::OSG::PerfMonitorForegroundBase::setMode )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::PerfMonitorForegroundBase::setTextMargin
        
            typedef void ( ::OSG::PerfMonitorForegroundBase::*setTextMargin_function_type )( ::OSG::Vec2f const & ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "setTextMargin"
                , setTextMargin_function_type( &::OSG::PerfMonitorForegroundBase::setTextMargin )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::Foreground::draw
        
            typedef void ( ::OSG::Foreground::*draw_function_type )( ::OSG::DrawEnv * ) ;
            
            PerfMonitorForegroundBase_exposer.def( 
                "draw"
                , draw_function_type( &::OSG::Foreground::draw )
                , ( bp::arg("pEnv") ) );
        
        }
        PerfMonitorForegroundBase_exposer.staticmethod( "create" );
        PerfMonitorForegroundBase_exposer.staticmethod( "createDependent" );
        PerfMonitorForegroundBase_exposer.staticmethod( "createLocal" );
        PerfMonitorForegroundBase_exposer.staticmethod( "getClassGroupId" );
        PerfMonitorForegroundBase_exposer.staticmethod( "getClassType" );
        PerfMonitorForegroundBase_exposer.staticmethod( "getClassTypeId" );
    }

}
