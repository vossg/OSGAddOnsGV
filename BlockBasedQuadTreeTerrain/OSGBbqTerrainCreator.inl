/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2007 by the OpenSG Forum                   *
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
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Notes                                      *
 *                                                                           *
 * Implementation based on the original thesis work by Julien Koenen         *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

OSG_BEGIN_NAMESPACE


template<class HeightType, class TextureType> inline
BbqCreateEngine<HeightType, TextureType>::BbqCreateEngine(void) :
     Inherited             (),
    _oCreationDataAllocator(),
    _vQuadtree             ()
{
}

template<class HeightType, class TextureType> inline
BbqCreateEngine<HeightType, TextureType>::~BbqCreateEngine(void)
{
}

template<class HeightType, class TextureType> inline
bool BbqCreateEngine<HeightType, TextureType>::start(
    ImageBlockAccessor *pHeightFieldImage,
    ImageBlockAccessor *pTextureImage,
    BbqFileWriter      *pOutputFile,
    Int32               iTileSize, 
    Int32               iTextureSize     )
{
    Inherited::start(pHeightFieldImage,
                     pTextureImage,
                     pOutputFile,
                     iTileSize,
                     iTextureSize);

    const Int32 iNodeCount = getFullQuadtreeNodeCount(_iLevelCount);
    
    _vQuadtree.resize(iNodeCount);
    
    const Int32 iVirtualSize = 
        getQuadtreeLeafNodeCount(_vHeightSampleCount.x() - 1, 
                                 _vHeightSampleCount.y() - 1, 
                                 _iTileSize              - 1 ) * 
        (_iTileSize - 1 ) + 1;

    // build quadtree:
    initializeNodesRec(0, 0, 0, iVirtualSize, iVirtualSize, 0);
    
    GeoReferenceAttachmentPtr pGeoRef = 
        pHeightFieldImage->getGeoRef();

    if(pGeoRef != NullFC)
    {
        fprintf(stderr, "Got GeoRef\n");

        fprintf(stderr, "%d \n%f %f\n%f %f | %f %f\n",
                pGeoRef->getDatum(),
                pGeoRef->getEllipsoidAxis()[0],
                pGeoRef->getEllipsoidAxis()[1],
                pGeoRef->getOrigin()[0],
                pGeoRef->getOrigin()[1],
                pGeoRef->getPixelSize()[0],
                pGeoRef->getPixelSize()[1]);
    }

    // 2. step: write the file header
    BbqFile::BbqFileHeader header;
    
    header._iHeightSampleCountX   = _vHeightSampleCount .x();
    header._iHeightSampleCountY   = _vHeightSampleCount .y();
    header._iTextureSampleCountX  = _vTextureSampleCount.x();
    header._iTextureSampleCountY  = _vTextureSampleCount.y();
    header._fHeightOffset         =  0.0f;
    header._fHeightScale          =  1.0f;
    header._iHeightTileSize       = _iTileSize;
    header._iTextureTileSize      = _iTextureSize;
    header._iNodeCount            =  iNodeCount;
    header._iLevelCount           = _iLevelCount;
    header._fSampleSpacing        =  1.0f;
    header._eHeightType           =  pHeightFieldImage->getType();

    if(_pTextureImage != NULL)
    {
        header._eTextureType          =  pTextureImage->getType();
    }
    else
    {
        header._eTextureType          =  Image::OSG_INVALID_IMAGEDATATYPE;
    }

    header._eHeightFormat         =  BbqFile::AbsoluteValues;
    header._eTextureFormat        =  BbqFile::RGB8;
    header._uiDatum               =  pGeoRef->getDatum();
    header._vEllipsoidAxis        =  pGeoRef->getEllipsoidAxis();
    header._vOrigin               =  pGeoRef->getOrigin();
    header._vPixelSize            =  pGeoRef->getPixelSize();

    _pOutputFile->startWriting(header);
    
    // 3. step: write the hollow tree to the file:
    BbqFileNode<HeightType> emptyInfo;
    
    emptyInfo._uiFlags          = 0;
    emptyInfo._iMaxHeightError  = 0;
    emptyInfo._iMinHeightSample = 0;
    emptyInfo._iMaxHeightSample = 0;
    emptyInfo._iDataPointer     = 0;
    
    for(Int32 i = 0; i < header._iNodeCount; ++i)
    {
        _pOutputFile->writeNodeInfo(emptyInfo);
    }
    
    // prepare the data cache:
    
    // todo: calculate the maximum needed number of entries.. 
    // 4 * levelCount_ seems enough..

    _oCreationDataAllocator.setSize(4 * _iLevelCount);
    
    for(BbqCreationNodeDataAllocatorIt it  = _oCreationDataAllocator.begin(); 
                                       it != _oCreationDataAllocator.end  (); 
                                     ++it )
    {
        BbqCreationNodeData &element = *it;
        
        element.heightDataValid  = false;
        element.textureDataValid = false;

        element.heightData .resize(    _iTileSize    * _iTileSize   );

        if(_pTextureImage != NULL)
            element.textureData.resize(3 * _iTextureSize * _iTextureSize);
    }
    
    // we can start now...
    _oNodeIterator.startIteration(iNodeCount);

    return true;
}

