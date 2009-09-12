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

/*****************************************************************************\
 *****************************************************************************
 **                                                                         **
 **                  This file is automatically generated.                  **
 **                                                                         **
 **          Any changes made to this file WILL be lost when it is          **
 **           regenerated, which can become necessary at any time.          **
 **                                                                         **
 **     Do not change this file, changes should be done in the derived      **
 **     class RTImageTarget!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/


#define OSG_COMPILERTIMAGETARGETINST

#include <cstdlib>
#include <cstdio>
#include <boost/assign/list_of.hpp>

#include <OSGConfig.h>



#include <OSGImage.h> // Image Class

#include "OSGRTImageTargetBase.h"
#include "OSGRTImageTarget.h"

#include "boost/bind.hpp"

OSG_BEGIN_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class OSG::RTImageTarget
    
 */

/***************************************************************************\
 *                         Field Description                               *
\***************************************************************************/

/*! \var ImagePtr        RTImageTargetBase::_sfImage
    
*/


void RTImageTargetBase::classDescInserter(TypeObject &oType)
{
    FieldDescriptionBase *pDesc = NULL;


    pDesc = new SFUnrecImagePtr::Description(
        SFUnrecImagePtr::getClassType(),
        "image",
        "",
        ImageFieldId, ImageFieldMask,
        false,
        Field::SFDefaultFlags,
        static_cast<FieldEditMethodSig>(&RTImageTargetBase::editHandleImage),
        static_cast<FieldGetMethodSig >(&RTImageTargetBase::getHandleImage));

    oType.addInitialDesc(pDesc);
}


RTImageTargetBase::TypeObject RTImageTargetBase::_type(
    RTImageTargetBase::getClassname(),
    Inherited::getClassname(),
    "NULL",
    0,
    (PrototypeCreateF) &RTImageTargetBase::createEmptyLocal,
    RTImageTarget::initMethod,
    RTImageTarget::exitMethod,
    (InitalInsertDescFunc) &RTImageTargetBase::classDescInserter,
    false,
    0,
    "<?xml version=\"1.0\"?>\n"
    "\n"
    "<FieldContainer\n"
    "    name=\"RTImageTarget\"\n"
    "    parent=\"RTTarget\"\n"
    "    library=\"ContribRRT\"\n"
    "    pointerfieldtypes=\"single\"\n"
    "    structure=\"concrete\"\n"
    "    systemcomponent=\"true\"\n"
    "    parentsystemcomponent=\"true\"\n"
    "    decoratable=\"false\"\n"
    "    useLocalIncludes=\"false\"\n"
    "    isNodeCore=\"false\"\n"
    "    libnamespace=\"OSG\"\n"
    ">\n"
    "    <Field\n"
    "        name=\"image\"\n"
    "        type=\"ImagePtr\"\n"
    "        cardinality=\"single\"\n"
    "        visibility=\"external\"\n"
    "        access=\"public\"\n"
    "    >\n"
    "    </Field>\n"
    "</FieldContainer>\n",
    ""
    );

/*------------------------------ get -----------------------------------*/

FieldContainerType &RTImageTargetBase::getType(void)
{
    return _type;
}

const FieldContainerType &RTImageTargetBase::getType(void) const
{
    return _type;
}

UInt32 RTImageTargetBase::getContainerSize(void) const
{
    return sizeof(RTImageTarget);
}

/*------------------------- decorator get ------------------------------*/


//! Get the RTImageTarget::_sfImage field.
const SFUnrecImagePtr *RTImageTargetBase::getSFImage(void) const
{
    return &_sfImage;
}





/*------------------------------ access -----------------------------------*/

UInt32 RTImageTargetBase::getBinSize(ConstFieldMaskArg whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);

    if(FieldBits::NoField != (ImageFieldMask & whichField))
    {
        returnValue += _sfImage.getBinSize();
    }

    return returnValue;
}

void RTImageTargetBase::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

    if(FieldBits::NoField != (ImageFieldMask & whichField))
    {
        _sfImage.copyToBin(pMem);
    }
}

void RTImageTargetBase::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

    if(FieldBits::NoField != (ImageFieldMask & whichField))
    {
        _sfImage.copyFromBin(pMem);
    }
}

