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


BbqTerrainEngineBase::BbqTerrainEngineBase(void) :
    _pDataSource     (NULL),
    _oDataSourceInfo (    ),
    _qLoadQueue      (    ),
    _qUnloadQueue    (    ),
    _pTreeAccessMutex(NULL),
    _oDataStatistics (    )
{
}

BbqTerrainEngineBase::~BbqTerrainEngineBase(void)
{
}

//-----------------------------------------------------------------------------


const BbqDataStatistics &BbqTerrainEngineBase::getDataStatistics(void) const
{
    return _oDataStatistics;
}


//-----------------------------------------------------------------------------


void BbqTerrainEngineBase::shutdown(void)
{
    OSG::subRef(_pTreeAccessMutex);
  
    _qUnloadQueue   .clear  ( );
    _qLoadQueue     .clear  ( );
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------


bool BbqTerrainEngineBase::loadQueueComp(const BbqBaseNode *lhs, 
                                         const BbqBaseNode *rhs)
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


bool BbqTerrainEngineBase::unloadQueueComp(const BbqBaseNode *lhs, 
                                           const BbqBaseNode *rhs)
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
     Inherited            (    ),
     terrainRenderOptions_(    ),
    _pEngine              (NULL)
{
}

BbqTerrain::BbqTerrain(const BbqTerrain &source) :
     Inherited            (source),
     terrainRenderOptions_(      ),
    _pEngine              (NULL  )
{
}

BbqTerrain::~BbqTerrain(void)
{
    if(_pEngine != NULL)
        _pEngine->shutdown();

    delete _pEngine;
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
        if(getDataSource() != NULL)
        {
            Image::Type eHeightType = getDataSource()->getHeightType();

            bool rc = false;

            if(eHeightType == Image::OSG_UINT16_IMAGEDATA)
            {
                fprintf(stderr, "BBQT::UInt data\n");

                BbqTerrainEngine<UInt16, Int16, UInt8> *pEngine = 
                    new BbqTerrainEngine<UInt16, Int16, UInt8>();
                
                rc = pEngine->initialize(getDataSource(), 
                                         getMaxNumResidentNodes());

                if(rc == true)
                {
                    _pEngine = pEngine;
                }
                else
                {
                    delete pEngine;
                }
            }
            else if(eHeightType == Image::OSG_INT16_IMAGEDATA)
            {
                fprintf(stderr, "BBQT::Int data\n");

                BbqTerrainEngine<Int16, Int16, UInt8> *pEngine = 
                    new BbqTerrainEngine<Int16, Int16, UInt8>();

                rc = pEngine->initialize(getDataSource(), 
                                         getMaxNumResidentNodes());

                if(rc == true)
                {
                    _pEngine = pEngine;
                }
                else
                {
                    delete pEngine;
                }
            }

            
            fprintf(stderr, "t::init %d\n", rc);
            
            terrainRenderOptions_.showSkirts           = false;
            terrainRenderOptions_.showSwitchDistance   = true;
        }

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

    if(_pEngine != NULL)
    {
        _pEngine->update(camPos);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        
        glLoadMatrixf(&(pDrawEnv->getCameraViewing()[0][0]));
        
        _pEngine->render(terrainRenderOptions_);
        
        glPopMatrix();
        
        _pEngine->prefetchNodeData();
    }
}











    
//-----------------------------------------------------------------------------



OSG_END_NAMESPACE
