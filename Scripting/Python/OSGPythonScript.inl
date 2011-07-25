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

#include "OSGSysSFields.h"
#include "OSGVecFields.h"
#include "OSGMathSFields.h"

//#define DEBUG_FIELDACCESS

OSG_BEGIN_NAMESPACE

#if 0
#define OSGPY_GENERATE_ACCESS_METHODS(FieldT, iNameSpace)            \
    template const FieldT& PythonScript::getSField<FieldT,         \
                                                   iNameSpace>     \
                                         (const std::string& name, \
                                          const FieldT& value);    \
    template void          PythonScript::setSField<FieldT,         \
                                                   iNameSpace>     \
                                         (const std::string& name, \
                                          const FieldT& value);
#endif

#define OSGPY_GENERATE_ACCESS_METHODS(FieldT)            \
    template const FieldT& PythonScript::getSField<FieldT>   \
                                         (const std::string& name, \
                                          const FieldT& value);    \
    template void          PythonScript::setSField<FieldT> \
                                         (const std::string& name, \
                                          const FieldT& value);

template <class T>
void PythonScript::setSField(const std::string& name, const T& value)
{
    FieldDescriptionBase *fieldDesc =
            this->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef SField<T> SFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*this);
    SFieldT *sfield = static_cast<SFieldT*>(editHandle->getField());
    sfield->setValue(value);

#ifdef DEBUG_FIELDACCESS
    std::cout << "Updated (id=" << this->getId() << "/name='" << name
              << "') to " << sfield->getValue() << std::endl;
#endif
}

template<class T>
const T& PythonScript::getSField(const std::string& name, const T& type)
{
    FieldDescriptionBase *fieldDesc = this->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef SField<T> SFieldT;

    GetFieldHandlePtr getHandle = fieldDesc->getField(*this);
    const SFieldT *sfield = static_cast<const SFieldT*>(getHandle->getField());

#ifdef DEBUG_FIELDACCESS
    std::cerr << "[" << fieldDesc->getFieldType().getName() << "] Retrieved value "
              << sfield->getValue() << " from (id="
              << this->getId() << "/name='" << name << "') from field '"
              << sfield->getClassType().getName() << "'" << std::endl;
#endif

    return(sfield->getValue());
}

template <class T, Int32 iNameSpace>
void PythonScript::setSFieldNS(const std::string& name, const T& value)
{
    FieldDescriptionBase *fieldDesc =
            this->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef SField<T, iNameSpace> SFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*this);
    SFieldT *sfield = static_cast<SFieldT*>(editHandle->getField());
    sfield->setValue(value);

#ifdef DEBUG_FIELDACCESS
    std::cout << "Updated (id=" << this->getId() << "/name='" << name
              << "') to " << sfield->getValue() << std::endl;
#endif
}

template<class T, Int32 iNameSpace>
const T& PythonScript::getSFieldNS(const std::string& name, const T& type)
{
    FieldDescriptionBase *fieldDesc = this->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef SField<T, iNameSpace> SFieldT;

    GetFieldHandlePtr getHandle = fieldDesc->getField(*this);
    const SFieldT *sfield = static_cast<const SFieldT*>(getHandle->getField());

#ifdef DEBUG_FIELDACCESS
    std::cerr << "[" << fieldDesc->getFieldType().getName() << "] Retrieved value "
              << sfield->getValue() << " from (id="
              << this->getId() << "/name='" << name << "') from field '"
              << sfield->getClassType().getName() << std::endl;
#endif

    return(sfield->getValue());
}

// Explicit instantiations:
// TODO: bool, Int, Real

//OSGPY_GENERATE_ACCESS_METHODS(bool       )
OSGPY_GENERATE_ACCESS_METHODS(Real64     )
OSGPY_GENERATE_ACCESS_METHODS(Vec2f      )
OSGPY_GENERATE_ACCESS_METHODS(Vec3f      )
OSGPY_GENERATE_ACCESS_METHODS(Color3f    )
OSGPY_GENERATE_ACCESS_METHODS(Color3ub   )
OSGPY_GENERATE_ACCESS_METHODS(Color4f    )
OSGPY_GENERATE_ACCESS_METHODS(Color4ub   )
OSGPY_GENERATE_ACCESS_METHODS(std::string)
OSGPY_GENERATE_ACCESS_METHODS(BoxVolume  )
OSGPY_GENERATE_ACCESS_METHODS(Plane      )
OSGPY_GENERATE_ACCESS_METHODS(Matrix     )
OSGPY_GENERATE_ACCESS_METHODS(Matrix4d   )

// TODO: bool, Int, Real
//OSGPY_GENERATE_ACCESS_METHODS(bool       ,2)
//OSGPY_GENERATE_ACCESS_METHODS(Vec2f      ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Vec3f      ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Color3f    ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Color3ub   ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Color4f    ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Color4ub   ,0)
//OSGPY_GENERATE_ACCESS_METHODS(std::string,0)
//OSGPY_GENERATE_ACCESS_METHODS(BoxVolume  ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Plane      ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Matrix     ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Matrix4d   ,0)

// not supported either by python bindings or csm:
//OSGPY_GENERATE_ACCESS_METHODS(Int32      ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Real32     ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Quaternion ,0)
//OSGPY_GENERATE_ACCESS_METHODS(Quaterniond,0)

OSG_END_NAMESPACE
