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
#include "SimpleStatisticsForegroundBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

struct SimpleStatisticsForegroundBase_wrapper : OSG::SimpleStatisticsForegroundBase, bp::wrapper< OSG::SimpleStatisticsForegroundBase > {



};

boost::python::list SimpleStatisticsForegroundBase_getMFFormats(OSG::SimpleStatisticsForegroundBase *self)
{
   boost::python::list result;
   OSG::MFString const * mf_data = self->getMFFormats();
   const OSG::UInt32 size(mf_data->size());
   for ( OSG::UInt32 i = 0; i < size; ++i )
   {
      result.append((*mf_data)[i]);
   }
   return result;
}

void register_SimpleStatisticsForegroundBase_class(){

    { //::OSG::SimpleStatisticsForegroundBase
        typedef bp::class_< SimpleStatisticsForegroundBase_wrapper, bp::bases< ::OSG::StatisticsForeground >, boost::noncopyable > SimpleStatisticsForegroundBase_exposer_t;
        SimpleStatisticsForegroundBase_exposer_t SimpleStatisticsForegroundBase_exposer = SimpleStatisticsForegroundBase_exposer_t( "SimpleStatisticsForegroundBase", bp::no_init );
        bp::scope SimpleStatisticsForegroundBase_scope( SimpleStatisticsForegroundBase_exposer );
        bp::scope().attr("FormatsFieldId") = (int)OSG::SimpleStatisticsForegroundBase::FormatsFieldId;
        bp::scope().attr("SizeFieldId") = (int)OSG::SimpleStatisticsForegroundBase::SizeFieldId;
        bp::scope().attr("ColorFieldId") = (int)OSG::SimpleStatisticsForegroundBase::ColorFieldId;
        bp::scope().attr("ShadowColorFieldId") = (int)OSG::SimpleStatisticsForegroundBase::ShadowColorFieldId;
        bp::scope().attr("BgColorFieldId") = (int)OSG::SimpleStatisticsForegroundBase::BgColorFieldId;
        bp::scope().attr("FamilyFieldId") = (int)OSG::SimpleStatisticsForegroundBase::FamilyFieldId;
        bp::scope().attr("ShadowOffsetFieldId") = (int)OSG::SimpleStatisticsForegroundBase::ShadowOffsetFieldId;
        bp::scope().attr("HorizontalAlignFieldId") = (int)OSG::SimpleStatisticsForegroundBase::HorizontalAlignFieldId;
        bp::scope().attr("VerticalAlignFieldId") = (int)OSG::SimpleStatisticsForegroundBase::VerticalAlignFieldId;
        bp::scope().attr("BorderColorFieldId") = (int)OSG::SimpleStatisticsForegroundBase::BorderColorFieldId;
        bp::scope().attr("BorderOffsetFieldId") = (int)OSG::SimpleStatisticsForegroundBase::BorderOffsetFieldId;
        bp::scope().attr("TextMarginFieldId") = (int)OSG::SimpleStatisticsForegroundBase::TextMarginFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::SimpleStatisticsForegroundBase::NextFieldId;
        { //::OSG::SimpleStatisticsForegroundBase::copyFromBin
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::SimpleStatisticsForegroundBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::copyToBin
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::SimpleStatisticsForegroundBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::create
        
            typedef ::OSG::SimpleStatisticsForegroundTransitPtr ( *create_function_type )(  );
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::SimpleStatisticsForegroundBase::create ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::createDependent
        
            typedef ::OSG::SimpleStatisticsForegroundTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::SimpleStatisticsForegroundBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::createLocal
        
            typedef ::OSG::SimpleStatisticsForegroundTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::SimpleStatisticsForegroundBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getBgColor
        
            typedef ::OSG::Color4f const & ( ::OSG::SimpleStatisticsForegroundBase::*getBgColor_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getBgColor"
                , getBgColor_function_type( &::OSG::SimpleStatisticsForegroundBase::getBgColor )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::SimpleStatisticsForegroundBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::SimpleStatisticsForegroundBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getBorderColor
        
            typedef ::OSG::Color4f const & ( ::OSG::SimpleStatisticsForegroundBase::*getBorderColor_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getBorderColor"
                , getBorderColor_function_type( &::OSG::SimpleStatisticsForegroundBase::getBorderColor )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getBorderOffset
        
            typedef ::OSG::Vec2f const & ( ::OSG::SimpleStatisticsForegroundBase::*getBorderOffset_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getBorderOffset"
                , getBorderOffset_function_type( &::OSG::SimpleStatisticsForegroundBase::getBorderOffset )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::SimpleStatisticsForegroundBase::getClassGroupId ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::SimpleStatisticsForegroundBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::SimpleStatisticsForegroundBase::getClassTypeId ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getColor
        
            typedef ::OSG::Color4f const & ( ::OSG::SimpleStatisticsForegroundBase::*getColor_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getColor"
                , getColor_function_type( &::OSG::SimpleStatisticsForegroundBase::getColor )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::SimpleStatisticsForegroundBase::*getContainerSize_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::SimpleStatisticsForegroundBase::getContainerSize ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getFamily
        
            typedef ::std::string const & ( ::OSG::SimpleStatisticsForegroundBase::*getFamily_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getFamily"
                , getFamily_function_type( &::OSG::SimpleStatisticsForegroundBase::getFamily )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getFormats
        
            typedef ::std::string const & ( ::OSG::SimpleStatisticsForegroundBase::*getFormats_function_type )( ::OSG::UInt32 const ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getFormats"
                , getFormats_function_type( &::OSG::SimpleStatisticsForegroundBase::getFormats )
                , ( bp::arg("index") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getHorizontalAlign
        
            typedef ::OSG::UInt8 ( ::OSG::SimpleStatisticsForegroundBase::*getHorizontalAlign_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getHorizontalAlign"
                , getHorizontalAlign_function_type( &::OSG::SimpleStatisticsForegroundBase::getHorizontalAlign ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFBgColor
        
            typedef ::OSG::SFColor4f const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFBgColor_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFBgColor"
                , getSFBgColor_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFBgColor )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFBorderColor
        
            typedef ::OSG::SFColor4f const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFBorderColor_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFBorderColor"
                , getSFBorderColor_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFBorderColor )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFBorderOffset
        
            typedef ::OSG::SFVec2f const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFBorderOffset_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFBorderOffset"
                , getSFBorderOffset_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFBorderOffset )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFColor
        
            typedef ::OSG::SFColor4f const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFColor_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFColor"
                , getSFColor_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFColor )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFFamily
        
            typedef ::OSG::SFString const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFFamily_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFFamily"
                , getSFFamily_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFFamily )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFHorizontalAlign
        
            typedef ::OSG::SFUInt8 const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFHorizontalAlign_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFHorizontalAlign"
                , getSFHorizontalAlign_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFHorizontalAlign )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFShadowColor
        
            typedef ::OSG::SFColor4f const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFShadowColor_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFShadowColor"
                , getSFShadowColor_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFShadowColor )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFShadowOffset
        
            typedef ::OSG::SFVec2f const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFShadowOffset_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFShadowOffset"
                , getSFShadowOffset_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFShadowOffset )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFSize
        
            typedef ::OSG::SFReal32 const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFSize_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFSize"
                , getSFSize_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFSize )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFTextMargin
        
            typedef ::OSG::SFVec2f const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFTextMargin_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFTextMargin"
                , getSFTextMargin_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFTextMargin )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSFVerticalAlign
        
            typedef ::OSG::SFUInt8 const * ( ::OSG::SimpleStatisticsForegroundBase::*getSFVerticalAlign_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSFVerticalAlign"
                , getSFVerticalAlign_function_type( &::OSG::SimpleStatisticsForegroundBase::getSFVerticalAlign )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getShadowColor
        
            typedef ::OSG::Color4f const & ( ::OSG::SimpleStatisticsForegroundBase::*getShadowColor_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getShadowColor"
                , getShadowColor_function_type( &::OSG::SimpleStatisticsForegroundBase::getShadowColor )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getShadowOffset
        
            typedef ::OSG::Vec2f const & ( ::OSG::SimpleStatisticsForegroundBase::*getShadowOffset_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getShadowOffset"
                , getShadowOffset_function_type( &::OSG::SimpleStatisticsForegroundBase::getShadowOffset )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getSize
        
            typedef ::OSG::Real32 ( ::OSG::SimpleStatisticsForegroundBase::*getSize_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getSize"
                , getSize_function_type( &::OSG::SimpleStatisticsForegroundBase::getSize ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getTextMargin
        
            typedef ::OSG::Vec2f const & ( ::OSG::SimpleStatisticsForegroundBase::*getTextMargin_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getTextMargin"
                , getTextMargin_function_type( &::OSG::SimpleStatisticsForegroundBase::getTextMargin )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::SimpleStatisticsForegroundBase::*getType_function_type )(  ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::SimpleStatisticsForegroundBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::SimpleStatisticsForegroundBase::*getType_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::SimpleStatisticsForegroundBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::getVerticalAlign
        
            typedef ::OSG::UInt8 ( ::OSG::SimpleStatisticsForegroundBase::*getVerticalAlign_function_type )(  ) const;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "getVerticalAlign"
                , getVerticalAlign_function_type( &::OSG::SimpleStatisticsForegroundBase::getVerticalAlign ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setBgColor
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setBgColor_function_type )( ::OSG::Color4f const & ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setBgColor"
                , setBgColor_function_type( &::OSG::SimpleStatisticsForegroundBase::setBgColor )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setBorderColor
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setBorderColor_function_type )( ::OSG::Color4f const & ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setBorderColor"
                , setBorderColor_function_type( &::OSG::SimpleStatisticsForegroundBase::setBorderColor )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setBorderOffset
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setBorderOffset_function_type )( ::OSG::Vec2f const & ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setBorderOffset"
                , setBorderOffset_function_type( &::OSG::SimpleStatisticsForegroundBase::setBorderOffset )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setColor
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setColor_function_type )( ::OSG::Color4f const & ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setColor"
                , setColor_function_type( &::OSG::SimpleStatisticsForegroundBase::setColor )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setFamily
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setFamily_function_type )( ::std::string const & ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setFamily"
                , setFamily_function_type( &::OSG::SimpleStatisticsForegroundBase::setFamily )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setHorizontalAlign
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setHorizontalAlign_function_type )( ::OSG::UInt8 const ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setHorizontalAlign"
                , setHorizontalAlign_function_type( &::OSG::SimpleStatisticsForegroundBase::setHorizontalAlign )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setShadowColor
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setShadowColor_function_type )( ::OSG::Color4f const & ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setShadowColor"
                , setShadowColor_function_type( &::OSG::SimpleStatisticsForegroundBase::setShadowColor )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setShadowOffset
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setShadowOffset_function_type )( ::OSG::Vec2f const & ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setShadowOffset"
                , setShadowOffset_function_type( &::OSG::SimpleStatisticsForegroundBase::setShadowOffset )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setSize
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setSize_function_type )( ::OSG::Real32 const ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setSize"
                , setSize_function_type( &::OSG::SimpleStatisticsForegroundBase::setSize )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setTextMargin
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setTextMargin_function_type )( ::OSG::Vec2f const & ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setTextMargin"
                , setTextMargin_function_type( &::OSG::SimpleStatisticsForegroundBase::setTextMargin )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleStatisticsForegroundBase::setVerticalAlign
        
            typedef void ( ::OSG::SimpleStatisticsForegroundBase::*setVerticalAlign_function_type )( ::OSG::UInt8 const ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "setVerticalAlign"
                , setVerticalAlign_function_type( &::OSG::SimpleStatisticsForegroundBase::setVerticalAlign )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::StatisticsForeground::draw
        
            typedef void ( ::OSG::StatisticsForeground::*draw_function_type )( ::OSG::DrawEnv *,::OSG::Viewport * ) ;
            
            SimpleStatisticsForegroundBase_exposer.def( 
                "draw"
                , draw_function_type( &::OSG::StatisticsForeground::draw )
                , ( bp::arg("pEnv"), bp::arg("pPort") ) );
        
        }
        SimpleStatisticsForegroundBase_exposer.staticmethod( "create" );
        SimpleStatisticsForegroundBase_exposer.staticmethod( "createDependent" );
        SimpleStatisticsForegroundBase_exposer.staticmethod( "createLocal" );
        SimpleStatisticsForegroundBase_exposer.staticmethod( "getClassGroupId" );
        SimpleStatisticsForegroundBase_exposer.staticmethod( "getClassType" );
        SimpleStatisticsForegroundBase_exposer.staticmethod( "getClassTypeId" );
        SimpleStatisticsForegroundBase_exposer.def("getMFFormats",SimpleStatisticsForegroundBase_getMFFormats);
    }

}
