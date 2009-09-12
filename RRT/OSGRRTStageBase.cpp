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
 **     class RRTStage!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/


#define OSG_COMPILERRTSTAGEINST

#include <cstdlib>
#include <cstdio>
#include <boost/assign/list_of.hpp>

#include <OSGConfig.h>



#include <OSGNode.h> // BackgroundRoot Class
#include <OSGTextureObjChunk.h> // TextureTarget Class

#include "OSGRRTStageBase.h"
#include "OSGRRTStage.h"

#include "boost/bind.hpp"

OSG_BEGIN_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class OSG::RRTStage
    
 */

/***************************************************************************\
 *                         Field Description                               *
\***************************************************************************/

/*! \var NodePtr         RRTStageBase::_sfBackgroundRoot
    
*/

/*! \var TextureObjChunkPtr RRTStageBase::_sfTextureTarget
    
*/

/*! \var UInt32          RRTStageBase::_sfWidth
    
*/

/*! \var UInt32          RRTStageBase::_sfHeight
    
*/

/*! \var bool            RRTStageBase::_sfSplitThreads
    
*/

/*! \var bool            RRTStageBase::_sfTiled
    
*/


void RRTStageBase::classDescInserter(TypeObject &oType)
{
    FieldDescriptionBase *pDesc = NULL;


    pDesc = new SFNodePtr::Description(
        SFNodePtr::getClassType(),
        "BackgroundRoot",
        "",
        BackgroundRootFieldId, BackgroundRootFieldMask,
        false,
        Field::SFDefaultFlags,
        static_cast<FieldEditMethodSig>(&RRTStageBase::editHandleBackgroundRoot),
        static_cast<FieldGetMethodSig >(&RRTStageBase::getHandleBackgroundRoot));

    oType.addInitialDesc(pDesc);

    pDesc = new SFTextureObjChunkPtr::Description(
        SFTextureObjChunkPtr::getClassType(),
        "TextureTarget",
        "",
        TextureTargetFieldId, TextureTargetFieldMask,
        false,
        Field::SFDefaultFlags,
        static_cast<FieldEditMethodSig>(&RRTStageBase::editHandleTextureTarget),
        static_cast<FieldGetMethodSig >(&RRTStageBase::getHandleTextureTarget));

    oType.addInitialDesc(pDesc);

    pDesc = new SFUInt32::Description(
        SFUInt32::getClassType(),
        "width",
        "",
        WidthFieldId, WidthFieldMask,
        false,
        Field::SFDefaultFlags,
        static_cast<FieldEditMethodSig>(&RRTStageBase::editHandleWidth),
        static_cast<FieldGetMethodSig >(&RRTStageBase::getHandleWidth));

    oType.addInitialDesc(pDesc);

    pDesc = new SFUInt32::Description(
        SFUInt32::getClassType(),
        "height",
        "",
        HeightFieldId, HeightFieldMask,
        false,
        Field::SFDefaultFlags,
        static_cast<FieldEditMethodSig>(&RRTStageBase::editHandleHeight),
        static_cast<FieldGetMethodSig >(&RRTStageBase::getHandleHeight));

    oType.addInitialDesc(pDesc);

    pDesc = new SFBool::Description(
        SFBool::getClassType(),
        "splitThreads",
        "",
        SplitThreadsFieldId, SplitThreadsFieldMask,
        false,
        Field::SFDefaultFlags,
        static_cast<FieldEditMethodSig>(&RRTStageBase::editHandleSplitThreads),
        static_cast<FieldGetMethodSig >(&RRTStageBase::getHandleSplitThreads));

    oType.addInitialDesc(pDesc);

    pDesc = new SFBool::Description(
        SFBool::getClassType(),
        "tiled",
        "",
        TiledFieldId, TiledFieldMask,
        false,
        Field::SFDefaultFlags,
        static_cast<FieldEditMethodSig>(&RRTStageBase::editHandleTiled),
        static_cast<FieldGetMethodSig >(&RRTStageBase::getHandleTiled));

    oType.addInitialDesc(pDesc);
}


