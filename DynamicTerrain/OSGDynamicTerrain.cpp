/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2002 by the OpenSG Forum                 *
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
#include <OSGImage.h>
#include <OSGTextureObjChunk.h>
#include <OSGIntersectAction.h>
#include <OSGRenderTraversalActionBase.h>
//#include <OSGDrawAction.h>
#include "OSGDynamicTerrain.h"
#include "OSGImageHeightDataSource.h"
#include "OSGTerrainTools.h"
#include "OSGImageTextureDataSource.h"
#include "OSGDrawEnv.h"
#include "OSGImageFileHandler.h"

OSG_BEGIN_NAMESPACE

/*************************************************************************** \
 *                            Description                                  *
\***************************************************************************/

/*************************************************************************** \
 *                           Class variables                               *
\***************************************************************************/

/*************************************************************************** \
 *                           Class methods                                 *
\***************************************************************************/


void DynamicTerrain::initMethod(InitPhase ePhase)
{
    // register draw action handler
    
    // register renderaction handler
    Inherited::initMethod(ePhase);
    
    if(ePhase == TypeObject::SystemPost)
    {
#if 0       // register intersection handler
        RenderAction::registerEnterDefault( 
            getClassType(),
            reinterpret_cast<Action::Callback>(
                &MaterialDrawable::renderActionHandler));
        
        IntersectAction::registerEnterDefault( 
            getClassType(),
            osgTypedMethodFunctor2BaseCPtrRef<Action::ResultE,
            DynamicTerrainPtr,
            CNodePtr,
            Action *>(&DynamicTerrain::intersect));     
#endif
    }
}


/*************************************************************************** \
 *                           Instance methods                              *
\***************************************************************************/

/*----------------------- constructors & destructors ----------------------*/


DynamicTerrain::DynamicTerrain(void) : 
    Inherited()
{
    needInitialize_   = true;
    globalTextureObj_ = NullFC;
    globalTextureEnv_ = NullFC;
}


DynamicTerrain::DynamicTerrain(const DynamicTerrain &source) : 
    Inherited(source)
{
    needInitialize_   = true;
    globalTextureObj_ = NullFC;
    globalTextureEnv_ = NullFC;
}


DynamicTerrain::~DynamicTerrain(void)
{
}


void DynamicTerrain::changed(BitVector whichField, 
                             UInt32    origin,
                             BitVector details)
{
    Inherited::changed(whichField, origin, details);
    
    // todo: prevent multiple initializations: (how?)
    if((whichField & DynamicTerrain::HeightDataFieldMask))
    {
        // heightdata image changed: use the image as data source and reinitialize the data structures:
        imageHeightSource_.setImage( getHeightData() );
        
        std::cerr << "HeightDataField changed" << std::endl;
        
        needInitialize_ = true;
    }
    if( ( whichField & DynamicTerrain::TextureDataFieldMask ) )
    {
        imageTextureSource_.setImage( getTextureData() );
        
        needInitialize_ = true;
    }
    if( ( whichField & DynamicTerrain::LevelSizeFieldMask ) )
    {
        // the level size has changed: we need to reinitialize:
        needInitialize_ = true;         
    }    
    if( whichField & DynamicTerrain::UseGpuRendererFieldMask )
    {
        // todo: change the used renderer
    }
    if( ( whichField & DynamicTerrain::HeightDataOffsetFieldMask ) ||
        ( whichField & DynamicTerrain::HeightDataScaleFieldMask ) )
    {
        invalidateVolume();
    }
    if( whichField & DynamicTerrain::CpuVertexProgramFieldId )
    {
        geoClipmaps_.setCpuVertexProgramText( getCpuVertexProgram() );
    }   
    if( whichField & DynamicTerrain::CpuFragmentProgramFieldId )
    {
        geoClipmaps_.setCpuFragmentProgramText( getCpuFragmentProgram() );
    }
    if( whichField & DynamicTerrain::TextureDataFieldId )
    {
        // create a texture chunk:
        ImagePtr textureImage = getTextureData();
        
        if( textureImage != NullFC )
        {
            globalTextureObj_ = TextureObjChunk::create();
            globalTextureEnv_ = TextureEnvChunk::create();
            
            globalTextureObj_->setImage( textureImage );
            globalTextureObj_->setWrapS( GL_CLAMP );
            globalTextureObj_->setWrapT( GL_CLAMP );
            globalTextureObj_->setWrapR( GL_CLAMP );
            
            globalTextureObj_->setMinFilter( GL_LINEAR );
            globalTextureObj_->setMagFilter( GL_LINEAR );
            
            globalTextureEnv_->setEnvMode( GL_MODULATE );           
        }
        else
        {
            globalTextureObj_ = NullFC;
            globalTextureEnv_ = NullFC;
        }
    }
}


