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

#include "OSGAddOnsConfig.h"

#include <boost/version.hpp>
#include <boost/python.hpp>

#include <iostream>

OSG_USING_NAMESPACE

PyFunction::PyFunction(void) : 
    _func   (     ),
    _isValid(false)
{
}

/*!\brief  Acquires a function from a given boost::python object.      */
/*                                                                     */
/* \param  dict     A boost::python object                             */
/* \param  funcname Name of the desired function                       */
/*                                                                     */
/* \return True if successfull, false otherwise. _isValid is set to    */
/*         false if bind is not successfull.                           */
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
    }
    else
    {
        _isValid = false;
        std::cerr << "[PyFunction] Error binding function '" << funcname << "'." << std::endl;
    }

    return _isValid;
}

/*!\brief Returns the boost::python object containing the function.    */
/*        You have to ensure that the function is valid before calling */
/*        it.                                                          */
bp::object& PyFunction::get()
{
    return _func;
}

/*!\brief Invalidates the function.                                    */
void PyFunction::invalidate()
{
    _isValid = false;
}
