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
 **     class PythonScript!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/

#include <cstdlib>
#include <cstdio>

#include "OSGAddOnsConfig.h"




#include "OSGPythonScriptBase.h"
#include "OSGPythonScript.h"

#include <boost/bind.hpp>

#ifdef WIN32 // turn off 'this' : used in base member initializer list warning
#pragma warning(disable:4355)
#endif

OSG_BEGIN_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class OSG::PythonScript
    
 */

/***************************************************************************\
 *                        Field Documentation                              *
\***************************************************************************/


/***************************************************************************\
 *                      FieldType/FieldTrait Instantiation                 *
\***************************************************************************/

#if !defined(OSG_DO_DOC) || defined(OSG_DOC_DEV)
PointerType FieldTraits<PythonScript *, nsOSG>::_type(
    "PythonScriptPtr", 
    "ScriptPtr", 
    PythonScript::getClassType(),
    nsOSG);
#endif

OSG_FIELDTRAITS_GETTYPE_NS(PythonScript *, nsOSG)

OSG_EXPORT_PTR_SFIELD_FULL(PointerSField,
                           PythonScript *,
                           nsOSG);

OSG_EXPORT_PTR_MFIELD_FULL(PointerMField,
                           PythonScript *,
                           nsOSG);

/***************************************************************************\
 *                         Field Description                               *
\***************************************************************************/

void PythonScriptBase::classDescInserter(TypeObject &oType)
{
}


PythonScriptBase::TypeObject PythonScriptBase::_type(
    PythonScriptBase::getClassname(),
    Inherited::getClassname(),
    "NULL",
    nsOSG, //Namespace
    reinterpret_cast<PrototypeCreateF>(&PythonScriptBase::createEmptyLocal),
    PythonScript::initMethod,
    PythonScript::exitMethod,
    reinterpret_cast<InitalInsertDescFunc>(&PythonScript::classDescInserter),
    true,
    0,
    "<?xml version=\"1.0\" ?>\n"
    "\n"
    "<FieldContainer\n"
    "    name=\"PythonScript\"\n"
    "    parent=\"Script\"\n"
    "    library=\"Scripting\"\n"
    "    package=\"OSGAddOns\"\n"
    "    structure=\"concrete\"\n"
    "    pointerfieldtypes=\"both\"\n"
    "    systemcomponent=\"true\"\n"
    "    parentsystemcomponent=\"true\"\n"
    "    isNodeCore=\"true\"\n"
    "    docGroupBase=\"GrpScripting\"\n"
    "    typeDescAddable=\"true\"\n"
    "    >\n"
    "</FieldContainer>\n",
    ""
    );

/*------------------------------ get -----------------------------------*/

FieldContainerType &PythonScriptBase::getType(void)
{
    return _type;
}

const FieldContainerType &PythonScriptBase::getType(void) const
{
    return _type;
}

UInt32 PythonScriptBase::getContainerSize(void) const
{
    return sizeof(PythonScript);
}

/*------------------------- decorator get ------------------------------*/






/*------------------------------ access -----------------------------------*/

SizeT PythonScriptBase::getBinSize(ConstFieldMaskArg whichField)
{
    SizeT returnValue = Inherited::getBinSize(whichField);


    return returnValue;
}

void PythonScriptBase::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

}

void PythonScriptBase::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

}

//! create a new instance of the class
PythonScriptTransitPtr PythonScriptBase::createLocal(BitVector bFlags)
{
    PythonScriptTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyLocal(bFlags);

        fc = dynamic_pointer_cast<PythonScript>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class, copy the container flags
PythonScriptTransitPtr PythonScriptBase::createDependent(BitVector bFlags)
{
    PythonScriptTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyDependent(bFlags);

        fc = dynamic_pointer_cast<PythonScript>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class
PythonScriptTransitPtr PythonScriptBase::create(void)
{
    PythonScriptTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopy();

        fc = dynamic_pointer_cast<PythonScript>(tmpPtr);
    }

    return fc;
}

PythonScript *PythonScriptBase::createEmptyLocal(BitVector bFlags)
{
    PythonScript *returnValue;

    newPtr<PythonScript>(returnValue, bFlags);

    returnValue->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

//! create an empty new instance of the class, do not copy the prototype
PythonScript *PythonScriptBase::createEmpty(void)
{
    PythonScript *returnValue;

    newPtr<PythonScript>(returnValue, Thread::getCurrentLocalFlags());

    returnValue->_pFieldFlags->_bNamespaceMask &=
        ~Thread::getCurrentLocalFlags();

    return returnValue;
}


FieldContainerTransitPtr PythonScriptBase::shallowCopyLocal(
    BitVector bFlags) const
{
    PythonScript *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const PythonScript *>(this), bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

FieldContainerTransitPtr PythonScriptBase::shallowCopyDependent(
    BitVector bFlags) const
{
    PythonScript *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const PythonScript *>(this), ~bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask = bFlags;

    return returnValue;
}

FieldContainerTransitPtr PythonScriptBase::shallowCopy(void) const
{
    PythonScript *tmpPtr;

    newPtr(tmpPtr,
           dynamic_cast<const PythonScript *>(this),
           Thread::getCurrentLocalFlags());

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~Thread::getCurrentLocalFlags();

    FieldContainerTransitPtr returnValue(tmpPtr);

    return returnValue;
}




/*------------------------- constructors ----------------------------------*/

PythonScriptBase::PythonScriptBase(void) :
    Inherited()
{
}

PythonScriptBase::PythonScriptBase(const PythonScriptBase &source) :
    Inherited(source)
{
}


/*-------------------------- destructors ----------------------------------*/

PythonScriptBase::~PythonScriptBase(void)
{
}



#ifdef OSG_MT_CPTR_ASPECT
void PythonScriptBase::execSyncV(      FieldContainer    &oFrom,
                                        ConstFieldMaskArg  whichField,
                                        AspectOffsetStore &oOffsets,
                                        ConstFieldMaskArg  syncMode,
                                  const UInt32             uiSyncInfo)
{
    PythonScript *pThis = static_cast<PythonScript *>(this);

    pThis->execSync(static_cast<PythonScript *>(&oFrom),
                    whichField,
                    oOffsets,
                    syncMode,
                    uiSyncInfo);
}
#endif


#ifdef OSG_MT_CPTR_ASPECT
FieldContainer *PythonScriptBase::createAspectCopy(
    const FieldContainer *pRefAspect) const
{
    PythonScript *returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const PythonScript *>(pRefAspect),
                  dynamic_cast<const PythonScript *>(this));

    return returnValue;
}
#endif

void PythonScriptBase::resolveLinks(void)
{
    Inherited::resolveLinks();


}


OSG_END_NAMESPACE
