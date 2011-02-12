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
#include "TileCameraDecoratorBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_TileCameraDecoratorBase_class(){

    { //::OSG::TileCameraDecoratorBase
        typedef bp::class_< OSG::TileCameraDecoratorBase, bp::bases< ::OSG::CameraDecorator >, boost::noncopyable > TileCameraDecoratorBase_exposer_t;
        TileCameraDecoratorBase_exposer_t TileCameraDecoratorBase_exposer = TileCameraDecoratorBase_exposer_t( "TileCameraDecoratorBase", bp::no_init );
        bp::scope TileCameraDecoratorBase_scope( TileCameraDecoratorBase_exposer );
        bp::scope().attr("LeftFieldId") = (int)OSG::TileCameraDecoratorBase::LeftFieldId;
        bp::scope().attr("RightFieldId") = (int)OSG::TileCameraDecoratorBase::RightFieldId;
        bp::scope().attr("BottomFieldId") = (int)OSG::TileCameraDecoratorBase::BottomFieldId;
        bp::scope().attr("TopFieldId") = (int)OSG::TileCameraDecoratorBase::TopFieldId;
        bp::scope().attr("FullWidthFieldId") = (int)OSG::TileCameraDecoratorBase::FullWidthFieldId;
        bp::scope().attr("FullHeightFieldId") = (int)OSG::TileCameraDecoratorBase::FullHeightFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::TileCameraDecoratorBase::NextFieldId;
        { //::OSG::TileCameraDecoratorBase::copyFromBin
        
            typedef void ( ::OSG::TileCameraDecoratorBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::TileCameraDecoratorBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::copyToBin
        
            typedef void ( ::OSG::TileCameraDecoratorBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::TileCameraDecoratorBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::create
        
            typedef ::OSG::TileCameraDecoratorTransitPtr ( *create_function_type )(  );
            
            TileCameraDecoratorBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::TileCameraDecoratorBase::create ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::createDependent
        
            typedef ::OSG::TileCameraDecoratorTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            TileCameraDecoratorBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::TileCameraDecoratorBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::createLocal
        
            typedef ::OSG::TileCameraDecoratorTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            TileCameraDecoratorBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::TileCameraDecoratorBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::TileCameraDecoratorBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::TileCameraDecoratorBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getBottom
        
            typedef ::OSG::Real32 ( ::OSG::TileCameraDecoratorBase::*getBottom_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getBottom"
                , getBottom_function_type( &::OSG::TileCameraDecoratorBase::getBottom ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            TileCameraDecoratorBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::TileCameraDecoratorBase::getClassGroupId ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            TileCameraDecoratorBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::TileCameraDecoratorBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::TileCameraDecoratorBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            TileCameraDecoratorBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::TileCameraDecoratorBase::getClassTypeId ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::TileCameraDecoratorBase::*getContainerSize_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::TileCameraDecoratorBase::getContainerSize ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getFullHeight
        
            typedef ::OSG::UInt32 ( ::OSG::TileCameraDecoratorBase::*getFullHeight_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getFullHeight"
                , getFullHeight_function_type( &::OSG::TileCameraDecoratorBase::getFullHeight ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getFullWidth
        
            typedef ::OSG::UInt32 ( ::OSG::TileCameraDecoratorBase::*getFullWidth_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getFullWidth"
                , getFullWidth_function_type( &::OSG::TileCameraDecoratorBase::getFullWidth ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getLeft
        
            typedef ::OSG::Real32 ( ::OSG::TileCameraDecoratorBase::*getLeft_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getLeft"
                , getLeft_function_type( &::OSG::TileCameraDecoratorBase::getLeft ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getRight
        
            typedef ::OSG::Real32 ( ::OSG::TileCameraDecoratorBase::*getRight_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getRight"
                , getRight_function_type( &::OSG::TileCameraDecoratorBase::getRight ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getSFBottom
        
            typedef ::OSG::SFReal32 const * ( ::OSG::TileCameraDecoratorBase::*getSFBottom_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getSFBottom"
                , getSFBottom_function_type( &::OSG::TileCameraDecoratorBase::getSFBottom )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TileCameraDecoratorBase::getSFFullHeight
        
            typedef ::OSG::SFUInt32 const * ( ::OSG::TileCameraDecoratorBase::*getSFFullHeight_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getSFFullHeight"
                , getSFFullHeight_function_type( &::OSG::TileCameraDecoratorBase::getSFFullHeight )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TileCameraDecoratorBase::getSFFullWidth
        
            typedef ::OSG::SFUInt32 const * ( ::OSG::TileCameraDecoratorBase::*getSFFullWidth_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getSFFullWidth"
                , getSFFullWidth_function_type( &::OSG::TileCameraDecoratorBase::getSFFullWidth )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TileCameraDecoratorBase::getSFLeft
        
            typedef ::OSG::SFReal32 const * ( ::OSG::TileCameraDecoratorBase::*getSFLeft_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getSFLeft"
                , getSFLeft_function_type( &::OSG::TileCameraDecoratorBase::getSFLeft )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TileCameraDecoratorBase::getSFRight
        
            typedef ::OSG::SFReal32 const * ( ::OSG::TileCameraDecoratorBase::*getSFRight_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getSFRight"
                , getSFRight_function_type( &::OSG::TileCameraDecoratorBase::getSFRight )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TileCameraDecoratorBase::getSFTop
        
            typedef ::OSG::SFReal32 const * ( ::OSG::TileCameraDecoratorBase::*getSFTop_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getSFTop"
                , getSFTop_function_type( &::OSG::TileCameraDecoratorBase::getSFTop )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TileCameraDecoratorBase::getTop
        
            typedef ::OSG::Real32 ( ::OSG::TileCameraDecoratorBase::*getTop_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getTop"
                , getTop_function_type( &::OSG::TileCameraDecoratorBase::getTop ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::TileCameraDecoratorBase::*getType_function_type )(  ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::TileCameraDecoratorBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TileCameraDecoratorBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::TileCameraDecoratorBase::*getType_function_type )(  ) const;
            
            TileCameraDecoratorBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::TileCameraDecoratorBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TileCameraDecoratorBase::setBottom
        
            typedef void ( ::OSG::TileCameraDecoratorBase::*setBottom_function_type )( ::OSG::Real32 const ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "setBottom"
                , setBottom_function_type( &::OSG::TileCameraDecoratorBase::setBottom )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::setFullHeight
        
            typedef void ( ::OSG::TileCameraDecoratorBase::*setFullHeight_function_type )( ::OSG::UInt32 const ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "setFullHeight"
                , setFullHeight_function_type( &::OSG::TileCameraDecoratorBase::setFullHeight )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::setFullWidth
        
            typedef void ( ::OSG::TileCameraDecoratorBase::*setFullWidth_function_type )( ::OSG::UInt32 const ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "setFullWidth"
                , setFullWidth_function_type( &::OSG::TileCameraDecoratorBase::setFullWidth )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::setLeft
        
            typedef void ( ::OSG::TileCameraDecoratorBase::*setLeft_function_type )( ::OSG::Real32 const ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "setLeft"
                , setLeft_function_type( &::OSG::TileCameraDecoratorBase::setLeft )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::setRight
        
            typedef void ( ::OSG::TileCameraDecoratorBase::*setRight_function_type )( ::OSG::Real32 const ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "setRight"
                , setRight_function_type( &::OSG::TileCameraDecoratorBase::setRight )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::TileCameraDecoratorBase::setTop
        
            typedef void ( ::OSG::TileCameraDecoratorBase::*setTop_function_type )( ::OSG::Real32 const ) ;
            
            TileCameraDecoratorBase_exposer.def( 
                "setTop"
                , setTop_function_type( &::OSG::TileCameraDecoratorBase::setTop )
                , ( bp::arg("value") ) );
        
        }
        TileCameraDecoratorBase_exposer.staticmethod( "create" );
        TileCameraDecoratorBase_exposer.staticmethod( "createDependent" );
        TileCameraDecoratorBase_exposer.staticmethod( "createLocal" );
        TileCameraDecoratorBase_exposer.staticmethod( "getClassGroupId" );
        TileCameraDecoratorBase_exposer.staticmethod( "getClassType" );
        TileCameraDecoratorBase_exposer.staticmethod( "getClassTypeId" );
    }

}
