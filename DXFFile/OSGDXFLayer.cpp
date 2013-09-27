/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2003 by the OpenSG Forum                 *
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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include "OSGConfig.h"

#include "OSGDXFLayer.h"

#include "OSGMaterialGroup.h"
#include "OSGSimpleMaterial.h"
#include "OSGNameAttachment.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFLayer
    \ingroup GrpSystemFileIO

  Parses and evaluates the file subsection inside a LAYER table between the
  group (0,LAYER) and any other group with group code 0.

 */

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/*! Pointer to singleton of this class. There is exactly one instance for each
 * derived class which can be instantiated.
 */
DXFLayer *DXFLayer::_the = new DXFLayer();

/* \var VARTYPE DXFLayer::_VARNAME
    variable documentation
 */

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/

/*! Evaluate records for LAYER table entries with the following group codes:
 * - 62 -- Color number
 * - 6 -- Linetype name
 */
DXFResult DXFLayer::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 62:
            _colorNumber = DXFRecord::getValueInt();
            break;
        case 6:
            _linetypeName = DXFRecord::getValueStr();
            break;          
        default:
            state = DXFStateUnknown;
    }
    if(state == DXFStateUnknown)
        state = Inherited::evalRecord();
    return state;
}

/*! Clear internal data.
 */
DXFResult DXFLayer::beginEntity(void)
{
    DXFResult state;
    state = Inherited::beginEntity();

    _colorNumber = 0;
    _linetypeName = "";

    return state;
}

/*! Create MaterialGroup Node from fetched data. The corresponding LineChunk
 * (created by DXFLtype) is attached.
 */
DXFResult DXFLayer::endEntity(void)
{
    // find LTYPE for layer
    LineChunkUnrecPtr ltype = NULL;
    StringToLineChunkPtrMap::iterator itr = _linetypeMapP->find(_linetypeName);
    if(itr!=_linetypeMapP->end())
        ltype = (*itr).second;

    // prepare material
    SimpleMaterialUnrecPtr mtrl = SimpleMaterial::create();
#if 0
    beginEditCP(mtrl);
#endif
    {
        mtrl->setDiffuse(getColor(_colorNumber));
        mtrl->setLit(false);
        if (ltype != NULL)
            mtrl->addChunk(ltype);
    }
#if 0
    endEditCP(mtrl);
#endif
	
	std::cout << _colorNumber << "  "<< _name << std::endl;
    // put layer group together
    MaterialGroupUnrecPtr mgrp=MaterialGroup::create();

#if 0
    beginEditCP(mgrp);
#endif
    {
        mgrp->setMaterial(mtrl);
    }
#if 0
    endEditCP  (mgrp);
#endif

    NodeUnrecPtr layer = Node::create();
#if 0
    beginEditCP(layer, Node::CoreFieldMask);
#endif
    {
        layer->setCore(mgrp);
    }
#if 0
    endEditCP  (layer, Node::CoreFieldMask);
#endif
    
    if(_name.empty())
        _name="<unnamed>"; // Never push key values into the map with invalid
                           // c_str()!
    
    FDEBUGx(("OSGDXFLayer: adding layer '%s'\n",    
            _name.c_str()));

    setName(layer, _entityTypeName + "=" + _name);

    (*_layersMapP)[_name] = layer;
    
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add TABLEENTRY:LAYER entity to DXF hierarchy as child of TABLE:LAYER (see
 *  DXFTable<>).
 */
DXFLayer::DXFLayer(void) :
    Inherited(),
    _colorNumber(0),
    _linetypeName()
{
    _entityTypeName  = "LAYER";

    registerToParentEntityType("TABLE:LAYER");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFLayer::~DXFLayer(void)
{
}

