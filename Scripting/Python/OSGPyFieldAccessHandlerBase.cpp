/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2013 by the OpenSG Forum                 *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 * contact: dirk@opensg.org, gerrit.voss@vossg.org, carsten_neumann@gmx.net  *
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

/*****************************************************************************\
 *****************************************************************************
 **                                                                         **
 **                  This file is automatically generated.                  **
 **                                                                         **
 **          Any changes made to this file WILL be lost when it is          **
 **           regenerated, which can become necessary at any time.          **
 **                                                                         **
 **     Do not change this file, changes should be done in the derived      **
 **     class PyFieldAccessHandler!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/

#include <cstdlib>
#include <cstdio>

#include "OSGAddOnsConfig.h"




#include "OSGPyFieldAccessHandlerBase.h"
#include "OSGPyFieldAccessHandler.h"

#include <boost/bind.hpp>

#ifdef WIN32 // turn off 'this' : used in base member initializer list warning
#pragma warning(disable:4355)
#endif

OSG_BEGIN_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class OSG::PyFieldAccessHandler
    Dummy fcd to automatically include the PyFieldAccessHandler files into the
    Python bindings generation process. 
 */

/***************************************************************************\
 *                        Field Documentation                              *
\***************************************************************************/


/***************************************************************************\
 *                      FieldType/FieldTrait Instantiation                 *
\***************************************************************************/

#if !defined(OSG_DO_DOC) || defined(OSG_DOC_DEV)
PointerType FieldTraits<PyFieldAccessHandler *, nsOSG>::_type(
    "PyFieldAccessHandlerPtr", 
    "FieldContainerPtr", 
    PyFieldAccessHandler::getClassType(),
    nsOSG);
#endif

OSG_FIELDTRAITS_GETTYPE_NS(PyFieldAccessHandler *, nsOSG)

OSG_EXPORT_PTR_SFIELD_FULL(PointerSField,
                           PyFieldAccessHandler *,
                           nsOSG);

OSG_EXPORT_PTR_MFIELD_FULL(PointerMField,
                           PyFieldAccessHandler *,
                           nsOSG);

/***************************************************************************\
 *                         Field Description                               *
\***************************************************************************/

void PyFieldAccessHandlerBase::classDescInserter(TypeObject &oType)
{
}


PyFieldAccessHandlerBase::TypeObject PyFieldAccessHandlerBase::_type(
    PyFieldAccessHandlerBase::getClassname(),
    Inherited::getClassname(),
    "NULL",
    nsOSG, //Namespace
    reinterpret_cast<PrototypeCreateF>(&PyFieldAccessHandlerBase::createEmptyLocal),
    PyFieldAccessHandler::initMethod,
    PyFieldAccessHandler::exitMethod,
    reinterpret_cast<InitalInsertDescFunc>(&PyFieldAccessHandler::classDescInserter),
    false,
    0,
    "<?xml version=\"1.0\" ?>\n"
    "\n"
    "<FieldContainer\n"
    "    name=\"PyFieldAccessHandler\"\n"
    "    parent=\"FieldContainer\"\n"
    "    library=\"Scripting\"\n"
    "    package=\"OSGAddOns\"\n"
    "    structure=\"concrete\"\n"
    "    pointerfieldtypes=\"both\"\n"
    "    systemcomponent=\"true\"\n"
    "    parentsystemcomponent=\"true\"\n"
    "    isNodeCore=\"false\"\n"
    "    docGroupBase=\"GrpScripting\"\n"
    "    >\n"
    "  Dummy fcd to automatically include the PyFieldAccessHandler files into the\n"
    "  Python bindings generation process. \n"
    "</FieldContainer>\n",
    "Dummy fcd to automatically include the PyFieldAccessHandler files into the\n"
    "Python bindings generation process. \n"
    );

/*------------------------------ get -----------------------------------*/

FieldContainerType &PyFieldAccessHandlerBase::getType(void)
{
    return _type;
}

const FieldContainerType &PyFieldAccessHandlerBase::getType(void) const
{
    return _type;
}

UInt32 PyFieldAccessHandlerBase::getContainerSize(void) const
{
    return sizeof(PyFieldAccessHandler);
}

/*------------------------- decorator get ------------------------------*/






/*------------------------------ access -----------------------------------*/

