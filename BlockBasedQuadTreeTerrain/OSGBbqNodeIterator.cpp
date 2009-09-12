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

#include "OSGBbqNodeIterator.h"


OSG_BEGIN_NAMESPACE

BbqNodeIterator::BbqNodeIterator(void) :
    _iNodeCount         (0),
    _iProcessedNodeCount(0),
    _sProcessStack      ( )
{
}

BbqNodeIterator::~BbqNodeIterator(void)
{
}

//-----------------------------------------------------------------------------


void BbqNodeIterator::startIteration(Int32 iNodeCount)
{
    _iNodeCount          = iNodeCount;
    _iProcessedNodeCount = 0;
    
    pushNode(0);
}


//-----------------------------------------------------------------------------


bool BbqNodeIterator::isFinished(void) const
{
    return _sProcessStack.empty();
}


//-----------------------------------------------------------------------------


Real32 BbqNodeIterator::getProgress(void) const
{
    if(_iNodeCount <= 0)
    {
        return 1.0f;
    }

    return Real32(_iProcessedNodeCount) / Real32(_iNodeCount);
}


//-----------------------------------------------------------------------------


void BbqNodeIterator::step(void)
{
    BbqNodeId iCurrentNode = getCurrentNode();
    
    if(isCurrentNodeBottomUp() == true)
    {
        // simply pop the node
        popCurrentNode();
    }
    else
    {   
        _sProcessStack.back().second = true;
        
        // and push the child nodes on the stack:
        pushChildNodes(iCurrentNode);
    }   
}


//-----------------------------------------------------------------------------


BbqNodeId BbqNodeIterator::getCurrentNode(void) const
{
    return _sProcessStack.back().first;
}


//-----------------------------------------------------------------------------


bool BbqNodeIterator::isCurrentNodeBottomUp() const
{
    return _sProcessStack.back().second;
}


//-----------------------------------------------------------------------------


void BbqNodeIterator::popCurrentNode(void)
{
    _iProcessedNodeCount++;
    
    _sProcessStack.pop_back();
}


//-----------------------------------------------------------------------------


void BbqNodeIterator::pushNode(BbqNodeId iId)
{
    _sProcessStack.push_back(std::make_pair(iId, false));
}


//-----------------------------------------------------------------------------


void BbqNodeIterator::pushChildNodes(BbqNodeId iParentId)
{
    for(Int32 i = 0; i < 4; ++i)
    {
        BbqNodeId iChildId = 4 * iParentId + (3 - i) + 1;
        
        if(iChildId < _iNodeCount)
        {
            pushNode(iChildId);
        }
    }
}

OSG_END_NAMESPACE

