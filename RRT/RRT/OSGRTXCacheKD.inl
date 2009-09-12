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
RTXCacheKD<DescT>::KDSIMDStackElem::KDSIMDStackElem(void)
{
}

template<typename DescT> inline
RTXCacheKD<DescT>::KDSIMDStackElem::KDSIMDStackElem(
    const KDSIMDStackElem &source)
{
    tmin4   = source.tmin4;
    tmax4   = source.tmax4;

    node = source.node;
}

template<typename DescT> inline
void RTXCacheKD<DescT>::KDSIMDStackElem::operator =(
    const KDSIMDStackElem &source)
{
    tmin4   = source.tmin4;
    tmax4   = source.tmax4;

    node = source.node;
}


OSG_FIELD_CONTAINER_TMPL_NO_TYPE_DEF  (RTXCacheKD, DescT)
//OSG_RC_GET_STATIC_TYPE_INL_TMPL_DEF   (DynFieldAttachment, AttachmentDescT)
OSG_RC_GET_STATIC_TYPE_ID_INL_TMPL_DEF(RTXCacheKD, DescT)

template<typename DescT> inline
RTXCacheKD<DescT>::RTXCacheKD(void) :
     Inherited        (    ),
    _mfKDTree         (    ),
    _uiNumAllocedNodes(   0),
    _uiNextFreeNode   (   0)
{
}


template<typename DescT> inline
RTXCacheKD<DescT>::RTXCacheKD(const RTXCacheKD &source) :
     Inherited        (source),
    _mfKDTree         (      ),
    _uiNumAllocedNodes(     0),
    _uiNextFreeNode   (     0)
{
}

template<typename DescT> inline
RTXCacheKD<DescT>::~RTXCacheKD(void)
{
}

template<typename DescT> inline
EditFieldHandlePtr RTXCacheKD<DescT>::editHandleKDTree(void)
{
    typename MFRTXCacheKDNode::EditHandlePtr returnValue(
        new typename MFRTXCacheKDNode::EditHandle(
             &_mfKDTree, 
             this->getType().getFieldDesc(KDTreeFieldId)));

    this->editMField(KDTreeFieldMask, _mfKDTree);

    return returnValue;
}

template<typename DescT> inline
GetFieldHandlePtr RTXCacheKD<DescT>::getHandleKDTree (void) const
{
    typename MFRTXCacheKDNode::GetHandlePtr returnValue(
        new typename MFRTXCacheKDNode::GetHandle(
             &_mfKDTree, 
             this->getType().getFieldDesc(KDTreeFieldId)));

    return returnValue;
}

template<typename DescT> inline
UInt32 RTXCacheKD<DescT>::getBinSize(ConstFieldMaskArg  whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);

    if(FieldBits::NoField != (KDTreeFieldMask & whichField))
    {
        returnValue += _mfKDTree.getBinSize();
    }

    return returnValue;
}

template<typename DescT> inline
void RTXCacheKD<DescT>::copyToBin(BinaryDataHandler &pMem,
                                  ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

    if(FieldBits::NoField != (KDTreeFieldMask & whichField))
    {
        _mfKDTree.copyToBin(pMem);
    }
}

template<typename DescT> inline
void RTXCacheKD<DescT>::copyFromBin(BinaryDataHandler &pMem,
                                    ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

    if(FieldBits::NoField != (KDTreeFieldMask & whichField))
    {
        _mfKDTree.copyFromBin(pMem);
    }
}


#ifdef OSG_MT_CPTR_ASPECT

template<typename DescT> inline
typename RTXCacheKD<DescT>::ObjPtr 
    RTXCacheKD<DescT>::createAspectCopy(void) const
{
    ObjPtr returnValue;

    newAspectCopy(returnValue,
                  dynamic_cast<const Self *>(this));

    return returnValue;
}

template<typename DescT> inline
void RTXCacheKD<DescT>::execSyncV(
          FieldContainer     &oFrom,
          ConstFieldMaskArg   whichField,
          AspectOffsetStore  &oOffsets,
          ConstFieldMaskArg   syncMode  ,
    const UInt32              uiSyncInfo)
{
    this->execSync(static_cast<RTXCacheKD *>(&oFrom),
                   whichField,
                   oOffsets,
                   syncMode,
                   uiSyncInfo);
}

