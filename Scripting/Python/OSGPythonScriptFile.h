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

#ifndef _OSGPYTHONSCRIPTFILE_H_
#define _OSGPYTHONSCRIPTFILE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGPythonScriptFileBase.h"
#include "OSGOSGSceneFileType.h"

OSG_BEGIN_NAMESPACE

/*! \brief A derived version of the PythonScript that provides a scriptUrl
           field to point to a Python script file instead of handing the
           script over as a string.
*/

class OSG_SCRIPTING_DLLMAPPING PythonScriptFile : public PythonScriptFileBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef PythonScriptFileBase Inherited;
    typedef PythonScriptFile     Self;

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
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in PythonScriptFileBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    PythonScriptFile(void);
    PythonScriptFile(const PythonScriptFile &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~PythonScriptFile(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    void postOSGLoading(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    Fileaccess                                */
    /*! \{                                                                 */
    bool readProgram(std::string  &szTarget,
                     const Char8  *szFilename);
    bool readProgram(std::string  &szTarget,
                     std::istream &iStream);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class PythonScriptFileBase;

    friend struct OSGSceneFileType::PostLoadingDispatcher<PythonScriptFile>;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const PythonScriptFile &source);
};

typedef PythonScriptFile *PythonScriptFileP;

OSG_END_NAMESPACE

#include "OSGPythonScriptFileBase.inl"
#include "OSGPythonScriptFile.inl"

#endif /* _OSGPYTHONSCRIPTFILE_H_ */
