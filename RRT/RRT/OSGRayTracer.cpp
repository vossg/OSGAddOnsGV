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

#include "OSGPerspectiveCamera.h"

#include "OSGRTTarget.h"
#include "OSGRayTracer.h"

#include "OSGOSGWriter.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGRayTracerBase.cpp file.
// To modify it, please change the .fcd file (OSGRayTracer.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/


/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void RayTracer::initMethod(InitPhase ePhase)
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

RayTracer::RayTracer(void) :
     Inherited     (    ),
    _pTarget       (NULL)
{
}

RayTracer::RayTracer(const RayTracer &source) :
     Inherited     (source),
    _pTarget       (NULL  )
{
}

RayTracer::~RayTracer(void)
{
    _pTarget = NullFC;
}

/*----------------------------- class specific ----------------------------*/



void RayTracer::setResolution(UInt32 uiWidth, UInt32 uiHeight)
{
    setWidth (uiWidth );
    setHeight(uiHeight);

    if(_pTarget != NULL)
    {
        _pTarget->resize(_sfWidth .getValue(), 
                         _sfHeight.getValue());
    }
}

void RayTracer::setTarget(RTTarget *pTarget)
{
    _pTarget = pTarget;

    if(_pTarget != NULL)
    {
        _pTarget->resize(_sfWidth .getValue(), 
                         _sfHeight.getValue());
    }
}

void RayTracer::changed(ConstFieldMaskArg whichField, 
                        UInt32            origin,
                        BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void RayTracer::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump RayTracer NI" << std::endl;
}

OSG_END_NAMESPACE
