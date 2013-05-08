/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2002 by the OpenSG Forum                 *
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
 **     class DynamicTerrain!
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/


#define OSG_COMPILEDYNAMICTERRAININST

#include <stdlib.h>
#include <stdio.h>

#include <OSGAddOnsConfig.h>

#include "OSGDynamicTerrainBase.h"
#include "OSGDynamicTerrain.h"


OSG_BEGIN_NAMESPACE

const OSG::BitVector  DynamicTerrainBase::LevelSizeFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::LevelSizeFieldId);

const OSG::BitVector  DynamicTerrainBase::HeightDataFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::HeightDataFieldId);

const OSG::BitVector  DynamicTerrainBase::HeightDataScaleFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::HeightDataScaleFieldId);

const OSG::BitVector  DynamicTerrainBase::HeightDataOffsetFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::HeightDataOffsetFieldId);

const OSG::BitVector  DynamicTerrainBase::SampleDistanceFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::SampleDistanceFieldId);

const OSG::BitVector  DynamicTerrainBase::TextureDataFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::TextureDataFieldId);

const OSG::BitVector  DynamicTerrainBase::HeightColorTextureFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::HeightColorTextureFieldId);

const OSG::BitVector  DynamicTerrainBase::SampleUpdateBudgetFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::SampleUpdateBudgetFieldId);

const OSG::BitVector  DynamicTerrainBase::EnableFrustumCullingFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::EnableFrustumCullingFieldId);

const OSG::BitVector  DynamicTerrainBase::UseGpuRendererFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::UseGpuRendererFieldId);

const OSG::BitVector  DynamicTerrainBase::UseVboExtensionFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::UseVboExtensionFieldId);

const OSG::BitVector  DynamicTerrainBase::EnableSmoothTransitionFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::EnableSmoothTransitionFieldId);

const OSG::BitVector  DynamicTerrainBase::ShowBoundingBoxesFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::ShowBoundingBoxesFieldId);

const OSG::BitVector  DynamicTerrainBase::ShowTransitionRegionsFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::ShowTransitionRegionsFieldId);

const OSG::BitVector  DynamicTerrainBase::DisableUpdateFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::DisableUpdateFieldId);

const OSG::BitVector  DynamicTerrainBase::CpuVertexProgramFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::CpuVertexProgramFieldId);

const OSG::BitVector  DynamicTerrainBase::CpuFragmentProgramFieldMask = 
    (TypeTraits<BitVector>::One << DynamicTerrainBase::CpuFragmentProgramFieldId);

const OSG::BitVector DynamicTerrainBase::MTInfluenceMask = 
    (Inherited::MTInfluenceMask) | 
    (static_cast<BitVector>(0x0) << Inherited::NextFieldId); 


// Field descriptions

/*! \var Int32           DynamicTerrainBase::_sfLevelSize
    
*/
/*! \var ImagePtr        DynamicTerrainBase::_sfHeightData
    
*/
/*! \var Real32          DynamicTerrainBase::_sfHeightDataScale
    
*/
/*! \var Real32          DynamicTerrainBase::_sfHeightDataOffset
    
*/
/*! \var Real32          DynamicTerrainBase::_sfSampleDistance
    
*/
/*! \var ImagePtr        DynamicTerrainBase::_sfTextureData
    
*/
/*! \var TextureChunkPtr DynamicTerrainBase::_sfHeightColorTexture
    
*/
/*! \var Int32           DynamicTerrainBase::_sfSampleUpdateBudget
    
*/
/*! \var bool            DynamicTerrainBase::_sfEnableFrustumCulling
    
*/
/*! \var bool            DynamicTerrainBase::_sfUseGpuRenderer
    
*/
/*! \var bool            DynamicTerrainBase::_sfUseVboExtension
    
*/
/*! \var bool            DynamicTerrainBase::_sfEnableSmoothTransition
    
*/
/*! \var bool            DynamicTerrainBase::_sfShowBoundingBoxes
    
*/
/*! \var bool            DynamicTerrainBase::_sfShowTransitionRegions
    
*/
/*! \var bool            DynamicTerrainBase::_sfDisableUpdate
    
*/
/*! \var std::string     DynamicTerrainBase::_sfCpuVertexProgram
    
*/
/*! \var std::string     DynamicTerrainBase::_sfCpuFragmentProgram
    
*/

