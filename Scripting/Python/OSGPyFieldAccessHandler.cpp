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

OSG_BEGIN_NAMESPACE

PyFieldAccessHandler::OSG2PyTypeMap PyFieldAccessHandler::_osg2pyTypeMap;

#define OSGPY_REGISTER_MAPPING(FieldT, PyInstance) \
    _osg2pyTypeMap.insert(std::make_pair(FieldT,PyInstance));


bool PyFieldAccessHandler::init(PythonScriptWeakPtr pythonScript,
                                PyInterpreter      *inter)
{
    _pPythonScript  = pythonScript;
    _pPyInterpreter = inter;

    _pPyInterpreter->addGlobalVariable<PyFieldAccessHandlerRecPtr>(this, "_fieldAccessHandler");

    const std::string addMethod(
                "def addmethod(inst, name, func):\n"
                "   cls = type(inst)\n"
                "   if not hasattr(inst.__class__, '__perinstance'):\n"
                "       inst.__class__ = type(cls.__name__, (cls,), {})\n"
                "       inst.__class__.__perinstance = True\n"
                "   setattr(inst.__class__, name, func)\n");

    return(_pPyInterpreter->run(addMethod));
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

        if(desc->isDynamic() == true)
        {
            if(exposeField(desc->getName(), desc->getFieldId()) == false)
            {
                return false;
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
                                       OSG::UInt32 fieldId)
{
    generateFieldAccessCode(fieldName);

    // add get<field>(), edit<field>() and set<field>() methods:
    std::ostringstream os;
    os << "addmethod(self, 'get_"  << fieldName << "', _get_"   << fieldName  << ")" << std::endl
       << "addmethod(self, 'edit_" << fieldName << "', _edit_"  << fieldName  << ")" << std::endl
       << "addmethod(self, 'set_"  << fieldName << "', _set_"   << fieldName  << ")" << std::endl
       << "self." << fieldName << "FieldMask = 1 << " << fieldId << std::endl
       << "self." << fieldName << "FieldId   = "      << fieldId << std::endl;


#ifdef OSGPY_DEBUG_CODEGENERATION
    //std::cout << "Generated python code for field '" << fieldName << "'" << std::endl;
    std::cout << os.str() << std::endl;
#endif

#ifdef OSGPY_DEBUG
    std::cout << "[PythonScript::exposeField] exposed field '"    << fieldName
              << "' as property '" << propName << "'. FieldId = " << fieldId
              << std::endl;
#endif

    return(_pPyInterpreter->run(os.str()));
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
    OSGPY_REGISTER_MAPPING("SFNode"       , "osg.Node().create()") // -> SFNode

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

void PyFieldAccessHandler::generateFieldAccessCode(const std::string& fieldName)
{
    // Example python code for a 'SFVec3f myField' entry in scene.osg's
    // PythonScript section:
    //
    // def _get_myField(self):                               -> non-public access method name
    //     if not hasattr(self, '__myField'):                -> check if template deduction hint parameter already exists
    //         self.__myField = osg.Vec3f()                  -> create it once if not
    //     return self.getSField('myField', self.__myField); -> call the getSField() method with field name
    //                                                          and the necessary template deduction hint parameter

    std::string pyTemplateTypeClass;
    std::string fieldTypeName(_pPythonScript->getType()
                              .getFieldDesc(fieldName.c_str())->getFieldType()
                              .getName());

    OSG2PyTypeMap::const_iterator it = _osg2pyTypeMap.find(fieldTypeName);
    if(it!=_osg2pyTypeMap.end())
    {
        pyTemplateTypeClass = (*it).second;
#ifdef OSGPY_DEBUG
        std::cout << "PyFieldAccessHandler: fieldName: " << fieldName << "  pyTemplateTypeClass: "
                  << pyTemplateTypeClass << std::endl;
#endif
    }
    else
    {
        std::ostringstream os;
        os << "PyFieldAccessHandler: Encountered unknown type: '"
           << fieldTypeName << "' for field '" << fieldName << "'."
           << "No access to this field will be possible via Python."
           << std::endl;
        FWARNING((os.str().c_str()));
    }

    const std::string pyTypeVar          ("__" + fieldName);
    const std::string pyTypeVarAssignCode(pyTypeVar + " = " + pyTemplateTypeClass);

    const std::string getMethodName("_get_" + fieldName);
    const std::string getMethod = "def " + getMethodName + "(self):\n"
            "   if not hasattr(self, '" + pyTypeVar + "'):\n"
            "      self." + pyTypeVarAssignCode + "\n"
            "   return self.getSField('" + fieldName + "', self." + pyTypeVar + ")\n";

    const std::string editMethodName("_edit_" + fieldName);
    const std::string editMethod = "def " + editMethodName + "(self):\n"
            "   if not hasattr(self, '" + pyTypeVar + "'):\n"
            "      self." + pyTypeVarAssignCode + "\n"
            "   return self.myEditSField('" + fieldName + "', self." + pyTypeVar + ")\n";

    const std::string setMethodName("_set_" + fieldName);
    const std::string setMethod = "def " + setMethodName + "(self, value):\n"
            "   return self.setSField('" + fieldName + "', value)\n";

#ifdef OSGPY_DEBUG_CODEGENERATION
    std::cout << "Generated Python code for field '" << fieldName << "'" << std::endl;
    std::cout << getMethod + editMethod + setMethod << std::endl;
#endif

    _pPyInterpreter->run(getMethod + editMethod + setMethod);
}

void PyFieldAccessHandler::listTypes() const
{
    UInt32 size = TypeFactory::the()->getNumTypes();
    int nr = 0;

    for(unsigned int idx=0; idx<size; ++idx)
    {
        TypeBase* type = TypeFactory::the()->findType(idx);
        if(type)
        {
            std::string name = type->getName();

            if(!boost::starts_with(name, "MF") &&
                    !boost::starts_with(name, "SF"))
            {
                std::cout << "Type: " << nr << "  " << name << std::endl;
                ++nr;

                // Open python interpreter, check if osg.name() is valid, add methods in case
            }
        }
    }
}

void PyFieldAccessHandler::generateSupportedFieldsList()
{
    PyThreadState *_pPyInterpreter;

    _pPyInterpreter = Py_NewInterpreter();

    PyEval_RestoreThread(_pPyInterpreter);

    try
    {
        PyRun_SimpleString("import osg2.osg as osg\n");
    }
    catch(...)
    {
        FFATAL(("PyExporter: Python bindings are not initialized "
                "correctly. Aborting with:\n"));
        //dumpAndClearError(std::cout);
        assert(false);
    }

    bp::object _pyGlobalDict = bp::import("__main__").attr("__dict__");

    PyEval_SaveThread();

    Py_EndInterpreter(_pPyInterpreter);
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
    _osg2pyTypeMap.clear();
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
