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

#ifndef _OSGRTCACHEKDNODE_H_
#define _OSGRTCACHEKDNODE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTCachePrimIdxStore.h"

OSG_BEGIN_NAMESPACE

struct RTKDNode
{
  protected:
    
    bool                 _bIsLeaf;
    
    UInt32               _uiSplitAxis;
    Real32               _fSplitPos;
    
    RTKDNode            *_pAboveChild;
    RTKDNode            *_pBelowChild;
    
    UInt32               _uiPrimIdx;
    
    typedef          std::vector<Int32>::iterator  IndexIterator;
    typedef          std::vector<Int32>::size_type IndexSize;

  public:
    
    RTKDNode(void);
    ~RTKDNode(void);
    
    void initLeaf    (IndexIterator          primNums, 
                      IndexSize              np,
                      MFRTCachePrimIdxStore &vStore); 
    
    void initInterior(UInt32                 uiAxis, 
                      Real32                 fSplitPos);
    
    
    bool                 isLeaf          (void           );
    
    void                 setAboveChild   (RTKDNode *pNode);
    RTKDNode            *getAboveChild   (void           );
    
    void                 setBelowChild   (RTKDNode *pNode);
    RTKDNode            *getBelowChild   (void           );
    
    UInt32               getSplitAxis    (void           );
    Real32               getSplitPos     (void           );
    
    UInt32               getPrimitiveIdx (void           );
};

struct RTCacheKDNode 
{
    union 
    {
        UInt32 _uiFlags;     // Leaf + Interior
        Real32 _fSplitPos;   // Interior
        UInt32 _uiNumPrims;  // Leaf
    };
    
    union 
    {
        UInt32  _uiAboveChild;      // Interior
        UInt32  _uiSinglePrimitive; // Leaf
        UInt32  _pPrimitiveIdx;     // Leaf
    };
    
    typedef          std::vector<Int32>::iterator  IndexIterator;
    typedef          std::vector<Int32>::size_type IndexSize;

    void initLeaf    (IndexIterator          primNums, 
                      IndexSize              np,
                      MFRTCachePrimIdxStore &vStore   ); 
    
    void initInterior(UInt32                 uiAxis, 
                      Real32                 fSplitPos);
    
    void initLeaf    (RTKDNode              *pNode,  
                      MFRTCachePrimIdxStore &vStore   ); 
    
    void initInterior(RTKDNode              *pNode    );
    
    
    Real32 getSplitPos     (void) const;
    UInt32 getNumPrimitives(void) const;
    UInt32 getSplitAxis    (void) const;
    bool   isLeaf          (void) const;

    void putToStream   (      OutStream &str  ) const;
    bool getFromCString(const Char8     *inVal);

    UInt32 getBinSize (void                   ) const;
    void   copyToBin  (BinaryDataHandler &pMem) const;
    void   copyFromBin(BinaryDataHandler &pMem);
    

    bool operator ==(const RTCacheKDNode &rhs) const;
};


template <>
struct FieldTraits<RTCacheKDNode> : 
    public FieldTraitsTemplateBase<RTCacheKDNode>
{
  private:

    static  DataType                   _type;

  public:

    typedef FieldTraits<RTCacheKDNode>  Self;

    enum                  { Convertible = (Self::FromStringConvertible |
                                           Self::ToStreamConvertible   )     };

    static OSG_BASE_DLLMAPPING
                 DataType     &getType     (void);

    static const Char8        *getPName    (void)
    {
        return NULL;
    }

    static const Char8        *getSName    (void) 
    {
        return "SFRTCacheKDNode"; 
    }

    static const Char8        *getMName    (void) 
    { 
        return "MFRTCacheKDNode"; 
    }

    static const RTCacheKDNode getDefault  (void) 
    {
        return RTCacheKDNode();   
    }


    static bool getFromCString(      RTCacheKDNode  &outVal,
                               const Char8         *&inVal)
    {
        return outVal.getFromCString(inVal);
    }


    static void putToStream(const RTCacheKDNode &val,
                                  OutStream     &str)
    {
        str << "\"";
        val.putToStream(str);
        str << "\"";
    }

    static UInt32 getBinSize(const RTCacheKDNode &oObject)
    {
        return oObject.getBinSize();
    }


    static UInt32 getBinSize(const RTCacheKDNode *pObjectStore,
                                   UInt32         uiNumObjects)
    {
        UInt32 size = 0;

        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            size += getBinSize(pObjectStore[i]);
        }

        return size;
    }

    static void copyToBin(      BinaryDataHandler &pMem, 
                          const RTCacheKDNode     &oObject)
    {
        oObject.copyToBin(pMem);
    }

    static void copyToBin(      BinaryDataHandler &pMem, 
                          const RTCacheKDNode     *pObjectStore,
                                UInt32             uiNumObjects)
    {
        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            copyToBin(pMem, pObjectStore[i]);
        }
    }

    static void copyFromBin(BinaryDataHandler &pMem, 
                            RTCacheKDNode     &oObject)
    {
        oObject.copyFromBin(pMem);
    }

    static void copyFromBin(BinaryDataHandler &pMem, 
                            RTCacheKDNode     *pObjectStore,
                            UInt32             uiNumObjects)
    {
        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            copyFromBin(pMem, pObjectStore[i]);
        }
    }
};


typedef MField<RTCacheKDNode> MFRTCacheKDNode;

OSG_END_NAMESPACE

#include "OSGRTCacheKDNode.inl"

#endif /* _OSGRTCACHEKDNODE_H_ */
