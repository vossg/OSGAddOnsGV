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

#ifndef _OSGBBQNODE_H_
#define _OSGBBQNODE_H_

#include "OSGVector.h"


OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------

typedef Int32 BbqNodeId;      

//-----------------------------------------------------------------------------

static const BbqNodeId InvalidBbqNodeId = -1;
static const BbqNodeId RootBbqNodeId    = 0;

//-----------------------------------------------------------------------------

enum BbqChildIndex
{
    BbqChild_TopLeft     = 0x0000,
    BbqChild_TopRight    = 0x0001,
    BbqChild_BottemLeft  = 0x0002,
    BbqChild_BottemRight = 0x0003,
    BbqChild_Count       = 0x0004
};

//-----------------------------------------------------------------------------

// Return the number of nodes in a fully populated quadtree of the 
// specified depth.

Int32 getFullQuadtreeNodeCount(Int32     iDepth     );

Int32 getQuadtreeDepth        (Int32     iSamplesX, 
                               Int32     iSamplesY, 
                               Int32     iTileSize  );

Int32 getQuadtreeLeafNodeCount(Int32     iSamplesX, 
                               Int32     iSamplesY, 
                               Int32     iTileSize  );

Int32 getQuadtreeChildIndex   (BbqNodeId iId        );

bool  isValidNodeId           (BbqNodeId iId, 
                               Int32     iNodeCount );

Vec2i getParentTileOffset     (Int32     iTileSize, 
                               Int32     iChildIndex);

OSG_END_NAMESPACE

#endif // _OSGBBQNODE_H_


