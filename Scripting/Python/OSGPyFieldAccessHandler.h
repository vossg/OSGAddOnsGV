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

#ifndef _OSGPYFIELDACCESSHANDLER_H_
#define _OSGPYFIELDACCESSHANDLER_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGPyFieldAccessHandlerBase.h"

#include "OSGPythonScript.h"
#include "OSGPyFieldContainerPtrMFieldEditHandle.h"
#include "OSGPyFieldContainerPtrMFieldGetHandle.h"

#include "OSGBaseFields.h"
#include "OSGVecFields.h"
#include "OSGQuaternion.h"
#include "OSGMathFields.h"

#include "OSGColor.h"
#include "OSGTime.h"
#include "OSGBoxVolume.h"
#include "OSGPlane.h"

OSG_BEGIN_NAMESPACE

class PyInterpreter;

/*! \brief PyFieldAccessHandler class. See \ref
           PageScriptingPyFieldAccessHandler for a description.

    The PyFieldAccessHandler is responsible for accessing the dynamic fields of
    a PythonScript. It supports access to value and pointer multi/single-fields.
    See the user FAQ for how to use the access methods.

    Each PythonScript has exactly one PyFieldAccessHandler instance that is
    managing the field access. The instance is exported to the Python
    interpreter with the name '_fieldAccessHandler'.
*/

// This macro is used instead of a template method for value single-field
// access. Reason being is that for getValueSField_XXX() lacks a type argument
// to deduce the template.
// As 'Bool' and 'String' fields use integral C++ types the macro uses a
// TypeName parameter denoting the OSG name of the type and an IntegralT
// parameter specifying the actual type (which corresponds to TypeName
// in all cases except for 'Bool' and 'String').
#define OSGPY_GEN_VALUESFIELD_CODE(TypeName, iNameSpace)        \
const TypeName& getValueSField_##TypeName(const std::string& fieldName)         \
{                                                                               \
    typedef SField<TypeName, iNameSpace> FieldT;                               \
                                                                                \
    GetFieldHandlePtr       handle =                                            \
            _pPythonScript->getDynamicFieldByName(fieldName.c_str());           \
    FieldT::GetHandlePtr getHandle =                                            \
            boost::dynamic_pointer_cast<FieldT::GetHandle>(handle);             \
    assert(getHandle);                                                          \
                                                                                \
    return getHandle->getField()->getValue();                                   \
}                                                                               \
                                                                                \
void setValueSField_##TypeName(const std::string& fieldName, TypeName& value)   \
{                                                                               \
    typedef SField<TypeName, iNameSpace> FieldT;                                \
                                                                                \
    EditFieldHandlePtr        handle =                                          \
        _pPythonScript->editDynamicFieldByName(fieldName.c_str());              \
                                                                                \
    FieldT::EditHandlePtr editHandle =                                          \
        boost::dynamic_pointer_cast<FieldT::EditHandle>(handle);                \
    assert(editHandle);                                                         \
                                                                                \
    editHandle->getField()->setValue(value);                                    \
}                                                                               \
                                                                                \
TypeName& changeValueSField_##TypeName(const std::string& fieldName)            \
{                                                                               \
    typedef SField<TypeName, iNameSpace> FieldT;                                \
                                                                                \
    EditFieldHandlePtr        handle =                                          \
        _pPythonScript->editDynamicFieldByName(fieldName.c_str());              \
                                                                                \
    FieldT::EditHandlePtr editHandle =                                          \
        boost::dynamic_pointer_cast<FieldT::EditHandle>(handle);                \
    assert(editHandle);                                                         \
                                                                                \
    return editHandle->getField()->getValue();                                  \
}                                                                               \

// NRT -> non referenceable types
#define OSGPY_GEN_VALUESFIELD_CODE_NRT(TypeName, IntegralT, iNameSpace)         \
const IntegralT& getValueSField_##TypeName(const std::string& fieldName)        \
{                                                                               \
    typedef SField<IntegralT, iNameSpace> FieldT;                               \
                                                                                \
    GetFieldHandlePtr       handle =                                            \
            _pPythonScript->getDynamicFieldByName(fieldName.c_str());           \
    FieldT::GetHandlePtr getHandle =                                            \
            boost::dynamic_pointer_cast<FieldT::GetHandle>(handle);             \
    assert(getHandle);                                                          \
                                                                                \
    return getHandle->getField()->getValue();                                   \
}                                                                               \
                                                                                \
