/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2003 by the OpenSG Forum                   *
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

#ifndef _OSGRTCACHEKDVER1_H_
#define _OSGRTCACHEKDVER1_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTCacheKDBase.h"
#include "OSGLine.h"

#include "stack"

OSG_BEGIN_NAMESPACE

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTCacheKDVer1 : public RTCacheKDBase<DescT>
{
  protected:

    struct KDStackElem;

    /*==========================  PUBLIC  =================================*/

  public:
    
    typedef std::vector<KDStackElem> KDElemStack;
    typedef KDElemStack              ElemStack;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTCacheKDVer1(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */
   
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void buildStructure(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void intersect            (RTRayPacket     &oRay, 
                               RTHitPacket     &oHit,
                               KDElemStack     &sKDToDoStack);

    void intersect            (RTRaySIMDPacket &oRay, 
                               RTHitSIMDPacket &oHit,
                               KDElemStack     &sKDToDoStack);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef RTCacheKDBase<DescT> Inherited;

    typedef typename Inherited::IndexStore      IndexStore;
    typedef typename Inherited::IndexIterator   IndexIterator;
    typedef typename Inherited::IndexSize       IndexSize;
    typedef typename Inherited::RTKDNode        RTKDNode;
    typedef typename Inherited::BBoxStore       BBoxStore;
    typedef typename Inherited::BBoxEdge        BBoxEdge;
    typedef typename Inherited::BBoxEdgeStore   BBoxEdgeStore;
    typedef typename Inherited::BBoxEdgeStoreIt BBoxEdgeStoreIt;

    typedef typename Inherited::PrimIdxStore    PrimIdxStore;

    struct RTCacheKDNode 
    {
        union 
        {
            UInt32 _uiFlags;     // Leave + Interior
            Real32 _fSplitPos;   // Interior
            UInt32 _uiNumPrims;  // Leaf
        };

        union 
        {
            UInt32  _uiAboveChild;      // Interior
            UInt32  _uiSinglePrimitive; // Leaf
            UInt32  _pPrimitiveIdx;     // Leaf
        };

        void initLeaf    (IndexIterator  primNums, 
                          IndexSize      np,
                          PrimIdxStore  &vStore   ); 

        void initInterior(UInt32         uiAxis, 
                          Real32         fSplitPos);

        void initLeaf    (RTKDNode      *pNode,  
                          PrimIdxStore  &vStore   ); 
        
        void initInterior(RTKDNode      *pNode    );


        Real32 getSplitPos     (void) const;
        UInt32 getNumPrimitives(void) const;
        UInt32 getSplitAxis    (void) const;
        bool   isLeaf          (void) const;
    };

    struct KDStackElem 
    {
        const RTCacheKDNode *node;
              Real32         tmin;
              Real32         tmax;
    };

    void buildTree(      Int32          iCurrentKDNode,
                   const BoxVolume     &nodeBounds,
                   const BBoxStore     &vBoundsStore, 
                         IndexIterator  currentPrimsIt,
                         IndexSize      numCurrentPrims, 
                         Int32          iCurrentDepth, 
                         BBoxEdgeStore  vEdgeStore[3],
                         IndexIterator  itPrimStore0, 
                         IndexIterator  itPrimStore1, 
                         Int32          iNumBadRefines = 0);

    void flattenTree(RTKDNode *pRoot, Int32 nodeNum);

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    std::vector<RTCacheKDNode> _vKDTree;

    UInt32 nAllocedNodes;
    UInt32 nextFreeNode;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTCacheKDVer1(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    /*! \}                                                                 */
   /*==========================  PRIVATE  ================================*/

  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    RTCacheKDVer1(const RTCacheKDVer1 &source);
    void operator =(const RTCacheKDVer1 &source);
};

OSG_END_NAMESPACE

#include "OSGRTCacheKDVer1.inl"

#endif /* _OSGRTCACHEKDVER1_H_ */
