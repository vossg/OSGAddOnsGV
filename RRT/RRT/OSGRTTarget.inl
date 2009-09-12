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
UInt32 RTTarget::getWidth (void) const
{
    return Inherited::getWidth();
}

inline
UInt32 RTTarget::getHeight(void) const
{
    return Inherited::getHeight();
}

inline
void RTTarget::markPixelHit(UInt32 uiX, UInt32 uiY)
{
    UInt32 uiIdx = (uiY * _sfWidth.getValue() + uiX) * 3;

    _mfPixel[uiIdx    ] = 0.8f;
    _mfPixel[uiIdx + 1] = 0.8f;
    _mfPixel[uiIdx + 2] = 0.8f;
}

inline
void RTTarget::markPixelNotHit(UInt32 uiX, UInt32 uiY)
{
    UInt32 uiIdx = (uiY * _sfWidth.getValue() + uiX) * 3;

    _mfPixel[uiIdx    ] = 0.3f;
    _mfPixel[uiIdx + 1] = 0.8f;
    _mfPixel[uiIdx + 2] = 0.3f;

}

inline
void RTTarget::setPixel(UInt32 uiX, UInt32 uiY, RTColorPacket &oColor)
{
    OSG_ASSERT(uiX < _sfWidth.getValue() && uiY < _sfHeight.getValue());

    UInt32 uiIdx = (uiY * _sfWidth.getValue() + uiX) * 3;

    _mfPixel[uiIdx    ] = oColor.getColor()[0];
    _mfPixel[uiIdx + 1] = oColor.getColor()[1];
    _mfPixel[uiIdx + 2] = oColor.getColor()[2];
}

inline
void RTTarget::setPixel(UInt32 uiX, UInt32 uiY, RTColorSIMDPacket &oColor)
{
    UInt32 uiAbsX = uiX * RTColorSIMDPacket::NumHColors;
    UInt32 uiAbsY = uiY * RTColorSIMDPacket::NumVColors;

    for(UInt32 i = 0; i < RTColorSIMDPacket::NumVColors; ++i)
    {
        for(UInt32 j = 0; j < RTColorSIMDPacket::NumHColors; ++j)
        {
            UInt32 uiPacketIndex = i * RTColorSIMDPacket::NumHColors + j;

            UInt32 uiPixelX = uiAbsX + j;
            UInt32 uiPixelY = uiAbsY + i;

            UInt32 uiIdx = (uiPixelY * _sfWidth.getValue() + uiPixelX) * 3;

            _mfPixel[uiIdx    ] = oColor.getColor(uiPacketIndex)[0];
            _mfPixel[uiIdx + 1] = oColor.getColor(uiPacketIndex)[1];
            _mfPixel[uiIdx + 2] = oColor.getColor(uiPacketIndex)[2];
        }
    }
}

OSG_END_NAMESPACE
