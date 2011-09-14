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
#include "OSGScripting_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "PyFieldAccessHandler.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_PyFieldAccessHandler_class(){

    { //::OSG::PyFieldAccessHandler
        typedef bp::class_< OSG::PyFieldAccessHandler, bp::bases< OSG::PyFieldAccessHandlerBase >, OSG::PyFieldAccessHandler::ObjRecPtr, boost::noncopyable > PyFieldAccessHandler_exposer_t;
        PyFieldAccessHandler_exposer_t PyFieldAccessHandler_exposer = PyFieldAccessHandler_exposer_t( "PyFieldAccessHandler", bp::no_init );
        bp::scope PyFieldAccessHandler_scope( PyFieldAccessHandler_exposer );
        { //::OSG::PyFieldAccessHandler::changed
        
            typedef void ( ::OSG::PyFieldAccessHandler::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            PyFieldAccessHandler_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::PyFieldAccessHandler::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::PyFieldAccessHandler::dump
        
            typedef void ( ::OSG::PyFieldAccessHandler::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            PyFieldAccessHandler_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::PyFieldAccessHandler::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::PyFieldAccessHandler::exposeField
        
            typedef bool ( ::OSG::PyFieldAccessHandler::*exposeField_function_type )( ::std::string const &,::OSG::UInt32 ) ;
            
            PyFieldAccessHandler_exposer.def( 
                "exposeField"
                , exposeField_function_type( &::OSG::PyFieldAccessHandler::exposeField )
                , ( bp::arg("fieldName"), bp::arg("fieldId") ) );
        
        }
        { //::OSG::PyFieldAccessHandler::generateSupportedFieldsList
        
            typedef void ( ::OSG::PyFieldAccessHandler::*generateSupportedFieldsList_function_type )(  ) ;
            
            PyFieldAccessHandler_exposer.def( 
                "generateSupportedFieldsList"
                , generateSupportedFieldsList_function_type( &::OSG::PyFieldAccessHandler::generateSupportedFieldsList ) );
        
        }
        { //::OSG::PyFieldAccessHandler::init
        
            typedef bool ( ::OSG::PyFieldAccessHandler::*init_function_type )( ::OSG::PythonScriptWeakPtr,::OSG::PyInterpreter * ) ;
            
            PyFieldAccessHandler_exposer.def( 
                "init"
                , init_function_type( &::OSG::PyFieldAccessHandler::init )
                , ( bp::arg("pythonScript"), bp::arg("inter") ) );
        
        }
        { //::OSG::PyFieldAccessHandler::listTypes
        
            typedef void ( ::OSG::PyFieldAccessHandler::*listTypes_function_type )(  ) const;
            
            PyFieldAccessHandler_exposer.def( 
                "listTypes"
                , listTypes_function_type( &::OSG::PyFieldAccessHandler::listTypes ) );
        
        }
        { //::OSG::PyFieldAccessHandler::registerTypeMappings
        
            typedef void ( *registerTypeMappings_function_type )(  );
            
            PyFieldAccessHandler_exposer.def( 
                "registerTypeMappings"
                , registerTypeMappings_function_type( &::OSG::PyFieldAccessHandler::registerTypeMappings ) );
        
        }
        { //::OSG::PyFieldAccessHandler::setupFieldAccess
        
            typedef bool ( ::OSG::PyFieldAccessHandler::*setupFieldAccess_function_type )(  ) ;
            
            PyFieldAccessHandler_exposer.def( 
                "setupFieldAccess"
                , setupFieldAccess_function_type( &::OSG::PyFieldAccessHandler::setupFieldAccess ) );
        
        }
        PyFieldAccessHandler_exposer.staticmethod( "registerTypeMappings" );
        pyopensg::register_transit< OSG::PyFieldAccessHandler >::execute();
        bp::implicitly_convertible< OSG::PyFieldAccessHandler::ObjRecPtr, OSG::PyFieldAccessHandler* >();
        bp::implicitly_convertible< OSG::PyFieldAccessHandler::ObjRecPtr, OSG::PyFieldAccessHandler::ObjCPtr >();
        bp::implicitly_convertible< OSG::PyFieldAccessHandler::ObjRecPtr, OSG::FieldContainer* >();
        bp::implicitly_convertible< OSG::PyFieldAccessHandler::ObjRecPtr, OSG::FieldContainer::ObjRecPtr >();
        bp::implicitly_convertible<OSG::PyFieldAccessHandler::ObjRecPtr, OSG::FieldContainer::ObjCPtr>();
    }

}