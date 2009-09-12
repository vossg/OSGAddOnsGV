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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include <OSGConfig.h>

#include "OSGBbqTerrain.h"
#include "OSGRenderAction.h"
#include "OSGBbqDataSource.h"

#include <stack>
#include <limits>

#include <boost/bind.hpp>

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGBbqTerrainBase.cpp file.
// To modify it, please change the .fcd file (OSGBbqTerrain.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void BbqTerrain::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
        RenderAction::registerEnterDefault(
            BbqTerrain::getClassType(), 
            reinterpret_cast<Action::Callback>(&BbqTerrain::renderEnter));
        
        RenderAction::registerLeaveDefault( 
            BbqTerrain::getClassType(), 
            reinterpret_cast<Action::Callback>(&BbqTerrain::renderLeave));
    }
}


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

BbqDataStatistics::BbqDataStatistics(void) :
    loadedNodeCount(0),
    loadedNodeBuget(0),
    loadQueueSize  (0),
    unloadQueueSize(0),
    cacheMisses    (0),
    cacheHits      (0),
    bytesLoaded    (0)
{
}

/*----------------------- constructors & destructors ----------------------*/

BbqTerrain::BbqTerrain(void) :
    Inherited(),
    _oDataSourceInfo (    ),
    _oRenderer       (    ),
    _oNodeAllocator  (    ),
    _pRootNode       (NULL),
    _vTraversalStack (    ),
    _qLoadQueue      (    ),
    _qUnloadQueue    (    ),
    _pTreeAccessMutex(NULL),
    _oDataStatistics (    )
{
}

BbqTerrain::BbqTerrain(const BbqTerrain &source) :
    Inherited(source),
    _oDataSourceInfo (    ),
    _oRenderer       (    ),
    _oNodeAllocator  (    ),
    _pRootNode       (NULL),
    _vTraversalStack (    ),
    _qLoadQueue      (    ),
    _qUnloadQueue    (    ),
    _pTreeAccessMutex(NULL),
    _oDataStatistics (    )
{
}

BbqTerrain::~BbqTerrain(void)
{
    shutdown();
}

/*----------------------------- class specific ----------------------------*/

void BbqTerrain::changed(ConstFieldMaskArg whichField, 
                            UInt32            origin,
                            BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void BbqTerrain::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump BbqTerrain NI" << std::endl;
}

void BbqTerrain::fill(OSG::DrawableStatsAttachment *)
{
}

Action::ResultE BbqTerrain::renderEnter(Action* action)
{
    RenderAction *a = dynamic_cast<RenderAction *>(action);



    a->pushPartition(RenderPartition::CopyViewing, 
                     RenderPartition::SimpleCallback);
    {
        RenderPartition *pPart  = a->getActivePartition();

        pPart->setWindow(a->getWindow());

        pPart->setSetupMode(RenderPartition::EmptySetup);
         
        RenderPartition::SimpleDrawCallback f;
        
        f = boost::bind(&BbqTerrain::execute, this, _1);
        
        pPart->dropFunctor(f);
    }
    a->popPartition();

    return Action::Skip;
}

Action::ResultE BbqTerrain::renderLeave(Action* action)
{
    return Action::Skip;
}

