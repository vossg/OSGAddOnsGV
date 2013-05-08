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
 **     class CudaBufferPnt3fInterpolator!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/

#include <cstdlib>
#include <cstdio>

#include "OSGAddOnsConfig.h"




#include "OSGCudaBufferPnt3fInterpolatorBase.h"
#include "OSGCudaBufferPnt3fInterpolator.h"

#include <boost/bind.hpp>

#ifdef WIN32 // turn off 'this' : used in base member initializer list warning
#pragma warning(disable:4355)
#endif

OSG_BEGIN_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class OSG::CudaBufferPnt3fInterpolator
    
 */

/***************************************************************************\
 *                        Field Documentation                              *
\***************************************************************************/


/***************************************************************************\
 *                      FieldType/FieldTrait Instantiation                 *
\***************************************************************************/

#if !defined(OSG_DO_DOC) || defined(OSG_DOC_DEV)
PointerType FieldTraits<CudaBufferPnt3fInterpolator *, nsOSG>::_type(
    "CudaBufferPnt3fInterpolatorPtr", 
    "BufferPnt3fInterpolatorPtr", 
    CudaBufferPnt3fInterpolator::getClassType(),
    nsOSG);
#endif

OSG_FIELDTRAITS_GETTYPE_NS(CudaBufferPnt3fInterpolator *, nsOSG)

/***************************************************************************\
 *                         Field Description                               *
\***************************************************************************/

void CudaBufferPnt3fInterpolatorBase::classDescInserter(TypeObject &oType)
{
    Inherited::classDescInserter(oType);

}


CudaBufferPnt3fInterpolatorBase::TypeObject CudaBufferPnt3fInterpolatorBase::_type(
    CudaBufferPnt3fInterpolatorBase::getClassname(),
    Inherited::getClassname(),
    "NULL",
    nsOSG, //Namespace
    reinterpret_cast<PrototypeCreateF>(&CudaBufferPnt3fInterpolatorBase::createEmptyLocal),
    CudaBufferPnt3fInterpolator::initMethod,
    CudaBufferPnt3fInterpolator::exitMethod,
    reinterpret_cast<InitalInsertDescFunc>(&CudaBufferPnt3fInterpolator::classDescInserter),
    false,
    0,
    "<?xml version=\"1.0\"?>\n"
    "\n"
    "<FieldContainer\n"
    "    name=\"CudaBufferPnt3fInterpolator\"\n"
    "    parent=\"BufferPnt3fInterpolator\"\n"
    "    mixinparent=\"CudaBufferPnt3fInterpolatorParent\"\n"
    "    library=\"ContribCompute\"\n"
    "    package=\"OSGAddOns\"\n"
    "    pointerfieldtypes=\"none\"\n"
    "    structure=\"concrete\"\n"
    "    systemcomponent=\"true\"\n"
    "    parentsystemcomponent=\"true\"\n"
    "    decoratable=\"false\"\n"
    "    useLocalIncludes=\"false\"\n"
    "    isNodeCore=\"true\"\n"
    "    isBundle=\"false\"\n"
    "    parentFields=\"none\"\n"
    "    >\n"
    "</FieldContainer>\n",
    ""
    );

/*------------------------------ get -----------------------------------*/

FieldContainerType &CudaBufferPnt3fInterpolatorBase::getType(void)
{
    return _type;
}

const FieldContainerType &CudaBufferPnt3fInterpolatorBase::getType(void) const
{
    return _type;
}

UInt32 CudaBufferPnt3fInterpolatorBase::getContainerSize(void) const
{
    return sizeof(CudaBufferPnt3fInterpolator);
}

/*------------------------- decorator get ------------------------------*/






/*------------------------------ access -----------------------------------*/

SizeT CudaBufferPnt3fInterpolatorBase::getBinSize(ConstFieldMaskArg whichField)
{
    SizeT returnValue = Inherited::getBinSize(whichField);


    return returnValue;
}

void CudaBufferPnt3fInterpolatorBase::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

}

void CudaBufferPnt3fInterpolatorBase::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

}

