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

#include "OSGBbqTerrainCompressor.h"
#include "OSGBbqNode.h"

#include "OSGSquish.h"

OSG_BEGIN_NAMESPACE


BbqTerrainCompressor::Statistics::Statistics(void) :
    originalHeightDataSize   (0  ),
    compressedHeightDataSize (0  ),
    originalTextureDataSize  (0  ),
    compressedTextureDataSize(0  ),
    maxAbsoluteError         (0  ),
    bitsPerHeightSample      (0.f)
{
}

BbqTerrainCompressor::Statistics::~Statistics(void)
{
}

//-----------------------------------------------------------------------------


BbqTerrainCompressor::BbqTerrainCompressor(void) :
    _oInputFile              ( ),
    _oInputHeader            ( ),
    _oOutputFile             ( ),
    _vNodeData               ( ),      
    _oNodeIterator           ( ),
    _oCompressorDataAllocator( ),
    _vResidualsBuffer        ( ),
    _oStatistics             ( ),
    _iCompressionQuality     (0),
    _oResidualCompressor     ( )
{
}
 
BbqTerrainCompressor::~BbqTerrainCompressor(void)
{
}
    

//-----------------------------------------------------------------------------


bool BbqTerrainCompressor::start(const std::string &szInputBbqFilename, 
                                 const std::string &szOutputFilename, 
                                       Int32        iCompressionQuality)
{
    _iCompressionQuality = clamp(iCompressionQuality, 1, 1000);
    
    if(!_oInputFile.open(szInputBbqFilename))
    {
        // file not found / invalid..
        // todo: output a warning/message
        return false;
    }

    if(!_oOutputFile.open(szOutputFilename))
    {
        return false;
    }
    
    _oInputHeader = _oInputFile.getHeader();
    
    const bool heightDataIsCompressed = 
        _oInputHeader._eHeightFormat == BbqFile::CompressedResiduals;

    const bool textureDataIsCompressed = 
        _oInputHeader._eTextureFormat == BbqFile::Dxt1;

    if(heightDataIsCompressed || textureDataIsCompressed)
    {
        // todo: output a error message: already compressed
        return false;
    }
    
    // write the file header
    BbqFile::BbqFileHeader outputHeader = _oInputHeader;
    
    outputHeader._eTextureFormat             = BbqFile::Dxt1;
    outputHeader._eHeightFormat              = BbqFile::CompressedResiduals;
    outputHeader._uiHeightCompressionQuality = _iCompressionQuality;
    
    if(!_oOutputFile.startWriting(outputHeader))
    {
        // todo: output a error message
        return false;
    }
    
    // fill the node data from the input file, and write the toc into the
    // outputfile: 

    BbqFileNode emptyInfo;
    
    emptyInfo._uiFlags            = 0;
    emptyInfo._iMaxHeightError    = 0;
    emptyInfo._iMinHeightSample   = 0;
    emptyInfo._iMaxHeightSample   = 0;
    emptyInfo._iDataPointer       = 0;
    
    _vNodeData.resize(_oInputHeader._iNodeCount);
    
    for(Int32 i = 0; i < _oInputHeader._iNodeCount; ++i)
    {
        if(!_oInputFile.readNodeInfo(_vNodeData[i].inputInfo))
        {
            return false;
        }

        _vNodeData[i].id                       = (BbqNodeId) i;
        _vNodeData[i].data                     = 0;
        _vNodeData[i].compressedDataPointer    = 0;
    }
    
    // write the dummy header:
    for(Int32 i = 0; i < _oInputHeader._iNodeCount; ++i)
    {
        if(!_oOutputFile.writeNodeInfo(emptyInfo))
        {
            return false;
        }
    }
    
    _oResidualCompressor.initialize(&_oOutputFile, 
                                     ResidualCompression::QuantizedHuffman, 
                                    _iCompressionQuality, 
                                    _oInputHeader._iHeightTileSize );

    // start the iteration:
    _oNodeIterator.startIteration(_oInputHeader._iNodeCount);
    
    _oCompressorDataAllocator.setSize(4 * _oInputHeader._iNodeCount);
    
    return true;
}


//-----------------------------------------------------------------------------


bool BbqTerrainCompressor::isFinished(void) const
{
    return _oNodeIterator.isFinished();
}


//-----------------------------------------------------------------------------


