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
#include "Action.pypp.hpp"

using namespace std;
namespace bp = boost::python;

struct Action_wrapper : OSG::Action, bp::wrapper< OSG::Action > {

    virtual ::OSG::ActionBase::ResultE apply( ::OSG::Node * const node ) {
        if( bp::override func_apply = this->get_override( "apply" ) )
            return func_apply( node );
        else{
            return this->OSG::Action::apply( node );
        }
    }
    
    ::OSG::ActionBase::ResultE default_apply( ::OSG::Node * const node ) {
        return OSG::Action::apply( node );
    }

    virtual OSG::ActionBase::ResultE apply(std::vector<OSG::Node*>::iterator begin,
                                           std::vector<OSG::Node*>::iterator end)
    {
       // Transforms this apply() overload into something that works for Python.
       if ( bp::override func_apply = this->get_override("apply") )
       {
          // Create a list containing the nodes in the range identified by the
          // iterators.
          bp::list nodes;
          typedef std::vector<OSG::Node*>::iterator iter_type;
          for ( iter_type i = begin; i != end; ++i )
          {
             nodes.append(OSG::Node::ObjTransitPtr(*i));
          }
    
          // Hand the list off to Python.
          return func_apply(nodes);
       }
       else
       {
          return this->OSG::Action::apply(begin, end);
       }
    }
        
    OSG::ActionBase::ResultE default_apply(std::vector<OSG::Node*>::iterator begin,
                                           std::vector<OSG::Node*>::iterator end)
    {
       return OSG::Action::apply(begin, end);
    }

};

namespace
{

OSG::ActionBase::ResultE invokeCallback(bp::object obj,
                                        OSG::NodeCore* const core,
                                        OSG::Action* action)
{
   OSG::NodeCoreRecPtr core_ptr(core);
   return bp::extract<OSG::ActionBase::ResultE>(obj(core_ptr, action));
}

}

namespace
{

void wrapRegisterEnterDefault(const OSG::FieldContainerType& type,
                              bp::object func)
{
   OSG::Action::registerEnterDefault(type, boost::bind(invokeCallback, func, _1, _2));
}

void wrapRegisterLeaveDefault(const OSG::FieldContainerType& type,
                              bp::object func)
{
   OSG::Action::registerLeaveDefault(type, boost::bind(invokeCallback, func, _1, _2));
}

}

namespace
{

void wrapRegisterEnterFunction(OSG::Action* self,
                               const OSG::FieldContainerType& type,
                               bp::object func)
{
   self->registerEnterFunction(type,
                               boost::bind(invokeCallback, func, _1, _2));
}

void wrapRegisterLeaveFunction(OSG::Action* self,
                               const OSG::FieldContainerType& type,
                               bp::object func)
{
   self->registerLeaveFunction(type,
                               boost::bind(invokeCallback, func, _1, _2));
}

}

namespace
{

OSG::ActionBase::ResultE wrapApply(OSG::Action* self, bp::list nodes)
{
   const int num_nodes(bp::extract<int>(nodes.attr("__len__")()));
   std::vector<OSG::Node*> node_vec(num_nodes);

   for ( int i = 0; i < num_nodes; ++i )
   {
      OSG::NodeRecPtr n = bp::extract<OSG::NodeRecPtr>(nodes[i]);
      node_vec[i] = n.get();
   }

   return self->apply(node_vec.begin(), node_vec.end());
}

}

