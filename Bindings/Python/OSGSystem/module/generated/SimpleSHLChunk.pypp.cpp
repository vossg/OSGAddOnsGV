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
#include "__call_policies.pypp.hpp"
#include "__convenience.pypp.hpp"
#include "OSGSystem_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "SimpleSHLChunk.pypp.hpp"

using namespace std;
namespace bp = boost::python;

static boost::python::tuple getUniformVariable_bf8055c14dc9a8ac3280d7ca8c70b9b4( ::OSG::SimpleSHLChunk & inst, ::OSG::Char8 const * name ){
    OSG::Point<float, 3u> value2;
    bool result = inst.getUniformVariable(name, value2);
    return bp::make_tuple( result, value2 );
}

static boost::python::tuple getUniformVariable_d465034bfebf811fa6544bddca71d278( ::OSG::SimpleSHLChunk & inst, ::OSG::Char8 const * name ){
    OSG::Point<float, 2u> value2;
    bool result = inst.getUniformVariable(name, value2);
    return bp::make_tuple( result, value2 );
}

static boost::python::tuple getUniformVariable_543f13cbf2d3a2aa8d6f2470334ffcd7( ::OSG::SimpleSHLChunk & inst, ::OSG::Char8 const * name ){
    OSG::TransformationMatrix<float> value2;
    bool result = inst.getUniformVariable(name, value2);
    return bp::make_tuple( result, value2 );
}

static boost::python::tuple getUniformVariable_5a15c57a6b1ccb986e43e0a4c3bed17a( ::OSG::SimpleSHLChunk & inst, ::OSG::Char8 const * name ){
    OSG::Vector<float, 4u> value2;
    bool result = inst.getUniformVariable(name, value2);
    return bp::make_tuple( result, value2 );
}

static boost::python::tuple getUniformVariable_f60bf20c666a0963101e3f711c47c79e( ::OSG::SimpleSHLChunk & inst, ::OSG::Char8 const * name ){
    OSG::Vector<float, 3u> value2;
    bool result = inst.getUniformVariable(name, value2);
    return bp::make_tuple( result, value2 );
}

static boost::python::tuple getUniformVariable_ffb8aca843ceb7773ac047a17d8db9f2( ::OSG::SimpleSHLChunk & inst, ::OSG::Char8 const * name ){
    OSG::Vector<float, 2u> value2;
    bool result = inst.getUniformVariable(name, value2);
    return bp::make_tuple( result, value2 );
}

static boost::python::tuple getUniformVariable_0fee8e1a616301b9c2a94586192d2fa0( ::OSG::SimpleSHLChunk & inst, ::OSG::Char8 const * name ){
    float value2;
    bool result = inst.getUniformVariable(name, value2);
    return bp::make_tuple( result, value2 );
}

static boost::python::tuple getUniformVariable_9a7a291a7c1fc89e91eb9d7c70d5c156( ::OSG::SimpleSHLChunk & inst, ::OSG::Char8 const * name ){
    int value2;
    bool result = inst.getUniformVariable(name, value2);
    return bp::make_tuple( result, value2 );
}

static boost::python::tuple getUniformVariable_88ae7a8044e530f5c16d07d43469503c( ::OSG::SimpleSHLChunk & inst, ::OSG::Char8 const * name ){
    bool value2;
    bool result = inst.getUniformVariable(name, value2);
    return bp::make_tuple( result, value2 );
}

namespace
{

void proceduralCallback(bp::object callable, OSG::DrawEnv* env, int i)
{
   callable(env, i);
}

bool
wrapAddProceduralVariable(
   OSG::SimpleSHLChunk* self, const OSG::Char8* name, bp::object pFunc,
   OSG::UInt32 uiDependency = OSG::ShaderProcVariable::SHDObject
)
{
   return self->addProceduralVariable(name,
                                      boost::bind(proceduralCallback, pFunc,
                                                  _1, _2),
                                      uiDependency);
}

bool
wrapUpdateProceduralVariable(
   OSG::SimpleSHLChunk* self, const OSG::Char8* name, bp::object pFunc,
   OSG::UInt32 uiDependency = OSG::ShaderProcVariable::SHDObject
)
{
   return self->updateProceduralVariable(name,
                                         boost::bind(proceduralCallback, pFunc,
                                                     _1, _2),
                                         uiDependency);
}

}

