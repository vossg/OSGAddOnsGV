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

OSG_BEGIN_NAMESPACE

template<typename DescT> inline
const typename RTCacheBase<DescT>::GeometryStorePtr 
    RTCacheBase<DescT>::getGeoStore(UInt32 uiIndex)
{
    return _mfGeos[uiIndex];
}

template<typename DescT> inline
const typename RTCacheBase<DescT>::MFTriangleAccel  &
    RTCacheBase<DescT>::getTrianglAccel(void)
{
    return _mfTriangleAcc;
}

template<typename DescT> inline
RTCacheBase<DescT>::RTCacheBase(void) :
     Inherited       (    ),
    _mfGeos          (    ),
    _mfTriangleAcc   (    ),
    _sfBoundingVolume(    )
{
}

template<typename DescT> inline
RTCacheBase<DescT>::RTCacheBase(const RTCacheBase &source) :
     Inherited       (source                  ),
    _mfGeos          (source._mfGeos          ),
    _mfTriangleAcc   (source._mfTriangleAcc   ),
    _sfBoundingVolume(source._sfBoundingVolume)
{
}

template<typename DescT> inline
RTCacheBase<DescT>::~RTCacheBase(void)
{
    for(UInt32 i = 0; i < _mfGeos.size(); ++i)
    {
        _mfGeos.replace(i, NULL);
    }
}

template<typename DescT> inline
UInt32 RTCacheBase<DescT>::getBinSize(ConstFieldMaskArg  whichField)
{
    UInt32 returnValue = Inherited::getBinSize(whichField);

    if(FieldBits::NoField != (GeoStoreFieldMask & whichField))
    {
        returnValue += _mfGeos.getBinSize();
    }

    if(FieldBits::NoField != (TriangleAccelFieldMask & whichField))
    {
        returnValue += _mfTriangleAcc.getBinSize();
    }

    if(FieldBits::NoField != (BoundingVolumeFieldMask & whichField))
    {
        returnValue += _sfBoundingVolume.getBinSize();
    }

    return returnValue;
}

template<typename DescT> inline
void RTCacheBase<DescT>::copyToBin(BinaryDataHandler &pMem,
                                   ConstFieldMaskArg  whichField)
{
    Inherited::copyToBin(pMem, whichField);

    if(FieldBits::NoField != (GeoStoreFieldMask & whichField))
    {
        _mfGeos.copyToBin(pMem);
    }

    if(FieldBits::NoField != (TriangleAccelFieldMask & whichField))
    {
        _mfTriangleAcc.copyToBin(pMem);
    }

    if(FieldBits::NoField != (BoundingVolumeFieldMask & whichField))
    {
        _sfBoundingVolume.copyToBin(pMem);
    }
}

template<typename DescT> inline
void RTCacheBase<DescT>::copyFromBin(BinaryDataHandler &pMem,
                                     ConstFieldMaskArg  whichField)
{
    Inherited::copyFromBin(pMem, whichField);

    if(FieldBits::NoField != (GeoStoreFieldMask & whichField))
    {
        _mfGeos.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (TriangleAccelFieldMask & whichField))
    {
        _mfTriangleAcc.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (BoundingVolumeFieldMask & whichField))
    {
        _sfBoundingVolume.copyFromBin(pMem);
    }
}

OSG_ABSTR_FIELD_CONTAINER_TMPL_DEF  (RTCacheBase, DescT)
//OSG_RC_GET_STATIC_TYPE_INL_TMPL_DEF   (DynFieldAttachment, AttachmentDescT)
OSG_RC_GET_STATIC_TYPE_ID_INL_TMPL_DEF(RTCacheBase, DescT)


template<typename DescT> inline
void RTCacheBase<DescT>::execSync(      RTCacheBase        *pFrom,
                                        ConstFieldMaskArg   whichField,
                                        AspectOffsetStore  &oOffsets,
                                        ConstFieldMaskArg   syncMode  ,
                                  const UInt32              uiSyncInfo)
{
    Inherited::execSync(pFrom, whichField, oOffsets, syncMode, uiSyncInfo);
}

template<typename DescT> inline
void RTCacheBase<DescT>::addGeoStore(RTCacheGeometryStore *pStore)
{
    editMField(GeoStoreFieldMask, _mfGeos);

    _mfGeos.push_back(pStore);
}

template<typename DescT> inline
EditFieldHandlePtr RTCacheBase<DescT>::editHandleGeoStore(void)
{
    MFUnrecRTCacheGeometryStorePtr::EditHandlePtr returnValue(
        new  MFUnrecRTCacheGeometryStorePtr::EditHandle(
             &_mfGeos, 
             this->getType().getFieldDesc(GeoStoreFieldId)));

    returnValue->setAddMethod(boost::bind(&RTCacheBase::addGeoStore, 
                                          this, 
                                          _1));
    return returnValue;
}

