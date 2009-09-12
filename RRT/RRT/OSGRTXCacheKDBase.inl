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
const Int32 RTXCacheKDBase<DescT>::otherAxis[3][2] = { {1,2}, {0,2}, {0,1} };

template<typename DescT> inline
RTXCacheKDBase<DescT>::BBoxEdge::BBoxEdge(void)
{
}

template<typename DescT> inline
RTXCacheKDBase<DescT>::BBoxEdge::BBoxEdge(Real32 rT, 
                                          Int32  iPrimId, 
                                          bool   bStarting) 
{
    _rT        = rT;
    _iPrimId   = iPrimId;
    _eEdgeType = bStarting ? START : END;
}

template<typename DescT> inline
bool RTXCacheKDBase<DescT>::BBoxEdge::operator<(const BBoxEdge &rhs) const 
{
    if(_rT == rhs._rT)
    {
        return (Int32) _eEdgeType < (Int32) rhs._eEdgeType;
    }
    else 
    {
        return _rT < rhs._rT;
    }
}

template<typename DescT> inline
RTXCacheKDBase<DescT>::RTXCacheKDBase(void) :
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
RTXCacheKDBase<DescT>::RTXCacheKDBase(const RTXCacheKDBase &source):
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
RTXCacheKDBase<DescT>::~RTXCacheKDBase(void)
{
}

template<typename DescT> inline
UInt32 RTXCacheKDBase<DescT>::getBinSize(ConstFieldMaskArg  whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);

    if(FieldBits::NoField != (PrimIdxStoreFieldMask & whichField))
    {
        returnValue += _mfPrimitives.getBinSize();
    }

    return returnValue;
}

template<typename DescT> inline
void RTXCacheKDBase<DescT>::copyToBin(BinaryDataHandler &pMem,
                                      ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

    if(FieldBits::NoField != (PrimIdxStoreFieldMask & whichField))
    {
        _mfPrimitives.copyToBin(pMem);
    }
}

template<typename DescT> inline
void RTXCacheKDBase<DescT>::copyFromBin(BinaryDataHandler &pMem,
                                        ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

    if(FieldBits::NoField != (PrimIdxStoreFieldMask & whichField))
    {
        _mfPrimitives.copyFromBin(pMem);
    }
}


OSG_ABSTR_FIELD_CONTAINER_TMPL_DEF  (RTXCacheKDBase, DescT)
//OSG_RC_GET_STATIC_TYPE_INL_TMPL_DEF   (DynFieldAttachment, AttachmentDescT)
OSG_RC_GET_STATIC_TYPE_ID_INL_TMPL_DEF(RTXCacheKDBase, DescT)

template<typename DescT> inline
void RTXCacheKDBase<DescT>::execSync(      RTXCacheKDBase     *pFrom,
                                           ConstFieldMaskArg   whichField,
                                           AspectOffsetStore  &oOffsets,
                                           ConstFieldMaskArg   syncMode  ,
                                     const UInt32              uiSyncInfo)
{
    Inherited::execSync(pFrom, whichField, oOffsets, syncMode, uiSyncInfo);
}




template<typename DescT> inline
EditFieldHandlePtr RTXCacheKDBase<DescT>::editHandlePrimIdxStore(void)
{
    MFRTCachePrimIdxStore::EditHandlePtr returnValue(
        new  MFRTCachePrimIdxStore::EditHandle(
             &_mfPrimitives, 
             this->getType().getFieldDesc(PrimIdxStoreFieldId)));

    Inherited::editMField(PrimIdxStoreFieldMask, _mfPrimitives);

    return returnValue;
}

template<typename DescT> inline
GetFieldHandlePtr RTXCacheKDBase<DescT>::getHandlePrimIdxStore(void) const
{
    MFRTCachePrimIdxStore::GetHandlePtr returnValue(
        new  MFRTCachePrimIdxStore::GetHandle(
             &_mfPrimitives, 
             this->getType().getFieldDesc(PrimIdxStoreFieldId)));

    return returnValue;
}

template<typename DescT> inline
void RTXCacheKDBase<DescT>::initAccel(BBoxStore &vBounds)
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
RTXKDNode *RTXCacheKDBase<DescT>::buildInternalTree(void)
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

    nodeCounter = 1;

    RTXKDNode *pKDTree = buildTree(this->_sfBoundingVolume.getValue(), 
                                          vBounds, 
                                          vPrimStore0.begin     (),
                                   this->_mfTriangleAcc.size(), 
                                         _iMaxDepth, 
                                          vEdgeStore,
                                          vPrimStore0.begin     (), 
                                          vPrimStore1.begin     ());
 

    boxVolume_t* pRootBox = new boxVolume_t;

    pKDTree->initRope();


    for(int i=0 ; i < 3 ; ++i)
    {
        pRootBox->min[i] = this->_sfBoundingVolume.getValue().getMin()[i];
        pRootBox->max[i] = this->_sfBoundingVolume.getValue().getMax()[i];
    }
 

    if(!pKDTree->isLeaf())
    {
        boxVolume_t* pBelowBox = new boxVolume_t;
        boxVolume_t* pAboveBox = new boxVolume_t;

        *pBelowBox = *pRootBox;
        *pAboveBox = *pRootBox;

     
        Real32 splitPos = pKDTree->getSplitPos();
        UInt32 axis = pKDTree->getSplitAxis();
          
        pBelowBox->max[axis] = splitPos;

	     
        pKDTree->getBelowChild()->initRope();
        pKDTree->getBelowChild()->setRope(
            pKDTree->getAboveChild()->getNodeId(),(UInt32)axis*2);

        processNode(pKDTree->getBelowChild(), pBelowBox); 

        pAboveBox->min[axis] = splitPos;
      
        pKDTree->getAboveChild()->initRope();
        pKDTree->getAboveChild()->setRope(
            pKDTree->getBelowChild()->getNodeId(), (UInt32)axis*2+1);

        processNode(pKDTree->getAboveChild(), pAboveBox);

        delete pBelowBox;
        delete pAboveBox;
    }

    delete pRootBox;

    return pKDTree;
}

