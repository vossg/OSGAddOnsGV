/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2003 by the OpenSG Forum                   *
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

OSG_BEGIN_NAMESPACE

template<typename DescT> inline
RTCacheBIH<DescT>::BIHSIMDStackElem::BIHSIMDStackElem(void)
{
}

template<typename DescT> inline
RTCacheBIH<DescT>::BIHSIMDStackElem::BIHSIMDStackElem(
    const BIHSIMDStackElem &source)
{
    tmin4   = source.tmin4;
    tmax4   = source.tmax4;

    node = source.node;
}

template<typename DescT> inline
void RTCacheBIH<DescT>::BIHSIMDStackElem::operator =(
    const BIHSIMDStackElem &source)
{
    tmin4   = source.tmin4;
    tmax4   = source.tmax4;

    node = source.node;
}

template<typename DescT> inline
void RTCacheBIH<DescT>::buildStructure(void)
{
    RTBIHNode *pBIHTree = Inherited::buildInternalTree();
}

template<typename DescT> inline
void RTCacheBIH<DescT>::intersect(BasicRayPacket     &oRay, 
                                  RTHitPacket        &oHit,
                                  BIHElemStack       &sBIHToDoStack,
                                  UInt32              uiCacheId   )
{
#if 1
    fprintf(stderr, "BIH Intersect float %d\n", this->_mfTriangleAcc.size());

    for(UInt32 i = 0; i < this->_mfTriangleAcc.size(); ++i)
    {
        this->_mfTriangleAcc[i].intersect(oRay, oHit, uiCacheId);
    }
#else
#endif
}

template<typename DescT> inline
void RTCacheBIH<DescT>::intersect(BasicSIMDRayPacket &oRay, 
                                  RTHitSIMDPacket    &oHit,
                                  BIHElemStack       &sBIHToDoStack,
                                  UInt32              uiCacheId   ,
                                  UInt32             *uiActive    )
{
#if 1
    fprintf(stderr, "BIH Intersect simd %d\n", this->_mfTriangleAcc.size());

    for(UInt32 i = 0; i < this->_mfTriangleAcc.size(); ++i)
    {
        for(UInt32 j = 0; j < 4; ++j)
        {
            this->_mfTriangleAcc[i].intersect(oRay, oHit, uiCacheId, j);
        }
    }
#else
#endif
}

template<typename DescT> inline
void RTCacheBIH<DescT>::intersectSingle(BasicSIMDRayPacket &oRay, 
                                        RTHitSIMDPacket    &oHit,
                                        BIHElemStack       &sBIHToDoStack,
                                        UInt32              uiCacheId   ,
                                        UInt32             *uiActive    )
{
}

OSG_FIELD_CONTAINER_TMPL_NO_TYPE_DEF  (RTCacheBIH, DescT)
//OSG_RC_GET_STATIC_TYPE_INL_TMPL_DEF   (DynFieldAttachment, AttachmentDescT)
OSG_RC_GET_STATIC_TYPE_ID_INL_TMPL_DEF(RTCacheBIH, DescT)

template<typename DescT> inline
RTCacheBIH<DescT>::RTCacheBIH(void) :
    Inherited()
{
}

template<typename DescT> inline
RTCacheBIH<DescT>::RTCacheBIH(const RTCacheBIH &source) :
    Inherited(source)
{
}

template<typename DescT> inline
RTCacheBIH<DescT>::~RTCacheBIH(void)
{
}

template<typename DescT> inline
UInt32 RTCacheBIH<DescT>::getBinSize(ConstFieldMaskArg  whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);

/*
    if(FieldBits::NoField != (KDTreeFieldMask & whichField))
    {
        returnValue += _mfKDTree.getBinSize();
    }
 */

    return returnValue;
}

template<typename DescT> inline
void RTCacheBIH<DescT>::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

/*
    if(FieldBits::NoField != (KDTreeFieldMask & whichField))
    {
        _mfKDTree.copyToBin(pMem);
    }
 */
}

template<typename DescT> inline
void RTCacheBIH<DescT>::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

/*
    if(FieldBits::NoField != (KDTreeFieldMask & whichField))
    {
        _mfKDTree.copyFromBin(pMem);
    }
 */
}


#ifdef OSG_MT_CPTR_ASPECT
template<typename DescT> inline
typename RTCacheBIH<DescT>::ObjPtr 
    RTCacheBIH<DescT>::createAspectCopy(void) const
{
    ObjPtr returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const Self *>(this));

    return returnValue;
}
#endif

template<typename DescT> inline
EditFieldHandlePtr RTCacheBIH<DescT>::editHandleBIHTree(void)
{
#if 0
    typename MFRTCacheKDNode::EditHandlePtr returnValue(
        new typename MFRTCacheKDNode::EditHandle(
             &_mfKDTree, 
             this->getType().getFieldDesc(KDTreeFieldId)));

    this->editMField(KDTreeFieldMask, _mfKDTree);

    return returnValue;
#else
    return EditFieldHandlePtr();
#endif
}

template<typename DescT> inline
GetFieldHandlePtr RTCacheBIH<DescT>::getHandleBIHTree (void) const
{
#if 0
    typename MFRTCacheKDNode::GetHandlePtr returnValue(
        new typename MFRTCacheKDNode::GetHandle(
             &_mfKDTree, 
             this->getType().getFieldDesc(KDTreeFieldId)));

    return returnValue;
#else
    return GetFieldHandlePtr();
#endif
}

#ifdef OSG_MT_CPTR_ASPECT
template<typename DescT> inline
void RTCacheBIH<DescT>::execSyncV(
          FieldContainer     &oFrom,
          ConstFieldMaskArg   whichField,
          AspectOffsetStore  &oOffsets,
          ConstFieldMaskArg   syncMode  ,
    const UInt32              uiSyncInfo)
{
    this->execSync(static_cast<RTCacheBIH *>(&oFrom),
                   whichField,
                   oOffsets,
                   syncMode,
                   uiSyncInfo);
}

template<typename DescT> inline
void RTCacheBIH<DescT>::execSync (
          RTCacheBIH         *pFrom,
          ConstFieldMaskArg   whichField,
          AspectOffsetStore  &oOffsets,
          ConstFieldMaskArg   syncMode  ,
    const UInt32              uiSyncInfo)
{
    Inherited::execSync(pFrom, whichField, oOffsets, syncMode, uiSyncInfo);
}
#endif

OSG_END_NAMESPACE
