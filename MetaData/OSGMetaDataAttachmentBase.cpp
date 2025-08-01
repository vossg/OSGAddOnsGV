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
 **     class MetaDataAttachment!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/

#include <cstdlib>
#include <cstdio>

#ifdef WIN32 
#pragma warning(disable: 4355) // turn off 'this' : used in base member initializer list warning
#pragma warning(disable: 4290) // disable exception specification warning
#endif

#include "OSGConfig.h"




#include "OSGMetaDataAttachmentBase.h"
#include "OSGMetaDataAttachment.h"

#include <boost/bind/bind.hpp>

OSG_BEGIN_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class OSG::MetaDataAttachment
    
 */

/***************************************************************************\
 *                        Field Documentation                              *
\***************************************************************************/

/*! \var std::string     MetaDataAttachmentBase::_mfKeys
    
*/

/*! \var std::string     MetaDataAttachmentBase::_mfValues
    
*/


/***************************************************************************\
 *                      FieldType/FieldTrait Instantiation                 *
\***************************************************************************/

#if !defined(OSG_DO_DOC) || defined(OSG_DOC_DEV)
PointerType FieldTraits<MetaDataAttachment *, nsOSG>::_type(
    "MetaDataAttachmentPtr", 
    "AttachmentPtr", 
    MetaDataAttachment::getClassType(),
    nsOSG);
#endif

OSG_FIELDTRAITS_GETTYPE_NS(MetaDataAttachment *, nsOSG)

OSG_EXPORT_PTR_SFIELD_FULL(PointerSField,
                           MetaDataAttachment *,
                           nsOSG)


/***************************************************************************\
 *                         Field Description                               *
\***************************************************************************/

void MetaDataAttachmentBase::classDescInserter(TypeObject &oType)
{
    FieldDescriptionBase *pDesc = NULL;


    pDesc = new MFString::Description(
        MFString::getClassType(),
        "keys",
        "",
        KeysFieldId, KeysFieldMask,
        false,
        (Field::MFDefaultFlags | Field::FStdAccess),
        static_cast<FieldEditMethodSig>(&MetaDataAttachment::editHandleKeys),
        static_cast<FieldGetMethodSig >(&MetaDataAttachment::getHandleKeys));

    oType.addInitialDesc(pDesc);

    pDesc = new MFString::Description(
        MFString::getClassType(),
        "values",
        "",
        ValuesFieldId, ValuesFieldMask,
        false,
        (Field::MFDefaultFlags | Field::FStdAccess),
        static_cast<FieldEditMethodSig>(&MetaDataAttachment::editHandleValues),
        static_cast<FieldGetMethodSig >(&MetaDataAttachment::getHandleValues));

    oType.addInitialDesc(pDesc);
}


MetaDataAttachmentBase::TypeObject MetaDataAttachmentBase::_type(
    MetaDataAttachmentBase::getClassname(),
    Inherited::getClassname(),
    "MetaDataAttachment",
    nsOSG, //Namespace
    reinterpret_cast<PrototypeCreateF>(&MetaDataAttachmentBase::createEmptyLocal),
    reinterpret_cast<InitContainerF>(&MetaDataAttachment::initMethod),
    reinterpret_cast<ExitContainerF>(&MetaDataAttachment::exitMethod),
    reinterpret_cast<InitalInsertDescFunc>(
        reinterpret_cast<void *>(&MetaDataAttachment::classDescInserter)),
    false,
    0,
    "<?xml version=\"1.0\"?>\n"
    "\n"
    "<FieldContainer\n"
    "\tname=\"MetaDataAttachment\"\n"
    "\tparent=\"Attachment\"\n"
    "\tlibrary=\"ContribMetaData\"\n"
    "\tpointerfieldtypes=\"single\"\n"
    "\tstructure=\"concrete\"\n"
    "\tsystemcomponent=\"true\"\n"
    "\tparentsystemcomponent=\"true\"\n"
    "\tdecoratable=\"false\"\n"
    "\tuseLocalIncludes=\"false\"\n"
    "    docGroupBase=\"GrpMetaData\"\n"
    "    >\n"
    "\n"
    "  <Field\n"
    "\t  name=\"keys\"\n"
    "\t  type=\"std::string\"\n"
    "\t  cardinality=\"multi\"\n"
    "\t  visibility=\"external\"\n"
    "\t  access=\"protected\"\n"
    "\t  >\n"
    "  </Field>\n"
    "  <Field\n"
    "\t  name=\"values\"\n"
    "\t  type=\"std::string\"\n"
    "\t  cardinality=\"multi\"\n"
    "\t  visibility=\"external\"\n"
    "\t  access=\"protected\"\n"
    "\t  >\n"
    "  </Field>\n"
    "</FieldContainer>\n",
    ""
    );

