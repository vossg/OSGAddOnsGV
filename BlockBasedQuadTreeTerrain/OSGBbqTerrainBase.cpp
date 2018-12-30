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
 **     class BbqTerrain!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/

#include <cstdlib>
#include <cstdio>

#ifdef WIN32 
#pragma warning(disable: 4355) // turn off 'this' : used in base member initializer list warning
#pragma warning(disable: 4290) // disable exception specification warning
#endif

#include "OSGAddOnsConfig.h"



#include "OSGNode.h"                    // Beacon Class
#include "OSGBbqDataSource.h"           // DataSource Class

#include "OSGBbqTerrainBase.h"
#include "OSGBbqTerrain.h"

#include <boost/bind.hpp>

OSG_BEGIN_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class OSG::BbqTerrain
    
 */

/***************************************************************************\
 *                        Field Documentation                              *
\***************************************************************************/

/*! \var Node *          BbqTerrainBase::_sfBeacon
    
*/

/*! \var BbqDataSource * BbqTerrainBase::_sfDataSource
    
*/

/*! \var UInt32          BbqTerrainBase::_sfMaxNumResidentNodes
    
*/

/*! \var Real32          BbqTerrainBase::_sfScreenSpaceError
    
*/

/*! \var bool            BbqTerrainBase::_sfEnableSkirts
    
*/

/*! \var bool            BbqTerrainBase::_sfShowSwitchDistance
    
*/


/***************************************************************************\
 *                      FieldType/FieldTrait Instantiation                 *
\***************************************************************************/

#if !defined(OSG_DO_DOC) || defined(OSG_DOC_DEV)
PointerType FieldTraits<BbqTerrain *, nsOSG>::_type(
    "BbqTerrainPtr", 
    "StageDrawablePtr", 
    BbqTerrain::getClassType(),
    nsOSG);
#endif

OSG_FIELDTRAITS_GETTYPE_NS(BbqTerrain *, nsOSG)

/***************************************************************************\
 *                         Field Description                               *
\***************************************************************************/

void BbqTerrainBase::classDescInserter(TypeObject &oType)
{
    FieldDescriptionBase *pDesc = NULL;


    pDesc = new SFUnrecNodePtr::Description(
        SFUnrecNodePtr::getClassType(),
        "beacon",
        "",
        BeaconFieldId, BeaconFieldMask,
        false,
        (Field::SFDefaultFlags | Field::FStdAccess),
        static_cast<FieldEditMethodSig>(&BbqTerrain::editHandleBeacon),
        static_cast<FieldGetMethodSig >(&BbqTerrain::getHandleBeacon));

    oType.addInitialDesc(pDesc);

    pDesc = new SFUnrecBbqDataSourcePtr::Description(
        SFUnrecBbqDataSourcePtr::getClassType(),
        "dataSource",
        "",
        DataSourceFieldId, DataSourceFieldMask,
        false,
        (Field::SFDefaultFlags | Field::FStdAccess),
        static_cast<FieldEditMethodSig>(&BbqTerrain::editHandleDataSource),
        static_cast<FieldGetMethodSig >(&BbqTerrain::getHandleDataSource));

    oType.addInitialDesc(pDesc);

    pDesc = new SFUInt32::Description(
        SFUInt32::getClassType(),
        "maxNumResidentNodes",
        "",
        MaxNumResidentNodesFieldId, MaxNumResidentNodesFieldMask,
        false,
        (Field::SFDefaultFlags | Field::FStdAccess),
        static_cast<FieldEditMethodSig>(&BbqTerrain::editHandleMaxNumResidentNodes),
        static_cast<FieldGetMethodSig >(&BbqTerrain::getHandleMaxNumResidentNodes));

    oType.addInitialDesc(pDesc);

    pDesc = new SFReal32::Description(
        SFReal32::getClassType(),
        "screenSpaceError",
        "",
        ScreenSpaceErrorFieldId, ScreenSpaceErrorFieldMask,
        false,
        (Field::SFDefaultFlags | Field::FStdAccess),
        static_cast<FieldEditMethodSig>(&BbqTerrain::editHandleScreenSpaceError),
        static_cast<FieldGetMethodSig >(&BbqTerrain::getHandleScreenSpaceError));

    oType.addInitialDesc(pDesc);

    pDesc = new SFBool::Description(
        SFBool::getClassType(),
        "enableSkirts",
        "",
        EnableSkirtsFieldId, EnableSkirtsFieldMask,
        false,
        (Field::SFDefaultFlags | Field::FStdAccess),
        static_cast<FieldEditMethodSig>(&BbqTerrain::editHandleEnableSkirts),
        static_cast<FieldGetMethodSig >(&BbqTerrain::getHandleEnableSkirts));

    oType.addInitialDesc(pDesc);

    pDesc = new SFBool::Description(
        SFBool::getClassType(),
        "showSwitchDistance",
        "",
        ShowSwitchDistanceFieldId, ShowSwitchDistanceFieldMask,
        false,
        (Field::SFDefaultFlags | Field::FStdAccess),
        static_cast<FieldEditMethodSig>(&BbqTerrain::editHandleShowSwitchDistance),
        static_cast<FieldGetMethodSig >(&BbqTerrain::getHandleShowSwitchDistance));

    oType.addInitialDesc(pDesc);
}