void BbqTerrainCompressor::step(void)
{
    // do N nodes per step:
    Int32 iLoopCount = 100;
    
    // recursive descent:
    
    while(!isFinished() && iLoopCount-- > 0)
    {
        BbqNodeId          nodeId     = _oNodeIterator.getCurrentNode();
        BbqCompressorNode &node       = _vNodeData[nodeId];
        BbqCompressorNode *parentNode = 0;
        
        if(nodeId > 0)
        {
            parentNode = &_vNodeData[(nodeId - 1) / 4];
        }
        
        if(!_oNodeIterator.isCurrentNodeBottomUp())
        {
            // top down..:
            if(!processNode(node, parentNode))
            {
                // todo: do something with the error..
            }
        }
        else
        {
            // bottom up.. free the data for this node
            assert(node.data);

            _oCompressorDataAllocator.free(node.data);
        }
        
        _oNodeIterator.step();
    }
    
    if(isFinished())
    {
        // close the output file
        _oOutputFile.gotoNodeInfoTable();
        
        const Int32 iNodeCount = (Int32) _vNodeData.size();

        for(Int32 i = 0; i < iNodeCount; ++i)
        {
            const BbqCompressorNode &node = _vNodeData[i];
            
            BbqFileNode outputNodeInfo = node.inputInfo;
            
            outputNodeInfo._iDataPointer  = node.compressedDataPointer;
            
            _oOutputFile.writeNodeInfo(outputNodeInfo);
        }
        
        const Real32 fHeightSampleCount = 
            (Real32)(_oInputHeader._iHeightSampleCountX * 
                     _oInputHeader._iHeightSampleCountY  );
        
        _oStatistics.bitsPerHeightSample = 
            8.0f * Real32 (_oStatistics.compressedHeightDataSize) / 
            (fHeightSampleCount);
    }
}


//-----------------------------------------------------------------------------


Real32 BbqTerrainCompressor::getProgress(void) const
{
    return _oNodeIterator.getProgress();
}


//-----------------------------------------------------------------------------


const BbqTerrainCompressor::Statistics &
    BbqTerrainCompressor::getStatistics(void) const
{
    return _oStatistics;
}


//-----------------------------------------------------------------------------


bool BbqTerrainCompressor::processNode(BbqCompressorNode &oNode, 
                                       BbqCompressorNode *pParentNode)
{
    // compress the node data, and write the compressed data into the file
    // (remembering the file offset) 
    // allocate a data node for this
    assert(!oNode.data);

    oNode.data = _oCompressorDataAllocator.allocate();

    assert(oNode.data);
    
    // fill the input data from the input file:
    oNode.data->heightData.resize(osgSqr(_oInputHeader._iHeightTileSize));

    const Int32 iHeightDataSize = 
        (Int32) (sizeof(oNode.data->heightData[0]) * 
                        oNode.data->heightData.size());

    if( !_oInputFile.setPosition( oNode.inputInfo._iDataPointer ) ||
        !_oInputFile.readData   (&(oNode.data->heightData[0]), 
                                   iHeightDataSize           )       )
    {
        // todo: output an error message:
        return false;
    }
    
    oNode.compressedDataPointer = _oOutputFile.getCurrentPosition();
    
    // if we have a parent:
    if(pParentNode != NULL)
    {
        assert(pParentNode->data);
        
        // compute the residuals to the parent: 
        // important note: take the residuals to the compressed and
        // decompressed data to prevent error accumulation  
        const Int32 iThisChildIndex = getQuadtreeChildIndex(oNode.id);
        
        Int32 iMaxResidual = 0;

        ResidualCompression::computeResiduals( 
              oNode.data->residuals, 
            &(oNode.data->heightData[0]), 
            &(pParentNode->data->decompressedHeightData[0]), 
            _oInputHeader._iHeightTileSize, 
             iThisChildIndex, 
             iMaxResidual                                  );
        
        pParentNode->inputInfo._iMaxHeightError = osgMax( 
            pParentNode->inputInfo._iMaxHeightError, 
            iMaxResidual);

        const Int32 iCompressedSize = _oResidualCompressor.compressResiduals( 
             oNode.data->residuals, 
            _vResidualsBuffer     );
            
        // update stats:
        _oStatistics.originalHeightDataSize += 
            (Int32) oNode.data->heightData.size() * 
            (Int32) sizeof(oNode.data->heightData[0]);

        _oStatistics.compressedHeightDataSize += iCompressedSize;
        
        // recreate the height data to prevent error accumulation:
        ResidualCompression::createChildHeightData( 
              oNode.data->decompressedHeightData, 
            &(_vResidualsBuffer[0]), 
            &(pParentNode->data->decompressedHeightData[0]),
            _oInputHeader._iHeightTileSize, 
             iThisChildIndex );
        
        // calculate the error introduced through this compression:
        const int error = getMaxDifference(oNode.data->heightData, 
                                           oNode.data->decompressedHeightData);

        _oStatistics.maxAbsoluteError = osgMax(_oStatistics.maxAbsoluteError, 
                                                error                       );
    }
    else
    {
        // no parent, we are the root node: just compress the data normally 
        // in this version we don't compress the root node height data at
        // all.. (its only 1k bytes) 
        if(!_oOutputFile.writeData(&(oNode.data->heightData[0]), 
                                     iHeightDataSize)          )
        {
            // todo: output an error message
            return false;
        }

        // just copy the data to the decompressed data field:
        oNode.data->decompressedHeightData = oNode.data->heightData;
    }
    
    // read the texture data:
    if((oNode.inputInfo._uiFlags & BbqFile::TextureDataValid) != 0)
    {
        // does this node has a texture?
        oNode.data->textureData.resize(3 * 
                                       osgSqr(_oInputHeader._iTextureTileSize));
        
        if(!_oInputFile.readData(&(oNode.data->textureData[0]), 
                                 (Int32) oNode.data->textureData.size()))
        {
            // todo: report an error
            return false;
        }
        
        // compress the texture data
        compressTextureData(oNode);
        
        // write the compressed texture to the output file
        _oOutputFile.writeData(
            &(oNode.data->compressedTextureData[0]), 
            (Int32) oNode.data->compressedTextureData.size());
        
        _oStatistics.originalTextureDataSize += 
            (Int32) oNode.data->textureData.size();

        _oStatistics.compressedTextureDataSize += 
            (Int32) oNode.data->compressedTextureData.size();
    }

    return true;
}


