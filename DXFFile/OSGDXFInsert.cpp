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

#include "OSGDXFInsert.h"

#include <OSGComponentTransform.h>

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFInsert
    \ingroup GrpSystemFileIO

  Parses and evaluates the file subsection inside a INSERT between the
  group (0,INSERT) and any other group with group code 0.

 */

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

/*! \enum DXFInsert::ENUMNAME
  
 */

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/*! Pointer to singleton of this class. There is exactly one instance for each
 * derived class which can be instantiated.
 */
DXFInsert *DXFInsert::_the = new DXFInsert();

/* \var VARTYPE DXFInsert::_VARNAME
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

/*! Evalute records for INSERT entities with the following group codes:
 *  - 2 -- Block name
 *  - 10, 20, 30 -- Insertion point X, Y and Z values
 *  - 41, 42, 43 -- X, Y and Z scale factor 
 *  - 50 -- Rotation angle
 *  - 70 -- Column count
 *  - 71 -- Row count
 *  - 44 -- Column spacing
 *  - 45 -- Row spacing
 *  - 210, 220, 230 -- Extrusion direction
 */
DXFResult DXFInsert::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 2:
            _blockName = DXFRecord::getValueStr();
            break;
        case 10:
            _insertionPoint[0] = DXFRecord::getValueDbl();
            break;
        case 20:            
            _insertionPoint[1] = DXFRecord::getValueDbl();
            break;
        case 30:            
            _insertionPoint[2] = DXFRecord::getValueDbl();
            break;
        case 41:            
        case 42:            
        case 43:            
            _scaleFactor[DXFRecord::getGroupCode()-41] = DXFRecord::getValueDbl();
            break;
        case 50:            
            _rotationAngle = DXFRecord::getValueDbl();
            break;
        case 70:            
            _columnCount = DXFRecord::getValueInt();
            break;
        case 71:            
            _rowCount = DXFRecord::getValueInt();
            break;
        case 44:            
            _columnSpacing = DXFRecord::getValueDbl();
            break;
        case 45:            
            _rowSpacing = DXFRecord::getValueDbl();
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
DXFResult DXFInsert::beginEntity(void)
{
    DXFResult state;
    state = Inherited::beginEntity();
    
    _insertionPoint.setNull();
    _scaleFactor.setValues(1.0, 1.0, 1.0);
    _rotationAngle = 0.0;
    _columnCount = 1;
    _rowCount = 1;
    _columnSpacing = 0.0;
    _rowSpacing = 0.0;
    
    return state;
}

/* Insert one or more shallow copies of a block (created by DXFBlock as
 * Transform Group) into a layer (created by DXFLayer as MaterialGroup) or
 * another block.
 * \todo 
 * Could there be a INSERT inside a block referring to another block which has
 * not been read yet? We then have to find a solution to enable deferred
 * instantiation of INSERT entities :-(
 */
DXFResult DXFInsert::endEntity(void)
{
    NodeUnrecPtr                ctrafoNodeP = NULL;
    ComponentTransformUnrecPtr  ctrafoCoreP = NULL;
    NodeUnrecPtr                blockNodeP  = NULL;

    Node                       *parentNodeP = getParentNode();
    
    StringToNodePtrMap::iterator itr = _blocksMapP->find(_blockName);
    if (itr != _blocksMapP->end() && parentNodeP != NULL)
    {
        blockNodeP = itr->second;
        // TODO: check fetched INSERT Data for consistency!

        // Insert multiple times in a grid...
        Vec3f offset(0.0, 0.0, 0.0);
        for(Int16 column = 0; column < _columnCount; ++ column)
        {
            offset[0] = column * _columnSpacing;
            for(Int16 row = 0; row < _rowCount; ++ row)
            {
                offset[1] = row * _rowSpacing;
                // TODO: find out about DXF insert semantics!

                ctrafoNodeP = Node::create();
                ctrafoCoreP = ComponentTransform::create();
                
#if 0
                beginEditCP(ctrafoCoreP);
#endif
                {
					OSG::Quaternion rot(OSG::Vec3f(0,0,1),OSG::Pi * _rotationAngle/180);
					ctrafoCoreP->setRotation(rot);
                    ctrafoCoreP->setTranslation(_insertionPoint + offset);
                    ctrafoCoreP->setScale(_scaleFactor);
					
                }
#if 0
                endEditCP(ctrafoCoreP);
#endif
#if 0
                beginEditCP(ctrafoNodeP);
#endif
                {
                    ctrafoNodeP->setCore(ctrafoCoreP);
#if 0
                    ctrafoNodeP->addChild(blockNodeP->clone());
#endif
                    NodeUnrecPtr pClone = cloneTree(blockNodeP);
                    ctrafoNodeP->addChild(pClone);
                }
#if 0
                endEditCP(ctrafoNodeP);
#endif                
#if 0
                beginEditCP(parentNodeP);
#endif
                {
                    parentNodeP->addChild(ctrafoNodeP);
                }
#if 0
                endEditCP(parentNodeP);
#endif
            }
        }

        // Warn for details not implemented or assured yet! TODO: better
        // implement missing features!
        
        /*if(fabs(_rotationAngle) > Eps)
            FWARNING(("DXF Loader: before line %d: "
                      "DXFInsert does not yet support ROTATION "
                      "(group code 50). "
                      "Most likely the graphics are incorrect!\n",
                      DXFRecord::getLineNumber()
                     ));*/
        if(_scaleFactor != Vec3f(1.0,1.0,1.0))
            FWARNING(("DXF Loader: before line %d: "
                      "DXFInsert may not interpret SCALING "
                      "(group codes 41, 42, 43) correctly."
                      "Graphics may be incorrect!\n",
                      DXFRecord::getLineNumber()
                     ));
        
        if(_columnCount != 1 || _rowCount != 1)
            FWARNING(("DXF Loader: before line %d: "
                      "DXFInsert may not interpret REPEATED INSERTION " 
                      "(group codes 70, 71, 44, 45) correctly."
                      "Graphics may be incorrect!\n",
                      DXFRecord::getLineNumber()
                     ));
        
    }
    else
    {
        if(itr == _blocksMapP->end())
            FWARNING(("DXF Loader: before line %d (inside %s section): "
                  "BLOCK '%s' to be inserted not found!\n",
                  DXFRecord::getLineNumber(),
                  _parent->getEntityTypeName(),
                  _blockName.c_str()
                  ));
        if(parentNodeP == NULL)
            FWARNING(("DXF Loader: before line %d (inside %s section): "
                  "layer %s to be inserted to not found!\n",
                  DXFRecord::getLineNumber(),
                  _parent->getEntityTypeName(),
                  _layerName.c_str()
                  ));
    }
    
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add ENTITY:INSERT entity to DXF hierarchy as child of SECTION:ENTITIES (see
 *  DXFEntities) and BLOCKSENTRY:BLOCK (see DXFBlock).
 */
DXFInsert::DXFInsert(void) :
    Inherited(),
    _insertionPoint(0.0, 0.0, 0.0),
    _scaleFactor(1.0, 1.0, 1.0),
    _rotationAngle(0.0),
    _columnCount(1),
    _rowCount(1),
    _columnSpacing(0.0),
    _rowSpacing(0.0)
{
    _entityTypeName  = "INSERT";

    registerToParentEntityType("SECTION:ENTITIES");
    registerToParentEntityType("BLOCKSENTRY:BLOCK");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFInsert::~DXFInsert(void)
{
}
