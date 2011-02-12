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
#include "OSGUtil_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "SimpleSceneManager.pypp.hpp"

using namespace std;
namespace bp = boost::python;

struct SimpleSceneManager_wrapper : OSG::SimpleSceneManager, bp::wrapper< OSG::SimpleSceneManager > {

    SimpleSceneManager_wrapper( )
    : OSG::SimpleSceneManager( )
      , bp::wrapper< OSG::SimpleSceneManager >(){
        // null constructor
    
    }

    virtual void addForeground( ::OSG::Foreground * const fg ) {
        if( bp::override func_addForeground = this->get_override( "addForeground" ) )
            func_addForeground( fg );
        else{
            this->OSG::SimpleSceneManager::addForeground( fg );
        }
    }
    
    void default_addForeground( ::OSG::Foreground * const fg ) {
        OSG::SimpleSceneManager::addForeground( fg );
    }

    virtual ::OSG::Camera * getCamera(  ) {
        if( bp::override func_getCamera = this->get_override( "getCamera" ) )
            return func_getCamera(  );
        else{
            return this->OSG::SimpleSceneManager::getCamera(  );
        }
    }
    
    ::OSG::Camera * default_getCamera(  ) {
        return OSG::SimpleSceneManager::getCamera( );
    }

    virtual ::OSG::DirectionalLight * getHeadlight(  ) {
        if( bp::override func_getHeadlight = this->get_override( "getHeadlight" ) )
            return func_getHeadlight(  );
        else{
            return this->OSG::SimpleSceneManager::getHeadlight(  );
        }
    }
    
    ::OSG::DirectionalLight * default_getHeadlight(  ) {
        return OSG::SimpleSceneManager::getHeadlight( );
    }

    virtual bool getHeadlightState(  ) {
        if( bp::override func_getHeadlightState = this->get_override( "getHeadlightState" ) )
            return func_getHeadlightState(  );
        else{
            return this->OSG::SimpleSceneManager::getHeadlightState(  );
        }
    }
    
    bool default_getHeadlightState(  ) {
        return OSG::SimpleSceneManager::getHeadlightState( );
    }

    virtual ::OSG::Node * getHighlight(  ) {
        if( bp::override func_getHighlight = this->get_override( "getHighlight" ) )
            return func_getHighlight(  );
        else{
            return this->OSG::SimpleSceneManager::getHighlight(  );
        }
    }
    
    ::OSG::Node * default_getHighlight(  ) {
        return OSG::SimpleSceneManager::getHighlight( );
    }

    virtual ::OSG::RenderAction * getRenderAction(  ) {
        if( bp::override func_getRenderAction = this->get_override( "getRenderAction" ) )
            return func_getRenderAction(  );
        else{
            return this->OSG::SimpleSceneManager::getRenderAction(  );
        }
    }
    
    ::OSG::RenderAction * default_getRenderAction(  ) {
        return OSG::SimpleSceneManager::getRenderAction( );
    }

    virtual ::OSG::Node * getRoot(  ) {
        if( bp::override func_getRoot = this->get_override( "getRoot" ) )
            return func_getRoot(  );
        else{
            return this->OSG::SimpleSceneManager::getRoot(  );
        }
    }
    
    ::OSG::Node * default_getRoot(  ) {
        return OSG::SimpleSceneManager::getRoot( );
    }

    virtual bool getUseTraversalAction(  ) {
        if( bp::override func_getUseTraversalAction = this->get_override( "getUseTraversalAction" ) )
            return func_getUseTraversalAction(  );
        else{
            return this->OSG::SimpleSceneManager::getUseTraversalAction(  );
        }
    }
    
    bool default_getUseTraversalAction(  ) {
        return OSG::SimpleSceneManager::getUseTraversalAction( );
    }

    virtual void redraw(  ) {
        if( bp::override func_redraw = this->get_override( "redraw" ) )
            func_redraw(  );
        else{
            this->OSG::SimpleSceneManager::redraw(  );
        }
    }
    
