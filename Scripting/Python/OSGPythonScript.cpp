/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2002 by the OpenSG Forum                   *
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

#include <cstdlib>
#include <cstdio>

#include "OSGConfig.h"

#include "OSGPythonScript.h"

#include "OSGComplexSceneManager.h"
#include "OSGLog.h"

#include <boost/python.hpp>

OSG_USING_NAMESPACE

//#define OSGPY_DEBUG
//#define OSGPY_DEBUG_CODEGENERATION

// Documentation for this class is emited in the
// OSGPythonScriptBase.cpp file.
// To modify it, please change the .fcd file (OSGPythonScript.fcd) and
// regenerate the base file.

PythonScript::OSG2PyTypeMap PythonScript::_osg2pyTypeM;

/*! \brief Registers the given type for the conversion functions between OpenSG
 *         and Python.
 *  \ingroup GrpScripting
 */
#define OSGPY_REGISTER_MAPPING(FieldT, PyInstance) \
    _osg2pyTypeM.insert(std::make_pair(FieldT,PyInstance));


/*-------------------------------------------------------------------------*/
/*                               Sync                                      */

FieldContainer *PythonScript::findNamedComponent(const Char8 *szName) const
{
    if(ComplexSceneManager::the() != NULL)
    {
        return(ComplexSceneManager::the()->findNamedComponent(szName));
    }

    return NULL;
}

/*-------------------------------------------------------------------------*/
/*                               Sync                                      */

void PythonScript::changed(ConstFieldMaskArg whichField,
                           UInt32            origin,
                           BitVector         details)
{
    Inherited::changed(whichField, origin, details);

    if(_pPyInterpreter == NULL) // applies in init phase
    {
        return;
    }

    pyActivate();

    if(whichField & ScriptFieldMask)
    {
        //std::cout << "[PythonScript::changed::ScriptFieldMask]" << std::endl;

        // TODO: cleanup interpreter variables and defs. How?
        execScript();

        if(_pyInitFunc != NULL && !pyCheckError())
        {
            try
            {
                _pyInitFunc->get()();
            }
            catch(bp::error_already_set)
            {
                FWARNING(("[PythonScript::changed] Error calling init() function:\n"));
                _pPyInterpreter->dumpError(std::cout);
                // Do not clear python error here. That is done in compileScript().
            }
        }
    }

    if(_pyChangedFunc != NULL && !pyCheckError())
    {
        try
        {
            _pyChangedFunc->get()(whichField, origin, details);
        }
        catch(bp::error_already_set)
        {
            FFATAL(("[PythonScript::changed] Error calling changed() function:\n"));
            _pPyInterpreter->dumpError(std::cout);
            // Do not clear python error here. That is done in compileScript().
        }
    }

    pyDeactivate();
}

/*-------------------------------------------------------------------------*/
/*                               Type                                      */

FieldContainerType &PythonScript::getType(void)
{
    return _localType;
}

const FieldContainerType &PythonScript::getType(void) const
{
    return _localType;
}

/*-------------------------------------------------------------------------*/
/*                               Dump                                      */

void PythonScript::dump(      UInt32    uiIndent,
                              const BitVector bvFlags) const
{
    Inherited::dump(uiIndent, bvFlags);
}

/*-------------------------------------------------------------------------*/
/*                            Constructors                                 */

PythonScript::PythonScript(void) :
    Inherited       (     ),
    _pPyInterpreter (NULL ),
    _pyInitFunc     (     ),
    _pyShutdownFunc (     ),
    _pyFrameFunc    (     ),
    _pyChangedFunc  (     ),
    _bScriptChanged (false)
{
}

// Creates a new, unitialized object at the moment
PythonScript::PythonScript(const PythonScript &source) :
    Inherited      (source),
    _pPyInterpreter(NULL  ),
    _pyInitFunc    (      ),
    _pyShutdownFunc(      ),
    _pyFrameFunc   (      ),
    _pyChangedFunc (      ),
    _bScriptChanged(false )
{
}

/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

PythonScript::~PythonScript(void)
{
    fprintf(stderr, "PythonScript : Destructor\n");

    // cleanup is handled in shutdown()
}

