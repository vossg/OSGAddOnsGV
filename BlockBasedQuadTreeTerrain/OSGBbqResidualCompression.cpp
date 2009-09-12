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

#include "OSGBbqResidualCompression.h"
#include "OSGBbqFile.h"

#include "OSGBbqNode.h"
#include "OSGTerrainHelper.h"

#include <iostream>

#include "OSGHuffman.h"
#include "zlib.h"

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------

ResidualCompression::ResidualCompression(void)
{
}

ResidualCompression::~ResidualCompression(void)
{
}


int ResidualCompression::getResidualsCount(Int32 iTileSize)
{
    return osgSqr(iTileSize) - osgSqr(iTileSize) / 4;
}


//-----------------------------------------------------------------------------


void ResidualCompression::quantize(Int16 *pData, 
                                   size_t uiDataSize, 
                                   Int32  iQualityFactor)
{
    const float step    = ( 000.0f / (Real32) iQualityFactor);
    const float invStep = 1.0f / step;
    
    for(size_t i = 0; i < uiDataSize; ++i)
    {
        if(*pData > 0)
        {
            *pData = (Int16)(Real32(*pData) * invStep + 0.5);
        }
        else
        {
            *pData = (Int16)(Real32(*pData) * invStep - 0.5);
        }
        
        pData++;
    }
}


//-----------------------------------------------------------------------------


void ResidualCompression::dequantize(Int16  *pData, 
                                     size_t  uiDataSize, 
                                     Int32   iQualityFactor)
{
    const float step = (1000.0f / (Real32) iQualityFactor);
    
    for(size_t i = 0; i < uiDataSize; ++i)
    {
        *pData = (Int16)(*pData * step);
        
        pData++;
    }
}


//-----------------------------------------------------------------------------


void ResidualCompression::compressHuffman(std::vector<UInt8>      &vTarget, 
                                          Int16             const *pSource, 
                                          size_t                   uiSourceSize)
{
    // resize to the worst case buffer size:
    vTarget.resize(4 * uiSourceSize);
    
    Int32 iSize = Huffman_Compress((unsigned char *) pSource, 
                                   &(vTarget[0]), 
                                   (unsigned int)(2 * uiSourceSize));
    
    //todo: if size > sourceSize.. save the original data uncompressed or
    //something more sensible 
    
    // resize to the actual size:
    vTarget.resize(iSize);
}


//-----------------------------------------------------------------------------


void ResidualCompression::decompressHuffman(
    std::vector<Int16>       &vTarget, 
    UInt8              const *pSource, 
    size_t                    uiSourceSize)
{
    // target size has to be correct..
    assert(vTarget.size() > 0);
    
    Huffman_Uncompress((unsigned char *)  pSource, 
                       (unsigned char *) &(vTarget[0]), 
                       (unsigned int   )  uiSourceSize, 
                       (unsigned int   ) (2 * vTarget.size()));      
}


//-----------------------------------------------------------------------------


void ResidualCompression::compressZlib(std::vector<UInt8>       &vTarget, 
                                       Int16              const *pSource, 
                                       size_t                    uiSourceSize)
{
    vTarget.resize(compressBound((unsigned long) (2 * uiSourceSize)));
    
    uLongf uiSize = (uLongf) vTarget.size();

    compress(&(vTarget[0]), 
             &uiSize, 
             (unsigned char *) pSource, 
              uLongf(2 * uiSourceSize));
    
    vTarget.resize(uiSize);
}


//-----------------------------------------------------------------------------


void ResidualCompression::decompressZlib(std::vector<Int16>      &vTarget, 
                                         UInt8             const *pSource, 
                                         size_t                   uiSourceSize)
{
    uLongf destLen = 0;

    uncompress((unsigned char *) &(vTarget[0]), 
               &destLen,
               pSource, 
               uLongf(uiSourceSize));
}


//-----------------------------------------------------------------------------


