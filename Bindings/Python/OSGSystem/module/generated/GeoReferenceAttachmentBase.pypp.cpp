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
#include "GeoReferenceAttachmentBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_GeoReferenceAttachmentBase_class(){

    { //::OSG::GeoReferenceAttachmentBase
        typedef bp::class_< OSG::GeoReferenceAttachmentBase, bp::bases< ::OSG::Attachment >, boost::noncopyable > GeoReferenceAttachmentBase_exposer_t;
        GeoReferenceAttachmentBase_exposer_t GeoReferenceAttachmentBase_exposer = GeoReferenceAttachmentBase_exposer_t( "GeoReferenceAttachmentBase", bp::no_init );
        bp::scope GeoReferenceAttachmentBase_scope( GeoReferenceAttachmentBase_exposer );
        bp::scope().attr("DatumFieldId") = (int)OSG::GeoReferenceAttachmentBase::DatumFieldId;
        bp::scope().attr("EllipsoidAxisFieldId") = (int)OSG::GeoReferenceAttachmentBase::EllipsoidAxisFieldId;
        bp::scope().attr("OriginFieldId") = (int)OSG::GeoReferenceAttachmentBase::OriginFieldId;
        bp::scope().attr("PixelSizeFieldId") = (int)OSG::GeoReferenceAttachmentBase::PixelSizeFieldId;
        bp::scope().attr("NoDataValueFieldId") = (int)OSG::GeoReferenceAttachmentBase::NoDataValueFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::GeoReferenceAttachmentBase::NextFieldId;
        { //::OSG::GeoReferenceAttachmentBase::copyFromBin
        
            typedef void ( ::OSG::GeoReferenceAttachmentBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::GeoReferenceAttachmentBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::copyToBin
        
            typedef void ( ::OSG::GeoReferenceAttachmentBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::GeoReferenceAttachmentBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::create
        
            typedef ::OSG::GeoReferenceAttachmentTransitPtr ( *create_function_type )(  );
            
            GeoReferenceAttachmentBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::GeoReferenceAttachmentBase::create ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::createDependent
        
            typedef ::OSG::GeoReferenceAttachmentTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            GeoReferenceAttachmentBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::GeoReferenceAttachmentBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::createLocal
        
            typedef ::OSG::GeoReferenceAttachmentTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            GeoReferenceAttachmentBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::GeoReferenceAttachmentBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::GeoReferenceAttachmentBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::GeoReferenceAttachmentBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::GeoReferenceAttachmentBase::getClassGroupId ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::GeoReferenceAttachmentBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::GeoReferenceAttachmentBase::getClassTypeId ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::GeoReferenceAttachmentBase::*getContainerSize_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::GeoReferenceAttachmentBase::getContainerSize ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getDatum
        
            typedef ::OSG::UInt32 ( ::OSG::GeoReferenceAttachmentBase::*getDatum_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getDatum"
                , getDatum_function_type( &::OSG::GeoReferenceAttachmentBase::getDatum ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getEllipsoidAxis
        
            typedef ::OSG::Vec2f const & ( ::OSG::GeoReferenceAttachmentBase::*getEllipsoidAxis_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getEllipsoidAxis"
                , getEllipsoidAxis_function_type( &::OSG::GeoReferenceAttachmentBase::getEllipsoidAxis )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getNoDataValue
        
            typedef ::OSG::Real64 ( ::OSG::GeoReferenceAttachmentBase::*getNoDataValue_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getNoDataValue"
                , getNoDataValue_function_type( &::OSG::GeoReferenceAttachmentBase::getNoDataValue ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getOrigin
        
            typedef ::OSG::Vec2f const & ( ::OSG::GeoReferenceAttachmentBase::*getOrigin_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getOrigin"
                , getOrigin_function_type( &::OSG::GeoReferenceAttachmentBase::getOrigin )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getPixelSize
        
            typedef ::OSG::Vec2f const & ( ::OSG::GeoReferenceAttachmentBase::*getPixelSize_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getPixelSize"
                , getPixelSize_function_type( &::OSG::GeoReferenceAttachmentBase::getPixelSize )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getSFDatum
        
            typedef ::OSG::SFUInt32 const * ( ::OSG::GeoReferenceAttachmentBase::*getSFDatum_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getSFDatum"
                , getSFDatum_function_type( &::OSG::GeoReferenceAttachmentBase::getSFDatum )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getSFEllipsoidAxis
        
            typedef ::OSG::SFVec2f const * ( ::OSG::GeoReferenceAttachmentBase::*getSFEllipsoidAxis_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getSFEllipsoidAxis"
                , getSFEllipsoidAxis_function_type( &::OSG::GeoReferenceAttachmentBase::getSFEllipsoidAxis )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getSFNoDataValue
        
            typedef ::OSG::SFReal64 const * ( ::OSG::GeoReferenceAttachmentBase::*getSFNoDataValue_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getSFNoDataValue"
                , getSFNoDataValue_function_type( &::OSG::GeoReferenceAttachmentBase::getSFNoDataValue )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getSFOrigin
        
            typedef ::OSG::SFVec2f const * ( ::OSG::GeoReferenceAttachmentBase::*getSFOrigin_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getSFOrigin"
                , getSFOrigin_function_type( &::OSG::GeoReferenceAttachmentBase::getSFOrigin )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getSFPixelSize
        
            typedef ::OSG::SFVec2f const * ( ::OSG::GeoReferenceAttachmentBase::*getSFPixelSize_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getSFPixelSize"
                , getSFPixelSize_function_type( &::OSG::GeoReferenceAttachmentBase::getSFPixelSize )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::GeoReferenceAttachmentBase::*getType_function_type )(  ) ;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::GeoReferenceAttachmentBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::GeoReferenceAttachmentBase::*getType_function_type )(  ) const;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::GeoReferenceAttachmentBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::setDatum
        
            typedef void ( ::OSG::GeoReferenceAttachmentBase::*setDatum_function_type )( ::OSG::UInt32 const ) ;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "setDatum"
                , setDatum_function_type( &::OSG::GeoReferenceAttachmentBase::setDatum )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::setEllipsoidAxis
        
            typedef void ( ::OSG::GeoReferenceAttachmentBase::*setEllipsoidAxis_function_type )( ::OSG::Vec2f const & ) ;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "setEllipsoidAxis"
                , setEllipsoidAxis_function_type( &::OSG::GeoReferenceAttachmentBase::setEllipsoidAxis )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::setNoDataValue
        
            typedef void ( ::OSG::GeoReferenceAttachmentBase::*setNoDataValue_function_type )( ::OSG::Real64 const ) ;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "setNoDataValue"
                , setNoDataValue_function_type( &::OSG::GeoReferenceAttachmentBase::setNoDataValue )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::setOrigin
        
            typedef void ( ::OSG::GeoReferenceAttachmentBase::*setOrigin_function_type )( ::OSG::Vec2f const & ) ;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "setOrigin"
                , setOrigin_function_type( &::OSG::GeoReferenceAttachmentBase::setOrigin )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::GeoReferenceAttachmentBase::setPixelSize
        
            typedef void ( ::OSG::GeoReferenceAttachmentBase::*setPixelSize_function_type )( ::OSG::Vec2f const & ) ;
            
            GeoReferenceAttachmentBase_exposer.def( 
                "setPixelSize"
                , setPixelSize_function_type( &::OSG::GeoReferenceAttachmentBase::setPixelSize )
                , ( bp::arg("value") ) );
        
        }
        GeoReferenceAttachmentBase_exposer.staticmethod( "create" );
        GeoReferenceAttachmentBase_exposer.staticmethod( "createDependent" );
        GeoReferenceAttachmentBase_exposer.staticmethod( "createLocal" );
        GeoReferenceAttachmentBase_exposer.staticmethod( "getClassGroupId" );
        GeoReferenceAttachmentBase_exposer.staticmethod( "getClassType" );
        GeoReferenceAttachmentBase_exposer.staticmethod( "getClassTypeId" );
    }

}
