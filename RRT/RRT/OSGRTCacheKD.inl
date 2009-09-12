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
RTCacheKD<DescT>::KDSIMDStackElem::KDSIMDStackElem(void)
{
}

template<typename DescT> inline
RTCacheKD<DescT>::KDSIMDStackElem::KDSIMDStackElem(
    const KDSIMDStackElem &source)
{
    tmin4[0] = source.tmin4[0];
    tmin4[1] = source.tmin4[1];
    tmin4[2] = source.tmin4[2];
    tmin4[3] = source.tmin4[3];

    tmax4[0] = source.tmax4[0];
    tmax4[1] = source.tmax4[1];
    tmax4[2] = source.tmax4[2];
    tmax4[3] = source.tmax4[3];

    node = source.node;
}

template<typename DescT> inline
void RTCacheKD<DescT>::KDSIMDStackElem::operator =(
    const KDSIMDStackElem &source)
{
    tmin4[0] = source.tmin4[0];
    tmin4[1] = source.tmin4[1];
    tmin4[2] = source.tmin4[2];
    tmin4[3] = source.tmin4[3];

    tmax4[0] = source.tmax4[0];
    tmax4[1] = source.tmax4[1];
    tmax4[2] = source.tmax4[2];
    tmax4[3] = source.tmax4[3];

    node = source.node;
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

    sKDToDoStack.clear  (   );

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

                sKDToDoStack.push_back(otherNode);
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

			if(sKDToDoStack.size() > 0) 
            {

                KDStackElem otherNode = sKDToDoStack.back();

				node = otherNode.node;
				tmin = otherNode.tmin;
				tmax = otherNode.tmax;

                sKDToDoStack.pop_back();
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

    sKDToDoStack.clear  (   );

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

                sKDToDoStack.push_back(otherNode);
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

        if(sKDToDoStack.size() > 0) 
        {
            KDStackElem otherNode = sKDToDoStack.back();

            node = otherNode.node;
            tmin = otherNode.tmin;
            tmax = otherNode.tmax;
            
            sKDToDoStack.pop_back();
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
void RTCacheKD<DescT>::intersectSingle(RTRaySIMDPacket &oRay, 
                                       RTHitSIMDPacket &oHit,
                                       KDElemStack     &sKDToDoStack,
                                       UInt32           uiCacheId,
                                       UInt32          *uiActive     )
{
    for(UInt32 iRay = 0; iRay < 4; ++iRay)
    {
        float tmin, tmax;

        Line lineRay(oRay.getOriginPnt(), oRay.getDirVec(iRay));
        
/*
	if (!bounds.IntersectP(ray, &tmin, &tmax))
		return false;
 */

        sKDToDoStack.clear  (   );

        if(this->_bBoundingVolume.intersect(lineRay, tmin, tmax) == false)
            continue;
        
        Vec3f invDir(1.f/oRay.getDirVec(iRay).x(), 
                     1.f/oRay.getDirVec(iRay).y(), 
                     1.f/oRay.getDirVec(iRay).z());

        
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

                float tplane = 
                    (node->getSplitPos() - oRay.getOriginPnt()[axis]) *
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

                bool belowFirst = 
                    oRay.getOriginPnt()[axis] <= node->getSplitPos();

                if(belowFirst == true) 
                {
                    nearAddr = addr + node->_uiAboveChild;

                    farAddr  = 
                        addr + node->_uiAboveChild + sizeof(RTCacheKDNode);
                }
                else 
                {
                    nearAddr = 
                        addr + node->_uiAboveChild + sizeof(RTCacheKDNode);

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
                    KDSIMDStackElem otherNode;
                    
                    otherNode.node        = farChild;
                    otherNode.tmin4[iRay] = tplane;
                    otherNode.tmax4[iRay] = tmax;
                    
                    node = nearChild;
                    tmax = tplane;

                    sKDToDoStack.push_back(otherNode);
                }
            }
            
            UInt32 nPrimitives = node->getNumPrimitives();

            if(nPrimitives == 1) 
            {
                this->_vTriangleAcc[node->_uiSinglePrimitive].intersect(
                    oRay, oHit, uiCacheId, iRay);
            }
            else 
            {
                std::vector<UInt32> &prims = 
                    this->_vPrimitives[node->_pPrimitiveIdx];
                
                for(u_int i = 0; i < nPrimitives; ++i) 
                {
                    this->_vTriangleAcc[prims[i]].intersect(oRay, 
                                                            oHit,
                                                            uiCacheId,
                                                            iRay);
                }
            }
            
            if(oHit.getDist(iRay) <= tmax)
                break;
            
            if(sKDToDoStack.size() > 0) 
            {
                KDSIMDStackElem otherNode = sKDToDoStack.back();
                
                node = otherNode.node;
                tmin = otherNode.tmin4[iRay];
                tmax = otherNode.tmax4[iRay];
                
                sKDToDoStack.pop_back();
            }
            else
            {
                break;
            }
        }
    }
}

template<typename DescT> inline
void RTCacheKD<DescT>::intersect(RTRaySIMDPacket &oRay, 
                                 RTHitSIMDPacket &oHit,
                                 KDElemStack     &sKDToDoStack,
                                 UInt32           uiCacheId,
                                 UInt32          *uiActive     )
{
#if 0
    for(UInt32 i = 0; i < this->_vTriangleAcc.size(); ++i)
    {
        for(UInt32 j = 0; j < 4; ++j)
        {
            this->_vTriangleAcc[i].intersect(oRay, oHit, uiCacheId, j);
        }
    }
#else

#if 0
    for(UInt32 iRay = 0; iRay < 4; ++iRay)
    {
        float tmin, tmax;

        Line lineRay(oRay.getOriginPnt(), oRay.getDirVec(iRay));
        
/*
	if (!bounds.IntersectP(ray, &tmin, &tmax))
		return false;
 */

        sKDToDoStack.clear  (   );

        if(this->_bBoundingVolume.intersect(lineRay, tmin, tmax) == false)
            continue;
        
        Vec3f invDir(1.f/oRay.getDirVec(iRay).x(), 
                     1.f/oRay.getDirVec(iRay).y(), 
                     1.f/oRay.getDirVec(iRay).z());

        
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

                float tplane = 
                    (node->getSplitPos() - oRay.getOriginPnt()[axis]) *
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

                bool belowFirst = 
                    oRay.getOriginPnt()[axis] <= node->getSplitPos();

                if(belowFirst == true) 
                {
                    nearAddr = addr + node->_uiAboveChild;

                    farAddr  = 
                        addr + node->_uiAboveChild + sizeof(RTCacheKDNode);
                }
                else 
                {
                    nearAddr = 
                        addr + node->_uiAboveChild + sizeof(RTCacheKDNode);

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
                    KDSIMDStackElem otherNode;
                    
                    otherNode.node        = farChild;
                    otherNode.tmin4[iRay] = tplane;
                    otherNode.tmax4[iRay] = tmax;
                    
                    node = nearChild;
                    tmax = tplane;

                    sKDToDoStack.push_back(otherNode);
                }
            }
            
            UInt32 nPrimitives = node->getNumPrimitives();

            if(nPrimitives == 1) 
            {
                this->_vTriangleAcc[node->_uiSinglePrimitive].intersect(
                    oRay, oHit, uiCacheId, iRay);
            }
            else 
            {
                std::vector<UInt32> &prims = 
                    this->_vPrimitives[node->_pPrimitiveIdx];
                
                for(u_int i = 0; i < nPrimitives; ++i) 
                {
                    this->_vTriangleAcc[prims[i]].intersect(oRay, 
                                                            oHit,
                                                            uiCacheId,
                                                            iRay);
                }
            }
            
            if(oHit.getDist(iRay) <= tmax)
                break;
            
            if(sKDToDoStack.size() > 0) 
            {
                KDSIMDStackElem otherNode = sKDToDoStack.back();
                
                node = otherNode.node;
                tmin = otherNode.tmin4[iRay];
                tmax = otherNode.tmax4[iRay];
                
                sKDToDoStack.pop_back();
            }
            else
            {
                break;
            }
        }
    }
