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
void RTCacheKD<DescT>::RTCacheKDNode::initLeaf(IndexIterator  primNums, 
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
void RTCacheKD<DescT>::RTCacheKDNode::initInterior(UInt32 uiAxis, 
                                                   Real32 fSplitPos)
{
    _fSplitPos = fSplitPos;

    _uiFlags &= ~3;
    _uiFlags |= uiAxis;
}

template<typename DescT> inline
void RTCacheKD<DescT>::RTCacheKDNode::initLeaf(RTKDNode     *pNode,  
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
void RTCacheKD<DescT>::RTCacheKDNode::initInterior(RTKDNode *pNode)
{
    _fSplitPos = pNode->getSplitPos();

    _uiFlags &= ~3;
    _uiFlags |= pNode->getSplitAxis();
}

template<typename DescT> inline
Real32 RTCacheKD<DescT>::RTCacheKDNode::getSplitPos(void) const
{ 
    return _fSplitPos; 
}

template<typename DescT> inline
UInt32 RTCacheKD<DescT>::RTCacheKDNode::getNumPrimitives(void) const 
{ 
    return _uiNumPrims >> 2; 
}

template<typename DescT> inline
UInt32 RTCacheKD<DescT>::RTCacheKDNode::getSplitAxis(void) const 
{ 
    return _uiFlags & 3; 
}

template<typename DescT> inline
bool RTCacheKD<DescT>::RTCacheKDNode::isLeaf(void) const 
{
    return (_uiFlags & 3) == 3; 
}





template<typename DescT> inline
RTCacheKD<DescT>::RTCacheKD(void) :
     Inherited    (    ),
    _vKDTree      (    ),
     nAllocedNodes(   0),
     nextFreeNode (   0)
{
}

template<typename DescT> inline
RTCacheKD<DescT>::~RTCacheKD(void)
{
}


template<typename DescT> inline
void RTCacheKD<DescT>::buildStructure(void)
{
	nextFreeNode = nAllocedNodes = 0;

    RTKDNode *pKDTree = Inherited::buildInternalTree();

    flattenTree(NULL, pKDTree);

    delete pKDTree;
}

template<typename DescT> inline
void RTCacheKD<DescT>::intersect(RTRayPacket &oRay, 
                                 RTHitPacket &oHit,
                                 KDElemStack &sKDToDoStack,
                                 UInt32       uiCacheId)
{
#if 0
    for(UInt32 i = 0; i < _vTriangleAcc.size(); ++i)
    {
        _vTriangleAcc[i].intersect(oRay, oHit, uiCacheId);
    }
#else

#if 0
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


    union
    {
        const RTCacheKDNode *node;
              unsigned int   addr;
    };

    node = &(_vKDTree[1]);

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

            union
            {
                const RTCacheKDNode *nearChild;
                      unsigned int   nearAddr;
            };

            union
            {
                const RTCacheKDNode *farChild;
                      unsigned int   farAddr;
            };

			bool belowFirst = oRay.getOrigin()[axis] <= node->getSplitPos();

			if(belowFirst == true) 
            {
				nearAddr = addr + node->_uiAboveChild;
				farAddr  = addr + node->_uiAboveChild + sizeof(RTCacheKDNode);
			}
			else 
            {
				nearAddr = addr + node->_uiAboveChild + sizeof(RTCacheKDNode);
				farAddr  = addr + node->_uiAboveChild;
			}

			if(tplane > tmax || tplane < 0)
            {
				node = nearChild;
            }
			else if(tplane < tmin)
            {
				node = farChild;
            }
			else 
            {
                KDStackElem otherNode;

				otherNode.node = farChild;
				otherNode.tmin = tplane;
				otherNode.tmax = tmax;

				node = nearChild;
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
                    oRay, oHit, uiCacheId);
			}
			else 
            {
				std::vector<UInt32> &prims = 
                    this->_vPrimitives[node->_pPrimitiveIdx];

				for(u_int i = 0; i < nPrimitives; ++i) 
                {
                    this->_vTriangleAcc[prims[i]].intersect(oRay, 
                                                            oHit,
                                                            uiCacheId);
				}
			}

            if(oHit.getDist() <= tmax)
                break;

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


    union
    {
        const RTCacheKDNode *node;
              unsigned int   addr;
    };

	node = &(_vKDTree[1]);

	while(node != NULL) 
    {
		while(!node->isLeaf()) 
        {
			int axis = node->getSplitAxis();

			float tplane = (node->getSplitPos() - oRay.getOrigin()[axis]) *
				invDir[axis];

            union
            {
                const RTCacheKDNode *nearChild;
                      unsigned int   nearAddr;
            };

            union
            {
                const RTCacheKDNode *farChild;
                      unsigned int   farAddr;
            };

			bool belowFirst = oRay.getOrigin()[axis] <= node->getSplitPos();

			if(belowFirst == true) 
            {
				nearAddr = addr + node->_uiAboveChild;
				farAddr  = addr + node->_uiAboveChild + sizeof(RTCacheKDNode);
			}
			else 
            {
				nearAddr = addr + node->_uiAboveChild + sizeof(RTCacheKDNode);
				farAddr  = addr + node->_uiAboveChild;
			}

			if(tplane >= tmax || tplane < 0)
            {
				node = nearChild;
            }
			else if(tplane <= tmin)
            {
				node = farChild;
            }
			else 
            {
                KDStackElem otherNode;

				otherNode.node = farChild;
				otherNode.tmin = tplane;
				otherNode.tmax = tmax;

				node = nearChild;
				tmax = tplane;

                _sKDToDoStack.push_back(otherNode);
			}
		}

        UInt32 nPrimitives = node->getNumPrimitives();

        if(nPrimitives == 1) 
        {
            this->_vTriangleAcc[node->_uiSinglePrimitive].intersect(
                oRay, oHit, uiCacheId);
        }
        else 
        {
            std::vector<UInt32> &prims = 
                this->_vPrimitives[node->_pPrimitiveIdx];
            
            for(u_int i = 0; i < nPrimitives; ++i) 
            {
                this->_vTriangleAcc[prims[i]].intersect(oRay, 
                                                        oHit,
                                                        uiCacheId);
            }
        }

        if(oHit.getDist() <= tmax)
            break;

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
#endif
#endif
}

template<typename DescT> inline
void RTCacheKD<DescT>::intersect(RTRaySIMDPacket &oRay, 
                                 RTHitSIMDPacket &oHit,
                                 KDElemStack     &sKDToDoStack,
                                 UInt32           uiCacheId,
                                 UInt32          *uiActive     )
{
#if 1
    for(UInt32 i = 0; i < this->_vTriangleAcc.size(); ++i)
    {
        this->_vTriangleAcc[i].intersect(oRay, oHit, uiCacheId, uiActive);
    }
#else
#endif
}

template<typename DescT> inline
void RTCacheKD<DescT>::flattenTree(RTKDNode *pLeft, 
                                   RTKDNode *pRight)
{
	if(nAllocedNodes <= nextFreeNode + 2) 
    {
		int nAlloc = osgMax(2 * _vKDTree.size(), 512u);

        _vKDTree.resize(nAlloc);

		nAllocedNodes = nAlloc;
	}

    if(pLeft == NULL)
    {
        if(pRight->isLeave() == true)
        {
            ++nextFreeNode;

            _vKDTree[nextFreeNode].initLeaf(pRight, this->_vPrimitives);

            return;
        }
        else
        {
            ++nextFreeNode;

            _vKDTree[nextFreeNode].initInterior(pRight);
            _vKDTree[nextFreeNode]._uiAboveChild = sizeof(RTCacheKDNode);

            ++nextFreeNode;

            flattenTree(pRight->getBelowChild(), pRight->getAboveChild());
        }
    }
    else
    {
        UInt32 uiLeft  = nextFreeNode++;
        UInt32 uiRight = nextFreeNode++;


        if(pLeft->isLeave() == false)
        {
            _vKDTree[uiLeft].initInterior(pLeft);
            _vKDTree[uiLeft]._uiAboveChild = 2 * sizeof(RTCacheKDNode);

            flattenTree(pLeft->getBelowChild(), pLeft->getAboveChild());
        }
        else
        {
            _vKDTree[uiLeft].initLeaf(pLeft, this->_vPrimitives);
        }


        if(pRight->isLeave() == false)
        {
            _vKDTree[uiRight].initInterior(pRight);

            _vKDTree[uiRight]._uiAboveChild = 
                (nextFreeNode - uiRight) * sizeof(RTCacheKDNode);

            flattenTree(pRight->getBelowChild(), pRight->getAboveChild());
        }
        else
        {
            _vKDTree[uiRight].initLeaf(pRight, this->_vPrimitives);
        }
    }
}


OSG_END_NAMESPACE
