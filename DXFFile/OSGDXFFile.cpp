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

#include <iostream>

#include "OSGConfig.h"

#include "OSGDXFFile.h"
#include "OSGDXFRecord.h"
#include "OSGDXFEntityBase.h"

#include "OSGNode.h"
#include "OSGGroup.h"

//#include <OSGSimpleAttachments.h>


OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFFile
    \ingroup GrpSystemFileIO

  Starts the parsing and dispatching of the DXF file and puts all built OpenSG
  data together finally.

 */

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/*! Pointer to singleton of this class. The instance of this class will be
 *  constructed in registerEntityType()! 
 */
DXFFile *DXFFile::_the = NULL;

//! Handles file name expansion using a search path.
PathHandler DXFFile::_pathHandler;

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/* load interface
 * @todo Evaluate options, see ScanParseSkel...
 * This interface will change due to AMZ modifications
 * @todo Option to trigger cleanup/free of allocated memory? May be faster
 * sometimes when you load many files not to clean up after every file but only
 * after loading all!
 */
NodeTransitPtr DXFFile::load(std::istream &is)
{
	Time startTime = getSystemTime();

	if ( ! _the->_entityTypeHierarchy.empty() )
	{
		FWARNING(("DXFFile::load(): BUG! "
				  "DXF entity structure not completely resolved! "
				  "Unknown entity types will be ignored!\n"));
		for(StringToStringMMap::iterator itr = 
				_the->_entityTypeHierarchy.begin();
			itr != _the->_entityTypeHierarchy.end();
			++ itr)
		{
			FWARNING(("     unresolved: %s <-- %s\n",
					  itr->first.c_str(), itr->second.c_str()
					 ));
		}	
	}

    if(is)
    {
        if(DXFRecord::setStream(is))
        {	
            _the->read( NULL );
        }	
    }
    
#if 0 //TODO: raus!
    if(szFilename != NULL)
    {
        std::string tmpName;

        _pathHandler.setBaseFile(szFilename);

        tmpName = _pathHandler.findFile(szFilename);

        if(tmpName.size() != 0)
        {
			if(DXFRecord::openStream(tmpName.c_str()))
			{	
				_the->read( NULL );
				DXFRecord::closeStream();
			}			
        }
    }
#endif
    
    fprintf(stderr, "Full Time : %lf\n", 
            getSystemTime() - startTime);
	
    NodeTransitPtr returnValue(_rootNodeP);

    _the->cleanup();

	return returnValue;
}

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/*! An entity type may be registered as child of more than one parent entity
 * type, e.g. VERTEX, which is child of 3DMESH, POLYLINE and many others.
 *
 * \pre entityType is a valid pointer != 0
 */
void DXFFile::registerEntityType( const Char8 *entityParentTypeName,
								  DXFEntityBase *entityType )
{
	StringToStringMMap::iterator  hmItr;

	// The ugly concept of entityTypeName, entityClassName and entityUniqueName
	// was introduced because of the **** "feature" of the DXF file format to
	// have pairs of different entities having the same keywords: there is a
	// TABLES entry named LAYER, thus a LAYERs table, and ther is the entry to
	// that table, the LAYER itself. The same **** holds for all other TABLE
	// types!

	// only for better readibility
	Char8 const * const entityTypeName  = entityType->getEntityTypeName();
	Char8 const * const entityClassName = entityType->getEntityClassName();
	const std::string entityUniqueName(std::string() + 
									   entityClassName + ":" + entityTypeName);

	if(_the == NULL)
	{
		// The constructor of DXFFile must be the only one of all instantiable
		// classes derived from DXFEntityBase, which does *NOT* call
		// DXFEntityBase::registerToParentEntity()!
		_the = new DXFFile();
	}

	// search for new entityType and add it if unknown. 
	EntityTypeMap::iterator itr = 
		_the->_entityTypes.find(entityUniqueName);
	if (itr == _the->_entityTypes.end())
		_the->_entityTypes[entityUniqueName] = entityType;

#if 1 // DEBUG
	for ( itr =  _the->_entityTypes.begin();
		  itr != _the->_entityTypes.end();
		  ++ itr)
	{
		FDEBUGx(("DXFFile::registerEntityType(%s(%p))\n", 
				itr->first.c_str(), itr->second
			   ));
	}
#endif

	if(entityType != _the) // (only) DXFFile has no parent entity type, thus
						   // ignore it!
	{
		// search for entityParentType
		EntityTypeMap::iterator itr = 
			_the->_entityTypes.find(entityParentTypeName);
		if (itr == _the->_entityTypes.end())
		{
			FDEBUGx(("DXFFile::registerEntityType: "
					"Parent Entity '%s' for Entity type '%s' not found. "
					"Storing for unresolved family\n",
					entityParentTypeName,
					entityTypeName));
			// Entity parent entity not found: the registration to the
			// respective entity parent type has to be deferred until the
			// parent type will be registered. Store the registration request.
			_the->_entityTypeHierarchy.insert(
				std::pair<const std::string, std::string>(entityParentTypeName,
													entityUniqueName)
											 );
		}
		else
		{			
			itr->second->_childEntityTypes[entityTypeName]
				= entityType;
			FDEBUGx(("DXFFile::registerEntityType: "
					"inserted entity type %s(%p) to parent type %s(%p).\n",
					itr->second->_childEntityTypes[entityTypeName]->getEntityTypeName(),
					itr->second->_childEntityTypes[entityTypeName],
					itr->second->getEntityTypeName(),
					itr->second
				   ));
			FDEBUGx(("DXFFile::registerEntityType: "
					"%s(%p)->_childEntityTypes.size() = %d\n",
					itr->second->getEntityTypeName(),
					itr->second,
					itr->second->_childEntityTypes.size()
				   ));
		}
	}

	// Resolve older registration requests for the added entity type
	// (entityUniqueName now has the role of a entityParentTypeName) and remove
	// it from unresolved list.

	std::pair<StringToStringMMap::iterator, StringToStringMMap::iterator>
		range = _the->_entityTypeHierarchy.equal_range(entityUniqueName);

	for( hmItr =  range.first;
		 hmItr != range.second;
		 ++ hmItr )
	{
		DXFEntityBase *childEntityType = 
			_the->_entityTypes[hmItr->second];
		Char8 const * const childEntityTypeName = 
			childEntityType->getEntityTypeName();
		
		_the->_entityTypes[hmItr->first]
			->_childEntityTypes[childEntityTypeName] = childEntityType;
	}
	_the->_entityTypeHierarchy.erase(range.first, range.second);
}

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/

