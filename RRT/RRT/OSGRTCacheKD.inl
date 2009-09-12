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
    tmin4   = source.tmin4;
    tmax4   = source.tmax4;

    node = source.node;
}

template<typename DescT> inline
void RTCacheKD<DescT>::KDSIMDStackElem::operator =(
    const KDSIMDStackElem &source)
{
    tmin4   = source.tmin4;
    tmax4   = source.tmax4;

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
#ifndef OSG_SIMD_RAYPACKET_DEBUG
    Vec3f vRayDirs[4];

    union
    {
        Float4 dir;
        Real32 dirConv[4];
    };

    for(UInt32 i = 0; i < 3; ++i)
    {
        dir = oRay.getDir(i);

        vRayDirs[0][i] = dirConv[0];
        vRayDirs[1][i] = dirConv[1];
        vRayDirs[2][i] = dirConv[2];
        vRayDirs[3][i] = dirConv[3];
    }

    Pnt3f vOrigin(oRay.getOriginComp(0),
                  oRay.getOriginComp(1),
                  oRay.getOriginComp(2));
#endif

    for(UInt32 iRay = 0; iRay < 4; ++iRay)
    {
        float tmin, tmax;

#ifndef OSG_SIMD_RAYPACKET_DEBUG
        Line lineRay(vOrigin, vRayDirs[iRay]);
#else
        Line lineRay(oRay.getOriginPnt(), oRay.getDirVec(iRay));
#endif
        
/*
	if (!bounds.IntersectP(ray, &tmin, &tmax))
		return false;
 */

        sKDToDoStack.clear  (   );

        if(this->_bBoundingVolume.intersect(lineRay, tmin, tmax) == false)
            continue;
        
#ifndef OSG_SIMD_RAYPACKET_DEBUG
        Vec3f invDir(1.f / vRayDirs[iRay].x(), 
                     1.f / vRayDirs[iRay].y(), 
                     1.f / vRayDirs[iRay].z());
#else
        Vec3f invDir(1.f / oRay.getDirVec(iRay).x(), 
                     1.f / oRay.getDirVec(iRay).y(), 
                     1.f / oRay.getDirVec(iRay).z());
#endif

        
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

#ifndef OSG_SIMD_RAYPACKET_DEBUG
                float tplane = 
                    (node->getSplitPos() - vOrigin[axis]) *
                    invDir[axis];
#else
                float tplane = 
                    (node->getSplitPos() - oRay.getOriginPnt()[axis]) *
                    invDir[axis];
#endif

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

#ifndef OSG_SIMD_RAYPACKET_DEBUG
                bool belowFirst = 
                    vOrigin[axis] <= node->getSplitPos();
#else
                bool belowFirst = 
                    oRay.getOriginPnt()[axis] <= node->getSplitPos();
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
                    otherNode.tminA[iRay] = tplane;
                    otherNode.tmaxA[iRay] = tmax;
                    
                    node = nearChild;
                    tmax = tplane;

                    sKDToDoStack.push_back(otherNode);
                }
            }
            
            UInt32 nPrimitives = node->getNumPrimitives();

            if(nPrimitives == 1) 
            {
#ifndef OSG_SIMD_RAYPACKET_DEBUG
                this->_vTriangleAcc[node->_uiSinglePrimitive].intersectSingle(
                    vRayDirs, vOrigin, oHit, uiCacheId, iRay);
#else
                this->_vTriangleAcc[node->_uiSinglePrimitive].intersectSingle(
                    oRay, oHit, uiCacheId, iRay);
#endif
            }
            else 
            {
                std::vector<UInt32> &prims = 
                    this->_vPrimitives[node->_pPrimitiveIdx];
                
                for(u_int i = 0; i < nPrimitives; ++i) 
                {
#ifndef OSG_SIMD_RAYPACKET_DEBUG
                    this->_vTriangleAcc[prims[i]].intersectSingle(vRayDirs,
                                                                  vOrigin, 
                                                                  oHit,
                                                                  uiCacheId,
                                                                  iRay);
#else
                    this->_vTriangleAcc[prims[i]].intersectSingle(oRay, 
                                                                  oHit,
                                                                  uiCacheId,
                                                                  iRay);
#endif
                }
            }
            
            if(oHit.getDist(iRay) <= tmax)
                break;
            
            if(sKDToDoStack.size() > 0) 
            {
                KDSIMDStackElem otherNode = sKDToDoStack.back();
                
                node = otherNode.node;
                tmin = otherNode.tminA[iRay];
                tmax = otherNode.tmaxA[iRay];
                
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

    UInt32 uiDirSign[3];

    uiDirSign[0] = osgSIMDMoveMask(oRay.getDir(RTRaySIMDPacket::X));
    uiDirSign[1] = osgSIMDMoveMask(oRay.getDir(RTRaySIMDPacket::Y));
    uiDirSign[2] = osgSIMDMoveMask(oRay.getDir(RTRaySIMDPacket::Z));

    if (((uiDirSign[0] != 0) && (uiDirSign[0] != 15)) || 
        ((uiDirSign[1] != 0) && (uiDirSign[1] != 15)) ||
        ((uiDirSign[2] != 0) && (uiDirSign[2] != 15)))

    {
        intersectSingle(oRay, 
                        oHit,
                        sKDToDoStack,
                        uiCacheId,
                        uiActive);
        return;
    }

    Float4 tmin4       = SIMDZero;
    Float4 tmax4       = SIMDInfinity;
    UInt32 activeMask  = 0xf;
    UInt32 termination = 0;


    SSEVec4 invDir;   

    invDir._data[RTRaySIMDPacket::X] = 
        osgSIMDInvert(oRay.getDir(RTRaySIMDPacket::X));
    invDir._data[RTRaySIMDPacket::Y] = 
        osgSIMDInvert(oRay.getDir(RTRaySIMDPacket::Y));
    invDir._data[RTRaySIMDPacket::Z] = 
        osgSIMDInvert(oRay.getDir(RTRaySIMDPacket::Z));


    // ClipX
    const Float4 clipMinX = 
        osgSIMDMul(
            osgSIMDSub(
                osgSIMDSet(
                    this->_bBoundingVolume.getMin()[RTRaySIMDPacket::X]),
                osgSIMDSet(oRay.getOriginComp      (RTRaySIMDPacket::X))),
            invDir._data[RTRaySIMDPacket::X]);

    const Float4 clipMaxX = 
        osgSIMDMul(
            osgSIMDSub(
                osgSIMDSet(
                    this->_bBoundingVolume.getMax()[RTRaySIMDPacket::X]),
                osgSIMDSet(oRay.getOriginComp      (RTRaySIMDPacket::X))),
            invDir._data[RTRaySIMDPacket::X]);


    const Float4 cmpX = osgSIMDCmpGT(oRay.getDir(RTRaySIMDPacket::X), 
                                     SIMDZero);

    tmin4 = osgSIMDMax(tmin4, osgSIMDUpdate(cmpX, clipMinX, clipMaxX));
    tmax4 = osgSIMDMin(tmax4, osgSIMDUpdate(cmpX, clipMaxX, clipMinX));



    // ClipY
    const Float4 clipMinY = 
        osgSIMDMul(
            osgSIMDSub(
                osgSIMDSet(
                    this->_bBoundingVolume.getMin()[RTRaySIMDPacket::Y]),
                osgSIMDSet(oRay.getOriginComp      (RTRaySIMDPacket::Y))),
            invDir._data[RTRaySIMDPacket::Y]);
    
    const Float4 clipMaxY = 
        osgSIMDMul(
            osgSIMDSub(
                osgSIMDSet(
                    this->_bBoundingVolume.getMax()[RTRaySIMDPacket::Y]),
                osgSIMDSet(oRay.getOriginComp      (RTRaySIMDPacket::Y))),
            invDir._data[RTRaySIMDPacket::Y]);
    
    const Float4 cmpY = osgSIMDCmpGT(oRay.getDir(RTRaySIMDPacket::Y), 
                                     SIMDZero);


    tmin4 = osgSIMDMax(tmin4, osgSIMDUpdate(cmpY, clipMinY, clipMaxY));
    tmax4 = osgSIMDMin(tmax4, osgSIMDUpdate(cmpY, clipMaxY, clipMinY));



    // ClipZ
    const Float4 clipMinZ = 
        osgSIMDMul(
            osgSIMDSub(
                osgSIMDSet(
                    this->_bBoundingVolume.getMin()[RTRaySIMDPacket::Z]),
                osgSIMDSet(oRay.getOriginComp      (RTRaySIMDPacket::Z))),
            invDir._data[RTRaySIMDPacket::Z]);

    const Float4 clipMaxZ = 
        osgSIMDMul(
            osgSIMDSub(
                osgSIMDSet(
                    this->_bBoundingVolume.getMax()[RTRaySIMDPacket::Z]),
                osgSIMDSet(oRay.getOriginComp      (RTRaySIMDPacket::Z))),
            invDir._data[RTRaySIMDPacket::Z]);
 
    const Float4 cmpZ = osgSIMDCmpGT(oRay.getDir(RTRaySIMDPacket::Z), 
                                     SIMDZero);

    tmin4 = osgSIMDMax(tmin4, osgSIMDUpdate(cmpZ, clipMinZ, clipMaxZ));
    tmax4 = osgSIMDMin(tmax4, osgSIMDUpdate(cmpZ, clipMaxZ, clipMinZ));

    activeMask = osgSIMDMoveMask(osgSIMDCmpLT(tmin4, tmax4));

    if(activeMask == 0x0)
    {
        return;
    }

    sKDToDoStack.clear  (   );

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
            
            const Float4 tplane4 = 
                osgSIMDMul(
                    osgSIMDSub(osgSIMDSet(node->getSplitPos()),
                               osgSIMDSet(oRay.getOriginComp(axis))),
                    invDir._data[axis]);

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

//            bool belowFirst = oRay.getDirVec(0)[axis] > 0;
            bool belowFirst = !uiDirSign[axis];

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

            if(!(osgSIMDMoveMask(osgSIMDCmpGT(tmax4, 
                                              tplane4)) & activeMask))
            {
                continue;
            }

            node = farChild;

            if(!(osgSIMDMoveMask(osgSIMDCmpLE(tmin4, 
                                              tplane4)) & activeMask))
            {
                continue;
            }

            KDSIMDStackElem otherNode;
                
            otherNode.node        = farChild;

            otherNode.tmin4 = osgSIMDMax(tplane4, tmin4);
            otherNode.tmax4 = tmax4;


            node       = nearChild;

            tmax4 = osgSIMDMin(tplane4, tmax4);

            activeMask = osgSIMDMoveMask(osgSIMDCmpLT(tmin4, tmax4));

            sKDToDoStack.push_back(otherNode);
        }
            

        UInt32 nPrimitives = node->getNumPrimitives();

        static const UInt32 ActiveMask[4] = { 0x1, 0x2, 0x4, 0x8 };

        if(nPrimitives == 1) 
        {
            this->_vTriangleAcc[node->_uiSinglePrimitive].intersect(
                oRay, oHit, uiCacheId, activeMask);
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
                                                        activeMask);
            }
        }

        termination |= (activeMask &
            osgSIMDMoveMask(osgSIMDCmpLE(osgSIMDSet(oHit.getDist(3),
                                                    oHit.getDist(2),
                                                    oHit.getDist(1),
                                                    oHit.getDist(0)),
                                         tmax4)));

        if(termination == 0xf)
            break;

        if(sKDToDoStack.size() > 0) 
        {
            KDSIMDStackElem otherNode = sKDToDoStack.back();
            
            node    = otherNode.node;

            tmin4 = otherNode.tmin4;
            tmax4 = otherNode.tmax4;

            activeMask = ~termination &
                osgSIMDMoveMask(osgSIMDCmpLE(tmin4, tmax4));

            sKDToDoStack.pop_back();
        }
        else
        {
            break;
        }
    }
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