//! DynamicTerrain description

FieldDescription *DynamicTerrainBase::_desc[] = 
{
    new FieldDescription(SFInt32::getClassType(), 
                     "levelSize", 
                     LevelSizeFieldId, LevelSizeFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFLevelSize)),
    new FieldDescription(SFImagePtr::getClassType(), 
                     "heightData", 
                     HeightDataFieldId, HeightDataFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFHeightData)),
    new FieldDescription(SFReal32::getClassType(), 
                     "heightDataScale", 
                     HeightDataScaleFieldId, HeightDataScaleFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFHeightDataScale)),
    new FieldDescription(SFReal32::getClassType(), 
                     "heightDataOffset", 
                     HeightDataOffsetFieldId, HeightDataOffsetFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFHeightDataOffset)),
    new FieldDescription(SFReal32::getClassType(), 
                     "sampleDistance", 
                     SampleDistanceFieldId, SampleDistanceFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFSampleDistance)),
    new FieldDescription(SFImagePtr::getClassType(), 
                     "textureData", 
                     TextureDataFieldId, TextureDataFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFTextureData)),
    new FieldDescription(SFTextureChunkPtr::getClassType(), 
                     "heightColorTexture", 
                     HeightColorTextureFieldId, HeightColorTextureFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFHeightColorTexture)),
    new FieldDescription(SFInt32::getClassType(), 
                     "sampleUpdateBudget", 
                     SampleUpdateBudgetFieldId, SampleUpdateBudgetFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFSampleUpdateBudget)),
    new FieldDescription(SFBool::getClassType(), 
                     "enableFrustumCulling", 
                     EnableFrustumCullingFieldId, EnableFrustumCullingFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFEnableFrustumCulling)),
    new FieldDescription(SFBool::getClassType(), 
                     "useGpuRenderer", 
                     UseGpuRendererFieldId, UseGpuRendererFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFUseGpuRenderer)),
    new FieldDescription(SFBool::getClassType(), 
                     "useVboExtension", 
                     UseVboExtensionFieldId, UseVboExtensionFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFUseVboExtension)),
    new FieldDescription(SFBool::getClassType(), 
                     "enableSmoothTransition", 
                     EnableSmoothTransitionFieldId, EnableSmoothTransitionFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFEnableSmoothTransition)),
    new FieldDescription(SFBool::getClassType(), 
                     "showBoundingBoxes", 
                     ShowBoundingBoxesFieldId, ShowBoundingBoxesFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFShowBoundingBoxes)),
    new FieldDescription(SFBool::getClassType(), 
                     "showTransitionRegions", 
                     ShowTransitionRegionsFieldId, ShowTransitionRegionsFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFShowTransitionRegions)),
    new FieldDescription(SFBool::getClassType(), 
                     "disableUpdate", 
                     DisableUpdateFieldId, DisableUpdateFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFDisableUpdate)),
    new FieldDescription(SFString::getClassType(), 
                     "cpuVertexProgram", 
                     CpuVertexProgramFieldId, CpuVertexProgramFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFCpuVertexProgram)),
    new FieldDescription(SFString::getClassType(), 
                     "cpuFragmentProgram", 
                     CpuFragmentProgramFieldId, CpuFragmentProgramFieldMask,
                     false,
                     reinterpret_cast<FieldAccessMethod>(&DynamicTerrainBase::editSFCpuFragmentProgram))
};


FieldContainerType DynamicTerrainBase::_type(
    "DynamicTerrain",
    "MaterialDrawable",
    NULL,
    reinterpret_cast<PrototypeCreateF>(&DynamicTerrainBase::createEmpty),
    DynamicTerrain::initMethod,
    _desc,
    sizeof(_desc));

