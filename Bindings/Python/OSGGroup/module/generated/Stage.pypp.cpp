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
#include "Stage.pypp.hpp"

using namespace std;
namespace bp = boost::python;

struct Stage_wrapper : OSG::Stage, bp::wrapper< OSG::Stage > {

    virtual void copyFromBin( ::OSG::BinaryDataHandler & pMem, ::OSG::ConstFieldMaskArg whichField ) {
        if( bp::override func_copyFromBin = this->get_override( "copyFromBin" ) )
            func_copyFromBin( boost::ref(pMem), whichField );
        else{
            this->OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyFromBin( boost::ref(pMem), whichField );
        }
    }
    
    void default_copyFromBin( ::OSG::BinaryDataHandler & pMem, ::OSG::ConstFieldMaskArg whichField ) {
        OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyFromBin( boost::ref(pMem), whichField );
    }

    virtual void copyToBin( ::OSG::BinaryDataHandler & pMem, ::OSG::ConstFieldMaskArg whichField ) {
        if( bp::override func_copyToBin = this->get_override( "copyToBin" ) )
            func_copyToBin( boost::ref(pMem), whichField );
        else{
            this->OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyToBin( boost::ref(pMem), whichField );
        }
    }
    
    void default_copyToBin( ::OSG::BinaryDataHandler & pMem, ::OSG::ConstFieldMaskArg whichField ) {
        OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyToBin( boost::ref(pMem), whichField );
    }

    virtual ::OSG::UInt32 getBinSize( ::OSG::ConstFieldMaskArg whichField ) {
        if( bp::override func_getBinSize = this->get_override( "getBinSize" ) )
            return func_getBinSize( whichField );
        else{
            return this->OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::getBinSize( whichField );
        }
    }
    
    ::OSG::UInt32 default_getBinSize( ::OSG::ConstFieldMaskArg whichField ) {
        return OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::getBinSize( whichField );
    }

};

void register_Stage_class(){

    { //::OSG::Stage
        typedef bp::class_< Stage_wrapper, bp::bases< OSG::StageBase >, OSG::Stage::ObjRecPtr, boost::noncopyable > Stage_exposer_t;
        Stage_exposer_t Stage_exposer = Stage_exposer_t( "Stage", bp::no_init );
        bp::scope Stage_scope( Stage_exposer );
        { //::OSG::Stage::changed
        
            typedef void ( ::OSG::Stage::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            Stage_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::Stage::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("detail") ) );
        
        }
        { //::OSG::Stage::dump
        
            typedef void ( ::OSG::Stage::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            Stage_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::Stage::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyFromBin
        
            typedef OSG::Stage exported_class_t;
            typedef void ( exported_class_t::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            typedef void ( Stage_wrapper::*default_copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            Stage_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type(&::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyFromBin)
                , default_copyFromBin_function_type(&Stage_wrapper::default_copyFromBin)
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyToBin
        
            typedef OSG::Stage exported_class_t;
            typedef void ( exported_class_t::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            typedef void ( Stage_wrapper::*default_copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            Stage_exposer.def( 
                "copyToBin"
                , copyToBin_function_type(&::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyToBin)
                , default_copyToBin_function_type(&Stage_wrapper::default_copyToBin)
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::getBinSize
        
            typedef OSG::Stage exported_class_t;
            typedef ::OSG::UInt32 ( exported_class_t::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            typedef ::OSG::UInt32 ( Stage_wrapper::*default_getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            Stage_exposer.def( 
                "getBinSize"
                , getBinSize_function_type(&::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::getBinSize)
                , default_getBinSize_function_type(&Stage_wrapper::default_getBinSize)
                , ( bp::arg("whichField") ) );
        
        }
        pyopensg::register_transit< OSG::Stage >::execute();
        bp::implicitly_convertible< OSG::Stage::ObjRecPtr, OSG::Stage* >();
        bp::implicitly_convertible< OSG::Stage::ObjRecPtr, OSG::Stage::ObjCPtr >();
        bp::implicitly_convertible< OSG::Stage::ObjRecPtr, OSG::Group* >();
        bp::implicitly_convertible< OSG::Stage::ObjRecPtr, OSG::Group::ObjRecPtr >();
        bp::implicitly_convertible<OSG::Stage::ObjRecPtr, OSG::Group::ObjCPtr>();
    }

}
