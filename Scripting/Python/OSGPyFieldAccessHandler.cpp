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

#include "OSGConfig.h"

#include "OSGPyFieldAccessHandler.h"

#include "OSGTypeFactory.h"

#include <boost/python/object.hpp>
#include <boost/python/import.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>

#define OSGPY_DEBUG_CODEGENERATION

OSG_BEGIN_NAMESPACE

PyFieldAccessHandler::PyFieldTypeMap PyFieldAccessHandler::_pyFieldTypeMap;

#define OSGPY_REGISTER_MAPPING(FieldT, PyInstance) \
    _pyFieldTypeMap.insert(std::make_pair(FieldT,PyInstance));


bool PyFieldAccessHandler::init(PythonScriptWeakPtr pythonScript,
                                PyInterpreter      *inter)
{
    _pPythonScript  = pythonScript;
    _pPyInterpreter = inter;

    // Here the reference count of this PyFieldAccessHandler is increased as
    // the Python bindings use XXXRecPtr when converting the C++ class into an
    // Python object. That prevents the destruction of this class as long as
    // the PyInterpreter is alive.
    _pPyInterpreter->addGlobalVariable<PyFieldAccessHandlerTransitPtr>(PyFieldAccessHandlerTransitPtr(this),
                                                                       "_fieldAccessHandler");

    const std::string addMethod(
                "def addmethod(inst, name, func):\n"
                "   cls = type(inst)\n"
                "   if not hasattr(inst.__class__, '__perinstance'):\n"
                "       inst.__class__ = type(cls.__name__, (cls,), {})\n"
                "       inst.__class__.__perinstance = True\n"
                "   setattr(inst.__class__, name, func)\n");

    return(_pPyInterpreter->run(addMethod));
}

/*!\brief Registers type mappings between OSG field types and Python   */
/*        instances, to generate the field access code later on.       */
void PyFieldAccessHandler::registerTypeMappings()
{                                                                // -> CSM equivalent
    OSGPY_REGISTER_MAPPING("SFInt32"      , "0"                ) // -> SFInt32
    OSGPY_REGISTER_MAPPING("SFReal64"     , "1.1"              ) // -> SFDouble
    OSGPY_REGISTER_MAPPING("SFString"     , "'dummystring'"    ) // -> SFString
    OSGPY_REGISTER_MAPPING("SFBool"       , "1"            ) // -> SFBool

    OSGPY_REGISTER_MAPPING("SFPnt2f"      , "osg.Pnt2f()"      ) // -> SFPnt2f
    OSGPY_REGISTER_MAPPING("SFPnt3f"      , "osg.Pnt3f()"      ) // -> SFPnt3f
    OSGPY_REGISTER_MAPPING("SFPnt4f"      , "osg.Pnt4f()"      ) // -> SFPnt4f
    OSGPY_REGISTER_MAPPING("SFPnt2d"      , "osg.Pnt2d()"      ) // -> SFPnt2d
    OSGPY_REGISTER_MAPPING("SFPnt3d"      , "osg.Pnt3d()"      ) // -> SFPnt3d
    OSGPY_REGISTER_MAPPING("SFPnt4d"      , "osg.Pnt4d()"      ) // -> SFPnt4d

    OSGPY_REGISTER_MAPPING("SFVec2f"      , "osg.Vec2f()"      ) // -> SFVec2f
    OSGPY_REGISTER_MAPPING("SFVec3f"      , "osg.Vec3f()"      ) // -> SFVec3f
    OSGPY_REGISTER_MAPPING("SFVec4f"      , "osg.Vec4f()"      ) // -> SFVec4f
    OSGPY_REGISTER_MAPPING("SFVec2d"      , "osg.Vec2d()"      ) // -> SFVec2d
    OSGPY_REGISTER_MAPPING("SFVec3d"      , "osg.Vec3d()"      ) // -> SFVec3d
    OSGPY_REGISTER_MAPPING("SFVec4d"      , "osg.Vec4d()"      ) // -> SFVec4d

    OSGPY_REGISTER_MAPPING("SFMatrix"     , "osg.Matrix()"     ) // -> SFMatrix
    OSGPY_REGISTER_MAPPING("SFMatrix4d"   , "osg.Matrix4d()"   ) // -> SFMatrix4d

    OSGPY_REGISTER_MAPPING("SFColor3f"    , "osg.Color3f()"    ) // -> SFColor
    OSGPY_REGISTER_MAPPING("SFColor4f"    , "osg.Color4f()"    ) // -> SFColorRGBA

    OSGPY_REGISTER_MAPPING("SFQuaternion" , "osg.Quaternion()" ) // -> SFRotation

    OSGPY_REGISTER_MAPPING("SFBoxVolume"  , "osg.BoxVolume()"  ) // -> SFVolume
    OSGPY_REGISTER_MAPPING("SFPlane"      , "osg.Plane()"      ) // -> SFPlane

    OSGPY_REGISTER_MAPPING("SFTime"       , "1.1"              ) // -> SFTime

    OSGPY_REGISTER_MAPPING("SFImage"      , "osg.Image.create()" ) // -> SFImage
    OSGPY_REGISTER_MAPPING("SFWeakNodePtr", "osg.Node().create()") // -> SFNode

    OSGPY_REGISTER_MAPPING("MFInt32"      , "int(0)")      // -> MFInt32
    OSGPY_REGISTER_MAPPING("MFVec3f"      , "osg.Vec3f()") // -> MFVec3f

#ifdef OSGPY_DEBUG
    OSG2PyTypeMap::const_iterator iter(_osg2pyTypeM.begin());
    OSG2PyTypeMap::const_iterator end (_osg2pyTypeM.end());

    std::cout << "PythonScript: registered type mappings:" << std::endl;
    for(;iter!=end;++iter)
    {
        std::cout << "[" << (*iter).first << " -> " << (*iter).second << "]"
                  << std::endl;
    }
#endif
}

