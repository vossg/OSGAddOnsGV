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

template<typename DescT> 
const Int32 RTCacheKDBase<DescT>::otherAxis[3][2] = { {1,2}, {0,2}, {0,1} };

template<typename DescT> inline
RTCacheKDBase<DescT>::BBoxEdge::BBoxEdge(void)
{
}

template<typename DescT> inline
RTCacheKDBase<DescT>::BBoxEdge::BBoxEdge(Real32 rT, 
                                         Int32  iPrimId, 
                                         bool   bStarting) 
{
    _rT        = rT;
    _iPrimId   = iPrimId;
    _eEdgeType = bStarting ? START : END;
}

template<typename DescT> inline
bool RTCacheKDBase<DescT>::BBoxEdge::operator<(const BBoxEdge &rhs) const 
{
    if(_rT == rhs._rT)
    {
        return Int32(_eEdgeType) < Int32(rhs._eEdgeType);
    }
    else 
    {
        return _rT < rhs._rT;
    }
}



template<typename DescT> inline
RTCacheKDBase<DescT>::RTCacheKDBase(void) :
     Inherited       (    ),
    _mfPrimitives    (    ),
    _iIsectCost      (80  ),
    _iTravCost       (1   ),
    _fEmptyBonus     (0.5f),
    _iMaxPrims       (1   ),
    _iMaxDepth       (-1  )
{
}

template<typename DescT> inline
RTCacheKDBase<DescT>::RTCacheKDBase(const RTCacheKDBase &source):
     Inherited       (source),
    _mfPrimitives    (source._mfPrimitives),
    _iIsectCost      (source._iIsectCost  ),
    _iTravCost       (source._iTravCost   ),
    _fEmptyBonus     (source._fEmptyBonus ),
    _iMaxPrims       (source._iMaxPrims   ),
    _iMaxDepth       (source._iMaxDepth   )
{
}

template<typename DescT> inline
RTCacheKDBase<DescT>::~RTCacheKDBase(void)
{
}

template<typename DescT> inline
UInt32 RTCacheKDBase<DescT>::getBinSize(ConstFieldMaskArg  whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);

    if(FieldBits::NoField != (PrimIdxStoreFieldMask & whichField))
    {
        returnValue += _mfPrimitives.getBinSize();
    }

    return returnValue;
}

template<typename DescT> inline
void RTCacheKDBase<DescT>::copyToBin(BinaryDataHandler &pMem,
                                     ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

    if(FieldBits::NoField != (PrimIdxStoreFieldMask & whichField))
    {
        _mfPrimitives.copyToBin(pMem);
    }
}

template<typename DescT> inline
void RTCacheKDBase<DescT>::copyFromBin(BinaryDataHandler &pMem,
                                       ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

    if(FieldBits::NoField != (PrimIdxStoreFieldMask & whichField))
    {
        _mfPrimitives.copyFromBin(pMem);
    }
}


OSG_ABSTR_FIELD_CONTAINER_TMPL_DEF  (RTCacheKDBase, DescT)
//OSG_RC_GET_STATIC_TYPE_INL_TMPL_DEF   (DynFieldAttachment, AttachmentDescT)
OSG_RC_GET_STATIC_TYPE_ID_INL_TMPL_DEF(RTCacheKDBase, DescT)


template<typename DescT> inline
void RTCacheKDBase<DescT>::execSync(      RTCacheKDBase      *pFrom,
                                          ConstFieldMaskArg   whichField,
                                          AspectOffsetStore  &oOffsets,
                                          ConstFieldMaskArg   syncMode  ,
                                    const UInt32              uiSyncInfo)
{
    Inherited::execSync(pFrom, whichField, oOffsets, syncMode, uiSyncInfo);
}




template<typename DescT> inline
EditFieldHandlePtr RTCacheKDBase<DescT>::editHandlePrimIdxStore(void)
{
    MFRTCachePrimIdxStore::EditHandlePtr returnValue(
        new  MFRTCachePrimIdxStore::EditHandle(
             &_mfPrimitives, 
             this->getType().getFieldDesc(PrimIdxStoreFieldId),
             this));

    Inherited::editMField(PrimIdxStoreFieldMask, _mfPrimitives);

    return returnValue;
}

template<typename DescT> inline
GetFieldHandlePtr RTCacheKDBase<DescT>::getHandlePrimIdxStore(void) const
{
    MFRTCachePrimIdxStore::GetHandlePtr returnValue(
        new  MFRTCachePrimIdxStore::GetHandle(
             &_mfPrimitives, 
             this->getType().getFieldDesc(PrimIdxStoreFieldId),
             const_cast<Self *>(this)));

    return returnValue;
}










