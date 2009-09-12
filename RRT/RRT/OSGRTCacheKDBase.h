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

#ifndef _OSGRTCACHEKDBASE_H_
#define _OSGRTCACHEKDBASE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTCacheBase.h"
#include "OSGRTCachePrimIdxStore.h"

#include "OSGRTCacheKDNode.h"

#include <boost/bind.hpp>

OSG_BEGIN_NAMESPACE

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTCacheKDBase : public RTCacheBase<DescT>
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef          DescT                                Desc;
    typedef typename Desc::TriangleAccel                  TriangleAccel;
    typedef typename Desc::MFTriangleAccel                MFTriangleAccel;

    typedef          RTCacheKDBase<DescT>                 Self;

    OSG_GEN_INTERNALPTR(Self);

    typedef          RTCacheBase<DescT>                   Inherited;
    typedef typename Inherited::TypeObject                TypeObject;
    typedef typename TypeObject::InitPhase                InitPhase;

    typedef          RTCacheGeometryStore                 GeometryStore;
    typedef          RTCacheGeometryStorePtr              GeometryStorePtr;

    enum 
    { 
        PrimIdxStoreFieldId   = Inherited::NextFieldId,

        NextFieldId           = PrimIdxStoreFieldId + 1
    };

    static const BitVector PrimIdxStoreFieldMask    = 
        Inherited::NextFieldMask;
    
    static const BitVector NextFieldMask            = 
        PrimIdxStoreFieldMask << 1;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    OSG_ABSTR_FIELD_CONTAINER_DECL;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    struct BBoxEdge 
    {
        enum EdgeType
        { 
            START, 
            END 
        };

        Real32   _rT;
        Int32    _iPrimId;
        EdgeType _eEdgeType;

        BBoxEdge(void);
        BBoxEdge(Real32 rT, Int32 iPrimNum, bool bStarting);

        bool operator<(const BBoxEdge &rhs) const;
    };


    typedef          std::vector<BoxVolume>             BBoxStore;
    typedef          std::vector<BBoxEdge >             BBoxEdgeStore;
    typedef typename std::vector<BBoxEdge >::iterator   BBoxEdgeStoreIt;

    typedef          std::vector<Int32>                 IndexStore;
    typedef          std::vector<Int32>::iterator       IndexIterator;
    typedef          std::vector<Int32>::size_type      IndexSize;

    typedef          MFRTCachePrimIdxStore              PrimIdxStore;


    RTKDNode *buildTree(const BoxVolume     &nodeBounds,
                        const BBoxStore     &vBoundsStore, 
                              IndexIterator  currentPrimsIt,
                              IndexSize      numCurrentPrims, 
                              Int32          iCurrentDepth, 
                              BBoxEdgeStore  vEdgeStore[3],
                              IndexIterator  itPrimStore0, 
                              IndexIterator  itPrimStore1, 
                              Int32          iNumBadRefines = 0);

    RTKDNode *buildInternalTree(void              );
    void      initAccel        (BBoxStore &vBounds);

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    static const Int32         otherAxis[3][2];
    static       TypeObject   _type;

    MFRTCachePrimIdxStore     _mfPrimitives;

	Int32                     _iIsectCost;
	Int32                     _iTravCost;
	Real32                    _fEmptyBonus;
	Int32                     _iMaxPrims;
	Int32                     _iMaxDepth;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    RTCacheKDBase(void);
    RTCacheKDBase(const RTCacheKDBase &source);

    virtual ~RTCacheKDBase(void); 

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual UInt32 getBinSize (ConstFieldMaskArg  whichField);
    virtual void   copyToBin  (BinaryDataHandler &pMem,
                               ConstFieldMaskArg  whichField);
    virtual void   copyFromBin(BinaryDataHandler &pMem,
                               ConstFieldMaskArg  whichField);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    EditFieldHandlePtr editHandlePrimIdxStore  (void);
    GetFieldHandlePtr  getHandlePrimIdxStore   (void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

#ifdef OSG_MT_CPTR_ASPECT
            void execSync (      RTCacheKDBase      *pFrom,
                                 ConstFieldMaskArg   whichField,
                                 AspectOffsetStore  &oOffsets,
                                 ConstFieldMaskArg   syncMode  ,
                           const UInt32              uiSyncInfo);
#endif

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    static void classDescInserter(TypeObject &oType);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Init                                   */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
   /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;

    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const RTCacheKDBase &source);
};

OSG_END_NAMESPACE

#include "OSGRTCacheKDBase.inl"

#endif /* _OSGRTCACHEKDBASE_H_ */