/*!\brief  Generates the Python code that allows field access from     */
/*         within the Python script.                                   */
/* \return true on success, false otherwise. In an error case you can  */
/*         retrieve more information via the interpreter error         */
/*         interface. Clearing the error is in the responsibility of   */
/*         the developer.                                              */
bool PyFieldAccessHandler::setupFieldAccess()
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
            if(exposeField(desc->getName(), desc->getFieldId()) == true)
            {
                std::cout << "Successfully exposed field '" << desc->getName()
                          << "'" << std::endl << std::endl;
            }
            else
            {
                std::cerr << "Error exposing field " << desc->getName()
                          << std::endl;

                _pPyInterpreter->activate();
                _pPyInterpreter->dumpError(std::cerr);
                _pPyInterpreter->deactivate();

                std::cerr << "The field will not be accessible in Python!"
                          << std::endl << std::endl;
            }
        }
    }

    return true;
}

/*!\brief Mapping a field adds a Python property to the to the Python  */
/*        representation of this core. The field is then accessible    */
/*        via this property. Besides the FieldMask and FieldId are     */
/*        exposed, too in the form fieldName+"FieldMask" and           */
/*        fieldName+"FieldId".                                         */
/* \param fieldName Name of the field to be exposed                    */
/* \param propName  Name of the property                               */
/* \param fieldId   Id of the field                                    */
/*                                                                     */
/* \return true on success, false otherwise. In an error case you can  */
/*         retrieve more information via the interpreter error         */
/*         interface. Clearing the error is in the responsibility of   */
/*         the developer.                                              */
bool PyFieldAccessHandler::exposeField(const std::string& fieldName,
                                             OSG::UInt32  fieldId)
{
    bool setAccess (false);
    bool editAccess(false);
    bool getAccess (false);

    if(generateFieldAccessBaseFunctions(fieldName,
                                        setAccess,
                                        editAccess,
                                        getAccess) == false)
    {
        return false;
    };

    std::ostringstream pyCode;
    if(setAccess == true)
    {
        pyCode << "addmethod(self, 'get_"  << fieldName << "', _get_"   << fieldName  << ")" << std::endl;
    }
    if(editAccess == true)
    {
        pyCode << "addmethod(self, 'edit_" << fieldName << "', _edit_"  << fieldName  << ")" << std::endl;
    }
    if(getAccess == true)
    {
       pyCode << "addmethod(self, 'set_"  << fieldName << "', _set_"   << fieldName  << ")" << std::endl;
    }

    pyCode << "self." << fieldName << "FieldMask = 1 << " << fieldId << std::endl
           << "self." << fieldName << "FieldId   = "      << fieldId << std::endl;

#ifdef OSGPY_DEBUG_CODEGENERATION
    std::cout << pyCode.str() << std::endl;
#endif

    return(_pPyInterpreter->run(pyCode.str()));
}

