/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include <OSGConfig.h>

#include "OSGBbqOutOfCoreDataSource.h"
#include "OSGBbqNode.h"
#include "OSGBbqTerrainNode.h"

#include "OSGBaseFunctions.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGBbqOutOfCoreDataSourceBase.cpp file.
// To modify it, please change the .fcd file (OSGBbqOutOfCoreDataSource.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void BbqOutOfCoreDataSource::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
    }
}


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

BbqOutOfCoreDataSource::BbqOutOfCoreDataSource(void) :
     Inherited            (),
    _oInput               (),
    _oHeader              (),
    _oStaticNodeData      (),
    _oResidualDecompressor(),
    _vResidualBuffer      ()
{
}

BbqOutOfCoreDataSource::BbqOutOfCoreDataSource(
    const BbqOutOfCoreDataSource &source) :

     Inherited            (source),
    _oInput               (),
    _oHeader              (),
    _oStaticNodeData      (),
    _oResidualDecompressor(),
    _vResidualBuffer      ()
{
}

BbqOutOfCoreDataSource::~BbqOutOfCoreDataSource(void)
{
}

/*----------------------------- class specific ----------------------------*/

void BbqOutOfCoreDataSource::changed(ConstFieldMaskArg whichField, 
                                     UInt32            origin,
                                     BitVector         details)
{
    Inherited::changed(whichField, origin, details);

    if(0x0000 != (whichField & (FilenameFieldMask     |
                                HeightScaleFieldMask  |
                                HeightOffsetFieldMask |
                                SampleSpacingFieldMask)))
    {
        initialize(getFilename().c_str(),
                   getHeightScale  (),
                   getHeightOffset (),
                   getSampleSpacing());
        
        
        const BbqDataSourceInformation &bbqInfo = 
            this->getInformation();
        
        
        fprintf(stderr, "%d %d\n",
                bbqInfo.levelCount,
                bbqInfo.nodeCount);
        
        fprintf(stderr, "%d %d %d\n",
                bbqInfo.heightTileSize,
                bbqInfo.heightSampleCount[0],
                bbqInfo.heightSampleCount[1]);
        
        fprintf(stderr, "%f %f %f\n",
                bbqInfo.heightScale,
                bbqInfo.heightOffset,
                bbqInfo.sampleSpacing);
    }
}

void BbqOutOfCoreDataSource::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump BbqOutOfCoreDataSource NI" << std::endl;
}


bool BbqOutOfCoreDataSource::initialize(const std::string &szFilename, 
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


//-----------------------------------------------------------------------------


bool BbqOutOfCoreDataSource::onLoadNodeData(BbqTerrainNode &oNode)
{
    assert(isValidNodeId(oNode.id, (Int32) _oStaticNodeData.size()));
    
    BbqFileNode &staticNodeData = _oStaticNodeData[oNode.id];
    
    if(!_oInput.setPosition(staticNodeData._iDataPointer))
    {
        return false;
    }
    
    const BbqFileNode &fileNode   = _oStaticNodeData[oNode.id];
    
    const Int32 heightSampleCount = osgSqr(_oInformation.heightTileSize);
    
    oNode.data.heightData.resize(heightSampleCount);
    
    // depending on the format of the file (compressed/uncompressed):
    // the root node is uncompressed!!

    if(oNode.id == 0 || _oHeader._eHeightFormat == BbqFile::AbsoluteValues)
    {
        const Int32 heightSampleCount = osgSqr(_oHeader._iHeightTileSize);
        
        if(!_oInput.readData(&(oNode.data.heightData[0]),
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

        // recreate the height data to prevent error accumulation:
        ResidualCompression::createChildHeightData( 
             oNode.data.heightData, 
            &(_vResidualBuffer[0]), 
            &(oNode.parent->data.heightData[0]),
              _oInformation.heightTileSize, 
              getQuadtreeChildIndex(oNode.id));
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
            
            oNode.data.textureData.resize(bytesPerBlock * blockCount);
            
            if(!_oInput.readData(&(oNode.data.textureData[0]), 
                                 (Int32) oNode.data.textureData.size()))
            {
                return false;
            }
        }
        else
        {
            assert(_oHeader._eTextureFormat == BbqFile::RGB8);
            
            oNode.data.textureData.resize( 
                3 * osgSqr(_oHeader._iTextureTileSize));
            
            if(!_oInput.readData(&(oNode.data.textureData[0]), 
                                 (Int32) oNode.data.textureData.size()))
            {
                return false;
            }               
        }
    }
    else
    {
        oNode.data.textureData.resize(0);
    }
    
    // compute the object space bounding box:
    oNode.maxHeightError = fileNode._iMaxHeightError;
    
    computeBoundingBox(oNode, 
                       Real32(fileNode._iMinHeightSample) / 65535.0f, 
                       Real32(fileNode._iMaxHeightSample) / 65535.0f);
    
    return true;
}

OSG_END_NAMESPACE
