/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

#ifndef _OSGBBQTERRAIN_H_
#define _OSGBBQTERRAIN_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGNode.h"
#include "OSGBbqTerrainBase.h"
#include "OSGAction.h"
#include "OSGBbqDataSource.h"

#include "OSGTerrainHelper.h"
#include "OSGBbqNode.h"
#include "OSGBbqTerrainRenderer.h"

#include "OSGVector.h"
#include "OSGLock.h"

#include <vector>

OSG_BEGIN_NAMESPACE

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

struct BbqRenderCacheEntry;

class BbqDataSource;

class BbqTerrainEngineBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainEngineBase(void);
    virtual ~BbqTerrainEngineBase(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual void shutdown(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // todo: add the expected future position in here..
    virtual void update(const Vec3f &vViewerPosition) = 0;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // render does not change anything at all. it simply renders
    // the existing tree recursively (and does some nice geomorphing.../
    // the update call did the frustum culling already)     

    virtual void render(const BbqRenderOptions &oOptions) = 0;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // call this from a different thread as the update/render calls: it 
    // tries to load the highest priority nodedata element.
    // and (if needed) unloads an unused element
    // returns true, if some data was loaded, false if nothing is waiting 
    // to get loaded

    virtual bool prefetchNodeData(void) = 0;
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

            const BbqDataStatistics   &getDataStatistics  (void) const;
    virtual const BbqRenderStatistics &getRenderStatistics(void) const = 0;
  
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    typedef BbqTerrainNodeBase BbqBaseNode;

    typedef std::vector<BbqBaseNode     *> BbqPriorityQueue;

    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    BbqDataSource                     *_pDataSource;
    BbqDataSourceInformation           _oDataSourceInfo;
    
    //BbqNodeData                   loadBuffer_;
    
    // the load queue contains the leaf nodes of the current quadtree with
    // the priorities. a load means that the child nodes of 
    // a node will be loaded and inserted into the tree

    BbqPriorityQueue                   _qLoadQueue;
    
    // the unload queue contains the pre-leaf nodes (nodes that have 4 leaf
    // child nodes). an unload operation unloads all 4
    // child nodes making the node in here a leaf node.
    // note that the root node cant ever be unloaded with this mechanism 
    // (that is a good thing..)

    BbqPriorityQueue                   _qUnloadQueue;
    
    //motor3d::Mutex                  treeAccessMutex_;
    Lock                              *_pTreeAccessMutex;

    // some statistics:
    BbqDataStatistics                  _oDataStatistics;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

   
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    static bool loadQueueComp  (const BbqBaseNode *lhs, 
                                const BbqBaseNode *rhs);
    static bool unloadQueueComp(const BbqBaseNode *lhs, 
                                const BbqBaseNode *rhs);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:
};

template<class HeightType, class HeightDeltaType, class TextureType>
class BbqTerrainEngine : public BbqTerrainEngineBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqTerrainEngineBase Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainEngine(void);
    ~BbqTerrainEngine(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    virtual void shutdown(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // todo: add the expected future position in here..
    virtual void update(const Vec3f &vViewerPosition);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    virtual void render(const BbqRenderOptions &oOptions);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool initialize(BbqDataSource *pDataSource, 
                    Int32          iMaxResidentNodeCount);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // call this from a different thread as the update/render calls: it 
    // tries to load the highest priority nodedata element.
    // and (if needed) unloads an unused element
    // returns true, if some data was loaded, false if nothing is waiting 
    // to get loaded

    virtual bool prefetchNodeData(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    virtual const BbqRenderStatistics &getRenderStatistics(void) const;

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    typedef BbqTerrainNode<HeightType, 
                           HeightDeltaType, 
                           TextureType    > BbqTerrNode;

    typedef FixedAllocator<BbqTerrNode>     BbqTerrainNodeAllocator;

    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // the renderer is in another class:
    BbqOpenGLTerrainRenderer<HeightType, 
                             HeightDeltaType, 
                             TextureType    >  _oRenderer;

    // the node allocator:
    BbqTerrainNodeAllocator                    _oNodeAllocator;

    BbqTerrNode                               *_pRootNode;
    std::vector<BbqTerrNode *>                 _vTraversalStack;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void requestLoad      (      BbqTerrNode *pNode, 
                           const Vec3f       &vViewerPosition);

    void requestUnload    (      BbqTerrNode *pNode, 
                           const Vec3f       &vViewerPosition);

    void calculatePriority(      BbqTerrNode *pNode, 
                           const Vec3f       &vViewerPosition);


    void initializeNode   (      BbqTerrNode *pNode, 
                                 BbqNodeId    id, 
                                 Int32        x0, 
                                 Int32        y0, 
                                 Int32        x1, 
                                 Int32        y1             );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    bool allocateChildren(BbqTerrNode *pNode);
    void freeChildren    (BbqTerrNode *pNode);
    bool loadChildren    (BbqTerrNode *pNode);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void checkTreeConsistency(BbqTerrNode *node);
    
    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:
};
    
/*! \brief BbqTerrain class. See \ref
           PageDrawableBbqTerrain for a description.
*/

class OSG_DRAWABLE_DLLMAPPING BbqTerrain : public BbqTerrainBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqTerrainBase Inherited;
    typedef BbqTerrain     Self;

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void fill(OSG::DrawableStatsAttachment *);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in BbqTerrainBase.

    // tmp
    BbqRenderOptions       terrainRenderOptions_;
    BbqTerrainEngineBase *_pEngine;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    BbqTerrain(void);
    BbqTerrain(const BbqTerrain &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~BbqTerrain(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    Action::ResultE renderEnter(Action  *action  );
    Action::ResultE renderLeave(Action  *action  );

    void            execute    (DrawEnv *pDrawEnv);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class BbqTerrainBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const BbqTerrain &source);
};

typedef BbqTerrain *BbqTerrainP;

OSG_END_NAMESPACE

#include "OSGBbqTerrainBase.inl"
#include "OSGBbqTerrain.inl"

#endif /* _OSGBBQTERRAIN_H_ */