void register_Action_class(){

    { //::OSG::Action
        typedef bp::class_< Action_wrapper, bp::bases< OSG::ActionBase >, boost::noncopyable > Action_exposer_t;
        Action_exposer_t Action_exposer = Action_exposer_t( "Action", bp::no_init );
        bp::scope Action_scope( Action_exposer );
        { //::OSG::Action::addNode
        
            typedef void ( ::OSG::Action::*addNode_function_type )( ::OSG::Node * const ) ;
            
            Action_exposer.def( 
                "addNode"
                , addNode_function_type( &::OSG::Action::addNode )
                , ( bp::arg("node") ) );
        
        }
        { //::OSG::Action::andTravMask
        
            typedef void ( ::OSG::Action::*andTravMask_function_type )( ::OSG::UInt32 ) ;
            
            Action_exposer.def( 
                "andTravMask"
                , andTravMask_function_type( &::OSG::Action::andTravMask )
                , ( bp::arg("val") ) );
        
        }
        { //::OSG::Action::apply
        
            typedef ::OSG::ActionBase::ResultE ( ::OSG::Action::*apply_function_type )( ::OSG::Node * const ) ;
            typedef ::OSG::ActionBase::ResultE ( Action_wrapper::*default_apply_function_type )( ::OSG::Node * const ) ;
            
            Action_exposer.def( 
                "apply"
                , apply_function_type(&::OSG::Action::apply)
                , default_apply_function_type(&Action_wrapper::default_apply)
                , ( bp::arg("node") ) );
        
        }
        { //::OSG::Action::create
        
            typedef ::OSG::Action * ( *create_function_type )(  );
            
            Action_exposer.def( 
                "create"
                , create_function_type( &::OSG::Action::create )
                , bp::return_value_policy< bp::manage_new_object >() );
        
        }
        { //::OSG::Action::getActNode
        
            typedef ::OSG::Node * ( ::OSG::Action::*getActNode_function_type )(  ) ;
            
            Action_exposer.def( 
                "getActNode"
                , getActNode_function_type( &::OSG::Action::getActNode )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::Action::getActParent
        
            typedef ::OSG::FieldContainer * ( ::OSG::Action::*getActParent_function_type )(  ) ;
            
            Action_exposer.def( 
                "getActParent"
                , getActParent_function_type( &::OSG::Action::getActParent )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::Action::getNNodes
        
            typedef ::OSG::UInt32 ( ::OSG::Action::*getNNodes_function_type )(  ) const;
            
            Action_exposer.def( 
                "getNNodes"
                , getNNodes_function_type( &::OSG::Action::getNNodes ) );
        
        }
        { //::OSG::Action::getNode
        
            typedef ::OSG::Node * ( ::OSG::Action::*getNode_function_type )( int ) ;
            
            Action_exposer.def( 
                "getNode"
                , getNode_function_type( &::OSG::Action::getNode )
                , ( bp::arg("index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::Action::getPrototype
        
            typedef ::OSG::Action * ( *getPrototype_function_type )(  );
            
            Action_exposer.def( 
                "getPrototype"
                , getPrototype_function_type( &::OSG::Action::getPrototype )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::Action::getTravMask
        
            typedef ::OSG::UInt32 ( ::OSG::Action::*getTravMask_function_type )(  ) const;
            
            Action_exposer.def( 
                "getTravMask"
                , getTravMask_function_type( &::OSG::Action::getTravMask ) );
        
        }
        { //::OSG::Action::orTravMask
        
            typedef void ( ::OSG::Action::*orTravMask_function_type )( ::OSG::UInt32 ) ;
            
            Action_exposer.def( 
                "orTravMask"
                , orTravMask_function_type( &::OSG::Action::orTravMask )
                , ( bp::arg("val") ) );
        
        }
        { //::OSG::Action::popTravMask
        
            typedef void ( ::OSG::Action::*popTravMask_function_type )(  ) ;
            
            Action_exposer.def( 
                "popTravMask"
                , popTravMask_function_type( &::OSG::Action::popTravMask ) );
        
        }
        { //::OSG::Action::pushTravMask
        
            typedef void ( ::OSG::Action::*pushTravMask_function_type )(  ) ;
            
            Action_exposer.def( 
                "pushTravMask"
                , pushTravMask_function_type( &::OSG::Action::pushTravMask ) );
        
        }
        { //::OSG::Action::setActNode
        
            typedef void ( ::OSG::Action::*setActNode_function_type )( ::OSG::Node * const ) ;
            
            Action_exposer.def( 
                "setActNode"
                , setActNode_function_type( &::OSG::Action::setActNode )
                , ( bp::arg("node") ) );
        
        }
        { //::OSG::Action::setPrototype
        
            typedef void ( *setPrototype_function_type )( ::OSG::Action * );
            
            Action_exposer.def( 
                "setPrototype"
                , setPrototype_function_type( &::OSG::Action::setPrototype )
                , ( bp::arg("proto") ) );
        
        }
        { //::OSG::Action::setTravMask
        
            typedef void ( ::OSG::Action::*setTravMask_function_type )( ::OSG::UInt32 ) ;
            
            Action_exposer.def( 
                "setTravMask"
                , setTravMask_function_type( &::OSG::Action::setTravMask )
                , ( bp::arg("val") ) );
        
        }
        { //::OSG::Action::useNodeList
        
            typedef void ( ::OSG::Action::*useNodeList_function_type )( bool ) ;
            
            Action_exposer.def( 
                "useNodeList"
                , useNodeList_function_type( &::OSG::Action::useNodeList )
                , ( bp::arg("bVal")=(bool)(true) ) );
        
        }
        Action_exposer.staticmethod( "create" );
        Action_exposer.staticmethod( "getPrototype" );
        Action_exposer.staticmethod( "setPrototype" );
        Action_exposer.def("registerEnterDefault", wrapRegisterEnterDefault, (bp::arg("type"), bp::arg("func")));
        Action_exposer.def("registerLeaveDefault", wrapRegisterLeaveDefault, (bp::arg("type"), bp::arg("func")));
        Action_exposer.def("registerEnterFunction", wrapRegisterEnterFunction, (bp::arg("type"), bp::arg("func")));
        Action_exposer.def("registerLeaveFunction", wrapRegisterLeaveFunction, (bp::arg("type"), bp::arg("func")));
        Action_exposer.def("apply", wrapApply, (bp::arg("nodes")));
    }

}
