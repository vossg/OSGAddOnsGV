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

#ifndef _OSGSOFASHADOWGRAPH_H_
#define _OSGSOFASHADOWGRAPH_H_

// SOFA
#include <sofa/simulation/common/Node.h>

#include "OSGContribSOFADef.h"
#include "OSGShadowStage.h"
#include "OSGChunkOverrideGroup.h"

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

class OSG_CONTRIBSOFA_DLLMAPPING OSGSofaShadowGraph 
{
  public:

    OSGSofaShadowGraph( void );

    ~OSGSofaShadowGraph( void );

  public:

    // OpenSG
    OSG::NodeTransitPtr build( sofa::simulation::Node::SPtr root , bool ignoreLights);
    OSG::ShadowStage* getShadowStage() const { return _shadowStage; }
    OSG::Node* getScene() const { return _scene; }

  private:
    

    OSG::ShadowStageUnrecPtr _shadowStage;
    OSG::ChunkOverrideGroupUnrecPtr _chunkOverrideGroup;
    OSG::NodeUnrecPtr _scene;


};

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFASHADOWGRAPH_H_
