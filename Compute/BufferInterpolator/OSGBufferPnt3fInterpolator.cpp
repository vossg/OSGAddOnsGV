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

#include "OSGAddOnsConfig.h"

#include "OSGBufferPnt3fInterpolator.h"
#include "OSGInterpolationHelper.h"
#include "OSGTypedGeoVectorProperty.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGBufferPnt3fInterpolatorBase.cpp file.
// To modify it, please change the .fcd file
// (OSGBufferPnt3fInterpolator.fcd) and regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void BufferPnt3fInterpolator::initMethod(InitPhase ePhase)
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

BufferPnt3fInterpolator::BufferPnt3fInterpolator(void) :
    Inherited()
{
}

BufferPnt3fInterpolator::BufferPnt3fInterpolator(
    const BufferPnt3fInterpolator &source) :

    Inherited(source)
{
}

BufferPnt3fInterpolator::~BufferPnt3fInterpolator(void)
{
}

/*----------------------------- class specific ----------------------------*/

void BufferPnt3fInterpolator::changed(ConstFieldMaskArg whichField, 
                                      UInt32            origin,
                                      BitVector         details)
{
    if(0x0000 != (whichField & InValueFieldMask))
    {
        if(_sfOutValue.getValue() == NULL)
        {
            GeoPnt3fPropertyUnrecPtr pPnts = GeoPnt3fProperty::create();

            setOutValue(pPnts);
        }

        GeoPnt3fProperty *pPnts = 
            dynamic_cast<GeoPnt3fProperty *>(_sfOutValue.getValue());

        InterpolationHelper<MFReal32, 
                            MFPnt3f, 
                            MFPnt3f>::interpolate(_sfInValue.getValue(),
                                                  _mfKey,
                                                  _mfKeyValue,
                                                  *(pPnts->editFieldPtr()));
    }

    Inherited::changed(whichField, origin, details);
}

void BufferPnt3fInterpolator::dump(      UInt32    ,
                                   const BitVector ) const
{
    SLOG << "Dump BufferPnt3fInterpolator NI" << std::endl;
}

OSG_END_NAMESPACE
