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

#include "OSGBbqDataSource.h"
#include "OSGBbqTerrainNode.h"

#include "OSGGeoReferenceAttachment.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGBbqDataSourceBase.cpp file.
// To modify it, please change the .fcd file (OSGBbqDataSource.fcd) and
// regenerate the base file.

BbqDataSourceEngine::BbqDataSourceEngine(void) :
    _oInformation()
{
}

BbqDataSourceEngine::~BbqDataSourceEngine(void)
{
}

void BbqDataSourceEngine::computeBoundingBox(
    BbqTerrainNodeBase &oNode, 
    Real32              fMinHeightSample, 
    Real32              fMaxHeightSample)
{
    const Real32 blockDimension = Real32(_oInformation.heightTileSize - 1);

    const Vec2f objectBlockScale = 
        Vec2f(_oInformation.sampleSpacing * oNode.blockScale, 
              _oInformation.sampleSpacing * oNode.blockScale);

    const Vec2f objectBlockOffset = 
        _oInformation.sampleSpacing * oNode.blockOrigin;

    
    const Vec2f objectBlockSize(blockDimension * objectBlockScale.x(), 
                                blockDimension * objectBlockScale.y());
    
    const Vec3f boxMin( 
        objectBlockOffset.x(), 
        fMinHeightSample * _oInformation.heightScale + 
                           _oInformation.heightOffset, 
        objectBlockOffset.y() );

    const Vec3f boxMax(objectBlockOffset.x() + objectBlockSize.x(), 
                       fMaxHeightSample * _oInformation.heightScale + 
                                          _oInformation.heightOffset,
                       objectBlockOffset.y() + objectBlockSize.y() );
    
    oNode.boundingBox.setBounds(boxMin, boxMax);
}

const BbqDataSourceInformation &BbqDataSourceEngine::getInformation(void) const
{
    return _oInformation;
}

Image::Type BbqDataSourceEngine::getHeightType(void) const
{
    return _oInformation.heightType;
}

Image::Type BbqDataSourceEngine::getTextureType(void) const
{
    return _oInformation.textureType;
}

bool BbqDataSourceEngine::hasGeoRef(void) const
{
    return (_oInformation.uiDatum != GeoReferenceAttachment::UnknownDatum);
}