template<typename DescT> inline
RTKDNode *RTCacheKDBase<DescT>::buildInternalTree(void)
{
    BBoxStore vBounds;

    initAccel(vBounds);

    BBoxEdgeStore vEdgeStore[3];

    for(Int32 i = 0; i < 3; ++i)
        vEdgeStore[i].resize(2 * this->_mfTriangleAcc.size());

    IndexStore vPrimStore0;
    IndexStore vPrimStore1;

    vPrimStore0.resize(this->_mfTriangleAcc.size());

    vPrimStore1.resize((_iMaxDepth + 1) * this->_mfTriangleAcc.size());

    for(UInt32 i = 0; i < this->_mfTriangleAcc.size(); ++i)
        vPrimStore0[i] = i;


    RTKDNode *pKDTree = buildTree(this->_sfBoundingVolume.getValue(), 
                                         vBounds, 
                                         vPrimStore0.begin     (),
                                  this->_mfTriangleAcc.size(), 
                                        _iMaxDepth, 
                                         vEdgeStore,
                                         vPrimStore0.begin     (), 
                                         vPrimStore1.begin     ());

    return pKDTree;
}

template<typename DescT> inline
void RTCacheKDBase<DescT>::initAccel(BBoxStore &vBounds)
{
    Inherited::initAccel(vBounds);

    if(_iMaxDepth <= 0)
    {
        _iMaxDepth = osgRound2Int(8 + 1.3f * 
                                  osgLog2Int(
                                      float(this->_mfTriangleAcc.size())));
    }
}

