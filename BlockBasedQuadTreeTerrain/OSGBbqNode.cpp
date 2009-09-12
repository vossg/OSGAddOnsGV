/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2007 by the OpenSG Forum                   *
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
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Notes                                      *
 *                                                                           *
 * Implementation based on the original thesis work by Julien Koenen         *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

#include "OSGBbqNode.h"
#include "OSGTerrainHelper.h"

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

Int32 getFullQuadtreeNodeCount(Int32 iDepth )
{
    return 0x55555555 & ((1 << iDepth * 2) - 1);
}


//-----------------------------------------------------------------------------


Int32 getQuadtreeLeafNodeCount(Int32 iSamplesX, 
                               Int32 iSamplesY, 
                               Int32 iTileSize)
{
    const Int32 iLeafNodeCountX = 
        getNextPowerOf2(getCeil(Real32(iSamplesX) / Real32(iTileSize)));

    const Int32 iLeafNodeCountY = 
        getNextPowerOf2(getCeil(Real32(iSamplesY) / Real32(iTileSize)));
    
    // todo: output a warning, if leafNodeCountX and leafNodeCountY are to 
    // much apart.. this wastes a lot of memory right now..

    return osgMax(iLeafNodeCountX, iLeafNodeCountY);
}


//-----------------------------------------------------------------------------


Int32 getQuadtreeDepth(Int32 iSamplesX, Int32 iSamplesY, Int32 iTileSize)
{
    return Int32(log2(Real32(getQuadtreeLeafNodeCount(iSamplesX, 
                                                      iSamplesY, 
                                                      iTileSize))) + 1.f);
}


//-----------------------------------------------------------------------------


Int32 getQuadtreeChildIndex(BbqNodeId iId)
{
    // compute the child index: 
    // its always 
    // 01
    // 23

    const BbqNodeId iParentId = (iId - 1) / 4;

    const Int32 iFirstChildId = 4 * iParentId + 1;

    return iId - iFirstChildId;
}


//-----------------------------------------------------------------------------


bool isValidNodeId(BbqNodeId iId, Int32 iNodeCount)
{
    return iId >= 0 && iId < iNodeCount;       
}


//-----------------------------------------------------------------------------


Vec2i getParentTileOffset(Int32 iTileSize, Int32 iChildIndex)
{
    OSG_ASSERT(0 <= iChildIndex && iChildIndex < 4);
    
    const Int32 iHalfTileSize = iTileSize / 2;
    
    const Int32 iParentOffsetX[4] = { 0, iHalfTileSize, 0, iHalfTileSize };
    const Int32 iParentOffsetY[4] = { 0, 0, iHalfTileSize, iHalfTileSize };
    
    return Vec2i(iParentOffsetX[iChildIndex], 
                 iParentOffsetY[iChildIndex]);
}


//-----------------------------------------------------------------------------

OSG_END_NAMESPACE