template<class HeightType, class TextureType> inline
void BbqCreateEngine<HeightType, TextureType>::step(void)
{
    // do N nodes per step:
    Int32 iLoopCount = 100;
    
    // recursiv descent:
    while(!isFinished() && iLoopCount-- > 0)
    {
        // 4. step: do a bottom-up processing of the tree and calculate local 
        // errors (get 4 nodes + their parent node, 
        //    load the heightdata, calculate the max error, write the data to 
        // the output file (remembering the fileoffset)
        //    and continue. (propagate the error upwards) 
        // (do the same for the texture (and compress the texture)

        const BbqNodeId nodeId = _oNodeIterator.getCurrentNode();

        if(_oNodeIterator.isCurrentNodeBottomUp())
        {
            // we are on the way up again.. process this node 
            // (and pop it from the stack)

            processNode(nodeId);
        }
        
        _oNodeIterator.step();
    }

    if(isFinished())
    {
        // if we are finished:
        // 5. step: write the table of contents (the dataoffsets)

        _pOutputFile->gotoNodeInfoTable();
        
        const Int32 nodeCount = (Int32) _vQuadtree.size();

        for(Int32 i = 0; i < nodeCount; ++i)
        {
            const BbqCreationNode &node = _vQuadtree[i];
            
            BbqFileNode<HeightType> nodeInfo;
            
            nodeInfo._uiFlags = 0;
            
            if(node.hasTextureData)
            {
                nodeInfo._uiFlags |= BbqFile::TextureDataValid;
            }               
            
            nodeInfo._iDataPointer        = node.fileDataPointer;
            nodeInfo._iMaxHeightError     = node.maxHeightError;
            nodeInfo._iMinHeightSample    = node.minHeightSample;
            nodeInfo._iMaxHeightSample    = node.maxHeightSample;
            
            _pOutputFile->writeNodeInfo( nodeInfo );
        }
    }
    
    // todo: where in this process fits the compression of the height values?
    // we need a second pass to rearrange+compress the data. 
    // compress the data top-bottom and calculate the residuals to the
    // decompressed values (to prevent error-summation)  
}


//-----------------------------------------------------------------------------

template<class HeightType, class TextureType> inline
bool BbqCreateEngine<HeightType, TextureType>::isLeafNodeId(BbqNodeId iId) const
{
    // if the first child is invalid this has to be a leaf node:
    return 4 * iId + 1 >= (Int32) _vQuadtree.size();
}


//-----------------------------------------------------------------------------


