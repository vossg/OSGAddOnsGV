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

#include "OSGBbqTerrainCreator.h"
#include "OSGLog.h"

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE


BbqCreateEngineBase::BbqCreateEngineBase(void):
    _iTileSize             (   0),
    _iTextureSize          (   0),
    _iLevelCount           (   0),
    _iTextureLeafLevel     (   0),
    _vHeightSampleCount    (0, 0),
    _vTextureSampleCount   (0, 0),
    _pHeightFieldImage     (NULL),
    _pTextureImage         (NULL),
    _pOutputFile           (NULL),
    _oNodeIterator         (    )
{
}

BbqCreateEngineBase::~BbqCreateEngineBase(void)
{
}

bool BbqCreateEngineBase::start(ImageBlockAccessor *pHeightFieldImage,
                                ImageBlockAccessor *pTextureImage,
                                BbqFileWriter      *pOutputFile,
                                Int32               iTileSize, 
                                Int32               iTextureSize     )
{
    _pHeightFieldImage = pHeightFieldImage;

    if(pTextureImage->isOpen() == true)
    {
        _pTextureImage = pTextureImage;
    }
    else
    {
        _pTextureImage = NULL;
    }
    
    _pOutputFile       = pOutputFile;

    _iTileSize    = iTileSize;
    _iTextureSize = iTextureSize;
    
    _vHeightSampleCount  = _pHeightFieldImage->getSize();

    if(_pTextureImage != NULL)
    {
        _vTextureSampleCount = _pTextureImage    ->getSize();
    }
    else
    {
        _vTextureSampleCount.setValues(0, 0);
    }
    
    // 1. step: build the hollow tree (dont read any data at all from the 
    // file for now)
    
    // todo: support more than one tree
    // todo: support different texture/heightdata resolutions:
    
    // compute the number of nodes needed:
    _iLevelCount = getQuadtreeDepth(_vHeightSampleCount.x() - 1, 
                                    _vHeightSampleCount.y() - 1, 
                                    _iTileSize              - 1);
    

    if(_pTextureImage != NULL)
    {
        // get the level, where we need to store the texture leafs:
        _iTextureLeafLevel = getQuadtreeDepth(_vTextureSampleCount.x(), 
                                              _vTextureSampleCount.y(), 
                                              _iTextureSize           ) - 1;
    
        if(_iTextureLeafLevel >= _iLevelCount)
        {
            // the texture image is to big to be displayed completely with this 
            // per tile textureSize..
            // todo: output a warning.
            _iTextureLeafLevel = _iLevelCount - 1;
        }
    }
    else
    {
        _iTextureLeafLevel = 0;
    }

    return true;
}

float BbqCreateEngineBase::getProgress(void) const
{
    return _oNodeIterator.getProgress();
}

bool BbqCreateEngineBase::isFinished(void) const
{
    return _oNodeIterator.isFinished();      
}


//-----------------------------------------------------------------------------


BbqNodeId BbqCreateEngineBase::getChildNodeId(BbqNodeId iId, Int32 iChildIndex)
{
    return 4 * iId + iChildIndex + 1;
}


//-----------------------------------------------------------------------------







//-----------------------------------------------------------------------------


BbqTerrainCreator::BbqTerrainCreator(void) :
    _pEngine          (NULL),
    _oHeightFieldImage(    ),
    _oTextureImage    (    ),
    _oOutputFile      (    )
{
}

BbqTerrainCreator::~BbqTerrainCreator(void)
{
    delete _pEngine;
}

//-----------------------------------------------------------------------------


bool BbqTerrainCreator::start(const std::string &szHeightFieldFilename, 
                              const std::string &szTextureFilename, 
                              const std::string &szOutputFilename, 
                                    Int32        iTileSize, 
                                    Int32        iTextureSize )
{
    if(iTileSize !=   9 && 
       iTileSize !=  17 && 
       iTileSize !=  33 && 
       iTileSize !=  65 && 
       iTileSize != 129  )
    {
        return false;
    }

    
    if(!_oHeightFieldImage.open(szHeightFieldFilename))
    {
        SWARNING <<  "Could not open heightmap file '"
                 << szHeightFieldFilename
                 << "' !\n";

        return false;
    }   
    
    fprintf(stderr, "%x %x\n",
            _oHeightFieldImage.getFormat(),
            _oHeightFieldImage.getType  ());

   
    if(!_oTextureImage.open(szTextureFilename))
    {
        SWARNING << "Could not open texture file '" 
                 << szTextureFilename
                 << "' !\n" ;
    }
    
    if(!_oOutputFile.open(szOutputFilename))
    {
        SWARNING << "Could not open output file '" 
                 << szOutputFilename
                 << "' !\n";

        return false;
    }

    if(_oHeightFieldImage.getFormat() == Image::OSG_L_PF)
    {
        if(_oHeightFieldImage.getType  () == Image::OSG_UINT16_IMAGEDATA)
        {
            _pEngine = new BbqCreateEngine<UInt16, UInt8>();
        }    
        else if(_oHeightFieldImage.getType  () == Image::OSG_INT16_IMAGEDATA)
        {
            _pEngine = new BbqCreateEngine<Int16, UInt8>();
        }    
    }

    if(_pEngine == NULL)
    {
        SWARNING << "Could not find matching engine '" 
                 << "' !\n";

        return false;
    }

    _pEngine->start(&_oHeightFieldImage,
                    &_oTextureImage,
                    &_oOutputFile,
                      iTileSize,
                      iTextureSize);

    
    
    return true;
}


//-----------------------------------------------------------------------------


void BbqTerrainCreator::step(void)
{
    if(_pEngine != NULL)
        _pEngine->step();
}


//-----------------------------------------------------------------------------


float BbqTerrainCreator::getProgress(void) const
{
    if(_pEngine != NULL)
    {
        return _pEngine->getProgress();
    }
    else
    {
        return 0.f;
    }
}


//-----------------------------------------------------------------------------


bool BbqTerrainCreator::isFinished(void) const
{
    if(_pEngine != NULL)
    {
        return _pEngine->isFinished();
    }
    else
    {
        return true;
    }
}


//-----------------------------------------------------------------------------

#if 0
void BbqTerrainCreator::fillHeightData(BbqCreationNode &oNode)
{
    for(Int32 y = 0; y < _iTileSize; ++y)
    {
        for(Int32 x = 0; x < _iTileSize; ++x)
        {
            oNode.data->heightData[y * _iTileSize + x] = 
                UInt16(100 * x + 400 * y);
        }
    }

    oNode.data->heightDataValid = true;
}
#endif


OSG_END_NAMESPACE
