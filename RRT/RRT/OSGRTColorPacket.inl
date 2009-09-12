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
RTColorPacket::RTColorPacket(void) :
    _oColor(0.f, 0.f, 0.f, 0.f)
{
}

inline
RTColorPacket::~RTColorPacket(void)
{
}

inline
void RTColorPacket::set(Real32 rR, 
                        Real32 rG, 
                        Real32 rB,
                        Real32 rA)
{
    _oColor.setValuesRGBA(rR, rG, rB, rA);
}

inline
void RTColorPacket::set(Color4f oCol)
{
    _oColor = oCol;
}

inline
Color4f RTColorPacket::getColor(void)
{
    return _oColor;
}

inline 
void RTColorPacket::operator =(const RTColorPacket &source)
{
    _oColor = source._oColor;
}

OSG_END_NAMESPACE
