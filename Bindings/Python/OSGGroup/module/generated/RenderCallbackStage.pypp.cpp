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
#include "RenderCallbackStage.pypp.hpp"

using namespace std;
namespace bp = boost::python;

struct RenderCallbackStage_wrapper : OSG::RenderCallbackStage, bp::wrapper< OSG::RenderCallbackStage > {

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

namespace
{

void renderCallback(bp::object callable, OSG::DrawEnv* drawEnv)
{
   callable(drawEnv);
}

void wrapAddPreRenderFunctor(OSG::RenderCallbackStage* self,
                             bp::object callable,
                             const std::string& createSymbol)
{
   self->addPreRenderFunctor(boost::bind(renderCallback, callable, _1),
                             createSymbol);
}

void wrapAddPostRenderFunctor(OSG::RenderCallbackStage* self,
                              bp::object callable,
                              const std::string& createSymbol)
{
   self->addPostRenderFunctor(boost::bind(renderCallback, callable, _1),
                              createSymbol);
}

inline void
fillFunctorStore(const bp::list& callables,
                 OSG::RenderCallbackStage::RenderFunctorStore& store)
{
   const int length = bp::extract<int>(callables.attr("__len__")());
   store.resize(length);

   for ( int i = 0; i < length; ++i )
   {
      store[i] = boost::bind(renderCallback, callables[i], _1);
   }
}

void wrapFillPreRenderStore(OSG::RenderCallbackStage* self, bp::list callables)
{
   OSG::RenderCallbackStage::RenderFunctorStore store;
   fillFunctorStore(callables, store);
   self->fillPreRenderStore(store);
}

void wrapFillPostRenderStore(OSG::RenderCallbackStage* self, bp::list callables)
{
   OSG::RenderCallbackStage::RenderFunctorStore store;
   fillFunctorStore(callables, store);
   self->fillPostRenderStore(store);
}

}

void register_RenderCallbackStage_class(){

    { //::OSG::RenderCallbackStage
        typedef bp::class_< RenderCallbackStage_wrapper, bp::bases< OSG::RenderCallbackStageBase >, OSG::RenderCallbackStage::ObjRecPtr, boost::noncopyable > RenderCallbackStage_exposer_t;
        RenderCallbackStage_exposer_t RenderCallbackStage_exposer = RenderCallbackStage_exposer_t( "RenderCallbackStage", bp::no_init );
        bp::scope RenderCallbackStage_scope( RenderCallbackStage_exposer );
        { //::OSG::RenderCallbackStage::changed
        
            typedef void ( ::OSG::RenderCallbackStage::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            RenderCallbackStage_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::RenderCallbackStage::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("detail") ) );
        
        }
        { //::OSG::RenderCallbackStage::clearPostRenderFunctors
        
            typedef void ( ::OSG::RenderCallbackStage::*clearPostRenderFunctors_function_type )(  ) ;
            
            RenderCallbackStage_exposer.def( 
                "clearPostRenderFunctors"
                , clearPostRenderFunctors_function_type( &::OSG::RenderCallbackStage::clearPostRenderFunctors ) );
        
        }
        { //::OSG::RenderCallbackStage::clearPreRenderFunctors
        
            typedef void ( ::OSG::RenderCallbackStage::*clearPreRenderFunctors_function_type )(  ) ;
            
            RenderCallbackStage_exposer.def( 
                "clearPreRenderFunctors"
                , clearPreRenderFunctors_function_type( &::OSG::RenderCallbackStage::clearPreRenderFunctors ) );
        
        }
        { //::OSG::RenderCallbackStage::dump
        
            typedef void ( ::OSG::RenderCallbackStage::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            RenderCallbackStage_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::RenderCallbackStage::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::RenderCallbackStage::subPostRenderFunctor
        
            typedef void ( ::OSG::RenderCallbackStage::*subPostRenderFunctor_function_type )( ::OSG::UInt32 ) ;
            
            RenderCallbackStage_exposer.def( 
                "subPostRenderFunctor"
                , subPostRenderFunctor_function_type( &::OSG::RenderCallbackStage::subPostRenderFunctor )
                , ( bp::arg("uiId") ) );
        
        }
        { //::OSG::RenderCallbackStage::subPreRenderFunctor
        
            typedef void ( ::OSG::RenderCallbackStage::*subPreRenderFunctor_function_type )( ::OSG::UInt32 ) ;
            
            RenderCallbackStage_exposer.def( 
                "subPreRenderFunctor"
                , subPreRenderFunctor_function_type( &::OSG::RenderCallbackStage::subPreRenderFunctor )
                , ( bp::arg("uiId") ) );
        
        }
        { //::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyFromBin
        
            typedef OSG::RenderCallbackStage exported_class_t;
            typedef void ( exported_class_t::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            typedef void ( RenderCallbackStage_wrapper::*default_copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            RenderCallbackStage_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type(&::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyFromBin)
                , default_copyFromBin_function_type(&RenderCallbackStage_wrapper::default_copyFromBin)
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyToBin
        
            typedef OSG::RenderCallbackStage exported_class_t;
            typedef void ( exported_class_t::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            typedef void ( RenderCallbackStage_wrapper::*default_copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            RenderCallbackStage_exposer.def( 
                "copyToBin"
                , copyToBin_function_type(&::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::copyToBin)
                , default_copyToBin_function_type(&RenderCallbackStage_wrapper::default_copyToBin)
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::getBinSize
        
            typedef OSG::RenderCallbackStage exported_class_t;
            typedef ::OSG::UInt32 ( exported_class_t::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            typedef ::OSG::UInt32 ( RenderCallbackStage_wrapper::*default_getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            RenderCallbackStage_exposer.def( 
                "getBinSize"
                , getBinSize_function_type(&::OSG::TraversalValidationHandlerMixin< OSG::ContainerMixinHead< OSG::StageDesc > >::getBinSize)
                , default_getBinSize_function_type(&RenderCallbackStage_wrapper::default_getBinSize)
                , ( bp::arg("whichField") ) );
        
        }
        pyopensg::register_transit< OSG::RenderCallbackStage >::execute();
        bp::implicitly_convertible< OSG::RenderCallbackStage::ObjRecPtr, OSG::RenderCallbackStage* >();
        bp::implicitly_convertible< OSG::RenderCallbackStage::ObjRecPtr, OSG::RenderCallbackStage::ObjCPtr >();
        bp::implicitly_convertible< OSG::RenderCallbackStage::ObjRecPtr, OSG::Stage* >();
        bp::implicitly_convertible< OSG::RenderCallbackStage::ObjRecPtr, OSG::Stage::ObjRecPtr >();
        bp::implicitly_convertible<OSG::RenderCallbackStage::ObjRecPtr, OSG::Stage::ObjCPtr>();
        RenderCallbackStage_exposer.def("addPreRenderFunctor",wrapAddPreRenderFunctor);
        RenderCallbackStage_exposer.def("addPostRenderFunctor",wrapAddPostRenderFunctor);
        RenderCallbackStage_exposer.def("fillPreRenderStore",wrapFillPreRenderStore);
        RenderCallbackStage_exposer.def("fillPostRenderStore",wrapFillPostRenderStore);
    }

}
