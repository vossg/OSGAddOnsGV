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

#ifndef _OSGPYFIELDACCESSHANDLER_H_
#define _OSGPYFIELDACCESSHANDLER_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGConfig.h"
#include "OSGScriptingDef.h"
#include "OSGPythonScript.h"

OSG_BEGIN_NAMESPACE

class PyInterpreter;

class OSG_SCRIPTING_DLLMAPPING PyFieldAccessHandler
{
public:
    PyFieldAccessHandler(PythonScriptWeakPtr  pythonScript,
                         PyInterpreter       *inter);
    ~PyFieldAccessHandler();

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Init                                  */
    /*! \{                                                                 */

    bool init();
    //void reset(); // TODO: how?
    static void registerTypeMappings();

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Field Access                          */
    /*! \{                                                                 */

    bool setupFieldAccess();
    bool exposeField(const std::string& fieldName,
                     OSG::UInt32 fieldId);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                  Code Generation                             */
    /*! \{                                                                 */

    void listTypes() const;
    void generateSupportedFieldsList();

private:
    PyInterpreter       &_pPyInterpreter;
    PythonScriptWeakPtr  _pPythonScript;

    typedef std::map<std::string, std::string> OSG2PyTypeMap;
    static OSG2PyTypeMap _osg2pyTypeMap;

    void generateFieldAccessCode(const std::string& fieldName);
};

OSG_END_NAMESPACE

#endif // _OSGPYFIELDACCESSHANDLER_H_