template<typename DescT> inline
RTKDNode *RTCacheKDBase<DescT>::buildTree(
    const BoxVolume     &nodeBounds,
    const BBoxStore     &vBoundsStore, 
          IndexIterator  currentPrimsIt,
          IndexSize      numCurrentPrims, 
          Int32          iCurrentDepth, 
          BBoxEdgeStore  vEdgeStore[3],
          IndexIterator  itPrimStore0, 
          IndexIterator  itPrimStore1, 
          Int32          iNumBadRefines) 
{
    RTKDNode *returnValue = NULL;

    if(numCurrentPrims <= _iMaxPrims || iCurrentDepth == 0) 
    {
        returnValue = new RTKDNode;

        returnValue->initLeaf( currentPrimsIt, 
                               numCurrentPrims,
                              _mfPrimitives    );

        return returnValue;
    }

    Int32  iCurrAxis     = 0;
    Int32  iBestAxis     = -1;
    Int32  iBestOffset   = -1;
	Real32 fBestCost     = TypeTraits<Real32>::getMax();
    Real32 fOldCost      = _iIsectCost * Real32(numCurrentPrims);

    Vec3f  vDiag         = nodeBounds.getMax() - nodeBounds.getMin();

    Real32 fTotalArea    = (2.f * (vDiag[0] * vDiag[1] + 
                                   vDiag[0] * vDiag[2] + 
                                   vDiag[1] * vDiag[2]));

    Real32 fInvTotalArea = 1.f / fTotalArea;


    if(vDiag[0] > vDiag[1] && vDiag[0] > vDiag[2]) 
    {
        iCurrAxis = 0;
    }
    else 
    {
        iCurrAxis = (vDiag[1] > vDiag[2]) ? 1 : 2;
    }

    Int32         iNumRetries = 0;
    IndexIterator primIt;

    do
    {
        primIt = currentPrimsIt;
        
        for(Int32 i = 0; i < numCurrentPrims; ++i) 
        {
            Int32 iPrimId = *primIt++;
            
            const BoxVolume &bbox = vBoundsStore[iPrimId];
            
            vEdgeStore[iCurrAxis][2 * i] =
                BBoxEdge(bbox.getMin()[iCurrAxis], iPrimId, true);

            vEdgeStore[iCurrAxis][2 * i + 1] =
                BBoxEdge(bbox.getMax()[iCurrAxis], iPrimId, false);
        }
        
        BBoxEdgeStoreIt edgesBegin = vEdgeStore[iCurrAxis].begin();
        BBoxEdgeStoreIt edgesEnd   = vEdgeStore[iCurrAxis].begin();
        
        edgesEnd += 2 * numCurrentPrims;
        
        std::sort(edgesBegin, edgesEnd);
               
        Int32 iNumBelow = 0;
        Int32 iNumAbove = numCurrentPrims;
        
        for(Int32 i = 0; i < 2 * numCurrentPrims; ++i) 
        {
            if(vEdgeStore[iCurrAxis][i]._eEdgeType == BBoxEdge::END) 
                --iNumAbove;
            
            Real32 fEdgeT = vEdgeStore[iCurrAxis][i]._rT;
            
            if(fEdgeT > nodeBounds.getMin()[iCurrAxis] &&
               fEdgeT < nodeBounds.getMax()[iCurrAxis]  ) 
            {
                Int32 otherAxis0  = otherAxis[iCurrAxis][0];
                Int32 otherAxis1  = otherAxis[iCurrAxis][1];

                Real32 rDiagSum   = vDiag[otherAxis0] + vDiag[otherAxis1];
                Real32 rDiagProd  = vDiag[otherAxis0] * vDiag[otherAxis1];

                Real32 fBelowArea = 2 * (rDiagProd +
                                         (fEdgeT - 
                                          nodeBounds.getMin()[iCurrAxis]) *
                                         rDiagSum);

                Real32 fAboveArea = 2 * (rDiagProd +
                                         (nodeBounds.getMax()[iCurrAxis] - 
                                          fEdgeT) *
                                         rDiagSum);

                Real32 fBelowProp = fBelowArea * fInvTotalArea;
                Real32 fAboveProp = fAboveArea * fInvTotalArea;

                Real32 eb = (iNumAbove == 0 || iNumBelow == 0) ? 
                    (1.f - _fEmptyBonus) : 1.f;

                Real32 fCurrentCost = 
                    _iTravCost + _iIsectCost * eb * (fBelowProp * iNumBelow + 
                                                     fAboveProp * iNumAbove);

                if(fCurrentCost < fBestCost)  
                {
                    fBestCost   = fCurrentCost;
                    iBestAxis   = iCurrAxis;
                    iBestOffset = i;
                }
            }

            if(vEdgeStore[iCurrAxis][i]._eEdgeType == BBoxEdge::START) 
                ++iNumBelow;
        }
        
        OSG_ASSERT(iNumBelow == numCurrentPrims && iNumAbove == 0); 
        
        if(iBestAxis == -1 && iNumRetries < 2) 
        {
            ++iNumRetries;
            iCurrAxis = (iCurrAxis + 1) % 3;
        }
    } while(iBestAxis == -1 && iNumRetries < 2);

    if(fBestCost > fOldCost) 
        ++iNumBadRefines;

    if((fBestCost > 4.f * fOldCost && numCurrentPrims < 16) ||
       iBestAxis == -1 || iNumBadRefines == 3) 
    {
        returnValue = new RTKDNode;
        
        returnValue->initLeaf( currentPrimsIt, 
                               numCurrentPrims,
                              _mfPrimitives    );
        
        return returnValue;
    }


    Int32 n0 = 0;
    Int32 n1 = 0;

    primIt = itPrimStore0;

    for(Int32 i = 0; i < iBestOffset; ++i)
    {
        if(vEdgeStore[iBestAxis][i]._eEdgeType == BBoxEdge::START)
        {
            *primIt++ = vEdgeStore[iBestAxis][i]._iPrimId;
            ++n0;
        }
    }

    primIt = itPrimStore1;

    for(Int32 i = iBestOffset + 1; i < 2 * numCurrentPrims; ++i)
    {
        if(vEdgeStore[iBestAxis][i]._eEdgeType == BBoxEdge::END)
        {
            *primIt++ = vEdgeStore[iBestAxis][i]._iPrimId;
            ++n1;
        }
    }

    Real tsplit = vEdgeStore[iBestAxis][iBestOffset]._rT;

    returnValue = new RTKDNode;

    returnValue->initInterior(iBestAxis, tsplit);


    Pnt3f newMin = nodeBounds.getMin();
    Pnt3f newMax = nodeBounds.getMax();

    newMax[iBestAxis] = tsplit;

    BoxVolume bounds0(newMin, newMax);



    newMin = nodeBounds.getMin();
    newMax = nodeBounds.getMax();

    newMin[iBestAxis] = tsplit;

    BoxVolume bounds1(newMin, newMax);



    RTKDNode *pBelowChild = buildTree(bounds0,
                                      vBoundsStore, 
                                      itPrimStore0, 
                                      n0, 
                                      iCurrentDepth - 1, 
                                      vEdgeStore,
                                      itPrimStore0, 
                                      itPrimStore1 + numCurrentPrims, 
                                      iNumBadRefines);


    RTKDNode *pAboveChild = buildTree(bounds1, 
                                      vBoundsStore,
                                      itPrimStore1, 
                                      n1, 
                                      iCurrentDepth - 1, 
                                      vEdgeStore,
                                      itPrimStore0, 
                                      itPrimStore1 + numCurrentPrims, 
                                      iNumBadRefines);

    returnValue->setAboveChild(pAboveChild);
    returnValue->setBelowChild(pBelowChild);

    return returnValue;
}

OSG_END_NAMESPACE