#else


    float tmin[4], tmax[4];
    bool  active[4];

    Line lineRay0(oRay.getOriginPnt(), oRay.getDirVec(0)); 
    Line lineRay1(oRay.getOriginPnt(), oRay.getDirVec(1)); 
    Line lineRay2(oRay.getOriginPnt(), oRay.getDirVec(2)); 
    Line lineRay3(oRay.getOriginPnt(), oRay.getDirVec(3)); 

      
/*
	if (!bounds.IntersectP(ray, &tmin, &tmax))
		return false;
 */

    sKDToDoStack.clear  (   );

    active[0] = this->_bBoundingVolume.intersect(lineRay0, tmin[0], tmax[0]);
    active[1] = this->_bBoundingVolume.intersect(lineRay1, tmin[1], tmax[1]);
    active[2] = this->_bBoundingVolume.intersect(lineRay2, tmin[2], tmax[2]);
    active[3] = this->_bBoundingVolume.intersect(lineRay3, tmin[3], tmax[3]);
    
    if(active[0] == false && 
       active[1] == false && 
       active[2] == false && 
       active[3] == false)
    {
        return;
    }

    Vec3f vInvDirs[4];

    vInvDirs[0].setValues(1.f / oRay.getDirVec(0)[0],
                          1.f / oRay.getDirVec(0)[1],
                          1.f / oRay.getDirVec(0)[2]);

    vInvDirs[1].setValues(1.f / oRay.getDirVec(1)[0],
                          1.f / oRay.getDirVec(1)[1],
                          1.f / oRay.getDirVec(1)[2]);

    vInvDirs[2].setValues(1.f / oRay.getDirVec(2)[0],
                          1.f / oRay.getDirVec(2)[1],
                          1.f / oRay.getDirVec(2)[2]);

    vInvDirs[3].setValues(1.f / oRay.getDirVec(3)[0],
                          1.f / oRay.getDirVec(3)[1],
                          1.f / oRay.getDirVec(3)[2]);

       
    union
    {
        const RTCacheKDNode *node;
              unsigned int   addr;
    };

    bool sameDir[3];

    sameDir[0] = 
        (oRay.getDirVec(0)[0] >= 0 &&
         oRay.getDirVec(1)[0] >= 0 &&
         oRay.getDirVec(2)[0] >= 0 &&
         oRay.getDirVec(3)[0] >= 0) ||
        (oRay.getDirVec(0)[0] < 0 &&
         oRay.getDirVec(1)[0] < 0 &&
         oRay.getDirVec(2)[0] < 0 &&
         oRay.getDirVec(3)[0] < 0);

    sameDir[1] = 
        (oRay.getDirVec(0)[1] >= 0 &&
         oRay.getDirVec(1)[1] >= 0 &&
         oRay.getDirVec(2)[1] >= 0 &&
         oRay.getDirVec(3)[1] >= 0) ||
        (oRay.getDirVec(0)[1] < 0 &&
         oRay.getDirVec(1)[1] < 0 &&
         oRay.getDirVec(2)[1] < 0 &&
         oRay.getDirVec(3)[1] < 0);

    sameDir[2] = 
        (oRay.getDirVec(0)[2] >= 0 &&
         oRay.getDirVec(1)[2] >= 0 &&
         oRay.getDirVec(2)[2] >= 0 &&
         oRay.getDirVec(3)[2] >= 0) ||
        (oRay.getDirVec(0)[2] < 0 &&
         oRay.getDirVec(1)[2] < 0 &&
         oRay.getDirVec(2)[2] < 0 &&
         oRay.getDirVec(3)[2] < 0);