template<class HeightType, class TextureType> inline
void BbqCreateEngine<HeightType, TextureType>::initializeNodesRec(
    BbqNodeId iId, 
    Int32     iX0, 
    Int32     iY0, 
    Int32     iX1, 
    Int32     iY1, 
    Int32     iLevel) 
{
    // check, if this is a leaf node:
    const bool hasChilds = iX1 - iX0 > _iTileSize;
    
    if(hasChilds)
    {
        // first build the children:
        
        int xm = (iX0 + iX1 - 1) / 2;
        int ym = (iY0 + iY1 - 1) / 2;
        
        BbqNodeId firstChildId = getChildNodeId(iId, 0);
        
        initializeNodesRec(firstChildId + 0, 
                           iX0,    iY0, 
                           xm + 1, ym + 1, 
                           iLevel + 1 );
        
        initializeNodesRec(firstChildId + 1, 
                           xm,  iY0, 
                           iX1, ym + 1, 
                           iLevel + 1 );
        
        initializeNodesRec(firstChildId + 2, 
                           iX0,    ym, 
                           xm + 1, iY1, 
                           iLevel + 1 );
        
        initializeNodesRec(firstChildId + 3, 
                           xm,  ym, 
                           iX1, iY1, 
                           iLevel + 1);
    }
    
    // fill the node structure with the static data:
    BbqCreationNode &node = _vQuadtree[iId];        
    
    node.sampleOrigin.setValues(iX0, iY0);
    node.sampleTarget.setValues(iX1, iY1);

    node.blockOrigin.setValues(iX0 - Real32(_vHeightSampleCount.x()) / 2.0f,  
                               iY0 - Real32(_vHeightSampleCount.y()) / 2.0f);

    node.treeLevel          = iLevel;
    node.sampleStepSize     = 1 << (_iLevelCount - iLevel - 1);
    node.hasTextureData     = false;
    node.maxHeightError     = 0;
    node.minHeightSample    = 0;
    node.maxHeightSample    = 0;
    node.fileDataPointer    = 0;
    node.data               = 0;
}


//-----------------------------------------------------------------------------


template<class HeightType, class TextureType> inline
void BbqCreateEngine<HeightType, TextureType>::processNode(BbqNodeId iId)
{
    BbqCreationNode &node = _vQuadtree[iId];
    
    // this is a new node.. we visit every node once.. so the data pointer
    // should be invalid 

    assert(!node.data);
    
    // and we should have at least one left in the cache:
    assert(!_oCreationDataAllocator.isEmpty());
    
    // get a valid data pointer
    node.data = _oCreationDataAllocator.allocate();
    
    // the data is not valid yet:
    node.data->heightDataValid  = false;
    node.data->textureDataValid = false;
    
    // check, if this is a leaf node for the texture:
    const bool isLeafTextureNode = node.treeLevel == _iTextureLeafLevel;
    
    // these might be invalid (if this is a leaf node)
    const BbqNodeId childId00 = getChildNodeId(iId, 0);
    const BbqNodeId childId10 = getChildNodeId(iId, 1);
    const BbqNodeId childId01 = getChildNodeId(iId, 2);
    const BbqNodeId childId11 = getChildNodeId(iId, 3);
    
    if(_pTextureImage != NULL)
    {
        if(isLeafTextureNode)
        {
            // get the texture from the image
            getTextureData(node);
        }
        else if(node.treeLevel < _iTextureLeafLevel)
        {
            // else: combine the 4 child textures to one texture (subsampling)
            BbqCreationNode &childNode00 = _vQuadtree[childId00];
            BbqCreationNode &childNode10 = _vQuadtree[childId10];
            BbqCreationNode &childNode01 = _vQuadtree[childId01];
            BbqCreationNode &childNode11 = _vQuadtree[childId11];
            
            combineTextureData(node, 
                               childNode00, 
                               childNode10, 
                               childNode01, 
                               childNode11);
        }
    }

    // is this a leaf node?
    if(isLeafNodeId(iId))
    {
        // get the height data from the heightfield image:
        getHeightData(node);
        
        // and the texture from the texture image (only, if we reached the
        // target resolution for textures) 
        
        // the error ist 0:
        node.maxHeightError = 0;
    }
    else
    {
        // if this is no leaf node we have 4 child nodes that contain data
        // already: get the data for the children: 

        BbqCreationNode &childNode00 = _vQuadtree[childId00];
        BbqCreationNode &childNode10 = _vQuadtree[childId10];
        BbqCreationNode &childNode01 = _vQuadtree[childId01];
        BbqCreationNode &childNode11 = _vQuadtree[childId11];
        
        // has to be valid..
        assert(childNode00.data && 
               childNode01.data && 
               childNode10.data && 
               childNode11.data );
        
        // combine the height data from the children:
        combineHeightData(node, 
                          childNode00, 
                          childNode10, 
                          childNode01, 
                          childNode11 );
        
        // compute the error: (todo: this should happen in the
        //combineHeightData function) 
        //compute
        
        // we dont need the data for the 4 child nodes anymore: release the
        // data nodes to the cache 

        _oCreationDataAllocator.free(childNode00.data);
        _oCreationDataAllocator.free(childNode10.data);
        _oCreationDataAllocator.free(childNode01.data);
        _oCreationDataAllocator.free(childNode11.data);
        
        // and (for safety reasons) put a zero in the data field:
        childNode00.data = 0;
        childNode10.data = 0;
        childNode01.data = 0;
        childNode11.data = 0;
    }

    // we have our height data. now write the data into the file and remember
    // the position: 

    if(node.data->heightDataValid)
    {
        // determine min/max height sample:
        node.minHeightSample = node.data->heightData[0];
        node.maxHeightSample = node.data->heightData[0];
        
        const Int32 heightSampleCount = (Int32) node.data->heightData.size();
        
        for(Int32 i = 1; i < heightSampleCount; ++i)
        {
            node.minHeightSample = std::min(node.minHeightSample, 
                                            node.data->heightData[i]);
            node.maxHeightSample = std::max(node.maxHeightSample, 
                                            node.data->heightData[i]);
        }

        node.fileDataPointer = _pOutputFile->getCurrentPosition();

        _pOutputFile->writeData( 
            &node.data->heightData[0], 
             sizeof(HeightType) * (Int32) node.data->heightData.size());
    }
    
    // write the texture data (if any)
    if(node.data->textureDataValid)
    {
        _pOutputFile->writeData(&node.data->textureData[0], 
                                (Int32) node.data->textureData.size());
        
        node.hasTextureData = true;
    }
    else
    {
        node.hasTextureData = false;
    }
    
    // we keep the data for this node because the parent node will need it to
    // create his data... 
}