void setValueSField_##TypeName(const std::string& fieldName,                    \
                               boost::mpl::if_                                  \
                               <boost::is_class<IntegralT>, IntegralT&, IntegralT>::type value) \
{                                                                               \
    typedef SField<IntegralT, iNameSpace> FieldT;                               \
                                                                                \
    EditFieldHandlePtr        handle =                                          \
        _pPythonScript->editDynamicFieldByName(fieldName.c_str());              \
                                                                                \
    FieldT::EditHandlePtr editHandle =                                          \
        boost::dynamic_pointer_cast<FieldT::EditHandle>(handle);                \
    assert(editHandle);                                                         \
                                                                                \
    editHandle->getField()->setValue(value);                                    \
}                                                                               \

#define OSGPY_GEN_VALUEMFIELD_CODE(TypeName, RealType, iNameSpace)              \
const MField<RealType, iNameSpace>* getValueMField_##TypeName(const std::string& fieldName) \
{                                                                               \
    typedef MField<RealType, iNameSpace> FieldT;                                \
                                                                                \
    GetFieldHandlePtr       handle =                                            \
        _pPythonScript->getDynamicFieldByName(fieldName.c_str());               \
    assert(handle);                                                             \
    FieldT::GetHandlePtr getHandle =                                            \
        boost::dynamic_pointer_cast<FieldT::GetHandle>(handle);                 \
    assert(getHandle);                                                          \
                                                                                \
    return getHandle->getField();                                               \
}                                                                               \
                                                                                \
void setValueMField_##TypeName(const std::string& fieldName, MField<RealType, iNameSpace>& value) \
{                                                                               \
    typedef MField<RealType, iNameSpace> FieldT;                                \
                                                                                \
    EditFieldHandlePtr        handle =                                          \
        _pPythonScript->editDynamicFieldByName(fieldName.c_str());              \
    FieldT::EditHandlePtr editHandle =                                          \
        boost::dynamic_pointer_cast<FieldT::EditHandle>(handle);                \
    assert(editHandle);                                                         \
                                                                                \
    static_cast<FieldT *>(editHandle->getField())->setValues(value);            \
}                                                                               \
                                                                                \
MField<RealType, iNameSpace>* changeValueMField_##TypeName(const std::string& fieldName) \
{                                                                               \
    typedef MField<RealType, iNameSpace> FieldT;                                \
                                                                                \
    EditFieldHandlePtr        handle =                                          \
        _pPythonScript->editDynamicFieldByName(fieldName.c_str());              \
    FieldT::EditHandlePtr editHandle =                                          \
        boost::dynamic_pointer_cast<FieldT::EditHandle>(handle);                \
    assert(editHandle);                                                         \
                                                                                \
    return editHandle->getField();                                              \
}                                                                               \

class OSG_SCRIPTING_DLLMAPPING PyFieldAccessHandler : public PyFieldAccessHandlerBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef PyFieldAccessHandlerBase Inherited;
    typedef PyFieldAccessHandler     Self;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Init                                  */
    /*! \{                                                                 */

    bool init(PythonScript* pythonScript);

    bool exposeAllFieldsToPython();
    bool exposeFieldToPython(const std::string& fieldName,
                     OSG::UInt32        fieldId);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Sync                                  */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Output                                 */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Pointer Field Access                         */
    /*! \{                                                                 */

    void                       setPtrSField (const std::string& name,
                                             FieldContainer*     value);
    const FieldContainerRecPtr getPtrSField (const std::string& name  );
    // TODO: editPtrSField() and editMyPtrSField() are not exported, for whatever reason...
    FieldContainerTransitPtr   changePtrSField(const std::string& name  );

    PyFieldContainerPtrMFieldGetHandleTransitPtr
        getPointerMField (const std::string &fieldName);
    PyFieldContainerPtrMFieldEditHandleTransitPtr
        changePointerMField(const std::string &fieldName);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Value Field Access                           */
    /*! \{                                                                 */

    OSGPY_GEN_VALUESFIELD_CODE_NRT(Bool      , bool       , 2)
    OSGPY_GEN_VALUESFIELD_CODE_NRT(Int32     , Int32      , 0)
//    OSGPY_GEN_VALUESFIELD_CODE_NRT(Int64     , Int64      , 0)
    OSGPY_GEN_VALUESFIELD_CODE_NRT(UInt32    , UInt32     , 0)
