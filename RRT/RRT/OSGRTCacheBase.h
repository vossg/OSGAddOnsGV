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

#ifndef _OSGRTCACHEBASE_H_
#define _OSGRTCACHEBASE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGFieldContainer.h"
#include "OSGContribRRTDef.h"
#include "OSGLine.h"

#include "OSGRTCacheGeometryStore.h"
#include "OSGRTCacheGeometryStoreFields.h"
#include "OSGRTTriangleAccelFields.h"

#include <boost/bind.hpp>

OSG_BEGIN_NAMESPACE

/*! Memory, simple reference counted memory object. Parent of
    everything that should be shared, but must not be thread safe.
    \ingroup GrpBaseBase
 */

template<typename DescT>
class RTCacheBase : public FieldContainer
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef          DescT                                Desc;
    typedef typename Desc::TriangleAccel                  TriangleAccel;
    typedef typename Desc::MFTriangleAccel                MFTriangleAccel;

    typedef          RTCacheBase<DescT>                   Self;

    OSG_GEN_INTERNALPTR(Self);

    typedef          FieldContainer                       Inherited;
    typedef typename Inherited::TypeObject                TypeObject;
    typedef typename TypeObject::InitPhase                InitPhase;

    typedef          RTCacheGeometryStore                 GeometryStore;
    typedef          RTCacheGeometryStorePtr              GeometryStorePtr;

    enum 
    { 
        GeoStoreFieldId       = Inherited::NextFieldId, 
        TriangleAccelFieldId  = GeoStoreFieldId       + 1,
        BoundingVolumeFieldId = TriangleAccelFieldId  + 1,

        NextFieldId           = BoundingVolumeFieldId + 1
    };

    static const BitVector GeoStoreFieldMask        = 
        Inherited::NextFieldMask;

    static const BitVector TriangleAccelFieldMask   = 
        GeoStoreFieldMask       << 1;
    
    static const BitVector BoundingVolumeFieldMask  = 
        TriangleAccelFieldMask  << 1;

    static const BitVector NextFieldMask            = 
        BoundingVolumeFieldMask << 1;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */
 
    OSG_ABSTR_FIELD_CONTAINER_DECL;

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

    const GeometryStorePtr getGeoStore(UInt32 uiIndex);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*---------------------------------------------------------------------*/

    typedef std::vector<BoxVolume> BBoxStore;

    /*---------------------------------------------------------------------*/

    static TypeObject _type;

    /*---------------------------------------------------------------------*/

    MFRTCacheGeometryStorePtr _mfGeos;
    MFTriangleAccel           _mfTriangleAcc;
    SFBoxVolume               _sfBoundingVolume;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    RTCacheBase(void);
    RTCacheBase(const RTCacheBase &source);

    virtual ~RTCacheBase(void); 

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

    void addGeoStore(RTCacheGeometryStorePtr pStore);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void initAccel(BBoxStore &vBounds);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    EditFieldHandlePtr editHandleGeoStore      (void);
    GetFieldHandlePtr  getHandleGeoStore       (void) const;

    EditFieldHandlePtr editHandleTriangleAccel (void);
    GetFieldHandlePtr  getHandleTriangleAccel  (void) const;

    EditFieldHandlePtr editHandleBoundingVolume(void);
    GetFieldHandlePtr  getHandleBoundingVolume (void) const;
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

#ifdef OSG_MT_CPTR_ASPECT
            void execSync (      RTCacheBase        *pFrom,
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
    void operator =(const RTCacheBase &source);
};

OSG_END_NAMESPACE

#include "OSGRTCacheBase.inl"

#endif /* _OSGRTCACHEBASE_H_ */
