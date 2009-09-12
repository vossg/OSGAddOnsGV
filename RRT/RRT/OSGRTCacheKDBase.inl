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

template<typename DescT> 
const Int32 RTCacheKDBase<DescT>::otherAxis[3][2] = { {1,2}, {0,2}, {0,1} };

template<typename DescT> inline
RTCacheKDBase<DescT>::BBoxEdge::BBoxEdge(void)
{
}

template<typename DescT> inline
RTCacheKDBase<DescT>::BBoxEdge::BBoxEdge(Real32 rT, 
                                         Int32  iPrimId, 
                                         bool   bStarting) 
{
    _rT        = rT;
    _iPrimId   = iPrimId;
    _eEdgeType = bStarting ? START : END;
}

template<typename DescT> inline
bool RTCacheKDBase<DescT>::BBoxEdge::operator<(const BBoxEdge &rhs) const 
{
    if(_rT == rhs._rT)
    {
        return (Int32) _eEdgeType < (Int32) rhs._eEdgeType;
    }
    else 
    {
        return _rT < rhs._rT;
    }
}


template<typename DescT> inline
const typename RTCacheKDBase<DescT>::GeometryStorePtr 
    RTCacheKDBase<DescT>::getGeoStore(UInt32 uiIndex)
{
    return _mfGeos[uiIndex];
}



template<typename DescT> inline
RTCacheKDBase<DescT>::RTCacheKDBase(void) :
     Inherited       (    ),
    _mfGeos          (    ),
    _mfTriangleAcc   (    ),
    _mfPrimitives    (    ),
    _sfBoundingVolume(    ),
    _iIsectCost      (80  ),
    _iTravCost       (1   ),
    _fEmptyBonus     (0.5f),
    _iMaxPrims       (1   ),
    _iMaxDepth       (-1  )
{
}

template<typename DescT> inline
RTCacheKDBase<DescT>::~RTCacheKDBase(void)
{
    for(UInt32 i = 0; i < _mfGeos.size(); ++i)
    {
        OSG::subRef(_mfGeos[i]);
    }
}

template<typename DescT> inline
UInt32 RTCacheKDBase<DescT>::getBinSize(ConstFieldMaskArg  whichField)
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

    if(FieldBits::NoField != (PrimIdxStoreFieldMask & whichField))
    {
        returnValue += _mfPrimitives.getBinSize();
    }

    if(FieldBits::NoField != (BoundingVolumeFieldMask & whichField))
    {
        returnValue += _sfBoundingVolume.getBinSize();
    }

    return returnValue;
}

template<typename DescT> inline
void RTCacheKDBase<DescT>::copyToBin(BinaryDataHandler &pMem,
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

    if(FieldBits::NoField != (PrimIdxStoreFieldMask & whichField))
    {
        _mfPrimitives.copyToBin(pMem);
    }

    if(FieldBits::NoField != (BoundingVolumeFieldMask & whichField))
    {
        _sfBoundingVolume.copyToBin(pMem);
    }
}

template<typename DescT> inline
void RTCacheKDBase<DescT>::copyFromBin(BinaryDataHandler &pMem,
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

    if(FieldBits::NoField != (PrimIdxStoreFieldMask & whichField))
    {
        _mfPrimitives.copyFromBin(pMem);
    }

    if(FieldBits::NoField != (BoundingVolumeFieldMask & whichField))
    {
        _sfBoundingVolume.copyFromBin(pMem);
    }
}


OSG_ABSTR_FIELD_CONTAINER_TMPL_DEF  (RTCacheKDBase, DescT)
//OSG_RC_GET_STATIC_TYPE_INL_TMPL_DEF   (DynFieldAttachment, AttachmentDescT)
OSG_RC_GET_STATIC_TYPE_ID_INL_TMPL_DEF(RTCacheKDBase, DescT)


template<typename DescT> inline
void RTCacheKDBase<DescT>::execSync(      RTCacheKDBase      *pFrom,
                                          ConstFieldMaskArg   whichField,
                                          AspectOffsetStore  &oOffsets,
                                          ConstFieldMaskArg   syncMode  ,
                                    const UInt32              uiSyncInfo)
{
    Inherited::execSync(pFrom, whichField, oOffsets, syncMode, uiSyncInfo);
}


template<typename DescT> inline
void RTCacheKDBase<DescT>::addGeoStore(RTCacheGeometryStorePtr pStore)
{
    editMField(GeoStoreFieldMask, _mfGeos);

    _mfGeos.push_back(pStore);
}

