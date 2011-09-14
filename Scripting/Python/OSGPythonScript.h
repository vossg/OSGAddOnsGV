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

#include "OSGConfig.h"
#include "OSGPythonScriptBase.h"
#include "OSGAction.h"

#include "OSGNodeFields.h"

#include "OSGPyInterpreter.h"
#include "OSGPyFieldAccessHandler.h"

OSG_BEGIN_NAMESPACE

/*! \brief PythonScript is the basic NodeCore for inner nodes in the tree.
    \ingroup GrpSystemNodeCoreGroupsCores
    \ingroup GrpLibOSGSystem
    \includebasedoc
 */

//class PyFieldAccessHandler;
class PyFunction;

class OSG_SCRIPTING_DLLMAPPING PythonScript : public PythonScriptBase
{
    /*==========================  PUBLIC  =================================*/

  public:

   /*! \}                                                                 */
   /*---------------------------------------------------------------------*/
   /*! \name                  Container Access                            */
   /*! \{                                                                 */

    FieldContainer *findNamedComponent(const Char8 *szName) const;


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
    /*! \name                       Action Callbacks                       */
    /*! \{                                                                 */

    virtual bool init    (void                                       );
    virtual void frame   (OSG::Time timeStamp, OSG::UInt32 frameCount);
    virtual void shutdown(void                                       );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Dump                                  */
    /*! \{                                                                 */

    virtual void dump(      UInt32    uiIndent = 0,
                      const BitVector bvFlags  = 0) const;

    /*! \}                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Field Access                          */
    /*! \{                                                                 */

#if 0
    /*\brief Return the FieldAccessHandler. The method is used by the      */
    /*       Python field access methods registered in                     */
    /*       FieldAccessHandler::exposeField().                            */
    /*                                                                     */
    /*\return FieldAccessHandler                                           */
    PyFieldAccessHandler *getFieldAccessHandler() const
    {
        return _pPyFieldAccessHandler;
    }
#endif

    // CAUTION: editSField, editMField members get excluded by Py++. Fix that
    // later!
    template<class T>
    void     setSField(const std::string& name,
                       const T& value            );
    template<class T>
          T& myEditSField(const std::string& name,
                          const T& type          );
    template<class T>
    const T& getSField(const std::string& name,
                       const T& type             );

    template<class T>
    void     setMField(const std::string& name,
                       const T& value            );
    template<class T>
    const T& getMField(const std::string& name,
                       const T& type             );

#if 0 // TODO
    template<class T>
    void     setPointerSField(const std::string& name,
                              T* value                );
    template<class T>
    const T* getPointerSField(const std::string& name,
                              T* type                 );
#endif

#if 0
    template<class T, Int32 iNamespace>
    void     setSFieldNS(const std::string& name,
                         const T& value          );
    template<class T, Int32 iNamespace>
    const T& getSFieldNS(const std::string& name,
                         const T& type           );
#endif

    void setSFieldBool(const std::string& name,
                       const bool type         );
    bool getSFieldBool(const std::string& name,
                       const bool type         );

    // TODO: add multi field equivalent. I have to figure out the best way
    //       to access multi fields from python

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
     void setScriptChanged() { _bScriptChanged = true; }

    /*! \}                                                                 */

    /*=========================  PROTECTED  ===============================*/

    protected:

    typedef PythonScriptBase Inherited;

    PyInterpreter                *_pPyInterpreter;
    PyFieldAccessHandlerUnrecPtr  _pPyFieldAccessHandler;

    /*---------------------------------------------------------------------*/
    /*! \name                Interpreter Control                           */
    /*! \{                                                                 */

    bool pyActivate  () { return _pPyInterpreter->activate();            }
    bool pyDeactivate() {        _pPyInterpreter->deactivate();          }

    // pyXXX members can only be called in between pyActivate() and
    // pyDeactivate()

    /*!\brief Gets the internal Python error flag.                         */
    bool pyCheckError() { return _pPyInterpreter->checkForError();       }

    /*!\brief Clears the internal Python error flag.                       */
    void pyClearError() {        _pPyInterpreter->clearError();          }

    /*!\brief Prints the error information to std::cerr                    */
    void pyDumpError () {        _pPyInterpreter->dumpError(std::cerr);  }


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

    /*!\brief prohibit default function (move to 'public' if needed)       */
    void operator =(const PythonScript &source);

    bool   _bScriptChanged;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Python Related                         */
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
