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
void RTCacheKDVer1<DescT>::RTCacheKDNode::initLeaf(IndexIterator  primNums, 
                                                   IndexSize      np,
                                                   PrimIdxStore  &vStore  )
{
    _uiNumPrims  = np << 2;
    _uiFlags    |= 3;

    if(np == 0)
    {
        _uiSinglePrimitive = 0;
    }
    else if(np == 1)
    {
        _uiSinglePrimitive = *primNums;
    }
    else 
    {
        _pPrimitiveIdx = vStore.size();

        std::vector<UInt32> tmpVec;
        
        vStore.push_back(tmpVec);
        
        vStore.back().reserve(np);

        for(UInt32 i = 0; i < np; ++i)
            vStore.back().push_back(*primNums++);
    }
}

template<typename DescT> inline
void RTCacheKDVer1<DescT>::RTCacheKDNode::initInterior(UInt32 uiAxis, 
                                                       Real32 fSplitPos)
{
    _fSplitPos = fSplitPos;

    _uiFlags &= ~3;
    _uiFlags |= uiAxis;
}

template<typename DescT> inline
void RTCacheKDVer1<DescT>::RTCacheKDNode::initLeaf(RTKDNode     *pNode,  
                                                   PrimIdxStore &vStore)
{
    std::vector<UInt32> &vPrimitives = vStore[pNode->getPrimitiveIdx()];

    _uiNumPrims  = vPrimitives.size() << 2;
    _uiFlags    |= 3;
    
    if(vPrimitives.size() == 0)
    {
        _uiSinglePrimitive = 0;
    }
    else if(vPrimitives.size() == 1)
    {
        _uiSinglePrimitive = vPrimitives[0];
    }
    else 
    {
        _pPrimitiveIdx = pNode->getPrimitiveIdx();
    }
}

template<typename DescT> inline
void RTCacheKDVer1<DescT>::RTCacheKDNode::initInterior(RTKDNode *pNode)
{
    _fSplitPos = pNode->getSplitPos();

    _uiFlags &= ~3;
    _uiFlags |= pNode->getSplitAxis();
}

template<typename DescT> inline
Real32 RTCacheKDVer1<DescT>::RTCacheKDNode::getSplitPos(void) const
{ 
    return _fSplitPos; 
}

template<typename DescT> inline
UInt32 RTCacheKDVer1<DescT>::RTCacheKDNode::getNumPrimitives(void) const 
{ 
    return _uiNumPrims >> 2; 
}

template<typename DescT> inline
UInt32 RTCacheKDVer1<DescT>::RTCacheKDNode::getSplitAxis(void) const 
{ 
    return _uiFlags & 3; 
}

template<typename DescT> inline
bool RTCacheKDVer1<DescT>::RTCacheKDNode::isLeaf(void) const 
{
    return (_uiFlags & 3) == 3; 
}





template<typename DescT> inline
RTCacheKDVer1<DescT>::RTCacheKDVer1(void) :
     Inherited    (    ),
    _vKDTree      (    ),
     nAllocedNodes(   0),
     nextFreeNode (   0)
{
}

template<typename DescT> inline
RTCacheKDVer1<DescT>::~RTCacheKDVer1(void)
{
}


template<typename DescT> inline
void RTCacheKDVer1<DescT>::buildStructure(void)
{
#if 0
    BBoxStore vBounds;

    initAccel(vBounds);

	nextFreeNode = nAllocedNodes = 0;

    BBoxEdgeStore vEdgeStore[3];

	for(Int32 i = 0; i < 3; ++i)
		vEdgeStore[i].resize(2 * this->_vTriangleAcc.size());

    IndexStore vPrimStore0;
    IndexStore vPrimStore1;

    vPrimStore0.resize(this->_vTriangleAcc.size());
    vPrimStore1.resize((this->_iMaxDepth + 1) * this->_vTriangleAcc.size());

	for(UInt32 i = 0; i < this->_vTriangleAcc.size(); ++i)
		vPrimStore0[i] = i;

	buildTree(0, 
              this->_bBoundingVolume, 
              vBounds, 
              vPrimStore0.begin(),
	          this->_vTriangleAcc.size(), 
              this->_iMaxDepth, 
              vEdgeStore,
			  vPrimStore0.begin(), 
              vPrimStore1.begin());
#else
	nextFreeNode = nAllocedNodes = 0;

    RTKDNode *pKDTree = Inherited::buildInternalTree();

    flattenTree(pKDTree, 0);

    delete pKDTree;
#endif
}

