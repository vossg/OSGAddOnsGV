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

//---------------------------------------------------------------------------
//  FileStreamMixin
//---------------------------------------------------------------------------

template <class ParentT> inline
FileStreamMixinX<ParentT>::FileStreamMixinX(void) : 
    Inherited  (_oFileStream),
   _oFileStream(            )
{
    
}

template <class ParentT> inline
FileStreamMixinX<ParentT>::FileStreamMixinX(
    const Char8                   *szFileName,
          std::ios_base::openmode  oMode    ) : 

     Inherited  (_oFileStream),
    _oFileStream( szFileName, (oMode | Inherited::eOpenFlags))
{
}

template <class ParentT> inline
FileStreamMixinX<ParentT>::~FileStreamMixinX(void) 
{
    if(_oFileStream.is_open() == true)
    {
        _oFileStream.close();
    }
}

template <class ParentT> inline
void FileStreamMixinX<ParentT>::open(
    const char                     *szFileName, 
          std::ios_base::openmode   oMode)
{
    if(_oFileStream.is_open() == true)
    {
        _oFileStream.close();
    }

    _oFileStream.open(szFileName, (oMode | Inherited::eOpenFlags));
}

template <class ParentT> inline
void FileStreamMixinX<ParentT>::close(void) 
{
    _oFileStream.close(); 
}

template <class ParentT> inline
bool FileStreamMixinX<ParentT>::setPosition(StreamPosType oPos)
{
    _oFileStream.seekg(oPos, std::ios_base::beg);

    return _oFileStream.good();
}

template <class ParentT> inline
typename FileStreamMixinX<ParentT>::StreamPosType 
    FileStreamMixinX<ParentT>::getPosition(void) 
{
    return _oFileStream.tellg();
}

template <class ParentT> inline
bool FileStreamMixinX<ParentT>::isOpen(void)
{
    return _oFileStream.is_open();
}

template <class ParentT> inline
bool FileStreamMixinX<ParentT>::isBad (void)
{
    return !_oFileStream.good();
}

template <class ParentT> inline
FileStreamMixinX<ParentT>::operator bool (void) 
{
    return _oFileStream; 
}

//---------------------------------------------------------------------------
//  InFileStreamMixin
//---------------------------------------------------------------------------

template <class ParentT> inline
InFileStreamMixin<ParentT>::InFileStreamMixin(void) :
    Inherited()
{
}

template <class ParentT> inline
InFileStreamMixin<ParentT>::InFileStreamMixin(const Char8 *szFileName) :
    Inherited(szFileName, std::ios_base::in)
{
}

template <class ParentT> inline
InFileStreamMixin<ParentT>::~InFileStreamMixin(void)
{
}

template <class ParentT> inline
void InFileStreamMixin<ParentT>::open(const Char8 *szFileName)
{
    Inherited::open(szFileName, std::ios_base::in);
}


//---------------------------------------------------------------------------
//  OutFileStreamMixin
//---------------------------------------------------------------------------

template <class ParentT> inline
OutFileStreamMixin<ParentT>::OutFileStreamMixin(void) :
    Inherited()
{
}


template <class ParentT> inline
OutFileStreamMixin<ParentT>::OutFileStreamMixin(const Char8 *szFileName):
    Inherited(szFileName, std::ios_base::out)
{
}


template <class ParentT> inline
OutFileStreamMixin<ParentT>::~OutFileStreamMixin(void)
{
}


template <class ParentT> inline
void OutFileStreamMixin<ParentT>::open(const Char8 *szFileName)
{
    Inherited::open(szFileName, std::ios_base::out);
}
 
//---------------------------------------------------------------------------
//  EndianStreamMixin
//---------------------------------------------------------------------------

template <class ParentT, class BaseStreamT> inline
EndianStreamMixin<ParentT, 
                  BaseStreamT>::EndianStreamMixin(BaseStreamT &baseStream):
     Inherited  (baseStream  ),
    _eEndianType(LittleEndian)
{
}

template <class ParentT, class BaseStreamT> inline
EndianStreamMixin<ParentT, BaseStreamT>::~EndianStreamMixin(void)
{
}

//---------------------------------------------------------------------------
//  EndianInStreamMixin
//---------------------------------------------------------------------------

template <class ParentT> inline
EndianInStreamMixin<ParentT>::EndianInStreamMixin(std::istream &baseStream):
    Inherited(baseStream)
{
}

