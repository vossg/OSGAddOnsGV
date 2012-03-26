/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                 Copyright (C) 2012 by the OpenSG Forum                    *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 * contact: dirk            'at' opensg.org                                  * 
 *          gerrit.voss     'at' vossg.org                                   *
 *          carsten_neumann 'at' gmx.net                                     *
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

#if __GNUC__ >= 4 || __GNUC_MINOR__ >=3
//#pragma GCC diagnostic warning "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include "OSGSofaClipPlane.h"
#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/ObjectFactory.h>

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

SOFA_DECL_CLASS(OSGClipPlane)

namespace
{
    int ClipPlaneClass = 
        core::RegisterObject("OpenSG2 Clipping Plane").add< OSGClipPlane >();
}

OSGClipPlane::OSGClipPlane(void) :
    position   (initData(&position, 
                          Vector3(0,0,0), 
                          "position", 
                          "Point crossed by the clipping plane")),
    normal     (initData(&normal, Vector3(1,0,0), 
                          "normal", 
                          "Normal of the clipping plane, "
                          "pointing toward the clipped region" )),
    id         (initData(&id, 
                          0, 
                          "id", 
                          "Clipping plane OpenGL ID"           )),
    active     (initData(&active, 
                          true, 
                          "active",
                          "Control whether the clipping plane "
                          "should be applied or not"           )),

    _bWasActive(true                                            ),
    _dSaveEq   (                                                ),
    _pClipper  (NULL                                            )
{
    _dSaveEq[0] = _dSaveEq[1] = _dSaveEq[2] = _dSaveEq[3] = 0.0;
}

OSGClipPlane::~OSGClipPlane(void)
{
}

void OSGClipPlane::init(void)
{
}

void OSGClipPlane::reinit(void)
{
}

/*
void OSGClipPlane::fwdDraw(core::visual::VisualParams*)
{
    wasActive = glIsEnabled(GL_CLIP_PLANE0+id.getValue());
    if (active.getValue())
    {
        glGetClipPlane(GL_CLIP_PLANE0+id.getValue(), saveEq);
        Vector3 p = position.getValue();
        Vector3 n = normal.getValue();
        GLdouble c[4] = { (GLdouble) -n[0], (GLdouble)-n[1], (GLdouble)-n[2], (GLdouble)(p*n) };
        glClipPlane(GL_CLIP_PLANE0+id.getValue(), c);
        if (!wasActive)
            glEnable(GL_CLIP_PLANE0+id.getValue());
    }
    else
    {
        if (wasActive)
            glDisable(GL_CLIP_PLANE0+id.getValue());
    }
}

void OSGClipPlane::bwdDraw(core::visual::VisualParams*)
{
    if (active.getValue())
    {
        glClipPlane(GL_CLIP_PLANE0+id.getValue(), saveEq);
        if (!wasActive)
            glDisable(GL_CLIP_PLANE0+id.getValue());
    }
    else
    {
        if (wasActive)
            glEnable(GL_CLIP_PLANE0+id.getValue());
    }
}
*/

END_SOFA_CMP_VISMODEL_NAMESPACE
