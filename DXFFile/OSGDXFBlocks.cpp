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

#include "OSGDXFBlocks.h"

#include "OSGTransform.h"
#include "OSGNameAttachment.h"
#include "OSGGeometry.h"
//#include <OSGSimpleAttachments.h>   // only for setName()

OSG_USING_NAMESPACE

//#undef FDEBUGx
//#define FDEBUGx(X) FDEBUG(X)

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFBlocks
    \ingroup GrpSystemFileIO

  Parses and dispatches the file section between the groups (0,SECTION),
  (2,BLOCKS) and (0,ENDSEC). Blocks subsections are dispatched to
  DXFBlock.

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
DXFBlocks *DXFBlocks::_the = new DXFBlocks();

/* \var VARTYPE DXFBlocks::_VARNAME
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

/*! Handle special dispatch case for BLOCKS section.
 *
 *  \dev This method is almost the same as DXFEntityBase::dispatch() besides
 *  that it dispatches a BLOCK directly without reading a name in a group with
 *  code 2. Thanks to the very very well designed DXF file format we need this
 *  distinction here :( \enddev 
 */
void DXFBlocks::dispatch(void)
{
    EntityTypeMap::iterator itr;

    // There must be a keyword!  // TODO: raus! DXFRecord::getValueType() < 0
    // wird schon in DXFEntityBase::read() gecheckt!!!
    if(DXFRecord::getValueStr().empty())
    {
        FWARNING(("DXF Loader: syntax error in line %d!\n",
                  DXFRecord::getLineNumber() + 1));
        _state = DXFStateError;
        FDEBUGx(("Line %d: %s:%s returns %s because of Record (%d|%s)\n",
                DXFRecord::getLineNumber(),
                getEntityClassName(),
                getEntityTypeName(),
                DXFResultNames[_state],
                DXFRecord::getGroupCode(),
                DXFRecord::getValueStr().c_str()
               ));
        return;
    }

    // Keyword ENDSEC for SECTIONs, TABLEs, BLOCKS, ...
    if(DXFRecord::getValueStr() == _parent->getChildEndKeyword())
    {
        FDEBUGx(("EndKeyword: %s\n", DXFRecord::getValueStr().c_str()));
        DXFRecord::unget();
        _state = DXFStateReturn;
        FDEBUGx(("Line %d: %s:%s returns %s because of Record (%d|%s)\n",
                 DXFRecord::getLineNumber(),
                 getEntityClassName(),
                 getEntityTypeName(),
                 DXFResultNames[_state],
                 DXFRecord::getGroupCode(),
                 DXFRecord::getValueStr().c_str()
                ));
        return;
    }

    if(DXFRecord::getValueStr() == _childBeginKeyword)  // == "BLOCK" !
    {
        itr = _childEntityTypes.find(DXFRecord::getValueStr());
        // unknown entity type?
        if(itr == _childEntityTypes.end())
        {
            FWARNING(("DXF Loader (%s:%s): in line %d: "
                      "%s '%s' not yet implemented. Ignoring!\n",
                      getEntityClassName(),
                      getEntityTypeName(),
                      DXFRecord::getLineNumber(),
                      _childBeginKeyword.c_str(),
                      DXFRecord::getValueStr().c_str()));
            _state = DXFStateIgnore;
            FDEBUGx(("(%s:%d) line %d: %s:%s returns %s because of Record (%d|%s)\n", 
                     __FILE__,
                     __LINE__,
                     DXFRecord::getLineNumber(),
                     getEntityClassName(),
                     getEntityTypeName(),
                     DXFResultNames[_state],
                     DXFRecord::getGroupCode(),
                     DXFRecord::getValueStr().c_str()
                    ));
            return;
        }

        // descent into the entity type hierarchy.
#if 1
        DXFEntityBase *entityBasePtr = dynamic_cast<DXFEntityBase *>(itr->second);
        entityBasePtr->read(this);
#else
        // doesn't work since read is protected in DFXEntityBase
        itr->second->DXFEntityBase::read(this);
#endif
        FDEBUGx(("(%s, %d) line %d: %s:%s returns %s because of Record (%d|%s)\n",
                 __FILE__, __LINE__,
                 DXFRecord::getLineNumber(),
                 getEntityClassName(),
                 getEntityTypeName(),
                 DXFResultNames[_state],
                 DXFRecord::getGroupCode(),
                 DXFRecord::getValueStr().c_str()
                ));
        return;
    }
        
    // Eat up the END* keyword, which has been put back by a child entity, or
    // start reading again after the end of an unknown high level entity's
    // child.
    if(DXFRecord::getValueStr() == _childEndKeyword)
    {
        _state = DXFStateContinue;
        FDEBUGx(("(%s, %d) line %d: %s:%s returns %s because of Record (%d|%s)\n",
                 __FILE__, __LINE__,
                DXFRecord::getLineNumber(),
                getEntityClassName(),
                getEntityTypeName(),
                DXFResultNames[_state],
                DXFRecord::getGroupCode(),
                DXFRecord::getValueStr().c_str()
               ));
        return;
    }

    if(_state == DXFStateIgnore)
    {
        FDEBUGx(("(%s, %d) line %d: %s:%s returns %s because of Record (%d|%s)\n",
                 __FILE__, __LINE__,
                DXFRecord::getLineNumber(),
                getEntityClassName(),
                getEntityTypeName(),
                DXFResultNames[_state],
                DXFRecord::getGroupCode(),
                DXFRecord::getValueStr().c_str()
               ));
        return;
    }
    
    // Something went wrong
    _state = DXFStateError;
    FWARNING(("DXF Loader: syntax error in line %d, reported by %s:%s, "
              "current record is (%d|%s). "
              "May have happened already before. Giving up! (return = %s)\n",
              DXFRecord::getLineNumber(),
              getEntityClassName(),
              getEntityTypeName(),
              DXFRecord::getGroupCode(),
              DXFRecord::getValueStr().c_str(), 
              DXFResultNames[_state]
             ));
    return;
}  // DXFBlocks::dispatch()