void DynamicTerrain::dumpIt(void)
{
    geoClipmaps_.dumpIt();
    
    ImageFileHandler::the()->write(getTextureData(), "/tmp/globaltex.png");
}

void DynamicTerrain::dump(UInt32, const BitVector) const
{
    SLOG << "Dump DynamicTerrain Not Implemented" << std::endl;
}



TextureDataSource *DynamicTerrain::getTextureSource(void)
{
    if( getTextureData() != NullFC )
    {
        return &imageTextureSource_;
    }
    else
    {
        return NULL;
    }
}


/// \brief Drawing the DynamicTerrain
Action::ResultE DynamicTerrain::drawPrimitives( DrawEnv *pEnv )
{
    // do frustum culling here.. extract frustum from the current camera:
    RenderTraversalActionBase* renderAction = 
        dynamic_cast< RenderTraversalActionBase* >( pEnv->getRTAction() );

    if( needInitialize_ )
    {
        if( getHeightData() != NullFC )
        {
            if( getLevelSize() < 3 )
            {
                SWARNING << "DynamicTerrain: LevelSize is below minimum "
                         << "(using default)!" << std::endl;
                
                setLevelSize( 63 );
            }
            
            // todo: choose the correct height-/texturedata source:
            geoClipmaps_.initialize( getLevelSize(), 
                                    &imageHeightSource_, 
                                     getTextureSource() );
            }
        needInitialize_ = false;
    }
    
    if( !geoClipmaps_.isValid() )
    {
        // no valid data yet
        return Action::Continue;
    }
    
    // todo: get the viewport of the RenderAction, check if the camera 
    // already has a terrain attachment:
    // if not: create a new TerrainView and attach it to the camera
    
    // update/render the view
    if( renderAction )
    {
        // frustum culling
        const FrustumVolume& frustum = renderAction->getFrustum();
        
        // make an update right here:
        Matrix camera  = pEnv->getCameraToWorld();
        Matrix toworld = pEnv->getObjectToWorld();          
        toworld.invert();
        camera.multLeft(toworld);
        Pnt3f eyePoint( camera[ 3 ][ 0 ], camera[ 3 ][ 1 ], camera[ 3 ][ 2 ] );
        
        // transform the eyePoint to the unscaled sample space:
        const WorldTransformation worldTransform = getWorldTransform();

        const Pnt3f worldOffset( worldTransform.offset[ 0 ], 
                                 0.0f, 
                                 worldTransform.offset[ 1 ] );
        
        const Pnt3f localEyePoint = 
            componentDivide( ( eyePoint - worldOffset ), 
                             worldTransform.sampleDistance );
        
        if( !getDisableUpdate() )
        {
            geoClipmaps_.update( localEyePoint );
        }           
        
        // and now draw what we have:
        ClipmapRenderParameters renderParams;
        
        renderParams.drawEnv                = pEnv;
        renderParams.window                 = renderAction->getWindow();
        renderParams.viewFrustum            = frustum;
        renderParams.enableFrustumCulling   = getEnableFrustumCulling();
        renderParams.showTransitionRegions  = getShowTransitionRegions();
        renderParams.useVboExtension        = getUseVboExtension();
        renderParams.globalTextureObj       = globalTextureObj_;
        renderParams.globalTextureEnv       = globalTextureEnv_;
        renderParams.heightColorTexture     = getHeightColorTexture();
        
        renderParams.worldTransform         = worldTransform;
        
        ClipmapRenderStatistics renderStats;
        
        geoClipmaps_.render( renderParams, renderStats );
        
        if( getShowBoundingBoxes() )
        {
            //drawBox( 
        }
        
        // update stats:
        StatCollector* statCollector = renderAction->getStatCollector();
        
        if( statCollector ) 
        {
            StatIntElem *statTriangleCount = 
                statCollector->getElem( Drawable::statNTriangles, false );
            StatIntElem *statVertexCount = 
                statCollector->getElem( Drawable::statNVertices, false );
            
            if( statTriangleCount )
            {
                statTriangleCount->add( renderStats.drawnTriangleCount );
            }
            if( statVertexCount )
            {
                statVertexCount->add( renderStats.transformedVertexCount );
            }
        }
    }
    else
    {
        //todo: can this ever happen?!
        SLOG << "Test\n";
    }
    
    return Action::Continue;
}



