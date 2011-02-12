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
#include "ShadowStageBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

boost::python::list ShadowStageBase_getMFLightNodes(OSG::ShadowStageBase *self)
{
   boost::python::list result;
   OSG::MFUnrecNodePtr const * mf_data = self->getMFLightNodes();
   const OSG::UInt32 size(mf_data->size());
   for ( OSG::UInt32 i = 0; i < size; ++i )
   {
      result.append(OSG::Node::ObjRecPtr((*mf_data)[i]));
   }
   return result;
}

boost::python::list ShadowStageBase_getMFExcludeNodes(OSG::ShadowStageBase *self)
{
   boost::python::list result;
   OSG::MFUnrecNodePtr const * mf_data = self->getMFExcludeNodes();
   const OSG::UInt32 size(mf_data->size());
   for ( OSG::UInt32 i = 0; i < size; ++i )
   {
      result.append(OSG::Node::ObjRecPtr((*mf_data)[i]));
   }
   return result;
}

void register_ShadowStageBase_class(){

    { //::OSG::ShadowStageBase
        typedef bp::class_< OSG::ShadowStageBase, bp::bases< ::OSG::Stage >, boost::noncopyable > ShadowStageBase_exposer_t;
        ShadowStageBase_exposer_t ShadowStageBase_exposer = ShadowStageBase_exposer_t( "ShadowStageBase", bp::no_init );
        bp::scope ShadowStageBase_scope( ShadowStageBase_exposer );
        bp::scope().attr("OffBiasFieldId") = (int)OSG::ShadowStageBase::OffBiasFieldId;
        bp::scope().attr("OffFactorFieldId") = (int)OSG::ShadowStageBase::OffFactorFieldId;
        bp::scope().attr("MapSizeFieldId") = (int)OSG::ShadowStageBase::MapSizeFieldId;
        bp::scope().attr("LightNodesFieldId") = (int)OSG::ShadowStageBase::LightNodesFieldId;
        bp::scope().attr("ExcludeNodesFieldId") = (int)OSG::ShadowStageBase::ExcludeNodesFieldId;
        bp::scope().attr("MapAutoUpdateFieldId") = (int)OSG::ShadowStageBase::MapAutoUpdateFieldId;
        bp::scope().attr("ShadowModeFieldId") = (int)OSG::ShadowStageBase::ShadowModeFieldId;
        bp::scope().attr("ShadowSmoothnessFieldId") = (int)OSG::ShadowStageBase::ShadowSmoothnessFieldId;
        bp::scope().attr("ShadowOnFieldId") = (int)OSG::ShadowStageBase::ShadowOnFieldId;
        bp::scope().attr("AutoSearchForLightsFieldId") = (int)OSG::ShadowStageBase::AutoSearchForLightsFieldId;
        bp::scope().attr("GlobalShadowIntensityFieldId") = (int)OSG::ShadowStageBase::GlobalShadowIntensityFieldId;
        bp::scope().attr("FboOnFieldId") = (int)OSG::ShadowStageBase::FboOnFieldId;
        bp::scope().attr("AutoExcludeTransparentNodesFieldId") = (int)OSG::ShadowStageBase::AutoExcludeTransparentNodesFieldId;
        bp::scope().attr("DisableOccludedLightsFieldId") = (int)OSG::ShadowStageBase::DisableOccludedLightsFieldId;
        bp::scope().attr("RedFieldId") = (int)OSG::ShadowStageBase::RedFieldId;
        bp::scope().attr("BlueFieldId") = (int)OSG::ShadowStageBase::BlueFieldId;
        bp::scope().attr("GreenFieldId") = (int)OSG::ShadowStageBase::GreenFieldId;
        bp::scope().attr("AlphaFieldId") = (int)OSG::ShadowStageBase::AlphaFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::ShadowStageBase::NextFieldId;
        { //::OSG::ShadowStageBase::assignExcludeNodes
        
            typedef void ( ::OSG::ShadowStageBase::*assignExcludeNodes_function_type )( ::OSG::MFUnrecNodePtr const & ) ;
            
            ShadowStageBase_exposer.def( 
                "assignExcludeNodes"
                , assignExcludeNodes_function_type( &::OSG::ShadowStageBase::assignExcludeNodes )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::assignLightNodes
        
            typedef void ( ::OSG::ShadowStageBase::*assignLightNodes_function_type )( ::OSG::MFUnrecNodePtr const & ) ;
            
            ShadowStageBase_exposer.def( 
                "assignLightNodes"
                , assignLightNodes_function_type( &::OSG::ShadowStageBase::assignLightNodes )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::clearExcludeNodes
        
            typedef void ( ::OSG::ShadowStageBase::*clearExcludeNodes_function_type )(  ) ;
            
            ShadowStageBase_exposer.def( 
                "clearExcludeNodes"
                , clearExcludeNodes_function_type( &::OSG::ShadowStageBase::clearExcludeNodes ) );
        
        }
        { //::OSG::ShadowStageBase::clearLightNodes
        
            typedef void ( ::OSG::ShadowStageBase::*clearLightNodes_function_type )(  ) ;
            
            ShadowStageBase_exposer.def( 
                "clearLightNodes"
                , clearLightNodes_function_type( &::OSG::ShadowStageBase::clearLightNodes ) );
        
        }
        { //::OSG::ShadowStageBase::copyFromBin
        
            typedef void ( ::OSG::ShadowStageBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            ShadowStageBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::ShadowStageBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::ShadowStageBase::copyToBin
        
            typedef void ( ::OSG::ShadowStageBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            ShadowStageBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::ShadowStageBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::ShadowStageBase::create
        
            typedef ::OSG::ShadowStageTransitPtr ( *create_function_type )(  );
            
            ShadowStageBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::ShadowStageBase::create ) );
        
        }
        { //::OSG::ShadowStageBase::createDependent
        
            typedef ::OSG::ShadowStageTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            ShadowStageBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::ShadowStageBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::ShadowStageBase::createLocal
        
            typedef ::OSG::ShadowStageTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            ShadowStageBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::ShadowStageBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::ShadowStageBase::getAlpha
        
            typedef bool ( ::OSG::ShadowStageBase::*getAlpha_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getAlpha"
                , getAlpha_function_type( &::OSG::ShadowStageBase::getAlpha ) );
        
        }
        { //::OSG::ShadowStageBase::getAutoExcludeTransparentNodes
        
            typedef bool ( ::OSG::ShadowStageBase::*getAutoExcludeTransparentNodes_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getAutoExcludeTransparentNodes"
                , getAutoExcludeTransparentNodes_function_type( &::OSG::ShadowStageBase::getAutoExcludeTransparentNodes ) );
        
        }
        { //::OSG::ShadowStageBase::getAutoSearchForLights
        
            typedef bool ( ::OSG::ShadowStageBase::*getAutoSearchForLights_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getAutoSearchForLights"
                , getAutoSearchForLights_function_type( &::OSG::ShadowStageBase::getAutoSearchForLights ) );
        
        }
        { //::OSG::ShadowStageBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::ShadowStageBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            ShadowStageBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::ShadowStageBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::ShadowStageBase::getBlue
        
            typedef bool ( ::OSG::ShadowStageBase::*getBlue_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getBlue"
                , getBlue_function_type( &::OSG::ShadowStageBase::getBlue ) );
        
        }
        { //::OSG::ShadowStageBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            ShadowStageBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::ShadowStageBase::getClassGroupId ) );
        
        }
        { //::OSG::ShadowStageBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            ShadowStageBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::ShadowStageBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::ShadowStageBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            ShadowStageBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::ShadowStageBase::getClassTypeId ) );
        
        }
        { //::OSG::ShadowStageBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::ShadowStageBase::*getContainerSize_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::ShadowStageBase::getContainerSize ) );
        
        }
        { //::OSG::ShadowStageBase::getDisableOccludedLights
        
            typedef bool ( ::OSG::ShadowStageBase::*getDisableOccludedLights_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getDisableOccludedLights"
                , getDisableOccludedLights_function_type( &::OSG::ShadowStageBase::getDisableOccludedLights ) );
        
        }
        { //::OSG::ShadowStageBase::getExcludeNodes
        
            typedef ::OSG::Node * ( ::OSG::ShadowStageBase::*getExcludeNodes_function_type )( ::OSG::UInt32 const ) const;
            
            ShadowStageBase_exposer.def( 
                "getExcludeNodes"
                , getExcludeNodes_function_type( &::OSG::ShadowStageBase::getExcludeNodes )
                , ( bp::arg("index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getFboOn
        
            typedef bool ( ::OSG::ShadowStageBase::*getFboOn_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getFboOn"
                , getFboOn_function_type( &::OSG::ShadowStageBase::getFboOn ) );
        
        }
        { //::OSG::ShadowStageBase::getGlobalShadowIntensity
        
            typedef ::OSG::Real32 ( ::OSG::ShadowStageBase::*getGlobalShadowIntensity_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getGlobalShadowIntensity"
                , getGlobalShadowIntensity_function_type( &::OSG::ShadowStageBase::getGlobalShadowIntensity ) );
        
        }
        { //::OSG::ShadowStageBase::getGreen
        
            typedef bool ( ::OSG::ShadowStageBase::*getGreen_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getGreen"
                , getGreen_function_type( &::OSG::ShadowStageBase::getGreen ) );
        
        }
        { //::OSG::ShadowStageBase::getLightNodes
        
            typedef ::OSG::Node * ( ::OSG::ShadowStageBase::*getLightNodes_function_type )( ::OSG::UInt32 const ) const;
            
            ShadowStageBase_exposer.def( 
                "getLightNodes"
                , getLightNodes_function_type( &::OSG::ShadowStageBase::getLightNodes )
                , ( bp::arg("index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getMapAutoUpdate
        
            typedef bool ( ::OSG::ShadowStageBase::*getMapAutoUpdate_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getMapAutoUpdate"
                , getMapAutoUpdate_function_type( &::OSG::ShadowStageBase::getMapAutoUpdate ) );
        
        }
        { //::OSG::ShadowStageBase::getMapSize
        
            typedef ::OSG::UInt32 ( ::OSG::ShadowStageBase::*getMapSize_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getMapSize"
                , getMapSize_function_type( &::OSG::ShadowStageBase::getMapSize ) );
        
        }
        { //::OSG::ShadowStageBase::getOffBias
        
            typedef ::OSG::Real32 ( ::OSG::ShadowStageBase::*getOffBias_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getOffBias"
                , getOffBias_function_type( &::OSG::ShadowStageBase::getOffBias ) );
        
        }
        { //::OSG::ShadowStageBase::getOffFactor
        
            typedef ::OSG::Real32 ( ::OSG::ShadowStageBase::*getOffFactor_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getOffFactor"
                , getOffFactor_function_type( &::OSG::ShadowStageBase::getOffFactor ) );
        
        }
        { //::OSG::ShadowStageBase::getRed
        
            typedef bool ( ::OSG::ShadowStageBase::*getRed_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getRed"
                , getRed_function_type( &::OSG::ShadowStageBase::getRed ) );
        
        }
        { //::OSG::ShadowStageBase::getSFAlpha
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFAlpha_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFAlpha"
                , getSFAlpha_function_type( &::OSG::ShadowStageBase::getSFAlpha )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFAutoExcludeTransparentNodes
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFAutoExcludeTransparentNodes_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFAutoExcludeTransparentNodes"
                , getSFAutoExcludeTransparentNodes_function_type( &::OSG::ShadowStageBase::getSFAutoExcludeTransparentNodes )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFAutoSearchForLights
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFAutoSearchForLights_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFAutoSearchForLights"
                , getSFAutoSearchForLights_function_type( &::OSG::ShadowStageBase::getSFAutoSearchForLights )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFBlue
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFBlue_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFBlue"
                , getSFBlue_function_type( &::OSG::ShadowStageBase::getSFBlue )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFDisableOccludedLights
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFDisableOccludedLights_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFDisableOccludedLights"
                , getSFDisableOccludedLights_function_type( &::OSG::ShadowStageBase::getSFDisableOccludedLights )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFFboOn
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFFboOn_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFFboOn"
                , getSFFboOn_function_type( &::OSG::ShadowStageBase::getSFFboOn )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFGlobalShadowIntensity
        
            typedef ::OSG::SFReal32 const * ( ::OSG::ShadowStageBase::*getSFGlobalShadowIntensity_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFGlobalShadowIntensity"
                , getSFGlobalShadowIntensity_function_type( &::OSG::ShadowStageBase::getSFGlobalShadowIntensity )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFGreen
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFGreen_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFGreen"
                , getSFGreen_function_type( &::OSG::ShadowStageBase::getSFGreen )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFMapAutoUpdate
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFMapAutoUpdate_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFMapAutoUpdate"
                , getSFMapAutoUpdate_function_type( &::OSG::ShadowStageBase::getSFMapAutoUpdate )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFMapSize
        
            typedef ::OSG::SFUInt32 const * ( ::OSG::ShadowStageBase::*getSFMapSize_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFMapSize"
                , getSFMapSize_function_type( &::OSG::ShadowStageBase::getSFMapSize )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFOffBias
        
            typedef ::OSG::SFReal32 const * ( ::OSG::ShadowStageBase::*getSFOffBias_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFOffBias"
                , getSFOffBias_function_type( &::OSG::ShadowStageBase::getSFOffBias )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFOffFactor
        
            typedef ::OSG::SFReal32 const * ( ::OSG::ShadowStageBase::*getSFOffFactor_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFOffFactor"
                , getSFOffFactor_function_type( &::OSG::ShadowStageBase::getSFOffFactor )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFRed
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFRed_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFRed"
                , getSFRed_function_type( &::OSG::ShadowStageBase::getSFRed )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFShadowMode
        
            typedef ::OSG::SFUInt32 const * ( ::OSG::ShadowStageBase::*getSFShadowMode_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFShadowMode"
                , getSFShadowMode_function_type( &::OSG::ShadowStageBase::getSFShadowMode )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFShadowOn
        
            typedef ::OSG::SFBool const * ( ::OSG::ShadowStageBase::*getSFShadowOn_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFShadowOn"
                , getSFShadowOn_function_type( &::OSG::ShadowStageBase::getSFShadowOn )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getSFShadowSmoothness
        
            typedef ::OSG::SFReal32 const * ( ::OSG::ShadowStageBase::*getSFShadowSmoothness_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getSFShadowSmoothness"
                , getSFShadowSmoothness_function_type( &::OSG::ShadowStageBase::getSFShadowSmoothness )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getShadowMode
        
            typedef ::OSG::UInt32 ( ::OSG::ShadowStageBase::*getShadowMode_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getShadowMode"
                , getShadowMode_function_type( &::OSG::ShadowStageBase::getShadowMode ) );
        
        }
        { //::OSG::ShadowStageBase::getShadowOn
        
            typedef bool ( ::OSG::ShadowStageBase::*getShadowOn_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getShadowOn"
                , getShadowOn_function_type( &::OSG::ShadowStageBase::getShadowOn ) );
        
        }
        { //::OSG::ShadowStageBase::getShadowSmoothness
        
            typedef ::OSG::Real32 ( ::OSG::ShadowStageBase::*getShadowSmoothness_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getShadowSmoothness"
                , getShadowSmoothness_function_type( &::OSG::ShadowStageBase::getShadowSmoothness ) );
        
        }
        { //::OSG::ShadowStageBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::ShadowStageBase::*getType_function_type )(  ) ;
            
            ShadowStageBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::ShadowStageBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::ShadowStageBase::*getType_function_type )(  ) const;
            
            ShadowStageBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::ShadowStageBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ShadowStageBase::pushToExcludeNodes
        
            typedef void ( ::OSG::ShadowStageBase::*pushToExcludeNodes_function_type )( ::OSG::Node * const ) ;
            
            ShadowStageBase_exposer.def( 
                "pushToExcludeNodes"
                , pushToExcludeNodes_function_type( &::OSG::ShadowStageBase::pushToExcludeNodes )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::pushToLightNodes
        
            typedef void ( ::OSG::ShadowStageBase::*pushToLightNodes_function_type )( ::OSG::Node * const ) ;
            
            ShadowStageBase_exposer.def( 
                "pushToLightNodes"
                , pushToLightNodes_function_type( &::OSG::ShadowStageBase::pushToLightNodes )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::removeFromExcludeNodes
        
            typedef void ( ::OSG::ShadowStageBase::*removeFromExcludeNodes_function_type )( ::OSG::UInt32 ) ;
            
            ShadowStageBase_exposer.def( 
                "removeFromExcludeNodes"
                , removeFromExcludeNodes_function_type( &::OSG::ShadowStageBase::removeFromExcludeNodes )
                , ( bp::arg("uiIndex") ) );
        
        }
        { //::OSG::ShadowStageBase::removeFromLightNodes
        
            typedef void ( ::OSG::ShadowStageBase::*removeFromLightNodes_function_type )( ::OSG::UInt32 ) ;
            
            ShadowStageBase_exposer.def( 
                "removeFromLightNodes"
                , removeFromLightNodes_function_type( &::OSG::ShadowStageBase::removeFromLightNodes )
                , ( bp::arg("uiIndex") ) );
        
        }
        { //::OSG::ShadowStageBase::removeObjFromExcludeNodes
        
            typedef void ( ::OSG::ShadowStageBase::*removeObjFromExcludeNodes_function_type )( ::OSG::Node * const ) ;
            
            ShadowStageBase_exposer.def( 
                "removeObjFromExcludeNodes"
                , removeObjFromExcludeNodes_function_type( &::OSG::ShadowStageBase::removeObjFromExcludeNodes )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::removeObjFromLightNodes
        
            typedef void ( ::OSG::ShadowStageBase::*removeObjFromLightNodes_function_type )( ::OSG::Node * const ) ;
            
            ShadowStageBase_exposer.def( 
                "removeObjFromLightNodes"
                , removeObjFromLightNodes_function_type( &::OSG::ShadowStageBase::removeObjFromLightNodes )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setAlpha
        
            typedef void ( ::OSG::ShadowStageBase::*setAlpha_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setAlpha"
                , setAlpha_function_type( &::OSG::ShadowStageBase::setAlpha )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setAutoExcludeTransparentNodes
        
            typedef void ( ::OSG::ShadowStageBase::*setAutoExcludeTransparentNodes_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setAutoExcludeTransparentNodes"
                , setAutoExcludeTransparentNodes_function_type( &::OSG::ShadowStageBase::setAutoExcludeTransparentNodes )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setAutoSearchForLights
        
            typedef void ( ::OSG::ShadowStageBase::*setAutoSearchForLights_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setAutoSearchForLights"
                , setAutoSearchForLights_function_type( &::OSG::ShadowStageBase::setAutoSearchForLights )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setBlue
        
            typedef void ( ::OSG::ShadowStageBase::*setBlue_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setBlue"
                , setBlue_function_type( &::OSG::ShadowStageBase::setBlue )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setDisableOccludedLights
        
            typedef void ( ::OSG::ShadowStageBase::*setDisableOccludedLights_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setDisableOccludedLights"
                , setDisableOccludedLights_function_type( &::OSG::ShadowStageBase::setDisableOccludedLights )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setFboOn
        
            typedef void ( ::OSG::ShadowStageBase::*setFboOn_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setFboOn"
                , setFboOn_function_type( &::OSG::ShadowStageBase::setFboOn )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setGlobalShadowIntensity
        
            typedef void ( ::OSG::ShadowStageBase::*setGlobalShadowIntensity_function_type )( ::OSG::Real32 const ) ;
            
            ShadowStageBase_exposer.def( 
                "setGlobalShadowIntensity"
                , setGlobalShadowIntensity_function_type( &::OSG::ShadowStageBase::setGlobalShadowIntensity )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setGreen
        
            typedef void ( ::OSG::ShadowStageBase::*setGreen_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setGreen"
                , setGreen_function_type( &::OSG::ShadowStageBase::setGreen )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setMapAutoUpdate
        
            typedef void ( ::OSG::ShadowStageBase::*setMapAutoUpdate_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setMapAutoUpdate"
                , setMapAutoUpdate_function_type( &::OSG::ShadowStageBase::setMapAutoUpdate )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setMapSize
        
            typedef void ( ::OSG::ShadowStageBase::*setMapSize_function_type )( ::OSG::UInt32 const ) ;
            
            ShadowStageBase_exposer.def( 
                "setMapSize"
                , setMapSize_function_type( &::OSG::ShadowStageBase::setMapSize )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setOffBias
        
            typedef void ( ::OSG::ShadowStageBase::*setOffBias_function_type )( ::OSG::Real32 const ) ;
            
            ShadowStageBase_exposer.def( 
                "setOffBias"
                , setOffBias_function_type( &::OSG::ShadowStageBase::setOffBias )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setOffFactor
        
            typedef void ( ::OSG::ShadowStageBase::*setOffFactor_function_type )( ::OSG::Real32 const ) ;
            
            ShadowStageBase_exposer.def( 
                "setOffFactor"
                , setOffFactor_function_type( &::OSG::ShadowStageBase::setOffFactor )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setRed
        
            typedef void ( ::OSG::ShadowStageBase::*setRed_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setRed"
                , setRed_function_type( &::OSG::ShadowStageBase::setRed )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setShadowMode
        
            typedef void ( ::OSG::ShadowStageBase::*setShadowMode_function_type )( ::OSG::UInt32 const ) ;
            
            ShadowStageBase_exposer.def( 
                "setShadowMode"
                , setShadowMode_function_type( &::OSG::ShadowStageBase::setShadowMode )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setShadowOn
        
            typedef void ( ::OSG::ShadowStageBase::*setShadowOn_function_type )( bool const ) ;
            
            ShadowStageBase_exposer.def( 
                "setShadowOn"
                , setShadowOn_function_type( &::OSG::ShadowStageBase::setShadowOn )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::ShadowStageBase::setShadowSmoothness
        
            typedef void ( ::OSG::ShadowStageBase::*setShadowSmoothness_function_type )( ::OSG::Real32 const ) ;
            
            ShadowStageBase_exposer.def( 
                "setShadowSmoothness"
                , setShadowSmoothness_function_type( &::OSG::ShadowStageBase::setShadowSmoothness )
                , ( bp::arg("value") ) );
        
        }
        ShadowStageBase_exposer.staticmethod( "create" );
        ShadowStageBase_exposer.staticmethod( "createDependent" );
        ShadowStageBase_exposer.staticmethod( "createLocal" );
        ShadowStageBase_exposer.staticmethod( "getClassGroupId" );
        ShadowStageBase_exposer.staticmethod( "getClassType" );
        ShadowStageBase_exposer.staticmethod( "getClassTypeId" );
        ShadowStageBase_exposer.def("getMFLightNodes",ShadowStageBase_getMFLightNodes);
        ShadowStageBase_exposer.def("getMFExcludeNodes",ShadowStageBase_getMFExcludeNodes);
    }

}
