/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

template<class HeightType, class HeightDeltaType, class TextureType> inline
BbqOutOfCoreEngine<HeightType, 
                   HeightDeltaType,
                   TextureType    >::BbqOutOfCoreEngine(void) :
     Inherited            (),
    _oInput               (),
    _oHeader              (),
    _oStaticNodeData      (),
    _oResidualDecompressor(),
    _vResidualBuffer      ()
{
}

template<class HeightType, class HeightDeltaType, class TextureType> inline
BbqOutOfCoreEngine<HeightType, 
                   HeightDeltaType,
                   TextureType    >::~BbqOutOfCoreEngine(void)
{
}

template<class HeightType, class HeightDeltaType, class TextureType> inline
bool BbqOutOfCoreEngine<HeightType, 
                        HeightDeltaType,
                        TextureType    >::initialize(
    const std::string &szFilename, 
          Real32       fHeightScale, 
          Real32       fHeightOffset, 
          Real32       fSampleSpacing)
{
    if(!_oInput.open(szFilename))
    {
        // file not found / invalid..
        // todo: output a warning/message
        return false;
    }
    
    // initialize the nodes in the tree:
    _oHeader = _oInput.getHeader();
    
    _oStaticNodeData.resize(_oHeader._iNodeCount);
    
    //const int virtualSize = getQuadtreeLeafNodeCount(
    //header_.heightSampleCountX - 1, header_.heightSampleCountY - 1,
    //header_.heightTileSize - 1 ) * ( header_.heightTileSize - 1 ) + 1; 

    //buildNodesRec( 0, 0, 0, virtualSize, virtualSize, 0 );

    // load the node info from the file:
    for(Int32 i = 0; i < _oHeader._iNodeCount; ++i)
    {
        _oInput.readNodeInfo(_oStaticNodeData[i]);
    }
    
    _oInformation.levelCount         = _oHeader._iLevelCount;
    _oInformation.nodeCount          = _oHeader._iNodeCount;
    _oInformation.heightTileSize     = _oHeader._iHeightTileSize;
    _oInformation.textureTileSize    = _oHeader._iTextureTileSize;

    _oInformation.heightSampleCount  = Vec2i(_oHeader._iHeightSampleCountX, 
                                             _oHeader._iHeightSampleCountY );
    _oInformation.textureSampleCount = Vec2i(_oHeader._iTextureSampleCountX, 
                                             _oHeader._iTextureSampleCountY);
    _oInformation.heightType         = _oHeader._eHeightType;
    _oInformation.textureType        = _oHeader._eTextureType;
    _oInformation.textureFormat      = _oHeader._eTextureFormat;
    _oInformation.heightFormat       = _oHeader._eHeightFormat;

    _oInformation.heightScale        = fHeightScale;
    _oInformation.heightOffset       = fHeightOffset;
    _oInformation.sampleSpacing      = fSampleSpacing;
    
    _oResidualDecompressor.initialize(&_oInput, 
                                        ResidualCompression::QuantizedHuffman, 
                                       _oHeader._uiHeightCompressionQuality, 
                                       _oHeader._iHeightTileSize            );
    
    return true;
}

template<class HeightType, class HeightDeltaType, class TextureType> inline
bool BbqOutOfCoreEngine<HeightType, 
                        HeightDeltaType,
                        TextureType    >::onLoadNodeData(
    BbqTerrainNodeBase &oNode)
{
    assert(isValidNodeId(oNode.id, (Int32) _oStaticNodeData.size()));
    
    BbqTerrNode *pNode = dynamic_cast<BbqTerrNode *>(&oNode);

    OSG_ASSERT(pNode != NULL);

    FileNode &staticNodeData = _oStaticNodeData[oNode.id];
    
    if(!_oInput.setPosition(staticNodeData._iDataPointer))
    {
        return false;
    }
    
    const FileNode &fileNode   = _oStaticNodeData[oNode.id];
    
    const Int32 heightSampleCount = osgSqr(_oInformation.heightTileSize);
    
    pNode->data.heightData.resize(heightSampleCount);
    
    // depending on the format of the file (compressed/uncompressed):
    // the root node is uncompressed!!

    if(oNode.id == 0 || _oHeader._eHeightFormat == BbqFile::AbsoluteValues)
    {
        const Int32 heightSampleCount = osgSqr(_oHeader._iHeightTileSize);
        
        if(!_oInput.readData(&(pNode->data.heightData[0]),
                              sizeof(UInt16) * heightSampleCount))
        {
            return false;
        }
    }
    else if(_oHeader._eHeightFormat == BbqFile::CompressedResiduals)
    {
        // decompress residuals         
        if(!_oResidualDecompressor.decompressResiduals(_vResidualBuffer))
        {
            return false;
        }

#ifdef CHECK_GV
        // recreate the height data to prevent error accumulation:
        ResidualCompression::createChildHeightData( 
             pNode->data.heightData, 
            &(_vResidualBuffer[0]), 
            &(pNode->parent->data.heightData[0]),
              _oInformation.heightTileSize, 
              getQuadtreeChildIndex(oNode.id));
#endif
    }
    
    if((fileNode._uiFlags & BbqFile::TextureDataValid) != 0)
    {
        // load the texture data:
        if(_oHeader._eTextureFormat == BbqFile::Dxt1)
        {
            // compressed texture..
            const Int32 textureSize   = _oHeader._iTextureTileSize;
            const Int32 blockCount    = osgSqr(textureSize / 4);
            const Int32 bytesPerBlock = 8;
            
            pNode->data.textureData.resize(bytesPerBlock * blockCount);
            
            if(!_oInput.readData(&(pNode->data.textureData[0]), 
                                 (Int32) pNode->data.textureData.size()))
            {
                return false;
            }
        }
        else
        {
            assert(_oHeader._eTextureFormat == BbqFile::RGB8);
            
            pNode->data.textureData.resize( 
                3 * osgSqr(_oHeader._iTextureTileSize));
            
            if(!_oInput.readData(&(pNode->data.textureData[0]), 
                                 (Int32) pNode->data.textureData.size()))
            {
                return false;
            }               
        }
    }
    else
    {
        pNode->data.textureData.resize(0);
    }
    
    // compute the object space bounding box:
    pNode->maxHeightError = fileNode._iMaxHeightError;
    
    computeBoundingBox(oNode, 
                       Real32(fileNode._iMinHeightSample) / 
                           TypeTraits<HeightType>::getMax(), 
                       Real32(fileNode._iMaxHeightSample) / 
                           TypeTraits<HeightType>::getMax());
    
    return true;
}


OSG_END_NAMESPACE
