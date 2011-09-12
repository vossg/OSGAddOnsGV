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
        // else: python is already initialized, e.g. when you load the python
        //       bindings in an interactive python console.

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

PyInterpreter::PyInterpreter()
    : _pPyInterpreter(Py_NewInterpreter())
    , _funcStore     (                   )
{
    _pyGlobalDict = bp::import("__main__").attr("__dict__");
}

PyInterpreter::~PyInterpreter()
{
    _funcStore.clear();

    Py_EndInterpreter(_pPyInterpreter);
    _pPyInterpreter = NULL;
}

/*!\brief  Runs a python command.                                            */
bool PyInterpreter::run(const std::string& cmd)
{
    bool flag = true;

    try
    {
        bp::exec(cmd.c_str(), _pyGlobalDict, _pyGlobalDict);
    }
    catch(...)
    {
        flag = false;
    }

    return flag;
}


/*-------------------------- Error Management -------------------------------*/


/*!\brief Dumps the current Python error to the given stream.                */
/* \param os Stream to output the error to                                   */
void PyInterpreter::dumpError(std::ostream &os)
{
    std::string errorMessage;
    long        lineNr;
    std::string funcName;

    fetchError(errorMessage, lineNr, funcName);

    os << std::endl;
    os << "--------------------- PYTHON ERROR BEGIN ---------------------"
       << std::endl << std::endl;

    os << "Error message:" << std::endl << errorMessage << std::endl
       << std::endl;

    os << "in " << funcName << ":" << lineNr << std::endl << std::endl;

    os << "---------------------- PYTHON ERROR END ----------------------"
       << std::endl << std::endl;

    if(checkError() == true) // ensure that an error exists, otherwise
                             // PyErr_Print() causes a fatal error
    {
        // TODO: how to redirect the output to the ostream?
        std::cerr << "Traceback:" << std::endl;
        PyErr_Print();
    }
}

/*!\brief Fetches the current interpreter error and fills the type,          */
/*        value and traceback (if available) objects.                        */
/*                                                                           */
/* \param errorMessage Error string                                          */
/* \param lineNo       Line number of error                                  */
/* \param funcName     Name of the function the error appeared in            */
void PyInterpreter::fetchError(std::string &errorMessage,
                               long        &lineNo,
                               std::string &funcName)
{
    PyObject *t, *v, *b;
    PyErr_Fetch(&t, &v, &b);
    bp::object type(bp::handle<>(bp::allow_null(t)));
    bp::object value(bp::handle<>(bp::allow_null(v)));

    errorMessage = bp::extract<std::string>(value);

    // Extract line number (top entry of call stack).
    // If you want to extract another levels of call stack
    // also process traceback.attr("tb_next") recurently.
    if(b != NULL)
    {
        bp::object traceback(bp::handle<>(bp::allow_null(b)));

        lineNo = bp::extract<long> (traceback.attr("tb_lineno"));
        //std::string filename = bp::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
        funcName = bp::extract<std::string>(traceback.attr("tb_frame").attr("f_code").attr("co_name"));
    }
    else
    {
        lineNo = -1;
        funcName = "unset";
    }
}


/*----------------------- Function Store ------------------------------*/


/*\brief Internally binds a function in the global namespace to a            */
/*       PyFunctionWrapper instance so that it can be called later           */
/*       without having to reinterpret the function code (which would be the */
/*       case when using run() to execute the function). Unbinding and       */
/*       deletion of the PyFunctionWrapper instances is handled by the       */
/*       PyInterpreter (see unBindFunction() and clearFunctionStore()).      */
/*                                                                           */
/*\params name Name of the function                                          */
/*                                                                           */
/*\return If successfull a pointer to a PythonFunctionWrapper instance is    */
/*        returned. NULL is returned in an error case (e.g. the function     */
/*        does not exist in the global namespace).                           */
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

/*!\brief Checks if a dict contains the given function.                      */
/*                                                                           */
/* \params dict Python dictionary to check                                   */
/* \params name Name of the function to check for                            */
/*                                                                           */
/* \returns true if the function is found in the dict, false otherwise       */
bool PyInterpreter::containsFunction(bp::object &dict,
                      const std::string& name)
{
    return(bp::extract<bool>(dict.contains(name)));
}
