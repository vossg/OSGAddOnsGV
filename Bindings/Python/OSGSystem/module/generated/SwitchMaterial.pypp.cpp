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
#include "SwitchMaterial.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_SwitchMaterial_class(){

    { //::OSG::SwitchMaterial
        typedef bp::class_< OSG::SwitchMaterial, bp::bases< OSG::SwitchMaterialBase >, OSG::SwitchMaterial::ObjRecPtr, boost::noncopyable > SwitchMaterial_exposer_t;
        SwitchMaterial_exposer_t SwitchMaterial_exposer = SwitchMaterial_exposer_t( "SwitchMaterial", bp::no_init );
        bp::scope SwitchMaterial_scope( SwitchMaterial_exposer );
        { //::OSG::SwitchMaterial::addMaterial
        
            typedef void ( ::OSG::SwitchMaterial::*addMaterial_function_type )( ::OSG::Material * ) ;
            
            SwitchMaterial_exposer.def( 
                "addMaterial"
                , addMaterial_function_type( &::OSG::SwitchMaterial::addMaterial )
                , ( bp::arg("mat") ) );
        
        }
        { //::OSG::SwitchMaterial::changed
        
            typedef void ( ::OSG::SwitchMaterial::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            SwitchMaterial_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::SwitchMaterial::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("detail") ) );
        
        }
        { //::OSG::SwitchMaterial::dump
        
            typedef void ( ::OSG::SwitchMaterial::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            SwitchMaterial_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::SwitchMaterial::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::SwitchMaterial::finalize
        
            typedef ::OSG::PrimeMaterial * ( ::OSG::SwitchMaterial::*finalize_function_type )( ::OSG::MaterialMapKey,::OSG::Window * ) ;
            
            SwitchMaterial_exposer.def( 
                "finalize"
                , finalize_function_type( &::OSG::SwitchMaterial::finalize )
                , ( bp::arg("oKey"), bp::arg("pWin") )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SwitchMaterial::getCurrentMaterial
        
            typedef ::OSG::Material * ( ::OSG::SwitchMaterial::*getCurrentMaterial_function_type )(  ) const;
            
            SwitchMaterial_exposer.def( 
                "getCurrentMaterial"
                , getCurrentMaterial_function_type( &::OSG::SwitchMaterial::getCurrentMaterial )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SwitchMaterial::getMaterial
        
            typedef ::OSG::Material * ( ::OSG::SwitchMaterial::*getMaterial_function_type )( ::OSG::UInt32 ) const;
            
            SwitchMaterial_exposer.def( 
                "getMaterial"
                , getMaterial_function_type( &::OSG::SwitchMaterial::getMaterial )
                , ( bp::arg("index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SwitchMaterial::hasMaterial
        
            typedef bool ( ::OSG::SwitchMaterial::*hasMaterial_function_type )( ::OSG::Material * ) const;
            
            SwitchMaterial_exposer.def( 
                "hasMaterial"
                , hasMaterial_function_type( &::OSG::SwitchMaterial::hasMaterial )
                , ( bp::arg("mat") ) );
        
        }
        { //::OSG::SwitchMaterial::isTransparent
        
            typedef bool ( ::OSG::SwitchMaterial::*isTransparent_function_type )(  ) const;
            
            SwitchMaterial_exposer.def( 
                "isTransparent"
                , isTransparent_function_type( &::OSG::SwitchMaterial::isTransparent ) );
        
        }
        { //::OSG::SwitchMaterial::subMaterial
        
            typedef void ( ::OSG::SwitchMaterial::*subMaterial_function_type )( ::OSG::Material * ) ;
            
            SwitchMaterial_exposer.def( 
                "subMaterial"
                , subMaterial_function_type( &::OSG::SwitchMaterial::subMaterial )
                , ( bp::arg("mat") ) );
        
        }
        pyopensg::register_transit< OSG::SwitchMaterial >::execute();
        bp::implicitly_convertible< OSG::SwitchMaterial::ObjRecPtr, OSG::SwitchMaterial* >();
        bp::implicitly_convertible< OSG::SwitchMaterial::ObjRecPtr, OSG::SwitchMaterial::ObjCPtr >();
        bp::implicitly_convertible< OSG::SwitchMaterial::ObjRecPtr, OSG::CompositeMaterial* >();
        bp::implicitly_convertible< OSG::SwitchMaterial::ObjRecPtr, OSG::CompositeMaterial::ObjRecPtr >();
        bp::implicitly_convertible<OSG::SwitchMaterial::ObjRecPtr, OSG::CompositeMaterial::ObjCPtr>();
    }

}
