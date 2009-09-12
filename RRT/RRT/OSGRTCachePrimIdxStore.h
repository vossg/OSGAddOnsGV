/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *           Copyright (C) 2003 by the OpenSG Forum                          *
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

#ifndef _OSGRTCACHEPRIMIDXSTORE_H_
#define _OSGRTCACHEPRIMIDXSTORE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGMField.h"

#include "vector"

OSG_BEGIN_NAMESPACE

typedef std::vector<UInt32> RTCachePrimIdxStore;

template <>
struct FieldTraits<RTCachePrimIdxStore> : 
    public FieldTraitsTemplateBase<RTCachePrimIdxStore>
{
  private:

    static  DataType                   _type;

  public:

    typedef FieldTraits<RTCachePrimIdxStore>  Self;

    enum                  { Convertible = (Self::FromStringConvertible |
                                           Self::ToStreamConvertible   )     };

    static OSG_BASE_DLLMAPPING
                 DataType     &getType     (void);

    static const Char8        *getSName    (void) 
    {
        return "SFRTCachePrimIdxStore"; 
    }

    static const Char8        *getMName    (void) 
    { 
        return "MFRTCachePrimIdxStore"; 
    }

    static const RTCachePrimIdxStore getDefault  (void) 
    {
        return RTCachePrimIdxStore();   
    }


    static bool getFromCString(      RTCachePrimIdxStore  &outVal,
                               const Char8               *&inVal)
    {
        if(inVal == NULL)
            return false;

        UInt32 uiNum  = 0;

        Char8 *c      = const_cast<Char8 *>(inVal);
        Char8 *tokenC = 0;
        
        Char8  token[256];
        bool   bFinished = false;
        bool   bFoundNum = false;
        
        while(bFinished == false)
        {
            switch (*c)
            {
                case '\0':

                    if(tokenC)
                    {
                        *tokenC = 0;

                        if(bFoundNum == false)
                        {
                            uiNum = TypeTraits<UInt32>::getFromCString(token);

                            bFoundNum = true;
                        }
                        else
                        {
                            outVal.push_back(
                                TypeTraits<UInt32>::getFromCString(token));
                        }
                    }

                    if(bFoundNum == true && outVal.size() < uiNum)
                    {
                        outVal.resize(uiNum, 
                                      TypeTraits<UInt32>::getZeroElement());
                    }

                    bFinished = true;

                    break;

                case ' ' :
                case '\t':
                case '\n':
                case ',':
                    if(tokenC)
                    {
                        *tokenC = 0;

                        if(bFoundNum == false)
                        {
                            uiNum = TypeTraits<UInt32>::getFromCString(token);

                            if(uiNum != 0)
                            {
                                outVal.reserve(uiNum);
                            }

                            bFoundNum = true;
                        }
                        else
                        {
                            outVal.push_back(
                                TypeTraits<UInt32>::getFromCString(token));
                        }

                        tokenC = 0;
                    }
                break;

                default:
                    if(!tokenC)
                    {
                        tokenC = token;
                    }
                    *tokenC++ = *c;
                    break;
            }
            ++c;
        }

        return true;
    }


    static void putToStream(const RTCachePrimIdxStore &val,
                                  OutStream           &str)
    {
        typedef TypeTraits<UInt32> TypeTrait;

        str << "\"";
        TypeTrait::putToStream(val.size(), str);
        str << " ";
        
        for(UInt32 i = 0; i < val.size(); ++i)
        {
            TypeTrait::putToStream(val[i], str);
            str << " ";
        }

        str << "\"";
    }

    static UInt32 getBinSize(const RTCachePrimIdxStore &oObject)
    {
        return sizeof(UInt32) + sizeof(UInt32) * oObject.size();
    }


    static UInt32 getBinSize(const RTCachePrimIdxStore *pObjectStore,
                                   UInt32               uiNumObjects)
    {
        UInt32 size = 0;

        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            size += getBinSize(pObjectStore[i]);
        }

        return size;
    }

    static void copyToBin(      BinaryDataHandler   &pMem, 
                          const RTCachePrimIdxStore &oObject)
    {
        UInt32 size = oObject.size();

        pMem.putValue(size);

        for(UInt32 i = 0; i < oObject.size(); ++i)
        {
            pMem.putValue(oObject[i]);
        }
    }

    static void copyToBin(      BinaryDataHandler   &pMem, 
                          const RTCachePrimIdxStore *pObjectStore,
                                UInt32               uiNumObjects)
    {
        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            copyToBin(pMem, pObjectStore[i]);
        }
    }

    static void copyFromBin(BinaryDataHandler   &pMem, 
                            RTCachePrimIdxStore &oObject)
    {
        UInt32 size;

        pMem.getValue(size);

        if(size != 0)
        {
            oObject.resize(size);

            for(UInt32 i = 0; i < size; ++i)
            {
                pMem.getValue(oObject[i]);
            }
        }
    }

    static void copyFromBin(BinaryDataHandler   &pMem, 
                            RTCachePrimIdxStore *pObjectStore,
                            UInt32                uiNumObjects)
    {
        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            copyFromBin(pMem, pObjectStore[i]);
        }
    }
};


typedef MField<RTCachePrimIdxStore> MFRTCachePrimIdxStore;

OSG_END_NAMESPACE

#endif /* _OSGMFIELD_H_ */
