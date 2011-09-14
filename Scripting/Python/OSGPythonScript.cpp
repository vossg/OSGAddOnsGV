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

#include "OSGPyFieldAccessHandler.h"
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

/*! \brief Registers the given type for the conversion functions between OpenSG
 *         and Python.
 *  \ingroup GrpScripting
 */

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

    // applies in init phase
    if(_pPyInterpreter == NULL) { return; }

    if(whichField & ScriptFieldMask)
    {
        //std::cout << "[PythonScript::changed::ScriptFieldMask]" << std::endl;

        // TODO: cleanup interpreter variables and defs. How?
        // _pPyInterpreter->reset();

        if(execScript() == false)
        {
            pyActivate();
            pyDumpError();
            pyDeactivate();
        }

        // internally checks if execScript() was successfully or not
        callInitFunction();
    }

    callChangedFunction(whichField, origin, details);
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

PythonScript::PythonScript(void ) :
    Inherited             (     ),
    _pPyInterpreter       (NULL ),
    _pPyFieldAccessHandler(NULL ),
    _pyInitFunc           (     ),
    _pyShutdownFunc       (     ),
    _pyFrameFunc          (     ),
    _pyChangedFunc        (     ),
    _bScriptChanged       (false)
{
}

// Creates a new, unitialized object at the moment
PythonScript::PythonScript(const PythonScript &source) :
    Inherited             (source),
    _pPyInterpreter       (NULL  ),
    _pPyFieldAccessHandler(NULL  ),
    _pyInitFunc           (      ),
    _pyShutdownFunc       (      ),
    _pyFrameFunc          (      ),
    _pyChangedFunc        (      ),
    _bScriptChanged       (false )
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

PythonScript::TypeObject       &PythonScript::getFinalType(void)
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
        PyFieldAccessHandler::registerTypeMappings();
    }
}

bool PythonScript::init(void)
{
    fprintf(stderr, "PythonScript : init\n");

    if(initPython() == false)
    {
        pyActivate();
        pyDumpError();
        pyDeactivate();

        //delete _pPyFieldAccessHandler;
        delete _pPyInterpreter;

        OSG::osgExit();
        std::abort();
    };

    std::cout << "PythonScript: after initPython" << std::endl;

    callInitFunction();

    std::cout << "PythonScript: after callInitFunction" << std::endl;

    return true;
}

void PythonScript::frame(OSG::Time timeStamp, OSG::UInt32 frameCount)
{
    // Allowiing external threads to trigger an reexecution of the changed
    // script:
    if(_bScriptChanged == true)
    {
        fprintf(stderr, "PythonScript : triggered external script change\n");
        changed(ScriptFieldMask, 0, 0);
        _bScriptChanged = false;
    }

    callFrameFunction(timeStamp, frameCount);
}

void PythonScript::shutdown(void)
{
    fprintf(stderr, "PythonScript : shutdown\n");

    callShutdownFunction();

    //delete _pPyFieldAccessHandler;
    delete _pPyInterpreter;
}

/*!\brief Sets up the Python environment, loads global functions and   */
/*        and exposes the PythonScript core as Python variable.        */
bool PythonScript::initPython(void)
{
    _pPyInterpreter = new PyInterpreter;
    if(_pPyInterpreter->run("import osg2.osg as osg") == false)
    {
        FFATAL(("PythonScript: Cannot load module osg2.osg. Ensure that "
                "the module is added to your PYTHONPATH environment "
                "variable!\n"));
        return false;
    }
    _pPyInterpreter->addGlobalVariable<PythonScriptRecPtr>(this, "self");

    _pPyFieldAccessHandler = PyFieldAccessHandler::create();
    if(_pPyFieldAccessHandler->init(this, _pPyInterpreter) == false)
    {
        FFATAL(("PythonScript: Error initializing the PyFieldHandler\n"));
        return false;
    }
    _pPyFieldAccessHandler->setupFieldAccess();

    return(execScript());
}

