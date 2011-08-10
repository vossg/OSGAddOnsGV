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

#include <boost/python.hpp>

// Temporary includes. Remove later!
//#include "OSGNameAttachment.h"
#include "OSGLog.h"

OSG_USING_NAMESPACE

#define OSGPY_DEBUG

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

namespace
{
    PyThreadState *pMainstate = NULL;

    bool initializPython(void)
    {
        // Only initialize python if it has not been done before, e.g. when
        // the OpenSG bindings are directly called by an python interpreter.
        if(Py_IsInitialized() == false)
        {
            Py_Initialize();
            pMainstate = PyEval_SaveThread();

            fprintf(stderr, "python initialized\n");
        }
        else
        {
            fprintf(stderr, "python already initialized\n");
        }

        return true;
    }

    bool finalizePython(void)
    {
        if(pMainstate != NULL)
        {
            PyEval_RestoreThread(pMainstate);

            Py_Finalize();

            fprintf(stderr, "python finalized\n");
        }

        return true;
    }

    bool registerPython(void)
    {
        addPreFactoryInitFunction (&initializPython);
        addPostFactoryExitFunction(&finalizePython );

        return true;
    }

    OSG::StaticInitFuncWrapper registerOpWrapper(registerPython);

} // namespace


/*-------------------------------------------------------------------------*/
/*                               Sync                                      */

void PythonScript::changed(ConstFieldMaskArg whichField,
                           UInt32            origin,
                           BitVector         details)
{
    Inherited::changed(whichField, origin, details);

    if(whichField & ScriptFieldMask)
    {
        //std::cout << "[PythonScript::changed::ScriptFieldMask]" << std::endl;

        if(_pPyInterpreter != NULL) // true in init phase
        {
            PyEval_RestoreThread(_pPyInterpreter);
            compileScript();
            PyEval_SaveThread();
        }
    }

    if(_pyChangedFunc.isValid())
    {
        PyEval_RestoreThread(_pPyInterpreter);

        try
        {
            _pyChangedFunc.get()(whichField, origin, details);
        }
        catch(bp::error_already_set)
        {
            dumpAndClearError(std::cout);
            FFATAL(("[PythonScript::frame] Error calling frame() function. Aborting...\n"));
            assert(false);
        }

        PyEval_SaveThread();
    }
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
    Inherited       (    ),
    _pPyInterpreter (NULL),
    _uiFCount       (   0),
    _uiFrameFreq    (   1)
{
}

PythonScript::PythonScript(const PythonScript &source) :
    Inherited      (source             ),
    _pPyInterpreter(NULL               ),
    _uiFCount      (source._uiFCount   ),
    _uiFrameFreq   (source._uiFrameFreq)
{
}

/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

PythonScript::~PythonScript(void)
{}

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

    bool flag = setupPython();

    if(_pyInitFunc.isValid())
    {
        PyEval_RestoreThread(_pPyInterpreter);

        try
        {
            _pyInitFunc.get()();
        }
        catch(bp::error_already_set)
        {
            dumpAndClearError(std::cout);
            FFATAL(("[PythonScript::init] Error calling init() function. Aborting...\n"));
            assert(false);
        }

        PyEval_SaveThread();
    }


    return(flag);
}

void PythonScript::frame(OSG::Time timeStamp, OSG::UInt32 frameCount)
{
    ++_uiFCount;

    // TODO: make this a field variable
    //_uiFrameFreq = 200;

    if(_pPyInterpreter != NULL)
    {
        PyEval_RestoreThread(_pPyInterpreter);

        if(_uiFCount % _uiFrameFreq == 0)
        {
            if(_pyFrameFunc.isValid())
            {
                try
                {
                    _pyFrameFunc.get()(timeStamp, frameCount);
                }
                catch(bp::error_already_set)
                {
                    dumpAndClearError(std::cout);
                    FFATAL(("[PythonScript::frame] Error calling frame() function. Aborting...\n"));
                    assert(false);
                }
            }
        }

        PyEval_SaveThread();
    }
}

