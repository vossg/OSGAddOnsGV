/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2002 by the OpenSG Forum                   *
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

#include <cstdlib>
#include <cstdio>

#include <OSGGL.h>

#ifndef OSG_WINCE
#include <OSGIntersectAction.h>
#endif

#include <OSGRenderAction.h>

#include "OSGLabel.h"
#include "OSGVolume.h"
#include "OSGTextureObjChunk.h"

OSG_USING_NAMESPACE

// Documentation for this class is emited in the
// OSGLabelBase.cpp file.
// To modify it, please change the .fcd file (OSGLabel.fcd) and
// regenerate the base file.

/*-------------------------------------------------------------------------*/
/*                               Sync                                      */

void Label::changed(ConstFieldMaskArg whichField, 
                    UInt32            origin,
                    BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

/*-------------------------------------------------------------------------*/
/*                                Dump                                     */

void Label::dump(UInt32 uiIndent, const BitVector bvFlags ) const
{
   Inherited::dump(uiIndent, bvFlags);
}

/*-------------------------------------------------------------------------*/
/*                            Constructors                                 */

Label::Label(void) :
    Inherited()
{
}

Label::Label(const Label &source) : Inherited(source)
{
}

/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

Label::~Label(void)
{
}


/*-------------------------------------------------------------------------*/
/*                                Render                                   */

ActionBase::ResultE Label::renderEnter(Action *action)
{
    RenderAction *pAction = 
        dynamic_cast<RenderAction *>(action);

    pAction->pushVisibility();

    return ActionBase::Continue;
}

ActionBase::ResultE Label::renderLeave(Action *action)
{
    RenderAction *pAction = 
        dynamic_cast<RenderAction *>(action);

    pAction->popVisibility();

    return ActionBase::Continue;
}

/*-------------------------------------------------------------------------*/
/*                            Intersect                                    */

#ifndef OSG_WINCE
ActionBase::ResultE Label::intersect(Action *action)
{
          IntersectAction *ia = dynamic_cast<IntersectAction *>(action);
    const BoxVolume       &bv = ia->getActNode()->getVolume();

    if(bv.isValid() && ! bv.intersect(ia->getLine()))
    {
        return Action::Skip;  //bv missed -> can not hit children
    }

    return ActionBase::Continue;
}
#endif

/*-------------------------------------------------------------------------*/
/*                                 Init                                    */

void Label::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
        RenderAction::registerEnterDefault(
            Label::getClassType(), 
            reinterpret_cast<Action::Callback>(&Label::renderEnter));

        RenderAction::registerLeaveDefault(
            Label::getClassType(), 
            reinterpret_cast<Action::Callback>(&Label::renderLeave));
#ifndef OSG_WINCE
        IntersectAction::registerEnterDefault(
            getClassType(),
            reinterpret_cast<Action::Callback>(&Label::intersect));
#endif 
    }
}