/// \brief Adjust the Volume of this DynamicTerrain Node (for culling!)
void DynamicTerrain::adjustVolume(Volume &volume)
{   
    volume.setValid();
    volume.setEmpty();
    
    const Pnt2f& worldOffset = getWorldOffset();
    const Pnt2f& worldSize = getWorldSize();
    Pnt2f worldMax;
    
    worldMax.setValues ( worldOffset[0] + worldSize[0],
                         worldOffset[1] + worldSize[1] );
    
    const float heightScale = getHeightDataScale();
    const float heightOffset = getHeightDataOffset();
    
    const float minHeight = 
        heightScale * imageHeightSource_.getMinHeight() + heightOffset;

    const float maxHeight = 
        heightScale * imageHeightSource_.getMaxHeight() + heightOffset;
    
    volume.extendBy( Pnt3f( worldOffset[ 0 ], minHeight, worldOffset[ 1 ] ) );
    volume.extendBy( Pnt3f( worldMax[ 0 ], maxHeight, worldMax[ 1 ] ) );
}


Action::ResultE DynamicTerrain::intersect(Action* action )
{
    IntersectAction* intersectAction = 
        dynamic_cast< IntersectAction* >( action );
    
    if( !intersectAction )
    {
        return Action::Continue;
    }
    
    Real32 t;
    Vec3f normal;
    
    if( geoClipmaps_.findFirstIntersection( intersectAction->getLine(), 
                                            t, 
                                            normal ) )
    {
        intersectAction->setHit( t, intersectAction->getActNode(), 0, normal );
    }
    
    return Action::Continue;
}


Pnt2f DynamicTerrain::getWorldOffset(void) const
{
    // offset the terrain, so that it is centered around zero:
    const Pnt2f size = getWorldSize();
    
    return Pnt2f( -size[ 0 ] / 2.0f, -size[ 1 ] / 2.0f );
}


Pnt2f DynamicTerrain::getWorldSize() const
{
    const float sampleDistance = getSampleDistance();
    const Pnt2i sampleCount = imageHeightSource_.getSampleCount();
    
    return Pnt2f( float( sampleCount[ 0 ] ) * sampleDistance, 
                  float( sampleCount[ 1 ] ) * sampleDistance );
}


WorldTransformation DynamicTerrain::getWorldTransform()
{
    WorldTransformation transform;
    
    transform.sampleDistance = getSampleDistance  ();
    transform.offset         = getWorldOffset     ();
    transform.heightScale    = getHeightDataScale ();
    transform.heightOffset   = getHeightDataOffset();
    
    return transform;
}

OSG_END_NAMESPACE
