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

#include "OSGRTStore.h"

OSG_BEGIN_NAMESPACE

RTStore::RTStore(void) :
    Inherited ( ),
   _uiWidth   (0),
   _uiHeight  (0),
   _uiNumTiles(0),
   _uiHTiles  (0),
   _uiVTiles  (0)
{
}

RTStore::~RTStore(void)
{
}

RTStore::RTStore(const RTStore &source)
{
}

void RTStore::updateNumTiles(UInt32 uiTargetWidth, 
                             UInt32 uiTargetHeight,
                             UInt32 uiTileWdith,
                             UInt32 uiTileHeight )
{
    fprintf(stderr, "TS : %d %d\n", 
            uiTargetWidth, uiTargetHeight);
    
    _uiHTiles = uiTargetWidth  / uiTileWdith;
    _uiVTiles = uiTargetHeight / uiTileHeight;
    
    UInt32 uiPartialH = uiTargetWidth  % uiTileWdith;
    UInt32 uiPartialV = uiTargetHeight % uiTileHeight;
    
    fprintf(stderr, "TS : %d %d | %d %d\n", 
            _uiHTiles, 
            _uiVTiles,
            uiPartialH,
            uiPartialV);
    
    if(uiPartialH != 0)
    {
        ++_uiHTiles;
    }
    
    if(uiPartialV != 0)
    {
        ++_uiVTiles;
    }
    
    _uiNumTiles = _uiHTiles * _uiVTiles;
    
    fprintf(stderr, "TS :  %d %d | %d %d | %d\n", 
            _uiHTiles, 
            _uiVTiles,
            uiPartialH,
            uiPartialV,
            _uiNumTiles);
    
    _uiWidth  = uiTargetWidth;
    _uiHeight = uiTargetHeight;
}

OSG_END_NAMESPACE
