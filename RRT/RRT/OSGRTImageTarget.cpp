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

#include "OSGRTImageTarget.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGRTImageTargetBase.cpp file.
// To modify it, please change the .fcd file (OSGRTImageTarget.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void RTImageTarget::initMethod(InitPhase ePhase)
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

RTImageTarget::RTImageTarget(void) :
    Inherited()
{
}

RTImageTarget::RTImageTarget(const RTImageTarget &source) :
    Inherited(source)
{
}

RTImageTarget::~RTImageTarget(void)
{
}

/*----------------------------- class specific ----------------------------*/

void RTImageTarget::changed(ConstFieldMaskArg whichField, 
                            UInt32            origin,
                            BitVector         details  )
{
    Inherited::changed(whichField, origin, details);
}

void RTImageTarget::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump RTImageTarget NI" << std::endl;
}

void RTImageTarget::finalize(DrawEnv *pEnv)
{
    if(_sfImage.getValue() == NullFC)
    {
        ImageUnrecPtr pImage = Image::create();

        _sfImage.setValue(pImage);
    }

    ImagePtr pImage = _sfImage.getValue();

    pImage->set(Image::OSG_RGB_PF ,
                _sfWidth .getValue(), 
                _sfHeight.getValue(), 
                1,
                1,
                1,
                0.0,
                (const UInt8 *) &(_mfPixel[0]),
                Image::OSG_FLOAT32_IMAGEDATA);

    pImage->convertDataTypeTo(Image::OSG_UINT8_IMAGEDATA);

    pImage->write("/tmp/foo.tif");
}

OSG_END_NAMESPACE
