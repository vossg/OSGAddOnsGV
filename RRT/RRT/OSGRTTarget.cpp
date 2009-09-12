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

#include "OSGRTTarget.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGRTTargetBase.cpp file.
// To modify it, please change the .fcd file (OSGRTTarget.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void RTTarget::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
    }
}

void RTTarget::onCreate(const RTTarget *source)
{
    if(GlobalSystemState == Startup)
        return;
}

void RTTarget::onDestroy(UInt32)
{
}


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

RTTarget::RTTarget(void) :
     Inherited(),
    _mfPixel  ()
{
}

RTTarget::RTTarget(const RTTarget &source) :
     Inherited(source),
    _mfPixel  (      )
{
}

RTTarget::~RTTarget(void)
{
}

/*----------------------------- class specific ----------------------------*/

void RTTarget::finalize(DrawEnv *)
{
}
    
void RTTarget::resize(UInt32 uiWidth, UInt32 uiHeight)
{
    setWidth (uiWidth );
    setHeight(uiHeight);

    _mfPixel.resize(uiWidth * uiHeight * 3);
}


void RTTarget::changed(ConstFieldMaskArg whichField, 
                       UInt32            origin,
                       BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void RTTarget::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump RTTarget NI" << std::endl;
}

void RTTarget::startFrame(void)
{
    memset(&(_mfPixel[0]), 
           0, 
           _sfWidth.getValue() * _sfHeight.getValue() * 3 * sizeof(Real32));
}

OSG_END_NAMESPACE