    void default_redraw(  ) {
        OSG::SimpleSceneManager::redraw( );
    }

    virtual void removeForeground( ::OSG::Foreground * const fg ) {
        if( bp::override func_removeForeground = this->get_override( "removeForeground" ) )
            func_removeForeground( fg );
        else{
            this->OSG::SimpleSceneManager::removeForeground( fg );
        }
    }
    
    void default_removeForeground( ::OSG::Foreground * const fg ) {
        OSG::SimpleSceneManager::removeForeground( fg );
    }

    virtual void setAction( ::OSG::RenderAction * action ) {
        if( bp::override func_setAction = this->get_override( "setAction" ) )
            func_setAction( boost::python::ptr(action) );
        else{
            this->OSG::SimpleSceneManager::setAction( boost::python::ptr(action) );
        }
    }
    
    void default_setAction( ::OSG::RenderAction * action ) {
        OSG::SimpleSceneManager::setAction( boost::python::ptr(action) );
    }

    virtual void setCamera( ::OSG::Camera * camera ) {
        if( bp::override func_setCamera = this->get_override( "setCamera" ) )
            func_setCamera( boost::python::ptr(camera) );
        else{
            this->OSG::SimpleSceneManager::setCamera( boost::python::ptr(camera) );
        }
    }
    
    void default_setCamera( ::OSG::Camera * camera ) {
        OSG::SimpleSceneManager::setCamera( boost::python::ptr(camera) );
    }

    virtual void setHeadlight( bool on ) {
        if( bp::override func_setHeadlight = this->get_override( "setHeadlight" ) )
            func_setHeadlight( on );
        else{
            this->OSG::SimpleSceneManager::setHeadlight( on );
        }
    }
    
    void default_setHeadlight( bool on ) {
        OSG::SimpleSceneManager::setHeadlight( on );
    }

    virtual void setHighlight( ::OSG::Node * obj ) {
        if( bp::override func_setHighlight = this->get_override( "setHighlight" ) )
            func_setHighlight( boost::python::ptr(obj) );
        else{
            this->OSG::SimpleSceneManager::setHighlight( boost::python::ptr(obj) );
        }
    }
    
    void default_setHighlight( ::OSG::Node * obj ) {
        OSG::SimpleSceneManager::setHighlight( boost::python::ptr(obj) );
    }

    virtual void setRoot( ::OSG::Node * root ) {
        if( bp::override func_setRoot = this->get_override( "setRoot" ) )
            func_setRoot( boost::python::ptr(root) );
        else{
            this->OSG::SimpleSceneManager::setRoot( boost::python::ptr(root) );
        }
    }
    
    void default_setRoot( ::OSG::Node * root ) {
        OSG::SimpleSceneManager::setRoot( boost::python::ptr(root) );
    }

    virtual void setStatistics( bool on ) {
        if( bp::override func_setStatistics = this->get_override( "setStatistics" ) )
            func_setStatistics( on );
        else{
            this->OSG::SimpleSceneManager::setStatistics( on );
        }
    }
    
    void default_setStatistics( bool on ) {
        OSG::SimpleSceneManager::setStatistics( on );
    }

    virtual void setUseTraversalAction( bool s ) {
        if( bp::override func_setUseTraversalAction = this->get_override( "setUseTraversalAction" ) )
            func_setUseTraversalAction( s );
        else{
            this->OSG::SimpleSceneManager::setUseTraversalAction( s );
        }
    }
    
    void default_setUseTraversalAction( bool s ) {
        OSG::SimpleSceneManager::setUseTraversalAction( s );
    }

    virtual void showAll(  ) {
        if( bp::override func_showAll = this->get_override( "showAll" ) )
            func_showAll(  );
        else{
            this->OSG::SimpleSceneManager::showAll(  );
        }
    }
    
    void default_showAll(  ) {
        OSG::SimpleSceneManager::showAll( );
    }

    virtual void turnHeadlightOff(  ) {
        if( bp::override func_turnHeadlightOff = this->get_override( "turnHeadlightOff" ) )
            func_turnHeadlightOff(  );
        else{
            this->OSG::SimpleSceneManager::turnHeadlightOff(  );
        }
    }
    