BbqTerrainBase::TypeObject BbqTerrainBase::_type(
    BbqTerrainBase::getClassname(),
    Inherited::getClassname(),
    "NULL",
    nsOSG, //Namespace
    reinterpret_cast<PrototypeCreateF>(&BbqTerrainBase::createEmptyLocal),
    reinterpret_cast<InitContainerF>(&BbqTerrain::initMethod),
    reinterpret_cast<ExitContainerF>(&BbqTerrain::exitMethod),
    reinterpret_cast<InitalInsertDescFunc>(
        reinterpret_cast<void *>(&BbqTerrain::classDescInserter)),
    false,
    0,
    "<?xml version=\"1.0\"?>\n"
    "\n"
    "<FieldContainer\n"
    "    name=\"BbqTerrain\"\n"
    "    parent=\"StageDrawable\"\n"
    "    pointerfieldtypes=\"none\"\n"
    "    structure=\"concrete\"\n"
    "    systemcomponent=\"true\"\n"
    "    parentsystemcomponent=\"true\"\n"
    "    decoratable=\"false\"\n"
    "    useLocalIncludes=\"false\"\n"
    "    library=\"ContribBbqTerrain\"\n"
    "    package=\"OSGAddOns\"\n"
    "    >\n"
    "  <Field\n"
    "\t  name=\"beacon\"\n"
    "\t  type=\"NodePtr\"\n"
    "\t  cardinality=\"single\"\n"
    "\t  visibility=\"external\"\n"
    "\t  defaultValue=\"NULL\"\n"
    "\t  access=\"public\"\n"
    "\t  >\n"
    "  </Field>\n"
    "  <Field\n"
    "\t  name=\"dataSource\"\n"
    "\t  type=\"BbqDataSourcePtr\"\n"
    "\t  cardinality=\"single\"\n"
    "\t  visibility=\"external\"\n"
    "\t  defaultValue=\"NULL\"\n"
    "\t  access=\"public\"\n"
    "\t  >\n"
    "  </Field>\n"
    "  <Field\n"
    "\t  name=\"maxNumResidentNodes\"\n"
    "\t  type=\"UInt32\"\n"
    "\t  cardinality=\"single\"\n"
    "\t  visibility=\"external\"\n"
    "\t  defaultValue=\"5000\"\n"
    "\t  access=\"public\"\n"
    "\t  >\n"
    "  </Field>\n"
    "  <Field\n"
    "\t  name=\"screenSpaceError\"\n"
    "\t  type=\"Real32\"\n"
    "\t  cardinality=\"single\"\n"
    "\t  visibility=\"external\"\n"
    "\t  defaultValue=\"5.f\"\n"
    "\t  access=\"public\"\n"
    "\t  >\n"
    "  </Field>\n"
    "  <Field\n"
    "\t  name=\"enableSkirts\"\n"
    "\t  type=\"bool\"\n"
    "\t  cardinality=\"single\"\n"
    "\t  visibility=\"external\"\n"
    "\t  defaultValue=\"true\"\n"
    "\t  access=\"public\"\n"
    "\t  >\n"
    "  </Field>    \n"
    "  <Field\n"
    "\t  name=\"showSwitchDistance\"\n"
    "\t  type=\"bool\"\n"
    "\t  cardinality=\"single\"\n"
    "\t  visibility=\"external\"\n"
    "\t  defaultValue=\"false\"\n"
    "\t  access=\"public\"\n"
    "\t  >\n"
    "  </Field>    \n"
    "</FieldContainer>\n",
    ""
    );