//OSG_FIELD_CONTAINER_DEF(DynamicTerrainBase, DynamicTerrainPtr)

/*------------------------------ get -----------------------------------*/

FieldContainerType &DynamicTerrainBase::getType(void) 
{
    return _type; 
} 

const FieldContainerType &DynamicTerrainBase::getType(void) const 
{
    return _type;
} 


FieldContainerPtr DynamicTerrainBase::shallowCopy(void) const 
{ 
    DynamicTerrainPtr returnValue; 

    newPtr(returnValue, dynamic_cast<const DynamicTerrain *>(this)); 

    return returnValue; 
}

UInt32 DynamicTerrainBase::getContainerSize(void) const 
{ 
    return sizeof(DynamicTerrain); 
}


#if !defined(OSG_FIXED_MFIELDSYNC)
void DynamicTerrainBase::executeSync(      FieldContainer &other,
                                    const BitVector      &whichField)
{
    this->executeSyncImpl(static_cast<DynamicTerrainBase *>(&other),
                          whichField);
}
#else
void DynamicTerrainBase::executeSync(      FieldContainer &other,
                                    const BitVector      &whichField,                                    const SyncInfo       &sInfo     )
{
    this->executeSyncImpl((DynamicTerrainBase *) &other, whichField, sInfo);
}
void DynamicTerrainBase::execBeginEdit(const BitVector &whichField, 
                                            UInt32     uiAspect,
                                            UInt32     uiContainerSize) 
{
    this->execBeginEditImpl(whichField, uiAspect, uiContainerSize);
}

void DynamicTerrainBase::onDestroyAspect(UInt32 uiId, UInt32 uiAspect)
{
    Inherited::onDestroyAspect(uiId, uiAspect);

}
#endif

/*------------------------- constructors ----------------------------------*/

#ifdef OSG_WIN32_ICL
#pragma warning (disable : 383)
#endif

DynamicTerrainBase::DynamicTerrainBase(void) :
    _sfLevelSize              (Int32(63)), 
    _sfHeightData             (), 
    _sfHeightDataScale        (Real32(1.0)), 
    _sfHeightDataOffset       (Real32(0)), 
    _sfSampleDistance         (Real32(1.0)), 
    _sfTextureData            (), 
    _sfHeightColorTexture     (), 
    _sfSampleUpdateBudget     (), 
    _sfEnableFrustumCulling   (), 
    _sfUseGpuRenderer         (bool(false)), 
    _sfUseVboExtension        (bool(true)), 
    _sfEnableSmoothTransition (), 
    _sfShowBoundingBoxes      (), 
    _sfShowTransitionRegions  (bool(false)), 
    _sfDisableUpdate          (bool(false)), 
    _sfCpuVertexProgram       (), 
    _sfCpuFragmentProgram     (), 
    Inherited() 
{
}

#ifdef OSG_WIN32_ICL
#pragma warning (default : 383)
#endif

DynamicTerrainBase::DynamicTerrainBase(const DynamicTerrainBase &source) :
    _sfLevelSize              (source._sfLevelSize              ), 
    _sfHeightData             (source._sfHeightData             ), 
    _sfHeightDataScale        (source._sfHeightDataScale        ), 
    _sfHeightDataOffset       (source._sfHeightDataOffset       ), 
    _sfSampleDistance         (source._sfSampleDistance         ), 
    _sfTextureData            (source._sfTextureData            ), 
    _sfHeightColorTexture     (source._sfHeightColorTexture     ), 
    _sfSampleUpdateBudget     (source._sfSampleUpdateBudget     ), 
    _sfEnableFrustumCulling   (source._sfEnableFrustumCulling   ), 
    _sfUseGpuRenderer         (source._sfUseGpuRenderer         ), 
    _sfUseVboExtension        (source._sfUseVboExtension        ), 
    _sfEnableSmoothTransition (source._sfEnableSmoothTransition ), 
    _sfShowBoundingBoxes      (source._sfShowBoundingBoxes      ), 
    _sfShowTransitionRegions  (source._sfShowTransitionRegions  ), 
    _sfDisableUpdate          (source._sfDisableUpdate          ), 
    _sfCpuVertexProgram       (source._sfCpuVertexProgram       ), 
    _sfCpuFragmentProgram     (source._sfCpuFragmentProgram     ), 
    Inherited                 (source)
{
}

