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
#include "OSGFileIO_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "VRMLAttachmentBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_VRMLAttachmentBase_class(){

    { //::OSG::VRMLAttachmentBase
        typedef bp::class_< OSG::VRMLAttachmentBase, bp::bases< ::OSG::Attachment >, boost::noncopyable > VRMLAttachmentBase_exposer_t;
        VRMLAttachmentBase_exposer_t VRMLAttachmentBase_exposer = VRMLAttachmentBase_exposer_t( "VRMLAttachmentBase", bp::no_init );
        bp::scope VRMLAttachmentBase_scope( VRMLAttachmentBase_exposer );
        bp::scope().attr("VrmlNodeTypenameFieldId") = (int)OSG::VRMLAttachmentBase::VrmlNodeTypenameFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::VRMLAttachmentBase::NextFieldId;
        { //::OSG::VRMLAttachmentBase::copyFromBin
        
            typedef void ( ::OSG::VRMLAttachmentBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            VRMLAttachmentBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::VRMLAttachmentBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::VRMLAttachmentBase::copyToBin
        
            typedef void ( ::OSG::VRMLAttachmentBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            VRMLAttachmentBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::VRMLAttachmentBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::VRMLAttachmentBase::create
        
            typedef ::OSG::VRMLAttachmentTransitPtr ( *create_function_type )(  );
            
            VRMLAttachmentBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::VRMLAttachmentBase::create ) );
        
        }
        { //::OSG::VRMLAttachmentBase::createDependent
        
            typedef ::OSG::VRMLAttachmentTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            VRMLAttachmentBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::VRMLAttachmentBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::VRMLAttachmentBase::createLocal
        
            typedef ::OSG::VRMLAttachmentTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            VRMLAttachmentBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::VRMLAttachmentBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::VRMLAttachmentBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::VRMLAttachmentBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            VRMLAttachmentBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::VRMLAttachmentBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::VRMLAttachmentBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            VRMLAttachmentBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::VRMLAttachmentBase::getClassGroupId ) );
        
        }
        { //::OSG::VRMLAttachmentBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            VRMLAttachmentBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::VRMLAttachmentBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::VRMLAttachmentBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            VRMLAttachmentBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::VRMLAttachmentBase::getClassTypeId ) );
        
        }
        { //::OSG::VRMLAttachmentBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::VRMLAttachmentBase::*getContainerSize_function_type )(  ) const;
            
            VRMLAttachmentBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::VRMLAttachmentBase::getContainerSize ) );
        
        }
        { //::OSG::VRMLAttachmentBase::getSFVrmlNodeTypename
        
            typedef ::OSG::SFString const * ( ::OSG::VRMLAttachmentBase::*getSFVrmlNodeTypename_function_type )(  ) const;
            
            VRMLAttachmentBase_exposer.def( 
                "getSFVrmlNodeTypename"
                , getSFVrmlNodeTypename_function_type( &::OSG::VRMLAttachmentBase::getSFVrmlNodeTypename )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::VRMLAttachmentBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::VRMLAttachmentBase::*getType_function_type )(  ) ;
            
            VRMLAttachmentBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::VRMLAttachmentBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::VRMLAttachmentBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::VRMLAttachmentBase::*getType_function_type )(  ) const;
            
            VRMLAttachmentBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::VRMLAttachmentBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::VRMLAttachmentBase::getVrmlNodeTypename
        
            typedef ::std::string const & ( ::OSG::VRMLAttachmentBase::*getVrmlNodeTypename_function_type )(  ) const;
            
            VRMLAttachmentBase_exposer.def( 
                "getVrmlNodeTypename"
                , getVrmlNodeTypename_function_type( &::OSG::VRMLAttachmentBase::getVrmlNodeTypename )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::VRMLAttachmentBase::setVrmlNodeTypename
        
            typedef void ( ::OSG::VRMLAttachmentBase::*setVrmlNodeTypename_function_type )( ::std::string const & ) ;
            
            VRMLAttachmentBase_exposer.def( 
                "setVrmlNodeTypename"
                , setVrmlNodeTypename_function_type( &::OSG::VRMLAttachmentBase::setVrmlNodeTypename )
                , ( bp::arg("value") ) );
        
        }
        VRMLAttachmentBase_exposer.staticmethod( "create" );
        VRMLAttachmentBase_exposer.staticmethod( "createDependent" );
        VRMLAttachmentBase_exposer.staticmethod( "createLocal" );
        VRMLAttachmentBase_exposer.staticmethod( "getClassGroupId" );
        VRMLAttachmentBase_exposer.staticmethod( "getClassType" );
        VRMLAttachmentBase_exposer.staticmethod( "getClassTypeId" );
    }

}
