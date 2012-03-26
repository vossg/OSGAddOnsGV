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

#ifndef _OSGSOFASHADERMACRO_H_
#define _OSGSOFASHADERMACRO_H_

#include "OSGSofaShader.h"

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_ShaderMacro : public OSG2_ShaderElement {

  public:

    SOFA_CLASS(OSG2_ShaderMacro, OSG2_ShaderElement);

  protected:

    OSG2_ShaderMacro();
    virtual ~OSG2_ShaderMacro();

  public:
    virtual void init();

};


class OSG_CONTRIBSOFA_DLLMAPPING OSG2_ShaderDefineMacro : public OSG2_ShaderMacro {

  public:

    SOFA_CLASS(OSG2_ShaderDefineMacro, OSG2_ShaderMacro);

  protected:

    Data<std::string> value;

  public:

    OSG2_ShaderDefineMacro();
    virtual ~OSG2_ShaderDefineMacro();

    virtual void init();

};

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFASHADERMACRO_H_