/*-------------------------------------------------------------------------*/
/*                              Render                                     */

PythonScript::TypeObject &PythonScript::getFinalType(void)
{
    return _type;
}

const PythonScript::TypeObject &PythonScript::getFinalType(void) const
{
    return _type;
}

/*-------------------------------------------------------------------------*/
/*                             Intersect                                   */

/*-------------------------------------------------------------------------*/
/*                                Init                                     */

void PythonScript::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
        registerTypeMappings();
    }
}

bool PythonScript::init(void)
{
    fprintf(stderr, "PythonScript : init\n");

    initPython();

    callInitFunction();

    return true;
}

void PythonScript::frame(OSG::Time timeStamp, OSG::UInt32 frameCount)
{
    // Temporary code to allow OSGSidebar to change the script from an external
    // thread:
    if(_bScriptChanged == true)
    {
        fprintf(stderr, "PythonScript : triggered external script change\n");
        changed(ScriptFieldMask, 0, 0);
        _bScriptChanged = false;
    }

    if(pyActivate() == true)
    {
        if(_pyFrameFunc != NULL && !pyCheckError())
        {
            try
            {
                _pyFrameFunc->get()(timeStamp, frameCount);
            }
            catch(bp::error_already_set)
            {
                FWARNING(("[PythonScript::frame] Error calling frame() function:\n"));
                _pPyInterpreter->dumpError(std::cout);
                // Do not clear python error here. That is done in compileScript().
            }
        }

        pyDeactivate();
    }
}

void PythonScript::shutdown(void)
{
    fprintf(stderr, "PythonScript : shutdown\n");

    pyActivate();

    if(_pyShutdownFunc != NULL && !pyCheckError())
    {
        try
        {
            _pyShutdownFunc->get()();
        }
        catch(...)
        {
            FWARNING(("PythonScript: Error calling shutdown() function.\n"));
            _pPyInterpreter->dumpError(std::cout);
        }
    }

    pyDeactivate();

    delete _pPyInterpreter;
}

void PythonScript::initPython(void)
{
    _pPyInterpreter = new PyInterpreter;

    if(_pPyInterpreter->run("import osg2.osg as osg") == false)
    {
#ifdef OSGPY_DEBUG
                _pPyInterpreter->dumpAndClearError(std::cerr);
#endif

        FFATAL(("OSGPyInterpreter: Cannot load module osg2.osg. Ensure that "
                "the module is added to your PYTHONPATH environment "
                "variable!\n"));

        OSG::osgExit();
        std::abort();
    }

    exposeContainerToPython();
    execScript();
}

void PythonScript::exposeContainerToPython(void)
{
    // TODO: create separate method for loading helper functions?
    std::string addPropFunction(
                "def addprop(inst, name, getter, setter):\n"
                "   cls = type(inst)\n"
                "   if not hasattr(inst.__class__, '__perinstance'):\n"
                "       inst.__class__ = type(cls.__name__, (cls,), {})\n"
                "       inst.__class__.__perinstance = True\n"
                "   setattr(inst.__class__, name, property(getter, setter))\n");

    std::string addMethod(
                "def addmethod(inst, name, func):\n"
                "   cls = type(inst)\n"
                "   if not hasattr(inst.__class__, '__perinstance'):\n"
                "       inst.__class__ = type(cls.__name__, (cls,), {})\n"
                "       inst.__class__.__perinstance = True\n"
                "   setattr(inst.__class__, name, func)\n");

    _pPyInterpreter->run(addPropFunction + addMethod);

    _pPyInterpreter->addGlobalVariable<PythonScriptRecPtr>(this, "self");

    for(UInt32 idx = 1; idx <= this->getType().getNumFieldDescs(); ++idx)
    {
        FieldDescriptionBase *desc = this->getType().getFieldDesc(idx);
        assert(desc);

        if(desc->isDynamic() == true)
        {
            std::string fieldName(desc->getName());
            std::string propName(fieldName);

            exposeField(fieldName, propName, desc->getFieldId());
        }
    }
}

