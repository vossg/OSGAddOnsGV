/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
 *                                                                           *
 *                            www.opensg.org                                 *
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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include "OSGAddOnsConfig.h"

#include "OSGPyFieldAccessHandler.h"

#include "OSGTypeFactory.h"

#include <boost/python/object.hpp>
#include <boost/python/import.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>

//#define OSGPY_DEBUG_CODEGENERATION
//#define OSGPY_DEBUG_FIELDACCESS

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGPyFieldAccessHandlerBase.cpp file.
// To modify it, please change the .fcd file (OSGPyFieldAccessHandler.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void PyFieldAccessHandler::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
    }
}

/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  public                                                                 -
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                                Init                                     */

/*!\brief Initializes the instance and binds the PythonScript to it.   */
/*        Internally global Python functions needed for the field      */
/*        access are added to the interpreter and the currently        */
/*        present dynamic fields are exposed to Python.                */
/*                                                                     */
/*        Dynamically added fields are handled in the addFields()      */
/*        methods.                                                     */
/*                                                                     */
/*\return true if the initialization was successful, false otherwise.  */
/*                                                                     */
/*\see addFields                                                       */
bool PyFieldAccessHandler::init(PythonScript* pythonScript)
{
    _pPythonScript  = pythonScript;
    _pPyInterpreter = _pPythonScript->getInterpreter();

    const std::string addGetSetProperty(
                "def addfullprop(inst, name, getter, setter):\n"
                "   cls = type(inst)\n"
                "   if not hasattr(inst.__class__, '__perinstance'):\n"
                "       inst.__class__ = type(cls.__name__, (cls,), {})\n"
                "       inst.__class__.__perinstance = True\n"
                "   setattr(inst.__class__, name, property(getter, setter))\n");

    const std::string addGetProperty(
                "def addgetprop(inst, name, getter):\n"
                "   cls = type(inst)\n"
                "   if not hasattr(inst.__class__, '__perinstance'):\n"
                "       inst.__class__ = type(cls.__name__, (cls,), {})\n"
                "       inst.__class__.__perinstance = True\n"
                "   setattr(inst.__class__, name, property(getter))\n");

    const std::string addMethod(
                "def addmethod(inst, name, func):\n"
                "   cls = type(inst)\n"
                "   if not hasattr(inst.__class__, '__perinstance'):\n"
                "       inst.__class__ = type(cls.__name__, (cls,), {})\n"
                "       inst.__class__.__perinstane = True\n"
                "   setattr(inst.__class__, name, func)\n");

    bool flag = _pPyInterpreter->run(addGetSetProperty + addGetProperty + addMethod);
    if(!flag)
    {
        return false;
    }

    return exposeAllFieldsToPython();
}

/*!\brief  Generates the Python glue code that allows field access     */
/*         from within the Python script for all dynamic fields        */
/*         currently attached to the PythonScript. If one field fails  */
/*         to be exposed it is not accessible via Python and an error  */
/*         message is printed to std::cerr. The other fields are still */
/*         processed in that case.                                     */
/*                                                                     */
/* \return true if every field was processed successfully. false if    */
/*         one or more fields failed to be exposed to Python.          */
bool PyFieldAccessHandler::exposeAllFieldsToPython()
{
    for(UInt32 idx = 1; idx <= _pPythonScript->getType().getNumFieldDescs(); ++idx)
    {
        FieldDescriptionBase *desc = _pPythonScript->getType().getFieldDesc(idx);
        assert(desc);

#ifdef OSGPY_DEBUG_CODEGENERATION
        std::string tmp = (desc->isDynamic()) ? "dynamic. Analyzing..." : "static. Skip...";
        std::cout << "Processing field '" << desc->getName() << "' | "
                  << tmp << std::endl;
#endif

        if(desc->isDynamic() == true)
        {
            if(exposeFieldToPython(desc->getName(), desc->getFieldId()) == true)
            {
#ifdef OSGPY_DEBUG_CODEGENERATION
                std::cout << "Successfully exposed field '" << desc->getName()
                          << "'" << std::endl << std::endl;
#endif
            }
            else
            {
                std::cerr << "Error exposing field " << desc->getName()
                          << std::endl;

                _pPyInterpreter->activate();
                _pPyInterpreter->dumpError();
                _pPyInterpreter->clearError();
                _pPyInterpreter->deactivate();

                std::cerr << "The field will not be accessible in Python!"
                          << std::endl << std::endl;
            }
        }
    }

    return true;
}