void BbqTerrain::execute(DrawEnv *pDrawEnv)
{
#if 0
    if(terrain_ == NULL && getDataSource() != NullFC)
    {
        outOfCoreDataSource_

        bool rc = outOfCoreDataSource_->initialize( 
//        "data/ps.bbq", 
            "data/ps_com.bbq", 
            650.0f, 
            0.0f, 
            1.0f );
    }
#endif

    static bool bInit = false;

    if(bInit == false)
    {
        int rc = this->initialize(getDataSource(), 
                                  getMaxNumResidentNodes());
        
        fprintf(stderr, "t::init %d\n", rc);
        
        terrainRenderOptions_.showSkirts           = false;
        terrainRenderOptions_.showSwitchDistance   = true;

        bInit = true;

        commitChanges();
    }

//    fprintf(stderr, "BBqTerrain::execute\n");

    Matrix m1c = getBeacon()->getToWorld();

    Vec3f camPos(m1c[3][0],
                 m1c[3][1],
                 m1c[3][2]);

    terrainRenderOptions_.showBoundingBoxes    = false;
    terrainRenderOptions_.useVboExtension      = true;
    terrainRenderOptions_.enableFrustumCulling = false;
    terrainRenderOptions_.screenSpaceError     = 5.f;
    terrainRenderOptions_.fovy                 = osgDegree2Rad(90);
    terrainRenderOptions_.screenSize.setValues(500, 500); //uiSize, uiSize);
    terrainRenderOptions_.pDrawEnv             = pDrawEnv;
    terrainRenderOptions_.viewerpos[0]         = camPos[0];
    terrainRenderOptions_.viewerpos[1]         = camPos[1];
    terrainRenderOptions_.viewerpos[2]         = camPos[2];
    terrainRenderOptions_.geoMorphFactor       = 0; //geoMorph;

//    Vec3f camPos(0, 1000, 0);
    
//    fprintf(stderr, "camerapos %f %f %f\n", 
//           camPos[0],
//          camPos[1],
//          camPos[2]);

    this->update(camPos);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadMatrixf(&(pDrawEnv->getCameraViewing()[0][0]));

    this->render(terrainRenderOptions_);

    glPopMatrix();

    this->prefetchNodeData();
}





bool BbqTerrain::initialize(BbqDataSource *pDataSource, 
                            Int32          iMaxResidentNodeCount)
{
    // compute the maximum number of nodes we can keey in memory:
    
    if(!pDataSource || iMaxResidentNodeCount < 1)
    {
        return false;
    }
    
    // todo: break this method up into 2-3 methods..
    
    // check the datasource, and get the global information from it:
    _pDataSource     = pDataSource;
    _oDataSourceInfo = _pDataSource->getInformation();
    
    // initializeNodeAllocator()
    _oDataStatistics.loadedNodeBuget = osgMax(iMaxResidentNodeCount, 1);

    _oNodeAllocator.setSize(_oDataStatistics.loadedNodeBuget);
    
    // createRootNode()
    _pRootNode = _oNodeAllocator.allocate();

    assert(_pRootNode);
    
    // fill the root node with meaningful data:
    const Int32 iVirtualSize = 
        getQuadtreeLeafNodeCount(_oDataSourceInfo.heightSampleCount.x() - 1, 
                                 _oDataSourceInfo.heightSampleCount.y() - 1, 
                                 _oDataSourceInfo.heightTileSize        - 1) *
        (_oDataSourceInfo.heightTileSize - 1) + 1;
    
    _pRootNode->id           = 0;
    _pRootNode->blockScale   = float(1 << (_oDataSourceInfo.levelCount - 1));

    _pRootNode->sampleRect.setValues(0, 0, iVirtualSize, iVirtualSize);

    _pRootNode->blockOrigin.setValues( 
        -Real32(_oDataSourceInfo.heightSampleCount.x()) / 2.0f, 
        -Real32(_oDataSourceInfo.heightSampleCount.y()) / 2.0f);
    
    // get the data from the data source
    if(!_pDataSource->loadNodeData(*_pRootNode))
    {
        return false;
    }

    _oDataStatistics.loadedNodeCount = 1;
    
    // initializeRenderer()
    if(!_oRenderer.initialize(_oDataSourceInfo))
    {
        shutdown();

        return false;
    }
    
    _pTreeAccessMutex = OSG::Lock::create();
    
    return true;
}


//-----------------------------------------------------------------------------


void BbqTerrain::shutdown(void)
{
    OSG::subRef(_pTreeAccessMutex);

    _oRenderer.shutdown();
    
    _oNodeAllocator .setSize(0);
    _qUnloadQueue   .clear  ( );
    _qLoadQueue     .clear  ( );
    _vTraversalStack.clear  ( );

    _pRootNode = NULL;
}


//-----------------------------------------------------------------------------