template<typename DescT> inline
void RTCacheKDVer1<DescT>::intersect(RTRayPacket &oRay, 
                                     RTHitPacket &oHit,
                                     KDElemStack &sKDToDoStack)
{
#if 0
    for(UInt32 i = 0; i < _vTriangleAcc.size(); ++i)
    {
        _vTriangleAcc[i].intersect(oRay, oHit);
    }
#else

	float tmin, tmax;

    Line lineRay(oRay.getOrigin(), oRay.getDir());

/*
	if (!bounds.IntersectP(ray, &tmin, &tmax))
		return false;
 */

    std::vector<KDStackElem  > _sKDToDoStack;
    _sKDToDoStack.reserve(256);


    _sKDToDoStack.clear  (   );

    if(this->_bBoundingVolume.intersect(lineRay, tmin, tmax) == false)
        return;

	Vec3f invDir(1.f/oRay.getDir().x(), 
                 1.f/oRay.getDir().y(), 
                 1.f/oRay.getDir().z());


	const RTCacheKDNode *node = &(_vKDTree[0]);

	while(node != NULL) 
    {
		if(oHit.getDist() < tmin) 
        {
            break;
        }

		if(!node->isLeaf()) 
        {
			int axis = node->getSplitAxis();

			float tplane = (node->getSplitPos() - oRay.getOrigin()[axis]) *
				invDir[axis];

			const RTCacheKDNode *firstChild;
            const RTCacheKDNode *secondChild;

			bool belowFirst = oRay.getOrigin()[axis] <= node->getSplitPos();

			if(belowFirst == true) 
            {
				firstChild = node + 1;
				secondChild = &(_vKDTree[node->_uiAboveChild]);
			}
			else 
            {
				firstChild = &(_vKDTree[node->_uiAboveChild]);
				secondChild = node + 1;
			}

			if(tplane > tmax || tplane < 0)
            {
				node = firstChild;
            }
			else if(tplane < tmin)
            {
				node = secondChild;
            }
			else 
            {
                KDStackElem otherNode;

				otherNode.node = secondChild;
				otherNode.tmin = tplane;
				otherNode.tmax = tmax;

				node = firstChild;
				tmax = tplane;

                _sKDToDoStack.push_back(otherNode);
			}
		}
		else 
        {
			UInt32 nPrimitives = node->getNumPrimitives();

			if(nPrimitives == 1) 
            {
                this->_vTriangleAcc[node->_uiSinglePrimitive].intersect(
                    oRay, oHit);
			}
			else 
            {
				std::vector<UInt32> &prims = 
                    this->_vPrimitives[node->_pPrimitiveIdx];

				for(u_int i = 0; i < nPrimitives; ++i) 
                {
                    this->_vTriangleAcc[prims[i]].intersect(oRay, oHit);
				}
			}

			if(_sKDToDoStack.size() > 0) 
            {

                KDStackElem otherNode = _sKDToDoStack.back();

				node = otherNode.node;
				tmin = otherNode.tmin;
				tmax = otherNode.tmax;

                _sKDToDoStack.pop_back();
			}
			else
            {
				break;
            }
		}
	}
#endif
}