/*! DXFFile currently has no group codes to evaluate.
 */
DXFResult DXFFile::evalRecord(void)
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

/*! Initialize root Node
 * @todo
 * Does the scene file root get's the filename as NodeName?
 */

DXFResult DXFFile::beginEntity()
{
	_rootNodeP            = Node::create();
	GroupUnrecPtr groupP  = Group::create();

    _rootNodeP->setCore(groupP);
    //setName(_rootNodeP, szFilename);

	if(_linetypeMapP == NULL)
	{
		_linetypeMapP = new StringToLineChunkPtrMap();
	}
	else
	{
		_linetypeMapP->clear();
	}
	
	if(_layersMapP == NULL)
	{
		_layersMapP = new StringToNodePtrMap();
	}
	else
	{
		_layersMapP->clear();
	}

	FDEBUGx(("DXFFile::beginEntity(): _layersMapP = %p\n", 
			_layersMapP));
	
	if(_blocksMapP == NULL)
	{
		_blocksMapP = new StringToNodePtrMap();
	}
	else
	{
		_blocksMapP->clear();
	}
	
	return DXFStateContinue;
}

/*! Put everything together and attach to root node.
 * @todo
 */

DXFResult DXFFile::endEntity()
{
	StringToNodePtrMap::iterator itr;

	FDEBUGx(("DXFFile::endEntity(): _layersMapP = %p\n", 
			_layersMapP));

	for(itr  = _layersMapP->begin();
		itr != _layersMapP->end();
		++ itr)
	{
		FDEBUGx(("DXFFile::endEntity(): Adding LAYER %s to root node.\n",
				itr->first.c_str()));

		_rootNodeP->addChild(itr->second);
	}

	FDEBUGx(("DXFFile::endEntity(): _blocksMapP = %p\n", 
			_blocksMapP));

	for(itr  = _blocksMapP->begin();
		itr != _blocksMapP->end();
		++ itr)
	{
		FDEBUGx(("DXFFile::endEntity(): Have  BLOCK %s stored...\n",
				itr->first.c_str()));
	}


	return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Initialize FILE:FILE as root of the DXF hierarchy.
 */
DXFFile::DXFFile(void) :
	 Inherited          (),
	_entityTypes        (),
	_entityTypeHierarchy()
{
	_entityClassName   = "FILE";
	_entityTypeName    = "FILE";

	_childBeginKeyword = "SECTION";
	_childEndKeyword   = "ENDSEC";
	
	// We need to do the relevant parts of registerToParentEntityType("FILE");
	// by hand here to avoid a infinite constructor call recursion.  DXFFile
	// has no parentEntityType and furthermore *DXFFile::_the will be
	// instantiated *before* all other entityTypes, thus we only need to put it
	// into the _entityTypes.
	std::string entityUniqueName =
		std::string() + getEntityClassName() + ":" + getEntityTypeName();
	_entityTypes[entityUniqueName] = this;
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */

DXFFile::~DXFFile(void)
{
}