/*-------------------------- destructors ----------------------------------*/

DynamicTerrainBase::~DynamicTerrainBase(void)
{
}

/*------------------------------ access -----------------------------------*/

UInt32 DynamicTerrainBase::getBinSize(const BitVector &whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);

    if(FieldBits::NoField != (LevelSizeFieldMask & whichField))
    {
        returnValue += _sfLevelSize.getBinSize();
    }

    if(FieldBits::NoField != (HeightDataFieldMask & whichField))
    {
        returnValue += _sfHeightData.getBinSize();
    }

    if(FieldBits::NoField != (HeightDataScaleFieldMask & whichField))
    {
        returnValue += _sfHeightDataScale.getBinSize();
    }

    if(FieldBits::NoField != (HeightDataOffsetFieldMask & whichField))
    {
        returnValue += _sfHeightDataOffset.getBinSize();
    }

    if(FieldBits::NoField != (SampleDistanceFieldMask & whichField))
    {
        returnValue += _sfSampleDistance.getBinSize();
    }

    if(FieldBits::NoField != (TextureDataFieldMask & whichField))
    {
        returnValue += _sfTextureData.getBinSize();
    }

    if(FieldBits::NoField != (HeightColorTextureFieldMask & whichField))
    {
        returnValue += _sfHeightColorTexture.getBinSize();
    }

    if(FieldBits::NoField != (SampleUpdateBudgetFieldMask & whichField))
    {
        returnValue += _sfSampleUpdateBudget.getBinSize();
    }

    if(FieldBits::NoField != (EnableFrustumCullingFieldMask & whichField))
    {
        returnValue += _sfEnableFrustumCulling.getBinSize();
    }

    if(FieldBits::NoField != (UseGpuRendererFieldMask & whichField))
    {
        returnValue += _sfUseGpuRenderer.getBinSize();
    }

    if(FieldBits::NoField != (UseVboExtensionFieldMask & whichField))
    {
        returnValue += _sfUseVboExtension.getBinSize();
    }

    if(FieldBits::NoField != (EnableSmoothTransitionFieldMask & whichField))
    {
        returnValue += _sfEnableSmoothTransition.getBinSize();
    }

    if(FieldBits::NoField != (ShowBoundingBoxesFieldMask & whichField))
    {
        returnValue += _sfShowBoundingBoxes.getBinSize();
    }

    if(FieldBits::NoField != (ShowTransitionRegionsFieldMask & whichField))
    {
        returnValue += _sfShowTransitionRegions.getBinSize();
    }

    if(FieldBits::NoField != (DisableUpdateFieldMask & whichField))
    {
        returnValue += _sfDisableUpdate.getBinSize();
    }

    if(FieldBits::NoField != (CpuVertexProgramFieldMask & whichField))
    {
        returnValue += _sfCpuVertexProgram.getBinSize();
    }

    if(FieldBits::NoField != (CpuFragmentProgramFieldMask & whichField))
    {
        returnValue += _sfCpuFragmentProgram.getBinSize();
    }


    return returnValue;
}

