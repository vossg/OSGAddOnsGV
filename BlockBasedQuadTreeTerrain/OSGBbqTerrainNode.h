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

//-----------------------------------------------------------------------------

#ifndef _OSGBBQTERRAINNODE_H_
#define _OSGBBQTERRAINNODE_H_

#include "OSGTerrainHelper.h"
#include "OSGBbqNode.h"

#include "OSGRectangle.h"
#include "OSGBoxVolume.h"

#include <vector>

OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------

typedef Real32 BbqPriority;

//-----------------------------------------------------------------------------

const BbqPriority BbqHightestPriority = 100.0f;

//-----------------------------------------------------------------------------

typedef std::vector<UInt8 >  BbqTextureDataContainer;
typedef std::vector<UInt16>  BbqHeightDataContainer;
typedef std::vector<Int16 >  BbqHeightDeltaDataContainer;

//-----------------------------------------------------------------------------

// todo: (performance): store the data in compressed format and uncompress
// on the fly while uploading to the gpu

struct BbqTerrainNodeData
{
    BbqHeightDataContainer  heightData;
    BbqTextureDataContainer textureData;
};

//-----------------------------------------------------------------------------

struct BbqTerrainNode
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainNode(void);
    ~BbqTerrainNode(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool isLeafNode   (void) const;
    bool isRootNode   (void) const;
    bool isPreLeafNode(void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */
    
            BbqNodeId           id;

            // this gets only computed for the leaf nodes (loading/unloading)
            BbqPriority         priority;           
    
            // sample rect:
            Rectangle2i         sampleRect;
    
            // todo: do i really need these??
            Vec2f               blockOrigin;
            Real32              blockScale;
    
            Int32               maxHeightError;
            Real32              geoMorphingFactor;

            // object space bounding box:
            BoxVolume           boundingBox;
    

            // the data:
            BbqTerrainNodeData  data;
    

            // pointer to the 4 children and the parent:
            Int32               treeLevel;
    
            BbqTerrainNode     *parent;
            BbqTerrainNode     *children   [BbqChild_Count          ];
     
    mutable void               *renderCache[BbqRenderCacheType_Count];

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:
};

OSG_END_NAMESPACE

//-----------------------------------------------------------------------------

#endif // _OSGBBQTERRAIN_H_
