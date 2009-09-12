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

#include "OSGBbqOutOfCoreDataSource.h"
#include "OSGBbqNode.h"

#include "OSGBaseFunctions.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGBbqOutOfCoreDataSourceBase.cpp file.
// To modify it, please change the .fcd file (OSGBbqOutOfCoreDataSource.fcd) and
// regenerate the base file.



/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void BbqOutOfCoreDataSource::initMethod(InitPhase ePhase)
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

BbqOutOfCoreDataSource::BbqOutOfCoreDataSource(void) :
    Inherited()
{
}

BbqOutOfCoreDataSource::BbqOutOfCoreDataSource(
    const BbqOutOfCoreDataSource &source) :

    Inherited(source)
{
}

BbqOutOfCoreDataSource::~BbqOutOfCoreDataSource(void)
{
}

/*----------------------------- class specific ----------------------------*/

void BbqOutOfCoreDataSource::changed(ConstFieldMaskArg whichField, 
                                     UInt32            origin,
                                     BitVector         details)
{
    Inherited::changed(whichField, origin, details);

    if(0x0000 != (whichField & (FilenameFieldMask     |
                                HeightScaleFieldMask  |
                                HeightOffsetFieldMask |
                                SampleSpacingFieldMask)))
    {
        initialize(getFilename().c_str(),
                   getHeightScale  (),
                   getHeightOffset (),
                   getSampleSpacing());
        
        
        const BbqDataSourceInformation &bbqInfo = 
            this->getInformation();
        
        
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
    }
}

void BbqOutOfCoreDataSource::dump(      UInt32    ,
                                  const BitVector ) const
{
    SLOG << "Dump BbqOutOfCoreDataSource NI" << std::endl;
}


bool BbqOutOfCoreDataSource::initialize(const std::string &szFilename, 
                                              Real32       fHeightScale, 
                                              Real32       fHeightOffset, 
                                              Real32       fSampleSpacing)
{
    if(_pEngine == NULL)
    {
        BbqFileReader oInput;

        if(!oInput.open(szFilename))
        {
            // file not found / invalid..
            // todo: output a warning/message
            return false;
        }

        // initialize the nodes in the tree:
        BbqFile::BbqFileHeader oHeader = oInput.getHeader();

        oInput.close();

        bool rc = false;

        if(oHeader._eHeightType == Image::OSG_UINT16_IMAGEDATA)
        {
            fprintf(stderr, "UInt data\n");

            BbqOutOfCoreEngine<UInt16, Int16, UInt8> *pEngine = 
                new BbqOutOfCoreEngine<UInt16, Int16, UInt8>();

            rc = pEngine->initialize(szFilename,
                                     fHeightScale,
                                     fHeightOffset,
                                     fSampleSpacing);
            
            if(rc == true)
            {
                _pEngine = pEngine;
            }
            else
            {
                delete pEngine;
            }
        }
        else if(oHeader._eHeightType == Image::OSG_INT16_IMAGEDATA)
        {
            fprintf(stderr, "Int data\n");

            BbqOutOfCoreEngine<Int16, Int16, UInt8> *pEngine = 
                new BbqOutOfCoreEngine<Int16, Int16, UInt8>();

            rc = pEngine->initialize(szFilename,
                                     fHeightScale,
                                     fHeightOffset,
                                     fSampleSpacing);
            
            if(rc == true)
            {
                _pEngine = pEngine;
            }
            else
            {
                delete pEngine;
            }
        }
       
        return rc;
    }

    return true;
}


//-----------------------------------------------------------------------------



OSG_END_NAMESPACE
