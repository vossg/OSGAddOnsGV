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

#ifndef _OSGRTCACHEBIH_H_
#define _OSGRTCACHEBIH_H_
#ifdef __sgi
#pragma once
#endif
#include "OSGRTCacheBIHBase.h"

OSG_BEGIN_NAMESPACE

template<typename DescT>
class RTCacheBIH : public RTCacheBIHBase<DescT>
{
  protected:

    struct BIHStackElem;
    struct BIHSIMDStackElem;

    /*==========================  PUBLIC  =================================*/

  public:

    typedef std::vector<BIHStackElem>          BIHFloatElemStack;

    typedef AlignedAllocator<BIHSIMDStackElem> SIMDStackAllocator;

    typedef std::vector<BIHSIMDStackElem, 
                        SIMDStackAllocator>    BIHSIMDElemStack;

    typedef typename
        boost::mpl::if_<boost::mpl::bool_<(DescT::SIMDMath == true)>,
                        BIHSIMDElemStack,
                        BIHFloatElemStack >::type        BIHElemStack;


    typedef          BIHElemStack                       ElemStack;

    typedef          DescT                              Desc;
    typedef          RTCacheBIH<DescT>                  Self;
    
    typedef typename DescT::BasicRayPacket              BasicRayPacket;
    typedef typename DescT::BasicSIMDRayPacket          BasicSIMDRayPacket;

    OSG_GEN_INTERNALPTR(Self);
    
    typedef          RTCacheBIHBase<DescT>              Inherited;
    typedef typename Inherited::TypeObject              TypeObject;
    typedef typename TypeObject::InitPhase              InitPhase;

    typedef          SFieldAdaptor<ObjPtr, 
                                   SFFieldContainerPtr> SField;
    typedef          MFieldAdaptor<ObjPtr, 
                                   MFFieldContainerPtr> MField;

    enum 
    { 
        BIHTreeFieldId        = Inherited::NextFieldId, 
        NextFieldId           = BIHTreeFieldId + 1
    };

    static const BitVector BIHTreeFieldMask       = 
        Inherited::NextFieldMask;

    static const BitVector NextFieldMask          = 
        BIHTreeFieldMask << 1;

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

    void intersect            (BasicRayPacket     &oRay, 
                               RTHitPacket        &oHit,
                               BIHElemStack       &sKDToDoStack,
                               UInt32              uiCacheId   );

    void intersect            (BasicSIMDRayPacket &oRay, 
                               RTHitSIMDPacket    &oHit,
                               BIHElemStack       &sKDToDoStack,
                               UInt32              uiCacheId   ,
                               UInt32             *uiActive    );

    void intersectSingle      (BasicSIMDRayPacket &oRay, 
                               RTHitSIMDPacket    &oHit,
                               BIHElemStack       &sKDToDoStack,
                               UInt32              uiCacheId   ,
                               UInt32             *uiActive    );

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    struct BIHStackElem 
    {
        Real32         tmin;
        Real32         tmax;

        union
        {
            const RTCacheBIHNode *node;
                  UIntPointer     addr;
        };
    };

    struct BIHSIMDStackElem 
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
            const RTCacheBIHNode *node;
                  unsigned int    addr;
        };

        unsigned int   pad0;
        unsigned int   pad1;
        unsigned int   pad2;

        BIHSIMDStackElem(void);
        BIHSIMDStackElem(const BIHSIMDStackElem &source);
        void operator  =(const BIHSIMDStackElem &source);
    };

    /*---------------------------------------------------------------------*/
    /*! \name                 Reference Counting                           */
    /*! \{                                                                 */

    static TypeObject _type;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    RTCacheBIH(void);
    RTCacheBIH(const RTCacheBIH &source);
    virtual ~RTCacheBIH(void); 

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

    EditFieldHandlePtr editHandleBIHTree(void);
    GetFieldHandlePtr  getHandleBIHTree (void) const;

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

            void execSync (      RTCacheBIH         *pFrom,
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
    void operator =(const RTCacheBIH &source);
};

template<typename DescT>
struct FieldTraits<RTCacheBIH<DescT> *> : 
    public FieldTraitsFCPtrBase<RTCacheBIH<DescT> *>
{
  private:

    static  DataType                                       _type;

  public:

    typedef FieldTraits<RTCacheBIH<DescT> *>  Self;


    enum             { Convertible = Self::NotConvertible                  };

    static OSG_SYSTEM_DLLMAPPING
                 DataType &getType      (void);

    static const Char8    *getSName     (void) 
    { 
        return DescT::getSFBIHCacheName();
    }

    static const Char8    *getMName     (void) 
    {
        return DescT::getMFBIHCacheName();
    }
};

OSG_END_NAMESPACE

#include "OSGRTCacheBIH.inl"

#endif /* _OSGRTCACHEBIH_H_ */
