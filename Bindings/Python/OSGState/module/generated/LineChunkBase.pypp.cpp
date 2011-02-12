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
#include "OSGState_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "LineChunkBase.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_LineChunkBase_class(){

    { //::OSG::LineChunkBase
        typedef bp::class_< OSG::LineChunkBase, bp::bases< ::OSG::StateChunk >, boost::noncopyable > LineChunkBase_exposer_t;
        LineChunkBase_exposer_t LineChunkBase_exposer = LineChunkBase_exposer_t( "LineChunkBase", bp::no_init );
        bp::scope LineChunkBase_scope( LineChunkBase_exposer );
        bp::scope().attr("WidthFieldId") = (int)OSG::LineChunkBase::WidthFieldId;
        bp::scope().attr("StippleRepeatFieldId") = (int)OSG::LineChunkBase::StippleRepeatFieldId;
        bp::scope().attr("StipplePatternFieldId") = (int)OSG::LineChunkBase::StipplePatternFieldId;
        bp::scope().attr("SmoothFieldId") = (int)OSG::LineChunkBase::SmoothFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::LineChunkBase::NextFieldId;
        { //::OSG::LineChunkBase::copyFromBin
        
            typedef void ( ::OSG::LineChunkBase::*copyFromBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            LineChunkBase_exposer.def( 
                "copyFromBin"
                , copyFromBin_function_type( &::OSG::LineChunkBase::copyFromBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::LineChunkBase::copyToBin
        
            typedef void ( ::OSG::LineChunkBase::*copyToBin_function_type )( ::OSG::BinaryDataHandler &,::OSG::ConstFieldMaskArg ) ;
            
            LineChunkBase_exposer.def( 
                "copyToBin"
                , copyToBin_function_type( &::OSG::LineChunkBase::copyToBin )
                , ( bp::arg("pMem"), bp::arg("whichField") ) );
        
        }
        { //::OSG::LineChunkBase::create
        
            typedef ::OSG::LineChunkTransitPtr ( *create_function_type )(  );
            
            LineChunkBase_exposer.def( 
                "create"
                , create_function_type( &::OSG::LineChunkBase::create ) );
        
        }
        { //::OSG::LineChunkBase::createDependent
        
            typedef ::OSG::LineChunkTransitPtr ( *createDependent_function_type )( ::OSG::BitVector );
            
            LineChunkBase_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::LineChunkBase::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::LineChunkBase::createLocal
        
            typedef ::OSG::LineChunkTransitPtr ( *createLocal_function_type )( ::OSG::BitVector );
            
            LineChunkBase_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::LineChunkBase::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::LineChunkBase::getBinSize
        
            typedef ::OSG::UInt32 ( ::OSG::LineChunkBase::*getBinSize_function_type )( ::OSG::ConstFieldMaskArg ) ;
            
            LineChunkBase_exposer.def( 
                "getBinSize"
                , getBinSize_function_type( &::OSG::LineChunkBase::getBinSize )
                , ( bp::arg("whichField") ) );
        
        }
        { //::OSG::LineChunkBase::getClassGroupId
        
            typedef ::OSG::UInt16 ( *getClassGroupId_function_type )(  );
            
            LineChunkBase_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::LineChunkBase::getClassGroupId ) );
        
        }
        { //::OSG::LineChunkBase::getClassType
        
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            LineChunkBase_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::LineChunkBase::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::LineChunkBase::getClassTypeId
        
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            LineChunkBase_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::LineChunkBase::getClassTypeId ) );
        
        }
        { //::OSG::LineChunkBase::getContainerSize
        
            typedef ::OSG::UInt32 ( ::OSG::LineChunkBase::*getContainerSize_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::LineChunkBase::getContainerSize ) );
        
        }
        { //::OSG::LineChunkBase::getSFSmooth
        
            typedef ::OSG::SFBool const * ( ::OSG::LineChunkBase::*getSFSmooth_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getSFSmooth"
                , getSFSmooth_function_type( &::OSG::LineChunkBase::getSFSmooth )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::LineChunkBase::getSFStipplePattern
        
            typedef ::OSG::SFUInt16 const * ( ::OSG::LineChunkBase::*getSFStipplePattern_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getSFStipplePattern"
                , getSFStipplePattern_function_type( &::OSG::LineChunkBase::getSFStipplePattern )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::LineChunkBase::getSFStippleRepeat
        
            typedef ::OSG::SFInt32 const * ( ::OSG::LineChunkBase::*getSFStippleRepeat_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getSFStippleRepeat"
                , getSFStippleRepeat_function_type( &::OSG::LineChunkBase::getSFStippleRepeat )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::LineChunkBase::getSFWidth
        
            typedef ::OSG::SFReal32 const * ( ::OSG::LineChunkBase::*getSFWidth_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getSFWidth"
                , getSFWidth_function_type( &::OSG::LineChunkBase::getSFWidth )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::LineChunkBase::getSmooth
        
            typedef bool ( ::OSG::LineChunkBase::*getSmooth_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getSmooth"
                , getSmooth_function_type( &::OSG::LineChunkBase::getSmooth ) );
        
        }
        { //::OSG::LineChunkBase::getStipplePattern
        
            typedef ::OSG::UInt16 ( ::OSG::LineChunkBase::*getStipplePattern_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getStipplePattern"
                , getStipplePattern_function_type( &::OSG::LineChunkBase::getStipplePattern ) );
        
        }
        { //::OSG::LineChunkBase::getStippleRepeat
        
            typedef ::OSG::Int32 ( ::OSG::LineChunkBase::*getStippleRepeat_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getStippleRepeat"
                , getStippleRepeat_function_type( &::OSG::LineChunkBase::getStippleRepeat ) );
        
        }
        { //::OSG::LineChunkBase::getType
        
            typedef ::OSG::FieldContainerType & ( ::OSG::LineChunkBase::*getType_function_type )(  ) ;
            
            LineChunkBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::LineChunkBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::LineChunkBase::getType
        
            typedef ::OSG::FieldContainerType const & ( ::OSG::LineChunkBase::*getType_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::LineChunkBase::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::LineChunkBase::getWidth
        
            typedef ::OSG::Real32 ( ::OSG::LineChunkBase::*getWidth_function_type )(  ) const;
            
            LineChunkBase_exposer.def( 
                "getWidth"
                , getWidth_function_type( &::OSG::LineChunkBase::getWidth ) );
        
        }
        { //::OSG::LineChunkBase::setSmooth
        
            typedef void ( ::OSG::LineChunkBase::*setSmooth_function_type )( bool const ) ;
            
            LineChunkBase_exposer.def( 
                "setSmooth"
                , setSmooth_function_type( &::OSG::LineChunkBase::setSmooth )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::LineChunkBase::setStipplePattern
        
            typedef void ( ::OSG::LineChunkBase::*setStipplePattern_function_type )( ::OSG::UInt16 const ) ;
            
            LineChunkBase_exposer.def( 
                "setStipplePattern"
                , setStipplePattern_function_type( &::OSG::LineChunkBase::setStipplePattern )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::LineChunkBase::setStippleRepeat
        
            typedef void ( ::OSG::LineChunkBase::*setStippleRepeat_function_type )( ::OSG::Int32 const ) ;
            
            LineChunkBase_exposer.def( 
                "setStippleRepeat"
                , setStippleRepeat_function_type( &::OSG::LineChunkBase::setStippleRepeat )
                , ( bp::arg("value") ) );
        
        }
        { //::OSG::LineChunkBase::setWidth
        
            typedef void ( ::OSG::LineChunkBase::*setWidth_function_type )( ::OSG::Real32 const ) ;
            
            LineChunkBase_exposer.def( 
                "setWidth"
                , setWidth_function_type( &::OSG::LineChunkBase::setWidth )
                , ( bp::arg("value") ) );
        
        }
        LineChunkBase_exposer.staticmethod( "create" );
        LineChunkBase_exposer.staticmethod( "createDependent" );
        LineChunkBase_exposer.staticmethod( "createLocal" );
        LineChunkBase_exposer.staticmethod( "getClassGroupId" );
        LineChunkBase_exposer.staticmethod( "getClassType" );
        LineChunkBase_exposer.staticmethod( "getClassTypeId" );
    }

}
