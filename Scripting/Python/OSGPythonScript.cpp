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

#include <cstdlib>
#include <cstdio>

#include "OSGConfig.h"

#include "OSGRenderAction.h"
#include "OSGIntersectAction.h"

#include "OSGPythonScript.h"

OSG_USING_NAMESPACE

// Documentation for this class is emited in the
// OSGPythonScriptBase.cpp file.
// To modify it, please change the .fcd file (OSGPythonScript.fcd) and
// regenerate the base file.

namespace
{
    PyThreadState *pMainstate = NULL;

    bool initializPython(void)
    {
        Py_Initialize();

        pMainstate = PyEval_SaveThread();

        fprintf(stderr, "python initialized\n");
        
        return true;
    }

    bool finalizePython(void)
    {
        PyEval_RestoreThread(pMainstate);

        Py_Finalize();

        fprintf(stderr, "python finalized\n");
        
        return true;
    }
    
    bool registerPython(void)
    {
        addPreFactoryInitFunction (&initializPython);
        addPostFactoryExitFunction(&finalizePython );

        return true;
    }

    OSG::StaticInitFuncWrapper registerOpWrapper(registerPython);

} // namespace


/*-------------------------------------------------------------------------*/
/*                               Sync                                      */

void PythonScript::changed(ConstFieldMaskArg whichField, 
                           UInt32            origin,
                           BitVector         details)
{
    Inherited::changed(whichField, origin, details);

    fprintf(stderr, "PythonScript::changed : dump\n");

    this->dump(0, 0);
}

/*-------------------------------------------------------------------------*/
/*                               Type                                      */

FieldContainerType &PythonScript::getType(void)
{
    return _localType;
}

const FieldContainerType &PythonScript::getType(void) const
{
    return _localType;
}

/*-------------------------------------------------------------------------*/
/*                               Dump                                      */

void PythonScript::dump(      UInt32    uiIndent, 
                        const BitVector bvFlags) const
{
   Inherited::dump(uiIndent, bvFlags);
}

/*-------------------------------------------------------------------------*/
/*                            Constructors                                 */

PythonScript::PythonScript(void) :
     Inherited     (    ),
    _pPyInterpreter(NULL)
{
}

PythonScript::PythonScript(const PythonScript &source) :
     Inherited     (source),
    _pPyInterpreter(NULL  )
{
}

/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

PythonScript::~PythonScript(void)
{
}

/*-------------------------------------------------------------------------*/
/*                              Render                                     */

PythonScript::TypeObject &PythonScript::getFinalType(void)
{
    return _type;
}

const PythonScript::TypeObject &PythonScript::getFinalType(void) const
{
    return _type;
}

/*-------------------------------------------------------------------------*/
/*                             Intersect                                   */

/*-------------------------------------------------------------------------*/
/*                                Init                                     */

void PythonScript::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);


    if(ePhase == TypeObject::SystemPost)
    {
    }
}

bool PythonScript::init(void)
{
    fprintf(stderr, "PythonScript : init\n");

    _pPyInterpreter = Py_NewInterpreter();

    return true;
}

void PythonScript::frame(OSG::Time, OSG::UInt32)
{
    static UInt32 uiFCount = 0;

    if(uiFCount % 100 == 0)
    {
        fprintf(stderr, "PythonScript : frame %d\n", uiFCount);
    }

    ++uiFCount;

    if(_pPyInterpreter != NULL)
    {
        PyEval_RestoreThread(_pPyInterpreter);
    }
}

void PythonScript::shutdown(void)
{
    fprintf(stderr, "PythonScript : shutdown\n");   

    if(_pPyInterpreter != NULL)
    {
        PyEval_RestoreThread(_pPyInterpreter);
        Py_EndInterpreter   (_pPyInterpreter);

        _pPyInterpreter = NULL;
    }
}