RRTStageBase::TypeObject RRTStageBase::_type(
    RRTStageBase::getClassname(),
    Inherited::getClassname(),
    "NULL",
    0,
    (PrototypeCreateF) &RRTStageBase::createEmpty,
    RRTStage::initMethod,
    RRTStage::exitMethod,
    (InitalInsertDescFunc) &RRTStageBase::classDescInserter,
    false,
    0,
    "<?xml version=\"1.0\"?>\n"
    "\n"
    "<FieldContainer\n"
    "\tname=\"RRTStage\"\n"
    "\tparent=\"Stage\"\n"
    "\tlibrary=\"ContribRRT\"\n"
    "\tpointerfieldtypes=\"none\"\n"
    "\tstructure=\"concrete\"\n"
    "\tsystemcomponent=\"true\"\n"
    "\tparentsystemcomponent=\"true\"\n"
    "\tdecoratable=\"false\"\n"
    "\tuseLocalIncludes=\"false\"\n"
    "    isNodeCore=\"true\"\n"
    ">\n"
    "    <Field\n"
    "        name=\"BackgroundRoot\"\n"
    "        type=\"NodePtr\"\n"
    "        cardinality=\"single\"\n"
    "        visibility=\"external\"\n"
    "        access=\"public\"\n"
    "        defaultValue=\"NullFC\"\n"
    "    >\n"
    "    </Field>\n"
    "    <Field\n"
    "        name=\"TextureTarget\"\n"
    "        type=\"TextureObjChunkPtr\"\n"
    "        cardinality=\"single\"\n"
    "        visibility=\"external\"\n"
    "        access=\"public\"\n"
    "        defaultValue=\"NullFC\"\n"
    "    >\n"
    "    </Field>\n"
    "\t<Field\n"
    "\t\tname=\"width\"\n"
    "\t\ttype=\"UInt32\"\n"
    "\t\tcardinality=\"single\"\n"
    "\t\tvisibility=\"external\"\n"
    "\t\taccess=\"public\"\n"
    "        defaultValue=\"16\"\n"
    "\t>\n"
    "\t</Field>\n"
    "\t<Field\n"
    "\t\tname=\"height\"\n"
    "\t\ttype=\"UInt32\"\n"
    "\t\tcardinality=\"single\"\n"
    "\t\tvisibility=\"external\"\n"
    "\t\taccess=\"public\"\n"
    "        defaultValue=\"16\"\n"
    "\t>\n"
    "\t</Field>\n"
    "\t<Field\n"
    "\t\tname=\"splitThreads\"\n"
    "\t\ttype=\"bool\"\n"
    "\t\tcardinality=\"single\"\n"
    "\t\tvisibility=\"external\"\n"
    "\t\taccess=\"public\"\n"
    "        defaultValue=\"false\"\n"
    "\t>\n"
    "\t</Field>\n"
    "\t<Field\n"
    "\t\tname=\"tiled\"\n"
    "\t\ttype=\"bool\"\n"
    "\t\tcardinality=\"single\"\n"
    "\t\tvisibility=\"external\"\n"
    "\t\taccess=\"public\"\n"
    "        defaultValue=\"false\"\n"
    "\t>\n"
    "\t</Field>\n"
    "</FieldContainer>\n",
    ""
    );

/*------------------------------ get -----------------------------------*/

FieldContainerType &RRTStageBase::getType(void)
{
    return _type;
}

const FieldContainerType &RRTStageBase::getType(void) const
{
    return _type;
}

UInt32 RRTStageBase::getContainerSize(void) const
{
    return sizeof(RRTStage);
}

/*------------------------- decorator get ------------------------------*/


//! Get the RRTStage::_sfBackgroundRoot field.
const SFNodePtr *RRTStageBase::getSFBackgroundRoot(void) const
{
    return &_sfBackgroundRoot;
}