/*------------------------------ get -----------------------------------*/

FieldContainerType &BbqTerrainBase::getType(void)
{
    return _type;
}

const FieldContainerType &BbqTerrainBase::getType(void) const
{
    return _type;
}

UInt32 BbqTerrainBase::getContainerSize(void) const
{
    return sizeof(BbqTerrain);
}

/*------------------------- decorator get ------------------------------*/


//! Get the BbqTerrain::_sfBeacon field.
const SFUnrecNodePtr *BbqTerrainBase::getSFBeacon(void) const
{
    return &_sfBeacon;
}

SFUnrecNodePtr      *BbqTerrainBase::editSFBeacon         (void)
{
    editSField(BeaconFieldMask);

    return &_sfBeacon;
}

//! Get the value of the BbqTerrain::_sfBeacon field.
Node * BbqTerrainBase::getBeacon(void) const
{
    return _sfBeacon.getValue();
}

//! Set the value of the BbqTerrain::_sfBeacon field.
void BbqTerrainBase::setBeacon(Node * const value)
{
    editSField(BeaconFieldMask);

    _sfBeacon.setValue(value);
}


//! Get the BbqTerrain::_sfDataSource field.
const SFUnrecBbqDataSourcePtr *BbqTerrainBase::getSFDataSource(void) const
{
    return &_sfDataSource;
}

SFUnrecBbqDataSourcePtr *BbqTerrainBase::editSFDataSource     (void)
{
    editSField(DataSourceFieldMask);

    return &_sfDataSource;
}

//! Get the value of the BbqTerrain::_sfDataSource field.
BbqDataSource * BbqTerrainBase::getDataSource(void) const
{
    return _sfDataSource.getValue();
}

//! Set the value of the BbqTerrain::_sfDataSource field.
void BbqTerrainBase::setDataSource(BbqDataSource * const value)
{
    editSField(DataSourceFieldMask);

    _sfDataSource.setValue(value);
}


SFUInt32 *BbqTerrainBase::editSFMaxNumResidentNodes(void)
{
    editSField(MaxNumResidentNodesFieldMask);

    return &_sfMaxNumResidentNodes;
}

const SFUInt32 *BbqTerrainBase::getSFMaxNumResidentNodes(void) const
{
    return &_sfMaxNumResidentNodes;
}


SFReal32 *BbqTerrainBase::editSFScreenSpaceError(void)
{
    editSField(ScreenSpaceErrorFieldMask);

    return &_sfScreenSpaceError;
}

const SFReal32 *BbqTerrainBase::getSFScreenSpaceError(void) const
{
    return &_sfScreenSpaceError;
}


SFBool *BbqTerrainBase::editSFEnableSkirts(void)
{
    editSField(EnableSkirtsFieldMask);

    return &_sfEnableSkirts;
}

const SFBool *BbqTerrainBase::getSFEnableSkirts(void) const
{
    return &_sfEnableSkirts;
}


SFBool *BbqTerrainBase::editSFShowSwitchDistance(void)
{
    editSField(ShowSwitchDistanceFieldMask);

    return &_sfShowSwitchDistance;
}

const SFBool *BbqTerrainBase::getSFShowSwitchDistance(void) const
{
    return &_sfShowSwitchDistance;
}






/*------------------------------ access -----------------------------------*/

SizeT BbqTerrainBase::getBinSize(ConstFieldMaskArg whichField)
{
    SizeT returnValue = Inherited::getBinSize(whichField);

    if(FieldBits::NoField != (BeaconFieldMask & whichField))
    {
        returnValue += _sfBeacon.getBinSize();
    }
    if(FieldBits::NoField != (DataSourceFieldMask & whichField))
    {
        returnValue += _sfDataSource.getBinSize();
    }
    if(FieldBits::NoField != (MaxNumResidentNodesFieldMask & whichField))
    {
        returnValue += _sfMaxNumResidentNodes.getBinSize();
    }
    if(FieldBits::NoField != (ScreenSpaceErrorFieldMask & whichField))
    {
        returnValue += _sfScreenSpaceError.getBinSize();
    }
    if(FieldBits::NoField != (EnableSkirtsFieldMask & whichField))
    {
        returnValue += _sfEnableSkirts.getBinSize();
    }
    if(FieldBits::NoField != (ShowSwitchDistanceFieldMask & whichField))
    {
        returnValue += _sfShowSwitchDistance.getBinSize();
    }

    return returnValue;
}

