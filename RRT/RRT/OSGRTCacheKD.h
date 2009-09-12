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

#ifndef _OSGRTCACHEKD_H_
#define _OSGRTCACHEKD_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTCacheKDBase.h"
#include "OSGLine.h"

#include "stack"
#include <boost/mpl/if.hpp>

OSG_BEGIN_NAMESPACE

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTCacheKD : public RTCacheKDBase<DescT>
{
  protected:

    struct KDStackElem;
    struct KDSIMDStackElem;

    /*==========================  PUBLIC  =================================*/

  public:

    typedef std::vector<KDStackElem>          KDFloatElemStack;

    typedef AlignedAllocator<KDSIMDStackElem> SIMDStackAllocator;

    typedef std::vector<KDSIMDStackElem, 
                        SIMDStackAllocator>   KDSIMDElemStack;


    typedef typename
        boost::mpl::if_<boost::mpl::bool_<(DescT::SIMDMath == true)>,
                        KDSIMDElemStack,
                        KDFloatElemStack >::type   KDElemStack;

    typedef          KDElemStack                   ElemStack;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTCacheKD(void);

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
                               KDElemStack     &sKDToDoStack,
                               UInt32           uiCacheId   );

    void intersect            (RTRaySIMDPacket &oRay, 
                               RTHitSIMDPacket &oHit,
                               KDElemStack     &sKDToDoStack,
                               UInt32           uiCacheId   ,
                               UInt32          *uiActive    );
    void intersectSingle      (RTRaySIMDPacket &oRay, 
                               RTHitSIMDPacket &oHit,
                               KDElemStack     &sKDToDoStack,
                               UInt32           uiCacheId   ,
                               UInt32          *uiActive    );

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
        Real32         tmin;
        Real32         tmax;

        union
        {
            const RTCacheKDNode *node;
                  unsigned int   addr;
        };
    };

    struct KDSIMDStackElem 
    {
        union
        {
            Float4         tmin4;
            Real32         tminA[4];
        };

        union
        {
            Float4         tmax4;
            Real32         tmaxA[4];
        };

        union
        {
            const RTCacheKDNode *node;
                  unsigned int   addr;
        };

        unsigned int   pad0;
        unsigned int   pad1;
        unsigned int   pad2;

        KDSIMDStackElem(void);
        KDSIMDStackElem(const KDSIMDStackElem &source);
        void operator =(const KDSIMDStackElem &source);
    };

    void flattenTree(RTKDNode *pLeft, 
                     RTKDNode *pRight);

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

    virtual ~RTCacheKD(void); 

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
    RTCacheKD(const RTCacheKD &source);
    void operator =(const RTCacheKD &source);
};

OSG_END_NAMESPACE

#include "OSGRTCacheKD.inl"

#endif /* _OSGRTCACHEKD_H_ */
