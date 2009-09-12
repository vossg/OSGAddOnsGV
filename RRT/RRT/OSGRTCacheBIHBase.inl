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
RTCacheBIHBase<DescT>::RTCacheBIHBase(void) :
     Inherited       (    )
{
}

template<typename DescT> inline
RTCacheBIHBase<DescT>::RTCacheBIHBase(const RTCacheBIHBase &source) :
    Inherited(source)
{
}

template<typename DescT> inline
RTCacheBIHBase<DescT>::~RTCacheBIHBase(void)
{
}

template<typename DescT> inline
UInt32 RTCacheBIHBase<DescT>::getBinSize(ConstFieldMaskArg  whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);

    return returnValue;
}

template<typename DescT> inline
void RTCacheBIHBase<DescT>::copyToBin(BinaryDataHandler &pMem,
                                      ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);
}

template<typename DescT> inline
void RTCacheBIHBase<DescT>::copyFromBin(BinaryDataHandler &pMem,
                                        ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);
}


OSG_ABSTR_FIELD_CONTAINER_TMPL_DEF  (RTCacheBIHBase, DescT)
//OSG_RC_GET_STATIC_TYPE_INL_TMPL_DEF   (DynFieldAttachment, AttachmentDescT)
OSG_RC_GET_STATIC_TYPE_ID_INL_TMPL_DEF(RTCacheBIHBase, DescT)


template<typename DescT> inline
void RTCacheBIHBase<DescT>::execSync(      RTCacheBIHBase     *pFrom,
                                           ConstFieldMaskArg   whichField,
                                           AspectOffsetStore  &oOffsets,
                                           ConstFieldMaskArg   syncMode  ,
                                     const UInt32              uiSyncInfo)
{
    Inherited::execSync(pFrom, whichField, oOffsets, syncMode, uiSyncInfo);
}


template<typename DescT> inline
RTBIHNode *RTCacheBIHBase<DescT>::buildInternalTree(void)
{
    BBoxStore vBounds;

    initAccel(vBounds);

    return NULL;
}


template<typename DescT> inline
void RTCacheBIHBase<DescT>::initAccel(BBoxStore &vBounds)
{
    Inherited::initAccel(vBounds);
}



OSG_END_NAMESPACE
