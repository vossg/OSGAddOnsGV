/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                     Copyright 2000-2002 by OpenSG Forum                   *
 *                                                                           *
 *   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                License                                    *
 *                                                                           *
 * This library is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Library General Public License as published    *
 * by the Free Software Foundation, version 2.                               *
 *                                                                           *
 * This library is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public         *
 * License along with this library; if not, write to the Free Software       *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Changes                                    *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

#include "OSGSysFields.h"
#include "OSGVecFields.h"
#include "OSGMathFields.h"

#include "OSGPyFieldAccessHandler.h"

//#define OSGPY_DEBUG_FIELDACCESS

OSG_BEGIN_NAMESPACE


//-----------------------------------------------------------------------------
//                           Data-field macros
//-----------------------------------------------------------------------------


#define OSGPY_DATA_FIELD_INST_GEN(FieldT)                   \
    template void          PyFieldAccessHandler::setSField<FieldT>           \
                                         (const std::string& name,   \
                                          const FieldT& value     ); \
    template       FieldT& PyFieldAccessHandler::myEditSField<FieldT>        \
                                         (const std::string& name,   \
                                          const FieldT& type      ); \
    template const FieldT& PyFieldAccessHandler::getSField<FieldT>           \
                                         (const std::string& name,   \
                                          const FieldT& type      ); \
    template const MField<FieldT>* PyFieldAccessHandler::myGetMField<FieldT> \
                                         (const std::string& name, \
                                          FieldT const & value);    \
    template void          PyFieldAccessHandler::setMField<FieldT>         \
                                         (const std::string& name, \
                                          bp::list values,         \
                                          const FieldT& type);


template <class T>
void PyFieldAccessHandler::setSField(const std::string& name, const T& value)
{
    FieldDescriptionBase *fieldDesc =
            _pPythonScript->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef SField<T> SFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*_pPythonScript);
    SFieldT *sfield = static_cast<SFieldT*>(editHandle->getField());
    sfield->setValue(value);

#ifdef OSGPY_DEBUG_FIELDACCESS
    std::cerr << "setSField: type=" << typeid(value).name() << std::endl;
    std::cerr << "sfield: " << sfield << std::endl;
    std::cerr << "Updated (id=" << fieldDesc->getTypeId() << "/name='" << name
              << "'/type='" << sfield->getClassType().getName() << "') to " << sfield->getValue() << std::endl;
#endif
}

template<class T>
const T& PyFieldAccessHandler::getSField(const std::string& name, const T& type)
{
    FieldDescriptionBase *fieldDesc = _pPythonScript->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef SField<T> SFieldT;

    GetFieldHandlePtr getHandle = fieldDesc->getField(*_pPythonScript);
    const SFieldT *sfield = static_cast<const SFieldT*>(getHandle->getField());

#ifdef OSGPY_DEBUG_FIELDACCESS
    std::cerr << "[" << fieldDesc->getFieldType().getName() << "] getSField value "
              << sfield->getValue() << " from (id="
              << fieldDesc->getTypeId() << "/name='" << name << "'/type='"
              << sfield->getClassType().getName() << "')" << std::endl;
#endif

    return(sfield->getValue());
}

template<class T>
T& PyFieldAccessHandler::myEditSField(const std::string& name, const T& type)
{
    FieldDescriptionBase *fieldDesc = _pPythonScript->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef SField<T> SFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*_pPythonScript);
    SFieldT *sfield = static_cast<const SFieldT*>(editHandle->getField());

#ifdef OSGPY_DEBUG_FIELDACCESS
    std::cerr << "[" << fieldDesc->getFieldType().getName() << "] myEditSField value "
              << sfield->getValue() << " from (id="
              << fieldDesc->getTypeId() << "/name='" << name << "'/type='"
              << sfield->getClassType().getName() << "')" << std::endl;
#endif

    return(sfield->getValue());
}

template <class T>
void PyFieldAccessHandler::setMField(const std::string& name,
                               bp::list values,
                               const T& type)
{
    FieldDescriptionBase *fieldDesc =
            _pPythonScript->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef MField<T> MFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*_pPythonScript);
    MFieldT *mfield = static_cast<MFieldT*>(editHandle->getField());

#ifdef OSGPY_DEBUG_FIELDACCESS
    std::cout << "Processing list with size = " << bp::len(values) << std::endl;
#endif

