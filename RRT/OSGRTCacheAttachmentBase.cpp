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
 **     class RTCacheAttachment!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/


#define OSG_COMPILERTCACHEATTACHMENTINST

#include <cstdlib>
#include <cstdio>
#include <boost/assign/list_of.hpp>

#include <OSGConfig.h>




#include "OSGRTCacheAttachmentBase.h"
#include "OSGRTCacheAttachment.h"

#include "boost/bind.hpp"

OSG_BEGIN_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class OSG::RTCacheAttachment
    rt cache attachment
 */


RTCacheAttachmentBase::TypeObject RTCacheAttachmentBase::_type(
    RTCacheAttachmentBase::getClassname(),
    Inherited::getClassname(),
    "RTCacheAttachment",
    0,
    (PrototypeCreateF) &RTCacheAttachmentBase::createEmpty,
    RTCacheAttachment::initMethod,
    NULL,
    false,
    0,
    "<?xml version=\"1.0\"?>\n"
    "\n"
    "<FieldContainer\n"
    "\tname=\"RTCacheAttachment\"\n"
    "\tparent=\"FieldContainerAttachment\"\n"
    "\tlibrary=\"ContribRRT\"\n"
    "\tpointerfieldtypes=\"both\"\n"
    "\tstructure=\"concrete\"\n"
    "\tsystemcomponent=\"true\"\n"
    "\tparentsystemcomponent=\"true\"\n"
    "\tdecoratable=\"false\"\n"
    ">\n"
    "rt cache attachment\n"
    "</FieldContainer>\n",
    "rt cache attachment\n"
    );

/*------------------------------ get -----------------------------------*/

FieldContainerType &RTCacheAttachmentBase::getType(void)
{
    return _type;
}

const FieldContainerType &RTCacheAttachmentBase::getType(void) const
{
    return _type;
}

UInt32 RTCacheAttachmentBase::getContainerSize(void) const
{
    return sizeof(RTCacheAttachment);
}

/*------------------------- decorator get ------------------------------*/






/*------------------------------ access -----------------------------------*/

UInt32 RTCacheAttachmentBase::getBinSize(ConstFieldMaskArg whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);


    return returnValue;
}

void RTCacheAttachmentBase::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

}

void RTCacheAttachmentBase::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

}

//! create a new instance of the class
RTCacheAttachmentPtr RTCacheAttachmentBase::create(void)
{
    RTCacheAttachmentPtr fc;

    if(getClassType().getPrototype() != NullFC)
    {
        fc = dynamic_cast<RTCacheAttachment::ObjPtr>(
            getClassType().getPrototype()-> shallowCopy());
    }

    return fc;
}

//! create an empty new instance of the class, do not copy the prototype
RTCacheAttachmentPtr RTCacheAttachmentBase::createEmpty(void)
{
    RTCacheAttachmentPtr returnValue;

    newPtr<RTCacheAttachment>(returnValue);

    return returnValue;
}

FieldContainerPtr RTCacheAttachmentBase::shallowCopy(void) const
{
    RTCacheAttachmentPtr returnValue;

    newPtr(returnValue, dynamic_cast<const RTCacheAttachment *>(this));

    return returnValue;
}



/*------------------------- constructors ----------------------------------*/

RTCacheAttachmentBase::RTCacheAttachmentBase(void) :
    Inherited()
{
}

RTCacheAttachmentBase::RTCacheAttachmentBase(const RTCacheAttachmentBase &source) :
    Inherited(source)
{
}


/*-------------------------- destructors ----------------------------------*/

RTCacheAttachmentBase::~RTCacheAttachmentBase(void)
{
}



#ifdef OSG_MT_CPTR_ASPECT
void RTCacheAttachmentBase::execSyncV(      FieldContainer    &oFrom,
                                        ConstFieldMaskArg  whichField,
                                        AspectOffsetStore &oOffsets,
                                        ConstFieldMaskArg  syncMode,
                                  const UInt32             uiSyncInfo)
{
    this->execSync(static_cast<RTCacheAttachmentBase *>(&oFrom),
                   whichField,
                   oOffsets,
                   syncMode,
                   uiSyncInfo);
}
#endif


#ifdef OSG_MT_CPTR_ASPECT
FieldContainerPtr RTCacheAttachmentBase::createAspectCopy(void) const
{
    RTCacheAttachmentPtr returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const RTCacheAttachment *>(this));

    return returnValue;
}
#endif

void RTCacheAttachmentBase::resolveLinks(void)
{
    Inherited::resolveLinks();


}


OSG_END_NAMESPACE

#include "OSGSFieldAdaptor.ins"
#include "OSGMFieldAdaptor.ins"

OSG_BEGIN_NAMESPACE

#if !defined(OSG_DO_DOC) || defined(OSG_DOC_DEV)
DataType FieldTraits<RTCacheAttachmentPtr>::_type("RTCacheAttachmentPtr", "FieldContainerAttachmentPtr");
#endif

OSG_FIELDTRAITS_GETTYPE(RTCacheAttachmentPtr)

OSG_FIELD_DLLEXPORT_DEF2(SFieldAdaptor, RTCacheAttachmentPtr, SFFieldContainerPtr);
OSG_FIELD_DLLEXPORT_DEF2(MFieldAdaptor, RTCacheAttachmentPtr, MFFieldContainerPtr);

OSG_END_NAMESPACE