/*------------------------------ get -----------------------------------*/

FieldContainerType &MetaDataAttachmentBase::getType(void)
{
    return _type;
}

const FieldContainerType &MetaDataAttachmentBase::getType(void) const
{
    return _type;
}

UInt32 MetaDataAttachmentBase::getContainerSize(void) const
{
    return sizeof(MetaDataAttachment);
}

/*------------------------- decorator get ------------------------------*/


MFString *MetaDataAttachmentBase::editMFKeys(void)
{
    editMField(KeysFieldMask, _mfKeys);

    return &_mfKeys;
}

const MFString *MetaDataAttachmentBase::getMFKeys(void) const
{
    return &_mfKeys;
}


MFString *MetaDataAttachmentBase::editMFValues(void)
{
    editMField(ValuesFieldMask, _mfValues);

    return &_mfValues;
}

const MFString *MetaDataAttachmentBase::getMFValues(void) const
{
    return &_mfValues;
}






/*------------------------------ access -----------------------------------*/

SizeT MetaDataAttachmentBase::getBinSize(ConstFieldMaskArg whichField)
{
    SizeT returnValue = Inherited::getBinSize(whichField);

    if(FieldBits::NoField != (KeysFieldMask & whichField))
    {
        returnValue += _mfKeys.getBinSize();
    }
    if(FieldBits::NoField != (ValuesFieldMask & whichField))
    {
        returnValue += _mfValues.getBinSize();
    }

    return returnValue;
}

void MetaDataAttachmentBase::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

    if(FieldBits::NoField != (KeysFieldMask & whichField))
    {
        _mfKeys.copyToBin(pMem);
    }
    if(FieldBits::NoField != (ValuesFieldMask & whichField))
    {
        _mfValues.copyToBin(pMem);
    }
}

void MetaDataAttachmentBase::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

    if(FieldBits::NoField != (KeysFieldMask & whichField))
    {
        editMField(KeysFieldMask, _mfKeys);
        _mfKeys.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (ValuesFieldMask & whichField))
    {
        editMField(ValuesFieldMask, _mfValues);
        _mfValues.copyFromBin(pMem);
    }
}