    void default_turnHeadlightOff(  ) {
        OSG::SimpleSceneManager::turnHeadlightOff( );
    }

    virtual void turnHeadlightOn(  ) {
        if( bp::override func_turnHeadlightOn = this->get_override( "turnHeadlightOn" ) )
            func_turnHeadlightOn(  );
        else{
            this->OSG::SimpleSceneManager::turnHeadlightOn(  );
        }
    }
    
    void default_turnHeadlightOn(  ) {
        OSG::SimpleSceneManager::turnHeadlightOn( );
    }

    virtual void update(  ) {
        if( bp::override func_update = this->get_override( "update" ) )
            func_update(  );
        else{
            this->OSG::SimpleSceneManager::update(  );
        }
    }
    
    void default_update(  ) {
        OSG::SimpleSceneManager::update( );
    }

    virtual void useOpenSGLogo(  ) {
        if( bp::override func_useOpenSGLogo = this->get_override( "useOpenSGLogo" ) )
            func_useOpenSGLogo(  );
        else{
            this->OSG::SimpleSceneManager::useOpenSGLogo(  );
        }
    }
    
    void default_useOpenSGLogo(  ) {
        OSG::SimpleSceneManager::useOpenSGLogo( );
    }

    virtual ::OSG::Navigator * getNavigator(  ) {
        if( bp::override func_getNavigator = this->get_override( "getNavigator" ) )
            return func_getNavigator(  );
        else{
            return this->OSG::NavigationManager::getNavigator(  );
        }
    }
    
    ::OSG::Navigator * default_getNavigator(  ) {
        return OSG::NavigationManager::getNavigator( );
    }

    virtual ::OSG::Window * getWindow(  ) {
        if( bp::override func_getWindow = this->get_override( "getWindow" ) )
            return func_getWindow(  );
        else{
            return this->OSG::NavigationManager::getWindow(  );
        }
    }
    
    ::OSG::Window * default_getWindow(  ) {
        return OSG::NavigationManager::getWindow( );
    }

    virtual void idle(  ) {
        if( bp::override func_idle = this->get_override( "idle" ) )
            func_idle(  );
        else{
            this->OSG::NavigationManager::idle(  );
        }
    }
    
    void default_idle(  ) {
        OSG::NavigationManager::idle( );
    }

    virtual void key( ::OSG::UChar8 key, ::OSG::Int16 x, ::OSG::Int16 y ) {
        if( bp::override func_key = this->get_override( "key" ) )
            func_key( key, x, y );
        else{
            this->OSG::NavigationManager::key( key, x, y );
        }
    }
    
    void default_key( ::OSG::UChar8 key, ::OSG::Int16 x, ::OSG::Int16 y ) {
        OSG::NavigationManager::key( key, x, y );
    }

    virtual void mouseButtonPress( ::OSG::UInt16 button, ::OSG::Int16 x, ::OSG::Int16 y ) {
        if( bp::override func_mouseButtonPress = this->get_override( "mouseButtonPress" ) )
            func_mouseButtonPress( button, x, y );
        else{
            this->OSG::NavigationManager::mouseButtonPress( button, x, y );
        }
    }
    
    void default_mouseButtonPress( ::OSG::UInt16 button, ::OSG::Int16 x, ::OSG::Int16 y ) {
        OSG::NavigationManager::mouseButtonPress( button, x, y );
    }

    virtual void mouseButtonRelease( ::OSG::UInt16 button, ::OSG::Int16 x, ::OSG::Int16 y ) {
        if( bp::override func_mouseButtonRelease = this->get_override( "mouseButtonRelease" ) )
            func_mouseButtonRelease( button, x, y );
        else{
            this->OSG::NavigationManager::mouseButtonRelease( button, x, y );
        }
    }
    
    void default_mouseButtonRelease( ::OSG::UInt16 button, ::OSG::Int16 x, ::OSG::Int16 y ) {
        OSG::NavigationManager::mouseButtonRelease( button, x, y );
    }

