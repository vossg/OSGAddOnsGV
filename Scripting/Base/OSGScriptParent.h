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

#ifndef _OSGSCRIPTPARENT_H_
#define _OSGSCRIPTRPARENT_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGNodeCore.h"
#include "OSGFrameTaskMixin.h"
#include "OSGDynamicContainerMixin.h"

OSG_BEGIN_NAMESPACE

/*! \ingroup GrpDynamicsBaseObj
    \nohierarchy
*/

struct NodeCoreScriptDesc
{
    typedef NodeCore                    ParentT;
    typedef NodeCore                   *ParentPtrT;
    typedef FieldContainer::TypeObject  TypeObject;

    static const Char8 *getTypeName      (void) { return "OSGScriptParent"; }

    static const Char8 *getParentTypeName(void) 
    {
        return "NodeCore"; 
    }
    static const Char8 *getGroupName     (void) { return "ScriptParent"; }


#if 0
    static InitContainerF         getInitMethod(void) { return NULL; }

    static FieldDescriptionBase **getDesc      (void) { return NULL; }
#endif
};

/*! \ingroup GrpDynamicsBaseObj
 */

typedef DynFieldContainer< 
          FrameTaskMixin<
            ContainerMixinHead< NodeCoreScriptDesc > > > ScriptParent;

OSG_END_NAMESPACE

#endif /* _OSGSCRIPTSORPARENT_H_ */
