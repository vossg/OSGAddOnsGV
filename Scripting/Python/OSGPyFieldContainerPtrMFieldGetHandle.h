/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

#ifndef _OSGPYFIELDCONTAINERPTRMFIELDGETHANDLE_H_
#define _OSGPYFIELDCONTAINERPTRMFIELDGETHANDLE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGPyFieldContainerPtrMFieldGetHandleBase.h"

OSG_BEGIN_NAMESPACE

/*! \brief PyFieldContainerPtrMFieldGetHandle class. See \ref
           PageScriptingPyFieldContainerPtrMFieldGetHandle for a description.
*/

class PythonScript;

class OSG_SCRIPTING_DLLMAPPING PyFieldContainerPtrMFieldGetHandle : 
    public PyFieldContainerPtrMFieldGetHandleBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef PyFieldContainerPtrMFieldGetHandleBase Inherited;
    typedef PyFieldContainerPtrMFieldGetHandle     Self;

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    bool init(PythonScript      *pythonScript,
              const std::string &fieldName);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name            Python Iterator Interface                         */
    /*! \{                                                                 */

    PyFieldContainerPtrMFieldGetHandleTransitPtr iter();
    FieldContainerTransitPtr                     next();
    OSG::UInt32                                  len ();

    FieldContainerTransitPtr getitem(int key);
    void                     setitem(int key,
                                     FieldContainer* value);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in PyFieldContainerPtrMFieldGetHandleBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    PyFieldContainerPtrMFieldGetHandle(void);
    PyFieldContainerPtrMFieldGetHandle(
        const PyFieldContainerPtrMFieldGetHandle &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~PyFieldContainerPtrMFieldGetHandle(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class PyFieldContainerPtrMFieldGetHandleBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const PyFieldContainerPtrMFieldGetHandle &source);

    FieldContainerPtrMFieldBase::GetHandlePtr _pGetMFHandle;
    UInt32                                    _uiIterPos;
};

typedef PyFieldContainerPtrMFieldGetHandle *PyFieldContainerPtrMFieldGetHandleP;

OSG_END_NAMESPACE

#include "OSGPyFieldContainerPtrMFieldGetHandleBase.inl"
#include "OSGPyFieldContainerPtrMFieldGetHandle.inl"

#endif /* _OSGPYFIELDCONTAINERPTRMFIELDGETHANDLE_H_ */