    virtual void mouseMove( ::OSG::Int16 x, ::OSG::Int16 y ) {
        if( bp::override func_mouseMove = this->get_override( "mouseMove" ) )
            func_mouseMove( x, y );
        else{
            this->OSG::NavigationManager::mouseMove( x, y );
        }
    }
    
    void default_mouseMove( ::OSG::Int16 x, ::OSG::Int16 y ) {
        OSG::NavigationManager::mouseMove( x, y );
    }

    virtual void resize( ::OSG::UInt16 width, ::OSG::UInt16 height ) {
        if( bp::override func_resize = this->get_override( "resize" ) )
            func_resize( width, height );
        else{
            this->OSG::NavigationManager::resize( width, height );
        }
    }
    
    void default_resize( ::OSG::UInt16 width, ::OSG::UInt16 height ) {
        OSG::NavigationManager::resize( width, height );
    }

    virtual void setBeacon( ::OSG::Node * const newBeacon ) {
        if( bp::override func_setBeacon = this->get_override( "setBeacon" ) )
            func_setBeacon( newBeacon );
        else{
            this->OSG::NavigationManager::setBeacon( newBeacon );
        }
    }
    
    void default_setBeacon( ::OSG::Node * const newBeacon ) {
        OSG::NavigationManager::setBeacon( newBeacon );
    }

    virtual void setWindow( ::OSG::Window * win ) {
        if( bp::override func_setWindow = this->get_override( "setWindow" ) )
            func_setWindow( boost::python::ptr(win) );
        else{
            this->OSG::NavigationManager::setWindow( boost::python::ptr(win) );
        }
    }
    
    void default_setWindow( ::OSG::Window * win ) {
        OSG::NavigationManager::setWindow( boost::python::ptr(win) );
    }

};

