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

template<class DescT> inline
RTFourHitSIMDPacket<DescT>::RTFourHitSIMDPacket(void) :
     Inherited (       ),
    _uiX       (0      ),
    _uiY       (0      ),
    _pRayPacket(NULL   )
{
}

template<class DescT> inline
RTFourHitSIMDPacket<DescT>::RTFourHitSIMDPacket(
    const RTFourHitSIMDPacket &source) :

     Inherited (source            ),
    _uiX       (source._uiX       ),
    _uiY       (source._uiY       ),
    _pRayPacket(source._pRayPacket)
{
}

template<class DescT> inline
RTFourHitSIMDPacket<DescT>::~RTFourHitSIMDPacket(void)
{
}

template<class DescT> inline
void RTFourHitSIMDPacket<DescT>::reset(void)
{
    Inherited::reset();

    _uiX        = 0;
    _uiY        = 0;
    _pRayPacket = NULL;
}


template<class DescT> inline 
void RTFourHitSIMDPacket<DescT>::setXY(UInt32 uiX,
                                       UInt32 uiY)
{
    _uiX = uiX;
    _uiY = uiY;
}

template<class DescT> inline 
UInt32 RTFourHitSIMDPacket<DescT>::getX(void)
{
    return _uiX;
}

template<class DescT> inline 
UInt32 RTFourHitSIMDPacket<DescT>::getY(void)
{
    return _uiY;
}

template<class DescT> inline 
void RTFourHitSIMDPacket<DescT>::setRayPacket(RayPacket *pRayPacket)
{
    _pRayPacket = pRayPacket;
}

template<class DescT> inline 
typename RTFourHitSIMDPacket<DescT>::RayPacket *
    RTFourHitSIMDPacket<DescT>::getRayPacket(void)
{
    return _pRayPacket;
}

template<class DescT> inline 
void RTFourHitSIMDPacket<DescT>::operator =(const RTFourHitSIMDPacket &source)
{
    (*static_cast<Inherited *>(this)) = 
        static_cast<const Inherited &>(source);

    _uiX        = source._uiX;
    _uiY        = source._uiY;
    _pRayPacket = source._pRayPacket;
}

OSG_END_NAMESPACE