//-----------------------------------------------------------------------------


template<class HeightType, class TextureType> inline
void BbqCreateEngine<HeightType, TextureType>::getTextureData(
    BbqCreationNode &oNode)
{
    // this is a leaf node.. 
    // check the resolution:
    
    // todo: support scaling here:
    assert(oNode.sampleTarget.x() - 
           oNode.sampleOrigin.x() - 1 == _iTextureSize);

    assert(oNode.sampleTarget.y() - 
           oNode.sampleOrigin.y() - 1 == _iTextureSize);
    
    // it is possible, that we need to clip the range here for heightfields
    // that are not power of two + 1 
    
    // copy the data from the block directly into the data:
    oNode.data->textureDataValid = 
        _pTextureImage->readBlockRGB(  oNode.sampleOrigin, 
                                      _iTextureSize, 
                                     &oNode.data->textureData[0], 
                                     (Int32) oNode.data->textureData.size());
}

//-----------------------------------------------------------------------------


template<class HeightType, class TextureType> inline
void BbqCreateEngine<HeightType, TextureType>::getHeightData(
    BbqCreationNode &oNode)
{
    // this is a leaf node.. 
    // check the resolution:

    assert(oNode.sampleStepSize                            == 1         );
    assert(oNode.sampleTarget.x() - oNode.sampleOrigin.x() == _iTileSize);
    assert(oNode.sampleTarget.y() - oNode.sampleOrigin.y() == _iTileSize);
    
    // it is possible, that we need to clip the range here for heightfields
    // that are not power of two + 1 
    
    // copy the data from the block directly into the data:
    oNode.data->heightDataValid = 
        _pHeightFieldImage->readBlockA16( 
             oNode.sampleOrigin, 
            _iTileSize,
            &oNode.data->heightData[0], 
            2 * (Int32) oNode.data->heightData.size());

    for(UInt32 i = 0; i < oNode.data->heightData.size(); ++i)
    {
        if(oNode.data->heightData[i] == -32768)
            oNode.data->heightData[i] = 0;
    }
}

//-----------------------------------------------------------------------------