SizeT PyFieldAccessHandlerBase::getBinSize(ConstFieldMaskArg whichField)
{
    SizeT returnValue = Inherited::getBinSize(whichField);


    return returnValue;
}

void PyFieldAccessHandlerBase::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

}

void PyFieldAccessHandlerBase::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

}

//! create a new instance of the class
PyFieldAccessHandlerTransitPtr PyFieldAccessHandlerBase::createLocal(BitVector bFlags)
{
    PyFieldAccessHandlerTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyLocal(bFlags);

        fc = dynamic_pointer_cast<PyFieldAccessHandler>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class, copy the container flags
PyFieldAccessHandlerTransitPtr PyFieldAccessHandlerBase::createDependent(BitVector bFlags)
{
    PyFieldAccessHandlerTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyDependent(bFlags);

        fc = dynamic_pointer_cast<PyFieldAccessHandler>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class
PyFieldAccessHandlerTransitPtr PyFieldAccessHandlerBase::create(void)
{
    PyFieldAccessHandlerTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopy();

        fc = dynamic_pointer_cast<PyFieldAccessHandler>(tmpPtr);
    }

    return fc;
}

PyFieldAccessHandler *PyFieldAccessHandlerBase::createEmptyLocal(BitVector bFlags)
{
    PyFieldAccessHandler *returnValue;

    newPtr<PyFieldAccessHandler>(returnValue, bFlags);

    returnValue->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

//! create an empty new instance of the class, do not copy the prototype
PyFieldAccessHandler *PyFieldAccessHandlerBase::createEmpty(void)
{
    PyFieldAccessHandler *returnValue;

    newPtr<PyFieldAccessHandler>(returnValue, Thread::getCurrentLocalFlags());

    returnValue->_pFieldFlags->_bNamespaceMask &=
        ~Thread::getCurrentLocalFlags();

    return returnValue;
}


FieldContainerTransitPtr PyFieldAccessHandlerBase::shallowCopyLocal(
    BitVector bFlags) const
{
    PyFieldAccessHandler *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const PyFieldAccessHandler *>(this), bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

FieldContainerTransitPtr PyFieldAccessHandlerBase::shallowCopyDependent(
    BitVector bFlags) const
{
    PyFieldAccessHandler *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const PyFieldAccessHandler *>(this), ~bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask = bFlags;

    return returnValue;
}

FieldContainerTransitPtr PyFieldAccessHandlerBase::shallowCopy(void) const
{
    PyFieldAccessHandler *tmpPtr;

    newPtr(tmpPtr,
           dynamic_cast<const PyFieldAccessHandler *>(this),
           Thread::getCurrentLocalFlags());

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~Thread::getCurrentLocalFlags();

    FieldContainerTransitPtr returnValue(tmpPtr);

    return returnValue;
}




/*------------------------- constructors ----------------------------------*/

PyFieldAccessHandlerBase::PyFieldAccessHandlerBase(void) :
    Inherited()
{
}

PyFieldAccessHandlerBase::PyFieldAccessHandlerBase(const PyFieldAccessHandlerBase &source) :
    Inherited(source)
{
}


/*-------------------------- destructors ----------------------------------*/

PyFieldAccessHandlerBase::~PyFieldAccessHandlerBase(void)
{
}



#ifdef OSG_MT_CPTR_ASPECT
void PyFieldAccessHandlerBase::execSyncV(      FieldContainer    &oFrom,
                                        ConstFieldMaskArg  whichField,
                                        AspectOffsetStore &oOffsets,
                                        ConstFieldMaskArg  syncMode,
                                  const UInt32             uiSyncInfo)
{
    PyFieldAccessHandler *pThis = static_cast<PyFieldAccessHandler *>(this);

    pThis->execSync(static_cast<PyFieldAccessHandler *>(&oFrom),
                    whichField,
                    oOffsets,
                    syncMode,
                    uiSyncInfo);
}
#endif


#ifdef OSG_MT_CPTR_ASPECT
FieldContainer *PyFieldAccessHandlerBase::createAspectCopy(
    const FieldContainer *pRefAspect) const
{
    PyFieldAccessHandler *returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const PyFieldAccessHandler *>(pRefAspect),
                  dynamic_cast<const PyFieldAccessHandler *>(this));

    return returnValue;
}
#endif

void PyFieldAccessHandlerBase::resolveLinks(void)
{
    Inherited::resolveLinks();


}


OSG_END_NAMESPACE
