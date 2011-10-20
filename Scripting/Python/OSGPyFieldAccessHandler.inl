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

#include "OSGPyFieldAccessHandler.h"

OSG_BEGIN_NAMESPACE

#if 0 // Causes multiple definitions...
std::string PyFieldAccessHandler::getValueSField_String(const std::string &fieldName)
{
    typedef SField<std::string, 0> FieldT;

    GetFieldHandlePtr       handle =
            _pPythonScript->getDynamicFieldByName(fieldName.c_str());
    FieldT::GetHandlePtr getHandle =
            boost::dynamic_pointer_cast<FieldT::GetHandle>(handle);
    assert(getHandle);

    return getHandle->getField()->getValue();
}

void PyFieldAccessHandler::setValueSField_String(const std::string& fieldName,
                                                 const std::string& value)
{
    typedef SField<std::string, 0> FieldT;

    EditFieldHandlePtr        handle =
        _pPythonScript->editDynamicFieldByName(fieldName.c_str());

    FieldT::EditHandlePtr editHandle =
        boost::dynamic_pointer_cast<FieldT::EditHandle>(handle);
    assert(editHandle);

    editHandle->getField()->setValue(value);
}
#endif

OSG_END_NAMESPACE

