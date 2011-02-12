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
#include "OSGDrawable_mainheader.h"
#include "OsgPtrHelpers.h"
#include "boost/python/suite/indexing/map_indexing_suite.hpp"
#include "boost/python/suite/indexing/vector_indexing_suite.hpp"
#include "Particles.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_Particles_class(){

    { //::OSG::Particles
        typedef bp::class_< OSG::Particles, bp::bases< OSG::ParticlesBase >, OSG::Particles::ObjRecPtr, boost::noncopyable > Particles_exposer_t;
        Particles_exposer_t Particles_exposer = Particles_exposer_t( "Particles", bp::no_init );
        bp::scope Particles_scope( Particles_exposer );
        bp::enum_< OSG::Particles::DrawOrderE>("DrawOrderE")
            .value("Any", OSG::Particles::Any)
            .value("BackToFront", OSG::Particles::BackToFront)
            .value("FrontToBack", OSG::Particles::FrontToBack)
            .export_values()
            ;
        bp::enum_< OSG::Particles::ModeE>("ModeE")
            .value("Points", OSG::Particles::Points)
            .value("Lines", OSG::Particles::Lines)
            .value("ViewDirQuads", OSG::Particles::ViewDirQuads)
            .value("ViewerQuads", OSG::Particles::ViewerQuads)
            .value("Arrows", OSG::Particles::Arrows)
            .value("ViewerArrows", OSG::Particles::ViewerArrows)
            .value("Rectangles", OSG::Particles::Rectangles)
            .value("ShaderQuads", OSG::Particles::ShaderQuads)
            .value("ShaderStrips", OSG::Particles::ShaderStrips)
            .value("LastMode", OSG::Particles::LastMode)
            .export_values()
            ;
        { //::OSG::Particles::changed
        
            typedef void ( ::OSG::Particles::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            Particles_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::Particles::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("detail") ) );
        
        }
        { //::OSG::Particles::drawPrimitives
        
            typedef void ( ::OSG::Particles::*drawPrimitives_function_type )( ::OSG::DrawEnv * ) ;
            
            Particles_exposer.def( 
                "drawPrimitives"
                , drawPrimitives_function_type( &::OSG::Particles::drawPrimitives )
                , ( bp::arg("pEnv") ) );
        
        }
        { //::OSG::Particles::dump
        
            typedef void ( ::OSG::Particles::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            Particles_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::Particles::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::Particles::fill
        
            typedef void ( ::OSG::Particles::*fill_function_type )( ::OSG::DrawableStatsAttachment * ) ;
            
            Particles_exposer.def( 
                "fill"
                , fill_function_type( &::OSG::Particles::fill )
                , ( bp::arg("pStat") ) );
        
        }
        pyopensg::register_transit< OSG::Particles >::execute();
        bp::implicitly_convertible< OSG::Particles::ObjRecPtr, OSG::Particles* >();
        bp::implicitly_convertible< OSG::Particles::ObjRecPtr, OSG::Particles::ObjCPtr >();
        bp::implicitly_convertible< OSG::Particles::ObjRecPtr, OSG::MaterialDrawable* >();
        bp::implicitly_convertible< OSG::Particles::ObjRecPtr, OSG::MaterialDrawable::ObjRecPtr >();
        bp::implicitly_convertible<OSG::Particles::ObjRecPtr, OSG::MaterialDrawable::ObjCPtr>();
    }

}