//    if(size > mfield->size())
//    {
//        const char message[] = "list is longer as the size of the multi-field";
//        PyErr_SetString(PyExc_ValueError, message);
//        throw bp::error_already_set();
//    }

    for(bp::ssize_t idx=0; idx<bp::len(values); ++idx)
    {
        if(idx < mfield->size())
        {
//            bp::object object = values[idx];//bp::extract<T>();
//            T asdf = bp::extract<T>(object);
//            mfield->replace(idx, asdf);
            mfield->replace(idx, bp::extract<T>(values[idx]));
#ifdef OSGPY_DEBUG_FIELDACCESS
            std::cout << "setMField[" << fieldDesc->getFieldType().getName() << "]: UPDATED value " << mfield->getValues()[idx]
                      << " at index " << idx
                      << std::endl;
#endif
        }
        else
        {
            mfield->push_back(bp::extract<T>(values[idx]));
#ifdef OSGPY_DEBUG_FIELDACCESS
            std::cout << "setMField[" << fieldDesc->getFieldType().getName() << "]: ADDED new value " << mfield->getValues()[idx]
                      << " at index " << idx
                      << std::endl;
#endif
        }
    }
}

template<class T>
const MField<T>* PyFieldAccessHandler::myGetMField(const std::string& name, T const & type)
{
    FieldDescriptionBase *fieldDesc = _pPythonScript->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef MField<T> MFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*_pPythonScript);
    const MFieldT *mfield = static_cast<const MFieldT*>(editHandle->getField());

#ifdef OSGPY_DEBUG_FIELDACCESS
    std::cerr << "[" << fieldDesc->getFieldType().getName() << "] Retrieved value "
              << sfield->getValue() << " from (id="
              << fieldDesc->getTypeId() << "/name='" << name << "'/type='"
              << sfield->getClassType().getName() << "')" << std::endl;
#endif

    return mfield;
}


#if 0
//-----------------------------------------------------------------------------
//                           Pointer-field macros
//-----------------------------------------------------------------------------


#define OSGPY_POINTER_FIELD_INST_GEN(FieldT)         \
    template const FieldT* PyFieldAccessHandler::getPointerSField<FieldT>  \
                                         (const std::string& name, \
                                          FieldT* value);    \
    template void          PyFieldAccessHandler::setPointerSField<FieldT>  \
                                         (const std::string& name, \
                                          FieldT* value); \
    template const PointerMField<FieldT*, UnrecordedRefCountPolicy, 0>* \
                           PyFieldAccessHandler::getPointerMField<FieldT> \
                                         (const std::string& name, \
                                          FieldT const & value);    \
    template void          PyFieldAccessHandler::setPointerMField<FieldT>         \
                                         (const std::string& name, \
                                          bp::list values,         \
                                          const FieldT& type);

template <class T>
void PyFieldAccessHandler::setPointerSField(const std::string& name, T* value)
{
    FieldDescriptionBase *fieldDesc =
            _pPythonScript->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef PointerSField<T*, UnrecordedRefCountPolicy, 0> SFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*_pPythonScript);
    SFieldT *sfield = static_cast<SFieldT*>(editHandle->getField());
    sfield->setValue(value);

#ifdef OSGPY_DEBUG_FIELDACCESS
    std::cerr << "Updated (id=" << fieldDesc->getTypeId() << "/name='" << name
              << "'/type='" << sfield->getClassType().getName() << "') to " << sfield->getValue() << std::endl;
#endif
}

template<class T>
const T* PyFieldAccessHandler::getPointerSField(const std::string& name, T* type)
{
    FieldDescriptionBase *fieldDesc = _pPythonScript->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef PointerSField<T*, UnrecordedRefCountPolicy, 0> SFieldT;

    GetFieldHandlePtr getHandle = fieldDesc->getField(*_pPythonScript);
    const SFieldT *sfield = static_cast<const SFieldT*>(getHandle->getField());

#ifdef OSGPY_DEBUG_FIELDACCESS
    std::cerr << "getPointerSField: type=" << typeid(type).name() << std::endl;
    std::cerr << "sfield: " << sfield << std::endl;
    std::cerr << "[" << fieldDesc->getFieldType().getName() << "] Retrieved value "
              << sfield->getValue() << " from (id="
              << fieldDesc->getTypeId() << "/name='" << name << "'/type='"
              << sfield->getClassType().getName() << "')" << std::endl;
#endif

    return(sfield->getValue());
}

template <class T>
void PyFieldAccessHandler::setPointerMField(const std::string& name,
                                            bp::list values,
                                            const T& type)
{
    FieldDescriptionBase *fieldDesc =
            _pPythonScript->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef PointerMField<T*, UnrecordedRefCountPolicy, 0> MFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*_pPythonScript);
    MFieldT *mfield = static_cast<MFieldT*>(editHandle->getField());