/*!\brief Exposes a single filed with the given fieldName and fieldId  */
/*        to the Python interpreter. Afterwards access methods are     */
/*        available to retrieve and set the field values within the    */
/*        Python script (see the user FAQ for a detailed description   */
/*        of those). The fields Id and the FieldMask are also exposed. */
/*                                                                     */
/* \param fieldName Name of the field to be exposed                    */
/* \param propName  Name of the property                               */
/* \param fieldId   Id of the field                                    */
/*                                                                     */
/* \return true on success, false otherwise. In an error case you can  */
/*         retrieve more information via the interpreter error         */
/*         interface. Clearing the error is in the responsibility of   */
/*         the developer.                                              */
bool PyFieldAccessHandler::exposeFieldToPython(const std::string& fieldName,
                                               OSG::UInt32  fieldId)
{
    bool setAccess (false);
    bool editAccess(false);
    bool getAccess (false);

    if(generateCorePythonCode(fieldName,
                                 getAccess,
                                 editAccess,
                                 setAccess) == false)
    {
        return false;
    };

    std::ostringstream pyCode;
    if(setAccess == true)
    {
        pyCode << "addmethod(self, 'set_"  << fieldName << "', _set_"   << fieldName  << ")" << std::endl;
    }
    if(editAccess == true)
    {
        pyCode << "addmethod(self, 'edit_" << fieldName << "', _edit_"  << fieldName  << ")" << std::endl;
    }
    if(getAccess == true)
    {
       pyCode << "addmethod(self, 'get_"  << fieldName << "', _get_"   << fieldName  << ")" << std::endl;
    }

    // Enable access via a property:
    if(getAccess && setAccess)
    {
        pyCode << "addfullprop(self, '" << fieldName  << "', _get_" << fieldName
               << ", _set_" << fieldName << ")" << std::endl;
    }
    else if(getAccess && !setAccess)
    {
        pyCode << "addgetprop(self, '" << fieldName  << "', _get_" << fieldName
               << ")" << std::endl;
    }

    pyCode << "self." << fieldName << "FieldMask = 1 << " << fieldId << std::endl
           << "self." << fieldName << "FieldId   = "      << fieldId << std::endl;

#ifdef OSGPY_DEBUG_CODEGENERATION
    std::cout << pyCode.str() << std::endl;
#endif

    return(_pPyInterpreter->run(pyCode.str()));
}

/*-------------------------------------------------------------------------*/
/*                                Sync                                     */

void PyFieldAccessHandler::changed(ConstFieldMaskArg whichField,
                            UInt32            origin,
                            BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

/*-------------------------------------------------------------------------*/
/*                               Output                                    */

void PyFieldAccessHandler::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump PyFieldAccessHandler NI" << std::endl;
}

/*-------------------------------------------------------------------------*/
/*                        Pointer Field Access                             */

/*\brief Sets a pointer single-field via its generic FieldContainer*.      */
void PyFieldAccessHandler::setPtrSField(const std::string& fieldName,
                                        FieldContainer*    value)
{
    EditFieldHandlePtr handle = _pPythonScript->editDynamicFieldByName(fieldName.c_str());

    FieldContainerPtrSFieldBase::EditHandlePtr sfFCPtr =
        boost::dynamic_pointer_cast<
            FieldContainerPtrSFieldBase::EditHandle>(handle);
    assert(sfFCPtr);

    sfFCPtr->set(value);
}

/*!\brief Returns a recorded reference to the FieldContainer* of the   */
/*        field. Note that the constness is not preserved in Python!   */
/*        In Python the FieldContainer is automatically casted to the  */
/*        correct type.                                                */
/*                                                                     */
/* \note Normally you would return a FieldContainerTransitPtr here,    */
/*       but there is no way to return a const                         */
/*       FieldContainerTransitPtr.                                     */
const FieldContainerRecPtr PyFieldAccessHandler::getPtrSField(const std::string& fieldName)
{
    GetFieldHandlePtr handle = _pPythonScript->getDynamicFieldByName(fieldName.c_str());

    FieldContainerPtrSFieldBase::GetHandlePtr sfFCPtr =
        boost::dynamic_pointer_cast<
            FieldContainerPtrSFieldBase::GetHandle>(handle);
    assert(sfFCPtr);

    const FieldContainerPtrSFieldBase* field =
            static_cast<const FieldContainerPtrSFieldBase *>(sfFCPtr->getField());
    return FieldContainerTransitPtr(field->getValue());
}

/*!\brief Returns the FieldContainer* of the given field. In Python    */
/*        the generic FieldContainer* is automatically casted to the   */
/*        correct type.                                                */
FieldContainerTransitPtr PyFieldAccessHandler::changePtrSField(const std::string& fieldName)
{
    EditFieldHandlePtr handle = _pPythonScript->editDynamicFieldByName(fieldName.c_str());

    FieldContainerPtrSFieldBase::EditHandlePtr sfFCPtr =
        boost::dynamic_pointer_cast<
            FieldContainerPtrSFieldBase::EditHandle>(handle);
    assert(sfFCPtr);

    FieldContainerPtrSFieldBase* field =
            static_cast<FieldContainerPtrSFieldBase *>(sfFCPtr->getField());
    return FieldContainerTransitPtr(field->getValue());
}

