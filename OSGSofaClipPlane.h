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

#ifndef _OSGSOFACLIPPLANE_H_
#define _OSGSOFACLIPPLANE_H_

#include <sofa/core/visual/VisualModel.h>
#include <sofa/component/component.h>
#include <sofa/defaulttype/Vec3Types.h>

//#include <sofa/helper/gl/template.h>

// why must OSGNode.h or others be included before OSGClipPlaneChunk 
// to avoid compilation error?
//#include "OSGChunkMaterial.h"
//#include "OSGSimpleMaterial.h"

#include "OSGContribSOFADef.h"
#include "OSGNode.h"
#include "OSGClipPlaneChunk.h"

// Clipping is not compatible with shaders unless shaders handle them

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

using ::sofa::defaulttype::Vector3;

class OSG_CONTRIBSOFA_DLLMAPPING OSGClipPlane : 
    public core::visual::VisualModel 
{

  public:

    SOFA_CLASS(OSGClipPlane, core::visual::VisualModel);

    Data<Vector3> position;
    Data<Vector3> normal;
    Data<int    > id;
    Data<bool   > active;

  public:

    virtual void init   (void                        );
    virtual void reinit (void                        );
    virtual void fwdDraw(core::visual::VisualParams *);
    virtual void bwdDraw(core::visual::VisualParams *);

    void                 setClipPlane(OSG::ClipPlaneChunk *pClip);
    OSG::ClipPlaneChunk *getClipPlane(void                      );

  protected:

    GLboolean                   _bWasActive;
    double                      _dSaveEq[4];

    OSG::ClipPlaneChunkUnrecPtr _pClipper;

    OSGClipPlane(void);
    virtual ~OSGClipPlane(void);

  private:

    // prohibit default functions (move to 'public' if you need one)
    OSGClipPlane(const OSGClipPlane &source);
    void operator =(const OSGClipPlane &source);
};

END_SOFA_CMP_VISMODEL_NAMESPACE

#include "OSGSofaClipPlane.inl"

#endif // _OSGSOFACLIPPLANE_H_
