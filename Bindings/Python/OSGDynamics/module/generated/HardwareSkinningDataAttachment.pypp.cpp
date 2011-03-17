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
#include "HardwareSkinningDataAttachment.pypp.hpp"

using namespace std;
namespace bp = boost::python;

void register_HardwareSkinningDataAttachment_class(){

    { //::OSG::HardwareSkinningDataAttachment
        typedef bp::class_< OSG::HardwareSkinningDataAttachment, bp::bases< OSG::HardwareSkinningDataAttachmentBase >, OSG::HardwareSkinningDataAttachment::ObjRecPtr, boost::noncopyable > HardwareSkinningDataAttachment_exposer_t;
        HardwareSkinningDataAttachment_exposer_t HardwareSkinningDataAttachment_exposer = HardwareSkinningDataAttachment_exposer_t( "HardwareSkinningDataAttachment", bp::no_init );
        bp::scope HardwareSkinningDataAttachment_scope( HardwareSkinningDataAttachment_exposer );
        { //::OSG::HardwareSkinningDataAttachment::changed
        
            typedef void ( ::OSG::HardwareSkinningDataAttachment::*changed_function_type )( ::OSG::ConstFieldMaskArg,::OSG::UInt32,::OSG::BitVector ) ;
            
            HardwareSkinningDataAttachment_exposer.def( 
                "changed"
                , changed_function_type( &::OSG::HardwareSkinningDataAttachment::changed )
                , ( bp::arg("whichField"), bp::arg("origin"), bp::arg("details") ) );
        
        }
        { //::OSG::HardwareSkinningDataAttachment::dump
        
            typedef void ( ::OSG::HardwareSkinningDataAttachment::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            HardwareSkinningDataAttachment_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::HardwareSkinningDataAttachment::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        pyopensg::register_transit< OSG::HardwareSkinningDataAttachment >::execute();
        bp::implicitly_convertible< OSG::HardwareSkinningDataAttachment::ObjRecPtr, OSG::HardwareSkinningDataAttachment* >();
        bp::implicitly_convertible< OSG::HardwareSkinningDataAttachment::ObjRecPtr, OSG::HardwareSkinningDataAttachment::ObjCPtr >();
        bp::implicitly_convertible< OSG::HardwareSkinningDataAttachment::ObjRecPtr, OSG::Attachment* >();
        bp::implicitly_convertible< OSG::HardwareSkinningDataAttachment::ObjRecPtr, OSG::Attachment::ObjRecPtr >();
        bp::implicitly_convertible<OSG::HardwareSkinningDataAttachment::ObjRecPtr, OSG::Attachment::ObjCPtr>();
    }

}
