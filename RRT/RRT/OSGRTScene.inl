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
RTScene<DescT>::RTScene(void) :
     Inherited  (                                        ),
    _eRayMode   (RTRaySIMDPacketInfo::SingleOriginQuadDir),
    _cBackground(0.4f, 0.4f, 0.4f, 1.f                   ),
    _pCam       (NULL                                    ),
    _vRTCaches  (                                        )
{
}

template<typename DescT> inline
RTScene<DescT>::~RTScene(void)
{
}

template<typename DescT> inline
void RTScene<DescT>::resolveLinks(void)
{
}

template<typename DescT> inline
typename RTScene<DescT>::RayMode RTScene<DescT>::getRayMode(void)
{
    return _eRayMode;
}

template<typename DescT> inline
void RTScene<DescT>::setRayMode(RayMode eMode)
{
    _eRayMode = eMode;
}

template<typename DescT> inline
void RTScene<DescT>::addCache(RTCache *pCache)
{
    if(pCache != NULL)
    {
        _vRTCaches.push_back(pCache);
    }
}

template<typename DescT> inline
void RTScene<DescT>::clearCache(void)
{
    _vRTCaches.clear();
}

template<typename DescT> inline
UInt32 RTScene<DescT>::getNumCaches(void)
{
    return _vRTCaches.size();
}


template<typename DescT> inline
void RTScene<DescT>::setCamera(RTCameraDecorator* pCam)
{
    _pCam = pCam;
}


template<typename DescT> inline
RTCameraDecorator *RTScene<DescT>::getCamera()
{
    return _pCam;
}

template<typename DescT> inline
const typename RTScene<DescT>::RTCacheStore &RTScene<DescT>::getCacheArray(void)
{
    return _vRTCaches;
}

template<typename DescT> inline
void RTScene<DescT>::tracePrimaryRays(BasicRayPacket &oRay, 
                                      HitPacket      &oHit,
                                      ElemStack      &sKDToDoStack,
                                      UInt16         *uiActive    )
{
    for(UInt32 k = 0; k < _vRTCaches.size(); ++k)
    {
#ifdef OSG_XCACHEKD
        _vRTCaches[k]->intersect(oRay, oHit, k);
#else
        _vRTCaches[k]->intersect(oRay, oHit, sKDToDoStack, k);
#endif
    }
}

template<typename DescT> inline
void RTScene<DescT>::tracePrimaryRaysSDQO(BasicRayPacket &oRay, 
                                          HitPacket      &oHit,
                                          ElemStack      &sKDToDoStack,
                                          UInt16         *uiActive    )
{
    OSG_ASSERT(false);
}

template<typename DescT> inline
void RTScene<DescT>::tracePrimaryRays(BasicSIMDRayPacket &oRay, 
                                      HitPacket          &oHit,
                                      ElemStack          &sKDToDoStack,
                                      UInt16             *uiActive    )
{
    for(UInt32 k = 0; k < _vRTCaches.size(); ++k)
    {
#ifdef OSG_XCACHEKD
        _vRTCaches[k]->intersect(oRay, oHit, k, uiActive);
#else
        _vRTCaches[k]->intersect(oRay, oHit, sKDToDoStack, k, uiActive);
#endif
    }
}

template<typename DescT> inline
void RTScene<DescT>::tracePrimaryRaysSDQO(BasicSIMDRayPacket &oRay, 
                                          HitPacket          &oHit,
                                          ElemStack          &sKDToDoStack,
                                          UInt16             *uiActive    )
{
    for(UInt32 k = 0; k < _vRTCaches.size(); ++k)
    {
#ifdef OSG_XCACHEKD
//        _vRTCaches[k]->intersect(oRay, oHit, k, uiActive);
#else
//        _vRTCaches[k]->intersect(oRay, oHit, sKDToDoStack, k, uiActive);
#endif
    }
}