//    OSGPY_GEN_VALUESFIELD_CODE_NRT(UInt64    , UInt64     , 0)
    OSGPY_GEN_VALUESFIELD_CODE_NRT(Real32    , Real32     , 0)
    OSGPY_GEN_VALUESFIELD_CODE_NRT(Real64    , Real64     , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Color3f                , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Color3ub               , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Color4f                , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Color4ub               , 0)
    OSGPY_GEN_VALUESFIELD_CODE_NRT(Time      , Time       , 1)
    OSGPY_GEN_VALUESFIELD_CODE    (BoxVolume              , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Plane                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Pnt2d                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Pnt2f                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Pnt3d                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Pnt3f                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Pnt4d                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Pnt4f                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Vec2d                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Vec2f                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Vec2ub                 , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Vec3d                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Vec3f                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Vec3ub                 , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Vec4d                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Vec4f                  , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Vec4ub                 , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Matrix                 , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Matrix4d               , 0)
    OSGPY_GEN_VALUESFIELD_CODE    (Quaternion             , 0)
    //OSGPY_GEN_VALUEFIELD_CODE     (Quaterniond            , 0)

    OSGPY_GEN_VALUEMFIELD_CODE(Bool       , bool       , 2)
    OSGPY_GEN_VALUEMFIELD_CODE(Int32      , Int32      , 0)
//    OSGPY_GEN_VALUEMFIELD_CODE(Int64      , Int64      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(UInt32     , UInt32     , 0)
//    OSGPY_GEN_VALUEMFIELD_CODE(UInt64     , UInt64     , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Real32     , Real32     , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Real64     , Real64     , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Color3f    , Color3f    , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Color3ub   , Color3ub   , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Color4f    , Color4f    , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Color4ub   , Color4ub   , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(String     , std::string, 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Time       , Time       , 1)
    OSGPY_GEN_VALUEMFIELD_CODE(BoxVolume  , BoxVolume  , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Plane      , Plane      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Pnt2d      , Pnt2d      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Pnt2f      , Pnt2f      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Pnt3d      , Pnt3d      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Pnt3f      , Pnt3f      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Pnt4d      , Pnt4d      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Pnt4f      , Pnt4f      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Vec2d      , Vec2d      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Vec2f      , Vec2f      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Vec2ub     , Vec2ub     , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Vec3d      , Vec3d      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Vec3f      , Vec3f      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Vec3ub     , Vec3ub     , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Vec4d      , Vec4d      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Vec4f      , Vec4f      , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Vec4ub     , Vec4ub     , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Matrix     , Matrix     , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Matrix4d   , Matrix4d   , 0)
    OSGPY_GEN_VALUEMFIELD_CODE(Quaternion , Quaternion , 0)
    //OSGPY_GEN_VALUEMFIELD_CODE(Quaterniond, Quaterniond, 0)

    // String handling is an exception as a string does not map to an integral
    // C++ type, but its OSG typename is different from its actual type
    // (std::string) and therefore does not fit into the macro structure above.
    std::string getValueSField_String(const std::string &fieldName)
    {
        typedef SField<std::string, 0> FieldT;

        GetFieldHandlePtr       handle =
                _pPythonScript->getDynamicFieldByName(fieldName.c_str());
        FieldT::GetHandlePtr getHandle =
                boost::dynamic_pointer_cast<FieldT::GetHandle>(handle);
        assert(getHandle);

        return getHandle->getField()->getValue();
    }

    void setValueSField_String(const std::string& fieldName,
                                                     const std::string& value)
    {
        typedef SField<std::string, 0> FieldT;

        EditFieldHandlePtr        handle =
            _pPythonScript->editDynamicFieldByName(fieldName.c_str());

        FieldT::EditHandlePtr editHandle =
            boost::dynamic_pointer_cast<FieldT::EditHandle>(handle);
        assert(editHandle);

        editHandle->getField()->setValue(value);
    }

    /*! \}                                                                 */

    /*=========================  PROTECTED  ===============================*/

protected:

    // Variables should all be in PyFieldAccessHandlerBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    PyFieldAccessHandler(void);
    PyFieldAccessHandler(const PyFieldAccessHandler &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~PyFieldAccessHandler(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class PyFieldAccessHandlerBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const PyFieldAccessHandler &source);

    PyInterpreter       *_pPyInterpreter;
    PythonScriptWeakPtr  _pPythonScript;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Python                                  */
    /*! \{                                                                 */

    bool generateCorePythonCode(const std::string& fieldName,
                                bool& getAccess,
                                bool& editAccess,
                                bool& setAccess);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Helper                                  */
    /*! \{                                                                 */

    void removeFieldQualifier   (std::string& str);
    void removeRefCountQualifier(std::string& str);

    /*! \}                                                                 */
};

typedef PyFieldAccessHandler *PyFieldAccessHandlerP;

OSG_END_NAMESPACE

#include "OSGPyFieldAccessHandlerBase.inl"
#include "OSGPyFieldAccessHandler.inl"

#endif /* _OSGPYFIELDACCESSHANDLER_H_ */