template <class ParentT> inline
EndianInStreamMixin<ParentT>::~EndianInStreamMixin(void)
{
}

template <class ParentT> inline
Int32 EndianInStreamMixin<ParentT>::readBlock (void  *pData, 
                                               Int32  iSize)
{
    (*Inherited::_pBaseStream).read(static_cast<char *>(pData), iSize);

    return (*Inherited::_pBaseStream).gcount();
}

template <class ParentT> inline
UInt8 EndianInStreamMixin<ParentT>::readUInt8(void)
{
    UInt8 returnValue;

    (*Inherited::_pBaseStream).read(reinterpret_cast<char *>(&returnValue), 
                                    sizeof(UInt8));

    return returnValue;
}

template <class ParentT> inline
UInt16 EndianInStreamMixin<ParentT>::readUInt16(void)
{
    UInt16 returnValue;

    (*Inherited::_pBaseStream).read(reinterpret_cast<char *>(&returnValue), 
                                    sizeof(UInt16));

    return returnValue;
}

template <class ParentT> inline
Int32 EndianInStreamMixin<ParentT>::readSInt32(void)
{
    Int32 returnValue;

    (*Inherited::_pBaseStream).read(reinterpret_cast<char *>(&returnValue), 
                                    sizeof(Int32));

    return returnValue;
}

template <class ParentT> inline
UInt32 EndianInStreamMixin<ParentT>::readUInt32(void)
{
    UInt32 returnValue;

    (*Inherited::_pBaseStream).read(reinterpret_cast<char *>(&returnValue), 
                                    sizeof(UInt32));

    return returnValue;
}

template <class ParentT> inline
Int64 EndianInStreamMixin<ParentT>::readSInt64(void)
{
    Int64 returnValue;

    (*Inherited::_pBaseStream).read(reinterpret_cast<char *>(&returnValue), 
                                    sizeof(Int64));

    return returnValue;
}

template <class ParentT> inline
Real32 EndianInStreamMixin<ParentT>::readFloat (void)
{
    Real32 returnValue;

    (*Inherited::_pBaseStream).read(reinterpret_cast<char *>(&returnValue), 
                                    sizeof(Real32));

    return returnValue;
}

//---------------------------------------------------------------------------
//  EndianInStreamMixin
//---------------------------------------------------------------------------

template <class ParentT> inline
EndianOutStreamMixin<ParentT>::EndianOutStreamMixin(std::ostream &baseStream):
    Inherited(baseStream)
{
}

template <class ParentT> inline
EndianOutStreamMixin<ParentT>::~EndianOutStreamMixin(void)
{
}

template <class ParentT> inline
bool EndianOutStreamMixin<ParentT>::writeBlock(const void   *pData, 
                                                     Int32   iSize)
{
    (*Inherited::_pBaseStream).write(static_cast<const char *>(pData), iSize);

    return (*Inherited::_pBaseStream).good();
}


template <class ParentT> inline
void EndianOutStreamMixin<ParentT>::writeUInt8(UInt8 val)
{
    (*Inherited::_pBaseStream).write(reinterpret_cast<const char *>(&val), 
                                     sizeof(UInt8));
}

template <class ParentT> inline
void EndianOutStreamMixin<ParentT>::writeUInt16(UInt16 val)
{
    (*Inherited::_pBaseStream).write(reinterpret_cast<const char *>(&val), 
                                     sizeof(UInt16));
}

template <class ParentT> inline
void EndianOutStreamMixin<ParentT>::writeSInt32(Int32 val)
{
    (*Inherited::_pBaseStream).write(reinterpret_cast<const char *>(&val), 
                                     sizeof(Int32));
}

template <class ParentT> inline
void EndianOutStreamMixin<ParentT>::writeUInt32(UInt32 val)
{
    (*Inherited::_pBaseStream).write(reinterpret_cast<const char *>(&val), 
                                     sizeof(UInt32));
}

template <class ParentT> inline
void EndianOutStreamMixin<ParentT>::writeSInt64(Int64 val)
{
    (*Inherited::_pBaseStream).write(reinterpret_cast<const char *>(&val), 
                                     sizeof(Int64));
}


template <class ParentT> inline
void EndianOutStreamMixin<ParentT>::writeFloat(Real32 val)
{
    (*Inherited::_pBaseStream).write(reinterpret_cast<const char *>(&val), 
                                     sizeof(Real32));
}

OSG_END_NAMESPACE
