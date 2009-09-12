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
     Inherited(),
    _vRTCaches()
{
}

template<typename DescT> inline
RTScene<DescT>::~RTScene(void)
{
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
void RTScene<DescT>::tracePrimaryRays(BasicRayPacket &oRay, 
                                      HitPacket      &oHit,
                                      ElemStack      &sKDToDoStack,
                                      UInt32         *uiActive    )
{
    for(UInt32 k = 0; k < _vRTCaches.size(); ++k)
    {    
        _vRTCaches[k]->intersect(oRay, oHit, sKDToDoStack, k);
    }
}

template<typename DescT> inline
void RTScene<DescT>::tracePrimaryRays(BasicSIMDRayPacket &oRay, 
                                      HitPacket          &oHit,
                                      ElemStack          &sKDToDoStack,
                                      UInt32             *uiActive    )
{
    for(UInt32 k = 0; k < _vRTCaches.size(); ++k)
    {    
        _vRTCaches[k]->intersect(oRay, oHit, sKDToDoStack, k, uiActive);
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

        MaterialChunkPtr pMat = 
            dynamic_cast<MaterialChunkPtr>(
                geoStore->getState()->getChunk(
                    MaterialChunk::getStaticClassId()));

        if(pMat != NULL)
        {
            GeometryPtr pGeo = geoStore->getGeo();

            if(pGeo != NULL)
            {
                GeoVectorPropertyPtr pNormals =
                    pGeo->getProperty(Geometry::NormalsIndex);

                GeoIntegralPropertyPtr pNormalsIdx =
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
        oResult.set(0.4f, 0.4f, 0.4f, 1.0f);
    }
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
        dir = oRay.getDir(i);

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

            MaterialChunkPtr pMat = 
                dynamic_cast<MaterialChunkPtr>(
                    geoStore->getState()->getChunk(
                        MaterialChunk::getStaticClassId()));
            
            if(pMat != NULL)
            {
                GeometryPtr pGeo = geoStore->getGeo();

                if(pGeo != NULL)
                {
                    GeoVectorPropertyPtr pNormals =
                        pGeo->getProperty(Geometry::NormalsIndex);
                    
                    GeoIntegralPropertyPtr pNormalsIdx =
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
            oResult.set(i, 0.3f, 0.3f, 0.3f, 1.0f);
        }
    }
}

OSG_END_NAMESPACE
