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
#include "AnimationBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

boost::python::list AnimationBase_getMFChannels(OSG::AnimationBase *self)
{
   boost::python::list result;
   OSG::MFUnrecChildAnimChannelPtr const * mf_data = self->getMFChannels();
   const OSG::UInt32 size(mf_data->size());
   for ( OSG::UInt32 i = 0; i < size; ++i )
   {
      result.append(OSG::AnimChannel::ObjRecPtr((*mf_data)[i]));
   }
   return result;
}

void register_AnimationBase_class(){

    { //::OSG::AnimationBase
        typedef bp::class_< OSG::AnimationBase, bp::bases< ::OSG::AttachmentContainer >, boost::noncopyable > AnimationBase_exposer_t;
        AnimationBase_exposer_t AnimationBase_exposer = AnimationBase_exposer_t( "AnimationBase", bp::no_init );
        bp::scope AnimationBase_scope( AnimationBase_exposer );
        bp::scope().attr("TimeSensorFieldId") = (int)OSG::AnimationBase::TimeSensorFieldId;
        bp::scope().attr("TemplateFieldId") = (int)OSG::AnimationBase::TemplateFieldId;
        bp::scope().attr("ChannelsFieldId") = (int)OSG::AnimationBase::ChannelsFieldId;
        bp::scope().attr("EnabledFieldId") = (int)OSG::AnimationBase::EnabledFieldId;
        bp::scope().attr("WeightFieldId") = (int)OSG::AnimationBase::WeightFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::AnimationBase::NextFieldId;
        { //::OSG::AnimationBase::assignChannels
        
            typedef void ( ::OSG::AnimationBase::*assignChannels_function_type )( ::OSG::MFUnrecChildAnimChannelPtr const & ) ;
            
            AnimationBase_exposer.def( 
                "assignChannels"
                , assignChannels_function_type( &::OSG::AnimationBase::assignChannels )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::AnimationBase::clearChannels
        
            typedef void ( ::OSG::AnimationBase::*clearChannels_function_type )(  ) ;
            
            AnimationBase_exposer.def( 
                "clearChannels"
                , clearChannels_function_type( &::OSG::AnimationBase::clearChannels ) );
        
        }
        { //::OSG::AnimationBase::copyFromBin
        
            typedef void ( ::OSG::AnimationBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            AnimationBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::AnimationBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::AnimationBase::copyToBin
        
            typedef void ( ::OSG::AnimationBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            AnimationBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::AnimationBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::AnimationBase::create
        
            typedef ::OSG::AnimationTransitPtr ( *create_function_type )(  );
            
            AnimationBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::AnimationBase::create ) );
        
        }
        { //::OSG::AnimationBase::createDependent
        
            typedef ::OSG::AnimationTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            AnimationBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::AnimationBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::AnimationBase::createLocal
        
            typedef ::OSG::AnimationTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            AnimationBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::AnimationBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::AnimationBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::AnimationBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            AnimationBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::AnimationBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::AnimationBase::getChannels
        
            typedef ::OSG::AnimChannel * ( ::OSG::AnimationBase::*getChannels_function_type )( ::OSG::UInt32 const ) const;
            
            AnimationBase_exposer.def( 
                "getChannels"
                , getChannels_function_type( &::OSG::AnimationBase::getChannels )
                , ( bp::arg("index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::AnimationBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            AnimationBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::AnimationBase::getClassGroupId ) );
        
        }
        { //::OSG::AnimationBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            AnimationBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::AnimationBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::AnimationBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            AnimationBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::AnimationBase::getClassTypeId ) );
        
        }
        { //::OSG::AnimationBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::AnimationBase::*getContainerSize_function_type )(  ) const;
            
            AnimationBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::AnimationBase::getContainerSize ) );
        
        }
        { //::OSG::AnimationBase::getEnabled
        
            typedef bool ( ::OSG::AnimationBase::*getEnabled_function_type )(  ) const;
            
            AnimationBase_exposer.def( 
                "getEnabled"
                , getEnabled_function_type( &::OSG::AnimationBase::getEnabled ) );
        
        }
        { //::OSG::AnimationBase::getSFEnabled
        
            typedef ::OSG::SFBool const * ( ::OSG::AnimationBase::*getSFEnabled_function_type )(  ) const;
            
            AnimationBase_exposer.def( 
                "getSFEnabled"
                , getSFEnabled_function_type( &::OSG::AnimationBase::getSFEnabled )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::AnimationBase::getSFTemplate
        
            typedef ::OSG::SFUnrecAnimTemplatePtr const * ( ::OSG::AnimationBase::*getSFTemplate_function_type )(  ) const;
            
            AnimationBase_exposer.def( 
                "getSFTemplate"
                , getSFTemplate_function_type( &::OSG::AnimationBase::getSFTemplate )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::AnimationBase::getSFWeight
        
            typedef ::OSG::SFReal32 const * ( ::OSG::AnimationBase::*getSFWeight_function_type )(  ) const;
            
            AnimationBase_exposer.def( 
                "getSFWeight"
                , getSFWeight_function_type( &::OSG::AnimationBase::getSFWeight )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::AnimationBase::getTemplate
        
            typedef ::OSG::AnimTemplate * ( ::OSG::AnimationBase::*getTemplate_function_type )(  ) const;
            
            AnimationBase_exposer.def( 
                "getTemplate"
                , getTemplate_function_type( &::OSG::AnimationBase::getTemplate )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::AnimationBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::AnimationBase::*getType_function_type )(  ) ;
            
            AnimationBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::AnimationBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::AnimationBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::AnimationBase::*getType_function_type )(  ) const;
            
            AnimationBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::AnimationBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::AnimationBase::getWeight
        
            typedef ::OSG::Real32 ( ::OSG::AnimationBase::*getWeight_function_type )(  ) const;
            
            AnimationBase_exposer.def( 
                "getWeight"
                , getWeight_function_type( &::OSG::AnimationBase::getWeight ) );
        
        }
        { //::OSG::AnimationBase::pushToChannels
        
            typedef void ( ::OSG::AnimationBase::*pushToChannels_function_type )( ::OSG::AnimChannel * const ) ;
            
            AnimationBase_exposer.def( 
                "pushToChannels"
                , pushToChannels_function_type( &::OSG::AnimationBase::pushToChannels )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::AnimationBase::removeFromChannels
        
            typedef void ( ::OSG::AnimationBase::*removeFromChannels_function_type )( ::OSG::UInt32 ) ;
            
            AnimationBase_exposer.def( 
                "removeFromChannels"
                , removeFromChannels_function_type( &::OSG::AnimationBase::removeFromChannels )
                , ( bp::arg("uiIndex") ) );
        
        }
        { //::OSG::AnimationBase::removeObjFromChannels
        
            typedef void ( ::OSG::AnimationBase::*removeObjFromChannels_function_type )( ::OSG::AnimChannel * const ) ;
            
            AnimationBase_exposer.def( 
                "removeObjFromChannels"
                , removeObjFromChannels_function_type( &::OSG::AnimationBase::removeObjFromChannels )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::AnimationBase::setEnabled
        
            typedef void ( ::OSG::AnimationBase::*setEnabled_function_type )( bool const ) ;
            
            AnimationBase_exposer.def( 
                "setEnabled"
                , setEnabled_function_type( &::OSG::AnimationBase::setEnabled )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::AnimationBase::setTemplate
        
            typedef void ( ::OSG::AnimationBase::*setTemplate_function_type )( ::OSG::AnimTemplate * const ) ;
            
            AnimationBase_exposer.def( 
                "setTemplate"
                , setTemplate_function_type( &::OSG::AnimationBase::setTemplate )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::AnimationBase::setWeight
        
            typedef void ( ::OSG::AnimationBase::*setWeight_function_type )( ::OSG::Real32 const ) ;
            
            AnimationBase_exposer.def( 
                "setWeight"
                , setWeight_function_type( &::OSG::AnimationBase::setWeight )
                , ( bp::arg("value") ) );
        
        }
        AnimationBase_exposer.staticmethod( "create" );
        AnimationBase_exposer.staticmethod( "createDependent" );
        AnimationBase_exposer.staticmethod( "createLocal" );
        AnimationBase_exposer.staticmethod( "getClassGroupId" );
        AnimationBase_exposer.staticmethod( "getClassType" );
        AnimationBase_exposer.staticmethod( "getClassTypeId" );
        AnimationBase_exposer.def("getMFChannels",AnimationBase_getMFChannels);
    }

}