void register_SimpleSceneManager_class(){

    { //::OSG::SimpleSceneManager
        typedef bp::class_< SimpleSceneManager_wrapper, bp::bases< OSG::NavigationManager >, boost::noncopyable > SimpleSceneManager_exposer_t;
        SimpleSceneManager_exposer_t SimpleSceneManager_exposer = SimpleSceneManager_exposer_t( "SimpleSceneManager", bp::init< >() );
        bp::scope SimpleSceneManager_scope( SimpleSceneManager_exposer );
        { //::OSG::SimpleSceneManager::addForeground
        
            typedef void ( ::OSG::SimpleSceneManager::*addForeground_function_type )( ::OSG::Foreground * const ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_addForeground_function_type )( ::OSG::Foreground * const ) ;
            
            SimpleSceneManager_exposer.def( 
                "addForeground"
                , addForeground_function_type(&::OSG::SimpleSceneManager::addForeground)
                , default_addForeground_function_type(&SimpleSceneManager_wrapper::default_addForeground)
                , ( bp::arg("fg") ) );
        
        }
        { //::OSG::SimpleSceneManager::calcViewRay
        
            typedef ::OSG::Line ( ::OSG::SimpleSceneManager::*calcViewRay_function_type )( ::OSG::Int16,::OSG::Int16 ) ;
            
            SimpleSceneManager_exposer.def( 
                "calcViewRay"
                , calcViewRay_function_type( &::OSG::SimpleSceneManager::calcViewRay )
                , ( bp::arg("x"), bp::arg("y") ) );
        
        }
        { //::OSG::SimpleSceneManager::create
        
            typedef ::OSG::SimpleSceneManager * ( ::OSG::SimpleSceneManager::*create_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "create"
                , create_function_type( &::OSG::SimpleSceneManager::create )
                , bp::return_value_policy< bp::manage_new_object >() );
        
        }
        { //::OSG::SimpleSceneManager::getCamera
        
            typedef ::OSG::Camera * ( ::OSG::SimpleSceneManager::*getCamera_function_type )(  ) ;
            typedef ::OSG::Camera * ( SimpleSceneManager_wrapper::*default_getCamera_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getCamera"
                , getCamera_function_type(&::OSG::SimpleSceneManager::getCamera)
                , default_getCamera_function_type(&SimpleSceneManager_wrapper::default_getCamera)
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleSceneManager::getHeadlight
        
            typedef ::OSG::DirectionalLight * ( ::OSG::SimpleSceneManager::*getHeadlight_function_type )(  ) ;
            typedef ::OSG::DirectionalLight * ( SimpleSceneManager_wrapper::*default_getHeadlight_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getHeadlight"
                , getHeadlight_function_type(&::OSG::SimpleSceneManager::getHeadlight)
                , default_getHeadlight_function_type(&SimpleSceneManager_wrapper::default_getHeadlight)
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleSceneManager::getHeadlightState
        
            typedef bool ( ::OSG::SimpleSceneManager::*getHeadlightState_function_type )(  ) ;
            typedef bool ( SimpleSceneManager_wrapper::*default_getHeadlightState_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getHeadlightState"
                , getHeadlightState_function_type(&::OSG::SimpleSceneManager::getHeadlightState)
                , default_getHeadlightState_function_type(&SimpleSceneManager_wrapper::default_getHeadlightState) );
        
        }
        { //::OSG::SimpleSceneManager::getHighlight
        
            typedef ::OSG::Node * ( ::OSG::SimpleSceneManager::*getHighlight_function_type )(  ) ;
            typedef ::OSG::Node * ( SimpleSceneManager_wrapper::*default_getHighlight_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getHighlight"
                , getHighlight_function_type(&::OSG::SimpleSceneManager::getHighlight)
                , default_getHighlight_function_type(&SimpleSceneManager_wrapper::default_getHighlight)
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleSceneManager::getRenderAction
        
            typedef ::OSG::RenderAction * ( ::OSG::SimpleSceneManager::*getRenderAction_function_type )(  ) ;
            typedef ::OSG::RenderAction * ( SimpleSceneManager_wrapper::*default_getRenderAction_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getRenderAction"
                , getRenderAction_function_type(&::OSG::SimpleSceneManager::getRenderAction)
                , default_getRenderAction_function_type(&SimpleSceneManager_wrapper::default_getRenderAction)
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleSceneManager::getRoot
        
            typedef ::OSG::Node * ( ::OSG::SimpleSceneManager::*getRoot_function_type )(  ) ;
            typedef ::OSG::Node * ( SimpleSceneManager_wrapper::*default_getRoot_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getRoot"
                , getRoot_function_type(&::OSG::SimpleSceneManager::getRoot)
                , default_getRoot_function_type(&SimpleSceneManager_wrapper::default_getRoot)
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::SimpleSceneManager::getStatistics
        
            typedef bool ( ::OSG::SimpleSceneManager::*getStatistics_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getStatistics"
                , getStatistics_function_type( &::OSG::SimpleSceneManager::getStatistics ) );
        
        }
        { //::OSG::SimpleSceneManager::getUseTraversalAction
        
            typedef bool ( ::OSG::SimpleSceneManager::*getUseTraversalAction_function_type )(  ) ;
            typedef bool ( SimpleSceneManager_wrapper::*default_getUseTraversalAction_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getUseTraversalAction"
                , getUseTraversalAction_function_type(&::OSG::SimpleSceneManager::getUseTraversalAction)
                , default_getUseTraversalAction_function_type(&SimpleSceneManager_wrapper::default_getUseTraversalAction) );
        
        }
        { //::OSG::SimpleSceneManager::redraw
        
            typedef void ( ::OSG::SimpleSceneManager::*redraw_function_type )(  ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_redraw_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "redraw"
                , redraw_function_type(&::OSG::SimpleSceneManager::redraw)
                , default_redraw_function_type(&SimpleSceneManager_wrapper::default_redraw) );
        
        }
        { //::OSG::SimpleSceneManager::removeForeground
        
            typedef void ( ::OSG::SimpleSceneManager::*removeForeground_function_type )( ::OSG::Foreground * const ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_removeForeground_function_type )( ::OSG::Foreground * const ) ;
            
            SimpleSceneManager_exposer.def( 
                "removeForeground"
                , removeForeground_function_type(&::OSG::SimpleSceneManager::removeForeground)
                , default_removeForeground_function_type(&SimpleSceneManager_wrapper::default_removeForeground)
                , ( bp::arg("fg") ) );
        
        }
        { //::OSG::SimpleSceneManager::setAction
        
            typedef void ( ::OSG::SimpleSceneManager::*setAction_function_type )( ::OSG::RenderAction * ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_setAction_function_type )( ::OSG::RenderAction * ) ;
            
            SimpleSceneManager_exposer.def( 
                "setAction"
                , setAction_function_type(&::OSG::SimpleSceneManager::setAction)
                , default_setAction_function_type(&SimpleSceneManager_wrapper::default_setAction)
                , ( bp::arg("action") ) );
        
        }
        { //::OSG::SimpleSceneManager::setCamera
        
            typedef void ( ::OSG::SimpleSceneManager::*setCamera_function_type )( ::OSG::Camera * ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_setCamera_function_type )( ::OSG::Camera * ) ;
            
            SimpleSceneManager_exposer.def( 
                "setCamera"
                , setCamera_function_type(&::OSG::SimpleSceneManager::setCamera)
                , default_setCamera_function_type(&SimpleSceneManager_wrapper::default_setCamera)
                , ( bp::arg("camera") ) );
        
        }
        { //::OSG::SimpleSceneManager::setHeadlight
        
            typedef void ( ::OSG::SimpleSceneManager::*setHeadlight_function_type )( bool ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_setHeadlight_function_type )( bool ) ;
            
            SimpleSceneManager_exposer.def( 
                "setHeadlight"
                , setHeadlight_function_type(&::OSG::SimpleSceneManager::setHeadlight)
                , default_setHeadlight_function_type(&SimpleSceneManager_wrapper::default_setHeadlight)
                , ( bp::arg("on") ) );
        
        }
        { //::OSG::SimpleSceneManager::setHighlight
        
            typedef void ( ::OSG::SimpleSceneManager::*setHighlight_function_type )( ::OSG::Node * ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_setHighlight_function_type )( ::OSG::Node * ) ;
            
            SimpleSceneManager_exposer.def( 
                "setHighlight"
                , setHighlight_function_type(&::OSG::SimpleSceneManager::setHighlight)
                , default_setHighlight_function_type(&SimpleSceneManager_wrapper::default_setHighlight)
                , ( bp::arg("obj") ) );
        
        }
        { //::OSG::SimpleSceneManager::setRoot
        
            typedef void ( ::OSG::SimpleSceneManager::*setRoot_function_type )( ::OSG::Node * ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_setRoot_function_type )( ::OSG::Node * ) ;
            
            SimpleSceneManager_exposer.def( 
                "setRoot"
                , setRoot_function_type(&::OSG::SimpleSceneManager::setRoot)
                , default_setRoot_function_type(&SimpleSceneManager_wrapper::default_setRoot)
                , ( bp::arg("root") ) );
        
        }
        { //::OSG::SimpleSceneManager::setStatistics
        
            typedef void ( ::OSG::SimpleSceneManager::*setStatistics_function_type )( bool ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_setStatistics_function_type )( bool ) ;
            
            SimpleSceneManager_exposer.def( 
                "setStatistics"
                , setStatistics_function_type(&::OSG::SimpleSceneManager::setStatistics)
                , default_setStatistics_function_type(&SimpleSceneManager_wrapper::default_setStatistics)
                , ( bp::arg("on") ) );
        
        }
        { //::OSG::SimpleSceneManager::setUseTraversalAction
        
            typedef void ( ::OSG::SimpleSceneManager::*setUseTraversalAction_function_type )( bool ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_setUseTraversalAction_function_type )( bool ) ;
            
            SimpleSceneManager_exposer.def( 
                "setUseTraversalAction"
                , setUseTraversalAction_function_type(&::OSG::SimpleSceneManager::setUseTraversalAction)
                , default_setUseTraversalAction_function_type(&SimpleSceneManager_wrapper::default_setUseTraversalAction)
                , ( bp::arg("s") ) );
        
        }
        { //::OSG::SimpleSceneManager::showAll
        
            typedef void ( ::OSG::SimpleSceneManager::*showAll_function_type )(  ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_showAll_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "showAll"
                , showAll_function_type(&::OSG::SimpleSceneManager::showAll)
                , default_showAll_function_type(&SimpleSceneManager_wrapper::default_showAll) );
        
        }
        { //::OSG::SimpleSceneManager::turnHeadlightOff
        
            typedef void ( ::OSG::SimpleSceneManager::*turnHeadlightOff_function_type )(  ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_turnHeadlightOff_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "turnHeadlightOff"
                , turnHeadlightOff_function_type(&::OSG::SimpleSceneManager::turnHeadlightOff)
                , default_turnHeadlightOff_function_type(&SimpleSceneManager_wrapper::default_turnHeadlightOff) );
        
        }
        { //::OSG::SimpleSceneManager::turnHeadlightOn
        
            typedef void ( ::OSG::SimpleSceneManager::*turnHeadlightOn_function_type )(  ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_turnHeadlightOn_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "turnHeadlightOn"
                , turnHeadlightOn_function_type(&::OSG::SimpleSceneManager::turnHeadlightOn)
                , default_turnHeadlightOn_function_type(&SimpleSceneManager_wrapper::default_turnHeadlightOn) );
        
        }
        { //::OSG::SimpleSceneManager::update
        
            typedef void ( ::OSG::SimpleSceneManager::*update_function_type )(  ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_update_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "update"
                , update_function_type(&::OSG::SimpleSceneManager::update)
                , default_update_function_type(&SimpleSceneManager_wrapper::default_update) );
        
        }
        { //::OSG::SimpleSceneManager::useOpenSGLogo
        
            typedef void ( ::OSG::SimpleSceneManager::*useOpenSGLogo_function_type )(  ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_useOpenSGLogo_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "useOpenSGLogo"
                , useOpenSGLogo_function_type(&::OSG::SimpleSceneManager::useOpenSGLogo)
                , default_useOpenSGLogo_function_type(&SimpleSceneManager_wrapper::default_useOpenSGLogo) );
        
        }
        { //::OSG::NavigationManager::getNavigator
        
            typedef ::OSG::Navigator * ( ::OSG::NavigationManager::*getNavigator_function_type )(  ) ;
            typedef ::OSG::Navigator * ( SimpleSceneManager_wrapper::*default_getNavigator_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getNavigator"
                , getNavigator_function_type(&::OSG::NavigationManager::getNavigator)
                , default_getNavigator_function_type(&SimpleSceneManager_wrapper::default_getNavigator)
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::NavigationManager::getWindow
        
            typedef ::OSG::Window * ( ::OSG::NavigationManager::*getWindow_function_type )(  ) ;
            typedef ::OSG::Window * ( SimpleSceneManager_wrapper::*default_getWindow_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "getWindow"
                , getWindow_function_type(&::OSG::NavigationManager::getWindow)
                , default_getWindow_function_type(&SimpleSceneManager_wrapper::default_getWindow)
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::NavigationManager::idle
        
            typedef void ( ::OSG::NavigationManager::*idle_function_type )(  ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_idle_function_type )(  ) ;
            
            SimpleSceneManager_exposer.def( 
                "idle"
                , idle_function_type(&::OSG::NavigationManager::idle)
                , default_idle_function_type(&SimpleSceneManager_wrapper::default_idle) );
        
        }
        { //::OSG::NavigationManager::key
        
            typedef void ( ::OSG::NavigationManager::*key_function_type )( ::OSG::UChar8,::OSG::Int16,::OSG::Int16 ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_key_function_type )( ::OSG::UChar8,::OSG::Int16,::OSG::Int16 ) ;
            
            SimpleSceneManager_exposer.def( 
                "key"
                , key_function_type(&::OSG::NavigationManager::key)
                , default_key_function_type(&SimpleSceneManager_wrapper::default_key)
                , ( bp::arg("key"), bp::arg("x"), bp::arg("y") ) );
        
        }
        { //::OSG::NavigationManager::mouseButtonPress
        
            typedef void ( ::OSG::NavigationManager::*mouseButtonPress_function_type )( ::OSG::UInt16,::OSG::Int16,::OSG::Int16 ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_mouseButtonPress_function_type )( ::OSG::UInt16,::OSG::Int16,::OSG::Int16 ) ;
            
            SimpleSceneManager_exposer.def( 
                "mouseButtonPress"
                , mouseButtonPress_function_type(&::OSG::NavigationManager::mouseButtonPress)
                , default_mouseButtonPress_function_type(&SimpleSceneManager_wrapper::default_mouseButtonPress)
                , ( bp::arg("button"), bp::arg("x"), bp::arg("y") ) );
        
        }
        { //::OSG::NavigationManager::mouseButtonRelease
        
            typedef void ( ::OSG::NavigationManager::*mouseButtonRelease_function_type )( ::OSG::UInt16,::OSG::Int16,::OSG::Int16 ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_mouseButtonRelease_function_type )( ::OSG::UInt16,::OSG::Int16,::OSG::Int16 ) ;
            
            SimpleSceneManager_exposer.def( 
                "mouseButtonRelease"
                , mouseButtonRelease_function_type(&::OSG::NavigationManager::mouseButtonRelease)
                , default_mouseButtonRelease_function_type(&SimpleSceneManager_wrapper::default_mouseButtonRelease)
                , ( bp::arg("button"), bp::arg("x"), bp::arg("y") ) );
        
        }
        { //::OSG::NavigationManager::mouseMove
        
            typedef void ( ::OSG::NavigationManager::*mouseMove_function_type )( ::OSG::Int16,::OSG::Int16 ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_mouseMove_function_type )( ::OSG::Int16,::OSG::Int16 ) ;
            
            SimpleSceneManager_exposer.def( 
                "mouseMove"
                , mouseMove_function_type(&::OSG::NavigationManager::mouseMove)
                , default_mouseMove_function_type(&SimpleSceneManager_wrapper::default_mouseMove)
                , ( bp::arg("x"), bp::arg("y") ) );
        
        }
        { //::OSG::NavigationManager::resize
        
            typedef void ( ::OSG::NavigationManager::*resize_function_type )( ::OSG::UInt16,::OSG::UInt16 ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_resize_function_type )( ::OSG::UInt16,::OSG::UInt16 ) ;
            
            SimpleSceneManager_exposer.def( 
                "resize"
                , resize_function_type(&::OSG::NavigationManager::resize)
                , default_resize_function_type(&SimpleSceneManager_wrapper::default_resize)
                , ( bp::arg("width"), bp::arg("height") ) );
        
        }
        { //::OSG::NavigationManager::setBeacon
        
            typedef void ( ::OSG::NavigationManager::*setBeacon_function_type )( ::OSG::Node * const ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_setBeacon_function_type )( ::OSG::Node * const ) ;
            
            SimpleSceneManager_exposer.def( 
                "setBeacon"
                , setBeacon_function_type(&::OSG::NavigationManager::setBeacon)
                , default_setBeacon_function_type(&SimpleSceneManager_wrapper::default_setBeacon)
                , ( bp::arg("newBeacon") ) );
        
        }
        { //::OSG::NavigationManager::setWindow
        
            typedef void ( ::OSG::NavigationManager::*setWindow_function_type )( ::OSG::Window * ) ;
            typedef void ( SimpleSceneManager_wrapper::*default_setWindow_function_type )( ::OSG::Window * ) ;
            
            SimpleSceneManager_exposer.def( 
                "setWindow"
                , setWindow_function_type(&::OSG::NavigationManager::setWindow)
                , default_setWindow_function_type(&SimpleSceneManager_wrapper::default_setWindow)
                , ( bp::arg("win") ) );
        
        }
    }

}
