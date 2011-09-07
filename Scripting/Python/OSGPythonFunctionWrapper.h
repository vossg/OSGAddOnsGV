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

#ifndef _OSGPYTHONFUNCTIONWRAPPER_H_
#define _OSGPYTHONFUNCTIONWRAPPER_H_

#include "OSGScriptingDef.h"

#include <boost/python/object.hpp>

namespace bp = boost::python;

class OSG_SCRIPTING_DLLMAPPING PythonFunctionWrapper
{
public:
    PythonFunctionWrapper();

    /*!\brief Acquires a function from a given boost::python object.       */
    /* \param dict     A boost::python object                              */
    /* \param funcname Name of the desired function                        */
    void bind(const bp::object& dict, std::string funcname);

    /*!\brief Checks if the wrapper contains a valid function.             */
    bool isValid() { return _isValid; };

    /*!\brief Returns the boost::python object containing the function. Be */
    /*        sure to check if the the function is valid before calling    */
    /*        it.                                                          */
    bp::object& get();

    /*!\brief Invalidates the wrapper.                                     */
    void reset();

private:
    bp::object     _func;
    bool           _isValid;

    PyThreadState *_pyInterpreter;
};

#endif // _OSGPYTHONFUNCTIONWRAPPER_H_
