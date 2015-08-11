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

OSG_BEGIN_NAMESPACE

template<class HeightType, class HeightDeltaType, class TextureType> inline
BbqTerrainNode<HeightType, 
               HeightDeltaType,
               TextureType    >::BbqTerrainNode(void) :
                   BbqTerrainNodeBase(          ),
                   maxHeightError    (0         ),
                   data              (          ),
                   parent            (NULL      )
{
    children[0] = 
        children[1] = 
        children[2] = 
        children[3] = NULL;
}

template<class HeightType, class HeightDeltaType, class TextureType> inline
BbqTerrainNode<HeightType, 
               HeightDeltaType,
               TextureType    >::BbqTerrainNode(const BbqTerrainNode &other) :
                   BbqTerrainNodeBase(other               ),
                   maxHeightError    (other.maxHeightError),
                   data              (other.data          ),
                   parent            (other.parent        )
{
    children[0] = other.children[0];
    children[1] = other.children[1];
    children[2] = other.children[2];
    children[3] = other.children[3];
}


template<class HeightType, class HeightDeltaType, class TextureType> inline
BbqTerrainNode<HeightType, 
               HeightDeltaType,
               TextureType    >::~BbqTerrainNode(void)
{
}

#ifdef WIN32
template<class HeightType, class HeightDeltaType, class TextureType> inline
const BbqTerrainNode<HeightType, 
                     HeightDeltaType,
                     TextureType    > &
    BbqTerrainNode::operator = (const BbqTerrainNode &other)
{

    maxHeightError = other.maxHeightError;
    data           = other.data;
    parent         = other.parent;

    children[0]    = other.children[0];
    children[1]    = other.children[1];
    children[2]    = other.children[2];
    children[3]    = other.children[3];

    return this;
}
#endif

//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType> inline
bool BbqTerrainNode<HeightType, 
                    HeightDeltaType,
                    TextureType    >::isLeafNode(void) const
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


template<class HeightType, class HeightDeltaType, class TextureType> inline
bool BbqTerrainNode<HeightType, 
                    HeightDeltaType,
                    TextureType    >::isRootNode(void) const
{
    return parent == NULL;
}


//-----------------------------------------------------------------------------


template<class HeightType, class HeightDeltaType, class TextureType> inline
bool BbqTerrainNode<HeightType, 
                    HeightDeltaType,
                    TextureType    >::isPreLeafNode(void) const
{
    return !isLeafNode() && 
        children[0]->isLeafNode() && 
        children[1]->isLeafNode() && 
        children[2]->isLeafNode() && 
        children[3]->isLeafNode();
}

OSG_END_NAMESPACE