template<typename DescT> inline
void RTScene<DescT>::shade(RTHitPacket    &oHit,
                           BasicRayPacket &oRay,
                           RTColorPacket  &oResult)
{

    if(oHit.getU() > -0.5)
    {
        const typename RTCache::GeometryStorePtr geoStore = 
            _vRTCaches[oHit.getCacheId()]->getGeoStore(oHit.getObjId());

        MaterialChunk *pMat = 
            dynamic_cast<MaterialChunk *>(
                geoStore->getState()->getChunk(
                    MaterialChunk::getStaticClassId()));

        if(pMat != NULL)
        {
            Geometry *pGeo = geoStore->getGeo();

            if(pGeo != NULL)
            {
                GeoVectorProperty *pNormals =
                    pGeo->getProperty(Geometry::NormalsIndex);

                GeoIntegralProperty *pNormalsIdx =
                    pGeo->getIndex(Geometry::NormalsIndex);
                
                if(pNormals != NULL && pNormalsIdx != NULL)
                {
                    Vec3f n1;
                    Vec3f n2;
                    Vec3f n3;

                    Real32 gamma = 1.f - (oHit.getU() + oHit.getV());

                    UInt32 idx = pNormalsIdx->getValue(oHit.getTriId() * 3);

                    pNormals->getValue(n1, idx);

                    idx = pNormalsIdx->getValue(oHit.getTriId() * 3 + 1);

                    pNormals->getValue(n2, idx);

                    idx = pNormalsIdx->getValue(oHit.getTriId() * 3 + 2);

                    pNormals->getValue(n3, idx);

                    n1[0] = 
                        n2[0] * oHit.getU() + 
                        n3[0] * oHit.getV() +
                        n1[0] * gamma;

                    n1[1] = 
                        n2[1] * oHit.getU() + 
                        n3[1] * oHit.getV() +
                        n1[1] * gamma;

                    n1[2] = 
                        n2[2] * oHit.getU() + 
                        n3[2] * oHit.getV() +
                        n1[2] * gamma;

                    n1.normalize();

                    n2 = oRay.getDir();
                    n2 *= -1.f;

                    gamma = n1.dot(n2);

                    if(gamma < 0.f)
                        gamma = 0.f;

                    oResult.set(pMat->getDiffuse()[0] * gamma,
                                pMat->getDiffuse()[1] * gamma,
                                pMat->getDiffuse()[2] * gamma,
                                1.f);
                                
                }
            }
            else
            {
                oResult.set(pMat->getDiffuse());
            }
        }
        else
        {
            oResult.set(0.8f, 0.8f, 0.8f, 1.0f);
        }
    }
    else
    {
        oResult.set(_cBackground[0], 
                    _cBackground[1], 
                    _cBackground[2], 
                    _cBackground[3]);
    }
}

template<typename DescT> inline
void RTScene<DescT>::shadeSDQO(RTHitPacket    &oHit,
                               BasicRayPacket &oRay,
                               RTColorPacket  &oResult)
{
    OSG_ASSERT(false);
}

template<typename DescT> inline
void RTScene<DescT>::shade(RTHitSIMDPacket    &oHit,
                           BasicSIMDRayPacket &oRay,
                           RTColorSIMDPacket  &oResult)
{
    Vec3f vRayDirs[4];

    union
    {
        Float4 dir;
        Real32 dirConv[4];
    };

    for(UInt32 i = 0; i < 3; ++i)
    {
        dir = oRay.getQuad(i);

        vRayDirs[0][i] = dirConv[0];
        vRayDirs[1][i] = dirConv[1];
        vRayDirs[2][i] = dirConv[2];
        vRayDirs[3][i] = dirConv[3];
    }

    for(UInt32 i = 0; i < RTHitSIMDPacket::NumHits; ++i)
    {
        if(oHit.getU(i) > -0.5)
        {
            const typename RTCache::GeometryStorePtr geoStore = 
                _vRTCaches[oHit.getCacheId(i)]->getGeoStore(oHit.getObjId(i));

            MaterialChunk *pMat = 
                dynamic_cast<MaterialChunk *>(
                    geoStore->getState()->getChunk(
                        MaterialChunk::getStaticClassId()));
            
            if(pMat != NULL)
            {
                Geometry *pGeo = geoStore->getGeo();

                if(pGeo != NULL)
                {
                    GeoVectorProperty *pNormals =
                        pGeo->getProperty(Geometry::NormalsIndex);
                    
                    GeoIntegralProperty *pNormalsIdx =
                        pGeo->getIndex(Geometry::NormalsIndex);
                    
                    if(pNormals != NULL && pNormalsIdx != NULL)
                    {
                        Vec3f n1;
                        Vec3f n2;
                        Vec3f n3;
                        
                        Real32 gamma = 1.f - (oHit.getU(i) + oHit.getV(i));
                        
                        UInt32 idx = 
                            pNormalsIdx->getValue(oHit.getTriId(i) * 3);

                        pNormals->getValue(n1, idx);
                        
                        idx = pNormalsIdx->getValue(oHit.getTriId(i) * 3 + 1);

                        pNormals->getValue(n2, idx);
                        
                        idx = pNormalsIdx->getValue(oHit.getTriId(i) * 3 + 2);
                        
                        pNormals->getValue(n3, idx);
                        
                        n1[0] = 
                            n2[0] * oHit.getU(i) + 
                            n3[0] * oHit.getV(i) +
                            n1[0] * gamma;
                        
                        n1[1] = 
                            n2[1] * oHit.getU(i) + 
                            n3[1] * oHit.getV(i) +
                            n1[1] * gamma;
                        
                        n1[2] = 
                            n2[2] * oHit.getU(i) + 
                            n3[2] * oHit.getV(i) +
                            n1[2] * gamma;
                        
                        n1.normalize();
                        
                        n2 = vRayDirs[i];
                        n2 *= -1.f;
                        
                        gamma = n1.dot(n2);
                        
                        if(gamma < 0.f)
                            gamma = 0.f;

                        oResult.set(i,
                                    pMat->getDiffuse()[0] * gamma,
                                    pMat->getDiffuse()[1] * gamma,
                                    pMat->getDiffuse()[2] * gamma,
                                    1.f);
                        
                    }
                }
                else
                {
                    oResult.set(i, pMat->getDiffuse());
                }
            }
            else
            {
                oResult.set(i, 0.8f, 0.8f, 0.8f, 1.0f);
            }
        }
        else
        {
            oResult.set(i,
                        _cBackground[0], 
                        _cBackground[1], 
                        _cBackground[2], 
                        _cBackground[3]);
        }
    }
}