//-----------------------------------------------------------------------------


void BbqTerrainCompressor::compressTextureData(BbqCompressorNode &oNode)
{
    // dxt1 texture compression:
    
    // todo: check the texture size:
    const Int32 iTextureSize   = _oInputHeader._iTextureTileSize;
    const Int32 iBlockCount    = osgSqr(iTextureSize / 4);
    const Int32 iBytesPerBlock = 8;            // dxt1 compression

    assert(oNode.data);

    oNode.data->compressedTextureData.resize(iBytesPerBlock * iBlockCount);
    
    // r8g8b8 data:
    const Vec2i sourceStride(3, 3 * iTextureSize);
    
    // loop over blocks and compress them
    UInt8 *targetPtr = &(oNode.data->compressedTextureData[0]);
    
    for(Int32 y = 0; y < iTextureSize; y += 4)
    {
        // process a row of blocks
        for(Int32 x = 0; x < iTextureSize; x += 4)
        {
            // get the block data
            UInt8  blockBuffer[16 * 4];

            UInt8 *blockBufferPtr = &(blockBuffer[0]);
            
            for(Int32 py = 0; py < 4; ++py)
            {
                const UInt8 *sourceRowPtr = 
                    &(oNode.data->textureData[(y + py) * sourceStride.y() + 
                                              x * sourceStride.x()        ]);
                
                for(Int32 px = 0; px < 4; ++px)
                {
                    // copy each pixel over to the temporary block buffer:
                    for(Int32 i = 0; i < 3; ++i)
                    {
                        *blockBufferPtr++ = *sourceRowPtr++;
                    }
                    
                    // and write a dummy alpha value:
                    *blockBufferPtr++ = 255;
                }
            }
            
            // compress this block
            Int32 iFastFlags = (osgsquish::kDxt1 | 
                                osgsquish::kColourMetricPerceptual | 
                                osgsquish::kColourRangeFit);

            //int qualityFlags = squish::kDxt1 |
            //squish::kColourMetricPerceptual | squish::kColourClusterFit;
            
            osgsquish::Compress(blockBuffer, targetPtr, iFastFlags);
            
            // advance
            targetPtr += iBytesPerBlock;
        }
    }
}


//-----------------------------------------------------------------------------


Int32 BbqTerrainCompressor::getMaxDifference( 
    const std::vector<UInt16> &vHeightData0, 
    const std::vector<UInt16> &vHeightData1 )
{
    const Int32 iSize = (Int32) vHeightData0.size();
    
    Int32 iResult = 0;
    
    for(Int32 i = 0; i < iSize; ++i)
    {
        Int32 iDifference = (Int32) vHeightData0[i] - (Int32) vHeightData1[i];
        
        iResult = osgMax(iResult, iDifference);
    }

    return iResult;
}


OSG_END_NAMESPACE