void ResidualCompression::computeResiduals(
    std::vector<Int16>       &vResiduals, 
    UInt16             const *pHeightData, 
    UInt16             const *pParentData, 
    Int32                     iTileSize, 
    Int32                     iChildIndex, 
    Int32                    &iMaxResidual)
{
    vResiduals.resize(osgSqr(iTileSize) - osgSqr(iTileSize) / 4);
    
    const Vec2i vParentOffset = getParentTileOffset(iTileSize, iChildIndex);
    
    Int32 iResidualIndex = 0;
    
    // for each sample: 
    Int32 iParentY = vParentOffset.y();

    const int parentIncX = 1;
    const int parentIncY = iTileSize;

    iMaxResidual = 0;

    for(Int32 y = 0; y < iTileSize; ++y)
    {
        const bool yIsEven = y % 2 == 0;
        
        Int32 iParentX = vParentOffset.x();
        
        for(Int32 x = 0; x < iTileSize; ++x)
        {
            const bool xIsEven = x % 2 == 0;
            
            if(xIsEven && yIsEven)
            {
                // residual is zero here
                continue;
            }
            
            // there always has to be a greater sample:
            UInt16 uiHeightSample            = pHeightData[y * iTileSize + x];
            Int32  iParentFirstSampleOffset  = iParentY * iTileSize + iParentX;
            Int32  iParentSecondSampleOffset = 0;
            
            if(xIsEven)
            {
                assert(y < iTileSize - 1);
                
                // vertical
                iParentSecondSampleOffset = parentIncY;
            }
            else if(yIsEven)
            {
                assert(x < iTileSize - 1);
                
                // horizontal
                iParentSecondSampleOffset = parentIncX;
            }
            else
            {
                assert(x < iTileSize - 1);
                assert(y < iTileSize - 1);
                
                // diagonal
                iParentSecondSampleOffset = parentIncX + parentIncY;
            }
            
            UInt16 uiParentHeightSample0 = 
                pParentData[iParentFirstSampleOffset];

            UInt16 uiParentHeightSample1 = 
                pParentData[iParentFirstSampleOffset + 
                            iParentSecondSampleOffset];
            
            Int32 iExpectedHeightValue = lerp((Int32) uiParentHeightSample0, 
                                              (Int32) uiParentHeightSample1, 
                                              0.5f );

            Int32 iResidual = uiHeightSample - iExpectedHeightValue;
            
            iMaxResidual = osgMax(iMaxResidual, abs(iResidual));
            
            vResiduals[iResidualIndex++] = (Int16) iResidual;
            
            if(!xIsEven)
            {
                iParentX++;
            }
        }
        
        if(!yIsEven)
        {
            iParentY++;
        }
    }
}


//-----------------------------------------------------------------------------


// create the child 
void ResidualCompression::createChildHeightData( 
    std::vector<UInt16>       &vHeightData, 
    Int16               const *pResiduals, 
    UInt16              const *pParentData, 
    Int32                      iTileSize, 
    Int32                      iChildIndex)
{
    const Vec2i parentOffset = getParentTileOffset(iTileSize, iChildIndex);
    
    vHeightData.resize(iTileSize * iTileSize);
    
    Int32 iParentY = parentOffset.y();

    const Int32 iParentIncX = 1;
    const Int32 iParentIncY = iTileSize;
    
    Int32 iResidualIndex = 0;
    
    for(Int32 y = 0; y < iTileSize; ++y)
    {
        const bool yIsEven = y % 2 == 0;
        
        Int32 iParentX = parentOffset.x();
        
        for(Int32 x = 0; x < iTileSize; ++x)
        {
            const bool xIsEven = x % 2 == 0;
            
            Int32 iParentFirstSampleOffset  = iParentY * iTileSize + iParentX;
            Int32 iParentSecondSampleOffset = 0;

            UInt16 iParentHeightSample0 = 
                pParentData[iParentFirstSampleOffset];
            
            if(xIsEven && yIsEven)
            {
                // residual is zero here: use the parent value:
                vHeightData[y * iTileSize + x] = iParentHeightSample0;
                continue;
            }
            
            if(xIsEven)
            {
                assert(y < iTileSize - 1);
                
                // vertical
                iParentSecondSampleOffset = iParentIncY;
            }
            else if(yIsEven)
            {
                assert(x < iTileSize - 1);
                
                // horizontal
                iParentSecondSampleOffset = iParentIncX;
            }
            else
            {
                assert(x < iTileSize - 1);
                assert(y < iTileSize - 1);
                
                // diagonal
                iParentSecondSampleOffset = iParentIncX + iParentIncY;
            }
            
            UInt16 iParentHeightSample1 = 
                pParentData[iParentFirstSampleOffset + 
                            iParentSecondSampleOffset];
            
            Int32 iExpectedHeightSample = lerp((Int32) iParentHeightSample0, 
                                               (Int32) iParentHeightSample1, 
                                               0.5f);

            Int32 iFinalHeightSample = 
                iExpectedHeightSample + pResiduals[iResidualIndex++];

            vHeightData[y * iTileSize + x] = 
                ( UInt16 )clamp(iFinalHeightSample, 0, 65535);

            if(!xIsEven)
            {
                iParentX++;
            }
        }
        
        if(!yIsEven)
        {
            iParentY++;
        }
    }
}


//-----------------------------------------------------------------------------


ResidualCompressor::ResidualCompressor(void) :
    _pOutput             (NULL        ),
    _eMethod             (Uncompressed),

    _iHeightTileSize     (           0),
    _iQuantizationQuality(           1),       

    _vQuantizationBuffer (            ),
    _vCompressionBuffer  (            )
{
}

ResidualCompressor::~ResidualCompressor(void)
{
}


//-----------------------------------------------------------------------------


void ResidualCompressor::initialize(BbqFileWriter        *pOutput, 
                                    BbqCompressionMethod  eMethod, 
                                    int                   iQuantizationQuality, 
                                    int                   iHeightTileSize     )
{
    assert(pOutput              != NULL);
    assert(iQuantizationQuality >= 1   );
    assert(iQuantizationQuality <= 1000);
    assert(iHeightTileSize      > 1    );
    
    shutdown();
    
    _pOutput                 = pOutput;
    _eMethod                 = eMethod;
    _iHeightTileSize         = iHeightTileSize;
    _iQuantizationQuality    = iQuantizationQuality;
}


