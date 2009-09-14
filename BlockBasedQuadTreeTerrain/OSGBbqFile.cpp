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

#include "OSGBbqFile.h"

OSG_BEGIN_NAMESPACE


//---------------------------------------------------------------------------
//  BbqFileBbqFileHeader
//---------------------------------------------------------------------------

BbqFile::BbqFileHeader::BbqFileHeader()
{
    _iHeightSampleCountX          = 0;
    _iHeightSampleCountY          = 0;
    _iTextureSampleCountX         = 0;
    _iTextureSampleCountY         = 0;
    _iLevelCount                  = 0;
    _iNodeCount                   = 0;
    _iHeightTileSize              = 0;
    _iTextureTileSize             = 0;
    _fSampleSpacing               = 0;
    _fHeightScale                 = 0;
    _fHeightOffset                = 0;
    _eHeightType                  = Image::OSG_INVALID_IMAGEDATATYPE;
    _eTextureType                 = Image::OSG_INVALID_IMAGEDATATYPE;
    _eHeightFormat                = AbsoluteValues;
    _eTextureFormat               = RGB8;
    _uiHeightCompressionQuality   = 1000;
}

BbqFile::BbqFileHeader::~BbqFileHeader()
{
}

//---------------------------------------------------------------------------
//  BbqFile
//---------------------------------------------------------------------------

BbqFile::BbqFile(void)
{
}

BbqFile::~BbqFile(void)
{
}

//-----------------------------------------------------------------------------


BbqFileWriter::BbqFileWriter(void) :
     Inherited    ( ),
    _oOutputStream( ),
    _oNodeTablePos(0)
{
}


//-----------------------------------------------------------------------------


BbqFileWriter::~BbqFileWriter()
{
    close();
}


//-----------------------------------------------------------------------------


bool BbqFileWriter::open(const std::string &filename)
{
    _oOutputStream.open(filename.c_str() /*,LittleEndian todo */);
    
    if(_oOutputStream.isOpen() == false)
    {
        return false;
    }

    return true;
}

    
//-----------------------------------------------------------------------------


void BbqFileWriter::close()
{
    _oOutputStream.close();
}


//-----------------------------------------------------------------------------


bool BbqFileWriter::startWriting(const BbqFileHeader &oHeader)
{
    // todo: add a version/ a more useful file format (chunk based for example)

    _oOutputStream.writeUInt8('B');
    _oOutputStream.writeUInt8('B');
    _oOutputStream.writeUInt8('Q');
    _oOutputStream.writeUInt8('T');
    
    _oOutputStream.writeSInt32(oHeader._iHeightSampleCountX       );
    _oOutputStream.writeSInt32(oHeader._iHeightSampleCountY       );
    _oOutputStream.writeSInt32(oHeader._iTextureSampleCountX      );
    _oOutputStream.writeSInt32(oHeader._iTextureSampleCountY      );
    _oOutputStream.writeSInt32(oHeader._iLevelCount               );
    _oOutputStream.writeSInt32(oHeader._iNodeCount                );
    _oOutputStream.writeSInt32(oHeader._iHeightTileSize           );
    _oOutputStream.writeSInt32(oHeader._iTextureTileSize          );
    _oOutputStream.writeFloat (oHeader._fSampleSpacing            );
    _oOutputStream.writeFloat (oHeader._fHeightScale              );
    _oOutputStream.writeFloat (oHeader._fHeightOffset             );
    _oOutputStream.writeUInt32(oHeader._eHeightType               );
    _oOutputStream.writeUInt32(oHeader._eTextureType              );
    _oOutputStream.writeUInt32(oHeader._eHeightFormat             );
    _oOutputStream.writeUInt32(oHeader._eTextureFormat            );   
    _oOutputStream.writeUInt32(oHeader._uiHeightCompressionQuality);

    _oOutputStream.writeUInt32(oHeader._uiDatum);
    _oOutputStream.writeFloat (oHeader._vEllipsoidAxis[0]);
    _oOutputStream.writeFloat (oHeader._vEllipsoidAxis[1]);
    _oOutputStream.writeFloat (oHeader._vOrigin[0]);
    _oOutputStream.writeFloat (oHeader._vOrigin[1]);
    _oOutputStream.writeFloat (oHeader._vPixelSize[0]);
    _oOutputStream.writeFloat (oHeader._vPixelSize[1]);
    
    _oNodeTablePos = _oOutputStream.getPosition();
    
    return !_oOutputStream.isBad();
}


