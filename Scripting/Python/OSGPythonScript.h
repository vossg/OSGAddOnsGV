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

#include "OSGPythonFunctionWrapper.h"

OSG_BEGIN_NAMESPACE

/*! \brief PythonScript is the basic NodeCore for inner nodes in the tree.
    \ingroup GrpSystemNodeCoreGroupsCores
    \ingroup GrpLibOSGSystem
    \includebasedoc
 */

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

    template<class T>
    void     setSField(const std::string& name,
                       const T& value          );
    template<class T>
    const T& getSField(const std::string& name,
                       const T& type           );

    template<class T, Int32 iNamespace>
    void     setSFieldNS(const std::string& name,
                         const T& value          );
    template<class T, Int32 iNamespace>
    const T& getSFieldNS(const std::string& name,
                         const T& type           );

    // TODO: add multi field equivalent. I have to figure out the best way
    //       to access multi fields from python

    virtual UInt32 addField(const UInt32  uiFieldTypeId,
                            const Char8  *szFieldName   );
    virtual UInt32 addField(const Char8  *szFieldType,
                            const Char8  *szFieldName   );

    // TODO: add code for removing a field

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                  Temporary/Testing                           */
    /*! \{                                                                 */

    /*! Temporary flag to allow an external thread (in this case OSGSidebar)
        to trigger a commitChanges() within the OpenSG thread.
     */
    void setScriptChanged() { _bScriptChanged = true; }

    // Temporary test method:
    UInt32 myId();

    typedef std::map<std::string, std::string> OSG2PyTypeMap;

    /*=========================  PROTECTED  ===============================*/

    protected:

    typedef PythonScriptBase Inherited;

    PyThreadState *_pPyInterpreter;

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
    bp::object     _pyGlobalDict;

    // Python script functions:
    PythonFunctionWrapper _pyInitFunc;
    PythonFunctionWrapper _pyShutdownFunc;
    PythonFunctionWrapper _pyFrameFunc;
    PythonFunctionWrapper _pyChangedFunc;

    /*!\brief prohibit default function (move to 'public' if needed)       */
    void operator =(const PythonScript &source);

    UInt32 _uiFCount;
    UInt32 _uiFrameFreq;

    bool   _bScriptChanged;
    bool   _bPyErrorFlag;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Python Related                         */
    /*! \{                                                                 */

    static OSG2PyTypeMap _osg2pyTypeM;

    /*!\brief Sets up the Python environment, loads global functions and   */
    /*        and exposes the PythonScript core as Python variable.        */
    /* \see   exposeContainer                                              */
    bool setupPython(void);

    /*!\brief Exposes the core and its fields to Python                    */
    /* \see   exposeContainer                                              */
    /* \see   exposeField                                                  */
    void exposeContainerToPython(void);

    /*!\brief Makes this PythonScript core available within the Python     */
    /*        interpreter as global variable with the given varName        */
    /* \param varName Name of the variable as which the container is       */
    /*        exposed to  Python                                           */
    bool exposeContainer(const std::string& varName);

    /*!\brief Mapping a field adds a Python property to the to the Python  */
    /*        representation of this core. The field is then accessible    */
    /*        via this property. Besides the FieldMask and FieldId are     */
    /*        exposed, too in the form fieldName+"FieldMask" and           */
    /*        fieldName+"FieldId".                                         */
    /* \param fieldName Name of the field to be exposed                    */
    /* \param propName  Name of the property                               */
    /* \param fieldId   Id of the field                                    */
    void exposeField(const std::string& fieldName,
                     const std::string& propName,
                     OSG::UInt32 fieldId);


    /*!\brief Compiles the script in the "script" field and stores the     */
    /*        "init()", "frame()", "onChange()" and "shutdown()" functions */
    /*        as bp::objects for fast access. A successful execution of    */
    /*        the script clears the internal Python error flag.            */
    /* \see setPyErrorFlag, getPyErrorFlag                                 */
    void compileScript();

    /*!\brief Generates a getter/setter function pair to access the field  */
    /*        \emph{fieldName} from python. The functions are not used     */
    /*        directly but are decorated with a \emph{property}.           */
    /* \param fieldName Name of the field for which the access functions   */
    /*        are generated                                                */
    /* \return A string pair with the names of the setter and getter       */
    /*         python functions                                            */
    /* \see   exposeField                                                  */
    std::pair<std::string, std::string>
            generatePythonFieldAccessFunctions(const std::string &fieldName);

    /*!\brief Dumps the current Python error to the given stream.          */
    /* \param os Stream to output the error to                             */
    void dumpAndClearError(std::ostream& os);

    /*!\brief Fetches the current error from Python and fills the type,    */
    /*        value and traceback (if available) objects.                  */
    /* \param type      Type of the error                                  */
    /* \param value     Error value                                        */
    /* \param traceback Error traceback, if available                      */
    void fetchInterpreterError(bp::object &type,
                               bp::object &value,
                               bp::object &traceback);

    /*!\brief Registers type mappings between OSG field types and Python   */
    /*        instances, necessary for the getter function that retrieves  */
    /*        an OSG field value with an Python property .                 */
    /* \todo  Better explanation!                                          */
    static void registerTypeMappings();

    /*!\brief Sets an internal error flag to indicate an Python error      */
    /*        occured during the compilation of the script or the          */
    /*        execution of the init(), frame(), changed() or shutdown()    */
    /*        funtion. If the flag is set the execution of either of these */
    /*        functions is prevented.                                      */
    /*        The flag is reset if compileScript() is successful.          */
    /* \see getPyErrorFlag, compileScript                                  */
    void setPyErrorFlag();

    /*!\brief Gets the internal Python error flag.                         */
    bool getPyErrorFlag();

    /*!\brief Clears the internal Python error flag.                       */
    bool clearPyErrorFlag();
};

typedef PythonScript *PythonScriptP;

OSG_END_NAMESPACE

#include "OSGPythonScriptBase.inl"
#include "OSGPythonScript.inl"

#endif /* _OSGPYTHONSCRIPT_H_ */
