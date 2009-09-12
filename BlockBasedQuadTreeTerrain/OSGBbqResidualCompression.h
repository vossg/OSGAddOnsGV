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

#ifndef _OSGBBQRESIDUALCOMPRESSION_H_
#define _OSGBBQRESIDUALCOMPRESSION_H_

//-----------------------------------------------------------------------------

#include "OSGVector.h"

#include <vector>

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

class BbqFileWriter;
class BbqFileReader;


//-----------------------------------------------------------------------------

// these could get private/static functions of the ResidualCompressor class
// below: 


//-----------------------------------------------------------------------------

class ResidualCompression
{
    /*==========================  PUBLIC  =================================*/

  public:

    enum BbqCompressionMethod
    {
        Uncompressed           = 0x0000,
        QuantizedHuffman       = 0x0001,
        CompressionMethodCount = 0x0002
    };


    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    ResidualCompression(void);
    ~ResidualCompression(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    Int32 getResidualsCount(Int32                     iTileSize     );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void  quantize         (Int16                    *pData, 
                            size_t                    uiDataSize, 
                            Int32                     iQualityFactor);

    void  dequantize       (Int16                    *pDataPtr, 
                            size_t                    uiDataSize, 
                            Int32                     iQualityFactor);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void  compressHuffman  (std::vector<UInt8>       &vTarget, 
                            Int16              const *pSource, 
                            size_t                    uiSourceSize   );
    
    void  decompressHuffman(std::vector<Int16>       &vTarget, 
                            UInt8              const *pSource, 
                            size_t                    uiSourceSize   );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void  compressZlib     (std::vector<UInt8>       &vTarget, 
                            Int16              const *pSource, 
                            size_t                    uiSourceSize   );

    void  decompressZlib   (std::vector<Int16>       &vTarget, 
                            UInt8              const *pSource, 
                            size_t                    uiSourceSize   );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    static void computeResiduals     (std::vector<Int16>       &vResiduals, 
                                      UInt16             const *pHeightData, 
                                      UInt16             const *pParentData, 
                                      Int32                     iTileSize, 
                                      Int32                     iChildIndex, 
                                      Int32                    &iMaxResidual);
    
    static void createChildHeightData(std::vector<UInt16>       &vHeightData, 
                                      Int16               const *pResiduals, 
                                      UInt16              const *pParentData, 
                                      Int32                      iTileSize, 
                                      Int32                      iChildIndex);
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/
    
  protected:

    /*==========================  PRIVATE  ================================*/

  private:
};


class ResidualCompressor : public ResidualCompression
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    ResidualCompressor(void);
    ~ResidualCompressor(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void initialize(BbqFileWriter        *pOutput, 
                    BbqCompressionMethod  eMethod, 
                    Int32                 iQuantizationQuality, 
                    Int32                 iHeightTileSize      );
 
    void shutdown  (void                                       );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    Int32 compressResiduals(const std::vector<Int16> &vResiduals, 
                                  std::vector<Int16> &vDecompressedResiduals);
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    typedef std::vector<Int16> QuantizationBuffer;
    typedef std::vector<UInt8> CompressionBuffer;

    BbqFileWriter         *_pOutput;
    BbqCompressionMethod   _eMethod;

    Int32                  _iHeightTileSize;
    Int32                  _iQuantizationQuality;       

    QuantizationBuffer     _vQuantizationBuffer;
    CompressionBuffer      _vCompressionBuffer;

    /*==========================  PRIVATE  ================================*/

  private:
};


//-----------------------------------------------------------------------------



class ResidualDecompressor : public ResidualCompression
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    ResidualDecompressor(void);
    ~ResidualDecompressor(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void initialize(BbqFileReader        *pInput, 
                    BbqCompressionMethod  eMethod, 
                    Int32                 iQuantizationQuality, 
                    Int32                 iHeightTileSize     );

    void shutdown  (void                                      );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    bool decompressResiduals(std::vector<Int16> &vResiduals);
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    typedef std::vector<UInt8>   CompressionBuffer;
    
    BbqFileReader        *_pInput;
    BbqCompressionMethod  _eMethod;

    Int32                 _iHeightTileSize;
    Int32                 _iQuantizationQuality;

    CompressionBuffer     _vCompressionBuffer;

    /*==========================  PRIVATE  ================================*/

  private:
};
    

OSG_END_NAMESPACE

#endif // _OSGBBQRESIDUALCOMPRESSION_H_

