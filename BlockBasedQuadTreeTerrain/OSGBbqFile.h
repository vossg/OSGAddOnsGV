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

#ifndef _OSGBBFILE_H_
#define _OSGBBFILE_H_

//-----------------------------------------------------------------------------

#include "OSGBaseTypes.h"
#include "OSGIOStream.h"
#include "OSGImage.h"

#include <ios>

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

template<class ParentT>
class FileStreamMixinX : public ParentT
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef ParentT Inherited;

    typedef std::streamoff StreamPosType;

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    FileStreamMixinX(void);
    FileStreamMixinX(const Char8                   *szFileName, 
                           std::ios_base::openmode  oMode = (
                               std::ios_base::in | 
                               std::ios_base::out));

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~FileStreamMixinX(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void open (const char                     *szFileName, 
                     std::ios_base::openmode   oMode = (std::ios_base::in | 
                                                        std::ios_base::out));
    void close(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool          setPosition(StreamPosType oPos);

    StreamPosType getPosition(void              );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

             bool isOpen(void);
             bool isBad (void);

    operator bool       (void);

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    std::fstream _oFileStream;

    /*==========================  PRIVATE  ================================*/

  private:
};




template<class ParentT>
class InFileStreamMixin : public FileStreamMixinX<ParentT>
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef FileStreamMixinX<ParentT> Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    InFileStreamMixin(void);
    InFileStreamMixin(const Char8 *szFileName);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~InFileStreamMixin(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void open (const Char8 *szFileName);
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:
};




template<class ParentT>
class OutFileStreamMixin : public FileStreamMixinX<ParentT>
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef FileStreamMixinX<ParentT> Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    OutFileStreamMixin(void);
    OutFileStreamMixin(const Char8 *szFileName);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~OutFileStreamMixin(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void open(const Char8 *szFileName);
    

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:
};


template<class ParentT, class BaseStreamT>
class EndianStreamMixin : public ParentT
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef ParentT Inherited;

    enum EndianType
    {
        LittleEndian,
        BigEndian
    };
    
    /*---------------------------------------------------------------------*/
    /*! \{                                                                 */

    EndianStreamMixin(BaseStreamT &baseStream);

    virtual ~EndianStreamMixin(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    static const std::ios_base::openmode eOpenFlags = std::ios_base::binary;

    /*==========================  PRIVATE  ================================*/

    EndianType _eEndianType;

  private:
};




template<class ParentT>
class EndianOutStreamMixin : public EndianStreamMixin<ParentT, std::ostream>
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef EndianStreamMixin<ParentT, std::ostream> Inherited;

    /*---------------------------------------------------------------------*/
    /*! \{                                                                 */

    EndianOutStreamMixin(std::ostream &baseStream);

    virtual ~EndianOutStreamMixin(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \{                                                                 */

    bool writeBlock (const void   *pData, 
                           Int32   iSize);

    void writeUInt8 (      UInt8   val  );
    void writeUInt16(      UInt16  val  );
    void writeSInt32(      Int32   val  );
    void writeUInt32(      UInt32  val  );
    void writeSInt64(      Int64   val  );
    void writeFloat (      Real32  val  );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \{                                                                 */

    void write(UInt16 val);
    void write(Int16  val);

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    static const std::ios_base::openmode eOpenFlags = std::ios_base::binary;

    /*==========================  PRIVATE  ================================*/

  private:
};




template<class ParentT>
class EndianInStreamMixin : public EndianStreamMixin<ParentT, std::istream>
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef EndianStreamMixin<ParentT, std::istream> Inherited;

    /*---------------------------------------------------------------------*/
    /*! \{                                                                 */

    EndianInStreamMixin(std::istream &baseStream);

    virtual ~EndianInStreamMixin(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \{                                                                 */

    Int32  readBlock (void  *pData, 
                      Int32  iSize);

    UInt8  readUInt8 (void        );
    UInt16 readUInt16(void        );
    Int32  readSInt32(void        );
    UInt32 readUInt32(void        );
    Int64  readSInt64(void        );
    Real32 readFloat (void        );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \{                                                                 */

    void read(UInt16 &val);
    void read(Int16  &val);


    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    static const std::ios_base::openmode eOpenFlags = std::ios_base::binary;

    /*==========================  PRIVATE  ================================*/

  private:
};




typedef OutFileStreamMixin<
           EndianOutStreamMixin<
               OutStream> > EndianFileOutStream;

typedef InFileStreamMixin<
            EndianInStreamMixin <
               InStream > > EndianFileInStream;


template<class HeightType>
struct BbqFileNode
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef Int64 TerrainDataPointer;

    //----------------------------------------------------------------------

    BbqFileNode(void);
    ~BbqFileNode(void);

    //----------------------------------------------------------------------
    
    // flags (bit 0 == texture data after the height data)
    UInt32             _uiFlags;
    
    // maximum error of this node (in object space) (todo: is a sint16
    //enough?)
    Int32              _iMaxHeightError;
    
    // for bounding box construction   (todo: do we need these at all?)
    HeightType         _iMinHeightSample;  // UInt16
    
    // for bounding box construction
    HeightType         _iMaxHeightSample;  // UInt16
    
    // pointer to the data. (points to the height/texture/normal/... other 
    //data in the file)
    TerrainDataPointer _iDataPointer;    
};
 

class BbqFile
{
    /*==========================  PUBLIC  =================================*/

  public:

    enum StatusFlags
    {
        TextureDataValid     = 0x0001,
        HeightDataCompressed = 0x0004,
    };

    enum HeightFormat
    {
        AbsoluteValues       = 0x0000,
        CompressedResiduals  = 0x0001,
        FormatCount          = 0x0002
    };

    enum TextureFormat
    {
        None  = 0x0000,
        Dxt1  = 0x0001,
        RGB8  = 0x0002,
        Count = 0x0003
    };

    struct BbqFileHeader
    {
        BbqFileHeader(void);
        ~BbqFileHeader(void);
        
        // domain size:
        Int32         _iHeightSampleCountX;
        Int32         _iHeightSampleCountY;
        Int32         _iTextureSampleCountX;
        Int32         _iTextureSampleCountY;
    
        // tree size:
        Int32         _iLevelCount;
        Int32         _iNodeCount;
        Int32         _iHeightTileSize;
        Int32         _iTextureTileSize;
        
        // data set informations:
        Real32        _fSampleSpacing;
        Real32        _fHeightScale;
        Real32        _fHeightOffset;
        
        Image::Type   _eHeightType;
        Image::Type   _eTextureType;

        HeightFormat  _eHeightFormat;
        TextureFormat _eTextureFormat;      
        UInt32        _uiHeightCompressionQuality;
        
        //geographic referencing:
        UInt32        _uiDatum;
        Vec2f         _vEllipsoidAxis;
        Vec2f         _vOrigin;
        Vec2f         _vPixelSize;
    };

    /*---------------------------------------------------------------------*/
    /*! \{                                                                 */

    BbqFile(void);
    ~BbqFile(void);

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/
    
  private:
};




class BbqFileWriter : public BbqFile
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqFile Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqFileWriter(void);
    ~BbqFileWriter(void);
    
    bool open (const std::string &filename);
    void close(      void                 );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    bool startWriting (const BbqFileHeader&header);

    bool writeData    (const void         *data, 
                             int           size  );

    template<class FileNodeT>
    bool writeNodeInfo(const FileNodeT    &node  );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */
    
    EndianFileOutStream::StreamPosType getCurrentPosition(void);
    void                               gotoNodeInfoTable (void);

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    EndianFileOutStream                _oOutputStream;
    EndianFileOutStream::StreamPosType _oNodeTablePos;
    
    /*==========================  PRIVATE  ================================*/

  private:
};




class BbqFileReader : public BbqFile
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqFile Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqFileReader(void);
    ~BbqFileReader(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    bool open (const std::string &filename);
    void close(void                       );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    bool setPosition(EndianFileInStream::StreamPosType fileOffset);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    const BbqFileHeader &getHeader(void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */
    
    template<class FileNodeT>
    bool readNodeInfo(FileNodeT   &node     );
    bool readData    (void        *targetPtr, 
                      Int32        size     );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    BbqFileHeader      _oHeader;
    EndianFileInStream _oInputStream;

    /*==========================  PRIVATE  ================================*/

  private:
};



OSG_END_NAMESPACE

#include "OSGBbqFile.inl"

#endif // _OSGBBFILE_H_


