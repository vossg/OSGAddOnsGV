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

//-----------------------------------------------------------------------------

#ifndef _OSGBBQTERRAINCOMPRESSOR_H_
#define _OSGBBQTERRAINCOMPRESSOR_H_

//-----------------------------------------------------------------------------

#include "OSGTerrainHelper.h"
#include "OSGBbqFile.h"
#include "OSGBbqNodeIterator.h"
#include "OSGBbqResidualCompression.h"

#include <stack>

OSG_BEGIN_NAMESPACE

class BbqTerrainCompressor
{
    /*==========================  PUBLIC  =================================*/

  public:
    
    struct Statistics
    {
        Statistics(void);
        ~Statistics(void);

        Int32  originalHeightDataSize;
        Int32  compressedHeightDataSize;
        Int32  originalTextureDataSize;
        Int32  compressedTextureDataSize;
        Int32  maxAbsoluteError;
        Real32 bitsPerHeightSample;
    };
    
    
    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainCompressor(void);
    ~BbqTerrainCompressor(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    bool   start      (const std::string &szInputBbqFilename, 
                       const std::string &szOutputFilename, 
                             Int32        iCompressionQuality);

    bool   isFinished (void) const;
    void   step       (void);

    Real32 getProgress(void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */
    
    const Statistics &getStatistics(void) const;
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    struct BbqCompressorNodeData
    {
        std::vector<UInt16>  heightData;
        std::vector<Int16 >  residuals;
        std::vector<UInt16>  decompressedHeightData;
        
        std::vector<UInt8 >  textureData;
        std::vector<UInt8 >  compressedTextureData;
    };
    
    struct BbqCompressorNode
    {
        BbqNodeId                        id;
#if 0
        BbqFileNode                      inputInfo;
        BbqFileNode::TerrainDataPointer  compressedDataPointer;
#endif
        BbqCompressorNodeData           *data;
    };
    
    typedef FixedAllocator< BbqCompressorNodeData > 
        BbqCompressorNodeDataAllocator;

    BbqFileReader                  _oInputFile;
    BbqFile::BbqFileHeader         _oInputHeader;
    BbqFileWriter                  _oOutputFile;

    std::vector<BbqCompressorNode> _vNodeData;      

    BbqNodeIterator                _oNodeIterator;

    BbqCompressorNodeDataAllocator _oCompressorDataAllocator;
    
    std::vector<Int16>             _vResidualsBuffer;

    Statistics                     _oStatistics;
    
    Int32                          _iCompressionQuality;
    
    ResidualCompressor             _oResidualCompressor;
    
    /*---------------------------------------------------------------------*/

    bool  processNode        (BbqCompressorNode         &oNode, 
                              BbqCompressorNode         *pParentNode );
    void  compressTextureData(BbqCompressorNode         &oNode       );
    
    Int32 getMaxDifference   (const std::vector<UInt16> &vHeightData0, 
                              const std::vector<UInt16> &vHeightData1); 

    /*==========================  PRIVATE  ================================*/

  private:
   
};

OSG_END_NAMESPACE

#endif // _OSGBBQTERRAINCOMPRESSOR_H_