#ifdef OSGPY_DEBUG_FIELDACCESS
    std::cout << "Processing list with size = " << bp::len(values) << std::endl;
#endif

//    if(size > mfield->size())
//    {
//        const char message[] = "list is longer as the size of the multi-field";
//        PyErr_SetString(PyExc_ValueError, message);
//        throw bp::error_already_set();
//    }

    for(bp::ssize_t idx=0; idx<bp::len(values); ++idx)
    {
        if(idx < mfield->size())
        {
//            bp::object object = values[idx];//bp::extract<T>();
//            T asdf = bp::extract<T>(object);
//            mfield->replace(idx, asdf);
            mfield->replace(idx, bp::extract<T>(values[idx]));
#ifdef OSGPY_DEBUG_FIELDACCESS
            std::cout << "setPointerMField[" << fieldDesc->getFieldType().getName() << "]: UPDATED value " << mfield->getValues()[idx]
                      << " at index " << idx
                      << std::endl;
#endif
        }
        else
        {
            mfield->push_back(bp::extract<T>(values[idx]));
#ifdef OSGPY_DEBUG_FIELDACCESS
            std::cout << "setPointerMField[" << fieldDesc->getFieldType().getName() << "]: ADDED new value " << mfield->getValues()[idx]
                      << " at index " << idx
                      << std::endl;
#endif
        }
    }
}

template<class T>
const PointerMField<T*, UnrecordedRefCountPolicy, 0>* PyFieldAccessHandler::getPointerMField(const std::string& name, T const & type)
{
    FieldDescriptionBase *fieldDesc = _pPythonScript->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef PointerMField<T*, UnrecordedRefCountPolicy, 0> MFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*_pPythonScript);
    const MFieldT *mfield = static_cast<const MFieldT*>(editHandle->getField());

#ifdef OSGPY_DEBUG_FIELDACCESS
    std::cerr << "[" << fieldDesc->getFieldType().getName() << "] Retrieved value "
              << sfield->getValue() << " from (id="
              << fieldDesc->getTypeId() << "/name='" << name << "'/type='"
              << sfield->getClassType().getName() << "')" << std::endl;
#endif

    return mfield;
}
#endif

//-----------------------------------------------------------------------------
//                         Template instantiations
//-----------------------------------------------------------------------------

                                                // -> CSM equivalent (taken from OSGScanParseSkelParser.cpp)
OSGPY_DATA_FIELD_INST_GEN(Int32      ) // -> SFInt32
OSGPY_DATA_FIELD_INST_GEN(Real64     ) // -> SFDouble
OSGPY_DATA_FIELD_INST_GEN(std::string) // -> SFString

OSGPY_DATA_FIELD_INST_GEN(Pnt2f      ) // -> SFPnt2f
OSGPY_DATA_FIELD_INST_GEN(Pnt3f      ) // -> SFPnt3f
OSGPY_DATA_FIELD_INST_GEN(Pnt4f      ) // -> SFPnt4f
OSGPY_DATA_FIELD_INST_GEN(Pnt2d      ) // -> SFPnt2d
OSGPY_DATA_FIELD_INST_GEN(Pnt3d      ) // -> SFPnt4d
OSGPY_DATA_FIELD_INST_GEN(Pnt4d      ) // -> SFPnt4d

OSGPY_DATA_FIELD_INST_GEN(Vec2f      ) // -> SFVec2f
OSGPY_DATA_FIELD_INST_GEN(Vec3f      ) // -> SFVec3f
OSGPY_DATA_FIELD_INST_GEN(Vec4f      ) // -> SFVec4f
OSGPY_DATA_FIELD_INST_GEN(Vec2d      ) // -> SFVec2d
OSGPY_DATA_FIELD_INST_GEN(Vec3d      ) // -> SFVec4d
OSGPY_DATA_FIELD_INST_GEN(Vec4d      ) // -> SFVec4d

OSGPY_DATA_FIELD_INST_GEN(Matrix     ) // -> SFMatrix
OSGPY_DATA_FIELD_INST_GEN(Matrix4d   ) // -> SFMatrix4d
OSGPY_DATA_FIELD_INST_GEN(Quaternion ) // -> SFRotation

OSGPY_DATA_FIELD_INST_GEN(Color3f    ) // -> SFColor
OSGPY_DATA_FIELD_INST_GEN(Color4f    ) // -> SFColorRGBA

OSGPY_DATA_FIELD_INST_GEN(BoxVolume  ) // -> SFVolume
OSGPY_DATA_FIELD_INST_GEN(Plane      ) // -> SFPlane

//OSGPY_POINTER_FIELD_INST_GEN(Node) // -> SFNode

OSG_END_NAMESPACE