//! Get the RRTStage::_sfTextureTarget field.
const SFTextureObjChunkPtr *RRTStageBase::getSFTextureTarget(void) const
{
    return &_sfTextureTarget;
}

SFUInt32 *RRTStageBase::editSFWidth(void)
{
    editSField(WidthFieldMask);

    return &_sfWidth;
}

const SFUInt32 *RRTStageBase::getSFWidth(void) const
{
    return &_sfWidth;
}

#ifdef OSG_1_GET_COMPAT
SFUInt32            *RRTStageBase::getSFWidth          (void)
{
    return this->editSFWidth          ();
}
#endif

SFUInt32 *RRTStageBase::editSFHeight(void)
{
    editSField(HeightFieldMask);

    return &_sfHeight;
}

const SFUInt32 *RRTStageBase::getSFHeight(void) const
{
    return &_sfHeight;
}

#ifdef OSG_1_GET_COMPAT
SFUInt32            *RRTStageBase::getSFHeight         (void)
{
    return this->editSFHeight         ();
}
#endif

SFBool *RRTStageBase::editSFSplitThreads(void)
{
    editSField(SplitThreadsFieldMask);

    return &_sfSplitThreads;
}

const SFBool *RRTStageBase::getSFSplitThreads(void) const
{
    return &_sfSplitThreads;
}

#ifdef OSG_1_GET_COMPAT
SFBool              *RRTStageBase::getSFSplitThreads   (void)
{
    return this->editSFSplitThreads   ();
}
#endif

SFBool *RRTStageBase::editSFTiled(void)
{
    editSField(TiledFieldMask);

    return &_sfTiled;
}

const SFBool *RRTStageBase::getSFTiled(void) const
{
    return &_sfTiled;
}

#ifdef OSG_1_GET_COMPAT
SFBool              *RRTStageBase::getSFTiled          (void)
{
    return this->editSFTiled          ();
}
#endif





/*------------------------------ access -----------------------------------*/

UInt32 RRTStageBase::getBinSize(ConstFieldMaskArg whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);

    if(FieldBits::NoField != (BackgroundRootFieldMask & whichField))
    {
        returnValue += _sfBackgroundRoot.getBinSize();
    }
    if(FieldBits::NoField != (TextureTargetFieldMask & whichField))
    {
        returnValue += _sfTextureTarget.getBinSize();
    }
    if(FieldBits::NoField != (WidthFieldMask & whichField))
    {
        returnValue += _sfWidth.getBinSize();
    }
    if(FieldBits::NoField != (HeightFieldMask & whichField))
    {
        returnValue += _sfHeight.getBinSize();
    }
    if(FieldBits::NoField != (SplitThreadsFieldMask & whichField))
    {
        returnValue += _sfSplitThreads.getBinSize();
    }
    if(FieldBits::NoField != (TiledFieldMask & whichField))
    {
        returnValue += _sfTiled.getBinSize();
    }

    return returnValue;
}

void RRTStageBase::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

    if(FieldBits::NoField != (BackgroundRootFieldMask & whichField))
    {
        _sfBackgroundRoot.copyToBin(pMem);
    }
    if(FieldBits::NoField != (TextureTargetFieldMask & whichField))
    {
        _sfTextureTarget.copyToBin(pMem);
    }
    if(FieldBits::NoField != (WidthFieldMask & whichField))
    {
        _sfWidth.copyToBin(pMem);
    }
    if(FieldBits::NoField != (HeightFieldMask & whichField))
    {
        _sfHeight.copyToBin(pMem);
    }
    if(FieldBits::NoField != (SplitThreadsFieldMask & whichField))
    {
        _sfSplitThreads.copyToBin(pMem);
    }
    if(FieldBits::NoField != (TiledFieldMask & whichField))
    {
        _sfTiled.copyToBin(pMem);
    }
}

