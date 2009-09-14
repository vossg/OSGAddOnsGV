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

#ifndef _OSGRTXCACHEKDNODE_H_
#define _OSGRTXCACHEKDNODE_H_
#ifdef __sgi
#pragma once
#endif

#include <assert.h>
#include <climits>

#include "OSGRTCachePrimIdxStore.h"
#include "OSGCellRT.h"

OSG_BEGIN_NAMESPACE


struct RTXKDNode
{
  protected:
    
    bool         _bIsLeaf;
    
    union
    {
        UInt32   _uiSplitAxis;  //interior
        UInt32   _uiPrimIdx;   // leaf
    };

    union
    {
        UInt32   _uiNumPrim;  // leaf
        Real32   _fSplitPos;  // interior
    };
      
    RTXKDNode   *_pAboveChild;
    RTXKDNode   *_pBelowChild;
    
    UInt32       _nodeId;
    UInt32       _rope[6];
    boxVolume_t *_pBBox;
      
    typedef std::vector<Int32>::iterator  IndexIterator;
    typedef std::vector<Int32>::size_type IndexSize;

  public:
    
    RTXKDNode(void);
  
    ~RTXKDNode(void);

    void initRope();

    void setRope(UInt32 nodeId, UInt8 index);

    void printBoxValues(void); //used for debugging, remove later

    void setBoundingBox(boxVolume_t* box);
  
    boxVolume_t getBoxVolume(void);

    UInt32 getNodeId(void);

    UInt32 getRope(UInt8 index);

    Real32 getBoxMinValue(UInt8 index);

    Real32 getBoxMaxValue(UInt8 index);

    void setBoxMinValue(UInt8 index, Real32 val);
    void setBoxMaxValue(UInt8 index, Real32 val);
    
    void initLeaf    (IndexIterator          primNums, 
                      IndexSize              np,
                      MFRTCachePrimIdxStore &vStore,
                      UInt32                 id);

     
    void initInterior(UInt32                 uiAxis, 
                      Real32                 fSplitPos,
                      UInt32                 id);
    
    
    bool                 isLeaf         (void             );

    void                 setAboveChild   (RTXKDNode *pNode);
    RTXKDNode           *getAboveChild   (void            );
    
    void                 setBelowChild   (RTXKDNode *pNode);
    RTXKDNode           *getBelowChild   (void            );
    
    UInt32               getSplitAxis    (void            );
    Real32               getSplitPos     (void            );
    
    UInt32               getPrimitiveIdx (void            );
    UInt32               getNumPrimitives(void            ) const;              
};



struct RTXCacheKDNode 
{
    cacheKDNode_t node;
 
    typedef          std::vector<Int32>::iterator  IndexIterator;
    typedef          std::vector<Int32>::size_type IndexSize;

    void initLeaf    (IndexIterator          primNums, 
                      IndexSize              np,
                      MFRTCachePrimIdxStore &vStore   ); 
    
    void initInterior(UInt32                 uiAxis, 
                      Real32                 fSplitPos);
    
    void initLeaf    (RTXKDNode              *pNode,  
                      MFRTCachePrimIdxStore &vStore ); 
    
    void initInterior(RTXKDNode              *pNode    );
    
    
    Real32 getSplitPos     (void) const;
    UInt32 getNumPrimitives(void) const;
    UInt32 getSplitAxis    (void) const;
    bool   isLeaf          (void) const;

    void putToStream   (      OutStream &str  ) const;
    bool getFromCString(const Char8     *inVal);

    UInt32 getBinSize (void                   ) const;
    void   copyToBin  (BinaryDataHandler &pMem) const;
    void   copyFromBin(BinaryDataHandler &pMem);
    
    bool operator ==(const RTXCacheKDNode &rhs) const;
};


template <>
struct FieldTraits<RTXCacheKDNode> : 
    public FieldTraitsTemplateBase<RTXCacheKDNode>
{
  private:

    static  DataType                   _type;

  public:

    typedef FieldTraits<RTXCacheKDNode>  Self;

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
        return "SFRTXCacheKDNode"; 
    }

    static const Char8        *getMName    (void) 
    { 
        return "MFRTXCacheKDNode"; 
    }

    static const RTXCacheKDNode getDefault  (void) 
    {
        return RTXCacheKDNode();   
    }


    static bool getFromCString(      RTXCacheKDNode  &outVal,
                               const Char8          *&inVal)
    {
        return outVal.getFromCString(inVal);
    }


    static void putToStream(const RTXCacheKDNode &val,
                                  OutStream      &str)
    {
        str << "\"";
        val.putToStream(str);
        str << "\"";
    }

    static UInt32 getBinSize(const RTXCacheKDNode &oObject)
    {
        return oObject.getBinSize();
    }


    static UInt32 getBinSize(const RTXCacheKDNode *pObjectStore,
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
                          const RTXCacheKDNode    &oObject)
    {
        oObject.copyToBin(pMem);
    }

    static void copyToBin(      BinaryDataHandler &pMem, 
                          const RTXCacheKDNode    *pObjectStore,
                                UInt32             uiNumObjects)
    {
        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            copyToBin(pMem, pObjectStore[i]);
        }
    }

    static void copyFromBin(BinaryDataHandler &pMem, 
                            RTXCacheKDNode    &oObject)
    {
        oObject.copyFromBin(pMem);
    }

    static void copyFromBin(BinaryDataHandler &pMem, 
                            RTXCacheKDNode    *pObjectStore,
                            UInt32             uiNumObjects)
    {
        for(UInt32 i = 0; i < uiNumObjects; ++i)
        {
            copyFromBin(pMem, pObjectStore[i]);
        }
    }
};


typedef MField<RTXCacheKDNode> MFRTXCacheKDNode;

OSG_END_NAMESPACE

#include "OSGRTXCacheKDNode.inl"

#endif /* _OSGRTXCACHEKDNODE_H_ */