void BbqTerrainBase::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

    if(FieldBits::NoField != (BeaconFieldMask & whichField))
    {
        _sfBeacon.copyToBin(pMem);
    }
    if(FieldBits::NoField != (DataSourceFieldMask & whichField))
    {
        _sfDataSource.copyToBin(pMem);
    }
    if(FieldBits::NoField != (MaxNumResidentNodesFieldMask & whichField))
    {
        _sfMaxNumResidentNodes.copyToBin(pMem);
    }
    if(FieldBits::NoField != (ScreenSpaceErrorFieldMask & whichField))
    {
        _sfScreenSpaceError.copyToBin(pMem);
    }
    if(FieldBits::NoField != (EnableSkirtsFieldMask & whichField))
    {
        _sfEnableSkirts.copyToBin(pMem);
    }
    if(FieldBits::NoField != (ShowSwitchDistanceFieldMask & whichField))
    {
        _sfShowSwitchDistance.copyToBin(pMem);
    }
}

void BbqTerrainBase::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

    if(FieldBits::NoField != (BeaconFieldMask & whichField))
    {
        editSField(BeaconFieldMask);
        _sfBeacon.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (DataSourceFieldMask & whichField))
    {
        editSField(DataSourceFieldMask);
        _sfDataSource.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (MaxNumResidentNodesFieldMask & whichField))
    {
        editSField(MaxNumResidentNodesFieldMask);
        _sfMaxNumResidentNodes.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (ScreenSpaceErrorFieldMask & whichField))
    {
        editSField(ScreenSpaceErrorFieldMask);
        _sfScreenSpaceError.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (EnableSkirtsFieldMask & whichField))
    {
        editSField(EnableSkirtsFieldMask);
        _sfEnableSkirts.copyFromBin(pMem);
    }
    if(FieldBits::NoField != (ShowSwitchDistanceFieldMask & whichField))
    {
        editSField(ShowSwitchDistanceFieldMask);
        _sfShowSwitchDistance.copyFromBin(pMem);
    }
}

//! create a new instance of the class
BbqTerrainTransitPtr BbqTerrainBase::createLocal(BitVector bFlags)
{
    BbqTerrainTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyLocal(bFlags);

        fc = dynamic_pointer_cast<BbqTerrain>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class, copy the container flags
BbqTerrainTransitPtr BbqTerrainBase::createDependent(BitVector bFlags)
{
    BbqTerrainTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopyDependent(bFlags);

        fc = dynamic_pointer_cast<BbqTerrain>(tmpPtr);
    }

    return fc;
}

//! create a new instance of the class
BbqTerrainTransitPtr BbqTerrainBase::create(void)
{
    BbqTerrainTransitPtr fc;

    if(getClassType().getPrototype() != NULL)
    {
        FieldContainerTransitPtr tmpPtr =
            getClassType().getPrototype()-> shallowCopy();

        fc = dynamic_pointer_cast<BbqTerrain>(tmpPtr);
    }

    return fc;
}

