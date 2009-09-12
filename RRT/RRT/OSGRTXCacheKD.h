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

#ifndef _OSGRTXCACHEKD_H_
#define _OSGRTXCACHEKD_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTXCacheKDBase.h"

OSG_BEGIN_NAMESPACE

/*! Memory, simple reference counted memory object. Parent of
  everything that should be shared, but must not be thread safe.
  \ingroup GrpBaseBase
 */

template<typename DescT>
class RTXCacheKD : public RTXCacheKDBase<DescT>
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
              KDFloatElemStack >::type        KDElemStack;
    
    typedef          KDElemStack                        ElemStack;

    typedef          DescT                              Desc;
    typedef          RTXCacheKD<DescT>                  Self;

    typedef typename DescT::BasicRayPacket              BasicRayPacket;
    typedef typename DescT::BasicSIMDRayPacket          BasicSIMDRayPacket;
    
    OSG_GEN_INTERNALPTR(Self);

    typedef          RTXCacheKDBase<DescT>              Inherited;
    typedef typename Inherited::TypeObject              TypeObject;
    typedef typename TypeObject::InitPhase              InitPhase;
    
    typedef          SFieldAdaptor<ObjPtr, 
                                   SFFieldContainerPtr> SField;
    typedef          MFieldAdaptor<ObjPtr, 
                                   MFFieldContainerPtr> MField;
    

    enum 
    { 
        KDTreeFieldId         = Inherited::NextFieldId, 
        NextFieldId           = KDTreeFieldId + 1
    };

    static const BitVector KDTreeFieldMask        = 
        Inherited::NextFieldMask;

    static const BitVector NextFieldMask          = 
        KDTreeFieldMask << 1;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    OSG_FIELD_CONTAINER_TMPL_DECL;

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

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    void intersect            (BasicRayPacket     &oRay, 
                               RTHitPacket        &oHit,
                               UInt32              uiCacheId   );

    void intersect            (BasicSIMDRayPacket &oRay, 
                               RTHitSIMDPacket    &oHit,
                               UInt32              uiCacheId,
                               UInt16             *uiActive    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    UInt32      getNumNodes         (void);
    boxVolume_t getRootBoxVolume    (void);
    UInt32      getNumTriangles     (void);
    UInt32      getPrimitiveListSize(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

          boxVolume_t         *getBoxCachePointer   (void              );

          cacheKDNode_t       *getXCacheKDPointer   (void              );

    const RTCachePrimIdxStore &getPrimitiveIndexList(UInt32 nodePrimIdx);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef typename Inherited::IndexStore      IndexStore;
    typedef typename Inherited::IndexIterator   IndexIterator;
    typedef typename Inherited::IndexSize       IndexSize;
    typedef typename Inherited::BBoxStore       BBoxStore;
    typedef typename Inherited::BBoxEdge        BBoxEdge;
    typedef typename Inherited::BBoxEdgeStore   BBoxEdgeStore;
    typedef typename Inherited::BBoxEdgeStoreIt BBoxEdgeStoreIt;

    typedef typename Inherited::PrimIdxStore    PrimIdxStore;


    struct KDStackElem 
    {
        Real32         tmin;
        Real32         tmax;

        union
        {
            const RTXCacheKDNode *node;
                  UIntPointer     addr;
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
            const RTXCacheKDNode *node;
                  unsigned int    addr;
        };

        unsigned int   pad0;
        unsigned int   pad1;
        unsigned int   pad2;

        KDSIMDStackElem(void);
        KDSIMDStackElem(const KDSIMDStackElem &source);
        void operator =(const KDSIMDStackElem &source);
    };

    void flattenTree(RTXKDNode *pLeft, 
                     RTXKDNode *pRight);

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    static TypeObject  _type;

    MFRTXCacheKDNode   _mfKDTree;

    UInt32             _uiNumAllocedNodes;
    UInt32             _uiNextFreeNode;


    UInt32              nextFreeLeaf;
    UInt32              boxShift;
    UInt32              nodeShift;

    RTXCacheKDNode    * XKDTree;
    boxVolume_t       * boxVolumeStore;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    RTXCacheKD(void);
    RTXCacheKD(const RTXCacheKD &source);
    virtual ~RTXCacheKD(void); 

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

#ifdef OSG_MT_CPTR_ASPECT
    virtual ObjPtr createAspectCopy(void) const;
#endif

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    EditFieldHandlePtr editHandleKDTree(void);
    GetFieldHandlePtr  getHandleKDTree (void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

#ifdef OSG_MT_CPTR_ASPECT
    virtual void execSyncV(      FieldContainer     &oFrom,
                                 ConstFieldMaskArg   whichField,
                                 AspectOffsetStore  &oOffsets,
                                 ConstFieldMaskArg   syncMode  ,
                           const UInt32              uiSyncInfo);

            void execSync (      RTXCacheKD         *pFrom,
                                 ConstFieldMaskArg   whichField,
                                 AspectOffsetStore  &oOffsets,
                                 ConstFieldMaskArg   syncMode  ,
                           const UInt32              uiSyncInfo);
#endif

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Init                                   */
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
    void operator =(const RTXCacheKD &source);
};

template<typename DescT>
struct FieldTraits<RTXCacheKD<DescT> *> : 
    public FieldTraitsFCPtrBase<RTXCacheKD<DescT> *>
{
  private:

    static  DataType                         _type;

  public:

    typedef FieldTraits<RTXCacheKD<DescT> *>  Self;


    enum             { Convertible = Self::NotConvertible };

    static OSG_SYSTEM_DLLMAPPING
    DataType &getType      (void);

    static const Char8    *getSName     (void) 
    { 
        return DescT::getSFKDCacheName();
    }

    static const Char8    *getMName     (void) 
    {
        return DescT::getMFKDCacheName();
    }
};

OSG_END_NAMESPACE

#include "OSGRTXCacheKD.inl"

#endif /* _OSGRTXCACHEKD_H_ */