/*! Evalute records for BLOCKS section with the following group codes:
 * (currently none).
 */
DXFResult DXFBlocks::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        default:
            state = DXFStateUnknown;
    }
    if(state == DXFStateUnknown)
        state = Inherited::evalRecord();
    return state;
}

DXFResult DXFBlocks::beginEntity(void)
{
    DXFResult state;
    state = Inherited::beginEntity();
    
    return state;
}

DXFResult DXFBlocks::endEntity(void)
{
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

 /*! Add SECTION:BLOCKS entity to DXF hierarchy as child of FILE:FILE (see
  *  DXFFile)
  */    
DXFBlocks::DXFBlocks(void) :
    Inherited()
{
    _entityClassName   = "SECTION";
    _entityTypeName    = "BLOCKS";

    _childBeginKeyword = "BLOCK";
    _childEndKeyword   = "ENDBLK";  

    registerToParentEntityType("FILE:FILE");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFBlocks::~DXFBlocks(void)
{
}

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFBlock
    \ingroup GrpSystemFileIO

  Parses and evaluates the file subsection inside a BLOCK between the groups
  (0,BLOCK) and (0,ENDBLK).

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
DXFBlock *DXFBlock::_the = new DXFBlock();

/* \var VARTYPE DXFBlock::_VARNAME
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

/*! Returns pointer to OpenSG Transform Node representing the current BLOCK.
 */
Node *DXFBlock::getNode(void)
{
    return _blockNodeP;
}

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/

/*! Evalute records for BLOCK entities with the following group codes:
 * - 8 -- Layer name
 * - 2, 3 -- Block name (can't guess, why there are 2 entries for that in DXF)
 * - 10, 20, 30 -- Base point X,Y and Z value
 */
DXFResult DXFBlock::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 8:
            _layerName = DXFRecord::getValueStr();
            break;
        case 2:
        case 3:
            // TODO: check, whether both name entries for the block are
            // identical? What if not? ERROR?
            _blockName = DXFRecord::getValueStr();
			if(_blockName == std::string("Rectangular Mullion - 64 x 128 rectangular-V2-Level 1"))
			{
				std::cout << "here" << std::endl;

			}
            break;
        case 10:
            _basePoint[0] = DXFRecord::getValueDbl();
            break;
        case 20:
            _basePoint[1] = DXFRecord::getValueDbl();
            break;
        case 30:
            _basePoint[2] = DXFRecord::getValueDbl();
            break;          
        default:
            state = DXFStateUnknown;
    }
    if(state == DXFStateUnknown)
        state = Inherited::evalRecord();
    return state;
}

