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

#include "OSGAddOnsConfig.h"

#include "OSGPyFieldContainerPtrMFieldGetHandle.h"
#include "OSGPythonScript.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGPyFieldContainerPtrMFieldGetHandleBase.cpp file.
// To modify it, please change the .fcd file (OSGPyFieldContainerPtrMFieldGetHandle.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void PyFieldContainerPtrMFieldGetHandle::initMethod(InitPhase ePhase)
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

void PyFieldContainerPtrMFieldGetHandle::changed(ConstFieldMaskArg whichField,
                            UInt32            origin,
                            BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

/*-------------------------------------------------------------------------*/
/*                                Output                                   */

void PyFieldContainerPtrMFieldGetHandle::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump PyFieldContainerPtrMFieldGetHandle NI" << std::endl;
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
bool PyFieldContainerPtrMFieldGetHandle::init(PythonScript      *pythonScript,
                                               const std::string &fieldName)
{
    if(!pythonScript) // necessary for copy constructor
    {
        return true;
    }

    GetFieldHandlePtr handle =
            pythonScript->getDynamicFieldByName(fieldName.c_str());
    if(!handle)
    {
        std::cerr << "PyFieldContainerPtrMFieldGetHandle: field '"
                  << fieldName << " unknown." << std::endl;
        return false;
    }

    // basic sanity checks:
    if(!handle->isPointerField() ||
       !(handle->getCardinality() == FieldType::MultiField))
    {
        std::cerr << "PyFieldContainerPtrMFieldGetHandle: field is not a "
                     "pointer field or not a multifield." << std::endl;
        return false;
    }

    _pGetMFHandle = boost::dynamic_pointer_cast<
            FieldContainerPtrMFieldBase::GetHandle>(handle);

    if(!_pGetMFHandle)
    {
        std::cerr << "PyFieldContainerPtrMFieldGetHandle: cannot create a "
                     "GetHandle for field " << fieldName << std::endl;
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

PyFieldContainerPtrMFieldGetHandleTransitPtr PyFieldContainerPtrMFieldGetHandle::iter()
{
    _uiIterPos = -1;
    return PyFieldContainerPtrMFieldGetHandleTransitPtr(this);
}

/*!\brief Python iterator interface: The method is called after each   */
/*        iteration. It raises a 'StopIteration' exception in case the */
/*        end of the iteration is reached.                             */
FieldContainerTransitPtr PyFieldContainerPtrMFieldGetHandle::next()
{
    ++_uiIterPos;

    if(_uiIterPos >= _pGetMFHandle->size())
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
OSG::UInt32 PyFieldContainerPtrMFieldGetHandle::len()
{
    return UInt32(_pGetMFHandle->size());
}

/*!\brief Python iterator interface: Returns an item. Negative key     */
/*        indices are supported.                                       */
FieldContainerTransitPtr PyFieldContainerPtrMFieldGetHandle::getitem(int key)
{
    if(key < 0)
    {
        key = UInt32(_pGetMFHandle->size()) - key;
    }

    if(key < 0 || UInt32(key) > _pGetMFHandle->size())
    {
        PyErr_SetNone(PyExc_IndexError);
        throw boost::python::error_already_set();
        return FieldContainerTransitPtr(NULL);
    }

    return FieldContainerTransitPtr(_pGetMFHandle->get(key));
}

/*\brief Python iterator interface: This method is only needed to      */
/*       the user that the GetHandle cannot be used to change a value. */
/*       When it is used a 'TypeError' exception is thrown with an     */
/*       according error message. No value is changed.                 */
void PyFieldContainerPtrMFieldGetHandle::setitem(int key, FieldContainer* value)
{
    const char message[] = "Cannot change the field via this GetHandle! Use self.edit_<fieldname> to get an EditHandle.";
    PyErr_SetString(PyExc_TypeError, message);
    throw boost::python::error_already_set();
}

/*-------------------------------------------------------------------------*\
 -  protected                                                              -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

PyFieldContainerPtrMFieldGetHandle::PyFieldContainerPtrMFieldGetHandle(void) :
     Inherited   (    ),
    _pGetMFHandle(NULL),
    _uiIterPos   (0   )
{
}

PyFieldContainerPtrMFieldGetHandle::PyFieldContainerPtrMFieldGetHandle(
    const PyFieldContainerPtrMFieldGetHandle &source) :

     Inherited   (source),
    _pGetMFHandle(NULL  ),
    _uiIterPos   (0     )
{
}

PyFieldContainerPtrMFieldGetHandle::~PyFieldContainerPtrMFieldGetHandle(void)
{
}

OSG_END_NAMESPACE