template<class HeightType, class TextureType> inline
void BbqCreateEngine<HeightType, TextureType>::combineHeightData(
    BbqCreationNode &oNode, 
    BbqCreationNode &oChildNode00, 
    BbqCreationNode &oChildNode10, 
    BbqCreationNode &oChildNode01, 
    BbqCreationNode &oChildNode11 )
{
    // subsample each childnode:
    //fillHeightData( node );
    
    subSampleHeightData(&oNode       .data->heightData[0], 
                        &oChildNode00.data->heightData[0], 
                         0, 
                         0, 
                        _iTileSize);

    subSampleHeightData(&oNode       .data->heightData[0], 
                        &oChildNode10.data->heightData[0], 
                        _iTileSize / 2, 
                         0, 
                        _iTileSize);

    subSampleHeightData(&oNode       .data->heightData[0], 
                        &oChildNode01.data->heightData[0], 
                         0, 
                        _iTileSize / 2, 
                        _iTileSize);

    subSampleHeightData(&oNode       .data->heightData[0], 
                        &oChildNode11.data->heightData[0], 
                        _iTileSize / 2, 
                        _iTileSize / 2, 
                        _iTileSize);

    oNode.data->heightDataValid = true;
}

//-----------------------------------------------------------------------------

template<class HeightType, class TextureType> inline
void BbqCreateEngine<HeightType, TextureType>::combineTextureData(
    BbqCreationNode &oNode, 
    BbqCreationNode &oChildNode00, 
    BbqCreationNode &oChildNode10, 
    BbqCreationNode &oChildNode01, 
    BbqCreationNode &oChildNode11)
{
    subSampleTextureData(&oNode       .data->textureData[0], 
                         &oChildNode00.data->textureData[0], 
                          0, 
                          0, 
                         _iTextureSize );

    subSampleTextureData(&oNode       .data->textureData[0], 
                         &oChildNode10.data->textureData[0], 
                         _iTextureSize / 2, 
                          0, 
                         _iTextureSize );

    subSampleTextureData(&oNode       .data->textureData[0], 
                         &oChildNode01.data->textureData[0], 
                          0, 
                         _iTextureSize / 2, 
                         _iTextureSize );

    subSampleTextureData(&oNode       .data->textureData[0], 
                         &oChildNode11.data->textureData[0], 
                          _iTextureSize / 2, 
                          _iTextureSize / 2, 
                          _iTextureSize );

    oNode.data->textureDataValid = true;
}


//-----------------------------------------------------------------------------


template<class HeightType, class TextureType> inline
void BbqCreateEngine<HeightType, TextureType>::subSampleHeightData(
          HeightType *       pTarget, 
    const HeightType * const pSource, 
          Int32              iX0, 
          Int32              iY0, 
          Int32              iSize )
{
    //todo: yes, i know.. i write over 4 pixel rows twice.. but im too lazy
    //right now.. and this is changing completely if i write residuals here.. 
    //todo: hmm.. should i use a filtering method here?

    Int32 iHalfSize = iSize / 2 + 1;
    
    for(Int32 y = 0; y < iHalfSize; ++y )
    {
              HeightType *pTargetPtr = pTarget + ( iY0 + y ) * iSize + iX0;

        // every other row..
        const HeightType *pSourcePtr = pSource + 2 * y * iSize;
        
        for(Int32 x = 0; x < iHalfSize; ++x)
        {
            *pTargetPtr++ = *pSourcePtr;
            
            // every other column.. subsampling by two
            pSourcePtr += 2;
        }
    }
}


//-----------------------------------------------------------------------------


template<class HeightType, class TextureType> inline
void BbqCreateEngine<HeightType, TextureType>::subSampleTextureData(
          TextureType *       pTarget, 
    const TextureType * const pSource, 
          Int32               iX0, 
          Int32               iY0, 
          Int32               iSize )
{
    // todo: it seams that there is a bug here... (textures jumping around)
    
    //todo: yes, i know.. i write over 4 pixel rows twice.. but im too lazy
    //right now.. and this is changing completely if i write residuals here.. 
    //todo: hmm.. should i use a filtering method here?

    Int32 iHalfSize = iSize / 2;
    
    for(Int32 y = 0; y < iHalfSize; ++y)
    {
        TextureType *pTargetPtr = pTarget + 3 * ((iY0 + y) * iSize + iX0);

        // every other row..
        const TextureType *pSourcePtr = pSource + 2 * 3 * (y * iSize); 
        
        for(Int32 x = 0; x < iHalfSize; ++x)
        {
            // one pixel of rgb:
            *pTargetPtr++ = *pSourcePtr++;
            *pTargetPtr++ = *pSourcePtr++;
            *pTargetPtr++ = *pSourcePtr++;
            
            // every other column.. subsampling by two
            pSourcePtr += 3;
        }
    }
}



OSG_END_NAMESPACE
