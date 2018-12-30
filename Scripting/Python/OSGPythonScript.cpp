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

#include "OSGAddOnsConfig.h"

#include "OSGPythonScript.h"

#include "OSGPyFieldAccessHandler.h"
#include "OSGNameAttachment.h"
#include "OSGLog.h"

#include <boost/python.hpp>

OSG_USING_NAMESPACE

// Documentation for this class is emited in the
// OSGPythonScriptBase.cpp file.
// To modify it, please change the .fcd file (OSGPythonScript.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void PythonScript::initMethod(InitPhase ePhase)
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
/*                               Sync                                      */

void PythonScript::changed(ConstFieldMaskArg whichField,
                           UInt32            origin,
                           BitVector         details)
{
    Inherited::changed(whichField, origin, details);

    // Applies in init and shutdown phase.
    if(_pPyInterpreter == NULL) { return; }

    if(whichField & ScriptFieldMask)
    {
        // TODO: cleanup interpreter variables and defs. How?
        // _pPyInterpreter->reset();

        if(execScript() == false)
        {
            pyActivate();
            pyDumpError();
            pyDeactivate();
        }

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

void PythonScript::dump(UInt32    uiIndent,
                        const BitVector bvFlags) const
{
    Inherited::dump(uiIndent, bvFlags);
}

/*-------------------------------------------------------------------------*/
/*                         Action Callbacks                                */

/* Initializes the Python environment and calls the init() function of */
/* the Python script. If anything goes wrong during the initialization */
/* the application is aborted.                                         */
bool PythonScript::init(void)
{
    fprintf(stderr, "PythonScript : init\n");

    if(initPython() == false)
    {
        pyActivate();
        pyDumpError();
        pyDeactivate();

        _pPyFieldAccessHandler = NULL;
        delete _pPyInterpreter;

        OSG::osgExit();
        exit(-1);
    };

    // An error when executing the script is no critical error as the script can be
    // fixed and reexecuted. Therefore init() still returns true in this case.
    if(execScript() == false)
    {
        pyActivate();
        pyDumpError("\nError loading the Python script:\n");
        pyDeactivate();
    }
    else
    {
        callInitFunction();
    }

    return true;
}

/* Executes the frame() function defined in the Python script.         */
/* Additionally checks if the script was changed by an external entity */
/* via the setScriptChanged() method. In that case the script is       */
/* reloaded before calling the frame() function.                       */
/*                                                                     */
/* \see setScriptChanged                                               */
void PythonScript::frame(OSG::Time timeStamp, OSG::UInt32 frameCount)
{
    // Allowing external threads to trigger an reexecution of the changed
    // script:
    if(_bExternalScriptChanged == true)
    {
        fprintf(stderr, "PythonScript : triggered external script change\n");
        changed(ScriptFieldMask, 0, 0);
        _bExternalScriptChanged = false;
    }

    callFrameFunction(timeStamp, frameCount);
}

/*\brief Tears down the PythonScript core and shuts down the Python    */
/*       interpreter.                                                  */
void PythonScript::shutdown(void)
{
    fprintf(stderr, "PythonScript : shutdown\n");

    // shutdown() is calles twice per core, the first time triggered by
    // ComplexSceneManager::shutdown() -> FrameTaskHandler::shutdown() and then
    // on the destruction of the FrameTaskMixin parent of this FieldContainer
    // in FrameTaskMixin::onDestroyAspect() -> FrameTaskHandler::removeTask().
    // We have to take care of not destructing the core twice:
    if(_pPyInterpreter == NULL)
    {
        fprintf(stderr, "PythonScript :     ... skipped. Already shutdown.\n");
        return;
    }

    callShutdownFunction();

    _pPyFieldAccessHandler = NULL;

    delete _pPyInterpreter;
    _pPyInterpreter = NULL;

    fprintf(stderr, "PythonScript :     ... done.\n");
}

/*-------------------------------------------------------------------------*/
/*                         Dynamic Fields                                  */

/*!\brief Exposes a field that is added to the PythonScript at runtime */
/*        to Python. Initial fields read from the CSM configuration    */
/*        are exposed in the initPython() method.                      */
/*                                                                     */
/* \return Id of the added field                                       */
/* \see    initPython                                                  */
UInt32 PythonScript::addField(const UInt32  uiFieldTypeId,
                              const Char8  *szFieldName  )
{
    UInt32 returnValue = Inherited::addField(uiFieldTypeId, szFieldName);

    if(_pPyInterpreter != NULL)
    {
        _pPyFieldAccessHandler->exposeFieldToPython(szFieldName, returnValue);

#ifdef OSGPY_DEBUG
        std::cout << "[PythonScript::addField] add dynamic property '"
                  << szFieldName << "' to Python object" << std::endl;
#endif
    }

    return returnValue;
}

/*!\brief Exposes a field that is added to the PythonScript at runtime */
/*        to Python. Initial fields read from the CSM configuration    */
/*        are exposed in the initPython() method.                      */
/*                                                                     */
/* \return Id of the added field                                       */
/* \see    initPython                                                  */
UInt32 PythonScript::addField(const Char8  *szFieldType,
                              const Char8  *szFieldName)
{
    UInt32 returnValue = Inherited::addField(szFieldType, szFieldName);

    // The initial dynamic fields are exposed in setupPython(). Only fields
    // at runtime are handled here:
    if(_pPyInterpreter != NULL)
    {
        _pPyFieldAccessHandler->exposeFieldToPython(szFieldName, returnValue);

#ifdef OSGPY_DEBUG
        std::cout << "[PythonScript::addField] add dynamic property '"
                  << szFieldName << "' to Python object" << std::endl;
#endif
    }

    return returnValue;
}

/*-------------------------------------------------------------------------*\
 -  protected                                                              -
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                            Constructors                                 */

PythonScript::PythonScript(void ) :
    Inherited              (     ),
    _pPyInterpreter        (NULL ),
    _pPyFieldAccessHandler (NULL ),
    _pyInitFunc            (     ),
    _pyShutdownFunc        (     ),
    _pyFrameFunc           (     ),
    _pyChangedFunc         (     ),
    _bExternalScriptChanged(false)
{
}

// Creates a new, unitialized object at the moment
PythonScript::PythonScript(const PythonScript &source) :
    Inherited              (source),
    _pPyInterpreter        (NULL  ),
    _pPyFieldAccessHandler (NULL  ),
    _pyInitFunc            (      ),
    _pyShutdownFunc        (      ),
    _pyFrameFunc           (      ),
    _pyChangedFunc         (      ),
    _bExternalScriptChanged(false )
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
/*                                Type                                     */

PythonScript::TypeObject       &PythonScript::getFinalType(void)
{
    return _type;
}

const PythonScript::TypeObject &PythonScript::getFinalType(void) const
{
    return _type;
}

/*-------------------------------------------------------------------------*\
 -  private                                                                -
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                            Python Related                               */

/*!\brief Sets up the Python environment, imports the osg2.osg         */
/*        module as osg, loads the global functions and makes the      */
/*        PythonScript core available in Python as 'self', as well as  */
/*        the PyFielDAccessHandler as '_fieldAccessHandler'.           */
bool PythonScript::initPython(void)
{
    _pPyInterpreter = new PyInterpreter;

    // pyActivate?

    if(_pPyInterpreter->run("import osg2.osg as osg") == false)
    {
        FFATAL(("PythonScript: Cannot load module osg2.osg. Ensure that "
                "the module is added to your PYTHONPATH environment "
                "variable!\n"));
        return false;
    }
    _pPyInterpreter->addGlobalVariable<PythonScriptRecPtr>(this, "self");

    _pPyFieldAccessHandler = PyFieldAccessHandler::create();
    if(_pPyFieldAccessHandler->init(this) == false)
    {
        FFATAL(("PythonScript: Error initializing the PyFieldHandler\n"));
        return false;
    }
    else
    {
        // Increases the refcount of the _pPyFieldAccessHandler
        _pPyInterpreter->addGlobalVariable<PyFieldAccessHandlerTransitPtr>
                (PyFieldAccessHandlerTransitPtr(_pPyFieldAccessHandler),
                 "_fieldAccessHandler");
    }

    return true;
}

/*!\brief Executes the script stored in the "script" field and stores  */
/*        the "init()", "frame()", "onChange()" and "shutdown()"       */
/*        functions as bp::objects for fast access. Before execution   */
/*        of the script an eventual Python error is cleared.           */
/*                                                                     */
/* \see pyCheckForError, pyClearError                                  */
bool PythonScript::execScript()
{
    pyActivate();

    if(pyCheckForError() == true)
    {
        pyClearError();
    }

    bool flag = _pPyInterpreter->run(getScript());

    if(flag == true)
    {
        // The script is expected to contain the following functions:
        //   - init()
        //   - shutdown()
        //   - frame()
        //   - changed()
        _pyInitFunc     = _pPyInterpreter->bindFunction("init"    );
        _pyShutdownFunc = _pPyInterpreter->bindFunction("shutdown");
        _pyFrameFunc    = _pPyInterpreter->bindFunction("frame"   );
        _pyChangedFunc  = _pPyInterpreter->bindFunction("changed" );

        // It is no error if one or more of the functions is not present in the
        // script. The corresponding callXXXFunction() members do not execute
        // in that case.

#ifdef OSGPY_DEBUG
    std::cout << "PythonScript: executed script and bound functions" << std::endl;
#endif
    }

    pyDeactivate();

    return flag;
}

/*!\brief Calls the script's init function if no internal Python error */
/*  is set. Otherwise returns immediately                              */
/*                                                                     */
/* \return true if the function was successfully executed, false       */
/*         otherwise                                                   */
bool PythonScript::callInitFunction()
{
    bool flag = false;

    pyActivate();

    if(_pyInitFunc != NULL && !pyCheckForError())
    {
        try
        {
            _pyInitFunc->get()();
        }
        catch(bp::error_already_set &)
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

/*!\brief Calls the script's shutdown function if no internal Python   */
/*  error is set. Otherwise returns immediately.                       */
/*                                                                     */
/* \return true if the function was successfully executed, false       */
/*         otherwise                                                   */
bool PythonScript::callShutdownFunction()
{
    bool flag = false;

    pyActivate();

    if(_pyShutdownFunc != NULL && !pyCheckForError())
    {
        try
        {
            _pyShutdownFunc->get()();
        }
        catch(bp::error_already_set &)
        {
            FWARNING(("PythonScript: Error calling shutdown() function:\n"));
            _pPyInterpreter->dumpError();
            // Do not clear python error here. That is done in compileScript().
        }

        flag = true;
    }

    pyDeactivate();

    return flag;
}

/*!\brief Calls the script's frame function if no internal Python      */
/*  error is set. Otherwise returns immediately.                       */
/*                                                                     */
/* \return true if the function was successfully executed, false       */
/*         otherwise                                                   */
bool PythonScript::callFrameFunction(OSG::Time timeStamp, OSG::UInt32 frameCount)
{
    bool flag = false;

    pyActivate();

    if(_pyFrameFunc != NULL && !pyCheckForError())
    {
        try
        {
            _pyFrameFunc->get()(timeStamp, frameCount);
        }
        catch(bp::error_already_set &)
        {
            FWARNING(("PythonScript: Error calling frame() function:\n"));
            _pPyInterpreter->dumpError();
            // Do not clear python error here. That is done in compileScript().
        }

        flag = true;
    }

    pyDeactivate();

    return flag;
}

/*!\brief Calls the script's changed function if no internal Python    */
/*  error is set. Otherwise returns immediately.                       */
/*                                                                     */
/* \return true if the function was successfully executed, false       */
/*         otherwise                                                   */
bool PythonScript::callChangedFunction(ConstFieldMaskArg whichField,
                                       UInt32            origin,
                                       BitVector         details)
{
    bool flag = false;

    pyActivate();

    if(_pyChangedFunc != NULL && !pyCheckForError())
    {
        try
        {
            _pyChangedFunc->get()(whichField, origin, details);
        }
        catch(bp::error_already_set &)
        {
            FWARNING(("PythonScript: Error calling changed() function:\n"));
            _pPyInterpreter->dumpError();
            // Do not clear python error here. That is done in compileScript().
        }

        flag = true;
    }

    pyDeactivate();

    return flag;
}