template<typename DescT> inline
void RTXCacheKD<DescT>::execSync (
          RTXCacheKD         *pFrom,
          ConstFieldMaskArg   whichField,
          AspectOffsetStore  &oOffsets,
          ConstFieldMaskArg   syncMode  ,
    const UInt32              uiSyncInfo)
{
    Inherited::execSync(pFrom, whichField, oOffsets, syncMode, uiSyncInfo);
}

#endif

template<typename DescT> inline
void RTXCacheKD<DescT>::buildStructure(void)
{
    // nextFreeNode = 0;
    _uiNumAllocedNodes = 128;

    RTXKDNode *pKDTree = Inherited::buildInternalTree();

    UInt32 maxLeafs = (_uiNumAllocedNodes-1)/2;

    // required because root node also count as leaf in extended tree
    maxLeafs++; 
 
    XKDTree = (RTXCacheKDNode *) 
        malloc(31 + sizeof(RTXCacheKDNode)* _uiNumAllocedNodes);

    nodeShift = 0;

    while (((unsigned long long) XKDTree) & 0x1F) 
    {
        XKDTree=(RTXCacheKDNode*)((char*)XKDTree+1);
        nodeShift++;
    }

    boxVolumeStore = (boxVolume_t*)malloc(31 + sizeof(boxVolume_t)*maxLeafs);

    boxShift = 0;

    while (((unsigned long long) boxVolumeStore) & 0x1F)
    {
        boxVolumeStore=(boxVolume_t*)((char*)boxVolumeStore+1);
        boxShift++;
    }

    XKDTree[_uiNextFreeNode].node.uiAboveChild = 0;
    XKDTree[_uiNextFreeNode].node.uiFlags = 0;


    for(int i=0 ; i < 3 ; ++i)
    {
        boxVolumeStore[0].min[i] = 
            Inherited::_sfBoundingVolume.getValue().getMin()[i];

        boxVolumeStore[0].max[i] = 
            Inherited::_sfBoundingVolume.getValue().getMax()[i];
    }

    _uiNextFreeNode++;
	
    for(int i = 0 ; i < 6 ; ++i)
    {
        XKDTree[_uiNextFreeNode].node.ropeOffset[i] = 0;
    }

    if(pKDTree->isLeaf() == true)
    {
        XKDTree[_uiNextFreeNode].initLeaf(pKDTree, this->_mfPrimitives);
        _uiNextFreeNode++;
        nextFreeLeaf++;
    }
    else
    {
        XKDTree[_uiNextFreeNode].initInterior(pKDTree);
        XKDTree[_uiNextFreeNode].node.uiAboveChild = sizeof(cacheKDNode_t);

        ++_uiNextFreeNode;

        flattenTree(pKDTree->getBelowChild(), pKDTree->getAboveChild());
    }

    XKDTree = (RTXCacheKDNode*)((char*)XKDTree-nodeShift);
    XKDTree = (RTXCacheKDNode*)
        realloc(XKDTree, 31 + _uiNextFreeNode*sizeof(RTXCacheKDNode));

    nodeShift=0;

    while (((unsigned long long) XKDTree) & 0x1F) 
    {
        XKDTree=(RTXCacheKDNode*)((char*)XKDTree+1);
        nodeShift++;
    }
    
    boxVolumeStore = (boxVolume_t*)((char*)boxVolumeStore-boxShift);

    boxVolumeStore = 
        (boxVolume_t*)realloc(boxVolumeStore, 
                              31 + (nextFreeLeaf)*sizeof(boxVolume_t));
    
    boxShift=0;

    while(((unsigned long long) boxVolumeStore) & 0x1F)
    {
        boxVolumeStore=(boxVolume_t*)((char*)boxVolumeStore+1);
        boxShift++;
    }

#if 1
    printf("Flattened XDKTree\n");
    printf("_uiNextFreeNode %d\n", _uiNextFreeNode);

    printf("size xkdtree: %u addr 0x%llx\n", 
           sizeof(*XKDTree)*(_uiNextFreeNode), 
           (unsigned long int)XKDTree);

    printf("size boxV: %u addr 0x%llx\n", 
           sizeof(*boxVolumeStore)*nextFreeLeaf, 
           (unsigned long int)boxVolumeStore);

    printf("Root Volume: %f %f %f %f %f %f\n",
           boxVolumeStore[0].min[0],
           boxVolumeStore[0].min[1],
           boxVolumeStore[0].min[2],
           boxVolumeStore[0].max[0],
           boxVolumeStore[0].max[1],
           boxVolumeStore[0].max[2]);
	 
    for(int i = 1 ; i < _uiNextFreeNode ; ++i)
    {
        if(XKDTree[i].isLeaf())
        {
            printf("%u: %u %u \t\tropes: %d %d %d %d %d %d\tbox: %u\n",
                   i,  
                   XKDTree[i].node.uiNumPrims >> 2, 
                   XKDTree[i].node.pPrimitiveIdx,
                   XKDTree[i].node.ropeOffset[0],
                   XKDTree[i].node.ropeOffset[1],
                   XKDTree[i].node.ropeOffset[2], 
                   XKDTree[i].node.ropeOffset[3],
                   XKDTree[i].node.ropeOffset[4],
                   XKDTree[i].node.ropeOffset[5],
                   XKDTree[i].node.pPrimitiveIdx+1);

            boxVolume_t* volume = 
                &boxVolumeStore[XKDTree[i].node.pPrimitiveIdx+1];

            printf("box volume: %f %f %f %f %f %f\n", 
                   volume->min[0],
                   volume->min[1],
                   volume->min[2],
                   volume->max[0],
                   volume->max[1],
                   volume->max[2]);
        }
        else
        {
            printf("%u: %.2f %u \t\tropes: %d %d %d %d %d %d\n",
                   i, 
                   XKDTree[i].node.fSplitPos, XKDTree[i].node.uiAboveChild,
                   XKDTree[i].node.ropeOffset[0],
                   XKDTree[i].node.ropeOffset[1],
                   XKDTree[i].node.ropeOffset[2],
                   XKDTree[i].node.ropeOffset[3],
                   XKDTree[i].node.ropeOffset[4],
                   XKDTree[i].node.ropeOffset[5]);
        }
    }	  

    boxVolumeStore=(boxVolume_t*)((char*)boxVolumeStore-boxShift);
    XKDTree=(RTXCacheKDNode*)((char*)XKDTree-nodeShift);
    free(XKDTree);
    free(boxVolumeStore);
    printf("Terminated at RTXCacheKD::buildStructure()\n");
    exit(0);
#endif

    delete pKDTree;
}

