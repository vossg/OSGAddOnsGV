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
void RTScene<DescT>::tracePrimaryRays(RayPacket &oRay, 
                                      HitPacket &oHit,
                                      ElemStack &sKDToDoStack)
{
    for(UInt32 k = 0; k < _vRTCaches.size(); ++k)
    {    
        oHit.setCacheId(k);

        _vRTCaches[k]->intersect(oRay, oHit, sKDToDoStack);
//                _vRTObjects[k]->intersectBarycentric0(rayPacket, hitPacket);
    }
}

template<typename DescT> inline
void RTScene<DescT>::shade(HitPacket &oHit,
                           RayPacket &oRay,
                           Color4f   &oResult)
{

    if(oHit.getU() > -0.5)
    {
        const typename RTCache::GeometryStore &geoStore = 
            _vRTCaches[oHit.getCacheId()]->getGeoStore(oHit.getObjId());

        MaterialChunkPtr pMat = 
            dynamic_cast<MaterialChunkPtr>(
                geoStore._pState->getChunk(MaterialChunk::getStaticClassId()));

        if(pMat != NULL)
        {
            oResult = pMat->getDiffuse();
        }
        else
        {
            oResult.setValuesRGBA(0.8f, 0.8f, 0.8f, 1.0f);
        }
    }
    else
    {
        oResult.setValuesRGBA(0.3f, 0.3f, 0.3f, 1.0f);
    }
}

OSG_END_NAMESPACE