template<typename DescT> inline
RTXKDNode *RTXCacheKDBase<DescT>::buildTree(
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
    RTXKDNode *returnValue = NULL;

    if(numCurrentPrims <= _iMaxPrims || iCurrentDepth == 0) 
    {
        returnValue = new RTXKDNode;

        returnValue->initLeaf( currentPrimsIt, 
                               numCurrentPrims,
                              _mfPrimitives,
                               nodeCounter);

        nodeCounter++;
        
        return returnValue;
    }

    Int32  iCurrAxis     = 0;
    Int32  iBestAxis     = -1;
    Int32  iBestOffset   = -1;
    Real32 fBestCost     = INFINITY;
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
        returnValue = new RTXKDNode;
        
        returnValue->initLeaf( currentPrimsIt, 
                               numCurrentPrims,
                              _mfPrimitives,
                               nodeCounter);

        nodeCounter++;
        
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

    returnValue = new RTXKDNode;

    returnValue->initInterior(iBestAxis, tsplit, nodeCounter);

    nodeCounter++;

    Pnt3f newMin = nodeBounds.getMin();
    Pnt3f newMax = nodeBounds.getMax();

    newMax[iBestAxis] = tsplit;

    BoxVolume bounds0(newMin, newMax);


    newMin = nodeBounds.getMin();
    newMax = nodeBounds.getMax();

    newMin[iBestAxis] = tsplit;

    BoxVolume bounds1(newMin, newMax);


    RTXKDNode *pBelowChild = buildTree(bounds0,
                                       vBoundsStore, 
                                       itPrimStore0, 
                                       n0, 
                                       iCurrentDepth - 1, 
                                       vEdgeStore,
                                       itPrimStore0, 
                                       itPrimStore1 + numCurrentPrims, 
                                       iNumBadRefines);


    RTXKDNode *pAboveChild = buildTree(bounds1, 
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

template<typename DescT> inline
void RTXCacheKDBase<DescT>::processNode(RTXKDNode   *pParent,
                                        boxVolume_t *pParentBox)
{
    if(pParent->isLeaf())
    {
        pParent->setBoundingBox(pParentBox);
        return;
    }
    else
    {
      // for(UInt8 i = 0 ; i<6 ; ++i){optimize(parent, i, box);}  // only for single rays

        boundingBoxSide below, above;

        const unsigned int axis = pParent->getSplitAxis();

        if(axis == 0) 
        {
            below = right;
            above = left;
        }
        else if(axis == 1)  
        {
            below = top;
            above = bottom;
        }
        else if(axis == 2)
        {
            below = front;
            above = back;
        }
        
        boxVolume_t *pBelowBox = new boxVolume_t;
        boxVolume_t *pAboveBox = new boxVolume_t;

        *pBelowBox = *pParentBox;
        *pAboveBox = *pParentBox;

        Real32 splitPos = pParent->getSplitPos();

        for(UInt8 i = 0 ; i<6 ; ++i)
        {
            pParent->getBelowChild()->setRope(pParent->getRope(i), i);
            
        }
        pParent->getBelowChild()->setRope(
            pParent->getAboveChild()->getNodeId(), below);
     
        pBelowBox->max[axis] = splitPos;
     
        processNode(pParent->getBelowChild(), pBelowBox);
      
        for(UInt8 i = 0 ; i<6 ; ++i)
        {
            pParent->getAboveChild()->setRope(pParent->getRope(i), i);
        }

        pParent->getAboveChild()->setRope(
            pParent->getBelowChild()->getNodeId(), above);
     
        pAboveBox->min[axis] = splitPos;
 
        processNode(pParent->getAboveChild(), pAboveBox);
	
        delete pBelowBox;
        delete pAboveBox;
    }
}


template<typename DescT> inline
void RTXCacheKDBase<DescT>::optimize(RTXKDNode   *rope,
                                     UInt8        face,
                                     boxVolume_t *box)
{
    while(!rope->isLeaf())
    {
        UInt8 axis = rope->getSplitAxis();
        
        if(face%2 == 0) //  right, top, front: 0,2,4
        {
            face=face>>1;
            
            if(face == axis || rope->getSplitPos() < box->max[face])
                rope=rope->getBelowChild();
        }
        else if(face%2 != 0) // left, bottom, back: 1,3,5
        {
            face=face>>1;
            
            if(face == axis || rope->getSplitPos() > box->min[face])
                rope = rope->getAboveChild();
        }
    }
}

OSG_END_NAMESPACE
