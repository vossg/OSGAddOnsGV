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

#ifndef _OSGPYFUNCTION_H_
#define _OSGPYFUNCTION_H_

#include "OSGAddOnsConfig.h"
#include "OSGScriptingDef.h"

#include <boost/python/object.hpp>

namespace bp = boost::python;

OSG_BEGIN_NAMESPACE

class OSG_SCRIPTING_DLLMAPPING PyFunction
{
public:
    PyFunction(void);

    bool        bind(const bp::object& dict, std::string funcname);
    bp::object& get();

    /*!\brief Checks if the wrapper contains a valid function.         */
    bool isValid   () { return _isValid; }
    void invalidate();

private:
    bp::object _func;
    bool       _isValid;
};

OSG_END_NAMESPACE

#endif // _OSGPYFUNCTION_H_
