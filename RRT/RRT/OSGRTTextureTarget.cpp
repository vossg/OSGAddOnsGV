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

#include "OSGRTTextureTarget.h"
#include "OSGWindow.h"
#include "OSGDrawEnv.h"
#include "OSGTextureObjChunk.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGRTTextureTargetBase.cpp file.
// To modify it, please change the .fcd file (OSGRTTextureTarget.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void RTTextureTarget::initMethod(InitPhase ePhase)
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

RTTextureTarget::RTTextureTarget(void) :
    Inherited()
{
}

RTTextureTarget::RTTextureTarget(const RTTextureTarget &source) :
    Inherited(source)
{
}

RTTextureTarget::~RTTextureTarget(void)
{
}

/*----------------------------- class specific ----------------------------*/

void RTTextureTarget::finalize(DrawEnv *pEnv)
{
    if(_sfTexObjChunk.getValue() != NULL)
    {
        Window *pWin = pEnv->getWindow();

        glBindTexture(GL_TEXTURE_2D, 
                      pWin->getGLObjectId(
                          _sfTexObjChunk.getValue()->getGLId()));

		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
                        _sfWidth .getValue(), 
                        _sfHeight.getValue(), 
                        GL_RGB,
                        GL_FLOAT, 
                        &(_mfPixel[0]));
    }

}

void RTTextureTarget::changed(ConstFieldMaskArg whichField, 
                              UInt32            origin,
                              BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void RTTextureTarget::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump RTTextureTarget NI" << std::endl;
}

OSG_END_NAMESPACE