//-----------------------------------------------------------------------------


bool BbqFileWriter::writeData(const void *pData, Int32 size)
{
    return _oOutputStream.writeBlock(pData, size);
}


//-----------------------------------------------------------------------------


EndianFileOutStream::StreamPosType BbqFileWriter::getCurrentPosition(void)
{
    return _oOutputStream.getPosition();
}


//-----------------------------------------------------------------------------


void BbqFileWriter::gotoNodeInfoTable(void)
{
    _oOutputStream.setPosition(_oNodeTablePos);
}

    

//-----------------------------------------------------------------------------


BbqFileReader::BbqFileReader() :
     Inherited   (),
    _oHeader     (),
    _oInputStream()
{
}


//-----------------------------------------------------------------------------


BbqFileReader::~BbqFileReader()
{
    close();
}


//-----------------------------------------------------------------------------


bool BbqFileReader::open( const std::string& filename )
{
    _oInputStream.open(filename.c_str() /*, LittleEndian todo */);
    
    if(_oInputStream.isOpen() == false)
    {
        return false;
    }
    
    // read the header:
    UInt8 tag[ 4 ];
    
    _oInputStream.readBlock(tag, sizeof(tag));
                            
    if( ( tag[ 0 ] != 'B' ) ||
        ( tag[ 1 ] != 'B' ) ||
        ( tag[ 2 ] != 'Q' ) ||
        ( tag[ 3 ] != 'T' ) )
    {
        // not a bbq terrain file
        return false;
    }

    _oHeader._iHeightSampleCountX        = _oInputStream.readSInt32();
    _oHeader._iHeightSampleCountY        = _oInputStream.readSInt32();
    _oHeader._iTextureSampleCountX       = _oInputStream.readSInt32();
    _oHeader._iTextureSampleCountY       = _oInputStream.readSInt32();
    _oHeader._iLevelCount                = _oInputStream.readSInt32();
    _oHeader._iNodeCount                 = _oInputStream.readSInt32();
    _oHeader._iHeightTileSize            = _oInputStream.readSInt32();
    _oHeader._iTextureTileSize           = _oInputStream.readSInt32();
    _oHeader._fSampleSpacing             = _oInputStream.readFloat ();
    _oHeader._fHeightScale               = _oInputStream.readFloat ();
    _oHeader._fHeightOffset              = _oInputStream.readFloat ();

    _oHeader._eHeightType              = 
        Image::Type(_oInputStream.readUInt32());

    _oHeader._eTextureType             = 
        Image::Type(_oInputStream.readUInt32());


    _oHeader._eHeightFormat              = 
        BbqFile::HeightFormat(_oInputStream.readUInt32());

    _oHeader._eTextureFormat             = 
        BbqFile::TextureFormat(_oInputStream.readUInt32());

    _oHeader._uiHeightCompressionQuality = _oInputStream.readUInt32();

    _oHeader._uiDatum           = _oInputStream.readUInt32();
    _oHeader._vEllipsoidAxis[0] = _oInputStream.readFloat ();
    _oHeader._vEllipsoidAxis[1] = _oInputStream.readFloat ();
    _oHeader._vOrigin[0]        = _oInputStream.readFloat ();
    _oHeader._vOrigin[1]        = _oInputStream.readFloat ();
    _oHeader._vPixelSize[0]     = _oInputStream.readFloat ();
    _oHeader._vPixelSize[1]     = _oInputStream.readFloat ();
    
    return !_oInputStream.isBad();
}


//-----------------------------------------------------------------------------


void BbqFileReader::close()
{
    _oInputStream.close();
}


//-----------------------------------------------------------------------------


const BbqFile::BbqFileHeader &BbqFileReader::getHeader(void) const
{
    return _oHeader;
}


//-----------------------------------------------------------------------------


bool BbqFileReader::setPosition(EndianFileInStream::StreamPosType fileOffset)
{
    return _oInputStream.setPosition(fileOffset);
}


//-----------------------------------------------------------------------------


bool BbqFileReader::readData(void *targetPtr, Int32 size)
{
    return (_oInputStream.readBlock(targetPtr, size) == size);
}


//-----------------------------------------------------------------------------

OSG_END_NAMESPACE