template<typename DescT> inline
GetFieldHandlePtr RTCacheBase<DescT>::getHandleGeoStore(void) const
{
    MFUnrecRTCacheGeometryStorePtr::GetHandlePtr returnValue(
        new  MFUnrecRTCacheGeometryStorePtr::GetHandle(
             &_mfGeos, 
             this->getType().getFieldDesc(GeoStoreFieldId)));

    return returnValue;
}

template<typename DescT> inline
EditFieldHandlePtr RTCacheBase<DescT>::editHandleTriangleAccel(void)
{
    typename MFTriangleAccel::EditHandlePtr returnValue(
        new typename MFTriangleAccel::EditHandle(
             &_mfTriangleAcc, 
             this->getType().getFieldDesc(TriangleAccelFieldId)));

    editMField(TriangleAccelFieldMask, _mfTriangleAcc);

    return returnValue;
}

template<typename DescT> inline
GetFieldHandlePtr RTCacheBase<DescT>::getHandleTriangleAccel(void) const
{
    typename MFTriangleAccel::GetHandlePtr returnValue(
        new typename MFTriangleAccel::GetHandle(
             &_mfTriangleAcc, 
             this->getType().getFieldDesc(TriangleAccelFieldId)));

    return returnValue;
}

template<typename DescT> inline
EditFieldHandlePtr RTCacheBase<DescT>::editHandleBoundingVolume(void)
{
    SFBoxVolume::EditHandlePtr returnValue(
        new  SFBoxVolume::EditHandle(
             &_sfBoundingVolume, 
             this->getType().getFieldDesc(BoundingVolumeFieldId)));

    editSField(BoundingVolumeFieldMask);

    return returnValue;
}


template<typename DescT> inline
GetFieldHandlePtr  RTCacheBase<DescT>::getHandleBoundingVolume(void) const
{
    SFBoxVolume::GetHandlePtr returnValue(
        new  SFBoxVolume::GetHandle(
             &_sfBoundingVolume, 
             this->getType().getFieldDesc(BoundingVolumeFieldId)));

    return returnValue;
}

template<typename DescT> inline
void RTCacheBase<DescT>::addGeometry(Geometry      *pGeo,
                                     Matrixr       &oMatrix,
                                     State         *pState,
                                     StateOverride *pStateOverride)
{
    if(pGeo != NULL)
    {
        GeometryStoreUnrecPtr oNewElem = GeometryStore::create();

        oNewElem->setGeo   (pGeo   );
        oNewElem->setMatrix(oMatrix);
        oNewElem->setState (pState );

//        oNewElem._pStateOverride = NULL;

        _mfGeos.push_back(oNewElem);
    }
}

template<typename DescT> inline
void RTCacheBase<DescT>::initAccel(BBoxStore &vBounds)
{
    _sfBoundingVolume.getValue().setEmpty();

    for(UInt32 i = 0; i < _mfGeos.size(); ++i)
    {
        Geometry *pGeo = _mfGeos[i]->getGeo();

        GeoVectorProperty *posPtr = pGeo->getPositions();

        TriangleAccel triangleAccel;
        Pnt3f         a;
        Pnt3f         b;
        Pnt3f         c;

        const Matrixr &mToWorld = _mfGeos[i]->getMatrix();

        for(TriangleIterator tI  = pGeo->beginTriangles(); 
                             tI != pGeo->endTriangles  ();
                           ++tI)
        {
            posPtr->getValue(a, tI.getPositionIndex(0));
            posPtr->getValue(b, tI.getPositionIndex(1));
            posPtr->getValue(c, tI.getPositionIndex(2));
            
            mToWorld.mult(a);
            mToWorld.mult(b);
            mToWorld.mult(c);

            triangleAccel.setup(a, b, c, i, tI.getIndex());
            
            _mfTriangleAcc.push_back(triangleAccel);

            BoxVolume triBox;

            triBox.extendBy(a);
            triBox.extendBy(b);
            triBox.extendBy(c);

            vBounds.push_back(triBox);

            _sfBoundingVolume.getValue().extendBy(triBox);
        }
        
    }

    fprintf(stderr, "RTCache::build %d\n", _mfTriangleAcc.size());

    fprintf(stderr, "Cache bounds %f %f %f -> %f %f %f\n",
            _sfBoundingVolume.getValue().getMin()[0],
            _sfBoundingVolume.getValue().getMin()[1],
            _sfBoundingVolume.getValue().getMin()[2],
            _sfBoundingVolume.getValue().getMax()[0],
            _sfBoundingVolume.getValue().getMax()[1],
            _sfBoundingVolume.getValue().getMax()[2]);
}

OSG_END_NAMESPACE