template<typename DescT> inline
void RTXCacheKD<DescT>::intersect(BasicRayPacket &oRay, 
                                  RTHitPacket    &oHit,
                                  UInt32          uiCacheId)
{
    fprintf(stderr, 
            "\nExtended KD Cache does not support ray intersection on PPU.\n");
    fprintf(stderr, "Application terminated\n");
    exit(-1);
}

template<typename DescT> inline
void RTXCacheKD<DescT>::flattenTree(RTXKDNode *pLeft, 
                                    RTXKDNode *pRight)
{ 
    if(_uiNumAllocedNodes <= _uiNextFreeNode + 2)
    {
        UInt32 nAlloc = osgMax((UInt32) 2* _uiNumAllocedNodes, 512u);
        
        XKDTree = 
            (RTXCacheKDNode*)realloc(XKDTree, 
                                     31 + nAlloc*sizeof(RTXCacheKDNode));

        while ((((unsigned long int) XKDTree) & 0x1F) != 0x0) 
            ++XKDTree;
      
        UInt32 nAllocLeafs = (nAlloc-1)/2;

        nAllocLeafs++; //required because root node also count as leaf
      
        boxVolumeStore = 
            (boxVolume_t*)realloc(boxVolumeStore, 
                                  31 + nAllocLeafs*sizeof(boxVolume_t));

        while((((unsigned long int) boxVolumeStore) & 0x1F) != 0x0)
            ++boxVolumeStore;
      
        _uiNumAllocedNodes = nAlloc;
    }

    UInt32 uiLeft  = _uiNextFreeNode++;
    UInt32 uiRight = _uiNextFreeNode++;

    if(pLeft->isLeaf() == false)
    {
        XKDTree[uiLeft].initInterior(pLeft);
        XKDTree[uiLeft].node.uiAboveChild = 2 * sizeof(cacheKDNode_t);

        for(int i = 0 ; i < 6 ; ++i)
        {
            Int32 offset = 0;

            if(pLeft->getRope(i) != 0)
            {
                offset = pLeft->getRope(i) - pLeft->getNodeId();
            }

            XKDTree[uiLeft].node.ropeOffset[i] = offset*sizeof(cacheKDNode_t);
        }

        flattenTree(pLeft->getBelowChild(), pLeft->getAboveChild());
    }
    else
    {
        XKDTree[uiLeft].initLeaf(pLeft, this->_mfPrimitives);

        nextFreeLeaf++;
        boxVolumeStore[nextFreeLeaf] = pLeft->getBoxVolume();

        for(int i = 0 ; i < 6 ; ++i)
        {
            Int32 offset = 0;
            if(pLeft->getRope(i) != 0)
                offset = pLeft->getRope(i) - pLeft->getNodeId();

            XKDTree[uiLeft].node.ropeOffset[i] = offset*sizeof(cacheKDNode_t);
        }
    }
    if(pRight->isLeaf() == false)
    {
        XKDTree[uiRight].initInterior(pRight);
        XKDTree[uiRight].node.uiAboveChild = 
            (_uiNextFreeNode - uiRight) * sizeof(cacheKDNode_t);
	  
        for(int i = 0 ; i < 6 ; ++i)
        {
            Int32 offset = 0;
            if(pRight->getRope(i) != 0)
                offset = pRight->getRope(i) - pRight->getNodeId();

            XKDTree[uiRight].node.ropeOffset[i] = offset*sizeof(cacheKDNode_t);
        }
        flattenTree(pRight->getBelowChild(), pRight->getAboveChild());
    }
    else
    {
        XKDTree[uiRight].initLeaf(pRight, this->_mfPrimitives);
	  
        nextFreeLeaf++;
        boxVolumeStore[nextFreeLeaf] = pRight->getBoxVolume();

        for(int i = 0 ; i < 6 ; ++i)
        {
            Int32 offset = 0;
            if(pRight->getRope(i) != 0)
                offset = pRight->getRope(i) - pRight->getNodeId();
            
            XKDTree[uiRight].node.ropeOffset[i] = offset*sizeof(cacheKDNode_t);
        }
    }
}
 
