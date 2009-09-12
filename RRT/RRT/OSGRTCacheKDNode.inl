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
RTKDNode::RTKDNode(void) :
    _bIsLeave   (false),
    _uiSplitAxis(0    ),
    _fSplitPos  (0.f  ),
    _pAboveChild(NULL ),
    _pBelowChild(NULL ),
    _uiPrimIdx  (0    )
{
}

inline
RTKDNode::~RTKDNode(void)
{
    delete _pAboveChild;
    delete _pBelowChild;
}

inline
void RTKDNode::initLeaf(IndexIterator          primNums, 
                        IndexSize              np,
                        MFRTCachePrimIdxStore &vStore)
{
    _bIsLeave = true;

    _pAboveChild = NULL;
    _pBelowChild = NULL;

    _uiPrimIdx = vStore.size();

    std::vector<UInt32> tmpVec;

    vStore.push_back(tmpVec);

    vStore.back().reserve(np);

    for(UInt32 i = 0; i < np; ++i)
        vStore.back().push_back(*primNums++);
}

inline
void RTKDNode::initInterior(UInt32 uiAxis, 
                            Real32 fSplitPos)
{
    _bIsLeave    = false;
    _uiSplitAxis = uiAxis;
    _fSplitPos   = fSplitPos;

    _pAboveChild = NULL;
    _pBelowChild = NULL;
}


inline
bool RTKDNode::isLeave(void)
{
    return _bIsLeave;
}

inline
void RTKDNode::setAboveChild(RTKDNode *pNode)
{
    _pAboveChild = pNode;
}

inline
RTKDNode *RTKDNode::getAboveChild(void)
{
    return _pAboveChild;
}

inline
void RTKDNode::setBelowChild(RTKDNode *pNode)
{
    _pBelowChild = pNode;
}

inline
RTKDNode *RTKDNode::getBelowChild(void)
{
    return _pBelowChild;
}

inline
UInt32 RTKDNode::getSplitAxis(void)
{
    return _uiSplitAxis;
}

inline
Real32 RTKDNode::getSplitPos(void)
{
    return _fSplitPos;
}

inline
UInt32 RTKDNode::getPrimitiveIdx(void)
{
    return _uiPrimIdx;
}





inline
void RTCacheKDNode::initLeaf(IndexIterator          primNums, 
                             IndexSize              np,
                             MFRTCachePrimIdxStore &vStore  )
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

inline
void RTCacheKDNode::initInterior(UInt32 uiAxis, 
                                 Real32 fSplitPos)
{
    _fSplitPos = fSplitPos;

    _uiFlags &= ~3;
    _uiFlags |= uiAxis;
}

inline
void RTCacheKDNode::initLeaf(RTKDNode              *pNode,  
                             MFRTCachePrimIdxStore &vStore)
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

inline
void RTCacheKDNode::initInterior(RTKDNode *pNode)
{
    _fSplitPos = pNode->getSplitPos();

    _uiFlags &= ~3;
    _uiFlags |= pNode->getSplitAxis();
}

inline
Real32 RTCacheKDNode::getSplitPos(void) const
{ 
    return _fSplitPos; 
}

inline
UInt32 RTCacheKDNode::getNumPrimitives(void) const 
{ 
    return _uiNumPrims >> 2; 
}

inline
UInt32 RTCacheKDNode::getSplitAxis(void) const 
{ 
    return _uiFlags & 3; 
}

inline
bool RTCacheKDNode::isLeaf(void) const 
{
    return (_uiFlags & 3) == 3; 
}

inline
void RTCacheKDNode::putToStream(OutStream &str) const
{
    typedef TypeTraits<UInt32> UInt32TypeTrait;

    UInt32TypeTrait::putToStream(_uiFlags, str);
    str << " ";

    UInt32TypeTrait::putToStream(_uiAboveChild, str);
    str << " ";
}

inline
bool RTCacheKDNode::getFromCString(const Char8 *inVal)
{
    if(inVal == NULL)
        return false;

    UInt32 rc = sscanf(inVal, "%u %u", &_uiFlags, &_uiAboveChild);

    if(rc != 2)
        return false;

    return true;
}

inline
bool RTCacheKDNode::operator ==(const RTCacheKDNode &rhs) const
{
    return 
        (_uiFlags      == rhs._uiFlags      &&
         _uiAboveChild == rhs._uiAboveChild  );
}

OSG_END_NAMESPACE