void RRTStageBase::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

    if(FieldBits::NoField != (BackgroundRootFieldMask & whichField))
    {
        _sfBackgroundRoot.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (TextureTargetFieldMask & whichField))
    {
        _sfTextureTarget.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (WidthFieldMask & whichField))
    {
        _sfWidth.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (HeightFieldMask & whichField))
    {
        _sfHeight.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (SplitThreadsFieldMask & whichField))
    {
        _sfSplitThreads.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (TiledFieldMask & whichField))
    {
        _sfTiled.copyFromBin(pMem);
    }
}

//! create a new instance of the class
RRTStagePtr RRTStageBase::create(void)
{
    RRTStagePtr fc;

    if(getClassType().getPrototype() != NullFC)
    {
        fc = dynamic_cast<RRTStage::ObjPtr>(
            getClassType().getPrototype()-> shallowCopy());
    }

    return fc;
}

//! create an empty new instance of the class, do not copy the prototype
RRTStagePtr RRTStageBase::createEmpty(void)
{
    RRTStagePtr returnValue;

    newPtr<RRTStage>(returnValue);

    return returnValue;
}

FieldContainerPtr RRTStageBase::shallowCopy(void) const
{
    RRTStagePtr returnValue;

    newPtr(returnValue, dynamic_cast<const RRTStage *>(this));

    return returnValue;
}



/*------------------------- constructors ----------------------------------*/

RRTStageBase::RRTStageBase(void) :
    Inherited(),
    _sfBackgroundRoot         (NodePtr(NullFC)),
    _sfTextureTarget          (TextureObjChunkPtr(NullFC)),
    _sfWidth                  (UInt32(16)),
    _sfHeight                 (UInt32(16)),
    _sfSplitThreads           (bool(false)),
    _sfTiled                  (bool(false))
{
}

RRTStageBase::RRTStageBase(const RRTStageBase &source) :
    Inherited(source),
    _sfBackgroundRoot         (NullFC),
    _sfTextureTarget          (NullFC),
    _sfWidth                  (source._sfWidth                  ),
    _sfHeight                 (source._sfHeight                 ),
    _sfSplitThreads           (source._sfSplitThreads           ),
    _sfTiled                  (source._sfTiled                  )
{
}


/*-------------------------- destructors ----------------------------------*/

RRTStageBase::~RRTStageBase(void)
{
}

void RRTStageBase::onCreate(const RRTStage *source)
{
    Inherited::onCreate(source);

    if(source != NULL)
    {

        this->setBackgroundRoot(source->getBackgroundRoot());

        this->setTextureTarget(source->getTextureTarget());
    }
}

GetFieldHandlePtr RRTStageBase::getHandleBackgroundRoot  (void) const
{
    SFNodePtr::GetHandlePtr returnValue(
        new  SFNodePtr::GetHandle(
             &_sfBackgroundRoot, 
             this->getType().getFieldDesc(BackgroundRootFieldId)));

    return returnValue;
}

EditFieldHandlePtr RRTStageBase::editHandleBackgroundRoot (void)
{
    SFNodePtr::EditHandlePtr returnValue(
        new  SFNodePtr::EditHandle(
             &_sfBackgroundRoot, 
             this->getType().getFieldDesc(BackgroundRootFieldId)));

    returnValue->setSetMethod(boost::bind(&RRTStage::setBackgroundRoot, 
                                          static_cast<RRTStage *>(this), _1));

    editSField(BackgroundRootFieldMask);

    return returnValue;
}

GetFieldHandlePtr RRTStageBase::getHandleTextureTarget   (void) const
{
    SFTextureObjChunkPtr::GetHandlePtr returnValue(
        new  SFTextureObjChunkPtr::GetHandle(
             &_sfTextureTarget, 
             this->getType().getFieldDesc(TextureTargetFieldId)));

    return returnValue;
}

EditFieldHandlePtr RRTStageBase::editHandleTextureTarget  (void)
{
    SFTextureObjChunkPtr::EditHandlePtr returnValue(
        new  SFTextureObjChunkPtr::EditHandle(
             &_sfTextureTarget, 
             this->getType().getFieldDesc(TextureTargetFieldId)));

    returnValue->setSetMethod(boost::bind(&RRTStage::setTextureTarget, 
                                          static_cast<RRTStage *>(this), _1));

    editSField(TextureTargetFieldMask);

    return returnValue;
}