template<typename DescT> inline
void RTCacheKDVer1<DescT>::intersect(RTRaySIMDPacket &oRay, 
                                     RTHitSIMDPacket &oHit,
                                     KDElemStack     &sKDToDoStack)
{
#if 0
    for(UInt32 i = 0; i < _vTriangleAcc.size(); ++i)
    {
        _vTriangleAcc[i].intersect(oRay, oHit);
    }
#else

	float tmin, tmax;

    Line lineRay(oRay.getOrigin(), oRay.getDir());

    std::vector<KDStackElem  > _sKDToDoStack;
    _sKDToDoStack.reserve(256);


    _sKDToDoStack.clear  (   );

    if(this->_bBoundingVolume.intersect(lineRay, tmin, tmax) == false)
        return;

	Vec3f invDir(1.f/oRay.getDir().x(), 
                 1.f/oRay.getDir().y(), 
                 1.f/oRay.getDir().z());


	const RTCacheKDNode *node = &(_vKDTree[0]);

	while(node != NULL) 
    {
		// Bail out if we found a hit closer than the current node
		if(oHit.getDist() < tmin) 
        {
            break;
        }

		if(!node->isLeaf()) 
        {
			int axis = node->getSplitAxis();

			float tplane = (node->getSplitPos() - oRay.getOrigin()[axis]) *
				invDir[axis];

			const RTCacheKDNode *firstChild;
            const RTCacheKDNode *secondChild;

			bool belowFirst = oRay.getOrigin()[axis] <= node->getSplitPos();

			if(belowFirst == true) 
            {
				firstChild = node + 1;
				secondChild = &(_vKDTree[node->_uiAboveChild]);
			}
			else 
            {
				firstChild = &(_vKDTree[node->_uiAboveChild]);
				secondChild = node + 1;
			}

			if(tplane > tmax || tplane < 0)
            {
				node = firstChild;
            }
			else if(tplane < tmin)
            {
				node = secondChild;
            }
			else 
            {
                KDStackElem otherNode;

				otherNode.node = secondChild;
				otherNode.tmin = tplane;
				otherNode.tmax = tmax;

				node = firstChild;
				tmax = tplane;

                _sKDToDoStack.push_back(otherNode);
			}
		}
		else 
        {
			UInt32 nPrimitives = node->getNumPrimitives();

			if(nPrimitives == 1) 
            {
                this->_vTriangleAcc[node->_uiSinglePrimitive].intersect(
                    oRay, oHit);
			}
			else 
            {
				std::vector<UInt32> &prims = 
                    this->_vPrimitives[node->_pPrimitiveIdx];

				for(u_int i = 0; i < nPrimitives; ++i) 
                {
                    this->_vTriangleAcc[prims[i]].intersect(oRay, oHit);
				}
			}

			if(_sKDToDoStack.size() > 0) 
            {

                KDStackElem otherNode = _sKDToDoStack.back();

				node = otherNode.node;
				tmin = otherNode.tmin;
				tmax = otherNode.tmax;

                _sKDToDoStack.pop_back();
			}
			else
            {
				break;
            }
		}
	}
#endif
}

template<typename DescT> inline
void RTCacheKDVer1<DescT>::flattenTree(RTKDNode *pRoot, Int32 iCurrentKDNode)
{
	if(nextFreeNode == nAllocedNodes) 
    {
		int nAlloc = osgMax(2 * _vKDTree.size(), 512u);

        _vKDTree.resize(nAlloc);

		nAllocedNodes = nAlloc;
	}

	++nextFreeNode;

    if(pRoot->isLeave() == true)
    {
        _vKDTree[iCurrentKDNode].initLeaf(pRoot, this->_vPrimitives);
        return;
    }

	_vKDTree[iCurrentKDNode].initInterior(pRoot);

    flattenTree(pRoot->getBelowChild(), iCurrentKDNode + 1);

    _vKDTree[iCurrentKDNode]._uiAboveChild = nextFreeNode;
        
    flattenTree(pRoot->getAboveChild(), 
                _vKDTree[iCurrentKDNode]._uiAboveChild);
}