template<typename DescT> inline
void RTXCacheKD<DescT>::intersect(BasicSIMDRayPacket &oRay, 
                                  RTHitSIMDPacket    &oHit,
                                  UInt32              uiCacheId,
                                  UInt16             *uiActive)
{
    fprintf(stderr, 
            "\nExtended KD Cache does not support ray intersection on PPU.\n");
    fprintf(stderr, "Application terminated\n");

    exit(0);
}

template<typename DescT> inline
cacheKDNode_t *RTXCacheKD<DescT>::getXCacheKDPointer(void)
{
    return (cacheKDNode_t *) &(XKDTree[0].node);
}

template<typename DescT> inline
boxVolume_t *RTXCacheKD<DescT>::getBoxCachePointer(void)
{
    return &(boxVolumeStore[1]);
}

template<typename DescT> inline
UInt32 RTXCacheKD<DescT>::getNumNodes(void)
{
    return _uiNextFreeNode;
}

template<typename DescT> inline
boxVolume_t RTXCacheKD<DescT>::getRootBoxVolume(void)
{
    return boxVolumeStore[0];
}

template<typename DescT> inline
const RTCachePrimIdxStore &
    RTXCacheKD<DescT>::getPrimitiveIndexList(UInt32 nodePrimIdx)
{
    return this->_mfPrimitives[nodePrimIdx];
}

template<typename DescT> inline
UInt32 RTXCacheKD<DescT>::getNumTriangles(void)
{
    return this->_mfTriangleAcc.size();
}

template<typename DescT> inline
UInt32 RTXCacheKD<DescT>::getPrimitiveListSize(void)
{
    return this->_mfPrimitives.size();
}

OSG_END_NAMESPACE
