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

#include "OSGSofaShadowGraph.h"
#include "OSGSofaVisualVisitor.h"

namespace sofa
{

namespace component
{

namespace visualmodel
{

OSG2_ShadowGraph::OSG2_ShadowGraph( void )
:
_shadowStage(NULL)
,_chunkOverrideGroup(NULL)
,_scene(NULL)
{

}

OSG2_ShadowGraph::~OSG2_ShadowGraph( void )
{
    _shadowStage = NULL;
    _scene = NULL;
    _chunkOverrideGroup = NULL;
}

OSG::NodeTransitPtr 
OSG2_ShadowGraph::build( sofa::simulation::Node::SPtr root, bool ignoreLights)
{
    _scene = NULL;
    _chunkOverrideGroup = NULL;
    _shadowStage = NULL;

    sofa::core::visual::VisualParams* vparams = sofa::core::visual::VisualParams::defaultInstance();

    OSG::NodeUnrecPtr        shadowStageNode;
    
    _chunkOverrideGroup = OSG::ChunkOverrideGroup::create();

    _scene = OSG::makeNodeFor(_chunkOverrideGroup);

    sofa::simulation::OSGVisualUpdateVisitor vm_visitor( vparams);
    vm_visitor.setChunkOverrideGroupNode(_scene);
    // get lights
    if (!ignoreLights)
    {
        if (!_shadowStage) _shadowStage = OSG::ShadowStage::create();
        _shadowStage->setMapSize(1024);
        _shadowStage->setShadowSmoothness(0.5f);
        _shadowStage->setShadowMode(OSG::ShadowStage::NO_SHADOW);
        //_shadowStage->setShadowMode(OSG::ShadowStage::STD_SHADOW_MAP);
        _shadowStage->setAutoSearchForLights(true);
        shadowStageNode = OSG::makeNodeFor(_shadowStage);

        sofa::simulation::OSGLightVisitor light_visitor( vparams);
        light_visitor.setOSG2Parent(_scene);
        light_visitor.setOSG2ShadowStage(_shadowStage);
        light_visitor.setAttachNode(_scene);
        root->execute(&light_visitor);
        if (light_visitor.getAttachNode())
        {
            vm_visitor.setOSG2Parent(light_visitor.getAttachNode());
            root->execute(&vm_visitor);
        }
    }
    else
    {
        vm_visitor.setOSG2Parent(_scene);
        root->execute(&vm_visitor);
    }

    if (shadowStageNode && !ignoreLights) 
    {
        shadowStageNode->addChild(_scene);
        return OSG::NodeTransitPtr(shadowStageNode);
    }

    return OSG::NodeTransitPtr(_scene);
}


} // namespace visualmodel

} // namespace component

} // namespace sofa