BbqTerrain *BbqTerrainBase::createEmptyLocal(BitVector bFlags)
{
    BbqTerrain *returnValue;

    newPtr<BbqTerrain>(returnValue, bFlags);

    returnValue->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

//! create an empty new instance of the class, do not copy the prototype
BbqTerrain *BbqTerrainBase::createEmpty(void)
{
    BbqTerrain *returnValue;

    newPtr<BbqTerrain>(returnValue, Thread::getCurrentLocalFlags());

    returnValue->_pFieldFlags->_bNamespaceMask &=
        ~Thread::getCurrentLocalFlags();

    return returnValue;
}


FieldContainerTransitPtr BbqTerrainBase::shallowCopyLocal(
    BitVector bFlags) const
{
    BbqTerrain *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const BbqTerrain *>(this), bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~bFlags;

    return returnValue;
}

FieldContainerTransitPtr BbqTerrainBase::shallowCopyDependent(
    BitVector bFlags) const
{
    BbqTerrain *tmpPtr;

    newPtr(tmpPtr, dynamic_cast<const BbqTerrain *>(this), ~bFlags);

    FieldContainerTransitPtr returnValue(tmpPtr);

    tmpPtr->_pFieldFlags->_bNamespaceMask = bFlags;

    return returnValue;
}

FieldContainerTransitPtr BbqTerrainBase::shallowCopy(void) const
{
    BbqTerrain *tmpPtr;

    newPtr(tmpPtr,
           dynamic_cast<const BbqTerrain *>(this),
           Thread::getCurrentLocalFlags());

    tmpPtr->_pFieldFlags->_bNamespaceMask &= ~Thread::getCurrentLocalFlags();

    FieldContainerTransitPtr returnValue(tmpPtr);

    return returnValue;
}




/*------------------------- constructors ----------------------------------*/

BbqTerrainBase::BbqTerrainBase(void) :
    Inherited(),
    _sfBeacon                 (NULL),
    _sfDataSource             (NULL),
    _sfMaxNumResidentNodes    (UInt32(5000)),
    _sfScreenSpaceError       (Real32(5.f)),
    _sfEnableSkirts           (bool(true)),
    _sfShowSwitchDistance     (bool(false))
{
}

BbqTerrainBase::BbqTerrainBase(const BbqTerrainBase &source) :
    Inherited(source),
    _sfBeacon                 (NULL),
    _sfDataSource             (NULL),
    _sfMaxNumResidentNodes    (source._sfMaxNumResidentNodes    ),
    _sfScreenSpaceError       (source._sfScreenSpaceError       ),
    _sfEnableSkirts           (source._sfEnableSkirts           ),
    _sfShowSwitchDistance     (source._sfShowSwitchDistance     )
{
}


/*-------------------------- destructors ----------------------------------*/

BbqTerrainBase::~BbqTerrainBase(void)
{
}

void BbqTerrainBase::onCreate(const BbqTerrain *source)
{
    Inherited::onCreate(source);

    if(source != NULL)
    {
        BbqTerrain *pThis = static_cast<BbqTerrain *>(this);

        pThis->setBeacon(source->getBeacon());

        pThis->setDataSource(source->getDataSource());
    }
}

GetFieldHandlePtr BbqTerrainBase::getHandleBeacon          (void) const
{
    SFUnrecNodePtr::GetHandlePtr returnValue(
        new  SFUnrecNodePtr::GetHandle(
             &_sfBeacon,
             this->getType().getFieldDesc(BeaconFieldId),
             const_cast<BbqTerrainBase *>(this)));

    return returnValue;
}

EditFieldHandlePtr BbqTerrainBase::editHandleBeacon         (void)
{
    SFUnrecNodePtr::EditHandlePtr returnValue(
        new  SFUnrecNodePtr::EditHandle(
             &_sfBeacon,
             this->getType().getFieldDesc(BeaconFieldId),
             this));

    returnValue->setSetMethod(
        boost::bind(&BbqTerrain::setBeacon,
                    static_cast<BbqTerrain *>(this), _1));

    editSField(BeaconFieldMask);

    return returnValue;
}

GetFieldHandlePtr BbqTerrainBase::getHandleDataSource      (void) const
{
    SFUnrecBbqDataSourcePtr::GetHandlePtr returnValue(
        new  SFUnrecBbqDataSourcePtr::GetHandle(
             &_sfDataSource,
             this->getType().getFieldDesc(DataSourceFieldId),
             const_cast<BbqTerrainBase *>(this)));

    return returnValue;
}

EditFieldHandlePtr BbqTerrainBase::editHandleDataSource     (void)
{
    SFUnrecBbqDataSourcePtr::EditHandlePtr returnValue(
        new  SFUnrecBbqDataSourcePtr::EditHandle(
             &_sfDataSource,
             this->getType().getFieldDesc(DataSourceFieldId),
             this));

    returnValue->setSetMethod(
        boost::bind(&BbqTerrain::setDataSource,
                    static_cast<BbqTerrain *>(this), _1));

    editSField(DataSourceFieldMask);

    return returnValue;
}

GetFieldHandlePtr BbqTerrainBase::getHandleMaxNumResidentNodes (void) const
{
    SFUInt32::GetHandlePtr returnValue(
        new  SFUInt32::GetHandle(
             &_sfMaxNumResidentNodes,
             this->getType().getFieldDesc(MaxNumResidentNodesFieldId),
             const_cast<BbqTerrainBase *>(this)));

    return returnValue;
}

EditFieldHandlePtr BbqTerrainBase::editHandleMaxNumResidentNodes(void)
{
    SFUInt32::EditHandlePtr returnValue(
        new  SFUInt32::EditHandle(
             &_sfMaxNumResidentNodes,
             this->getType().getFieldDesc(MaxNumResidentNodesFieldId),
             this));


    editSField(MaxNumResidentNodesFieldMask);

    return returnValue;
}

GetFieldHandlePtr BbqTerrainBase::getHandleScreenSpaceError (void) const
{
    SFReal32::GetHandlePtr returnValue(
        new  SFReal32::GetHandle(
             &_sfScreenSpaceError,
             this->getType().getFieldDesc(ScreenSpaceErrorFieldId),
             const_cast<BbqTerrainBase *>(this)));

    return returnValue;
}

EditFieldHandlePtr BbqTerrainBase::editHandleScreenSpaceError(void)
{
    SFReal32::EditHandlePtr returnValue(
        new  SFReal32::EditHandle(
             &_sfScreenSpaceError,
             this->getType().getFieldDesc(ScreenSpaceErrorFieldId),
             this));


    editSField(ScreenSpaceErrorFieldMask);

    return returnValue;
}

GetFieldHandlePtr BbqTerrainBase::getHandleEnableSkirts    (void) const
{
    SFBool::GetHandlePtr returnValue(
        new  SFBool::GetHandle(
             &_sfEnableSkirts,
             this->getType().getFieldDesc(EnableSkirtsFieldId),
             const_cast<BbqTerrainBase *>(this)));

    return returnValue;
}

EditFieldHandlePtr BbqTerrainBase::editHandleEnableSkirts   (void)
{
    SFBool::EditHandlePtr returnValue(
        new  SFBool::EditHandle(
             &_sfEnableSkirts,
             this->getType().getFieldDesc(EnableSkirtsFieldId),
             this));


    editSField(EnableSkirtsFieldMask);

    return returnValue;
}

GetFieldHandlePtr BbqTerrainBase::getHandleShowSwitchDistance (void) const
{
    SFBool::GetHandlePtr returnValue(
        new  SFBool::GetHandle(
             &_sfShowSwitchDistance,
             this->getType().getFieldDesc(ShowSwitchDistanceFieldId),
             const_cast<BbqTerrainBase *>(this)));

    return returnValue;
}

EditFieldHandlePtr BbqTerrainBase::editHandleShowSwitchDistance(void)
{
    SFBool::EditHandlePtr returnValue(
        new  SFBool::EditHandle(
             &_sfShowSwitchDistance,
             this->getType().getFieldDesc(ShowSwitchDistanceFieldId),
             this));


    editSField(ShowSwitchDistanceFieldMask);

    return returnValue;
}


#ifdef OSG_MT_CPTR_ASPECT
void BbqTerrainBase::execSyncV(      FieldContainer    &oFrom,
                                        ConstFieldMaskArg  whichField,
                                        AspectOffsetStore &oOffsets,
                                        ConstFieldMaskArg  syncMode,
                                  const UInt32             uiSyncInfo)
{
    BbqTerrain *pThis = static_cast<BbqTerrain *>(this);

    pThis->execSync(static_cast<BbqTerrain *>(&oFrom),
                    whichField,
                    oOffsets,
                    syncMode,
                    uiSyncInfo);
}
#endif


#ifdef OSG_MT_CPTR_ASPECT
FieldContainer *BbqTerrainBase::createAspectCopy(
    const FieldContainer *pRefAspect) const
{
    BbqTerrain *returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const BbqTerrain *>(pRefAspect),
                  dynamic_cast<const BbqTerrain *>(this));

    return returnValue;
}
#endif

void BbqTerrainBase::resolveLinks(void)
{
    Inherited::resolveLinks();

    static_cast<BbqTerrain *>(this)->setBeacon(NULL);

    static_cast<BbqTerrain *>(this)->setDataSource(NULL);


}


OSG_END_NAMESPACE
