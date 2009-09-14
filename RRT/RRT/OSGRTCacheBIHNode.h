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

#ifndef _OSGRTCACHEBIHNODE_H_
#define _OSGRTCACHEBIHNODE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGConfig.h"
#include "OSGDataType.h"
#include "OSGContribRRTDef.h"

OSG_BEGIN_NAMESPACE

struct RTBIHNode
{
  protected:
    
    bool                 _bIsLeave;
    
    UInt32               _uiSplitAxis;
    Real32               _fSplitPos[2];
    
    RTBIHNode           *_pAboveChild;
    RTBIHNode           *_pBelowChild;
    
    UInt32               _uiPrimId;
    
  public:
    
    RTBIHNode(void);
    ~RTBIHNode(void);
    
    void initLeaf    (UInt32 uiPrimId); 
    
    void initInterior(UInt32                 uiAxis, 
                      Real32                 fSplitPos1,
                      Real32                 fSplitPos2);
    
    
    bool                 isLeave         (void            );
    
    void                 setAboveChild   (RTBIHNode *pNode);
    RTBIHNode           *getAboveChild   (void            );
    
    void                 setBelowChild   (RTBIHNode *pNode);
    RTBIHNode           *getBelowChild   (void            );
    
    UInt32               getSplitAxis    (void            );
    Real32               getSplitPos     (UInt32 uiIdx    );
    
    UInt32               getPrimitiveId  (void            );
};

struct RTCacheBIHNode
{
    RTCacheBIHNode(void);
    ~RTCacheBIHNode(void);

    void   initLeaf      (      UInt32             uiPrimId  ); 
    
    void   initInterior  (      UInt32             uiAxis, 
                                Real32             fSplitPos1,
                                Real32             fSplitPos2);
    
    void   initLeaf      (      RTBIHNode         *pNode,  
                                UInt32             uiPrinId  ); 
    
    void   initInterior  (      RTBIHNode         *pNode     );

    bool   isLeave       (      void                         );

    UInt32 getSplitAxis  (      void                         );
    Real32 getSplitPos   (      UInt32 uiIdx                 );
    
    UInt32 getPrimitiveId(      void                         );

    void   putToStream   (      OutStream         &str       ) const;
    bool   getFromCString(const Char8             *inVal     );

    UInt32 getBinSize    (      void                         ) const;
    void   copyToBin     (      BinaryDataHandler &pMem      ) const;
    void   copyFromBin   (      BinaryDataHandler &pMem      );

    bool operator ==(const RTCacheBIHNode &rhs) const;
};

template <>
struct FieldTraits<RTCacheBIHNode> : 
    public FieldTraitsTemplateBase<RTCacheBIHNode>
{
  private:

    static  DataType                   _type;

  public:

    typedef FieldTraits<RTCacheBIHNode>  Self;

    enum                  { Convertible = (Self::FromStringConvertible |
                                           Self::ToStreamConvertible   )     };

    static OSG_CONTRIBRRT_DLLMAPPING
                 DataType     &getType     (void);

    static const Char8        *getPName    (void)
    {
        return NULL;
    }

    static const Char8        *getSName    (void) 
    {
        return "SFRTCacheBIHNode"; 
    }

    static const Char8        *getMName    (void) 
    { 
        return "MFRTCacheBIHNode"; 
    }

    static const RTCacheBIHNode getDefault  (void) 
    {
        return RTCacheBIHNode();   
    }


    static bool getFromCString(      RTCacheBIHNode  &outVal,
                               const Char8          *&inVal)
    {
        return outVal.getFromCString(inVal);
    }


    static void putToStream(const RTCacheBIHNode &val,
                                  OutStream      &str)
    {
        str << "\"";
        val.putToStream(str);
        str << "\"";
    }

    static UInt32 getBinSize(const RTCacheBIHNode &oObject)
    {
        return oObject.getBinSize();
    }


    static UInt32 getBinSize(const RTCacheBIHNode *pObjectStore,
                                   UInt32          uiNumObjects)
    {
        UInt32 size = 0;

        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            size += getBinSize(pObjectStore[i]);
        }

        return size;
    }

    static void copyToBin(      BinaryDataHandler &pMem, 
                          const RTCacheBIHNode    &oObject)
    {
        oObject.copyToBin(pMem);
    }

    static void copyToBin(      BinaryDataHandler &pMem, 
                          const RTCacheBIHNode    *pObjectStore,
                                UInt32             uiNumObjects)
    {
        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            copyToBin(pMem, pObjectStore[i]);
        }
    }

    static void copyFromBin(BinaryDataHandler &pMem, 
                            RTCacheBIHNode    &oObject)
    {
        oObject.copyFromBin(pMem);
    }

    static void copyFromBin(BinaryDataHandler &pMem, 
                            RTCacheBIHNode    *pObjectStore,
                            UInt32             uiNumObjects)
    {
        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            copyFromBin(pMem, pObjectStore[i]);
        }
    }
};


typedef MField<RTCacheBIHNode> MFRTCacheBIHNode;

OSG_END_NAMESPACE

#include "OSGRTCacheBIHNode.inl"

#endif /* _OSGRTCACHEBIHNODE_H_ */