UInt32 PythonScript::addField(const UInt32  uiFieldTypeId,
                              const Char8  *szFieldName  )
{
    UInt32 returnValue = Inherited::addField(uiFieldTypeId, szFieldName);

    // The initial dynamic fields are exposed in setupPython(). Only fields
    // at runtime are handled here:
    if(_pPyInterpreter != NULL)
    {
        _pPyFieldAccessHandler->exposeField(szFieldName, returnValue);

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
        _pPyFieldAccessHandler->exposeField(szFieldName, returnValue);

#ifdef OSGPY_DEBUG
        std::cout << "[PythonScript::addField] add dynamic property '"
                  << szFieldName << "' to Python object" << std::endl;
#endif
    }

    return returnValue;
}

/*!\brief Executes the script in the "script" field and stores the     */
/*        "init()", "frame()", "onChange()" and "shutdown()" functions */
/*        as bp::objects for fast access. Before execution of the      */
/*        script an eventual Python error is cleared.                  */
/*                                                                     */
/* \see pyCheckError, pyClearError                                     */
bool PythonScript::execScript()
{
    pyActivate();

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

        // It is no error if one or more of the functions is not present in the
        // script. The callXXXFunction() members do not execute them in that
        // case.

#ifdef OSGPY_DEBUG
    std::cout << "PythonScript: executed script and bound functions" << std::endl;
#endif
    }

    bool flag = !pyCheckError();

    pyDeactivate();

    return flag;
}

/*!\brief Calls the script's init function and checks for errors.      */
/*                                                                     */
/* \return true if the function was successfully executed, false       */
/*         otherwise                                                   */
bool PythonScript::callInitFunction()
{
    bool flag = false;

    pyActivate();

    if(_pyInitFunc != NULL && !pyCheckError())
    {
        try
        {
            _pyInitFunc->get()();
        }
        catch(bp::error_already_set)
        {
            FWARNING(("PythonScript: Error calling init() function:\n"));
            pyDumpError();
            // Do not clear python error here. That is done in compileScript().
        }

        flag = true;
    }

    pyDeactivate();

    return flag;
}

/*!\brief Calls the script's shutdown function and checks for errors.  */
/*                                                                     */
/* \return true if the function was successfully executed, false       */
/*         otherwise                                                   */
bool PythonScript::callShutdownFunction()
{
    bool flag = false;

    pyActivate();

    if(_pyShutdownFunc != NULL && !pyCheckError())
    {
        try
        {
            _pyShutdownFunc->get()();
        }
        catch(bp::error_already_set)
        {
            FWARNING(("PythonScript: Error calling shutdown() function:\n"));
            _pPyInterpreter->dumpError(std::cout);
            // Do not clear python error here. That is done in compileScript().
        }

        flag = true;
    }

    pyDeactivate();

    return flag;
}

/*!\brief Calls the script's frame function and checks for errors.     */
/*                                                                     */
/* \return true if the function was successfully executed, false       */
/*         otherwise                                                   */
bool PythonScript::callFrameFunction(OSG::Time timeStamp, OSG::UInt32 frameCount)
{
    bool flag = false;

    pyActivate();

    if(_pyFrameFunc != NULL && !pyCheckError())
    {
        try
        {
            _pyFrameFunc->get()(timeStamp, frameCount);
        }
        catch(bp::error_already_set)
        {
            FWARNING(("PythonScript: Error calling frame() function:\n"));
            _pPyInterpreter->dumpError(std::cout);
            // Do not clear python error here. That is done in compileScript().
        }

        flag = true;
    }

    pyDeactivate();

    return flag;
}

/*!\brief Calls the script's change function and checks for errors.    */
/*                                                                     */
/* \return true if the function was successfully executed, false       */
/*         otherwise                                                   */
bool PythonScript::callChangedFunction(ConstFieldMaskArg whichField,
                                       UInt32            origin,
                                       BitVector         details)
{
    bool flag = false;

    pyActivate();

    if(_pyChangedFunc != NULL && !pyCheckError())
    {
        try
        {
            _pyChangedFunc->get()(whichField, origin, details);
        }
        catch(bp::error_already_set)
        {
            FWARNING(("PythonScript: Error calling changed() function:\n"));
            _pPyInterpreter->dumpError(std::cout);
            // Do not clear python error here. That is done in compileScript().
        }

        flag = true;
    }

    pyDeactivate();

    return flag;
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