void PyFieldAccessHandler::setSFieldBool(const std::string& name, const bool value)
{
    FieldDescriptionBase *fieldDesc =
            this->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef SField<bool, 2> SFieldT;

    EditFieldHandlePtr editHandle = fieldDesc->editField(*this);
    SFieldT *sfield = static_cast<SFieldT*>(editHandle->getField());
    sfield->setValue(value);

#ifdef DEBUG_FIELDACCESS
    std::cerr << "Updated (id=" << fieldDesc->getTypeId() << "/name='" << name
              << "'/type='" << sfield->getClassType().getName() << "') to " << sfield->getValue() << std::endl;
#endif
}

bool PyFieldAccessHandler::getSFieldBool(const std::string& name, const bool type)
{
    FieldDescriptionBase *fieldDesc = this->getType().getFieldDesc(name.c_str());
    assert(fieldDesc);

    typedef SField<bool, 2> SFieldT;

    GetFieldHandlePtr getHandle = fieldDesc->getField(*this);
    const SFieldT *sfield = static_cast<const SFieldT*>(getHandle->getField());

#ifdef DEBUG_FIELDACCESS
    std::cerr << "getSField: type=" << typeid(type).name() << std::endl;
    std::cerr << "sfield: " << sfield << std::endl;
    std::cerr << "[" << fieldDesc->getFieldType().getName() << "] Retrieved value "
              << sfield->getValue() << " from (id="
              << fieldDesc->getTypeId() << "/name='" << name << "'/type='"
              << sfield->getClassType().getName() << "')" << std::endl;
#endif

    return(sfield->getValue());
}

