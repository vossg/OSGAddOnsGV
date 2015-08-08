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

//-----------------------------------------------------------------------------

// todo: (performance): store the data in compressed format and uncompress
// on the fly while uploading to the gpu

//-----------------------------------------------------------------------------

struct BbqTerrainNodeBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainNodeBase(void);
    BbqTerrainNodeBase(const BbqTerrainNodeBase &other);
    virtual ~BbqTerrainNodeBase(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

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
    
    Real32              geoMorphingFactor;

    // object space bounding box:
    BoxVolume           boundingBox;
    

    // pointer to the 4 children and the parent:
    Int32               treeLevel;
         
    mutable void               *renderCache[BbqRenderCacheType_Count];

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:

    void operator =(const BbqTerrainNodeBase &rhs); 
};


template<class HeightType, class HeightDeltaType, class TextureType>
struct BbqTerrainNode : public BbqTerrainNodeBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainNode(void);
    BbqTerrainNode(const BbqTerrainNode &other);

    virtual ~BbqTerrainNode(void);

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

    typedef std::vector<TextureType    >  BbqTextureDataContainer;     // UInt8

    typedef std::vector<HeightType     >  BbqHeightDataContainer;      // UInt16
    typedef std::vector<HeightDeltaType>  BbqHeightDeltaDataContainer; // Int16
 
    struct BbqTerrainNodeData
    {
        BbqHeightDataContainer  heightData;
        BbqTextureDataContainer textureData;

        BbqTerrainNodeData(void) : heightData(), textureData() {}
    };
   
    HeightDeltaType     maxHeightError; // Int16

    // the data:
    BbqTerrainNodeData  data;

    BbqTerrainNode     *parent;
    BbqTerrainNode     *children   [BbqChild_Count          ];

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:

    void operator =(const BbqTerrainNode &rhs); 
};

OSG_END_NAMESPACE

#include "OSGBbqTerrainNode.inl"

#endif // _OSGBBQTERRAIN_H_
