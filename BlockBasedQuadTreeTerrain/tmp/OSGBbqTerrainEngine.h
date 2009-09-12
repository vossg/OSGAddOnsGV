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

#ifndef _OSGBBQTERRAINENGINE_H_
#define _OSGBBQTERRAINENGINE_H_

//-----------------------------------------------------------------------------

#include "OSGTerrainHelper.h"
#include "OSGBbqNode.h"
#include "OSGBbqTerrainRenderer.h"

#include "OSGVector.h"
#include "OSGLock.h"

#include <vector>

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------

// todo: put this structures into another file...   
struct BbqDataStatistics
{
    BbqDataStatistics(void);
    
    Int32  loadedNodeCount;
    Int32  loadedNodeBuget;
    Int32  loadQueueSize;
    Int32  unloadQueueSize;
    Int32  cacheMisses;
    Int32  cacheHits;
    UInt64 bytesLoaded;
};

//-----------------------------------------------------------------------------

struct BbqRenderCacheEntry;

class BbqDataSourceEngine;

//-----------------------------------------------------------------------------

class BbqTerrainEngine
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainEngine(void);
    ~BbqTerrainEngine(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool initialize(BbqDataSourceEngine *pDataSource, 
                    Int32                iMaxResidentNodeCount);
    void shutdown  (void                                      );
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // todo: add the expected future position in here..
    void update(const Vec3f &vViewerPosition);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // render does not change anything at all. it simply renders
    // the existing tree recursively (and does some nice geomorphing.../
    // the update call did the frustum culling already)     

    void render(const BbqRenderOptions &oOptions);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // call this from a different thread as the update/render calls: it 
    // tries to load the highest priority nodedata element.
    // and (if needed) unloads an unused element
    // returns true, if some data was loaded, false if nothing is waiting 
    // to get loaded

    bool prefetchNodeData(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // for debugging the shader (instant reload)
    void reloadShader(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    const BbqDataStatistics   &getDataStatistics  (void) const;
    const BbqRenderStatistics &getRenderStatistics(void) const;
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:

    typedef FixedAllocator< BbqTerrainNode  > BbqTerrainNodeAllocator;

    typedef std::vector<BbqTerrainNode     *> BbqPriorityQueue;
    
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    BbqDataSourceEngine           *_pDataSource;
    BbqDataSourceInformation       _oDataSourceInfo;
    
    //BbqNodeData                   loadBuffer_;
    
    // the renderer is in another class:
    BbqOpenGLTerrainRenderer       _oRenderer;
    
    // the node allocator:
    BbqTerrainNodeAllocator        _oNodeAllocator;
    BbqTerrainNode                *_pRootNode;
    std::vector<BbqTerrainNode *>  _vTraversalStack;
    
    // the load queue contains the leaf nodes of the current quadtree with
    // the priorities. a load means that the child nodes of 
    // a node will be loaded and inserted into the tree

    BbqPriorityQueue               _qLoadQueue;
    
    // the unload queue contains the pre-leaf nodes (nodes that have 4 leaf
    // child nodes). an unload operation unloads all 4
    // child nodes making the node in here a leaf node.
    // note that the root node cant ever be unloaded with this mechanism 
    // (that is a good thing..)

    BbqPriorityQueue               _qUnloadQueue;
    
    //motor3d::Mutex                  treeAccessMutex_;
    Lock                          *_pTreeAccessMutex;

    // some statistics:
    BbqDataStatistics              _oDataStatistics;
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void requestLoad      (      BbqTerrainNode *pNode, 
                           const Vec3f          &vViewerPosition);

    void requestUnload    (      BbqTerrainNode *pNode, 
                           const Vec3f          &vViewerPosition);
    
    void calculatePriority(      BbqTerrainNode *pNode, 
                           const Vec3f          &vViewerPosition);
    
    void initializeNode   (      BbqTerrainNode *pNode, 
                                 BbqNodeId       id, 
                                 Int32           x0, 
                                 Int32           y0, 
                                 Int32           x1, 
                                 Int32           y1             );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool allocateChildren(BbqTerrainNode *pNode);
    void freeChildren    (BbqTerrainNode *pNode);
    bool loadChildren    (BbqTerrainNode *pNode);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void checkTreeConsistency(BbqTerrainNode *node);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    static bool loadQueueComp  (const BbqTerrainNode *lhs, 
                                const BbqTerrainNode *rhs);
    static bool unloadQueueComp(const BbqTerrainNode *lhs, 
                                const BbqTerrainNode *rhs);
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
};

OSG_END_NAMESPACE

#endif // #ifdef _OSGBBQTERRAINENGINE_H_
