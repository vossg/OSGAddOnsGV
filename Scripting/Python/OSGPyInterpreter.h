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
#include "OSGPyFunction.h"

#include "OSGScriptingDef.h"

#include <boost/python.hpp>
#include <vector>

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

    bool activate()
    {
        if(!_pPyInterpreter)
        {
            return false;
        }

        PyEval_RestoreThread(_pPyInterpreter);

        return true;
    }

    void deactivate()
    {
        PyEval_SaveThread();
    }

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                Runtime Interface                             */
    /*! \{                                                                 */

    bool run(const std::string& cmd);

    template<class T>
    bool addGlobalVariable(const T& object, const std::string& name);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Error Interface                              */
    /*! \{                                                                 */

    void dumpAndClearError(std::ostream& os);

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

    bool containsFunction(bp::object &dict,
                          const std::string& name);

    void fetchError(std::string &errorMessage,
                    long        &lineNo,
                    std::string &funcName);
};

OSG_END_NAMESPACE

#include "OSGPyInterpreter.inl"

#endif // _OSGPYINTERPRETER_H_