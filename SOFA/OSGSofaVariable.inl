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

#ifndef SOFA_COMPONENT_VISUALMODEL_OSGVARIABLE_INL
#define SOFA_COMPONENT_VISUALMODEL_OSGVARIABLE_INL

#include "OSGSofaVariable.h"
#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace visualmodel
{

//
//template<class DataTypes>
//OSGVariable<DataTypes>::OSGVariable()
//: value(initData(&value, DataTypes(), "value", "Set Uniform Value"))
//{
//    addAlias(&value, "values"); // some variable types hold multiple values, so we authorize both names for this attribute
//}
//
//template<class DataTypes>
//OSGVariable<DataTypes>::~OSGVariable()
//{
//}
//
//template<class DataTypes>
//void OSGVariable<DataTypes>::init()
//{
//    OSGShaderElement::init();
//}
//
//template<class DataTypes>
//void OSGVariable<DataTypes>::initVisual()
//{ 
//    core::visual::VisualModel::initVisual();
//}
//
//template<class DataTypes>
//void OSGVariable<DataTypes>::reinit()
//{
//    init();
//    initVisual();
//}

} // namespace visualmodel

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_VISUALMODEL_OGLVARIABLE_H
