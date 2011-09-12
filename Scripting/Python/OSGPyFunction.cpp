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

#include "OSGPyFunction.h"

#include "OSGConfig.h"

#include <boost/version.hpp>
#include <boost/python.hpp>

#include <iostream>

OSG_USING_NAMESPACE

PyFunction::PyFunction()
    : _isValid(false)
{
}

bool PyFunction::bind(const bp::object& dict, std::string funcname)
{
#if BOOST_VERSION >= 104100
    if(bp::extract<bool>(dict.contains(funcname)))
#else
    if(PyDict_Contains(dict.ptr(), bp::object(funcname).ptr()))
#endif
    {
        _func = dict[funcname];
        _isValid = true;

        //std::cout << "[PyFunction] Successfully bound function '" << funcname << "'." << std::endl;
    }
    else
    {
        _isValid = false;

        std::cout << "[PyFunction] Error binding function '" << funcname << "'." << std::endl;
    }

    return _isValid;
}

bp::object& PyFunction::get()
{
    return _func;
}

void PyFunction::invalidate()
{
    _isValid = false;
}