void PythonScript::shutdown(void)
{
    fprintf(stderr, "PythonScript : shutdown\n");

    if(_pPyInterpreter != NULL)
    {
        PyEval_RestoreThread(_pPyInterpreter);

        if(_pyShutdownFunc.isValid())
        {
            try
            {
                _pyShutdownFunc.get()();
            }
            catch(...)
            {
                dumpAndClearError(std::cout);
                FFATAL(("[PythonScript::frame] Error calling frame() function. Aborting...\n"));
                assert(false);
            }
        }

        Py_EndInterpreter(_pPyInterpreter);

        _pPyInterpreter = NULL;
    }
}

bool PythonScript::setupPython(void)
{
    _pPyInterpreter = Py_NewInterpreter();

    PyEval_RestoreThread(_pPyInterpreter);

    bp::object main = bp::import("__main__");
    _pyGlobalDict   = main.attr ("__dict__");

    try
    {
        PyRun_SimpleString("import osg2.osg as osg\n");
    }
    catch(...)
    {
        dumpAndClearError(std::cout);
        FFATAL(("[PythonScript::setupPython] Python bindings are not initialized "
               "correctly. Aborting...\n"));
        assert(false);
    }

    exposeContainerToPython();
    compileScript();

    PyEval_SaveThread();

    return true;
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
    PyRun_SimpleString(addPropFunction.c_str());

    exposeContainer("self");

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

bool PythonScript::exposeContainer(const std::string& varName)
{
    try
    {
        // Py++ generates registration code for the recorded, reference counted
        // pointer of the actual (raw) pointer type, in this case here the
        // PythonScript type. To bring the "this" pointer into the Python world
        // it has therefore to be converted to its RecPtr representation.
        // Otherwise a "No to_python (by-value) converter found for C++ type:
        // OSG::PythonScript" error occurs. This took me some time to figure
        // out...
        // Note: only RecPtr and TransitPtr are supported
        _pyGlobalDict[varName] = PythonScriptRecPtr(this);
    } catch( bp::error_already_set )
    {
        dumpAndClearError(std::cout);
        FFATAL(("[PythonScript::exposeContainer] Python bindings are not initialized "
               "correctly. Aborting...\n"));
        dumpAndClearError(std::cout);
        assert(false);
    }

    return true;
}

void PythonScript::exposeField(const std::string& fieldName,
                               const std::string& propName,
                               OSG::UInt32 fieldId)
{
    std::pair<std::string, std::string> functions =
            generatePythonFieldAccessFunctions(fieldName);

    std::ostringstream os;
    os << "addprop(self, '" << propName         << "', "         << std::endl
                            << functions.first  << ", "          << std::endl
                            << functions.second << ")"           << std::endl
       << "self." << fieldName << "FieldMask = 1 << " << fieldId << std::endl
       << "self." << fieldName << "FieldId   = "      << fieldId << std::endl;

    PyRun_SimpleString(os.str().c_str());

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

    // Python only uses double for floating point values
    if(typeName == "SFReal32")
    {
        typeName = "SFReal64";
    }

    OSG2PyTypeMap::const_iterator it = _osg2pyTypeM.find(typeName);
    if(it!=_osg2pyTypeM.end())
    {
        privTypeInstance = (*it).second;
#ifdef DEBUG
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

    std::string privTypeVar("__" + fieldName);
    std::string privTypeLine(privTypeVar + " = " + privTypeInstance);

    std::string getterFunction(
                "def " + getterName + "(self):\n"
                "   if not hasattr(self, '" + privTypeVar + "'):\n"
                "      self." + privTypeLine + "\n"
                "   return self.getSField('" + fieldName + "', self." + privTypeVar + ")");

    std::string setterFunction(
                "def " + setterName + "(self,value):\n"
                "   return self.setSField('" + fieldName + "', value)");

    PyRun_SimpleString(getterFunction.c_str());
    PyRun_SimpleString(setterFunction.c_str());

    return(std::make_pair(getterName, setterName));
}

void PythonScript::fetchInterpreterError(bp::object &type,
                                         bp::object &value,
                                         bp::object &traceback)
{
    PyObject *t, *v, *b;
    PyErr_Fetch(&t, &v, &b);
    type      = bp::object(bp::handle<>(bp::allow_null(t)));
    value     = bp::object(bp::handle<>(bp::allow_null(v)));
    traceback = bp::object(bp::handle<>(bp::allow_null(b)));
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

void PythonScript::registerTypeMappings()
{
    //OSGPY_REGISTER_MAPPING("SFBool"      , "True"              );
    OSGPY_REGISTER_MAPPING("SFVec2f"      , "osg.Vec2f()"      )
    OSGPY_REGISTER_MAPPING("SFVec3f"      , "osg.Vec3f()"      )
    OSGPY_REGISTER_MAPPING("SFColor3f"    , "osg.Color3f()"    )
    OSGPY_REGISTER_MAPPING("SFColor3ub"   , "osg.Color3ub()"   )
    OSGPY_REGISTER_MAPPING("SFColor4f"    , "osg.Color4f()"    )
    OSGPY_REGISTER_MAPPING("SFColor4ub"   , "osg.Color4ub()"   )
    OSGPY_REGISTER_MAPPING("SFString"     , "'dummystring'"    )
    OSGPY_REGISTER_MAPPING("SFBoxVolume"  , "osg.BoxVolume()"  )
    OSGPY_REGISTER_MAPPING("SFPlane"      , "osg.Plane()"      )
    OSGPY_REGISTER_MAPPING("SFMatrix"     , "osg.Matrix()"     )
    OSGPY_REGISTER_MAPPING("SFReal64"     , "1.1"              ); // for SFDouble in CSM
    OSGPY_REGISTER_MAPPING("SFMatrix4d"   , "osg.Matrix4d()"   )

    // there is no CSM support for following types (or they have other names):
    //SFUInt32
    //SFReal32
    //SFQuaternion
    //SFQuaterniond

    // there are no python bindings for the following types:
    //SFColor3fx
    //SFColor4fx
    //SFTime
    //SFBitVector
    //SFGLenum
    //SFMatrix4fx
    //SFQuaternionfx

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

void PythonScript::compileScript()
{
    try
    {
        bp::exec(getScript().c_str(), _pyGlobalDict, _pyGlobalDict);
    }
    catch(bp::error_already_set)
    {
        dumpAndClearError(std::cout);
        FFATAL(("[PythonScript::compile] Error compiling script. Aborting...\n"));
        assert(false);

        // TODO: do not abort here. the script should be changed from the
        //       application in case of an error and recompiled
    }

    // The script is expected to contain the following functions:
    //   - init()
    //   - shutdown()
    //   - frame()
    //   - changed()
    _pyInitFunc    .bind(_pyGlobalDict, "init");
    _pyShutdownFunc.bind(_pyGlobalDict, "shutdown");
    _pyFrameFunc   .bind(_pyGlobalDict, "frame");
    _pyChangedFunc .bind(_pyGlobalDict, "changed");

#if 0 // Test function calls:
    _pyInitFunc.get()();
    _pyShutdownFunc.get()();
    _pyFrameFunc.get()();
    _pyChangedFunc.get()();
#endif
}

void PythonScript::dumpAndClearError(std::ostream &os)
{
    bp::object type, value, traceback;
    fetchInterpreterError(type, value, traceback);

    os << std::endl;
    os << "--------------------- PYTHON ERROR BEGIN ---------------------" << std::endl << std::endl;

    os << "TYPE:"      << std::endl;
    os << PyString_AsString(PyObject_Str(type.ptr()))       << std::endl << std::endl;

    os << "MESSAGE:"   << std::endl;
    os << PyString_AsString(PyObject_Str(value.ptr()))      << std::endl << std::endl;

    os << "BACKTRACE:" << std::endl;
    os << PyString_AsString(PyObject_Str(traceback.ptr()))  << std::endl << std::endl;

    os << "---------------------- PYTHON ERROR END ----------------------" << std::endl << std::endl;

    PyErr_Print();
    PyErr_Clear();
}