//-----------------------------------------------------------------------------


void ResidualCompressor::shutdown(void)
{
    _pOutput                 = NULL;
    _eMethod                 = Uncompressed;
    _iQuantizationQuality    = 1;
}


//-----------------------------------------------------------------------------


int ResidualCompressor::compressResiduals( 
    const std::vector<Int16> &vResiduals, 
          std::vector<Int16> &vDecompressedResiduals)
{
    assert(_pOutput);
    
    if(_eMethod != QuantizedHuffman)
    {
        // todo: implement other methods here..
        return -1;
    }
    
    // 1. step: quantization:
    _vQuantizationBuffer = vResiduals;
    
    // todo: make this not in-place but on-copy..
    quantize( &(_vQuantizationBuffer[0]), 
                 vResiduals.size(), 
                _iQuantizationQuality );
    
    // 2.step: compression of the residuals:
    // todo: is 16 bit enough?
    if(_eMethod == QuantizedHuffman)
    {
        compressHuffman(  _vCompressionBuffer, 
                        &(_vQuantizationBuffer[0]), 
                          _vQuantizationBuffer.size());
    }
    else
    {
        compressZlib(  _vCompressionBuffer, 
                     &(_vQuantizationBuffer[0]), 
                       _vQuantizationBuffer.size() );
    }
    
    UInt16 iCompressedSize = (UInt16) _vCompressionBuffer.size();
    
    if(iCompressedSize >= 
        sizeof(_vQuantizationBuffer[0]) * _vQuantizationBuffer.size())
    {
        // todo: the compressed residuals are bigger than the original
        // data.. save the data with another method: 
        std::cout << "the compressed residuals are bigger than the "
                  << "original data\n" << std::endl;
    }
    
    // 3.step: write the compressed residuals into the output file:
    if( !_pOutput->writeData(&iCompressedSize, sizeof(iCompressedSize)) ||
        !_pOutput->writeData(&_vCompressionBuffer[0], 
                             (Int32) _vCompressionBuffer.size())       )
    {
        return -1;
    }
    
    // decompress the compressed data again (with the same algorithm that the
    // client application will use) for the children 
    vDecompressedResiduals.resize(getResidualsCount(_iHeightTileSize));
    
    decompressHuffman(   vDecompressedResiduals, 
                      &(_vCompressionBuffer[0]), 
                        _vCompressionBuffer.size() );        

    dequantize(&(vDecompressedResiduals[0]), 
                 vDecompressedResiduals.size(), 
                _iQuantizationQuality         );
    
    return (Int32) _vCompressionBuffer.size();
}


//-----------------------------------------------------------------------------


ResidualDecompressor::ResidualDecompressor(void) :
    _pInput              (NULL        ),
    _eMethod             (Uncompressed),

    _iHeightTileSize     (           0),
    _iQuantizationQuality(           1),

    _vCompressionBuffer  (            )
{
}

ResidualDecompressor::~ResidualDecompressor(void)
{
}

//-----------------------------------------------------------------------------


void ResidualDecompressor::initialize(
    BbqFileReader        *pInput, 
    BbqCompressionMethod  eMethod, 
    Int32                 iQuantizationQuality, 
    Int32                 iHeightTileSize)
{
    assert(pInput               != NULL                             );
    assert(iQuantizationQuality >= 1 && iQuantizationQuality <= 1000);
    assert(iHeightTileSize      >  1                                );
    
    _pInput                  = pInput;
    _eMethod                 = eMethod;
    _iHeightTileSize         = iHeightTileSize;
    _iQuantizationQuality    = iQuantizationQuality;
}


//-----------------------------------------------------------------------------


void ResidualDecompressor::shutdown(void)
{
    _pInput                  = NULL;

    _eMethod                 = Uncompressed;
    _iQuantizationQuality    = 1;
}


//-----------------------------------------------------------------------------


bool ResidualDecompressor::decompressResiduals(std::vector<Int16> &vResiduals)
{
    assert(_pInput);
    
    if(_eMethod != QuantizedHuffman)
    {
        // todo: implement other methods here..
        return false;
    }
    
    // read the compressed residuals from the file:
    UInt16 iCompressedSize = 0;

    if(!_pInput->readData(&iCompressedSize, sizeof(iCompressedSize)))
    {
        return false;
    }

    _vCompressionBuffer.resize(iCompressedSize);
    
    if(!_pInput->readData( &(_vCompressionBuffer[0]), 
                           (Int32) _vCompressionBuffer.size()))
    {
        return false;
    }
    
    // decompress the compressed data again (with the same algorithm that the
    // client application will use) for the children 

    vResiduals.resize(getResidualsCount(_iHeightTileSize));
    
    decompressHuffman(   vResiduals, 
                      &(_vCompressionBuffer[0]), 
                        _vCompressionBuffer.size());

    dequantize(&(vResiduals[0]), 
                 vResiduals.size(), 
                _iQuantizationQuality );
    
    return true;
}


OSG_END_NAMESPACE