//! create a new instance of the class
RTImageTargetTransitPtr RTImageTargetBase::create(void)
{
    RTImageTargetTransitPtr fc;

    if(getClassType().getPrototype() != NullFC)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopy();

        fc = dynamic_pointer_cast<RTImageTarget>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class
RTImageTargetTransitPtr RTImageTargetBase::createLocal(BitVector bFlags)
{
    RTImageTargetTransitPtr fc;

    if(getClassType().getPrototype() != NullFC)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyLocal(bFlags);

        fc = dynamic_pointer_cast<RTImageTarget>(tmpPtr);
    }

    return fc;
}

//! create an empty new instance of the class, do not copy the prototype
RTImageTargetPtr RTImageTargetBase::createEmpty(void)
{
    RTImageTargetPtr returnValue;

    newPtr<RTImageTarget>(returnValue, Thread::getCurrentLocalFlags());

    returnValue->_pFieldFlags->_bNamespaceMask &= 
        ~Thread::getCurrentLocalFlags(); 

    return returnValue;
}

RTImageTargetPtr RTImageTargetBase::createEmptyLocal(BitVector bFlags)
{
    RTImageTargetPtr returnValue;

    newPtr<RTImageTarget>(returnValue, bFlags);

    returnValue->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

FieldContainerTransitPtr RTImageTargetBase::shallowCopy(void) const
{
    RTImageTargetPtr tmpPtr;

    newPtr(tmpPtr, 
           dynamic_cast<const RTImageTarget *>(this), 
           Thread::getCurrentLocalFlags());

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~Thread::getCurrentLocalFlags();

    FieldContainerTransitPtr returnValue(tmpPtr);

    return returnValue;
}

FieldContainerTransitPtr RTImageTargetBase::shallowCopyLocal(
    BitVector bFlags) const
{
    RTImageTargetPtr tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const RTImageTarget *>(this), bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}



/*------------------------- constructors ----------------------------------*/

RTImageTargetBase::RTImageTargetBase(void) :
    Inherited(),
    _sfImage                  (NullFC)
{
}

RTImageTargetBase::RTImageTargetBase(const RTImageTargetBase &source) :
    Inherited(source),
    _sfImage                  (NullFC)
{
}


/*-------------------------- destructors ----------------------------------*/

RTImageTargetBase::~RTImageTargetBase(void)
{
}

void RTImageTargetBase::onCreate(const RTImageTarget *source)
{
    Inherited::onCreate(source);

    if(source != NULL)
    {

        this->setImage(source->getImage());
    }
}

GetFieldHandlePtr RTImageTargetBase::getHandleImage           (void) const
{
    SFUnrecImagePtr::GetHandlePtr returnValue(
        new  SFUnrecImagePtr::GetHandle(
             &_sfImage, 
             this->getType().getFieldDesc(ImageFieldId)));

    return returnValue;
}

EditFieldHandlePtr RTImageTargetBase::editHandleImage          (void)
{
    SFUnrecImagePtr::EditHandlePtr returnValue(
        new  SFUnrecImagePtr::EditHandle(
             &_sfImage, 
             this->getType().getFieldDesc(ImageFieldId)));

    returnValue->setSetMethod(boost::bind(&RTImageTarget::setImage, 
                                          static_cast<RTImageTarget *>(this), _1));

    editSField(ImageFieldMask);

    return returnValue;
}


#ifdef OSG_MT_CPTR_ASPECT
void RTImageTargetBase::execSyncV(      FieldContainer    &oFrom,
                                        ConstFieldMaskArg  whichField,
                                        AspectOffsetStore &oOffsets,
                                        ConstFieldMaskArg  syncMode,
                                  const UInt32             uiSyncInfo)
{
    this->execSync(static_cast<RTImageTargetBase *>(&oFrom),
                   whichField,
                   oOffsets,
                   syncMode,
                   uiSyncInfo);
}
#endif


#ifdef OSG_MT_CPTR_ASPECT
FieldContainerPtr RTImageTargetBase::createAspectCopy(void) const
{
    RTImageTargetPtr returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const RTImageTarget *>(this));

    return returnValue;
}
#endif

void RTImageTargetBase::resolveLinks(void)
{
    Inherited::resolveLinks();

    static_cast<RTImageTarget *>(this)->setImage(NullFC);


}


#if !defined(OSG_DO_DOC) || defined(OSG_DOC_DEV)
DataType FieldTraits<RTImageTargetPtr>::_type("RTImageTargetPtr", "RTTargetPtr");
#endif

OSG_FIELDTRAITS_GETTYPE(RTImageTargetPtr)

OSG_EXPORT_PTR_SFIELD_FULL(FieldContainerPtrSField, 
                           RTImageTargetPtr, 
                           0);


OSG_END_NAMESPACE
