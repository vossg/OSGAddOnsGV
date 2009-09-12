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

#include "OSGMemoryObject.h"
#include "OSGContribRRTDef.h"
#include "OSGGeometry.h"
#include "OSGLine.h"

OSG_BEGIN_NAMESPACE

class State;
class StateOverride;

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTCacheKDBase : public MemoryObject
{

    /*==========================  PUBLIC  =================================*/

  public:

    struct GeometryStore
    {
        GeometryPtr    _pGeo;
        Matrixr        _oMatrix;
        State         *_pState;
        StateOverride *_pStateOverride;    
    };

    typedef          DescT               Desc;
    typedef typename Desc::TriangleAccel TriangleAccel;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    RTCacheKDBase(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    void addGeometry(GeometryPtr    pGeo,
                     Matrixr       &oMatrix,
                     State         *pState,
                     StateOverride *pStateOverride);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    const GeometryStore &getGeoStore(UInt32 uiIndex);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef MemoryObject Inherited;

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

    typedef          std::vector< std::vector<UInt32> > PrimIdxStore;


    struct RTKDNode
    {
      protected:

        bool                 _bIsLeave;

        UInt32               _uiSplitAxis;
        Real32               _fSplitPos;

        RTKDNode            *_pAboveChild;
        RTKDNode            *_pBelowChild;

        UInt32               _uiPrimIdx;

      public:

        RTKDNode(void);
        ~RTKDNode(void);

        void initLeaf    (IndexIterator  primNums, 
                          IndexSize      np,
                          PrimIdxStore  &vStore); 
        
        void initInterior(UInt32        uiAxis, 
                          Real32        fSplitPos);
        

        bool                 isLeave         (void           );
        
        void                 setAboveChild   (RTKDNode *pNode);
        RTKDNode            *getAboveChild   (void           );

        void                 setBelowChild   (RTKDNode *pNode);
        RTKDNode            *getBelowChild   (void           );
        
        UInt32               getSplitAxis    (void           );
        Real32               getSplitPos     (void           );

        UInt32               getPrimitiveIdx (void           );
    };

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

    static const Int32 otherAxis[3][2];

    std::vector<GeometryStore> _vGeos;
    std::vector<TriangleAccel> _vTriangleAcc;
    PrimIdxStore               _vPrimitives;

    BoxVolume                  _bBoundingVolume;
	Int32                      _iIsectCost;
	Int32                      _iTravCost;
	Real32                     _fEmptyBonus;
	Int32                      _iMaxPrims;
	Int32                      _iMaxDepth;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTCacheKDBase(void); 

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
    RTCacheKDBase(const RTCacheKDBase &source);
    void operator =(const RTCacheKDBase &source);
};

OSG_END_NAMESPACE

#include "OSGRTCacheKDBase.inl"

#endif /* _OSGRTCACHEKDBASE_H_ */
