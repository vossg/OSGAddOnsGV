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

#ifndef _OSGSOFASHADOWSHADER_H_
#define _OSGSOFASHADOWSHADER_H_

#include <sofa/component/visualmodel/OglShader.h>

// OpenSG
#include "OSGContribSOFADef.h"
#include "OSGShaderProgramChunk.h"

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_ShadowShader : public sofa::component::visualmodel::OglShader {
  public:
    SOFA_CLASS(OSG2_ShadowShader, sofa::component::visualmodel::OglShader);
  protected:
    OSG2_ShadowShader();
    virtual ~OSG2_ShadowShader();
  public:
    void init( void );

    void initShaders(unsigned int numberOfLights, bool softShadow);

    void start( void );
    void stop( void );
    bool isActive( void );

  protected:
    static const std::string PATH_TO_SHADOW_VERTEX_SHADERS;
    static const std::string PATH_TO_SHADOW_FRAGMENT_SHADERS;
    static const std::string PATH_TO_SOFT_SHADOW_VERTEX_SHADERS;
    static const std::string PATH_TO_SOFT_SHADOW_FRAGMENT_SHADERS;

    //OpenSG
    OSG::ShaderProgramRecPtr _shaderChunk;



};

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFASHADOWSHADER_H_