template<typename DescT> inline
EditFieldHandlePtr RTCacheKDBase<DescT>::editHandleGeoStore(void)
{
    MFRTCacheGeometryStorePtr::EditHandlePtr returnValue(
        new  MFRTCacheGeometryStorePtr::EditHandle(
             &_mfGeos, 
             this->getType().getFieldDesc(GeoStoreFieldId)));

    returnValue->setAddMethod(boost::bind(&RTCacheKDBase::addGeoStore, 
                                          this, 
                                          _1));
    return returnValue;
}

template<typename DescT> inline
GetFieldHandlePtr RTCacheKDBase<DescT>::getHandleGeoStore(void) const
{
    MFRTCacheGeometryStorePtr::GetHandlePtr returnValue(
        new  MFRTCacheGeometryStorePtr::GetHandle(
             &_mfGeos, 
             this->getType().getFieldDesc(GeoStoreFieldId)));

    return returnValue;
}

template<typename DescT> inline
EditFieldHandlePtr RTCacheKDBase<DescT>::editHandleTriangleAccel(void)
{
    typename MFTriangleAccel::EditHandlePtr returnValue(
        new typename MFTriangleAccel::EditHandle(
             &_mfTriangleAcc, 
             this->getType().getFieldDesc(TriangleAccelFieldId)));

    editMField(TriangleAccelFieldMask, _mfTriangleAcc);

    return returnValue;
}

template<typename DescT> inline
GetFieldHandlePtr RTCacheKDBase<DescT>::getHandleTriangleAccel(void) const
{
    typename MFTriangleAccel::GetHandlePtr returnValue(
        new typename MFTriangleAccel::GetHandle(
             &_mfTriangleAcc, 
             this->getType().getFieldDesc(TriangleAccelFieldId)));

    return returnValue;
}

template<typename DescT> inline
EditFieldHandlePtr RTCacheKDBase<DescT>::editHandlePrimIdxStore(void)
{
    MFRTCachePrimIdxStore::EditHandlePtr returnValue(
        new  MFRTCachePrimIdxStore::EditHandle(
             &_mfPrimitives, 
             this->getType().getFieldDesc(PrimIdxStoreFieldId)));

    editMField(PrimIdxStoreFieldMask, _mfPrimitives);

    return returnValue;
}

template<typename DescT> inline
GetFieldHandlePtr RTCacheKDBase<DescT>::getHandlePrimIdxStore(void) const
{
    MFRTCachePrimIdxStore::GetHandlePtr returnValue(
        new  MFRTCachePrimIdxStore::GetHandle(
             &_mfPrimitives, 
             this->getType().getFieldDesc(PrimIdxStoreFieldId)));

    return returnValue;
}


template<typename DescT> inline
EditFieldHandlePtr RTCacheKDBase<DescT>::editHandleBoundingVolume(void)
{
    SFBoxVolume::EditHandlePtr returnValue(
        new  SFBoxVolume::EditHandle(
             &_sfBoundingVolume, 
             this->getType().getFieldDesc(BoundingVolumeFieldId)));

    editSField(BoundingVolumeFieldMask);

    return returnValue;
}


template<typename DescT> inline
GetFieldHandlePtr  RTCacheKDBase<DescT>::getHandleBoundingVolume(void) const
{
    SFBoxVolume::GetHandlePtr returnValue(
        new  SFBoxVolume::GetHandle(
             &_sfBoundingVolume, 
             this->getType().getFieldDesc(BoundingVolumeFieldId)));

    return returnValue;
}





template<typename DescT> inline
void RTCacheKDBase<DescT>::addGeometry(GeometryPtr    pGeo,
                                       Matrixr       &oMatrix,
                                       State         *pState,
                                       StateOverride *pStateOverride)
{
    if(pGeo != NULL)
    {
        GeometryStorePtr oNewElem = GeometryStore::create();

        oNewElem->setGeo   (pGeo   );
        oNewElem->setMatrix(oMatrix);
        oNewElem->setState (pState );

//        oNewElem._pStateOverride = NULL;

        _mfGeos.push_back(oNewElem);
    }
}


