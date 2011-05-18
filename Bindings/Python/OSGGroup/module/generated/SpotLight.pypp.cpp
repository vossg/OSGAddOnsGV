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
#include "SpotLight.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_SpotLight_class(){

    { //::OSG::SpotLight
        typedef bp::class_< OSG::SpotLight, bp::bases< OSG::SpotLightBase >, OSG::SpotLight::ObjRecPtr, boost::noncopyable > SpotLight_exposer_t;
        SpotLight_exposer_t SpotLight_exposer = SpotLight_exposer_t( "SpotLight", bp::no_init );
        bp::scope SpotLight_scope( SpotLight_exposer );
        { //::OSG::SpotLight::callLightEngineEnter
        
            typedef void ( ::OSG::SpotLight::*callLightEngineEnter_function_type )( ::OSG::RenderAction * ) ;
            
            SpotLight_exposer.def( 
                "callLightEngineEnter"
                , callLightEngineEnter_function_type( &::OSG::SpotLight::callLightEngineEnter )
                , ( bp::arg("ract") ) );
        
        }
        { //::OSG::SpotLight::callLightEngineLeave
        
            typedef void ( ::OSG::SpotLight::*callLightEngineLeave_function_type )( ::OSG::RenderAction * ) ;
            
            SpotLight_exposer.def( 
                "callLightEngineLeave"
                , callLightEngineLeave_function_type( &::OSG::SpotLight::callLightEngineLeave )
                , ( bp::arg("ract") ) );
        
        }
        { //::OSG::SpotLight::changed
        
            typedef void ( ::OSG::SpotLight::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            SpotLight_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::SpotLight::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("detail") ) );
        
        }
        { //::OSG::SpotLight::dump
        
            typedef void ( ::OSG::SpotLight::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            SpotLight_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::SpotLight::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::SpotLight::getSpotCutOffDeg
        
            typedef ::OSG::Real32 ( ::OSG::SpotLight::*getSpotCutOffDeg_function_type )(  ) ;
            
            SpotLight_exposer.def( 
                "getSpotCutOffDeg"
                , getSpotCutOffDeg_function_type( &::OSG::SpotLight::getSpotCutOffDeg ) );
        
        }
        { //::OSG::SpotLight::makeChunk
        
            typedef void ( ::OSG::SpotLight::*makeChunk_function_type )(  ) ;
            
            SpotLight_exposer.def( 
                "makeChunk"
                , makeChunk_function_type( &::OSG::SpotLight::makeChunk ) );
        
        }
        { //::OSG::SpotLight::setSpotCutOffDeg
        
            typedef void ( ::OSG::SpotLight::*setSpotCutOffDeg_function_type )( ::OSG::Real32 ) ;
            
            SpotLight_exposer.def( 
                "setSpotCutOffDeg"
                , setSpotCutOffDeg_function_type( &::OSG::SpotLight::setSpotCutOffDeg )
                , ( bp::arg("angle") ) );
        
        }
        { //::OSG::SpotLight::setSpotDirection
        
            typedef void ( ::OSG::SpotLight::*setSpotDirection_function_type )( ::OSG::Real32,::OSG::Real32,::OSG::Real32 ) ;
            
            SpotLight_exposer.def( 
                "setSpotDirection"
                , setSpotDirection_function_type( &::OSG::SpotLight::setSpotDirection )
                , ( bp::arg("rX"), bp::arg("rY"), bp::arg("rZ") ) );
        
        }
        SpotLight_exposer.def_readonly( "statNSpotLights", OSG::SpotLight::statNSpotLights );
        pyopensg::register_transit< OSG::SpotLight >::execute();
        bp::implicitly_convertible< OSG::SpotLight::ObjRecPtr, OSG::SpotLight* >();
        bp::implicitly_convertible< OSG::SpotLight::ObjRecPtr, OSG::SpotLight::ObjCPtr >();
        bp::implicitly_convertible< OSG::SpotLight::ObjRecPtr, OSG::PointLight* >();
        bp::implicitly_convertible< OSG::SpotLight::ObjRecPtr, OSG::PointLight::ObjRecPtr >();
        bp::implicitly_convertible<OSG::SpotLight::ObjRecPtr, OSG::PointLight::ObjCPtr>();
    }

}