void BbqTerrain::update(const Vec3f &vViewerPosition)
{
//    Profile( terrainUpdate );
    // lock
    //MutexLock lock( treeAccessMutex_ );

    _pTreeAccessMutex->acquire();
    
    // clear both queues:
    _qLoadQueue  .erase(_qLoadQueue  .begin(), _qLoadQueue  .end());
    _qUnloadQueue.erase(_qUnloadQueue.begin(), _qUnloadQueue.end());     
    
    // calculate the priorities for all leaf nodes (and store them into the
    // unload queue) 
    // calculate the priorities for all unloaded nodes that could be loaded
    // next (and store them into the load queue) 

    if(_pRootNode)
    {
        assert(_vTraversalStack.empty());
        
        _vTraversalStack.push_back(_pRootNode);
        
        while(!_vTraversalStack.empty())
        {
            BbqTerrainNode *node = _vTraversalStack.back();

            assert(node);
            
            const bool isBottomNode = 
                (node->treeLevel + 1 >= _oDataSourceInfo.levelCount);
            
            if(node->isLeafNode() && !isBottomNode)
            {
                // insert this node into the load queue:
                requestLoad(node, vViewerPosition);
            }
            else if(node->isPreLeafNode())
            {
                // insert this node into the unload queue:
                requestUnload(node, vViewerPosition);
            }
            
            _vTraversalStack.pop_back();

            // push the children (if any);
            if(!node->isLeafNode())
            {
                _vTraversalStack.push_back(node->children[0]);
                _vTraversalStack.push_back(node->children[1]);
                _vTraversalStack.push_back(node->children[2]);
                _vTraversalStack.push_back(node->children[3]);
            }
        }       
    }
    
    // make the heaps: (sort them)
    std::make_heap(_qLoadQueue  .begin(), _qLoadQueue  .end(), loadQueueComp  );
    std::make_heap(_qUnloadQueue.begin(), _qUnloadQueue.end(), unloadQueueComp);
    
    _oDataStatistics.loadQueueSize   = (Int32) _qLoadQueue  .size();
    _oDataStatistics.unloadQueueSize = (Int32) _qUnloadQueue.size();

    _pTreeAccessMutex->release();
    
    // unlock
}


//-----------------------------------------------------------------------------


void BbqTerrain::render(const BbqRenderOptions &oOptions)
{
//    Profile( terrainRender );
    
    // lock the tree..
    // render recursively
    _oRenderer.render(_pRootNode, oOptions);
    // unlock
} 


//-----------------------------------------------------------------------------


bool BbqTerrain::prefetchNodeData(void)
{
    //Profile( prefetchNodeData );
    
//    MutexLock lock( treeAccessMutex_ );
    _pTreeAccessMutex->acquire();

    // lock
    // get the id of the next node that should be loaded
    // unlock
    
    // load the node data (this happens parallel to the rendering/updating
    
    if(_qLoadQueue.empty())
    {
        // nothing loaded:
        _pTreeAccessMutex->release();

        return false;
    }
    
    // get the node data information to load
    BbqTerrainNode *pLoadNode = _qLoadQueue.front();
    
    // this has to be a leaf node:
    assert(pLoadNode->isLeafNode());
    
    // get 4 free slots for the children of this node:
    // if we cant get them, we need to free a node from the unload list..

    if(!allocateChildren(pLoadNode))
    {
        //checkTreeConsistency( rootNode_ );
        
        // there has to be an entry in the unload queue:
        assert(!_qUnloadQueue.empty());
        
        BbqTerrainNode *pUnloadNode = _qUnloadQueue.front();
        
        assert(pUnloadNode);
        
        // if unloading this node makes it worse, so we keep it:
        if(pUnloadNode->priority >= pLoadNode->priority)
        {
            // nothing loaded
            _pTreeAccessMutex->release();

            return false;
        }
        
        // now we can free the 4 children of this node
        freeChildren(pUnloadNode);
        //checkTreeConsistency( rootNode_ );
        
        _oDataStatistics.loadedNodeCount -= 4;
        
        assert(pLoadNode->id >= 0);
        
        if(!allocateChildren(pLoadNode))
        {
            // hhmm.. 
            //assert( false );
            _pTreeAccessMutex->release();

            return false;
        }           
    }
    
    // we have 4 valid (but empty) children now:
    assert(!pLoadNode->isLeafNode   ());
    assert( pLoadNode->isPreLeafNode());
    
    // load the data of the 4 child nodes:
    if(!loadChildren(pLoadNode))
    {
        // somthing fishy has happend..
        freeChildren(pLoadNode);

        _pTreeAccessMutex->release();

        return false;
    }
    
    //checkTreeConsistency( rootNode_ );
    
    _oDataStatistics.loadedNodeCount += 4;

    // ok.. ready to go..
    
    // remove this node from the queue
    std::pop_heap(_qLoadQueue.begin(), _qLoadQueue.end(), loadQueueComp);

    _qLoadQueue.pop_back();
    
    // lock
    // insert node into tree
    // unlock
    _pTreeAccessMutex->release();

    return false;
}


