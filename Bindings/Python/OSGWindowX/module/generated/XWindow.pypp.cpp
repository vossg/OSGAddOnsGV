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
#include "OSGWindowX_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "XWindow.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_XWindow_class(){

    { //::OSG::XWindow
        typedef bp::class_< OSG::XWindow, bp::bases< OSG::XWindowBase >, OSG::XWindow::ObjRecPtr, boost::noncopyable > XWindow_exposer_t;
        XWindow_exposer_t XWindow_exposer = XWindow_exposer_t( "XWindow", bp::no_init );
        bp::scope XWindow_scope( XWindow_exposer );
        { //::OSG::XWindow::changed
        
            typedef void ( ::OSG::XWindow::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            XWindow_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::XWindow::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("detail") ) );
        
        }
        { //::OSG::XWindow::dump
        
            typedef void ( ::OSG::XWindow::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            XWindow_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::XWindow::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::XWindow::terminate
        
            typedef void ( ::OSG::XWindow::*terminate_function_type )(  ) ;
            
            XWindow_exposer.def( 
                "terminate"
                , terminate_function_type( &::OSG::XWindow::terminate ) );
        
        }
        pyopensg::register_transit< OSG::XWindow >::execute();
        bp::implicitly_convertible< OSG::XWindow::ObjRecPtr, OSG::XWindow* >();
        bp::implicitly_convertible< OSG::XWindow::ObjRecPtr, OSG::XWindow::ObjCPtr >();
        bp::implicitly_convertible< OSG::XWindow::ObjRecPtr, OSG::Window* >();
        bp::implicitly_convertible< OSG::XWindow::ObjRecPtr, OSG::Window::ObjRecPtr >();
        bp::implicitly_convertible<OSG::XWindow::ObjRecPtr, OSG::Window::ObjCPtr>();
    }

}
