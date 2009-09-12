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

#include "OSGBbqTerrainNode.h"

OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------

BbqTerrainNodeBase::BbqTerrainNodeBase(void):
    id               (0         ),
    priority         (0.f       ),
    sampleRect       (0, 0, 0, 0),
    blockOrigin      (0.f, 0.f  ),
    blockScale       (0.f       ),
    maxHeightError   (0         ),
    geoMorphingFactor(0.5f      ),
    data             (          ),
    treeLevel        (0         ),
    parent           (NULL      )
{
    renderCache[0] = renderCache[1] = NULL;

    children[0] = 
        children[1] = 
        children[2] = 
        children[3] = NULL;
}

BbqTerrainNodeBase::~BbqTerrainNodeBase(void)
{
}


//-----------------------------------------------------------------------------


bool BbqTerrainNodeBase::isLeafNode(void) const
{
    if(children[0] == NULL)
    {
        assert(children[1] == NULL);
        assert(children[2] == NULL);
        assert(children[3] == NULL);

        return true;
    }
    else
    {
        assert(children[1] != NULL);
        assert(children[2] != NULL);
        assert(children[3] != NULL);

        return false;
    }
}

//-----------------------------------------------------------------------------


bool BbqTerrainNodeBase::isRootNode(void) const
{
    return parent == NULL;
}


//-----------------------------------------------------------------------------


bool BbqTerrainNodeBase::isPreLeafNode(void) const
{
    return !isLeafNode() && 
        children[0]->isLeafNode() && 
        children[1]->isLeafNode() && 
        children[2]->isLeafNode() && 
        children[3]->isLeafNode();
}

OSG_END_NAMESPACE
