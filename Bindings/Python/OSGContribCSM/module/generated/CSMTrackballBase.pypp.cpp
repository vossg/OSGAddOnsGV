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
#include "OSGContribCSM_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "CSMTrackballBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_CSMTrackballBase_class(){

    { //::OSG::CSMTrackballBase
        typedef bp::class_< OSG::CSMTrackballBase, bp::bases< ::OSG::AttachmentContainer >, boost::noncopyable > CSMTrackballBase_exposer_t;
        CSMTrackballBase_exposer_t CSMTrackballBase_exposer = CSMTrackballBase_exposer_t( "CSMTrackballBase", bp::no_init );
        bp::scope CSMTrackballBase_scope( CSMTrackballBase_exposer );
        bp::scope().attr("ModifierMaskFieldId") = (int)OSG::CSMTrackballBase::ModifierMaskFieldId;
        bp::scope().attr("ProcessingFieldId") = (int)OSG::CSMTrackballBase::ProcessingFieldId;
        bp::scope().attr("TranslationScaleFactorFieldId") = (int)OSG::CSMTrackballBase::TranslationScaleFactorFieldId;
        bp::scope().attr("MatrixResultFieldId") = (int)OSG::CSMTrackballBase::MatrixResultFieldId;
        bp::scope().attr("ReferencePositionFieldId") = (int)OSG::CSMTrackballBase::ReferencePositionFieldId;
        bp::scope().attr("ReferenceMatrixFieldId") = (int)OSG::CSMTrackballBase::ReferenceMatrixFieldId;
        bp::scope().attr("TransformCenterFieldId") = (int)OSG::CSMTrackballBase::TransformCenterFieldId;
        bp::scope().attr("WorldDiagFieldId") = (int)OSG::CSMTrackballBase::WorldDiagFieldId;
        bp::scope().attr("MouseDataFieldId") = (int)OSG::CSMTrackballBase::MouseDataFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::CSMTrackballBase::NextFieldId;
        { //::OSG::CSMTrackballBase::copyFromBin
        
            typedef void ( ::OSG::CSMTrackballBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            CSMTrackballBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::CSMTrackballBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::CSMTrackballBase::copyToBin
        
            typedef void ( ::OSG::CSMTrackballBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            CSMTrackballBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::CSMTrackballBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::CSMTrackballBase::create
        
            typedef ::OSG::CSMTrackballTransitPtr ( *create_function_type )(  );
            
            CSMTrackballBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::CSMTrackballBase::create ) );
        
        }
        { //::OSG::CSMTrackballBase::createDependent
        
            typedef ::OSG::CSMTrackballTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            CSMTrackballBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::CSMTrackballBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::CSMTrackballBase::createLocal
        
            typedef ::OSG::CSMTrackballTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            CSMTrackballBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::CSMTrackballBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::CSMTrackballBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::CSMTrackballBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            CSMTrackballBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::CSMTrackballBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::CSMTrackballBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            CSMTrackballBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::CSMTrackballBase::getClassGroupId ) );
        
        }
        { //::OSG::CSMTrackballBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            CSMTrackballBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::CSMTrackballBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::CSMTrackballBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            CSMTrackballBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::CSMTrackballBase::getClassTypeId ) );
        
        }
        { //::OSG::CSMTrackballBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::CSMTrackballBase::*getContainerSize_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::CSMTrackballBase::getContainerSize ) );
        
        }
        { //::OSG::CSMTrackballBase::getMatrixResult
        
            typedef ::OSG::Matrix const & ( ::OSG::CSMTrackballBase::*getMatrixResult_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getMatrixResult"
                , getMatrixResult_function_type( &::OSG::CSMTrackballBase::getMatrixResult )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::CSMTrackballBase::getModifierMask
        
            typedef ::OSG::UInt32 ( ::OSG::CSMTrackballBase::*getModifierMask_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getModifierMask"
                , getModifierMask_function_type( &::OSG::CSMTrackballBase::getModifierMask ) );
        
        }
        { //::OSG::CSMTrackballBase::getMouseData
        
            typedef ::OSG::MouseData const & ( ::OSG::CSMTrackballBase::*getMouseData_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getMouseData"
                , getMouseData_function_type( &::OSG::CSMTrackballBase::getMouseData )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::CSMTrackballBase::getReferenceMatrix
        
            typedef ::OSG::Matrix const & ( ::OSG::CSMTrackballBase::*getReferenceMatrix_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getReferenceMatrix"
                , getReferenceMatrix_function_type( &::OSG::CSMTrackballBase::getReferenceMatrix )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::CSMTrackballBase::getReferencePosition
        
            typedef ::OSG::Pnt3f const & ( ::OSG::CSMTrackballBase::*getReferencePosition_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getReferencePosition"
                , getReferencePosition_function_type( &::OSG::CSMTrackballBase::getReferencePosition )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::CSMTrackballBase::getSFMatrixResult
        
            typedef ::OSG::SFMatrix const * ( ::OSG::CSMTrackballBase::*getSFMatrixResult_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getSFMatrixResult"
                , getSFMatrixResult_function_type( &::OSG::CSMTrackballBase::getSFMatrixResult )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getSFModifierMask
        
            typedef ::OSG::SFUInt32 const * ( ::OSG::CSMTrackballBase::*getSFModifierMask_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getSFModifierMask"
                , getSFModifierMask_function_type( &::OSG::CSMTrackballBase::getSFModifierMask )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getSFMouseData
        
            typedef ::OSG::SFMouseData const * ( ::OSG::CSMTrackballBase::*getSFMouseData_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getSFMouseData"
                , getSFMouseData_function_type( &::OSG::CSMTrackballBase::getSFMouseData )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getSFReferenceMatrix
        
            typedef ::OSG::SFMatrix const * ( ::OSG::CSMTrackballBase::*getSFReferenceMatrix_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getSFReferenceMatrix"
                , getSFReferenceMatrix_function_type( &::OSG::CSMTrackballBase::getSFReferenceMatrix )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getSFReferencePosition
        
            typedef ::OSG::SFPnt3f const * ( ::OSG::CSMTrackballBase::*getSFReferencePosition_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getSFReferencePosition"
                , getSFReferencePosition_function_type( &::OSG::CSMTrackballBase::getSFReferencePosition )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getSFTransformCenter
        
            typedef ::OSG::SFPnt3f const * ( ::OSG::CSMTrackballBase::*getSFTransformCenter_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getSFTransformCenter"
                , getSFTransformCenter_function_type( &::OSG::CSMTrackballBase::getSFTransformCenter )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getSFTranslationScaleFactor
        
            typedef ::OSG::SFReal32 const * ( ::OSG::CSMTrackballBase::*getSFTranslationScaleFactor_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getSFTranslationScaleFactor"
                , getSFTranslationScaleFactor_function_type( &::OSG::CSMTrackballBase::getSFTranslationScaleFactor )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getSFWorldDiag
        
            typedef ::OSG::SFVec3f const * ( ::OSG::CSMTrackballBase::*getSFWorldDiag_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getSFWorldDiag"
                , getSFWorldDiag_function_type( &::OSG::CSMTrackballBase::getSFWorldDiag )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getTransformCenter
        
            typedef ::OSG::Pnt3f const & ( ::OSG::CSMTrackballBase::*getTransformCenter_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getTransformCenter"
                , getTransformCenter_function_type( &::OSG::CSMTrackballBase::getTransformCenter )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::CSMTrackballBase::getTranslationScaleFactor
        
            typedef ::OSG::Real32 ( ::OSG::CSMTrackballBase::*getTranslationScaleFactor_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getTranslationScaleFactor"
                , getTranslationScaleFactor_function_type( &::OSG::CSMTrackballBase::getTranslationScaleFactor ) );
        
        }
        { //::OSG::CSMTrackballBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::CSMTrackballBase::*getType_function_type )(  ) ;
            
            CSMTrackballBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::CSMTrackballBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::CSMTrackballBase::*getType_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::CSMTrackballBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::CSMTrackballBase::getWorldDiag
        
            typedef ::OSG::Vec3f const & ( ::OSG::CSMTrackballBase::*getWorldDiag_function_type )(  ) const;
            
            CSMTrackballBase_exposer.def( 
                "getWorldDiag"
                , getWorldDiag_function_type( &::OSG::CSMTrackballBase::getWorldDiag )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::CSMTrackballBase::setMatrixResult
        
            typedef void ( ::OSG::CSMTrackballBase::*setMatrixResult_function_type )( ::OSG::Matrix const & ) ;
            
            CSMTrackballBase_exposer.def( 
                "setMatrixResult"
                , setMatrixResult_function_type( &::OSG::CSMTrackballBase::setMatrixResult )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::CSMTrackballBase::setModifierMask
        
            typedef void ( ::OSG::CSMTrackballBase::*setModifierMask_function_type )( ::OSG::UInt32 const ) ;
            
            CSMTrackballBase_exposer.def( 
                "setModifierMask"
                , setModifierMask_function_type( &::OSG::CSMTrackballBase::setModifierMask )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::CSMTrackballBase::setMouseData
        
            typedef void ( ::OSG::CSMTrackballBase::*setMouseData_function_type )( ::OSG::MouseData const & ) ;
            
            CSMTrackballBase_exposer.def( 
                "setMouseData"
                , setMouseData_function_type( &::OSG::CSMTrackballBase::setMouseData )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::CSMTrackballBase::setReferenceMatrix
        
            typedef void ( ::OSG::CSMTrackballBase::*setReferenceMatrix_function_type )( ::OSG::Matrix const & ) ;
            
            CSMTrackballBase_exposer.def( 
                "setReferenceMatrix"
                , setReferenceMatrix_function_type( &::OSG::CSMTrackballBase::setReferenceMatrix )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::CSMTrackballBase::setReferencePosition
        
            typedef void ( ::OSG::CSMTrackballBase::*setReferencePosition_function_type )( ::OSG::Pnt3f const & ) ;
            
            CSMTrackballBase_exposer.def( 
                "setReferencePosition"
                , setReferencePosition_function_type( &::OSG::CSMTrackballBase::setReferencePosition )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::CSMTrackballBase::setTransformCenter
        
            typedef void ( ::OSG::CSMTrackballBase::*setTransformCenter_function_type )( ::OSG::Pnt3f const & ) ;
            
            CSMTrackballBase_exposer.def( 
                "setTransformCenter"
                , setTransformCenter_function_type( &::OSG::CSMTrackballBase::setTransformCenter )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::CSMTrackballBase::setTranslationScaleFactor
        
            typedef void ( ::OSG::CSMTrackballBase::*setTranslationScaleFactor_function_type )( ::OSG::Real32 const ) ;
            
            CSMTrackballBase_exposer.def( 
                "setTranslationScaleFactor"
                , setTranslationScaleFactor_function_type( &::OSG::CSMTrackballBase::setTranslationScaleFactor )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::CSMTrackballBase::setWorldDiag
        
            typedef void ( ::OSG::CSMTrackballBase::*setWorldDiag_function_type )( ::OSG::Vec3f const & ) ;
            
            CSMTrackballBase_exposer.def( 
                "setWorldDiag"
                , setWorldDiag_function_type( &::OSG::CSMTrackballBase::setWorldDiag )
                , ( bp::arg("value") ) );
        
        }
        CSMTrackballBase_exposer.staticmethod( "create" );
        CSMTrackballBase_exposer.staticmethod( "createDependent" );
        CSMTrackballBase_exposer.staticmethod( "createLocal" );
        CSMTrackballBase_exposer.staticmethod( "getClassGroupId" );
        CSMTrackballBase_exposer.staticmethod( "getClassType" );
        CSMTrackballBase_exposer.staticmethod( "getClassTypeId" );
    }

}
