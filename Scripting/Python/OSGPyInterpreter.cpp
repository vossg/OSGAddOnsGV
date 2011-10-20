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

#include "OSGPyInterpreter.h"
#include "OSGBaseInitFunctions.h"

#include <boost/version.hpp>

OSG_USING_NAMESPACE

// Static Python setup:
namespace
{
    PyThreadState *pMainstate = NULL;

    bool initializePython(void)
    {
        // Only initialize python if it has not been done before, e.g. when
        // the OpenSG bindings are directly called by an python interpreter.
        if(Py_IsInitialized() == false)
        {
            Py_Initialize();
            pMainstate = PyEval_SaveThread();

            fprintf(stderr, "python initialized\n");
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
        addPreFactoryInitFunction (&initializePython);
        addPostFactoryExitFunction(&finalizePython );

        return true;
    }

    OSG::StaticInitFuncWrapper registerOpWrapper(registerPython);

} // namespace

/*-------------------------------------------------------------------------*\
 -  public                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

PyInterpreter::PyInterpreter()
    : _pPyInterpreter(Py_NewInterpreter())
    , _funcStore     (                   )
    , _isActive      (false              )
    , _refCount      (0                  )
{
    _pyGlobalDict = bp::import("__main__").attr("__dict__");
}

PyInterpreter::~PyInterpreter()
{
    _funcStore.clear();

    activate();
    Py_EndInterpreter(_pPyInterpreter);

    _pPyInterpreter  = NULL;
    _isActive        = false;

    if(--_refCount != 0)
    {
        std::cerr << "WARNING: destorying a PyInterpreter with "
                     "_activationCount != 0!"
                  << std::endl;
    }
}

/*-------------------------------------------------------------------------*/
/*                         Interpreter Interface                           */

/*!\brief  Runs a python command. The interpreter has to be activated  */
/*         to run the method.                                          */
/* \return true if no Python error happended, false otherwise.         */
bool PyInterpreter::run(const std::string& cmd)
{
    bool flag = true;

    try
    {
        bp::exec(cmd.c_str(), _pyGlobalDict, _pyGlobalDict);
    }
    catch(bp::error_already_set)
    {
        flag = false;
    }

    return flag;
}

/*-------------------------------------------------------------------------*/
/*                             Error Interface                             */


/*!\brief Dumps the current Python error.                              */
/*                                                                     */
/* \todo  Dump the error to a given stream.                            */
void PyInterpreter::dumpError()
{
    if(checkForError() == true) // ensure that an error exists, otherwise
                                // PyErr_Print() causes a fatal error
    {
        PyErr_Print();
    }
    else
    {
        std::cout << "No python error." << std::endl;
    }
}

/*-------------------------------------------------------------------------*/
/*                           Function Store                                */


/*\brief Internally binds a function from  the global namespace to a   */
/*       PyFunction instance so that it can be called later without    */
/*       having to reinterpret the string.                             */
/*                                                                     */
/*\params name Name of the function to bind                            */
/*                                                                     */
/*\return If the function is found a pointer to a PythonFunction       */
/*        instance is returned. NULL is returned in an error case      */
/*        (e.g. the function does not exist in the global namespace).  */
PyFunction* PyInterpreter::bindFunction(const std::string& name)
{
    if(containsFunction(_pyGlobalDict, name))
    {
        PyFunction *f = new PyFunction();
        f->bind(_pyGlobalDict,name);

        _funcStore.push_back(f);

        return f;
    }

    return NULL;
}

/* Removes all stored PyFunctionWrapper instances.                           */
void PyInterpreter::clearFunctionStore()
{
    FunctionStoreIter fIter = _funcStore.begin();
    FunctionStoreIter fEnd  = _funcStore.end();
    for(;fIter!=fEnd;++fIter)
    {
        delete (*fIter);
    }
    _funcStore.clear();
}

/*!\brief Checks if a dict contains the given function.                */
/*                                                                     */
/* \params dict Python dictionary to check                             */
/* \params name Name of the function to check for                      */
/*                                                                     */
/* \returns true if the function is found in the dict, false otherwise */
bool PyInterpreter::containsFunction(bp::object &dict,
                      const std::string& name)
{
    return(bp::extract<bool>(dict.contains(name)));
}

#if 0 // needs a cleaner implementation!
/*!\brief Fetches the current interpreter error and fills the type,    */
/*        value and traceback (if available) objects.                  */
/*                                                                     */
/* \param errorMessage Error string                                    */
/* \param lineNo       Line number of error                            */
/* \param funcName     Name of the function the error appeared in      */
void PyInterpreter::fetchError(std::string &errorType,
                               long        &lineNo,
                               std::string &funcName)
{
    PyObject *type(NULL), *value, *traceback;
    PyErr_Fetch(&type, &value, &traceback);

    // A NULL type means that there is no available Python exception
    if (!type) return;

    try
    {
    bp::object otype(bp::handle<>(bp::allow_null(type)));
    errorType = bp::extract<std::string>(otype);

    bp::object ovalue(bp::handle<>(bp::allow_null(value)));
    std::string errorValue = bp::extract<std::string>(ovalue);

    bp::object otraceback(bp::handle<>(bp::allow_null(traceback)));
    lineNo = bp::extract<long> (otraceback.attr("tb_lineno"));
    //std::string filename = bp::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
    funcName = bp::extract<std::string>(otraceback.attr("tb_frame").attr("f_code").attr("co_name"));

    }
    catch(bp::error_already_set)
    {
        std::cerr << "Error retrieving error" << std::endl;
    }
}
#endif
