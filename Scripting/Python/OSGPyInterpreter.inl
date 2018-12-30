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

OSG_BEGIN_NAMESPACE

#if 0 // multiple definitions!?

/*!\brief  Activates the Python interpreter.                                 */
/*                                                                           */
/* \return True on success, false otherwise                                  */
bool PyInterpreter::activate()
{
    if(!_pPyInterpreter)
    {
        return false;
    }

    PyEval_RestoreThread(_pPyInterpreter);

    return true;
}

/*!\brief Deactivates the Python interpreter.                                */
void PyInterpreter::deactivate()
{
    PyEval_SaveThread();
}

#endif

/*!\brief  Adds an object to the interpreter's global namespace. The object's */
/*         type has to be supported by the interpreter. The object is exposed */
/*         under the given name.                                              */
/*                                                                            */
/*         Sets an interpreter error in case of a failure.                    */
/*                                                                            */
/* \return True on success, false otherwise                                   */
/*                                                                            */
/* \note   For OpenSG types the object has to be a RecPtr or TransitPtr,      */
/*         otherwise the interpreter error "No to_python (by-value) converter */
/*         found for C++ type <TYPE>" is raised.                              */
template<class T>
bool PyInterpreter::addGlobalVariable(const T& object, const std::string& name)
{
    bool flag = true;

    try
    {
        _pyGlobalDict[name] = object;
    }
    catch(bp::error_already_set &)
    {
        flag = false;
    }

    return flag;
}

OSG_END_NAMESPACE