bool PyFieldAccessHandler::generateFieldAccessBaseFunctions(const std::string& fieldName,
                                                            bool& getAccess,
                                                            bool& editAccess,
                                                            bool& setAccess)
{
    // Example python code for a 'SFVec3f myField' entry in scene.osg's
    // PythonScript section:
    //
    // def _get_myField(self):                               -> non-public access method name
    //     if not hasattr(self, '__myField'):                -> check if template deduction hint parameter already exists
    //         self.__myField = osg.Vec3f()                  -> create it once if not
    //     return self.getSField('myField', self.__myField); -> call the getSField() method with field name
    //                                                          and the necessary template deduction hint parameter

    std::string setAccessMethod ("unsupported");
    std::string editAccessMethod("unsupported");
    std::string getAccessMethod ("unsupported");

    FieldDescriptionBase *desc =
            _pPythonScript->getType().getFieldDesc(fieldName.c_str());
    FieldType::Class classType = desc->getFieldType().getClass();

    switch(classType)
    {
    case FieldType::ValueField:
    {
        if(desc->isSField())
        {
            setAccessMethod  = "setSField";
            editAccessMethod = "myEditSField";
            getAccessMethod  = "getSField";
#ifdef OSGPY_DEBUG_CODEGENERATION
            std::cout << "    VALUE single-field" << std::endl;
#endif
        }
        else
        {
            setAccessMethod  = "setMField";
            editAccessMethod = "unsupported";
            getAccessMethod  = "myGetMField";
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
            setAccessMethod  = "setPointerSField";
            editAccessMethod = "unsupported";
            getAccessMethod  = "getPointerSField";
#ifdef OSGPY_DEBUG_CODEGENERATION
            std::cout << "    pointer single-field" << std::endl;
#endif
        }
        else
        {
            setAccessMethod  = "setPointerMField";
            editAccessMethod = "unsupported";
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
            std::cout << "    UNSUPPORTED single-field type: " << tmp << std::endl;
        }
        else
        {
            std::cout << "    UNSUPPORTED multi-field type: " << tmp << std::endl;

        }
    }
    }

    std::string fieldTypeName(desc->getFieldType().getName());
    std::string pyTemplateTypeClass;

    PyFieldTypeMapConstIterator it = _pyFieldTypeMap.find(fieldTypeName);
    if(it!=_pyFieldTypeMap.end())
    {
        pyTemplateTypeClass = (*it).second;
#ifdef OSGPY_DEBUG_CODEGENERATION
        std::cout << "    Python instance creation code: '" << pyTemplateTypeClass
                  << "'" << std::endl;
#endif
    }
#ifdef OSGPY_DEBUG_CODEGENERATION
    else
    {
        std::cerr << "    Python instance creation: No code registered. Aborting..."
                  << std::endl;
        return false;
    }
#endif

    const std::string pyTypeVar          ("__" + fieldName);
    const std::string pyTypeVarAssignCode(pyTypeVar + " = " + pyTemplateTypeClass);  

    std::string pyCode;

    if(setAccessMethod != "unsupported")
    {
        setAccess = true;

        const std::string setMethodName("_set_" + fieldName);
        if(desc->isSField())
        {
            const std::string setMethod = "def " + setMethodName + "(self, value):\n"
                    "   return _fieldAccessHandler." + setAccessMethod + "('" + fieldName + "', value)\n";
            pyCode += setMethod;
        }
        else
        {
            const std::string setMethod = "def " + setMethodName + "(self, value):\n"
                    "   if not hasattr(self, '" + pyTypeVar + "'):\n"
                    "      self." + pyTypeVarAssignCode + "\n"
                    "   return _fieldAccessHandler." + setAccessMethod + "('" + fieldName + "', value, " + "self." + pyTypeVar + ")\n";
            pyCode += setMethod;
        }

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

        const std::string editMethodName("_edit_" + fieldName);
        const std::string editMethod = "def " + editMethodName + "(self):\n"
                "   if not hasattr(self, '" + pyTypeVar + "'):\n"
                "      self." + pyTypeVarAssignCode + "\n"
                "   return _fieldAccessHandler." + editAccessMethod + "('" + fieldName + "', self." + pyTypeVar + ")\n";
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

        const std::string getMethodName("_get_" + fieldName);
        const std::string getMethod = "def " + getMethodName + "(self):\n"
                "   if not hasattr(self, '" + pyTypeVar + "'):\n"
                "      self." + pyTypeVarAssignCode + "\n"
                "   return _fieldAccessHandler." + getAccessMethod + "('" + fieldName + "', self." + pyTypeVar + ")\n";
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


    std::cout << std::endl << "Generating python glue code:" << std::endl;
    std::cout << pyCode << std::endl;
    std::cout << "Executing code..." << std::endl;
#endif

    return(_pPyInterpreter->run(pyCode));
}

void PyFieldAccessHandler::listSupportedFieldTypes()
{
    std::cout << "----------- Supported field types: -----------" << std::endl;
    PyFieldTypeMapConstIterator iter(_pyFieldTypeMap.begin());
    PyFieldTypeMapConstIterator end (_pyFieldTypeMap.end());
    for(;iter!=end;++iter)
    {
        std::cout << iter->second << std::endl;
    }
}

void PyFieldAccessHandler::generateSupportedFieldsList()
{
    //PyInterpreter inter;

    PyInterpreter &inter = *_pPyInterpreter;
    inter.activate();

//    if(inter.run("import osg2.osg as osg\n"))
//    {
//        std::cerr << "PyFieldAccessHandler: error importing "
//                     "osg2.osg as osg. Aborting..." << std::endl;
//        osgExit();
//        std::abort();
//    }

    bp::object global = bp::import("__main__").attr("__dict__");

    bp::object result   = bp::eval("dir(osg)", global, global);
    bp::list osgClasses = bp::extract<bp::list>(result);

    //std::cout << "Length: " << bp::len(osgClasses) << std::endl;

    std::vector<std::string> classStore;
    for(int idx = 0; idx < bp::len(osgClasses); ++idx)
    {
        classStore.push_back(bp::extract<std::string>(osgClasses[idx]));
        //std::cout << "Extracted    '" << classStore.back() << "'" << std::endl;
    }

    UInt32 size = TypeFactory::the()->getNumTypes();
    int nr = 0;

    for(unsigned int idx = 0; idx < size; ++idx)
    {
        TypeBase* type = TypeFactory::the()->findType(idx);
        if(type)
        {
            std::string name = type->getName();

//            if(!boost::starts_with(name, "MF") ||
//               !boost::starts_with(name, "SF"))
//            {
                std::cout << "processing field type: " << name
                          << " (id: " << nr << ")" << std::endl;
                ++nr;

//                std::string tmp(name);
//                boost::trim_left_if(tmp,boost::is_any_of("MF"));
//                boost::trim_left_if(tmp,boost::is_any_of("SF"));

                std::vector<std::string>::const_iterator iter =
                        std::find(classStore.begin(), classStore.end(), name);
                if(iter != classStore.end())
                {
                    std::cout << "    ... supported" << std::endl;
                    _pyFieldTypeMap.insert(PyFieldTypeMap::value_type(name, "osg." + name + "()"));
                }
                else
                {
                    std::cout << "    ... not supported" << std::endl;
                }
//            }
        }
    }

    //inter.deactivate();
}

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
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

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

    _pyFieldTypeMap.clear();
    _pPythonScript = 0;
}

