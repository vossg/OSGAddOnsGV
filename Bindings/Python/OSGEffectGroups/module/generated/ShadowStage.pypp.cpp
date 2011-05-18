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
#include "OSGEffectGroups_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "ShadowStage.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_ShadowStage_class(){

    { //::OSG::ShadowStage
        typedef bp::class_< OSG::ShadowStage, bp::bases< OSG::ShadowStageBase >, OSG::ShadowStage::ObjRecPtr, boost::noncopyable > ShadowStage_exposer_t;
        ShadowStage_exposer_t ShadowStage_exposer = ShadowStage_exposer_t( "ShadowStage", bp::no_init );
        bp::scope ShadowStage_scope( ShadowStage_exposer );
        bp::enum_< OSG::ShadowStage::Mode>("Mode")
            .value("NO_SHADOW", OSG::ShadowStage::NO_SHADOW)
            .value("STD_SHADOW_MAP", OSG::ShadowStage::STD_SHADOW_MAP)
            .value("PERSPECTIVE_SHADOW_MAP", OSG::ShadowStage::PERSPECTIVE_SHADOW_MAP)
            .value("DITHER_SHADOW_MAP", OSG::ShadowStage::DITHER_SHADOW_MAP)
            .value("PCF_SHADOW_MAP", OSG::ShadowStage::PCF_SHADOW_MAP)
            .value("PCSS_SHADOW_MAP", OSG::ShadowStage::PCSS_SHADOW_MAP)
            .value("VARIANCE_SHADOW_MAP", OSG::ShadowStage::VARIANCE_SHADOW_MAP)
            .export_values()
            ;
        { //::OSG::ShadowStage::changed
        
            typedef void ( ::OSG::ShadowStage::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            ShadowStage_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::ShadowStage::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::ShadowStage::checkLightsOcclusion
        
            typedef void ( ::OSG::ShadowStage::*checkLightsOcclusion_function_type )( ::OSG::RenderActionBase * ) ;
            
            ShadowStage_exposer.def( 
                "checkLightsOcclusion"
                , checkLightsOcclusion_function_type( &::OSG::ShadowStage::checkLightsOcclusion )
                , ( bp::arg("action") ) );
        
        }
        { //::OSG::ShadowStage::drawOcclusionBB
        
            typedef void ( ::OSG::ShadowStage::*drawOcclusionBB_function_type )( ::OSG::Pnt3f const &,::OSG::Pnt3f const & ) ;
            
            ShadowStage_exposer.def( 
                "drawOcclusionBB"
                , drawOcclusionBB_function_type( &::OSG::ShadowStage::drawOcclusionBB )
                , ( bp::arg("bbmin"), bp::arg("bbmax") ) );
        
        }
        { //::OSG::ShadowStage::dump
        
            typedef void ( ::OSG::ShadowStage::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            ShadowStage_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::ShadowStage::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::ShadowStage::triggerMapUpdate
        
            typedef void ( ::OSG::ShadowStage::*triggerMapUpdate_function_type )(  ) ;
            
            ShadowStage_exposer.def( 
                "triggerMapUpdate"
                , triggerMapUpdate_function_type( &::OSG::ShadowStage::triggerMapUpdate ) );
        
        }
        pyopensg::register_transit< OSG::ShadowStage >::execute();
        bp::implicitly_convertible< OSG::ShadowStage::ObjRecPtr, OSG::ShadowStage* >();
        bp::implicitly_convertible< OSG::ShadowStage::ObjRecPtr, OSG::ShadowStage::ObjCPtr >();
        bp::implicitly_convertible< OSG::ShadowStage::ObjRecPtr, OSG::Stage* >();
        bp::implicitly_convertible< OSG::ShadowStage::ObjRecPtr, OSG::Stage::ObjRecPtr >();
        bp::implicitly_convertible<OSG::ShadowStage::ObjRecPtr, OSG::Stage::ObjCPtr>();
    }

}