template<typename DescT> inline
void RTCacheKDBase<DescT>::initAccel(BBoxStore &vBounds)
{
    _sfBoundingVolume.getValue().setEmpty();

    for(UInt32 i = 0; i < _mfGeos.size(); ++i)
    {
        GeometryPtr pGeo = _mfGeos[i]->getGeo();

        GeoVectorPropertyPtr posPtr = pGeo->getPositions();

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

    fprintf(stderr, "RTCacheKD::build %d\n", _mfTriangleAcc.size());

    fprintf(stderr, "Cache bounds %f %f %f -> %f %f %f\n",
            _sfBoundingVolume.getValue().getMin()[0],
            _sfBoundingVolume.getValue().getMin()[1],
            _sfBoundingVolume.getValue().getMin()[2],
            _sfBoundingVolume.getValue().getMax()[0],
            _sfBoundingVolume.getValue().getMax()[1],
            _sfBoundingVolume.getValue().getMax()[2]);

	if(_iMaxDepth <= 0)
    {
		_iMaxDepth = osgRound2Int(8 + 1.3f * 
                                  osgLog2Int(float(_mfTriangleAcc.size())));
    }
}

template<typename DescT> inline
RTKDNode *RTCacheKDBase<DescT>::buildInternalTree(void)
{
    BBoxStore vBounds;

    initAccel(vBounds);

    BBoxEdgeStore vEdgeStore[3];

	for(Int32 i = 0; i < 3; ++i)
		vEdgeStore[i].resize(2 * _mfTriangleAcc.size());

    IndexStore vPrimStore0;
    IndexStore vPrimStore1;

    vPrimStore0.resize(_mfTriangleAcc.size());

    vPrimStore1.resize((_iMaxDepth + 1) * _mfTriangleAcc.size());

	for(UInt32 i = 0; i < _mfTriangleAcc.size(); ++i)
		vPrimStore0[i] = i;


    RTKDNode *pKDTree = buildTree(_sfBoundingVolume.getValue(), 
                                   vBounds, 
                                   vPrimStore0.begin     (),
                                  _mfTriangleAcc.size(), 
                                  _iMaxDepth, 
                                   vEdgeStore,
                                   vPrimStore0.begin     (), 
                                   vPrimStore1.begin     ());

    return pKDTree;
}

template<typename DescT> inline
RTKDNode *RTCacheKDBase<DescT>::buildTree(
    const BoxVolume     &nodeBounds,
    const BBoxStore     &vBoundsStore, 
          IndexIterator  currentPrimsIt,
          IndexSize      numCurrentPrims, 
          Int32          iCurrentDepth, 
          BBoxEdgeStore  vEdgeStore[3],
          IndexIterator  itPrimStore0, 
          IndexIterator  itPrimStore1, 
          Int32          iNumBadRefines) 
{
    RTKDNode *returnValue = NULL;

	if(numCurrentPrims <= _iMaxPrims || iCurrentDepth == 0) 
    {
        returnValue = new RTKDNode;

        returnValue->initLeaf( currentPrimsIt, 
                               numCurrentPrims,
                              _mfPrimitives    );

		return returnValue;
	}

	Int32  iCurrAxis     = 0;
	Int32  iBestAxis     = -1;
    Int32  iBestOffset   = -1;
	Real32 fBestCost     = INFINITY;
	Real32 fOldCost      = _iIsectCost * Real32(numCurrentPrims);

	Vec3f  vDiag         = nodeBounds.getMax() - nodeBounds.getMin();

	Real32 fTotalArea    = (2.f * (vDiag[0] * vDiag[1] + 
                                   vDiag[0] * vDiag[2] + 
                                   vDiag[1] * vDiag[2]));

	Real32 fInvTotalArea = 1.f / fTotalArea;


	if(vDiag[0] > vDiag[1] && vDiag[0] > vDiag[2]) 
    {
        iCurrAxis = 0;
    }
	else 
    {
        iCurrAxis = (vDiag[1] > vDiag[2]) ? 1 : 2;
    }

	Int32         iNumRetries = 0;
    IndexIterator primIt;

    do
    {
        primIt = currentPrimsIt;
        
        for(Int32 i = 0; i < numCurrentPrims; ++i) 
        {
            Int32 iPrimId = *primIt++;
            
            const BoxVolume &bbox = vBoundsStore[iPrimId];
            
            vEdgeStore[iCurrAxis][2 * i] =
                BBoxEdge(bbox.getMin()[iCurrAxis], iPrimId, true);

            vEdgeStore[iCurrAxis][2 * i + 1] =
                BBoxEdge(bbox.getMax()[iCurrAxis], iPrimId, false);
        }
        
        BBoxEdgeStoreIt edgesBegin = vEdgeStore[iCurrAxis].begin();
        BBoxEdgeStoreIt edgesEnd   = vEdgeStore[iCurrAxis].begin();
        
        edgesEnd += 2 * numCurrentPrims;
        
        std::sort(edgesBegin, edgesEnd);
               
        Int32 iNumBelow = 0;
        Int32 iNumAbove = numCurrentPrims;
        
        for(Int32 i = 0; i < 2 * numCurrentPrims; ++i) 
        {
            if(vEdgeStore[iCurrAxis][i]._eEdgeType == BBoxEdge::END) 
                --iNumAbove;
            
            Real32 fEdgeT = vEdgeStore[iCurrAxis][i]._rT;
            
            if(fEdgeT > nodeBounds.getMin()[iCurrAxis] &&
               fEdgeT < nodeBounds.getMax()[iCurrAxis]  ) 
            {
                Int32 otherAxis0  = otherAxis[iCurrAxis][0];
                Int32 otherAxis1  = otherAxis[iCurrAxis][1];

                Real32 rDiagSum   = vDiag[otherAxis0] + vDiag[otherAxis1];
                Real32 rDiagProd  = vDiag[otherAxis0] * vDiag[otherAxis1];

                Real32 fBelowArea = 2 * (rDiagProd +
                                         (fEdgeT - 
                                          nodeBounds.getMin()[iCurrAxis]) *
                                         rDiagSum);

                Real32 fAboveArea = 2 * (rDiagProd +
                                         (nodeBounds.getMax()[iCurrAxis] - 
                                          fEdgeT) *
                                         rDiagSum);

                Real32 fBelowProp = fBelowArea * fInvTotalArea;
                Real32 fAboveProp = fAboveArea * fInvTotalArea;

                Real32 eb = (iNumAbove == 0 || iNumBelow == 0) ? 
                    (1.f - _fEmptyBonus) : 1.f;

                Real32 fCurrentCost = 
                    _iTravCost + _iIsectCost * eb * (fBelowProp * iNumBelow + 
                                                     fAboveProp * iNumAbove);

                if(fCurrentCost < fBestCost)  
                {
                    fBestCost   = fCurrentCost;
                    iBestAxis   = iCurrAxis;
                    iBestOffset = i;
                }
            }

            if(vEdgeStore[iCurrAxis][i]._eEdgeType == BBoxEdge::START) 
                ++iNumBelow;
        }
        
        OSG_ASSERT(iNumBelow == numCurrentPrims && iNumAbove == 0); 
        
        if(iBestAxis == -1 && iNumRetries < 2) 
        {
            ++iNumRetries;
            iCurrAxis = (iCurrAxis + 1) % 3;
        }
    } while(iBestAxis == -1 && iNumRetries < 2);

	if(fBestCost > fOldCost) 
        ++iNumBadRefines;

	if((fBestCost > 4.f * fOldCost && numCurrentPrims < 16) ||
       iBestAxis == -1 || iNumBadRefines == 3) 
    {
        returnValue = new RTKDNode;
        
        returnValue->initLeaf( currentPrimsIt, 
                               numCurrentPrims,
                              _mfPrimitives    );
        
		return returnValue;
	}


	Int32 n0 = 0;
    Int32 n1 = 0;

    primIt = itPrimStore0;

	for(Int32 i = 0; i < iBestOffset; ++i)
    {
		if(vEdgeStore[iBestAxis][i]._eEdgeType == BBoxEdge::START)
        {
			*primIt++ = vEdgeStore[iBestAxis][i]._iPrimId;
            ++n0;
        }
    }

    primIt = itPrimStore1;

	for(Int32 i = iBestOffset + 1; i < 2 * numCurrentPrims; ++i)
    {
		if(vEdgeStore[iBestAxis][i]._eEdgeType == BBoxEdge::END)
        {
			*primIt++ = vEdgeStore[iBestAxis][i]._iPrimId;
            ++n1;
        }
    }

	Real tsplit = vEdgeStore[iBestAxis][iBestOffset]._rT;

    returnValue = new RTKDNode;

	returnValue->initInterior(iBestAxis, tsplit);


    Pnt3f newMin = nodeBounds.getMin();
    Pnt3f newMax = nodeBounds.getMax();

    newMax[iBestAxis] = tsplit;

	BoxVolume bounds0(newMin, newMax);



    newMin = nodeBounds.getMin();
    newMax = nodeBounds.getMax();

    newMin[iBestAxis] = tsplit;

    BoxVolume bounds1(newMin, newMax);



	RTKDNode *pBelowChild = buildTree(bounds0,
                                      vBoundsStore, 
                                      itPrimStore0, 
                                      n0, 
                                      iCurrentDepth - 1, 
                                      vEdgeStore,
                                      itPrimStore0, 
                                      itPrimStore1 + numCurrentPrims, 
                                      iNumBadRefines);


	RTKDNode *pAboveChild = buildTree(bounds1, 
                                      vBoundsStore,
                                      itPrimStore1, 
                                      n1, 
                                      iCurrentDepth - 1, 
                                      vEdgeStore,
                                      itPrimStore0, 
                                      itPrimStore1 + numCurrentPrims, 
                                      iNumBadRefines);

    returnValue->setAboveChild(pAboveChild);
    returnValue->setBelowChild(pBelowChild);

    return returnValue;
}

OSG_END_NAMESPACE
