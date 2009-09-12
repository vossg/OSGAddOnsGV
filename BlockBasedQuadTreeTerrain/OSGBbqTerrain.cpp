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
    _oTerrainRenderOptions(    ),
    _pEngine              (NULL)
{
}

BbqTerrain::BbqTerrain(const BbqTerrain &source) :
     Inherited            (source),
    _oTerrainRenderOptions(      ),
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

    if(0x0000 != (whichField & ShowSwitchDistanceFieldMask))
    {
        _oTerrainRenderOptions.showSwitchDistance = getShowSwitchDistance(); 
    }

    if(0x0000 != (whichField & EnableSkirtsFieldMask))
    {
        _oTerrainRenderOptions.showSkirts = getEnableSkirts(); 
    }

    if(0x0000 != (whichField & ScreenSpaceErrorFieldMask))
    {
        _oTerrainRenderOptions.screenSpaceError = getScreenSpaceError(); 
    }

    fprintf(stderr, "BBQ:TERR changed\n");

    if(0x0000 != (whichField & DataSourceFieldMask))
    {
        fprintf(stderr, "BBQ:TERR DS changed\n");

        if(_pEngine != NULL)
        {
            _pEngine->shutdown();

            delete _pEngine;
        }

        if(getDataSource() != NULL)
        {
            Image::Type eHeightType = getDataSource()->getHeightType();
            bool        bHasGeoRef  = getDataSource()->hasGeoRef    ();

            bool rc = false;

            if(bHasGeoRef == true)
            {
                if(eHeightType == Image::OSG_UINT16_IMAGEDATA)
                {
                    fprintf(stderr, "BBQT::UInt data\n");
#define ENABLE_GEOREF
#ifdef ENABLE_GEOREF                    
                    typedef 
                        BbqGeoRefdTerrainRenderer<
                            UInt16, Int16, UInt8> TerrainRenderer;
#else
                    typedef 
                        BbqTerrainRenderer<
                            UInt16, Int16, UInt8> TerrainRenderer;
#endif

                    BbqTerrainEngine<UInt16, 
                        Int16, 
                        UInt8,
                        TerrainRenderer> *pEngine = 
                        new BbqTerrainEngine<UInt16, 
                        Int16, 
                        UInt8,
                        TerrainRenderer>();
                    
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
                    
#ifdef ENABLE_GEOREF                    
                    typedef 
                        BbqGeoRefdTerrainRenderer<
                            Int16, Int16, UInt8> TerrainRenderer;
#else
                    typedef 
                        BbqTerrainRenderer<
                            Int16, Int16, UInt8> TerrainRenderer;
#endif
                    BbqTerrainEngine<Int16, 
                        Int16, 
                        UInt8,
                        TerrainRenderer> *pEngine = 
                        new BbqTerrainEngine<Int16, 
                        Int16, 
                        UInt8,
                        TerrainRenderer>();
                    
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
            }
            else
            {
                if(eHeightType == Image::OSG_UINT16_IMAGEDATA)
                {
                    fprintf(stderr, "BBQT::UInt data\n");
                    
                    typedef 
                        BbqTerrainRenderer<
                            UInt16, Int16, UInt8> TerrainRenderer;

                    BbqTerrainEngine<UInt16, 
                        Int16, 
                        UInt8,
                        TerrainRenderer> *pEngine = 
                        new BbqTerrainEngine<UInt16, 
                        Int16, 
                        UInt8,
                        TerrainRenderer>();
                    
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

                    typedef 
                        BbqTerrainRenderer<
                            Int16, Int16, UInt8> TerrainRenderer;
                    
                    BbqTerrainEngine<Int16, 
                                     Int16, 
                                     UInt8, 
                                     TerrainRenderer> *pEngine = 
                        new BbqTerrainEngine<Int16, 
                                             Int16, 
                                             UInt8,
                                             TerrainRenderer>();
                    
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
            }
        }
    }
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



    a->pushPartition((RenderPartition::CopyViewing |
                      RenderPartition::CopyViewportSize), 
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
    Matrix m1c;

    if(getBeacon() != NullFC)
    {
        m1c = getBeacon()->getToWorld();
    }
    else
    {
        m1c.setIdentity();
    }

    Vec3f camProj(m1c[3][0],
                  m1c[3][1],
                  m1c[3][2]);

    Vec3f camPos;

    if(getDataSource()->hasGeoRef() == true)
    {
        Vec3f camPosTmp;

        const BbqDataSourceInformation &tInfo = 
            getDataSource()->getInformation();
        
        backProjectPnt(camPosTmp, camProj);
        
        camPos[0] = osgRad2Degree(camPosTmp[0]);
        camPos[1] = camPosTmp[2];
        camPos[2] = osgRad2Degree(camPosTmp[1]);

        camPos[0] = camPos[0] * tInfo.vScale[0] + tInfo.vOffset[0]; 
        camPos[2] = camPos[2] * tInfo.vScale[1] + tInfo.vOffset[1];
    }
    else
    {
        camPos = camProj;
    }

    _oTerrainRenderOptions.showBoundingBoxes    = false;
    _oTerrainRenderOptions.useVboExtension      = true;
    _oTerrainRenderOptions.enableFrustumCulling = false;

    _oTerrainRenderOptions.fovy                 = osgDegree2Rad(90);

    _oTerrainRenderOptions.screenSize.setValues(pDrawEnv->getPixelWidth(), 
                                                pDrawEnv->getPixelHeight());

    _oTerrainRenderOptions.pDrawEnv             = pDrawEnv;

    _oTerrainRenderOptions.viewerpos[0]         = camPos[0];
    _oTerrainRenderOptions.viewerpos[1]         = camPos[1];
    _oTerrainRenderOptions.viewerpos[2]         = camPos[2];

    _oTerrainRenderOptions.geoMorphFactor       = 0; //geoMorph;

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
        
        _pEngine->render(_oTerrainRenderOptions);
        
        glPopMatrix();
        
        _pEngine->prefetchNodeData();
    }
}











    
//-----------------------------------------------------------------------------



OSG_END_NAMESPACE
