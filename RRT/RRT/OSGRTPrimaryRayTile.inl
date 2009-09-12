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
void RTPrimaryRayTile<DescT>::setXY(UInt32 uiX, 
                                    UInt32 uiY)
{
    _uiX = uiX;
    _uiY = uiY;
}

template<typename DescT> inline
UInt32 RTPrimaryRayTile<DescT>::getX(void)
{
    return _uiX;
}

template<typename DescT> inline
UInt32 RTPrimaryRayTile<DescT>::getY(void)
{
    return _uiY;
}

template<typename DescT> inline
void RTPrimaryRayTile<DescT>::setOrigin(UInt32 uiPacketIndex, Pnt3f vOrigin)
{
    OSG_ASSERT(uiPacketIndex < _vRayStore.size());

    _vRayStore[uiPacketIndex].setOrigin(vOrigin);
}

template<typename DescT> inline
void RTPrimaryRayTile<DescT>::setDirection(UInt32 uiPacketIndex, Vec3f vDir)
{
    OSG_ASSERT(uiPacketIndex < _vRayStore.size());

    _vRayStore[uiPacketIndex].setDirection(vDir);
}

template<typename DescT> inline
void RTPrimaryRayTile<DescT>::setActive(UInt32 uiPacketIndex, UInt16 uiVal)
{
    OSG_ASSERT(uiPacketIndex < _vActiveStore.size());

    _vActiveStore[uiPacketIndex] = uiVal;
}

template<typename DescT> inline
bool RTPrimaryRayTile<DescT>::hasActive(UInt32 uiPacketIndex)
{
    OSG_ASSERT(uiPacketIndex < _vActiveStore.size());

    return (_vActiveStore[uiPacketIndex] != 0);
}

template<typename DescT> inline
typename RTPrimaryRayTile<DescT>::ActiveStore &
    RTPrimaryRayTile<DescT>::getActive(void)
{
    return _vActiveStore;
}

template<typename DescT> inline
UInt16 *RTPrimaryRayTile<DescT>::getActiveRays(void)
{
    return &(_vActiveStore[0]);
}


template<typename DescT> inline
typename RTPrimaryRayTile<DescT>::RayPacket &
    RTPrimaryRayTile<DescT>::getPacket(UInt32 uiPacketIndex)
{
    OSG_ASSERT(uiPacketIndex < _vRayStore.size());

    return _vRayStore[uiPacketIndex];
}





template<typename DescT> inline
RTPrimaryRayTile<DescT>::RTPrimaryRayTile(void) :
     Inherited   ( ),
    _vRayStore   ( ),
    _vActiveStore( ),
    _uiX         (0),
    _uiY         (0)
{
    _vRayStore   .resize(NumRays);
    _vActiveStore.resize(NumRays, 0);
}

template<typename DescT> inline
RTPrimaryRayTile<DescT>::~RTPrimaryRayTile(void)
{
}

template<typename DescT> inline
RTPrimaryRayTile<DescT>::RTPrimaryRayTile(const RTPrimaryRayTile &source) :
     Inherited   (source              ),
    _vRayStore   (source._vRayStore   ),
    _vActiveStore(source._vActiveStore),
    _uiX         (source._uiX         ),
    _uiY         (source._uiY         )
{
}

template<typename DescT> inline
void RTPrimaryRayTile<DescT>::operator =(const RTPrimaryRayTile &source)
{
    _vRayStore    = source._vRayStore;
    _vActiveStore = source._vActiveStore;
    _uiX          = source._uiX;
    _uiY          = source._uiY;
}

template<typename DescT> inline
void RTPrimaryRayTile<DescT>::normalizeDirection(void)
{
    for(UInt32 i = 0; i < NumRays; ++i)
    {
        _vRayStore[i].normalizeDirection();
    }
}

OSG_END_NAMESPACE