void register_SimpleSHLChunk_class(){

    { //::OSG::SimpleSHLChunk
        typedef bp::class_< OSG::SimpleSHLChunk, bp::bases< OSG::SimpleSHLChunkBase >, OSG::SimpleSHLChunk::ObjRecPtr, boost::noncopyable > SimpleSHLChunk_exposer_t;
        SimpleSHLChunk_exposer_t SimpleSHLChunk_exposer = SimpleSHLChunk_exposer_t( "SimpleSHLChunk", bp::no_init );
        bp::scope SimpleSHLChunk_scope( SimpleSHLChunk_exposer );
        { //::OSG::SimpleSHLChunk::addOSGVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addOSGVariable_function_type )( ::OSG::Char8 const * ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addOSGVariable"
                , addOSGVariable_function_type( &::OSG::SimpleSHLChunk::addOSGVariable )
                , ( bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::addUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Pnt3f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addUniformVariable"
                , addUniformVariable_function_type( &::OSG::SimpleSHLChunk::addUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::addUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Pnt2f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addUniformVariable"
                , addUniformVariable_function_type( &::OSG::SimpleSHLChunk::addUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::addUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Matrix const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addUniformVariable"
                , addUniformVariable_function_type( &::OSG::SimpleSHLChunk::addUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::addUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Vec4f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addUniformVariable"
                , addUniformVariable_function_type( &::OSG::SimpleSHLChunk::addUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::addUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Vec3f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addUniformVariable"
                , addUniformVariable_function_type( &::OSG::SimpleSHLChunk::addUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::addUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Vec2f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addUniformVariable"
                , addUniformVariable_function_type( &::OSG::SimpleSHLChunk::addUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::addUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Real32 const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addUniformVariable"
                , addUniformVariable_function_type( &::OSG::SimpleSHLChunk::addUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::addUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Int32 const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addUniformVariable"
                , addUniformVariable_function_type( &::OSG::SimpleSHLChunk::addUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::addUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*addUniformVariable_function_type )( ::OSG::Char8 const *,bool const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "addUniformVariable"
                , addUniformVariable_function_type( &::OSG::SimpleSHLChunk::addUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::changed
        
            typedef void ( ::OSG::SimpleSHLChunk::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            SimpleSHLChunk_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::SimpleSHLChunk::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::SimpleSHLChunk::clearProgramParameters
        
            typedef void ( ::OSG::SimpleSHLChunk::*clearProgramParameters_function_type )(  ) ;
            
            SimpleSHLChunk_exposer.def( 
                "clearProgramParameters"
                , clearProgramParameters_function_type( &::OSG::SimpleSHLChunk::clearProgramParameters ) );
        
        }
        { //::OSG::SimpleSHLChunk::clearUniformVariables
        
            typedef void ( ::OSG::SimpleSHLChunk::*clearUniformVariables_function_type )(  ) ;
            
            SimpleSHLChunk_exposer.def( 
                "clearUniformVariables"
                , clearUniformVariables_function_type( &::OSG::SimpleSHLChunk::clearUniformVariables ) );
        
        }
        { //::OSG::SimpleSHLChunk::dump
        
            typedef void ( ::OSG::SimpleSHLChunk::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            SimpleSHLChunk_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::SimpleSHLChunk::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::SimpleSHLChunk::getChunkId
        
            typedef ::OSG::UInt16 ( ::OSG::SimpleSHLChunk::*getChunkId_function_type )(  ) ;
            
            SimpleSHLChunk_exposer.def( 
                "getChunkId"
                , getChunkId_function_type( &::OSG::SimpleSHLChunk::getChunkId ) );
        
        }
        { //::OSG::SimpleSHLChunk::getClass
        
            typedef ::OSG::StateChunkClass const * ( ::OSG::SimpleSHLChunk::*getClass_function_type )(  ) const;
            
            SimpleSHLChunk_exposer.def( 
                "getClass"
                , getClass_function_type( &::OSG::SimpleSHLChunk::getClass )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleSHLChunk::getMFParameters
        
            typedef ::OSG::UInt32 const * ( ::OSG::SimpleSHLChunk::*getMFParameters_function_type )(  ) const;
            
            SimpleSHLChunk_exposer.def( 
                "getMFParameters"
                , getMFParameters_function_type( &::OSG::SimpleSHLChunk::getMFParameters )
                , bp::return_value_policy< bp::return_by_value >() );
        
        }
        { //::OSG::SimpleSHLChunk::getProgramParameter
        
            typedef ::OSG::UInt32 ( ::OSG::SimpleSHLChunk::*getProgramParameter_function_type )( ::GLenum ) ;
            
            SimpleSHLChunk_exposer.def( 
                "getProgramParameter"
                , getProgramParameter_function_type( &::OSG::SimpleSHLChunk::getProgramParameter )
                , ( bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::getStaticClass
        
            typedef ::OSG::StateChunkClass const * ( *getStaticClass_function_type )(  );
            
            SimpleSHLChunk_exposer.def( 
                "getStaticClass"
                , getStaticClass_function_type( &::OSG::SimpleSHLChunk::getStaticClass )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleSHLChunk::getStaticClassId
        
            typedef ::OSG::UInt32 ( *getStaticClassId_function_type )(  );
            
            SimpleSHLChunk_exposer.def( 
                "getStaticClassId"
                , getStaticClassId_function_type( &::OSG::SimpleSHLChunk::getStaticClassId ) );
        
        }
        { //::OSG::SimpleSHLChunk::getUniformVariable
        
            typedef boost::python::tuple ( *getUniformVariable_Point_less__float_comma__3u__greater__function_type )( ::OSG::SimpleSHLChunk &,::OSG::Char8 const * );
            
            SimpleSHLChunk_exposer.def( 
                "getUniformVariable_Point_less__float_comma__3u__greater_"
                , getUniformVariable_Point_less__float_comma__3u__greater__function_type( &getUniformVariable_bf8055c14dc9a8ac3280d7ca8c70b9b4 )
                , ( bp::arg("inst"), bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::getUniformVariable
        
            typedef boost::python::tuple ( *getUniformVariable_Point_less__float_comma__2u__greater__function_type )( ::OSG::SimpleSHLChunk &,::OSG::Char8 const * );
            
            SimpleSHLChunk_exposer.def( 
                "getUniformVariable_Point_less__float_comma__2u__greater_"
                , getUniformVariable_Point_less__float_comma__2u__greater__function_type( &getUniformVariable_d465034bfebf811fa6544bddca71d278 )
                , ( bp::arg("inst"), bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::getUniformVariable
        
            typedef boost::python::tuple ( *getUniformVariable_TransformationMatrix_less__float__greater__function_type )( ::OSG::SimpleSHLChunk &,::OSG::Char8 const * );
            
            SimpleSHLChunk_exposer.def( 
                "getUniformVariable_TransformationMatrix_less__float__greater_"
                , getUniformVariable_TransformationMatrix_less__float__greater__function_type( &getUniformVariable_543f13cbf2d3a2aa8d6f2470334ffcd7 )
                , ( bp::arg("inst"), bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::getUniformVariable
        
            typedef boost::python::tuple ( *getUniformVariable_Vector_less__float_comma__4u__greater__function_type )( ::OSG::SimpleSHLChunk &,::OSG::Char8 const * );
            
            SimpleSHLChunk_exposer.def( 
                "getUniformVariable_Vector_less__float_comma__4u__greater_"
                , getUniformVariable_Vector_less__float_comma__4u__greater__function_type( &getUniformVariable_5a15c57a6b1ccb986e43e0a4c3bed17a )
                , ( bp::arg("inst"), bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::getUniformVariable
        
            typedef boost::python::tuple ( *getUniformVariable_Vector_less__float_comma__3u__greater__function_type )( ::OSG::SimpleSHLChunk &,::OSG::Char8 const * );
            
            SimpleSHLChunk_exposer.def( 
                "getUniformVariable_Vector_less__float_comma__3u__greater_"
                , getUniformVariable_Vector_less__float_comma__3u__greater__function_type( &getUniformVariable_f60bf20c666a0963101e3f711c47c79e )
                , ( bp::arg("inst"), bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::getUniformVariable
        
            typedef boost::python::tuple ( *getUniformVariable_Vector_less__float_comma__2u__greater__function_type )( ::OSG::SimpleSHLChunk &,::OSG::Char8 const * );
            
            SimpleSHLChunk_exposer.def( 
                "getUniformVariable_Vector_less__float_comma__2u__greater_"
                , getUniformVariable_Vector_less__float_comma__2u__greater__function_type( &getUniformVariable_ffb8aca843ceb7773ac047a17d8db9f2 )
                , ( bp::arg("inst"), bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::getUniformVariable
        
            typedef boost::python::tuple ( *getUniformVariable_float_function_type )( ::OSG::SimpleSHLChunk &,::OSG::Char8 const * );
            
            SimpleSHLChunk_exposer.def( 
                "getUniformVariable_float"
                , getUniformVariable_float_function_type( &getUniformVariable_0fee8e1a616301b9c2a94586192d2fa0 )
                , ( bp::arg("inst"), bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::getUniformVariable
        
            typedef boost::python::tuple ( *getUniformVariable_int_function_type )( ::OSG::SimpleSHLChunk &,::OSG::Char8 const * );
            
            SimpleSHLChunk_exposer.def( 
                "getUniformVariable_int"
                , getUniformVariable_int_function_type( &getUniformVariable_9a7a291a7c1fc89e91eb9d7c70d5c156 )
                , ( bp::arg("inst"), bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::getUniformVariable
        
            typedef boost::python::tuple ( *getUniformVariable_bool_function_type )( ::OSG::SimpleSHLChunk &,::OSG::Char8 const * );
            
            SimpleSHLChunk_exposer.def( 
                "getUniformVariable_bool"
                , getUniformVariable_bool_function_type( &getUniformVariable_88ae7a8044e530f5c16d07d43469503c )
                , ( bp::arg("inst"), bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::readFragmentProgram
        
            typedef bool ( ::OSG::SimpleSHLChunk::*readFragmentProgram_function_type )( ::OSG::Char8 const * ) ;
            
            SimpleSHLChunk_exposer.def( 
                "readFragmentProgram"
                , readFragmentProgram_function_type( &::OSG::SimpleSHLChunk::readFragmentProgram )
                , ( bp::arg("file") ) );
        
        }
        { //::OSG::SimpleSHLChunk::readGeometryProgram
        
            typedef bool ( ::OSG::SimpleSHLChunk::*readGeometryProgram_function_type )( ::OSG::Char8 const * ) ;
            
            SimpleSHLChunk_exposer.def( 
                "readGeometryProgram"
                , readGeometryProgram_function_type( &::OSG::SimpleSHLChunk::readGeometryProgram )
                , ( bp::arg("file") ) );
        
        }
        { //::OSG::SimpleSHLChunk::readVertexProgram
        
            typedef bool ( ::OSG::SimpleSHLChunk::*readVertexProgram_function_type )( ::OSG::Char8 const * ) ;
            
            SimpleSHLChunk_exposer.def( 
                "readVertexProgram"
                , readVertexProgram_function_type( &::OSG::SimpleSHLChunk::readVertexProgram )
                , ( bp::arg("file") ) );
        
        }
        { //::OSG::SimpleSHLChunk::setProgramParameter
        
            typedef void ( ::OSG::SimpleSHLChunk::*setProgramParameter_function_type )( ::GLenum,::OSG::UInt32 ) ;
            
            SimpleSHLChunk_exposer.def( 
                "setProgramParameter"
                , setProgramParameter_function_type( &::OSG::SimpleSHLChunk::setProgramParameter )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::subUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*subUniformVariable_function_type )( ::OSG::Char8 const * ) ;
            
            SimpleSHLChunk_exposer.def( 
                "subUniformVariable"
                , subUniformVariable_function_type( &::OSG::SimpleSHLChunk::subUniformVariable )
                , ( bp::arg("name") ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateParameters
        
            typedef void ( ::OSG::SimpleSHLChunk::*updateParameters_function_type )( ::OSG::Window *,::OSG::UInt32 const &,bool,bool,bool ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateParameters"
                , updateParameters_function_type( &::OSG::SimpleSHLChunk::updateParameters )
                , ( bp::arg("win"), bp::arg("parameters"), bp::arg("useProgram")=(bool)(true), bp::arg("force")=(bool)(false), bp::arg("keepProgramActive")=(bool)(false) ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*updateUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Pnt3f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateUniformVariable"
                , updateUniformVariable_function_type( &::OSG::SimpleSHLChunk::updateUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*updateUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Pnt2f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateUniformVariable"
                , updateUniformVariable_function_type( &::OSG::SimpleSHLChunk::updateUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*updateUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Matrix const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateUniformVariable"
                , updateUniformVariable_function_type( &::OSG::SimpleSHLChunk::updateUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*updateUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Vec4f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateUniformVariable"
                , updateUniformVariable_function_type( &::OSG::SimpleSHLChunk::updateUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*updateUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Vec3f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateUniformVariable"
                , updateUniformVariable_function_type( &::OSG::SimpleSHLChunk::updateUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*updateUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Vec2f const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateUniformVariable"
                , updateUniformVariable_function_type( &::OSG::SimpleSHLChunk::updateUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*updateUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Real32 const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateUniformVariable"
                , updateUniformVariable_function_type( &::OSG::SimpleSHLChunk::updateUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*updateUniformVariable_function_type )( ::OSG::Char8 const *,::OSG::Int32 const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateUniformVariable"
                , updateUniformVariable_function_type( &::OSG::SimpleSHLChunk::updateUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        { //::OSG::SimpleSHLChunk::updateUniformVariable
        
            typedef bool ( ::OSG::SimpleSHLChunk::*updateUniformVariable_function_type )( ::OSG::Char8 const *,bool const & ) ;
            
            SimpleSHLChunk_exposer.def( 
                "updateUniformVariable"
                , updateUniformVariable_function_type( &::OSG::SimpleSHLChunk::updateUniformVariable )
                , ( bp::arg("name"), bp::arg("value") ) );
        
        }
        SimpleSHLChunk_exposer.staticmethod( "getStaticClass" );
        SimpleSHLChunk_exposer.staticmethod( "getStaticClassId" );
        pyopensg::register_transit< OSG::SimpleSHLChunk >::execute();
        bp::implicitly_convertible< OSG::SimpleSHLChunk::ObjRecPtr, OSG::SimpleSHLChunk* >();
        bp::implicitly_convertible< OSG::SimpleSHLChunk::ObjRecPtr, OSG::SimpleSHLChunk::ObjCPtr >();
        bp::implicitly_convertible< OSG::SimpleSHLChunk::ObjRecPtr, OSG::ShaderProgramChunk* >();
        bp::implicitly_convertible< OSG::SimpleSHLChunk::ObjRecPtr, OSG::ShaderProgramChunk::ObjRecPtr >();
        bp::implicitly_convertible<OSG::SimpleSHLChunk::ObjRecPtr, OSG::ShaderProgramChunk::ObjCPtr>();
        SimpleSHLChunk_exposer.def("addProceduralVariable", wrapAddProceduralVariable,
                     (bp::arg("name"), bp::arg("pFunc"),
                      bp::arg("uiDependency") = OSG::ShaderProcVariable::SHDObject));
        SimpleSHLChunk_exposer.def("updateProceduralVariable", wrapUpdateProceduralVariable,
                     (bp::arg("name"), bp::arg("pFunc"),
                      bp::arg("uiDependency") = OSG::ShaderProcVariable::SHDObject));
    }

}
