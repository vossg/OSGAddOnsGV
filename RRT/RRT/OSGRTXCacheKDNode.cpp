/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *           Copyright (C) 2003 by the OpenSG Forum                          *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 *   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
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

#include "OSGRTXCacheKDNode.h"

#include <OSGMField.h>
#include <OSGBaseMFields.h>

OSG_BEGIN_NAMESPACE

template <class ValueT, Int32 iNamespace, class AllocT>
FieldType MField<ValueT, iNamespace, AllocT>::_fieldType(
    MFieldTraits::getMName(),
    MFieldTraits::getPName(),
    MFieldTraits::getType (),
    FieldType::MULTI_FIELD,
    MFString::getClassType());

DataType FieldTraits<RTXCacheKDNode>::_type("RTXCacheKDNode", 
                                            "BaseType"      );

OSG_FIELDTRAITS_GETTYPE(RTXCacheKDNode)

OSG_FIELD_DLLEXPORT_DEF1(MField, RTXCacheKDNode)

OSG_END_NAMESPACE