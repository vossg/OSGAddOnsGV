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

#ifndef _OSGPYINTERPRETER_H_
#define _OSGPYINTERPRETER_H_

#include "OSGConfig.h"
#include "OSGBaseTypes.h"
#include "OSGPyFunction.h"

#include "OSGScriptingDef.h"

#include <boost/python.hpp>

#include <vector>
#include <iostream>

namespace bp = boost::python;

OSG_BEGIN_NAMESPACE

class PyFunction;

/*! \brief Encapsulates a Python interpreter thread and performs the initial
           static initialization.
 */
class OSG_SCRIPTING_DLLMAPPING PyInterpreter
{
public:
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    PyInterpreter();

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    ~PyInterpreter();

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Activation                                 */
    /*! \{                                                                 */

    /*!\brief Activates the Python interpreter. Multiple activations are   */
    /*        supported. However, it is assumed that each activate() is    */
    /*        followed by a corresponding deactivate(). Internally simply  */
    /*        counter is incremented each time activate() is called. If    */
    /*        the counter is greater than 1 when deactivate() is called    */
    /*        the Python thread is not deactivated.                        */
    /*                                                                     */
    /* \return True on success, false otherwise                            */
    bool activate()
    {
        if(!_pPyInterpreter)
        {
            return false;
        }

        if(!_isActive)
        {
            PyEval_RestoreThread(_pPyInterpreter);
            _isActive = true;
        }

        ++_refCount;

        return true;
    }

    /*!\brief Deactivates the Python interpreter. It checks if the     */
    /*        the interpreter was activated multiple times. In that    */
    /*        case the Python thread is only deactivated if the        */
    /*        internal activation counter equals 1.                    */
    /*                                                                 */
    /* \see activate                                                   */
    void deactivate()
    {
        if(--_refCount == 0)
        {
            PyEval_SaveThread();
            _isActive = false;
        }
    }

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                Interpreter Interface                         */
    /*! \{                                                                 */

    bool run(const std::string& cmd);

    template<class T>
    bool addGlobalVariable(const T& object, const std::string& name);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                  Error Interface                             */
    /*! \{                                                                 */

    /*!\brief Checks for an interpreter error. The interpreter has to be   */
    /*        activated to run the method.                                 */
    /*                                                                     */
    /* \return true in case of an interpreter error, false otherwise       */
    bool checkForError() { return (PyErr_Occurred()!=NULL); }

    /*!\brief Clears an interpreter error. The interpreter has to be       */
    /*        activated to run the method.                                 */
    void clearError() { PyErr_Clear(); }

    void dumpError();

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Function Store                               */
    /*! \{                                                                 */

    PyFunction* bindFunction      (const std::string& name);
    void        clearFunctionStore();

    /*! \}                                                                 */

private:
    typedef std::vector<PyFunction*> FunctionStore;
    typedef FunctionStore::iterator  FunctionStoreIter;

    PyThreadState *_pPyInterpreter;
    bp::object     _pyGlobalDict;

    FunctionStore  _funcStore;

    bool           _isActive;
    UInt32         _refCount;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Helper                                  */
    /*! \{                                                                 */

    bool containsFunction(bp::object &dict,
                          const std::string& name);

#if 0
    void fetchError(std::string &errorMessage,
                    long        &lineNo,
                    std::string &funcName);
#endif
};

OSG_END_NAMESPACE

#include "OSGPyInterpreter.inl"

#endif // _OSGPYINTERPRETER_H_