//! create a new instance of the class
CudaBufferPnt3fInterpolatorTransitPtr CudaBufferPnt3fInterpolatorBase::createLocal(BitVector bFlags)
{
    CudaBufferPnt3fInterpolatorTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyLocal(bFlags);

        fc = dynamic_pointer_cast<CudaBufferPnt3fInterpolator>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class, copy the container flags
CudaBufferPnt3fInterpolatorTransitPtr CudaBufferPnt3fInterpolatorBase::createDependent(BitVector bFlags)
{
    CudaBufferPnt3fInterpolatorTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyDependent(bFlags);

        fc = dynamic_pointer_cast<CudaBufferPnt3fInterpolator>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class
CudaBufferPnt3fInterpolatorTransitPtr CudaBufferPnt3fInterpolatorBase::create(void)
{
    CudaBufferPnt3fInterpolatorTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopy();

        fc = dynamic_pointer_cast<CudaBufferPnt3fInterpolator>(tmpPtr);
    }

    return fc;
}

CudaBufferPnt3fInterpolator *CudaBufferPnt3fInterpolatorBase::createEmptyLocal(BitVector bFlags)
{
    CudaBufferPnt3fInterpolator *returnValue;

    newPtr<CudaBufferPnt3fInterpolator>(returnValue, bFlags);

    returnValue->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

//! create an empty new instance of the class, do not copy the prototype
CudaBufferPnt3fInterpolator *CudaBufferPnt3fInterpolatorBase::createEmpty(void)
{
    CudaBufferPnt3fInterpolator *returnValue;

    newPtr<CudaBufferPnt3fInterpolator>(returnValue, Thread::getCurrentLocalFlags());

    returnValue->_pFieldFlags->_bNamespaceMask &=
        ~Thread::getCurrentLocalFlags();

    return returnValue;
}


FieldContainerTransitPtr CudaBufferPnt3fInterpolatorBase::shallowCopyLocal(
    BitVector bFlags) const
{
    CudaBufferPnt3fInterpolator *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const CudaBufferPnt3fInterpolator *>(this), bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

FieldContainerTransitPtr CudaBufferPnt3fInterpolatorBase::shallowCopyDependent(
    BitVector bFlags) const
{
    CudaBufferPnt3fInterpolator *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const CudaBufferPnt3fInterpolator *>(this), ~bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask = bFlags;

    return returnValue;
}

FieldContainerTransitPtr CudaBufferPnt3fInterpolatorBase::shallowCopy(void) const
{
    CudaBufferPnt3fInterpolator *tmpPtr;

    newPtr(tmpPtr,
           dynamic_cast<const CudaBufferPnt3fInterpolator *>(this),
           Thread::getCurrentLocalFlags());

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~Thread::getCurrentLocalFlags();

    FieldContainerTransitPtr returnValue(tmpPtr);

    return returnValue;
}




/*------------------------- constructors ----------------------------------*/

CudaBufferPnt3fInterpolatorBase::CudaBufferPnt3fInterpolatorBase(void) :
    Inherited()
{
}

CudaBufferPnt3fInterpolatorBase::CudaBufferPnt3fInterpolatorBase(const CudaBufferPnt3fInterpolatorBase &source) :
    Inherited(source)
{
}


/*-------------------------- destructors ----------------------------------*/

CudaBufferPnt3fInterpolatorBase::~CudaBufferPnt3fInterpolatorBase(void)
{
}



#ifdef OSG_MT_CPTR_ASPECT
void CudaBufferPnt3fInterpolatorBase::execSyncV(      FieldContainer    &oFrom,
                                        ConstFieldMaskArg  whichField,
                                        AspectOffsetStore &oOffsets,
                                        ConstFieldMaskArg  syncMode,
                                  const UInt32             uiSyncInfo)
{
    CudaBufferPnt3fInterpolator *pThis = static_cast<CudaBufferPnt3fInterpolator *>(this);

    pThis->execSync(static_cast<CudaBufferPnt3fInterpolator *>(&oFrom),
                    whichField,
                    oOffsets,
                    syncMode,
                    uiSyncInfo);
}
#endif


#ifdef OSG_MT_CPTR_ASPECT
FieldContainer *CudaBufferPnt3fInterpolatorBase::createAspectCopy(
    const FieldContainer *pRefAspect) const
{
    CudaBufferPnt3fInterpolator *returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const CudaBufferPnt3fInterpolator *>(pRefAspect),
                  dynamic_cast<const CudaBufferPnt3fInterpolator *>(this));

    return returnValue;
}
#endif

void CudaBufferPnt3fInterpolatorBase::resolveLinks(void)
{
    Inherited::resolveLinks();


}


OSG_END_NAMESPACE