template<typename DescT> inline
void RTScene<DescT>::shadeSDQO(RTHitSIMDPacket    &oHit,
                               BasicSIMDRayPacket &oRay,
                               RTColorSIMDPacket  &oResult)
{
    return;

    Vec3f vRayDirs[4];

    union
    {
        Float4 dir;
        Real32 dirConv[4];
    };

    for(UInt32 i = 0; i < 3; ++i)
    {
        dir = oRay.getQuad(i);

        vRayDirs[0][i] = dirConv[0];
        vRayDirs[1][i] = dirConv[1];
        vRayDirs[2][i] = dirConv[2];
        vRayDirs[3][i] = dirConv[3];
    }

    for(UInt32 i = 0; i < RTHitSIMDPacket::NumHits; ++i)
    {
        if(oHit.getU(i) > -0.5)
        {
            const typename RTCache::GeometryStorePtr geoStore = 
                _vRTCaches[oHit.getCacheId(i)]->getGeoStore(oHit.getObjId(i));

            MaterialChunk *pMat = 
                dynamic_cast<MaterialChunk *>(
                    geoStore->getState()->getChunk(
                        MaterialChunk::getStaticClassId()));
            
            if(pMat != NULL)
            {
                Geometry *pGeo = geoStore->getGeo();

                if(pGeo != NULL)
                {
                    GeoVectorProperty *pNormals =
                        pGeo->getProperty(Geometry::NormalsIndex);
                    
                    GeoIntegralProperty *pNormalsIdx =
                        pGeo->getIndex(Geometry::NormalsIndex);
                    
                    if(pNormals != NULL && pNormalsIdx != NULL)
                    {
                        Vec3f n1;
                        Vec3f n2;
                        Vec3f n3;
                        
                        Real32 gamma = 1.f - (oHit.getU(i) + oHit.getV(i));
                        
                        UInt32 idx = 
                            pNormalsIdx->getValue(oHit.getTriId(i) * 3);

                        pNormals->getValue(n1, idx);
                        
                        idx = pNormalsIdx->getValue(oHit.getTriId(i) * 3 + 1);

                        pNormals->getValue(n2, idx);
                        
                        idx = pNormalsIdx->getValue(oHit.getTriId(i) * 3 + 2);
                        
                        pNormals->getValue(n3, idx);
                        
                        n1[0] = 
                            n2[0] * oHit.getU(i) + 
                            n3[0] * oHit.getV(i) +
                            n1[0] * gamma;
                        
                        n1[1] = 
                            n2[1] * oHit.getU(i) + 
                            n3[1] * oHit.getV(i) +
                            n1[1] * gamma;
                        
                        n1[2] = 
                            n2[2] * oHit.getU(i) + 
                            n3[2] * oHit.getV(i) +
                            n1[2] * gamma;
                        
                        n1.normalize();
                        
                        n2 = vRayDirs[i];
                        n2 *= -1.f;
                        
                        gamma = n1.dot(n2);
                        
                        if(gamma < 0.f)
                            gamma = 0.f;

                        oResult.set(i,
                                    pMat->getDiffuse()[0] * gamma,
                                    pMat->getDiffuse()[1] * gamma,
                                    pMat->getDiffuse()[2] * gamma,
                                    1.f);
                        
                    }
                }
                else
                {
                    oResult.set(i, pMat->getDiffuse());
                }
            }
            else
            {
                oResult.set(i, 0.8f, 0.8f, 0.8f, 1.0f);
            }
        }
        else
        {
            oResult.set(i,
                        _cBackground[0], 
                        _cBackground[1], 
                        _cBackground[2], 
                        _cBackground[3]);
        }
    }
}

template<typename DescT> inline
void RTScene<DescT>::setBackgroundColor(Real32 rR,
                                        Real32 rG,
                                        Real32 rB,
                                        Real32 rA)
{
    _cBackground.setValuesRGBA(rR, rG, rB, rA);
}

OSG_END_NAMESPACE
