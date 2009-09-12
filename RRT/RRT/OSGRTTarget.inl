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
void RTTarget::setPixel(UInt32 uiX, UInt32 uiY, Color4f &oColor)
{
    OSG_ASSERT(uiX < _sfWidth.getValue() && uiY < _sfHeight.getValue());

    UInt32 uiIdx = (uiY * _sfWidth.getValue() + uiX) * 3;

    _mfPixel[uiIdx    ] = oColor[0];
    _mfPixel[uiIdx + 1] = oColor[1];
    _mfPixel[uiIdx + 2] = oColor[2];
}

OSG_END_NAMESPACE
