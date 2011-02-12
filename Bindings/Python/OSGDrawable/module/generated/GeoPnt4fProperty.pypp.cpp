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
#include "GeoPnt4fProperty.pypp.hpp"

using namespace std;
namespace bp = boost::python;

boost::python::list TypedGeoVectorProperty_OSG__GeoPnt4fPropertyDesc__getFieldPtr(OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc> *self)
{
   boost::python::list result;
   OSG::MFPnt4f const * mf_data = self->getFieldPtr();
   const OSG::UInt32 size(mf_data->size());
   for ( OSG::UInt32 i = 0; i < size; ++i )
   {
      result.append((*mf_data)[i]);
   }
   return result;
}

boost::python::list TypedGeoVectorProperty_OSG__GeoPnt4fPropertyDesc__getField(OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc> *self)
{
   boost::python::list result;
   OSG::MFPnt4f const & mf_data = self->getField();
   const OSG::UInt32 size(mf_data.size());
   for ( OSG::UInt32 i = 0; i < size; ++i )
   {
      result.append(mf_data[i]);
   }
   return result;
}

void register_GeoPnt4fProperty_class(){

    { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >
        typedef bp::class_< OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >, bp::bases< OSG::GeoVectorProperty >, OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>::ObjRecPtr, boost::noncopyable > GeoPnt4fProperty_exposer_t;
        GeoPnt4fProperty_exposer_t GeoPnt4fProperty_exposer = GeoPnt4fProperty_exposer_t( "GeoPnt4fProperty", bp::no_init );
        bp::scope GeoPnt4fProperty_scope( GeoPnt4fProperty_exposer );
        bp::scope().attr("GeoPropDataFieldId") = (int)OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>::GeoPropDataFieldId;
        bp::scope().attr("NextFieldId") = (int)OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>::NextFieldId;
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::clone
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::TransitPtr< OSG::GeoProperty > ( exported_class_t::*clone_function_type )(  ) ;
            
            GeoPnt4fProperty_exposer.def( 
                "clone"
                , clone_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::clone ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::create
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::TransitPtr< OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > > ( *create_function_type )(  );
            
            GeoPnt4fProperty_exposer.def( 
                "create"
                , create_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::create ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::createDependent
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::TransitPtr< OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > > ( *createDependent_function_type )( ::OSG::BitVector );
            
            GeoPnt4fProperty_exposer.def( 
                "createDependent"
                , createDependent_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::createDependent )
                , ( bp::arg("bFlags") ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::createLocal
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::TransitPtr< OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > > ( *createLocal_function_type )( ::OSG::BitVector );
            
            GeoPnt4fProperty_exposer.def( 
                "createLocal"
                , createLocal_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::createLocal )
                , ( bp::arg("bFlags")=(::OSG::BitVector)(OSG::FCLocal::All) ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::dump
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef void ( exported_class_t::*dump_function_type )( ::OSG::UInt32,long unsigned int const ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "dump"
                , dump_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::dump )
                , ( bp::arg("uiIndent")=(::OSG::UInt32)(0), bp::arg("bvFlags")=(long unsigned int const)(0) ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getClassGroupId
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::UInt32 ( *getClassGroupId_function_type )(  );
            
            GeoPnt4fProperty_exposer.def( 
                "getClassGroupId"
                , getClassGroupId_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getClassGroupId ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getClassType
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::FieldContainerType & ( *getClassType_function_type )(  );
            
            GeoPnt4fProperty_exposer.def( 
                "getClassType"
                , getClassType_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getClassType )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getClassTypeId
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::UInt32 ( *getClassTypeId_function_type )(  );
            
            GeoPnt4fProperty_exposer.def( 
                "getClassTypeId"
                , getClassTypeId_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getClassTypeId ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getContainerSize
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::UInt32 ( exported_class_t::*getContainerSize_function_type )(  ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "getContainerSize"
                , getContainerSize_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getContainerSize ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getFormat
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::UInt32 ( exported_class_t::*getFormat_function_type )(  ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "getFormat"
                , getFormat_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getFormat ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getFormatSize
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::UInt32 ( exported_class_t::*getFormatSize_function_type )(  ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "getFormatSize"
                , getFormatSize_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getFormatSize ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getStride
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::UInt32 ( exported_class_t::*getStride_function_type )(  ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "getStride"
                , getStride_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getStride ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getType
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::FieldContainerType & ( exported_class_t::*getType_function_type )(  ) ;
            
            GeoPnt4fProperty_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getType
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::FieldContainerType const & ( exported_class_t::*getType_function_type )(  ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "getType"
                , getType_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getType )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getValue
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::Pnt4f ( exported_class_t::*getValue_function_type )( ::OSG::UInt32 const ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "getValue"
                , getValue_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getValue )
                , ( bp::arg("index") ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getValue
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef void ( exported_class_t::*getValue_function_type )( ::OSG::Pnt4f &,::OSG::UInt32 const ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "getValue"
                , getValue_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getValue )
                , ( bp::arg("val"), bp::arg("index") ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getVectorType
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::UInt32 ( exported_class_t::*getVectorType_function_type )(  ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "getVectorType"
                , getVectorType_function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::getVectorType ) );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::operator[]
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::Point< float, 4u > & ( exported_class_t::*__getitem___function_type )( ::OSG::UInt32 ) ;
            
            GeoPnt4fProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::operator[] )
                , ( bp::arg("index") )
                , bp::return_internal_reference< >() );
        
        }
        { //::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::operator[]
        
            typedef OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc > exported_class_t;
            typedef ::OSG::Point< float, 4u > const & ( exported_class_t::*__getitem___function_type )( ::OSG::UInt32 ) const;
            
            GeoPnt4fProperty_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::OSG::TypedGeoVectorProperty< OSG::GeoPnt4fPropertyDesc >::operator[] )
                , ( bp::arg("index") )
                , bp::return_value_policy< bp::copy_const_reference >() );
        
        }
        GeoPnt4fProperty_exposer.staticmethod( "create" );
        GeoPnt4fProperty_exposer.staticmethod( "createDependent" );
        GeoPnt4fProperty_exposer.staticmethod( "createLocal" );
        GeoPnt4fProperty_exposer.staticmethod( "getClassGroupId" );
        GeoPnt4fProperty_exposer.staticmethod( "getClassType" );
        GeoPnt4fProperty_exposer.staticmethod( "getClassTypeId" );
        GeoPnt4fProperty_exposer.def("getFieldPtr",TypedGeoVectorProperty_OSG__GeoPnt4fPropertyDesc__getFieldPtr);
        GeoPnt4fProperty_exposer.def("getField",TypedGeoVectorProperty_OSG__GeoPnt4fPropertyDesc__getField);
        pyopensg::register_transit< OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc> >::execute();
        bp::implicitly_convertible< OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>::ObjRecPtr, OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>* >();
        bp::implicitly_convertible< OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>::ObjRecPtr, OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>::ObjCPtr >();
        bp::implicitly_convertible< OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>::ObjRecPtr, OSG::GeoVectorProperty* >();
        bp::implicitly_convertible< OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>::ObjRecPtr, OSG::GeoVectorProperty::ObjRecPtr >();
        bp::implicitly_convertible<OSG::TypedGeoVectorProperty<OSG::GeoPnt4fPropertyDesc>::ObjRecPtr, OSG::GeoVectorProperty::ObjCPtr>();
    }

}
