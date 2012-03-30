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

BEGIN_SOFA_SIM_NAMESPACE

inline
OSGLightVisitor::OSGLightVisitor(const core::ExecParams *pParams) : 
     Visitor       (pParams),
    _pAttachNode   (NULL   ),
    _pParent       (NULL   ),
    _pShadowStage  (NULL   ), 
    _bTurnOffLights(false  ) 
{
}

inline
OSGLightVisitor::~OSGLightVisitor(void)
{
}

inline
const char *OSGLightVisitor::getClassName(void) const 
{
    return "OSGLightVisitor"; 
}

inline
OSG::Node *OSGLightVisitor::getAttachNode(void) const 
{
    return _pAttachNode; 
}

inline
void OSGLightVisitor::setAttachNode(OSG::Node *pNode) 
{
    _pAttachNode = pNode; 
}

inline
OSG::Node *OSGLightVisitor::getOSG2Parent(void) const 
{
    return _pParent; 
}

inline
void OSGLightVisitor::setOSG2Parent(OSG::Node *pParent)
{
    _pParent = pParent; 
}

inline
bool OSGLightVisitor::getTurnOffLights(void) const 
{
    return _bTurnOffLights; 
}

inline
void OSGLightVisitor::setTurnOffLights(bool off) 
{
    _bTurnOffLights = off; 
}

inline
OSG::ShadowStage *OSGLightVisitor::getOSG2ShadowStage(void) const
{
    return _pShadowStage; 
}

inline
void OSGLightVisitor::setOSG2ShadowStage(OSG::ShadowStage *pShadow)
{ 
    _pShadowStage = pShadow;
}




inline
OSGVisualUpdateVisitor::OSGVisualUpdateVisitor(const core::ExecParams *pParams):
     Visitor                (pParams),
    _pParent                (NULL   ),
    _pChunkOverrideGroupNode(NULL   ) 
{
}

inline
OSGVisualUpdateVisitor::~OSGVisualUpdateVisitor(void)
{
}

inline
const char *OSGVisualUpdateVisitor::getClassName(void) const
{ 
    return "OSGVisualUpdateVisitor"; 
}

inline
void OSGVisualUpdateVisitor::setOSG2Parent(OSG::Node *pParent) 
{ 
    _pParent = pParent; 
}

inline
OSG::Node *OSGVisualUpdateVisitor::getOSG2Parent(void) const 
{
    return _pParent; 
}

inline
void OSGVisualUpdateVisitor::setChunkOverrideGroupNode(OSG::Node *pGp)
{
    _pChunkOverrideGroupNode = pGp; 
}

inline
OSG::Node *OSGVisualUpdateVisitor::getChunkOverrideGroupNode(void) const
{
    return _pChunkOverrideGroupNode; 
}




inline
OSGVisualInitVisitor::OSGVisualInitVisitor(const core::ExecParams *pParams) : 
    Visitor(params) 
{
}

inline
OSGVisualInitVisitor::~OSGVisualInitVisitor(void)
{
}

inline
const char *OSGVisualInitVisitor::getClassName(void) const 
{ 
    return "OSGVisualInitVisitor"; 
}

END_SOFA_SIM_NAMESPACE
