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

#ifndef _OSGPYTHONSCRIPT_H_
#define _OSGPYTHONSCRIPT_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGAddOnsConfig.h"
#include "OSGPythonScriptBase.h"
#include "OSGAction.h"

//#include "OSGNodeFields.h"
#include "OSGPyFieldAccessHandlerFields.h"

#include "OSGPyInterpreter.h"

OSG_BEGIN_NAMESPACE

/*! \brief PythonScript allows you to load a Python script and control the
    scene graph with it. You can route in data via dynamic fields or access
    components in using the findNamedComponent method.
    \ingroup GrpSystemNodeCoreGroupsCores
    \ingroup GrpLibOSGSystem
    \includebasedoc
 */

class PyFieldAccessHandler;
class PyFunction;

class OSG_SCRIPTING_DLLMAPPING PythonScript : public PythonScriptBase
{
    /*==========================  PUBLIC  =================================*/

public:

    /*---------------------------------------------------------------------*/
    /*! \name                       Sync                                   */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         detail);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Type                                  */
    /*! \{                                                                 */

    virtual       FieldContainerType &getType         (void);
    virtual const FieldContainerType &getType         (void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Dump                                  */
    /*! \{                                                                 */

    virtual void dump(      UInt32    uiIndent = 0,
                            const BitVector bvFlags  = 0) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                  Action Callbacks                            */
    /*! \{                                                                 */

    virtual bool init    (void                                       );
    virtual void frame   (OSG::Time timeStamp, OSG::UInt32 frameCount);
    virtual void shutdown(void                                       );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Dynamic Fields                             */
    /*! \{                                                                 */
    virtual UInt32 addField(const UInt32  uiFieldTypeId,
                            const Char8  *szFieldName   );
    virtual UInt32 addField(const Char8  *szFieldType,
                            const Char8  *szFieldName   );

    // TODO: add code for removing a field

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                  External Access                             */
    /*! \{                                                                 */

    /*!\brief Allows an external (non-OSG) thread to trigger an            */
    /*        of the script (used for the OSGSidebar application).         */
    void setScriptChanged() { _bExternalScriptChanged = true; }

    /*\brief Returns the Python interpreter.                              */
    PyInterpreter* getInterpreter() const { return _pPyInterpreter; }

    /*! \}                                                                 */

    /*=========================  PROTECTED  ===============================*/

protected:

    typedef PythonScriptBase Inherited;

    PyInterpreter                *_pPyInterpreter;
    PyFieldAccessHandlerUnrecPtr  _pPyFieldAccessHandler;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    PythonScript(void);
    PythonScript(const PythonScript &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~PythonScript(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Type                                  */
    /*! \{                                                                 */

    virtual       TypeObject &getFinalType(void);
    virtual const TypeObject &getFinalType(void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Init                                  */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*---------------------------------------------------------------------*/
    /*! \name                        Python                                */
    /*! \{                                                                 */

    bool pyActivate  () { return _pPyInterpreter->activate();                }
    bool pyDeactivate() {        _pPyInterpreter->deactivate(); return true; }

    // pyXXX members can only be called in between pyActivate() and
    // pyDeactivate()

    /*!\brief Returns the internal Python error flag. The interpreter  */
    /*        needs to be active when the method is called.            */
    bool pyCheckForError() { return _pPyInterpreter->checkForError();    }

    /*!\brief Clears the internal Python error flag. The interpreter   */
    /*        needs to be active when the method is called.            */
    void pyClearError() {        _pPyInterpreter->clearError();          }

    /*!\brief Prints the error information to std::cerr.  The          */
    /*        interpreter needs to be active when the method is        */
    /*        called.                                                  */
    void pyDumpError (const std::string& description = "")
    {
        std::cerr << description << std::endl;
        _pPyInterpreter->dumpError();
    }

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

private:

    friend class FieldContainer;
    friend class PythonScriptBase;

    /*---------------------------------------------------------------------*/

    PyFunction *_pyInitFunc;
    PyFunction *_pyShutdownFunc;
    PyFunction *_pyFrameFunc;
    PyFunction *_pyChangedFunc;

    bool        _bExternalScriptChanged;

    /*!\brief prohibit default function (move to 'public' if needed)       */
    void operator =(const PythonScript &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    Python Related                            */
    /*! \{                                                                 */

    bool initPython(void);

    bool execScript();

    // The callXXXFunction() methods can be called anytime. They setup the
    // Python interpreter themselves and only execute if there is no Python
    // error set.

    bool callInitFunction    ();
    bool callShutdownFunction();
    bool callFrameFunction   (OSG::Time timeStamp, OSG::UInt32 frameCount);
    bool callChangedFunction (ConstFieldMaskArg whichField,
                              UInt32            origin,
                              BitVector         details);

    /*! \}                                                                 */
};

typedef PythonScript *PythonScriptP;

OSG_END_NAMESPACE

#include "OSGPythonScriptBase.inl"
#include "OSGPythonScript.inl"

#endif /* _OSGPYTHONSCRIPT_H_ */