void DynamicTerrainBase::copyToBin(      BinaryDataHandler &pMem,
                                  const BitVector         &whichField)
{
    Inherited::copyToBin(pMem, whichField);

    if(FieldBits::NoField != (LevelSizeFieldMask & whichField))
    {
        _sfLevelSize.copyToBin(pMem);
    }

    if(FieldBits::NoField != (HeightDataFieldMask & whichField))
    {
        _sfHeightData.copyToBin(pMem);
    }

    if(FieldBits::NoField != (HeightDataScaleFieldMask & whichField))
    {
        _sfHeightDataScale.copyToBin(pMem);
    }

    if(FieldBits::NoField != (HeightDataOffsetFieldMask & whichField))
    {
        _sfHeightDataOffset.copyToBin(pMem);
    }

    if(FieldBits::NoField != (SampleDistanceFieldMask & whichField))
    {
        _sfSampleDistance.copyToBin(pMem);
    }

    if(FieldBits::NoField != (TextureDataFieldMask & whichField))
    {
        _sfTextureData.copyToBin(pMem);
    }

    if(FieldBits::NoField != (HeightColorTextureFieldMask & whichField))
    {
        _sfHeightColorTexture.copyToBin(pMem);
    }

    if(FieldBits::NoField != (SampleUpdateBudgetFieldMask & whichField))
    {
        _sfSampleUpdateBudget.copyToBin(pMem);
    }

    if(FieldBits::NoField != (EnableFrustumCullingFieldMask & whichField))
    {
        _sfEnableFrustumCulling.copyToBin(pMem);
    }

    if(FieldBits::NoField != (UseGpuRendererFieldMask & whichField))
    {
        _sfUseGpuRenderer.copyToBin(pMem);
    }

    if(FieldBits::NoField != (UseVboExtensionFieldMask & whichField))
    {
        _sfUseVboExtension.copyToBin(pMem);
    }

    if(FieldBits::NoField != (EnableSmoothTransitionFieldMask & whichField))
    {
        _sfEnableSmoothTransition.copyToBin(pMem);
    }

    if(FieldBits::NoField != (ShowBoundingBoxesFieldMask & whichField))
    {
        _sfShowBoundingBoxes.copyToBin(pMem);
    }

    if(FieldBits::NoField != (ShowTransitionRegionsFieldMask & whichField))
    {
        _sfShowTransitionRegions.copyToBin(pMem);
    }

    if(FieldBits::NoField != (DisableUpdateFieldMask & whichField))
    {
        _sfDisableUpdate.copyToBin(pMem);
    }

    if(FieldBits::NoField != (CpuVertexProgramFieldMask & whichField))
    {
        _sfCpuVertexProgram.copyToBin(pMem);
    }

    if(FieldBits::NoField != (CpuFragmentProgramFieldMask & whichField))
    {
        _sfCpuFragmentProgram.copyToBin(pMem);
    }


}

void DynamicTerrainBase::copyFromBin(      BinaryDataHandler &pMem,
                                    const BitVector    &whichField)
{
    Inherited::copyFromBin(pMem, whichField);

    if(FieldBits::NoField != (LevelSizeFieldMask & whichField))
    {
        _sfLevelSize.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (HeightDataFieldMask & whichField))
    {
        _sfHeightData.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (HeightDataScaleFieldMask & whichField))
    {
        _sfHeightDataScale.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (HeightDataOffsetFieldMask & whichField))
    {
        _sfHeightDataOffset.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (SampleDistanceFieldMask & whichField))
    {
        _sfSampleDistance.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (TextureDataFieldMask & whichField))
    {
        _sfTextureData.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (HeightColorTextureFieldMask & whichField))
    {
        _sfHeightColorTexture.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (SampleUpdateBudgetFieldMask & whichField))
    {
        _sfSampleUpdateBudget.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (EnableFrustumCullingFieldMask & whichField))
    {
        _sfEnableFrustumCulling.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (UseGpuRendererFieldMask & whichField))
    {
        _sfUseGpuRenderer.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (UseVboExtensionFieldMask & whichField))
    {
        _sfUseVboExtension.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (EnableSmoothTransitionFieldMask & whichField))
    {
        _sfEnableSmoothTransition.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (ShowBoundingBoxesFieldMask & whichField))
    {
        _sfShowBoundingBoxes.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (ShowTransitionRegionsFieldMask & whichField))
    {
        _sfShowTransitionRegions.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (DisableUpdateFieldMask & whichField))
    {
        _sfDisableUpdate.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (CpuVertexProgramFieldMask & whichField))
    {
        _sfCpuVertexProgram.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (CpuFragmentProgramFieldMask & whichField))
    {
        _sfCpuFragmentProgram.copyFromBin(pMem);
    }


}