/*!\brief Returns a custom handle to the pointer multi-field that      */
/*        provides read access to the content of the multi-field via   */
/*        a pythonic iterator interface.                               */
/*                                                                     */
/* \see PyFieldContainerPtrMFieldGetHandle                             */
PyFieldContainerPtrMFieldGetHandleTransitPtr PyFieldAccessHandler::getPointerMField(const std::string& fieldName)
{
    PyFieldContainerPtrMFieldGetHandleTransitPtr handle =
            PyFieldContainerPtrMFieldGetHandle::create();
    handle->init(_pPythonScript, fieldName);

    return(handle);
}

/*!\brief Returns a custom handle to the pointer multi-field that      */
/*        provides read/write access to the content of the multi-field */
/*        via a pythonic iterator interface.                           */
/*                                                                     */
/* \see PyFieldContainerPtrMFieldEditHandle                            */
PyFieldContainerPtrMFieldEditHandleTransitPtr PyFieldAccessHandler::changePointerMField(const std::string& fieldName)
{
    PyFieldContainerPtrMFieldEditHandleTransitPtr handle =
            PyFieldContainerPtrMFieldEditHandle::create();
    handle->init(_pPythonScript, fieldName);

    return(handle);
}

/*-------------------------------------------------------------------------*\
 -  protected                                                                 -
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                      Constructors & Destructors                         */

PyFieldAccessHandler::PyFieldAccessHandler(void) :
    Inherited()
{
}

PyFieldAccessHandler::PyFieldAccessHandler(const PyFieldAccessHandler &source) :
    Inherited(source)
{
}

PyFieldAccessHandler::~PyFieldAccessHandler(void)
{
    fprintf(stderr, "PyFieldAccessHandler: Destructor\n");

    _pPythonScript  = 0;
    _pPyInterpreter = 0;
}

/*-------------------------------------------------------------------------*\
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                                  Python                                 */