//-----------------------------------------------------------------------------


const BbqDataStatistics &BbqTerrain::getDataStatistics(void) const
{
    return _oDataStatistics;
}


//-----------------------------------------------------------------------------


const BbqRenderStatistics &BbqTerrain::getRenderStatistics(void) const
{
    return _oRenderer.getStatistics();
}


//-----------------------------------------------------------------------------


void BbqTerrain::requestLoad(      BbqTerrainNode *pNode, 
                             const Vec3f          &vViewerPosition)
{
    assert(pNode         );
    assert(pNode->id >= 0);

    calculatePriority(pNode, vViewerPosition);
    
    if(pNode->priority > 0.1f)
    {
        _qLoadQueue.push_back(pNode);
    }
}


//-----------------------------------------------------------------------------


void BbqTerrain::requestUnload(      BbqTerrainNode *pNode, 
                               const Vec3f          &vViewerPosition)
{
    assert(pNode         );
    assert(pNode->id >= 0);

    calculatePriority(pNode, vViewerPosition);

    _qUnloadQueue.push_back(pNode);
}


//-----------------------------------------------------------------------------


void BbqTerrain::calculatePriority(      BbqTerrainNode *pNode, 
                                   const Vec3f          &vViewerPosition)
{
    assert(pNode);
    
    // todo: improve this calculation..
    
    // get the bounding box, and check the distance of this node to the viewer  

    Pnt3f center;

    pNode->boundingBox.getCenter(center);

    Vec3f dist = center.subZero() - vViewerPosition;

    const float distance = dist.length();

    Vec3f size;

    pNode->boundingBox.getSize(size);

    const float nodeSize = size.x();

    pNode->priority = 1.0f / distance * ( nodeSize );
    
    // make sure, that the parent node always has at least the same priority
    // as the child node: 

    BbqTerrainNode *pParent = pNode->parent;

    while(pParent)
    {
        if(pNode->priority > pParent->priority)
        {
            pParent->priority = pNode->priority;
        }

        pParent = pParent->parent;
    }
}


//-----------------------------------------------------------------------------


void BbqTerrain::initializeNode(BbqTerrainNode *pNode, 
                                BbqNodeId       id, 
                                Int32           x0,
                                Int32           y0, 
                                Int32           x1,
                                Int32           y1  )
{
    assert(pNode        );
    assert(pNode->parent);
    
    pNode->id            = id;
    pNode->treeLevel     = pNode->parent->treeLevel + 1;
    pNode->blockScale    = 
        float(1 << (_oDataSourceInfo.levelCount - pNode->treeLevel - 1));
    
    pNode->sampleRect.setValues(x0, y0, x1, y1);

    pNode->blockOrigin.setValues( 
        x0 - Real32(_oDataSourceInfo.heightSampleCount.x()) / 2.0f,  
        y0 - Real32(_oDataSourceInfo.heightSampleCount.y()) / 2.0f);
}


//-----------------------------------------------------------------------------


