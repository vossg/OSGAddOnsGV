/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *           Copyright (C) 2003 by the OpenSG Forum                          *
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

inline
void RTXKDNode::printBoxValues(void) // used for debugging, remove later
{
    printf("BoundingBox values: %f %f %f %f %f %f\n", 
           _pBBox->min[0], _pBBox->min[1], _pBBox->min[2], 
           _pBBox->max[0], _pBBox->max[1], _pBBox->max[2]);	 
}

inline
RTXKDNode::RTXKDNode(void) :
    _bIsLeaf    (false   ),
    _uiSplitAxis(0       ),
    _fSplitPos  (0.f     ),
    _pAboveChild(NULL    ),
    _pBelowChild(NULL    ),
    _nodeId     (UINT_MAX),
    _pBBox      (NULL    )
{
}

inline
RTXKDNode::~RTXKDNode(void)
{
    delete _pAboveChild;
    delete _pBelowChild;
    delete _pBBox; // Unsure why I can't delete boundingbox pointer, fix later
}

inline
void RTXKDNode::initLeaf(IndexIterator          primNums, 
                         IndexSize              np,
                         MFRTCachePrimIdxStore &vStore,
                         UInt32                 id      )
{
    _uiNumPrim  = np << 2;
  
    _nodeId = id;
    _bIsLeaf = true;

    _pAboveChild = NULL;
    _pBelowChild = NULL;

    _pBBox = new boxVolume_t;

    _uiPrimIdx = vStore.size();

    std::vector<UInt32> tmpVec;

    vStore.push_back(tmpVec);

    vStore.back().reserve(np);

    if(np==0)
    {
        vStore.back().push_back(UINT_MAX);
    }
    else
    {
        for(UInt32 i = 0; i < np; ++i)
            vStore.back().push_back(*primNums++);
    }
}

inline
void RTXKDNode::initInterior(UInt32 uiAxis, 
                             Real32 fSplitPos,
                             UInt32 id       )
{
    _bIsLeaf     = false;
    _uiSplitAxis = uiAxis;
    _fSplitPos   = fSplitPos;

    _pAboveChild = NULL;
    _pBelowChild = NULL;
    _pBBox       = NULL;
    _nodeId      = id;
}

inline
void RTXKDNode::initRope(void)
{
    for(UInt32 i = 0 ; i < 6 ; ++i)
        _rope[i]= 0;
}

inline
bool RTXKDNode::isLeaf(void)
{
    return _bIsLeaf;
}

inline
void RTXKDNode::setAboveChild(RTXKDNode *pNode)
{
    _pAboveChild = pNode;
}

inline
RTXKDNode *RTXKDNode::getAboveChild(void)
{
    return _pAboveChild;
}

inline
void RTXKDNode::setBelowChild(RTXKDNode *pNode)
{
    _pBelowChild = pNode;
}

inline
RTXKDNode *RTXKDNode::getBelowChild(void)
{
    return _pBelowChild;
}

inline
UInt32 RTXKDNode::getSplitAxis(void)
{
    return _uiSplitAxis;
}

inline
Real32 RTXKDNode::getSplitPos(void)
{
    return _fSplitPos;
}

inline
UInt32 RTXKDNode::getPrimitiveIdx(void)
{
    return _uiPrimIdx;
}

inline
void RTXKDNode::setRope(UInt32 id, UInt8 index)
{
    _rope[index] = id;
}

inline
void RTXKDNode::setBoundingBox(boxVolume_t* box)
{
    *_pBBox = *box;
}

inline
UInt32 RTXKDNode::getRope(UInt8 index)
{
    return _rope[index];
}

inline 
Real32 RTXKDNode::getBoxMinValue(UInt8 index)
{
    assert(index < 3);

    return _pBBox->min[index];
}

inline 
Real32 RTXKDNode::getBoxMaxValue(UInt8 index)
{
    assert(index < 3);

    return _pBBox->max[index];
}

inline
void RTXKDNode::setBoxMinValue(UInt8 index, Real32 val)
{
    assert(index < 3);

    _pBBox->min[index] = val;
}

inline
void RTXKDNode::setBoxMaxValue(UInt8 index, Real32 val)
{
    assert(index < 3);

    _pBBox->max[index] = val;
}

