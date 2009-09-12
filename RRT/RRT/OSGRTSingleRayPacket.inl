/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

inline
RTSingleRayPacketInfo::RTSingleRayPacketInfo(void) :
     Inherited( ),
    _uiX      (0),
    _uiY      (0)
{
}


inline
RTSingleRayPacketInfo::RTSingleRayPacketInfo(
    const RTSingleRayPacketInfo &source) :

     Inherited(source     ),
    _uiX      (source._uiX),
    _uiY      (source._uiY)
{
}

inline
RTSingleRayPacketInfo::~RTSingleRayPacketInfo(void)
{
}

inline 
void RTSingleRayPacketInfo::setXY(UInt32 uiX,
                                  UInt32 uiY)
{
    _uiX = uiX;
    _uiY = uiY;
}

inline 
UInt32 RTSingleRayPacketInfo::getX(void)
{
    return _uiX;
}

inline 
UInt32 RTSingleRayPacketInfo::getY(void)
{
    return _uiY;
}

inline
void RTSingleRayPacketInfo::operator =(const RTSingleRayPacketInfo &source)
{
    (*static_cast<Inherited *>(this)) = 
        static_cast<const Inherited &>(source);

    _uiX       = source._uiX;
    _uiY       = source._uiY;
}

inline
void RTSingleRayPacketInfo::dump(void)
{
    fprintf(stderr, "%d %d %d\n", _uiX, _uiY, _bIsActive);
}




template<class DescT> inline
RTCacheRefRayPacketMixin<DescT>::RTCacheRefRayPacketMixin(void) : 
     Inherited (    ),
    _pCacheNode(NULL)
{
}

template<class DescT> inline
RTCacheRefRayPacketMixin<DescT>::RTCacheRefRayPacketMixin(
    const RTCacheRefRayPacketMixin &source) :

     Inherited (source            ),
    _pCacheNode(source._pCacheNode)
{
}

template<class DescT> inline
RTCacheRefRayPacketMixin<DescT>::~RTCacheRefRayPacketMixin(void)
{
}

template<class DescT> inline
void RTCacheRefRayPacketMixin<DescT>::setCacheNode(const CacheNode *pNode)
{
    _pCacheNode = pNode;
}

template<class DescT> inline
typename RTCacheRefRayPacketMixin<DescT>::CacheNode *
    RTCacheRefRayPacketMixin<DescT>::getCacheNode(void)
{
    return _pCacheNode;
}

template<class DescT> inline
void RTCacheRefRayPacketMixin<DescT>::operator =(
    const RTCacheRefRayPacketMixin &source)
{
    Inherited::operator =(source);

    _pCacheNode = source._pCacheNode;
}






template<class ParentT> inline
RTSingleRayPacket<ParentT>::RTSingleRayPacket(void) :
    Inherited()
{
}

template<class ParentT> inline
RTSingleRayPacket<ParentT>::RTSingleRayPacket(const RTSingleRayPacket &source) :
    Inherited(source)
{
}

template<class ParentT> inline
RTSingleRayPacket<ParentT>::~RTSingleRayPacket(void)
{
}

template<class ParentT> inline 
void RTSingleRayPacket<ParentT>::operator =(const RTSingleRayPacket &source)
{
    (*static_cast<Inherited *>(this)) = 
        static_cast<const Inherited &>(source);
}

template<class ParentT> inline
void RTSingleRayPacket<ParentT>::dump(void)
{
    fprintf(stderr, "%f %f %f -> %f %f %f\n",
            this->_vOrigin[0],
            this->_vOrigin[1],
            this->_vOrigin[2],
            this->_vDir[0],
            this->_vDir[1],
            this->_vDir[2]);
}

OSG_END_NAMESPACE
