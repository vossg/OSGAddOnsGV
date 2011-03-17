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
#include "OSGDynamics_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "SkinnedGeometryBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

boost::python::list SkinnedGeometryBase_getMFJointIds(OSG::SkinnedGeometryBase *self)
{
   boost::python::list result;
   OSG::MFInt16 const * mf_data = self->getMFJointIds();
   const OSG::UInt32 size(mf_data->size());
   for ( OSG::UInt32 i = 0; i < size; ++i )
   {
      result.append((*mf_data)[i]);
   }
   return result;
}

void register_SkinnedGeometryBase_class(){

    { //::OSG::SkinnedGeometryBase
        typedef bp::class_< OSG::SkinnedGeometryBase, bp::bases< ::OSG::Geometry >, boost::noncopyable > SkinnedGeometryBase_exposer_t;
        SkinnedGeometryBase_exposer_t SkinnedGeometryBase_exposer = SkinnedGeometryBase_exposer_t( "SkinnedGeometryBase", bp::no_init );
        bp::scope SkinnedGeometryBase_scope( SkinnedGeometryBase_exposer );
        bp::scope().attr("SkeletonFieldId") = (int)OSG::SkinnedGeometryBase::SkeletonFieldId;
        bp::scope().attr("BindShapeMatrixFieldId") = (int)OSG::SkinnedGeometryBase::BindShapeMatrixFieldId;
        bp::scope().attr("JointIdsFieldId") = (int)OSG::SkinnedGeometryBase::JointIdsFieldId;
        bp::scope().attr("JointIndexPropertyFieldId") = (int)OSG::SkinnedGeometryBase::JointIndexPropertyFieldId;
        bp::scope().attr("JointWeightPropertyFieldId") = (int)OSG::SkinnedGeometryBase::JointWeightPropertyFieldId;
        bp::scope().attr("RenderModeFieldId") = (int)OSG::SkinnedGeometryBase::RenderModeFieldId;
        bp::scope().attr("SkinningAlgorithmFieldId") = (int)OSG::SkinnedGeometryBase::SkinningAlgorithmFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::SkinnedGeometryBase::NextFieldId;
        { //::OSG::SkinnedGeometryBase::copyFromBin
        
            typedef void ( ::OSG::SkinnedGeometryBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            SkinnedGeometryBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::SkinnedGeometryBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::SkinnedGeometryBase::copyToBin
        
            typedef void ( ::OSG::SkinnedGeometryBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            SkinnedGeometryBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::SkinnedGeometryBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::SkinnedGeometryBase::create
        
            typedef ::OSG::SkinnedGeometryTransitPtr ( *create_function_type )(  );
            
            SkinnedGeometryBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::SkinnedGeometryBase::create ) );
        
        }
        { //::OSG::SkinnedGeometryBase::createDependent
        
            typedef ::OSG::SkinnedGeometryTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            SkinnedGeometryBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::SkinnedGeometryBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::SkinnedGeometryBase::createLocal
        
            typedef ::OSG::SkinnedGeometryTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            SkinnedGeometryBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::SkinnedGeometryBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::SkinnedGeometryBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::SkinnedGeometryBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            SkinnedGeometryBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::SkinnedGeometryBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::SkinnedGeometryBase::getBindShapeMatrix
        
            typedef ::OSG::Matrix const & ( ::OSG::SkinnedGeometryBase::*getBindShapeMatrix_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getBindShapeMatrix"
                , getBindShapeMatrix_function_type( &::OSG::SkinnedGeometryBase::getBindShapeMatrix )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SkinnedGeometryBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            SkinnedGeometryBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::SkinnedGeometryBase::getClassGroupId ) );
        
        }
        { //::OSG::SkinnedGeometryBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            SkinnedGeometryBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::SkinnedGeometryBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::SkinnedGeometryBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            SkinnedGeometryBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::SkinnedGeometryBase::getClassTypeId ) );
        
        }
        { //::OSG::SkinnedGeometryBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::SkinnedGeometryBase::*getContainerSize_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::SkinnedGeometryBase::getContainerSize ) );
        
        }
        { //::OSG::SkinnedGeometryBase::getJointIds
        
            typedef ::OSG::Int16 ( ::OSG::SkinnedGeometryBase::*getJointIds_function_type )( ::OSG::UInt32 const ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getJointIds"
                , getJointIds_function_type( &::OSG::SkinnedGeometryBase::getJointIds )
                , ( bp::arg("index") ) );
        
        }
        { //::OSG::SkinnedGeometryBase::getJointIndexProperty
        
            typedef ::OSG::UInt16 ( ::OSG::SkinnedGeometryBase::*getJointIndexProperty_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getJointIndexProperty"
                , getJointIndexProperty_function_type( &::OSG::SkinnedGeometryBase::getJointIndexProperty ) );
        
        }
        { //::OSG::SkinnedGeometryBase::getJointWeightProperty
        
            typedef ::OSG::UInt16 ( ::OSG::SkinnedGeometryBase::*getJointWeightProperty_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getJointWeightProperty"
                , getJointWeightProperty_function_type( &::OSG::SkinnedGeometryBase::getJointWeightProperty ) );
        
        }
        { //::OSG::SkinnedGeometryBase::getRenderMode
        
            typedef ::OSG::UInt32 ( ::OSG::SkinnedGeometryBase::*getRenderMode_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getRenderMode"
                , getRenderMode_function_type( &::OSG::SkinnedGeometryBase::getRenderMode ) );
        
        }
        { //::OSG::SkinnedGeometryBase::getSFBindShapeMatrix
        
            typedef ::OSG::SFMatrix const * ( ::OSG::SkinnedGeometryBase::*getSFBindShapeMatrix_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getSFBindShapeMatrix"
                , getSFBindShapeMatrix_function_type( &::OSG::SkinnedGeometryBase::getSFBindShapeMatrix )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SkinnedGeometryBase::getSFJointIndexProperty
        
            typedef ::OSG::SFUInt16 const * ( ::OSG::SkinnedGeometryBase::*getSFJointIndexProperty_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getSFJointIndexProperty"
                , getSFJointIndexProperty_function_type( &::OSG::SkinnedGeometryBase::getSFJointIndexProperty )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SkinnedGeometryBase::getSFJointWeightProperty
        
            typedef ::OSG::SFUInt16 const * ( ::OSG::SkinnedGeometryBase::*getSFJointWeightProperty_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getSFJointWeightProperty"
                , getSFJointWeightProperty_function_type( &::OSG::SkinnedGeometryBase::getSFJointWeightProperty )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SkinnedGeometryBase::getSFRenderMode
        
            typedef ::OSG::SFUInt32 const * ( ::OSG::SkinnedGeometryBase::*getSFRenderMode_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getSFRenderMode"
                , getSFRenderMode_function_type( &::OSG::SkinnedGeometryBase::getSFRenderMode )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SkinnedGeometryBase::getSFSkeleton
        
            typedef ::OSG::SFUnrecSkeletonPtr const * ( ::OSG::SkinnedGeometryBase::*getSFSkeleton_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getSFSkeleton"
                , getSFSkeleton_function_type( &::OSG::SkinnedGeometryBase::getSFSkeleton )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SkinnedGeometryBase::getSkeleton
        
            typedef ::OSG::Skeleton * ( ::OSG::SkinnedGeometryBase::*getSkeleton_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getSkeleton"
                , getSkeleton_function_type( &::OSG::SkinnedGeometryBase::getSkeleton )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SkinnedGeometryBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::SkinnedGeometryBase::*getType_function_type )(  ) ;
            
            SkinnedGeometryBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::SkinnedGeometryBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SkinnedGeometryBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::SkinnedGeometryBase::*getType_function_type )(  ) const;
            
            SkinnedGeometryBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::SkinnedGeometryBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SkinnedGeometryBase::setBindShapeMatrix
        
            typedef void ( ::OSG::SkinnedGeometryBase::*setBindShapeMatrix_function_type )( ::OSG::Matrix const & ) ;
            
            SkinnedGeometryBase_exposer.def( 
                "setBindShapeMatrix"
                , setBindShapeMatrix_function_type( &::OSG::SkinnedGeometryBase::setBindShapeMatrix )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SkinnedGeometryBase::setJointIndexProperty
        
            typedef void ( ::OSG::SkinnedGeometryBase::*setJointIndexProperty_function_type )( ::OSG::UInt16 const ) ;
            
            SkinnedGeometryBase_exposer.def( 
                "setJointIndexProperty"
                , setJointIndexProperty_function_type( &::OSG::SkinnedGeometryBase::setJointIndexProperty )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SkinnedGeometryBase::setJointWeightProperty
        
            typedef void ( ::OSG::SkinnedGeometryBase::*setJointWeightProperty_function_type )( ::OSG::UInt16 const ) ;
            
            SkinnedGeometryBase_exposer.def( 
                "setJointWeightProperty"
                , setJointWeightProperty_function_type( &::OSG::SkinnedGeometryBase::setJointWeightProperty )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SkinnedGeometryBase::setRenderMode
        
            typedef void ( ::OSG::SkinnedGeometryBase::*setRenderMode_function_type )( ::OSG::UInt32 const ) ;
            
            SkinnedGeometryBase_exposer.def( 
                "setRenderMode"
                , setRenderMode_function_type( &::OSG::SkinnedGeometryBase::setRenderMode )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SkinnedGeometryBase::setSkeleton
        
            typedef void ( ::OSG::SkinnedGeometryBase::*setSkeleton_function_type )( ::OSG::Skeleton * const ) ;
            
            SkinnedGeometryBase_exposer.def( 
                "setSkeleton"
                , setSkeleton_function_type( &::OSG::SkinnedGeometryBase::setSkeleton )
                , ( bp::arg("value") ) );
        
        }
        SkinnedGeometryBase_exposer.staticmethod( "create" );
        SkinnedGeometryBase_exposer.staticmethod( "createDependent" );
        SkinnedGeometryBase_exposer.staticmethod( "createLocal" );
        SkinnedGeometryBase_exposer.staticmethod( "getClassGroupId" );
        SkinnedGeometryBase_exposer.staticmethod( "getClassType" );
        SkinnedGeometryBase_exposer.staticmethod( "getClassTypeId" );
        SkinnedGeometryBase_exposer.def("getMFJointIds",SkinnedGeometryBase_getMFJointIds);
    }

}