void BbqDataSourceEngine::initSourceInformation(
    BbqFile::BbqFileHeader &oHeader, 
    bool                    bIgnoreGeoRef,
    Real32                  fHeightScale, 
    Real32                  fHeightOffset, 
    Real32                  fSampleSpacing)
{
    _oInformation.levelCount         = oHeader._iLevelCount;
    _oInformation.nodeCount          = oHeader._iNodeCount;
    _oInformation.heightTileSize     = oHeader._iHeightTileSize;
    _oInformation.textureTileSize    = oHeader._iTextureTileSize;

    _oInformation.heightSampleCount  = Vec2i(oHeader._iHeightSampleCountX, 
                                             oHeader._iHeightSampleCountY );
    _oInformation.textureSampleCount = Vec2i(oHeader._iTextureSampleCountX, 
                                             oHeader._iTextureSampleCountY);
    _oInformation.heightType         = oHeader._eHeightType;
    _oInformation.textureType        = oHeader._eTextureType;
    _oInformation.textureFormat      = oHeader._eTextureFormat;
    _oInformation.heightFormat       = oHeader._eHeightFormat;

    _oInformation.uiDatum            = oHeader._uiDatum;
    _oInformation.vEllipsoidAxis     = oHeader._vEllipsoidAxis;
    _oInformation.vOrigin            = oHeader._vOrigin;
    _oInformation.vPixelSize         = oHeader._vPixelSize;

    if(hasGeoRef() && !bIgnoreGeoRef)
    {
        fprintf(stderr, "GeoRef data\n");

        Pnt3f x1;
        Pnt3f x2;

        projectPnt(x1, 
                   oHeader._vOrigin[0], 
                   oHeader._vOrigin[1], 
                   0);

        projectPnt(x2, 
                   oHeader._vOrigin[0] + oHeader._vPixelSize[0], 
                   oHeader._vOrigin[1], 
                   0);
        
        Vec3f v1 = x1 - x2;
        
        fprintf(stderr, "l:%f\n", v1.length());

        _oInformation.heightScale        = 32.768f;
        _oInformation.heightOffset       = 0.f;
        _oInformation.sampleSpacing      = v1.length();
    }
    else
    {
        fprintf(stderr, "No GeoRef data %d %d\n", hasGeoRef(), bIgnoreGeoRef);

        _oInformation.heightScale        = fHeightScale;
        _oInformation.heightOffset       = fHeightOffset;
        _oInformation.sampleSpacing      = fSampleSpacing;
    }

    fprintf(stderr, "%d \n%f %f\n%f %f | %f %f\n",
            oHeader._uiDatum,
            oHeader._vEllipsoidAxis[0],
            oHeader._vEllipsoidAxis[1],
            oHeader._vOrigin[0],
            oHeader._vOrigin[1],
            oHeader._vPixelSize[0],
            oHeader._vPixelSize[1]);

    const Real32 rootScale     =  Real32(1 << (_oInformation.levelCount - 1));
    const Real32 rootDimension = Real32 (_oInformation.heightTileSize - 1);


    const Vec2f rootOrigin(-Real32(_oInformation.heightSampleCount.x()) / 2.0f, 
                           -Real32(_oInformation.heightSampleCount.y()) / 2.0f);

    const Vec2f rootBlockOffset = _oInformation.sampleSpacing * rootOrigin;

    const Vec2f rootBlockScale(_oInformation.sampleSpacing * rootScale, 
                               _oInformation.sampleSpacing * rootScale);

    const Vec2f rootBlockSize(rootDimension * rootBlockScale.x(), 
                              rootDimension * rootBlockScale.y());

    _oInformation.gridBBoxMin.setValues(rootBlockOffset.x(), 
                                        rootBlockOffset.y());

    
    _oInformation.gridBBoxMax.setValues(
        rootBlockOffset.x() + rootBlockSize.x(), 
        rootBlockOffset.y() + rootBlockSize.y() );

    _oInformation.vScale.setValues(
        _oInformation.sampleSpacing / _oInformation.vPixelSize[0],
        _oInformation.sampleSpacing / _oInformation.vPixelSize[1]);

    _oInformation.vOffset.setValues(
        _oInformation.gridBBoxMin[0] - (_oInformation.vOrigin[0] * 
                                        _oInformation.vScale [0]),
        _oInformation.gridBBoxMin[1] - (_oInformation.vOrigin[1] * 
                                        _oInformation.vScale [1]));

}



/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void BbqDataSource::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
    }
}


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

BbqDataSource::BbqDataSource(void) :
     Inherited   (    ),
    _pEngine     (NULL),
    _oDefaultInfo(    )
{
}

BbqDataSource::BbqDataSource(const BbqDataSource &source) :
     Inherited   (source),
    _pEngine     (NULL  ),
    _oDefaultInfo(      )
{
}

BbqDataSource::~BbqDataSource(void)
{
    delete _pEngine;
}

/*----------------------------- class specific ----------------------------*/

void BbqDataSource::changed(ConstFieldMaskArg whichField, 
                            UInt32            origin,
                            BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void BbqDataSource::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump BbqDataSource NI" << std::endl;
}

const BbqDataSourceInformation &BbqDataSource::getInformation(void) const
{
    if(_pEngine != NULL)
        return _pEngine->getInformation();

    return _oDefaultInfo;
}

Image::Type BbqDataSource::getHeightType(void) const
{
    if(_pEngine != NULL)
        return _pEngine->getHeightType();

    return Image::OSG_INVALID_IMAGEDATATYPE;
}

Image::Type BbqDataSource::getTextureType(void) const
{
    if(_pEngine != NULL)
        return _pEngine->getTextureType();

    return Image::OSG_INVALID_IMAGEDATATYPE;
}

bool BbqDataSource::hasGeoRef(void) const
{
    if(_pEngine != NULL)
        return (_pEngine->hasGeoRef() & !getIgnoreGeoRef());

    return false;
}

bool BbqDataSource::loadNodeData(BbqTerrainNodeBase &oNode)
{
    if(_pEngine != NULL)
        return _pEngine->onLoadNodeData(oNode);
    else
        return false;
}



OSG_END_NAMESPACE
