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
RTFourRaySIMDPacketInfo::RTFourRaySIMDPacketInfo(void) :
     Inherited( ),
    _uiX      (0),
    _uiY      (0)
{
}

RTFourRaySIMDPacketInfo::RTFourRaySIMDPacketInfo(
    const RTFourRaySIMDPacketInfo &source) :
     Inherited(source     ),
    _uiX      (source._uiX),
    _uiY      (source._uiY)
{
}

inline 
RTFourRaySIMDPacketInfo::~RTFourRaySIMDPacketInfo(void)
{
}

inline 
void RTFourRaySIMDPacketInfo::setXY(UInt32 uiX,
                                    UInt32 uiY)
{
    _uiX = uiX;
    _uiY = uiY;
}

inline 
UInt32 RTFourRaySIMDPacketInfo::getX(void)
{
    return _uiX;
}

inline 
UInt32 RTFourRaySIMDPacketInfo::getY(void)
{
    return _uiY;
}


inline 
void RTFourRaySIMDPacketInfo::operator =(const RTFourRaySIMDPacketInfo &source)
{
    Inherited::operator =(source);

    _uiX = source._uiX;
    _uiY = source._uiY;
}


inline
void RTFourRaySIMDPacketInfo::dump(void)
{
    fprintf(stderr, "%d %d\n", _uiX, _uiY);
    fprintf(stderr, "%d %d %d %d\n", 
            _bIsActive[0],
            _bIsActive[1],
            _bIsActive[2],
            _bIsActive[3]);
}





inline
RTFourRaySIMDPacket::RTFourRaySIMDPacket(void) :
    Inherited()
{
}

RTFourRaySIMDPacket::RTFourRaySIMDPacket(const RTFourRaySIMDPacket &source) :
    Inherited()
{
}

inline
RTFourRaySIMDPacket::~RTFourRaySIMDPacket(void)
{
}

inline 
void RTFourRaySIMDPacket::operator =(const RTFourRaySIMDPacket &source)
{
    Inherited::operator =(source);
}

inline
void RTFourRaySIMDPacket::dump(void)
{
    fprintf(stderr, "%f %f %f ->\n",
            _vOriginA[0],
            _vOriginA[1],
            _vOriginA[2]);

    fprintf(stderr, "    %f %f %f\n",
            _vDirA[0][0],
            _vDirA[1][0],
            _vDirA[2][0]);
    fprintf(stderr, "    %f %f %f\n",
            _vDirA[0][1],
            _vDirA[1][1],
            _vDirA[2][1]);
    fprintf(stderr, "    %f %f %f\n",
            _vDirA[0][2],
            _vDirA[1][2],
            _vDirA[2][2]);
    fprintf(stderr, "    %f %f %f\n",
            _vDirA[0][3],
            _vDirA[1][3],
            _vDirA[2][3]);
}

OSG_END_NAMESPACE
