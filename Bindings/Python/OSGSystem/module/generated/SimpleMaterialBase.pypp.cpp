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
#include "SimpleMaterialBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_SimpleMaterialBase_class(){

    { //::OSG::SimpleMaterialBase
        typedef bp::class_< OSG::SimpleMaterialBase, bp::bases< OSG::ChunkMaterial >, boost::noncopyable > SimpleMaterialBase_exposer_t;
        SimpleMaterialBase_exposer_t SimpleMaterialBase_exposer = SimpleMaterialBase_exposer_t( "SimpleMaterialBase", bp::no_init );
        bp::scope SimpleMaterialBase_scope( SimpleMaterialBase_exposer );
        bp::scope().attr("AmbientFieldId") = (int)OSG::SimpleMaterialBase::AmbientFieldId;
        bp::scope().attr("DiffuseFieldId") = (int)OSG::SimpleMaterialBase::DiffuseFieldId;
        bp::scope().attr("SpecularFieldId") = (int)OSG::SimpleMaterialBase::SpecularFieldId;
        bp::scope().attr("ShininessFieldId") = (int)OSG::SimpleMaterialBase::ShininessFieldId;
        bp::scope().attr("EmissionFieldId") = (int)OSG::SimpleMaterialBase::EmissionFieldId;
        bp::scope().attr("TransparencyFieldId") = (int)OSG::SimpleMaterialBase::TransparencyFieldId;
        bp::scope().attr("LitFieldId") = (int)OSG::SimpleMaterialBase::LitFieldId;
        bp::scope().attr("ColorMaterialFieldId") = (int)OSG::SimpleMaterialBase::ColorMaterialFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::SimpleMaterialBase::NextFieldId;
        { //::OSG::SimpleMaterialBase::copyFromBin
        
            typedef void ( ::OSG::SimpleMaterialBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            SimpleMaterialBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::SimpleMaterialBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::SimpleMaterialBase::copyToBin
        
            typedef void ( ::OSG::SimpleMaterialBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            SimpleMaterialBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::SimpleMaterialBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::SimpleMaterialBase::create
        
            typedef ::OSG::SimpleMaterialTransitPtr ( *create_function_type )(  );
            
            SimpleMaterialBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::SimpleMaterialBase::create ) );
        
        }
        { //::OSG::SimpleMaterialBase::createDependent
        
            typedef ::OSG::SimpleMaterialTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            SimpleMaterialBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::SimpleMaterialBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::SimpleMaterialBase::createLocal
        
            typedef ::OSG::SimpleMaterialTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            SimpleMaterialBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::SimpleMaterialBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::SimpleMaterialBase::getAmbient
        
            typedef ::OSG::Color3f const & ( ::OSG::SimpleMaterialBase::*getAmbient_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getAmbient"
                , getAmbient_function_type( &::OSG::SimpleMaterialBase::getAmbient )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleMaterialBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::SimpleMaterialBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            SimpleMaterialBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::SimpleMaterialBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::SimpleMaterialBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            SimpleMaterialBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::SimpleMaterialBase::getClassGroupId ) );
        
        }
        { //::OSG::SimpleMaterialBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            SimpleMaterialBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::SimpleMaterialBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::SimpleMaterialBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            SimpleMaterialBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::SimpleMaterialBase::getClassTypeId ) );
        
        }
        { //::OSG::SimpleMaterialBase::getColorMaterial
        
            typedef ::GLenum const & ( ::OSG::SimpleMaterialBase::*getColorMaterial_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getColorMaterial"
                , getColorMaterial_function_type( &::OSG::SimpleMaterialBase::getColorMaterial )
                , bp::return_value_policy< bp::return_by_value >() );
        
        }
        { //::OSG::SimpleMaterialBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::SimpleMaterialBase::*getContainerSize_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::SimpleMaterialBase::getContainerSize ) );
        
        }
        { //::OSG::SimpleMaterialBase::getDiffuse
        
            typedef ::OSG::Color3f const & ( ::OSG::SimpleMaterialBase::*getDiffuse_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getDiffuse"
                , getDiffuse_function_type( &::OSG::SimpleMaterialBase::getDiffuse )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleMaterialBase::getEmission
        
            typedef ::OSG::Color3f const & ( ::OSG::SimpleMaterialBase::*getEmission_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getEmission"
                , getEmission_function_type( &::OSG::SimpleMaterialBase::getEmission )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleMaterialBase::getLit
        
            typedef bool ( ::OSG::SimpleMaterialBase::*getLit_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getLit"
                , getLit_function_type( &::OSG::SimpleMaterialBase::getLit ) );
        
        }
        { //::OSG::SimpleMaterialBase::getSFAmbient
        
            typedef ::OSG::SFColor3f const * ( ::OSG::SimpleMaterialBase::*getSFAmbient_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getSFAmbient"
                , getSFAmbient_function_type( &::OSG::SimpleMaterialBase::getSFAmbient )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::getSFColorMaterial
        
            typedef ::OSG::SFGLenum const * ( ::OSG::SimpleMaterialBase::*getSFColorMaterial_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getSFColorMaterial"
                , getSFColorMaterial_function_type( &::OSG::SimpleMaterialBase::getSFColorMaterial )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::getSFDiffuse
        
            typedef ::OSG::SFColor3f const * ( ::OSG::SimpleMaterialBase::*getSFDiffuse_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getSFDiffuse"
                , getSFDiffuse_function_type( &::OSG::SimpleMaterialBase::getSFDiffuse )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::getSFEmission
        
            typedef ::OSG::SFColor3f const * ( ::OSG::SimpleMaterialBase::*getSFEmission_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getSFEmission"
                , getSFEmission_function_type( &::OSG::SimpleMaterialBase::getSFEmission )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::getSFLit
        
            typedef ::OSG::SFBool const * ( ::OSG::SimpleMaterialBase::*getSFLit_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getSFLit"
                , getSFLit_function_type( &::OSG::SimpleMaterialBase::getSFLit )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::getSFShininess
        
            typedef ::OSG::SFReal32 const * ( ::OSG::SimpleMaterialBase::*getSFShininess_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getSFShininess"
                , getSFShininess_function_type( &::OSG::SimpleMaterialBase::getSFShininess )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::getSFSpecular
        
            typedef ::OSG::SFColor3f const * ( ::OSG::SimpleMaterialBase::*getSFSpecular_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getSFSpecular"
                , getSFSpecular_function_type( &::OSG::SimpleMaterialBase::getSFSpecular )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::getSFTransparency
        
            typedef ::OSG::SFReal32 const * ( ::OSG::SimpleMaterialBase::*getSFTransparency_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getSFTransparency"
                , getSFTransparency_function_type( &::OSG::SimpleMaterialBase::getSFTransparency )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::getShininess
        
            typedef ::OSG::Real32 ( ::OSG::SimpleMaterialBase::*getShininess_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getShininess"
                , getShininess_function_type( &::OSG::SimpleMaterialBase::getShininess ) );
        
        }
        { //::OSG::SimpleMaterialBase::getSpecular
        
            typedef ::OSG::Color3f const & ( ::OSG::SimpleMaterialBase::*getSpecular_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getSpecular"
                , getSpecular_function_type( &::OSG::SimpleMaterialBase::getSpecular )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        { //::OSG::SimpleMaterialBase::getTransparency
        
            typedef ::OSG::Real32 ( ::OSG::SimpleMaterialBase::*getTransparency_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getTransparency"
                , getTransparency_function_type( &::OSG::SimpleMaterialBase::getTransparency ) );
        
        }
        { //::OSG::SimpleMaterialBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::SimpleMaterialBase::*getType_function_type )(  ) ;
            
            SimpleMaterialBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::SimpleMaterialBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::SimpleMaterialBase::*getType_function_type )(  ) const;
            
            SimpleMaterialBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::SimpleMaterialBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleMaterialBase::setAmbient
        
            typedef void ( ::OSG::SimpleMaterialBase::*setAmbient_function_type )( ::OSG::Color3f const & ) ;
            
            SimpleMaterialBase_exposer.def( 
                "setAmbient"
                , setAmbient_function_type( &::OSG::SimpleMaterialBase::setAmbient )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleMaterialBase::setColorMaterial
        
            typedef void ( ::OSG::SimpleMaterialBase::*setColorMaterial_function_type )( ::GLenum const & ) ;
            
            SimpleMaterialBase_exposer.def( 
                "setColorMaterial"
                , setColorMaterial_function_type( &::OSG::SimpleMaterialBase::setColorMaterial )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleMaterialBase::setDiffuse
        
            typedef void ( ::OSG::SimpleMaterialBase::*setDiffuse_function_type )( ::OSG::Color3f const & ) ;
            
            SimpleMaterialBase_exposer.def( 
                "setDiffuse"
                , setDiffuse_function_type( &::OSG::SimpleMaterialBase::setDiffuse )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleMaterialBase::setEmission
        
            typedef void ( ::OSG::SimpleMaterialBase::*setEmission_function_type )( ::OSG::Color3f const & ) ;
            
            SimpleMaterialBase_exposer.def( 
                "setEmission"
                , setEmission_function_type( &::OSG::SimpleMaterialBase::setEmission )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleMaterialBase::setLit
        
            typedef void ( ::OSG::SimpleMaterialBase::*setLit_function_type )( bool const ) ;
            
            SimpleMaterialBase_exposer.def( 
                "setLit"
                , setLit_function_type( &::OSG::SimpleMaterialBase::setLit )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleMaterialBase::setShininess
        
            typedef void ( ::OSG::SimpleMaterialBase::*setShininess_function_type )( ::OSG::Real32 const ) ;
            
            SimpleMaterialBase_exposer.def( 
                "setShininess"
                , setShininess_function_type( &::OSG::SimpleMaterialBase::setShininess )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleMaterialBase::setSpecular
        
            typedef void ( ::OSG::SimpleMaterialBase::*setSpecular_function_type )( ::OSG::Color3f const & ) ;
            
            SimpleMaterialBase_exposer.def( 
                "setSpecular"
                , setSpecular_function_type( &::OSG::SimpleMaterialBase::setSpecular )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::SimpleMaterialBase::setTransparency
        
            typedef void ( ::OSG::SimpleMaterialBase::*setTransparency_function_type )( ::OSG::Real32 const ) ;
            
            SimpleMaterialBase_exposer.def( 
                "setTransparency"
                , setTransparency_function_type( &::OSG::SimpleMaterialBase::setTransparency )
                , ( bp::arg("value") ) );
        
        }
        SimpleMaterialBase_exposer.staticmethod( "create" );
        SimpleMaterialBase_exposer.staticmethod( "createDependent" );
        SimpleMaterialBase_exposer.staticmethod( "createLocal" );
        SimpleMaterialBase_exposer.staticmethod( "getClassGroupId" );
        SimpleMaterialBase_exposer.staticmethod( "getClassType" );
        SimpleMaterialBase_exposer.staticmethod( "getClassTypeId" );
    }

}
