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
#include "AnimDataSourceBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_AnimDataSourceBase_class(){

    { //::OSG::AnimDataSourceBase
        typedef bp::class_< OSG::AnimDataSourceBase, bp::bases< ::OSG::AttachmentContainer >, boost::noncopyable > AnimDataSourceBase_exposer_t;
        AnimDataSourceBase_exposer_t AnimDataSourceBase_exposer = AnimDataSourceBase_exposer_t( "AnimDataSourceBase", bp::no_init );
        bp::scope AnimDataSourceBase_scope( AnimDataSourceBase_exposer );
        { //::OSG::AnimDataSourceBase::copyFromBin
        
            typedef void ( ::OSG::AnimDataSourceBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            AnimDataSourceBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::AnimDataSourceBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::AnimDataSourceBase::copyToBin
        
            typedef void ( ::OSG::AnimDataSourceBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            AnimDataSourceBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::AnimDataSourceBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::AnimDataSourceBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::AnimDataSourceBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            AnimDataSourceBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::AnimDataSourceBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::AnimDataSourceBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            AnimDataSourceBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::AnimDataSourceBase::getClassGroupId ) );
        
        }
        { //::OSG::AnimDataSourceBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            AnimDataSourceBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::AnimDataSourceBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::AnimDataSourceBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            AnimDataSourceBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::AnimDataSourceBase::getClassTypeId ) );
        
        }
        { //::OSG::AnimDataSourceBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::AnimDataSourceBase::*getContainerSize_function_type )(  ) const;
            
            AnimDataSourceBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::AnimDataSourceBase::getContainerSize ) );
        
        }
        { //::OSG::AnimDataSourceBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::AnimDataSourceBase::*getType_function_type )(  ) ;
            
            AnimDataSourceBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::AnimDataSourceBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::AnimDataSourceBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::AnimDataSourceBase::*getType_function_type )(  ) const;
            
            AnimDataSourceBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::AnimDataSourceBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        AnimDataSourceBase_exposer.staticmethod( "getClassGroupId" );
        AnimDataSourceBase_exposer.staticmethod( "getClassType" );
        AnimDataSourceBase_exposer.staticmethod( "getClassTypeId" );
    }

}