//! create a new instance of the class
MetaDataAttachmentTransitPtr MetaDataAttachmentBase::createLocal(BitVector bFlags)
{
    MetaDataAttachmentTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyLocal(bFlags);

        fc = dynamic_pointer_cast<MetaDataAttachment>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class, copy the container flags
MetaDataAttachmentTransitPtr MetaDataAttachmentBase::createDependent(BitVector bFlags)
{
    MetaDataAttachmentTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyDependent(bFlags);

        fc = dynamic_pointer_cast<MetaDataAttachment>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class
MetaDataAttachmentTransitPtr MetaDataAttachmentBase::create(void)
{
    MetaDataAttachmentTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopy();

        fc = dynamic_pointer_cast<MetaDataAttachment>(tmpPtr);
    }

    return fc;
}

MetaDataAttachment *MetaDataAttachmentBase::createEmptyLocal(BitVector bFlags)
{
    MetaDataAttachment *returnValue;

    newPtr<MetaDataAttachment>(returnValue, bFlags);

    returnValue->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

//! create an empty new instance of the class, do not copy the prototype
MetaDataAttachment *MetaDataAttachmentBase::createEmpty(void)
{
    MetaDataAttachment *returnValue;

    newPtr<MetaDataAttachment>(returnValue, Thread::getCurrentLocalFlags());

    returnValue->_pFieldFlags->_bNamespaceMask &=
        ~Thread::getCurrentLocalFlags();

    return returnValue;
}


FieldContainerTransitPtr MetaDataAttachmentBase::shallowCopyLocal(
    BitVector bFlags) const
{
    MetaDataAttachment *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const MetaDataAttachment *>(this), bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

FieldContainerTransitPtr MetaDataAttachmentBase::shallowCopyDependent(
    BitVector bFlags) const
{
    MetaDataAttachment *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const MetaDataAttachment *>(this), ~bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask = bFlags;

    return returnValue;
}

FieldContainerTransitPtr MetaDataAttachmentBase::shallowCopy(void) const
{
    MetaDataAttachment *tmpPtr;

    newPtr(tmpPtr,
           dynamic_cast<const MetaDataAttachment *>(this),
           Thread::getCurrentLocalFlags());

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~Thread::getCurrentLocalFlags();

    FieldContainerTransitPtr returnValue(tmpPtr);

    return returnValue;
}




/*------------------------- constructors ----------------------------------*/

MetaDataAttachmentBase::MetaDataAttachmentBase(void) :
    Inherited(),
    _mfKeys                   (),
    _mfValues                 ()
{
}

MetaDataAttachmentBase::MetaDataAttachmentBase(const MetaDataAttachmentBase &source) :
    Inherited(source),
    _mfKeys                   (source._mfKeys                   ),
    _mfValues                 (source._mfValues                 )
{
}


/*-------------------------- destructors ----------------------------------*/

MetaDataAttachmentBase::~MetaDataAttachmentBase(void)
{
}


GetFieldHandlePtr MetaDataAttachmentBase::getHandleKeys            (void) const
{
    MFString::GetHandlePtr returnValue(
        new  MFString::GetHandle(
             &_mfKeys,
             this->getType().getFieldDesc(KeysFieldId),
             const_cast<MetaDataAttachmentBase *>(this)));

    return returnValue;
}

EditFieldHandlePtr MetaDataAttachmentBase::editHandleKeys           (void)
{
    MFString::EditHandlePtr returnValue(
        new  MFString::EditHandle(
             &_mfKeys,
             this->getType().getFieldDesc(KeysFieldId),
             this));


    editMField(KeysFieldMask, _mfKeys);

    return returnValue;
}

GetFieldHandlePtr MetaDataAttachmentBase::getHandleValues          (void) const
{
    MFString::GetHandlePtr returnValue(
        new  MFString::GetHandle(
             &_mfValues,
             this->getType().getFieldDesc(ValuesFieldId),
             const_cast<MetaDataAttachmentBase *>(this)));

    return returnValue;
}

EditFieldHandlePtr MetaDataAttachmentBase::editHandleValues         (void)
{
    MFString::EditHandlePtr returnValue(
        new  MFString::EditHandle(
             &_mfValues,
             this->getType().getFieldDesc(ValuesFieldId),
             this));


    editMField(ValuesFieldMask, _mfValues);

    return returnValue;
}


#ifdef OSG_MT_CPTR_ASPECT
void MetaDataAttachmentBase::execSyncV(      FieldContainer    &oFrom,
                                        ConstFieldMaskArg  whichField,
                                        AspectOffsetStore &oOffsets,
                                        ConstFieldMaskArg  syncMode,
                                  const UInt32             uiSyncInfo)
{
    MetaDataAttachment *pThis = static_cast<MetaDataAttachment *>(this);

    pThis->execSync(static_cast<MetaDataAttachment *>(&oFrom),
                    whichField,
                    oOffsets,
                    syncMode,
                    uiSyncInfo);
}
#endif


#ifdef OSG_MT_CPTR_ASPECT
FieldContainer *MetaDataAttachmentBase::createAspectCopy(
    const FieldContainer *pRefAspect) const
{
    MetaDataAttachment *returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const MetaDataAttachment *>(pRefAspect),
                  dynamic_cast<const MetaDataAttachment *>(this));

    return returnValue;
}
#endif

void MetaDataAttachmentBase::resolveLinks(void)
{
    Inherited::resolveLinks();

#ifdef OSG_MT_CPTR_ASPECT
    AspectOffsetStore oOffsets;

    _pAspectStore->fillOffsetArray(oOffsets, this);
#endif

#ifdef OSG_MT_CPTR_ASPECT
    _mfKeys.terminateShare(Thread::getCurrentAspect(),
                                      oOffsets);
#endif
#ifdef OSG_MT_CPTR_ASPECT
    _mfValues.terminateShare(Thread::getCurrentAspect(),
                                      oOffsets);
#endif
}


OSG_END_NAMESPACE
