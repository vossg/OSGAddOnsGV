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

#include "OSGSofaShaderMacro.h"
#include <sofa/core/ObjectFactory.h>
namespace sofa
{

namespace component
{

namespace visualmodel
{

SOFA_DECL_CLASS(OSG2_ShaderDefineMacro)

//Register OSG2_IntVariable in the Object Factory
int OSG2_ShaderDefineMacroClass = core::RegisterObject("OSG2_ShaderDefineMacro")
.add< OSG2_ShaderDefineMacro >();

OSG2_ShaderMacro::OSG2_ShaderMacro()
{
    
}

OSG2_ShaderMacro::~OSG2_ShaderMacro()
{
}

void OSG2_ShaderMacro::init()
{
    OSG2_ShaderElement::init();
}

OSG2_ShaderDefineMacro::OSG2_ShaderDefineMacro()
: value(initData(&value, (std::string) "", "value", "Set a value for define macro"))
{
    
}

OSG2_ShaderDefineMacro::~OSG2_ShaderDefineMacro()
{
}

void OSG2_ShaderDefineMacro::init()
{
    std::cerr << " OSG2_ShaderDefineMacro::init()" << std::endl;
    OSG2_ShaderMacro::init();
    
    shader->addDefineMacro(indexShader.getValue(), id.getValue(), value.getValue());
}

}

}

}