bool BbqTerrain::allocateChildren(BbqTerrainNode *pNode)
{
    assert(pNode              );
    assert(pNode->id >= 0     );
    assert(pNode->isLeafNode());
    
    for(Int32 i = 0; i < 4; ++i)
    {
        pNode->children[i] = _oNodeAllocator.allocate();
        
        assert(pNode->children[i] != pNode);
        
        if(!pNode->children[i])
        {
            freeChildren(pNode);

            return false;
        }
        
        assert(pNode->children[i]->isLeafNode());

        pNode->children[i]->parent = pNode;
    }

    return true;
}


//-----------------------------------------------------------------------------


void BbqTerrain::freeChildren(BbqTerrainNode *pNode)
{
    assert(pNode);
    
    for(Int32 i = 0; i < 4; ++i)
    {
        if(pNode->children[i])
        {
            // safety..
            assert(pNode->children[ i ]->isLeafNode());

            pNode->children[i]->parent         =  0;
            pNode->children[i]->id             = -1;
            pNode->children[i]->renderCache[0] = 
                pNode->children[i]->renderCache[1] = 0;

            _oNodeAllocator.free(pNode->children[i]);

            pNode->children[i] = 0;
        }
    }
}


//-----------------------------------------------------------------------------


void BbqTerrain::checkTreeConsistency(BbqTerrainNode *pNode)
{
    assert(pNode                                 );
    assert(pNode->id >= 0                        );
    assert(pNode->id < _oDataSourceInfo.nodeCount);
    
    for(Int32 i = 0; i < 4; ++i)
    {
        if(pNode->children[i])
        {
            assert(pNode->children[i]->parent == pNode);

            checkTreeConsistency(pNode->children[i]);
        }
    }
}


//-----------------------------------------------------------------------------


bool BbqTerrain::loadChildren(BbqTerrainNode *pNode)
{
    assert(pNode                 );
    assert(pNode->isPreLeafNode());
    
    int x0 = pNode->sampleRect.x0;
    int y0 = pNode->sampleRect.y0;
    int x1 = pNode->sampleRect.x1;
    int y1 = pNode->sampleRect.y1;
    
    int xm = (x0 + x1 - 1) / 2;
    int ym = (y0 + y1 - 1) / 2;
    
    BbqNodeId firstChildId = 4 * pNode->id + 1;
    
    initializeNode(pNode->children[0], 
                   firstChildId + 0, 
                   x0, y0, 
                   xm + 1, ym + 1 );

    initializeNode(pNode->children[1], 
                   firstChildId + 1, 
                   xm, y0, 
                   x1, ym + 1 );

    initializeNode(pNode->children[2], 
                   firstChildId + 2, 
                   x0, ym, 
                   xm + 1, y1 );

    initializeNode(pNode->children[3], 
                   firstChildId + 3, 
                   xm, ym, 
                   x1, y1 );
    
    for(Int32 i = 0; i < 4; ++i)
    {
        BbqTerrainNode *pChildNode = pNode->children[i];
        
        // compute the static information for the child nodes:
        
        // and load the data:
        if(!_pDataSource->loadNodeData(*pChildNode))
        {
            return false;
        }
    }
    
    return true;
}


//-----------------------------------------------------------------------------


bool BbqTerrain::loadQueueComp(const BbqTerrainNode *lhs, 
                               const BbqTerrainNode *rhs)
{
    assert(lhs     && rhs     );
    assert(lhs->id != rhs-> id);
    
    if(lhs->priority < rhs->priority)
    {
        return true;
    }
    else if(lhs->priority > rhs->priority)
    {
        return false;
    }
    else
    {
        // use id to order nodes with the same priority:
        return lhs->id < rhs->id;
    }
}


//-----------------------------------------------------------------------------


bool BbqTerrain::unloadQueueComp(const BbqTerrainNode *lhs, 
                                 const BbqTerrainNode *rhs)
{
    assert(lhs     && rhs     );
    assert(lhs->id != rhs-> id);
    
    if(lhs->priority > rhs->priority)
    {
        return true;
    }
    else if(lhs->priority < rhs->priority)
    {
        return false;
    }
    else
    {
        // use id to order nodes with the same priority:
        return lhs->id > rhs->id;
    }
}

    
//-----------------------------------------------------------------------------



OSG_END_NAMESPACE