template<typename DescT> inline
void RTCacheKDVer1<DescT>::buildTree(      Int32          iCurrentKDNode,
                                     const BoxVolume     &nodeBounds,
                                     const BBoxStore     &vBoundsStore, 
                                           IndexIterator  currentPrimsIt,
                                           IndexSize      numCurrentPrims, 
                                           Int32          iCurrentDepth, 
                                           BBoxEdgeStore  vEdgeStore[3],
                                           IndexIterator  itPrimStore0, 
                                           IndexIterator  itPrimStore1, 
                                           Int32          iNumBadRefines ) 
{
	if(nextFreeNode == nAllocedNodes) 
    {
		int nAlloc = osgMax(2 * _vKDTree.size(), 512u);

        _vKDTree.resize(nAlloc);

		nAllocedNodes = nAlloc;
	}

	++nextFreeNode;

	if(numCurrentPrims <= this->_iMaxPrims || iCurrentDepth == 0) 
    {
		_vKDTree[iCurrentKDNode].initLeaf(currentPrimsIt, 
                                          numCurrentPrims,
                                          this->_vPrimitives);
		return;
	}

	Int32  iCurrAxis     = 0;
	Int32  iBestAxis     = -1;
    Int32  iBestOffset   = -1;
	Real32 fBestCost     = INFINITY;
	Real32 fOldCost      = this->_iIsectCost * Real32(numCurrentPrims);


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

	Int32         retries = 0;
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
            
            float fEdgeT = vEdgeStore[iCurrAxis][i]._rT;
            
            if(fEdgeT > nodeBounds.getMin()[iCurrAxis] &&
               fEdgeT < nodeBounds.getMax()[iCurrAxis]) 
            {
                Int32 otherAxis0  = this->otherAxis[iCurrAxis][0];
                Int32 otherAxis1  = this->otherAxis[iCurrAxis][1];

                Real32 rDiagSum   = vDiag[otherAxis0] + vDiag[otherAxis1];
                Real32 rDiagProd  = vDiag[otherAxis0] * vDiag[otherAxis1];

                Real32 fBelowArea = 2 * (rDiagProd +
                                         (fEdgeT - 
                                          nodeBounds.getMin()[iCurrAxis]) *
                                         rDiagProd);
                
                Real32 fAboveArea = 2 * (rDiagProd +
                                         (nodeBounds.getMax()[iCurrAxis] -
                                          fEdgeT) *
                                         rDiagSum);

                Real32 fBelowProp = fBelowArea * fInvTotalArea;
                Real32 fAboveProp = fAboveArea * fInvTotalArea;

                Real32 eb = (iNumAbove == 0 || iNumBelow == 0) ? 
                    (1.f - this->_fEmptyBonus) : 1.f;

                Real32 fCurrentCost = 
                    this->_iTravCost + 
                    this->_iIsectCost * eb * (fBelowProp * iNumBelow + 
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
        
        if(iBestAxis == -1 && retries < 2) 
        {
            ++retries;
            iCurrAxis = (iCurrAxis+1) % 3;
        }

    } while(iBestAxis == -1 && retries < 2);

	if(fBestCost > fOldCost) 
        ++iNumBadRefines;

	if((fBestCost > 4.f * fOldCost && numCurrentPrims < 16) ||
       iBestAxis == -1 || iNumBadRefines == 3) 
    {
		_vKDTree[iCurrentKDNode].initLeaf(currentPrimsIt, 
                                          numCurrentPrims,
                                          this->_vPrimitives);
		return;
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

	_vKDTree[iCurrentKDNode].initInterior(iBestAxis, tsplit);


    Pnt3f newMin = nodeBounds.getMin();
    Pnt3f newMax = nodeBounds.getMax();

    newMax[iBestAxis] = tsplit;

	BoxVolume bounds0(newMin, newMax);


    newMin = nodeBounds.getMin();
    newMax = nodeBounds.getMax();

    newMin[iBestAxis] = tsplit;

    BoxVolume bounds1(newMin, newMax);

	buildTree(iCurrentKDNode + 1, 
              bounds0,
              vBoundsStore, 
              itPrimStore0, 
              n0, 
              iCurrentDepth - 1, 
              vEdgeStore,
              itPrimStore0, 
              itPrimStore1 + numCurrentPrims, 
              iNumBadRefines);

	_vKDTree[iCurrentKDNode]._uiAboveChild = nextFreeNode;

	buildTree(_vKDTree[iCurrentKDNode]._uiAboveChild, 
              bounds1, 
              vBoundsStore,
              itPrimStore1, 
              n1, 
              iCurrentDepth - 1, 
              vEdgeStore,
              itPrimStore0, 
              itPrimStore1 + numCurrentPrims, 
              iNumBadRefines);
}

OSG_END_NAMESPACE
