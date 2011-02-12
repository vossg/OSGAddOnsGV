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
#include "OSGCluster_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "ClusterServer.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_ClusterServer_class(){

    { //::OSG::ClusterServer
        typedef bp::class_< OSG::ClusterServer, boost::noncopyable > ClusterServer_exposer_t;
        ClusterServer_exposer_t ClusterServer_exposer = ClusterServer_exposer_t( "ClusterServer", bp::init< OSG::Window *, std::string const &, bp::optional< std::string const &, std::string const &, OSG::UInt32, std::string const & > >(( bp::arg("window"), bp::arg("serviceName"), bp::arg("connectionType")="StreamSock", bp::arg("address")="", bp::arg("servicePort")=(::OSG::UInt32)(8437), bp::arg("serviceGroup")="" )) );
        bp::scope ClusterServer_scope( ClusterServer_exposer );
        { //::OSG::ClusterServer::doRender
        
            typedef void ( ::OSG::ClusterServer::*doRender_function_type )( ::OSG::RenderActionBase * ) ;
            
            ClusterServer_exposer.def( 
                "doRender"
                , doRender_function_type( &::OSG::ClusterServer::doRender )
                , ( bp::arg("action") ) );
        
        }
        { //::OSG::ClusterServer::doSwap
        
            typedef void ( ::OSG::ClusterServer::*doSwap_function_type )(  ) ;
            
            ClusterServer_exposer.def( 
                "doSwap"
                , doSwap_function_type( &::OSG::ClusterServer::doSwap ) );
        
        }
        { //::OSG::ClusterServer::doSync
        
            typedef void ( ::OSG::ClusterServer::*doSync_function_type )( bool ) ;
            
            ClusterServer_exposer.def( 
                "doSync"
                , doSync_function_type( &::OSG::ClusterServer::doSync )
                , ( bp::arg("applyToChangelist") ) );
        
        }
        { //::OSG::ClusterServer::getClusterWindow
        
            typedef ::OSG::Window * ( ::OSG::ClusterServer::*getClusterWindow_function_type )(  ) ;
            
            ClusterServer_exposer.def( 
                "getClusterWindow"
                , getClusterWindow_function_type( &::OSG::ClusterServer::getClusterWindow )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ClusterServer::getRemoteAspect
        
            typedef ::OSG::RemoteAspect * ( ::OSG::ClusterServer::*getRemoteAspect_function_type )(  ) const;
            
            ClusterServer_exposer.def( 
                "getRemoteAspect"
                , getRemoteAspect_function_type( &::OSG::ClusterServer::getRemoteAspect )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ClusterServer::getServerId
        
            typedef ::OSG::UInt32 ( ::OSG::ClusterServer::*getServerId_function_type )(  ) const;
            
            ClusterServer_exposer.def( 
                "getServerId"
                , getServerId_function_type( &::OSG::ClusterServer::getServerId ) );
        
        }
        { //::OSG::ClusterServer::getServerWindow
        
            typedef ::OSG::Window * ( ::OSG::ClusterServer::*getServerWindow_function_type )(  ) ;
            
            ClusterServer_exposer.def( 
                "getServerWindow"
                , getServerWindow_function_type( &::OSG::ClusterServer::getServerWindow )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::ClusterServer::render
        
            typedef void ( ::OSG::ClusterServer::*render_function_type )( ::OSG::RenderActionBase * ) ;
            
            ClusterServer_exposer.def( 
                "render"
                , render_function_type( &::OSG::ClusterServer::render )
                , ( bp::arg("action") ) );
        
        }
        { //::OSG::ClusterServer::setInterface
        
            typedef void ( ::OSG::ClusterServer::*setInterface_function_type )( ::std::string const & ) ;
            
            ClusterServer_exposer.def( 
                "setInterface"
                , setInterface_function_type( &::OSG::ClusterServer::setInterface )
                , ( bp::arg("interf") ) );
        
        }
        { //::OSG::ClusterServer::start
        
            typedef void ( ::OSG::ClusterServer::*start_function_type )(  ) ;
            
            ClusterServer_exposer.def( 
                "start"
                , start_function_type( &::OSG::ClusterServer::start ) );
        
        }
        { //::OSG::ClusterServer::stop
        
            typedef void ( ::OSG::ClusterServer::*stop_function_type )(  ) ;
            
            ClusterServer_exposer.def( 
                "stop"
                , stop_function_type( &::OSG::ClusterServer::stop ) );
        
        }
    }

}