/*\brief Adds the core Python code to access a field. Internally       */
/*       Python methods are generated that make use of the C++ access  */
/*       methods to retrieve or set the field value. These methods are */
/*       considered private (they have a underscore prefix) and should */
/*       not be used by the user directly. They are wrapped into       */
/*       public methods in the exposeFieldToPython() method.           */
/*                                                                     */
/*\param fieldName name of the field to generate the core access code  */
/*\param getAccess a bool that is set to true if a get-access method   */
/*                 was generated for the field (which is true for all  */
/*                 fields)                                             */
/*\param editAccess a bool that is set to true if an edit-access       */
/*                 method was generated for the field (false for all   */
/*                 integral OSG types: Bool, Int32, Int64, UInt32,     */
/*                 UInt64, Real32, Real64, Time and String, true       */
/*                 otherwise.)                                         */
/*                 was generated for the field (which is true for all  */
/*                 fields)                                             */
/*\param setAccess a bool that is set to true if a set-access method   */
/*                 was generated for the field (false for all pointer  */
/*                 multi-fields, true otherwise)                       */
/*                                                                     */
/*\return true if no Python error was set, false otherwise             */
bool PyFieldAccessHandler::generateCorePythonCode(const std::string& fieldName,
                                                  bool& getAccess,
                                                  bool& editAccess,
                                                  bool& setAccess)
{
    FieldDescriptionBase *desc =
            _pPythonScript->getType().getFieldDesc(fieldName.c_str());
    FieldType::Class classType = desc->getFieldType().getClass();
    std::string fieldTypeName(desc->getFieldType().getName());
    std::string valueTypeName(fieldTypeName);

    removeFieldQualifier   (valueTypeName);
    removeRefCountQualifier(valueTypeName);

    std::string setAccessMethod ("unsupported");
    std::string editAccessMethod("unsupported");
    std::string getAccessMethod ("unsupported");

    switch(classType)
    {
    case FieldType::ValueField:
    {
        if(desc->isSField())
        {
            setAccessMethod  = "setValueSField_"    + valueTypeName;
            getAccessMethod  = "getValueSField_"    + valueTypeName;

            // The following types cannot be changed using the
            // editAccessMethod, as Python does not provide access via a
            // reference for them. They have to be set with the
            // setAccessMethod.
            if(valueTypeName != "Bool"   &&
               valueTypeName != "Int32"  &&
               valueTypeName != "Int64"  &&
               valueTypeName != "UInt32" &&
               valueTypeName != "UInt64" &&
               valueTypeName != "Real32" &&
               valueTypeName != "Real64" &&
               valueTypeName != "Time"   &&
               valueTypeName != "String"   )
            {
                editAccessMethod = "changeValueSField_" + valueTypeName;
            }

#ifdef OSGPY_DEBUG_CODEGENERATION
            std::cout << "    VALUE single-field" << std::endl;
#endif
        }
        else
        {
            setAccessMethod  = "setValueMField_"    + valueTypeName;
            editAccessMethod = "changeValueMField_" + valueTypeName;
            getAccessMethod  = "getValueMField_"    + valueTypeName;

#ifdef OSGPY_DEBUG_CODEGENERATION
            std::cout << "    VALUE multi-field" << std::endl;
#endif
        }
    }
    break;
    case FieldType::PtrField:
    {
        if(desc->isSField())
        {
            setAccessMethod  = "setPtrSField";
            editAccessMethod = "changePtrSField";
            getAccessMethod  = "getPtrSField";
#ifdef OSGPY_DEBUG_CODEGENERATION
            std::cout << "    pointer single-field" << std::endl;
#endif
        }
        else
        {
            setAccessMethod  = "unsupported";
            editAccessMethod = "changePointerMField";
            getAccessMethod  = "getPointerMField";
#ifdef OSGPY_DEBUG_CODEGENERATION
            std::cout << "    pointer multi-field" << std::endl;
#endif
        }
    }
    break;
    default:
    {
        std::string tmp("unknown");
        if(classType == FieldType::ChildPtrField)
        {
            tmp = "ChildPtrField";
        }
        else if (classType == FieldType::ParentPtrField)
        {
            tmp = "ParentPtrField";
        }

        if(desc->isSField())
        {
            std::cerr << "    UNSUPPORTED single-field type: " << tmp << std::endl;
        }
        else
        {
            std::cerr << "    UNSUPPORTED multi-field type: "  << tmp << std::endl;

        }

        assert(false);
    }
    }

    std::string pyCode;

    if(setAccessMethod != "unsupported")
    {
        setAccess = true;

        const std::string setMethod = "def _set_" + fieldName + "(self, value):\n"
                "   _fieldAccessHandler." + setAccessMethod + "('" + fieldName + "', value)\n";
        pyCode += setMethod;

#ifdef OSGPY_DEBUG_CODEGENERATION
        std::cout << "    'set'  support: yes" << std::endl;
#endif
    }
#ifdef OSGPY_DEBUG_CODEGENERATION
    else
    {
        std::cout << "    'set'  support: no" << std::endl;
    }
#endif
    if(editAccessMethod != "unsupported")
    {
        editAccess = true;

        const std::string editMethod = "def _edit_" + fieldName + "(self):\n"
                "   return _fieldAccessHandler." + editAccessMethod + "('" + fieldName + "')\n";
        pyCode += editMethod;
#ifdef OSGPY_DEBUG_CODEGENERATION
        std::cout << "    'edit' support: yes" << std::endl;
#endif
    }
#ifdef OSGPY_DEBUG_CODEGENERATION
    else
    {
        std::cout << "    'edit' support: no" << std::endl;
    }
#endif
    if(getAccessMethod != "unsupported")
    {
        getAccess = true;

        const std::string getMethod = "def _get_" + fieldName + "(self):\n"
                "   return _fieldAccessHandler." + getAccessMethod + "('" + fieldName + "')\n";
        pyCode += getMethod;
#ifdef OSGPY_DEBUG_CODEGENERATION
        std::cout << "    'get'  support: yes" << std::endl;
#endif
    }
#ifdef OSGPY_DEBUG_CODEGENERATION
    else
    {
        std::cout << "    'get'  support: no" << std::endl;
    }


    std::cout << std::endl << "Generated Python glue code:" << std::endl;
    std::cout << pyCode << std::endl;
#endif

    return(_pPyInterpreter->run(pyCode));
}

/*\brief Convenience method to remove a "SF" or "MF" prefix from a     */
/*       given string.                                                 */
void PyFieldAccessHandler::removeFieldQualifier(std::string& str)
{
    if(boost::starts_with(str, "SF") ||
       boost::starts_with(str, "MF"))
    {
        str.erase(0,2);
    }
}

/*-------------------------------------------------------------------------*/
/*                                  Helper                                 */

/*\brief Convenience method to remove a "Rec"/"Unrec"/"Weak" prefix    */
/*       and a "Ptr" suffix from a given string.                       */
void PyFieldAccessHandler::removeRefCountQualifier(std::string& str)
{
    if(boost::ends_with(str, "Ptr"))
    {
        str.erase(str.length()-3, str.length());
    }

    if(boost::starts_with(str, "Rec"))
    {
            str.erase(0,3);
    }
    else if(boost::starts_with(str, "Unrec"))
    {
            str.erase(0,5);
    }
    else if(boost::starts_with(str, "Weak"))
    {
            str.erase(0,4);
    }
}

OSG_END_NAMESPACE
