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

#ifndef _OSGSOFALIGHTMANAGER_H_
#define _OSGSOFALIGHTMANAGER_H_

#include <sofa/core/visual/VisualManager.h>

#include "OSGContribSOFADef.h"

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE
using namespace sofa::core::visual;

// Dummy LightManager class to circumvent the calling of 
// non-virtual restoreDefaultLight() which is executed in 
// the original LightManager's destructor. restoreDefaultLight
// issues OpenGL calls but the openGL context may not be valid at that point.

class OSG_CONTRIBSOFA_DLLMAPPING OSGSofaLightManager : public VisualManager
{

  public:
    SOFA_CLASS(OSGSofaLightManager, VisualManager);
    OSGSofaLightManager() {}
    ~OSGSofaLightManager() {}

    void init() {}
    void initVisual() {}
    void reinit() {}
    void update() {}

};



END_SOFA_CMP_VISMODEL_NAMESPACE

//#include "OSGSofaLightManager.inl"

#endif // _OSGSOFALIGHTMANAGER