void PythonScript::exposeField(const std::string& fieldName,
                               const std::string& propName,
                               OSG::UInt32 fieldId)
{
    std::pair<std::string, std::string> functions =
            generatePythonFieldAccessFunctions(fieldName);

    // add get<field>(), edit<field>() and set<field>() methods:
    std::ostringstream os;
    os << "addmethod(self, 'get_"  << fieldName << "', _get_"   << fieldName  << ")" << std::endl
       << "addmethod(self, 'edit_" << fieldName << "', _edit_"  << fieldName  << ")" << std::endl
       << "addmethod(self, 'set_"  << fieldName << "', _set_"   << fieldName  << ")" << std::endl
       << "self." << fieldName << "FieldMask = 1 << " << fieldId << std::endl
       << "self." << fieldName << "FieldId   = "      << fieldId << std::endl;

    _pPyInterpreter->run(os.str());

#ifdef OSGPY_DEBUG_CODEGENERATION
    //std::cout << "Generated python code for field '" << fieldName << "'" << std::endl;
    std::cout << os.str() << std::endl;
#endif

#ifdef OSGPY_DEBUG
    std::cout << "[PythonScript::exposeField] exposed field '"    << fieldName
              << "' as property '" << propName << "'. FieldId = " << fieldId
              << std::endl;
#endif
}

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

UInt32 PythonScript::myId()
{
    return this->getId();
}

UInt32 PythonScript::addField(const UInt32  uiFieldTypeId,
                              const Char8  *szFieldName  )
{
    UInt32 returnValue = Inherited::addField(uiFieldTypeId, szFieldName);

    // The initial dynamic fields are exposed in setupPython(). Only fields
    // at runtime are handled here:
    if(_pPyInterpreter != NULL)
    {
        exposeField(szFieldName, szFieldName, returnValue);

#ifdef OSGPY_DEBUG
        std::cout << "[PythonScript::addField] add dynamic property '"
                  << szFieldName << "' to Python object" << std::endl;
#endif
    }

    return returnValue;
}

UInt32 PythonScript::addField(const Char8  *szFieldType,
                              const Char8  *szFieldName)
{
    UInt32 returnValue = Inherited::addField(szFieldType, szFieldName);

    // The initial dynamic fields are exposed in setupPython(). Only fields
    // at runtime are handled here:
    if(_pPyInterpreter != NULL)
    {
        exposeField(szFieldName, szFieldName, returnValue);

#ifdef OSGPY_DEBUG
        std::cout << "[PythonScript::addField] add dynamic property '"
                  << szFieldName << "' to Python object" << std::endl;
#endif
    }

    return returnValue;
}

void PythonScript::execScript()
{
    if(pyCheckError() == true)
    {
        pyClearError();
    }

    if(_pPyInterpreter->run(getScript()) != false)
    {
        // The script is expected to contain the following functions:
        //   - init()
        //   - shutdown()
        //   - frame()
        //   - changed()
        _pyInitFunc     = _pPyInterpreter->bindFunction("init");
        _pyShutdownFunc = _pPyInterpreter->bindFunction("shutdown");
        _pyFrameFunc    = _pPyInterpreter->bindFunction("frame");
        _pyChangedFunc  = _pPyInterpreter->bindFunction("changed");

#ifdef OSGPY_DEBUG
    std::cout << "PythonScript: compiled script and bound functions" << std::endl;
#endif
    }
    else
    {
        FWARNING(("Python Error compiling script:\n"));
        pyDumpError();
    }
}

void PythonScript::registerTypeMappings()
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

void PythonScript::setSFieldBool(const std::string& name, const bool value)
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

bool PythonScript::getSFieldBool(const std::string& name, const bool type)
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

bool PythonScript::callInitFunction()
{
    bool flag = false;

    if(_pyInitFunc != NULL && !pyCheckError())
    {
        pyActivate();

        try
        {
            _pyInitFunc->get()();
        }
        catch(bp::error_already_set)
        {
            FFATAL(("[PythonScript::init] Error calling init() function:\n"));
            _pPyInterpreter->dumpError(std::cout);
            // Do not clear python error here. That is done in compileScript().
        }

        flag = true;

        pyDeactivate();
    }

    return flag;
}