/*----------------------------- class specific ----------------------------*/

void PyFieldAccessHandler::changed(ConstFieldMaskArg whichField, 
                            UInt32            origin,
                            BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void PyFieldAccessHandler::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump PyFieldAccessHandler NI" << std::endl;
}

OSG_END_NAMESPACE



#if 0 // for lookup, remove later
/*!\brief Generates a getter/setter function pair to access the field  */
/*        \emph{fieldName} from python. The functions are not used     */
/*        directly but are decorated with a \emph{property}.           */
/* \param fieldName Name of the field for which the access functions   */
/*        are generated                                                */
/* \return A string pair with the names of the setter and getter       */
/*         python functions                                            */
/* \see   exposeField                                                  */
std::pair<std::string, std::string>
PythonScript::generatePythonFieldAccessFunctions(const std::string& fieldName)
{
    // getter and setter are private in python per name convention _xxx
    std::string getterName("_get_" + fieldName);
    std::string setterName("_set_" + fieldName);

    std::string typeName(this->getType()
                         .getFieldDesc(fieldName.c_str())->getFieldType()
                         .getName());

    std::string privTypeInstance;

    OSG2PyTypeMap::const_iterator it = _osg2pyTypeM.find(typeName);
    if(it!=_osg2pyTypeM.end())
    {
        privTypeInstance = (*it).second;
#ifdef OSGPY_DEBUG
        std::cout << "[PythonScript::generatePythonFieldAccessFunctions] "
                     " generating getter/setter pair for type: '" << typeName << "'"
                  << " with Python instance '" << privTypeInstance << "'"
                  <<std::endl;
#endif
    }
    else
    {
        std::cout << "[PythonScript::generatePythonFieldAccessFunctions] "
                     "Encountered unknown type: '" << typeName << "' for "
                     "field '" << fieldName << "'" << std::endl;
        assert(false);
    }

#if 0
    std::string privTypeVar("__" + fieldName);
    std::string privTypeLine(privTypeVar + " = " + privTypeInstance);

    std::string getterFunction;
    std::string setterFunction;

    if(typeName == "SFBool")
    {
        getterFunction = "def " + getterName + "(self):\n"
                    "   if not hasattr(self, '" + privTypeVar + "'):\n"
                    "      self." + privTypeLine + "\n"
                    "   return self.getSFieldBool('" + fieldName + "', self." + privTypeVar + ")\n";

        setterFunction = "def " + setterName + "(self,value):\n"
                    "#   if not isinstance(value, int):\n"
                    "#      raise ValueError\n"
                    "   return self.setSFieldBool('" + fieldName + "', value)\n";
    }
    else
    {
        getterFunction = "def " + getterName + "(self):\n"
                    "   if not hasattr(self, '" + privTypeVar + "'):\n"
                    "      self." + privTypeLine + "\n"
                    "   print 'getter for " + fieldName + "'\n"
                    "   return self.getSField('" + fieldName + "', self." + privTypeVar + ")\n";

        setterFunction = "def " + setterName + "(self, value):\n"
                    "#   if not isinstance(value, int):\n"
                    "#      raise ValueError\n"
                    "   print 'setter for " + fieldName + "'\n"
                    "   return self.setSField('" + fieldName + "', value)\n";
    }
#else

    std::string getMethod;
    std::string editMethod;
    std::string setMethod;

    if(typeName == "SFWeakNodePtr")
    {
        std::cout << "SFWeakNodePtr" << std::endl;
        std::string privTypeVar  ("__" + fieldName);
        std::string privTypeLine(privTypeVar + " = " + privTypeInstance);

        std::string getMethodName("_get_" + fieldName);

        getMethod = "def " + getMethodName + "(self):\n"
                "   if not hasattr(self, '" + privTypeVar + "'):\n"
                "      self." + privTypeLine + "\n"
                "   return self.getPointerSField('" + fieldName + "', self." + privTypeVar + ")\n";

        std::string editMethodName("_edit_" + fieldName);
        editMethod = "def " + editMethodName + "(self):\n"
                "   if not hasattr(self, '" + privTypeVar + "'):\n"
                "      self." + privTypeLine + "\n"
                "   return self.myEditSField('" + fieldName + "', self." + privTypeVar + ")\n";

        std::string setMethodName("_set_" + fieldName);
        setMethod = "def " + setMethodName + "(self, value):\n"
                "   return self.setSField('" + fieldName + "', value)\n";
    }
    else
    {
        std::string privTypeVar  ("__" + fieldName);
        std::string privTypeLine(privTypeVar + " = " + privTypeInstance);

        std::string getMethodName("_get_" + fieldName);

        getMethod = "def " + getMethodName + "(self):\n"
                "   if not hasattr(self, '" + privTypeVar + "'):\n"
                "      self." + privTypeLine + "\n"
                "   return self.getSField('" + fieldName + "', self." + privTypeVar + ")\n";

        std::string editMethodName("_edit_" + fieldName);
        editMethod = "def " + editMethodName + "(self):\n"
                "   if not hasattr(self, '" + privTypeVar + "'):\n"
                "      self." + privTypeLine + "\n"
                "   return self.myEditSField('" + fieldName + "', self." + privTypeVar + ")\n";

        std::string setMethodName("_set_" + fieldName);
        setMethod = "def " + setMethodName + "(self, value):\n"
                "   return self.setSField('" + fieldName + "', value)\n";

#ifdef OSGPY_DEBUG_CODEGENERATION
    std::cout << "Generated python code for field '" << fieldName << "'" << std::endl;
    std::cout << getMethod + editMethod + setMethod << std::endl;
#endif
    }

    _pPyInterpreter->run(getMethod + editMethod + setMethod);

    return(std::make_pair(getterName, setterName));
#endif
}
#endif // for lookup, remove later