#if 1
    if(!sameDir[0] || !sameDir[1] || !sameDir[2])
    {
        intersectSingle(oRay, 
                        oHit,
                        sKDToDoStack,
                        uiCacheId,
                        uiActive);
//        fprintf(stderr, "Single\n");
        return;
    }
#endif

//    fprintf(stderr, "Full\n");

    node = &(_vKDTree[1]);

    while(node != NULL) 
    {
        while(!node->isLeaf()) 
        {
            int axis = node->getSplitAxis();
            
            float tplane[4];
            
            tplane[0] = 
                (node->getSplitPos() - oRay.getOriginPnt()[axis]) *
                vInvDirs[0][axis];
            
            tplane[1] = 
                (node->getSplitPos() - oRay.getOriginPnt()[axis]) *
                vInvDirs[1][axis];
            
            tplane[2] = 
                (node->getSplitPos() - oRay.getOriginPnt()[axis]) *
                vInvDirs[2][axis];
            
            tplane[3] = 
                (node->getSplitPos() - oRay.getOriginPnt()[axis]) *
                vInvDirs[3][axis];
            
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

            bool belowFirst = oRay.getDirVec(0)[axis] > 0;

#if 0
            if(sameDir[axis] == false)
            {
                belowFirst = 
                    oRay.getOriginPnt()[axis] <= node->getSplitPos();
            }
#endif

            if(belowFirst == true) 
            {
                nearAddr = addr + node->_uiAboveChild;
                
                farAddr  = 
                    addr + node->_uiAboveChild + sizeof(RTCacheKDNode);
            }
            else 
            {
                nearAddr = 
                    addr + node->_uiAboveChild + sizeof(RTCacheKDNode);
                
                farAddr  = addr + node->_uiAboveChild;
            }
            
            node = nearChild;

            bool cmpN[4];
            bool cmpF[4];

            cmpN[0] = tplane[0] >= tmax[0];// || tplane[0] < 0;
            cmpN[1] = tplane[1] >= tmax[1];// || tplane[1] < 0;
            cmpN[2] = tplane[2] >= tmax[2];// || tplane[2] < 0;
            cmpN[3] = tplane[3] >= tmax[3];// || tplane[3] < 0;
               
            cmpN[0] |= !active[0];
            cmpN[1] |= !active[1];
            cmpN[2] |= !active[2];
            cmpN[3] |= !active[3];

            if(cmpN[0] == true && 
               cmpN[1] == true && 
               cmpN[2] == true && 
               cmpN[3] == true)
            {
                continue;
            }

            node = farChild;

            cmpF[0] = tplane[0] <= tmin[0];
            cmpF[1] = tplane[1] <= tmin[1];
            cmpF[2] = tplane[2] <= tmin[2];
            cmpF[3] = tplane[3] <= tmin[3];

            cmpF[0] |= !active[0];
            cmpF[1] |= !active[1];
            cmpF[2] |= !active[2];
            cmpF[3] |= !active[3];

            if(cmpF[0] == true && 
               cmpF[1] == true && 
               cmpF[2] == true && 
               cmpF[3] == true)
            {
                continue;
            }

            KDSIMDStackElem otherNode;
                
            otherNode.node        = farChild;

            otherNode.tmin4[0] = osgMax(tplane[0], tmin[0]);
            otherNode.tmin4[1] = osgMax(tplane[1], tmin[1]);
            otherNode.tmin4[2] = osgMax(tplane[2], tmin[2]);
            otherNode.tmin4[3] = osgMax(tplane[3], tmin[3]);

            otherNode.tmax4[0] = tmax[0];
            otherNode.tmax4[1] = tmax[1];
            otherNode.tmax4[2] = tmax[2];
            otherNode.tmax4[3] = tmax[3];
                
            otherNode.active[0] = active[0];
            otherNode.active[1] = active[1];
            otherNode.active[2] = active[2];
            otherNode.active[3] = active[3];

            node       = nearChild;

            tmax[0] = osgMin(tplane[0], tmax[0]);
            tmax[1] = osgMin(tplane[1], tmax[1]);
            tmax[2] = osgMin(tplane[2], tmax[2]);
            tmax[3] = osgMin(tplane[3], tmax[3]);

            active[0] = tmin[0] < tmax[0];
            active[1] = tmin[1] < tmax[1];
            active[2] = tmin[2] < tmax[2];
            active[3] = tmin[3] < tmax[3];
            
            sKDToDoStack.push_back(otherNode);
        }
            
        UInt32 nPrimitives = node->getNumPrimitives();
        
        if(nPrimitives == 1) 
        {
            for(UInt32 iRay = 0; iRay < 4; ++iRay)
            {
                if(active[iRay])
                {
                    this->_vTriangleAcc[node->_uiSinglePrimitive].intersect(
                        oRay, oHit, uiCacheId, iRay);
                }
            }
        }
        else 
        {
            std::vector<UInt32> &prims = 
                this->_vPrimitives[node->_pPrimitiveIdx];
            
            for(u_int i = 0; i < nPrimitives; ++i) 
            {
                for(UInt32 iRay = 0; iRay < 4; ++iRay)
                {
                    if(active[iRay])
                    {
                        this->_vTriangleAcc[prims[i]].intersect(oRay, 
                                                                oHit,
                                                                uiCacheId,
                                                                iRay);
                    }
                }
            }
        }
            
        if(oHit.getDist(0) <= tmax[0] &&
           oHit.getDist(1) <= tmax[1] &&
           oHit.getDist(2) <= tmax[2] &&
           oHit.getDist(3) <= tmax[3])
        {
            break;
        }   

        if(sKDToDoStack.size() > 0) 
        {
            KDSIMDStackElem otherNode = sKDToDoStack.back();
            
            node    = otherNode.node;

            tmin[0] = otherNode.tmin4[0];
            tmin[1] = otherNode.tmin4[1];
            tmin[2] = otherNode.tmin4[2];
            tmin[3] = otherNode.tmin4[3];

            tmax[0] = otherNode.tmax4[0];
            tmax[1] = otherNode.tmax4[1];
            tmax[2] = otherNode.tmax4[2];
            tmax[3] = otherNode.tmax4[3];

/*
            active[0] = otherNode.active[0] && tmin[0] < tmax[0];
            active[1] = otherNode.active[1] && tmin[1] < tmax[1];
            active[2] = otherNode.active[2] && tmin[2] < tmax[2];
            active[3] = otherNode.active[3] && tmin[3] < tmax[3];
 */
            active[0] = tmin[0] < tmax[0];
            active[1] = tmin[1] < tmax[1];
            active[2] = tmin[2] < tmax[2];
            active[3] = tmin[3] < tmax[3];

            sKDToDoStack.pop_back();
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