GetFieldHandlePtr RRTStageBase::getHandleWidth           (void) const
{
    SFUInt32::GetHandlePtr returnValue(
        new  SFUInt32::GetHandle(
             &_sfWidth, 
             this->getType().getFieldDesc(WidthFieldId)));

    return returnValue;
}

EditFieldHandlePtr RRTStageBase::editHandleWidth          (void)
{
    SFUInt32::EditHandlePtr returnValue(
        new  SFUInt32::EditHandle(
             &_sfWidth, 
             this->getType().getFieldDesc(WidthFieldId)));

    editSField(WidthFieldMask);

    return returnValue;
}

GetFieldHandlePtr RRTStageBase::getHandleHeight          (void) const
{
    SFUInt32::GetHandlePtr returnValue(
        new  SFUInt32::GetHandle(
             &_sfHeight, 
             this->getType().getFieldDesc(HeightFieldId)));

    return returnValue;
}

EditFieldHandlePtr RRTStageBase::editHandleHeight         (void)
{
    SFUInt32::EditHandlePtr returnValue(
        new  SFUInt32::EditHandle(
             &_sfHeight, 
             this->getType().getFieldDesc(HeightFieldId)));

    editSField(HeightFieldMask);

    return returnValue;
}

GetFieldHandlePtr RRTStageBase::getHandleSplitThreads    (void) const
{
    SFBool::GetHandlePtr returnValue(
        new  SFBool::GetHandle(
             &_sfSplitThreads, 
             this->getType().getFieldDesc(SplitThreadsFieldId)));

    return returnValue;
}

EditFieldHandlePtr RRTStageBase::editHandleSplitThreads   (void)
{
    SFBool::EditHandlePtr returnValue(
        new  SFBool::EditHandle(
             &_sfSplitThreads, 
             this->getType().getFieldDesc(SplitThreadsFieldId)));

    editSField(SplitThreadsFieldMask);

    return returnValue;
}

GetFieldHandlePtr RRTStageBase::getHandleTiled           (void) const
{
    SFBool::GetHandlePtr returnValue(
        new  SFBool::GetHandle(
             &_sfTiled, 
             this->getType().getFieldDesc(TiledFieldId)));

    return returnValue;
}

EditFieldHandlePtr RRTStageBase::editHandleTiled          (void)
{
    SFBool::EditHandlePtr returnValue(
        new  SFBool::EditHandle(
             &_sfTiled, 
             this->getType().getFieldDesc(TiledFieldId)));

    editSField(TiledFieldMask);

    return returnValue;
}


#ifdef OSG_MT_CPTR_ASPECT
void RRTStageBase::execSyncV(      FieldContainer    &oFrom,
                                        ConstFieldMaskArg  whichField,
                                        AspectOffsetStore &oOffsets,
                                        ConstFieldMaskArg  syncMode,
                                  const UInt32             uiSyncInfo)
{
    this->execSync(static_cast<RRTStageBase *>(&oFrom),
                   whichField,
                   oOffsets,
                   syncMode,
                   uiSyncInfo);
}
#endif


#ifdef OSG_MT_CPTR_ASPECT
FieldContainerPtr RRTStageBase::createAspectCopy(void) const
{
    RRTStagePtr returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const RRTStage *>(this));

    return returnValue;
}
#endif

void RRTStageBase::resolveLinks(void)
{
    Inherited::resolveLinks();

    static_cast<RRTStage *>(this)->setBackgroundRoot(NullFC);

    static_cast<RRTStage *>(this)->setTextureTarget(NullFC);


}


#if !defined(OSG_DO_DOC) || defined(OSG_DOC_DEV)
DataType FieldTraits<RRTStagePtr>::_type("RRTStagePtr", "StagePtr");
#endif


OSG_END_NAMESPACE