#if !defined(OSG_FIXED_MFIELDSYNC)
void DynamicTerrainBase::executeSyncImpl(      DynamicTerrainBase *pOther,
                                        const BitVector         &whichField)
{

    Inherited::executeSyncImpl(pOther, whichField);

    if(FieldBits::NoField != (LevelSizeFieldMask & whichField))
        _sfLevelSize.syncWith(pOther->_sfLevelSize);

    if(FieldBits::NoField != (HeightDataFieldMask & whichField))
        _sfHeightData.syncWith(pOther->_sfHeightData);

    if(FieldBits::NoField != (HeightDataScaleFieldMask & whichField))
        _sfHeightDataScale.syncWith(pOther->_sfHeightDataScale);

    if(FieldBits::NoField != (HeightDataOffsetFieldMask & whichField))
        _sfHeightDataOffset.syncWith(pOther->_sfHeightDataOffset);

    if(FieldBits::NoField != (SampleDistanceFieldMask & whichField))
        _sfSampleDistance.syncWith(pOther->_sfSampleDistance);

    if(FieldBits::NoField != (TextureDataFieldMask & whichField))
        _sfTextureData.syncWith(pOther->_sfTextureData);

    if(FieldBits::NoField != (HeightColorTextureFieldMask & whichField))
        _sfHeightColorTexture.syncWith(pOther->_sfHeightColorTexture);

    if(FieldBits::NoField != (SampleUpdateBudgetFieldMask & whichField))
        _sfSampleUpdateBudget.syncWith(pOther->_sfSampleUpdateBudget);

    if(FieldBits::NoField != (EnableFrustumCullingFieldMask & whichField))
        _sfEnableFrustumCulling.syncWith(pOther->_sfEnableFrustumCulling);

    if(FieldBits::NoField != (UseGpuRendererFieldMask & whichField))
        _sfUseGpuRenderer.syncWith(pOther->_sfUseGpuRenderer);

    if(FieldBits::NoField != (UseVboExtensionFieldMask & whichField))
        _sfUseVboExtension.syncWith(pOther->_sfUseVboExtension);

    if(FieldBits::NoField != (EnableSmoothTransitionFieldMask & whichField))
        _sfEnableSmoothTransition.syncWith(pOther->_sfEnableSmoothTransition);

    if(FieldBits::NoField != (ShowBoundingBoxesFieldMask & whichField))
        _sfShowBoundingBoxes.syncWith(pOther->_sfShowBoundingBoxes);

    if(FieldBits::NoField != (ShowTransitionRegionsFieldMask & whichField))
        _sfShowTransitionRegions.syncWith(pOther->_sfShowTransitionRegions);

    if(FieldBits::NoField != (DisableUpdateFieldMask & whichField))
        _sfDisableUpdate.syncWith(pOther->_sfDisableUpdate);

    if(FieldBits::NoField != (CpuVertexProgramFieldMask & whichField))
        _sfCpuVertexProgram.syncWith(pOther->_sfCpuVertexProgram);

    if(FieldBits::NoField != (CpuFragmentProgramFieldMask & whichField))
        _sfCpuFragmentProgram.syncWith(pOther->_sfCpuFragmentProgram);


}
#else
void DynamicTerrainBase::executeSyncImpl(      DynamicTerrainBase *pOther,
                                        const BitVector         &whichField,
                                        const SyncInfo          &sInfo      )
{

    Inherited::executeSyncImpl(pOther, whichField, sInfo);

    if(FieldBits::NoField != (LevelSizeFieldMask & whichField))
        _sfLevelSize.syncWith(pOther->_sfLevelSize);

    if(FieldBits::NoField != (HeightDataFieldMask & whichField))
        _sfHeightData.syncWith(pOther->_sfHeightData);

    if(FieldBits::NoField != (HeightDataScaleFieldMask & whichField))
        _sfHeightDataScale.syncWith(pOther->_sfHeightDataScale);

    if(FieldBits::NoField != (HeightDataOffsetFieldMask & whichField))
        _sfHeightDataOffset.syncWith(pOther->_sfHeightDataOffset);

    if(FieldBits::NoField != (SampleDistanceFieldMask & whichField))
        _sfSampleDistance.syncWith(pOther->_sfSampleDistance);

    if(FieldBits::NoField != (TextureDataFieldMask & whichField))
        _sfTextureData.syncWith(pOther->_sfTextureData);

    if(FieldBits::NoField != (HeightColorTextureFieldMask & whichField))
        _sfHeightColorTexture.syncWith(pOther->_sfHeightColorTexture);

    if(FieldBits::NoField != (SampleUpdateBudgetFieldMask & whichField))
        _sfSampleUpdateBudget.syncWith(pOther->_sfSampleUpdateBudget);

    if(FieldBits::NoField != (EnableFrustumCullingFieldMask & whichField))
        _sfEnableFrustumCulling.syncWith(pOther->_sfEnableFrustumCulling);

    if(FieldBits::NoField != (UseGpuRendererFieldMask & whichField))
        _sfUseGpuRenderer.syncWith(pOther->_sfUseGpuRenderer);

    if(FieldBits::NoField != (UseVboExtensionFieldMask & whichField))
        _sfUseVboExtension.syncWith(pOther->_sfUseVboExtension);

    if(FieldBits::NoField != (EnableSmoothTransitionFieldMask & whichField))
        _sfEnableSmoothTransition.syncWith(pOther->_sfEnableSmoothTransition);

    if(FieldBits::NoField != (ShowBoundingBoxesFieldMask & whichField))
        _sfShowBoundingBoxes.syncWith(pOther->_sfShowBoundingBoxes);

    if(FieldBits::NoField != (ShowTransitionRegionsFieldMask & whichField))
        _sfShowTransitionRegions.syncWith(pOther->_sfShowTransitionRegions);

    if(FieldBits::NoField != (DisableUpdateFieldMask & whichField))
        _sfDisableUpdate.syncWith(pOther->_sfDisableUpdate);

    if(FieldBits::NoField != (CpuVertexProgramFieldMask & whichField))
        _sfCpuVertexProgram.syncWith(pOther->_sfCpuVertexProgram);

    if(FieldBits::NoField != (CpuFragmentProgramFieldMask & whichField))
        _sfCpuFragmentProgram.syncWith(pOther->_sfCpuFragmentProgram);



}