/*! Set up new Transform (Group) Node for starting block.
 */
DXFResult DXFBlock::beginEntity(void)
{
    DXFResult state;
    state = Inherited::beginEntity();
    if(_blockName == std::string("Rectangular Mullion - 64 x 128 rectangular-V2-Level 1"))
	{
		std::cout << "here" << std::endl;

	}
    // clear DXFBlocks data
    _layerName = "";
    _blockName = "";
    _basePoint.setNull();

    _blockNodeP               = Node::create(); // TODO: check for success! !=
                                                // NullFC!
    TransformUnrecPtr trfCore = Transform::create();
    
#if 0
    beginEditCP(_blockNodeP);
#endif
    _blockNodeP->setCore(trfCore);

#if 0
    addRefCP(_blockNodeP);
#endif

    return state;
}

/*! Complete Node representing the block from the fetched data.
 * \todo set trafo to _basePoint[]
 */
DXFResult DXFBlock::endEntity(void)
{
    StringToNodePtrMap::iterator itr = _blocksMapP->begin();

    setName(_blockNodeP, _entityTypeName + "=" + _blockName);
	if(_blockName == std::string("Rectangular Mullion - 64 x 128 rectangular-V2-Level 1"))
	{
		std::cout << "here" << std::endl;
		std::cout << _blockNodeP->getNChildren() << std::endl;
		for(int i=0;i<_blockNodeP->getNChildren();i++)
		{
			OSG::Node* p = _blockNodeP->getChild(i);
			std::string name = OSG::getName(p);
			std::cout << name << std::endl;
			 Geometry* geoCore = dynamic_cast<Geometry *>(p->getCore());
			 GeoPnt3dProperty *pointsP =  
			  dynamic_cast<GeoPnt3dProperty *>(geoCore->getPositions());
			 for(int i=0;i<pointsP->size();i++)
			 {
				 std::cout << pointsP->getValue(i) << std::endl;
			 }
			 Transform* transformCore = dynamic_cast<Transform *>(_blockNodeP->getCore());
			 std::cout << transformCore->getMatrix() << std::endl;

		}

	}
    while(itr != _blocksMapP->end())
    {
        itr = _blocksMapP->find(_blockName);
        if( itr != _blocksMapP->end())
        {   
            _blockName += _handle;
            FWARNING(("DXF Loader: in line %d: "
                      "block with same name '%s' already exists! "
                      "Renaming the new one to '%s' "
                      "(won't be able to access it for referencing though)\n",
                      DXFRecord::getLineNumber(),
                      itr->first.c_str(),
                      _blockName.c_str()
                     ));
        }
    }
    (*_blocksMapP)[_blockName] = _blockNodeP;

    FDEBUGx(("DXFBlocks::endEntity(): BLOCK '%s' in map and is stored as '%s'.\n",
            getName((*_blocksMapP)[_blockName]),
            _blockName.c_str()));

    // TODO: set transformation to _basePoint[] ?!?!?

#if 0
    endEditCP(_blockNodeP);
#endif

    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add BLOCKSENTRY:BLOCK entity to DXF hierarchy as child of SECTION:BLOCKS
 *  (see DXFBlocks).
 */
DXFBlock::DXFBlock(void) :
    Inherited(),
    _layerName(""),
    _blockName(""),
    _basePoint(0.0, 0.0, 0.0),
    _blockNodeP(NULL)
{
    _entityClassName = "BLOCKSENTRY";
    _entityTypeName  = "BLOCK";

    registerToParentEntityType("SECTION:BLOCKS");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFBlock::~DXFBlock(void)
{
}
