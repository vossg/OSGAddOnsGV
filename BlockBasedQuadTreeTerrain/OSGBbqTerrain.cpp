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

/*----------------------- constructors & destructors ----------------------*/

BbqTerrain::BbqTerrain(void) :
    Inherited()
{
    terrain_             = NULL;
    outOfCoreDataSource_ = NULL;
}

BbqTerrain::BbqTerrain(const BbqTerrain &source) :
    Inherited(source)
{
    terrain_             = NULL;
    outOfCoreDataSource_ = NULL;
}

BbqTerrain::~BbqTerrain(void)
{
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
    if(terrain_ == NULL)
    {
        outOfCoreDataSource_ = new BbqOutOfCoreDataSourceEngine();

        bool rc = outOfCoreDataSource_->initialize( 
//        "data/ps.bbq", 
            "data/ps_com.bbq", 
            650.0f, 
            0.0f, 
            1.0f );
        
        fprintf(stderr, "ds::init %d\n", rc);
        
        const BbqDataSourceInformation &bbqInfo = 
            outOfCoreDataSource_->getInformation();
        
        
        fprintf(stderr, "%d %d\n",
                bbqInfo.levelCount,
                bbqInfo.nodeCount);
        
        fprintf(stderr, "%d %d %d\n",
                bbqInfo.heightTileSize,
                bbqInfo.heightSampleCount[0],
                bbqInfo.heightSampleCount[1]);
        
        fprintf(stderr, "%f %f %f\n",
                bbqInfo.heightScale,
                bbqInfo.heightOffset,
                bbqInfo.sampleSpacing);
        
    

        const int maxResidentNodeCount = 5000;
        
        terrain_ = new BbqTerrainEngine();
        
        rc = terrain_->initialize(outOfCoreDataSource_, maxResidentNodeCount);
        
        fprintf(stderr, "t::init %d\n", rc);
        
        terrainRenderOptions_.showSkirts           = false;
        terrainRenderOptions_.showSwitchDistance   = true;
    }

    fprintf(stderr, "BBqTerrain::execute\n");

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

    terrain_->update(camPos);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadMatrixf(&(pDrawEnv->getCameraViewing()[0][0]));

    terrain_->render(terrainRenderOptions_);

    glPopMatrix();

    terrain_->prefetchNodeData();
}

OSG_END_NAMESPACE
