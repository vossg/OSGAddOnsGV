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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include "OSGConfig.h"
#include "OSGPointerType.h"

#include "OSGPyFieldContainerPtrMFieldEditHandle.h"
#include "OSGPythonScript.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGPyFieldContainerPtrMFieldEditHandleBase.cpp file.
// To modify it, please change the .fcd file (OSGPyFieldContainerPtrMFieldEditHandle.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void PyFieldContainerPtrMFieldEditHandle::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
    }
}


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  public                                                                 -
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/*                                Sync                                     */

void PyFieldContainerPtrMFieldEditHandle::changed(ConstFieldMaskArg whichField,
                            UInt32            origin,
                            BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

/*-------------------------------------------------------------------------*/
/*                                Output                                   */

void PyFieldContainerPtrMFieldEditHandle::dump(UInt32,
                                               const BitVector ) const
{
    SLOG << "Dump PyFieldContainerPtrMFieldEditHandle NI" << std::endl;
}

/*-------------------------------------------------------------------------*/
/*                                 Init                                    */

/*!\brief Initializes the EditHandle in retrieving field from the */
/*  given PythonScript. The field has to be a dynamic field.      */
/*                                                                */
/* \return true on success, false otherwise                       */
/* \param pythonScript The PythonScript object to retrieve the    */
/*                     dynamic field from                         */
/* \param fieldName    The name of the field to retrieve          */
bool PyFieldContainerPtrMFieldEditHandle::init(PythonScript      *pythonScript,
                                               const std::string &fieldName)
{
    if(!pythonScript) // necessary for copy constructor
    {
        return true;
    }

    EditFieldHandlePtr handle =
            pythonScript->editDynamicFieldByName(fieldName.c_str());
    if(!handle)
    {
        std::cerr << "PyFieldContainerPtrMFieldEditHandle: field '"
                  << fieldName << " unknown." << std::endl;
        return false;
    }

    // basic sanity checks:
    if(!handle->isPointerField() ||
       !(handle->getCardinality() == FieldType::MultiField))
    {
        std::cerr << "PyFieldContainerPtrMFieldEditHandle: field is not a "
                     "pointer field or not a multifield." << std::endl;
        return false;
    }

    _pEditMFHandle = boost::dynamic_pointer_cast<
            FieldContainerPtrMFieldBase::EditHandle>(handle);

    if(!_pEditMFHandle)
    {
        std::cerr << "PyFieldContainerPtrMFieldEditHandle: cannot create a "
                     "EditHandle for field " << fieldName << std::endl;
        return false;
    }

    return true;
}

/*-------------------------------------------------------------------------*/
/*                       Python Iterator Interface                         */

/*!\brief Python iterator interface: This method is called before an   */
/*        iteration takes place. It resets the iterator and returns    */
/*        itself as the container interface is directly handled by the */
/*        class itself.                                                */
PyFieldContainerPtrMFieldEditHandleTransitPtr PyFieldContainerPtrMFieldEditHandle::iter()
{
    _uiIterPos = -1;
    return PyFieldContainerPtrMFieldEditHandleTransitPtr(this);
}

/*!\brief Python iterator interface: The method is called after each   */
/*        iteration. It raises a 'StopIteration' exception in case the */
/*        end of the iteration is reached.                             */
FieldContainerTransitPtr PyFieldContainerPtrMFieldEditHandle::next()
{
    ++_uiIterPos;

    if(_uiIterPos >= _pEditMFHandle->size())
    {
        PyErr_SetNone(PyExc_StopIteration);
        throw boost::python::error_already_set();
        return FieldContainerTransitPtr(NULL);
    }
    else
    {
        return getitem(_uiIterPos);
    }
}

/*!\brief Python iterator interface: Returns the length of the         */
/*        container.                                                   */
OSG::UInt32 PyFieldContainerPtrMFieldEditHandle::len()
{
    return _pEditMFHandle->size();
}

/*!\brief Python iterator interface: Returns an item. Negative key     */
/*        indices are supported.                                       */
FieldContainerTransitPtr PyFieldContainerPtrMFieldEditHandle::getitem(int key)
{
    if(key < 0)
    {
        key = key + _pEditMFHandle->size();
    }

    if(key < 0 || UInt32(key) > _pEditMFHandle->size())
    {
        PyErr_SetNone(PyExc_IndexError);
        throw boost::python::error_already_set();
        return FieldContainerTransitPtr(NULL);
    }

    return FieldContainerTransitPtr(_pEditMFHandle->get(key));
}

/*!\brief Python iterator interface: Sets an item. Negative key        */
/*        indices are supported.                                       */
void PyFieldContainerPtrMFieldEditHandle::setitem(int key, FieldContainer* value)
{
    if(key < 0)
    {
        key = _pEditMFHandle->size() - key;
    }

    if(key < 0 || UInt32(key) > _pEditMFHandle->size())
    {
        PyErr_SetNone(PyExc_IndexError);
        throw boost::python::error_already_set();
        return;
    }

    // Type check:

    // We are working on a pointer multi-field, so it is always safe to
    // downcast the content type to a PointerType.
    const PointerType& pType = dynamic_cast<const PointerType&>(_pEditMFHandle->getType().getContentType());

    if(value->getTypeId() != pType.getContentType().getId())
    {
        const char message[] = "Multi-field type does not match the given value type. Cannot replace the value!";
        PyErr_SetString(PyExc_TypeError, message);
        throw boost::python::error_already_set();
    }

    _pEditMFHandle->replace(key, value);
}

/*!\brief Python iterator interface: Appends a FieldContainer to the   */
/*        multi-field.                                                 */
void PyFieldContainerPtrMFieldEditHandle::append(FieldContainer *fc)
{
    _pEditMFHandle->add(fc);
}

/*-------------------------------------------------------------------------*\
 -  protected                                                              -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

PyFieldContainerPtrMFieldEditHandle::PyFieldContainerPtrMFieldEditHandle(void) :
    Inherited()
{
}

PyFieldContainerPtrMFieldEditHandle::PyFieldContainerPtrMFieldEditHandle(const PyFieldContainerPtrMFieldEditHandle &source) :
    Inherited(source)
{
}

PyFieldContainerPtrMFieldEditHandle::~PyFieldContainerPtrMFieldEditHandle(void)
{
}

OSG_END_NAMESPACE
