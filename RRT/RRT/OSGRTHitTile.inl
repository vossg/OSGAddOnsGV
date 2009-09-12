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
void RTHitTile<DescT>::setXY(UInt32 uiX, 
                             UInt32 uiY)
{
    _uiX = uiX;
    _uiY = uiY;
}


template<typename DescT> inline
UInt32 RTHitTile<DescT>::getX(void)
{
    return _uiX;
}

template<typename DescT> inline
UInt32 RTHitTile<DescT>::getY(void)
{
    return _uiY;
}

template<typename DescT> inline
typename RTHitTile<DescT>::HitPacket &
    RTHitTile<DescT>::getPacket(UInt32 uiPacketIndex)
{
    OSG_ASSERT(uiPacketIndex < _vHitStore.size());

    return _vHitStore[uiPacketIndex];
}

template<typename DescT> inline
void RTHitTile<DescT>::setRayTile(PrimaryRayTile *pRayTile)
{
    _pRayTile = pRayTile;
}

template<typename DescT> inline
typename RTHitTile<DescT>::RayPacket &
    RTHitTile<DescT>::getRayPacket(UInt32 uiPacketIndex)
{
    return _pRayTile->getPacket(uiPacketIndex);
}

template<typename DescT> inline
void RTHitTile<DescT>::reset(void)
{
    _uiX      = 0;
    _uiY      = 0;
    _pRayTile = NULL;

    for(UInt32 i = 0; i < NumHits; ++i)
    {
        _vHitStore[i].reset();
    }
}

template<typename DescT> inline
RTHitTile<DescT>::RTHitTile(void) :
     Inherited(    ),
    _vHitStore(    ),
    _uiX      (   0),
    _uiY      (   0),
    _pRayTile (NULL)
{
    _vHitStore.resize(NumHits);
}

template<typename DescT> inline
RTHitTile<DescT>::RTHitTile(const RTHitTile &source) :
     Inherited(source           ),
    _vHitStore(source._vHitStore),
    _uiX      (source._uiX      ),
    _uiY      (source._uiY      ),
    _pRayTile (source._pRayTile )
{
}

template<typename DescT> inline
RTHitTile<DescT>::~RTHitTile(void)
{
}

template<typename DescT> inline
void RTHitTile<DescT>::operator =(const RTHitTile &source)
{
    _vHitStore = source._vHitStore;
    _uiX       = source._uiX;
    _uiY       = source._uiY;
    _pRayTile  = source._pRayTile;
}

OSG_END_NAMESPACE