inline
boxVolume_t RTXKDNode::getBoxVolume()
{
    return *_pBBox;
}

inline
UInt32 RTXKDNode::getNodeId()
{
    return _nodeId;
}

inline
UInt32 RTXKDNode::getNumPrimitives(void) const 
{ 
    return _uiNumPrim >> 2; 
}

inline
void RTXCacheKDNode::initLeaf(IndexIterator          primNums, 
                             IndexSize              np,
                             MFRTCachePrimIdxStore &vStore  )
{
    node.uiNumPrims  = np << 2;
    node.uiFlags    |= 3;

    node.pPrimitiveIdx = vStore.size();

    std::vector<UInt32> tmpVec;
        
    vStore.push_back(tmpVec);
  
    if(np==0)
    {
        vStore.back().reserve(np+1);
        vStore.back().push_back(UINT_MAX);
    }
    else
    {
        for(UInt32 i = 0; i < np; ++i)
            vStore.back().push_back(*primNums++);
    }
}

inline
void RTXCacheKDNode::initInterior(UInt32 uiAxis, 
                                  Real32 fSplitPos)
{
    node.fSplitPos = fSplitPos;

    node.uiFlags &= ~3;
    node.uiFlags |= uiAxis;
}

inline
void RTXCacheKDNode::initLeaf(RTXKDNode             *pNode,  
                              MFRTCachePrimIdxStore &vStore)
{
    std::vector<UInt32> &vPrimitives = vStore[pNode->getPrimitiveIdx()];

    node.uiNumPrims  = vPrimitives.size() << 2;
    node.uiFlags    |= 3;
  
    if(vPrimitives.size() == 0)
    {
        node.pPrimitiveIdx = 0;
    }
    else 
    {
        node.pPrimitiveIdx = pNode->getPrimitiveIdx();
    }
}

inline
void RTXCacheKDNode::initInterior(RTXKDNode *pNode)
{
    node.fSplitPos = pNode->getSplitPos();

    node.uiFlags &= ~3;
    node.uiFlags |= pNode->getSplitAxis();
}

inline
Real32 RTXCacheKDNode::getSplitPos(void) const
{ 
    return node.fSplitPos; 
}

inline
UInt32 RTXCacheKDNode::getNumPrimitives(void) const 
{ 
    return node.uiNumPrims >> 2; 
}

inline
UInt32 RTXCacheKDNode::getSplitAxis(void) const 
{ 
    return node.uiFlags & 3; 
}

inline
bool RTXCacheKDNode::isLeaf(void) const 
{
    return (node.uiFlags & 3) == 3; 
}

inline
void RTXCacheKDNode::putToStream(OutStream &str) const
{
    typedef TypeTraits<UInt32> UInt32TypeTrait;

    UInt32TypeTrait::putToStream(node.uiFlags, str);
    str << " ";

    UInt32TypeTrait::putToStream(node.uiAboveChild, str);
    str << " ";
}

inline
bool RTXCacheKDNode::getFromCString(const Char8 *inVal)
{
    if(inVal == NULL)
        return false;

    UInt32 rc = sscanf(inVal, "%u %u", &node.uiFlags, &node.uiAboveChild);

    if(rc != 2)
        return false;

    return true;
}

inline
UInt32 RTXCacheKDNode::getBinSize(void) const
{
    return 
        sizeof(UInt32) + sizeof(UInt32) + 
        sizeof(UInt32) * 6 + sizeof(boxVolume_t);
}

inline
void RTXCacheKDNode::copyToBin(BinaryDataHandler &pMem) const
{
    pMem.putValue(node.uiFlags     );
    pMem.putValue(node.uiAboveChild);
}

inline
void RTXCacheKDNode::copyFromBin(BinaryDataHandler &pMem)
{
    pMem.getValue(node.uiFlags     );
    pMem.getValue(node.uiAboveChild);
}
    

inline
bool RTXCacheKDNode::operator ==(const RTXCacheKDNode &rhs) const
{
    return 
        (node.uiFlags      == rhs.node.uiFlags      &&
         node.uiAboveChild == rhs.node.uiAboveChild  );
}

OSG_END_NAMESPACE