void DynamicTerrainBase::execBeginEditImpl (const BitVector &whichField, 
                                                 UInt32     uiAspect,
                                                 UInt32     uiContainerSize)
{
    Inherited::execBeginEditImpl(whichField, uiAspect, uiContainerSize);

}
#endif



#if !defined(OSG_DO_DOC) || defined(OSG_DOC_DEV)
DataType FieldDataTraits<DynamicTerrainPtr>::_type("DynamicTerrainPtr", "MaterialDrawablePtr");
#endif



/*------------------------------------------------------------------------*/
/*                              cvs id's                                  */

#ifdef OSG_SGI_CC
#pragma set woff 1174
#endif

#ifdef OSG_LINUX_ICC
#pragma warning( disable : 177 )
#endif

namespace
{
    static Char8 cvsid_cpp       [] = "@(#)$Id: FCBaseTemplate_cpp.h,v 1.49 2008/06/09 07:30:44 vossg Exp $";
    static Char8 cvsid_hpp       [] = OSGDYNAMICTERRAINBASE_HEADER_CVSID;
    static Char8 cvsid_inl       [] = OSGDYNAMICTERRAINBASE_INLINE_CVSID;

    static Char8 cvsid_fields_hpp[] = OSGDYNAMICTERRAINFIELDS_HEADER_CVSID;
}

OSG_END_NAMESPACE

