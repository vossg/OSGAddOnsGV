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
#include "OSGLog.h"

#include "OSGDXFEntityBase.h"
#include "OSGDXFFile.h"

OSG_USING_NAMESPACE

//#undef FDEBUGx
//#define FDEBUGx(X) FDEBUG(X)

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFEntityBase 
    \ingroup GrpSystemFileIO
 
  The read() and dispatch() methods provide the intelligence to deal with the
  sophisiticated - or however you like to call it - AutoCAD DXF file structure.
 
  This class furthermore implements evalRecord(). Derived classes are
  DXF<EntityType> classes and DXF<EntityType>Entry classes. All of them
  reimplement evalRecord() which *must* call the same method of their parent
  classes (by Inherited::evalRecord()) for records they couldn't evaluate
  themselves.
 
  With beginEntity() the data structures for the current entity are prepared
  (be sure to call Inherited::beginEntity() inside this method!). The method
  evalRecord() is responsible for storing relevant DXF data in the internal
  data structures of the classes which implement them. The method endEntity()
  translates the fetched DXF data to OpenSG data structures.
 
  <EntityType> structures in the DXF file may contain subsections which
  represent other <EntityType>s: the ENTITES entity contains POLYLINE
  subsections for example. For easier understanding, I stay at the example:
  DXFEntities will implement the ENTITIES section of the DXF file
  structure. All structures which may be found in this section are derived from
  the class DXFEntitiesEntry, as for example DXFPolyline, which reads a
  POLYLINE entity. This hierarchical class relationship should be used where
  ever there are "Common Group Codes" for a set of DXF entities.

  \todo
  Check for "memory leaks": all classes derived of DXFEntityBase are
  singletons which are instantiated statically, no destructors are called after
  loading where you could ensure that dynamically allocated memory is
  freed. Thus maybe there should be a "clear" method which could be called
  explicitly when all loading with the DXF Loader is done.

  \todo
  Check conformance to AutoCAD semantics. At some places, different coordinate
  systems may be interpreted badly (what sematics do combinations of
  translation, scaling, rotation have?). Some Features are missing in the
  implemented entities: the extrusion functionality (using the parameter
  "Thickness"?) is not implemented at all!
 */

// TODO: only for debugging?!?
//! String representation of DXFResult codes.

namespace
{
    Char8 const * const resultnames[]=
    {
        "ERROR",
        "CONTINUE",
        "RETURN",
        "UNKNOWN",
        "IGNORE"
    };
}

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

Char8 const * const * const DXFEntityBase::DXFResultNames = &resultnames[1];

//! Options for file loading. Not used yet. (TODO)
UInt32                     DXFEntityBase::_options           = 0;

DXFResult                  DXFEntityBase::_state             = DXFStateContinue;

NodeUnrecPtr               DXFEntityBase::_rootNodeP         = NULL;
NodeUnrecPtr               DXFEntityBase::_activeNodeP       = NULL;

GeoPnt3dPropertyUnrecPtr   DXFEntityBase::_pointsP           = NULL;
MFPnt3d                   *DXFEntityBase::_pointsF           = NULL;
MFPnt3d::iterator          DXFEntityBase::_pointsIt;

GeoUInt32PropertyUnrecPtr  DXFEntityBase::_pointIndicesP     = NULL;
MFUInt32                  *DXFEntityBase::_pointIndicesF     = NULL;
MFUInt32::iterator         DXFEntityBase::_pointIndicesIt;

GeoUInt32PropertyUnrecPtr  DXFEntityBase::_faceLengthP       = NULL;
MFUInt32                  *DXFEntityBase::_faceLengthF       = NULL;
MFUInt32::iterator         DXFEntityBase::_faceLengthIt;

GeoUInt8PropertyUnrecPtr   DXFEntityBase::_faceTypeP         = NULL;
MFUInt8                   *DXFEntityBase::_faceTypeF         = NULL;
MFUInt8::iterator          DXFEntityBase::_faceTypeIt;

UInt32                     DXFEntityBase::_uiNumFacePoints   = 0;
Int32                      DXFEntityBase::_uiCurrentFaceType = 0;

StringToLineChunkPtrMap   *DXFEntityBase::_linetypeMapP      = NULL;
StringToNodePtrMap        *DXFEntityBase::_layersMapP        = NULL;
StringToNodePtrMap        *DXFEntityBase::_blocksMapP        = NULL;


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

/*! Return name of entity type (::_entityTypeName).
 */

const Char8 *DXFEntityBase::getEntityTypeName(void)
{
    return _entityTypeName.c_str();
}

/*! Return name of class (::_entityClassName) this entity type belongs to.
 */
const Char8 *DXFEntityBase::getEntityClassName(void)
{
    return _entityClassName.c_str();
}

/*=============================== PROTECTED ===============================*/

/*! Register this entity type as child for it's parent entity types.
 */

void DXFEntityBase::registerToParentEntityType(
    const Char8 *parentEntityTypeName)
{
    DXFFile::registerEntityType(parentEntityTypeName,
                                this);
}

/*! Free all memory allocated to class variable pointers. To be called after
 * all loading is done.
 */

void DXFEntityBase::freeMemory(void)
{
    if ( _layersMapP != NULL )
    {
        delete _layersMapP;
        _layersMapP = NULL;
    }
    if ( _blocksMapP != NULL )
    {
        delete _blocksMapP;
        _blocksMapP = NULL;
    }
}


/*! Return _childEndToken, e.g. "ENDSEC" for a SECTION entry entity.
 */

const std::string &DXFEntityBase::getChildEndKeyword(void)
{
    return _childEndKeyword;
}

/*! Calculate RGB values from colorNumber and return them as Color3f.
 *  \dev The color is calculated thus that one channel has a value l, another
 *  one 1-l and the third one is 0. \enddev
 *
 *  \todo This method can be made more customizable, e.g. for the use of a
 *  color table. Should we handle a negative colorNumber, which has the meaning
 *  of an invisible layer in DXF, differently than done here?
 */
Color3f DXFEntityBase::getColor(Int16 colorNumber)
{
    Color3f color;  
    const int NCOLORS = 256;    // change, when more colors are wanted
    int a=0;
    int b=0;
    int c=0;

    if( colorNumber < 0 )
    {
        color = Color3f( 1.0, 1.0, 1.0);  // TODO: colorNumber < 0 means
                                          // invisible! 
    }
    else
    {
        a = ( colorNumber % NCOLORS ) % 3;
        b = ( a + 1 ) % 3;
        c = ( b + 1 ) % 3;  

        color[a] = float( colorNumber % NCOLORS ) / float(NCOLORS);
        color[b] = 1.0 - color[a];
        color[c] = 0.0;
    }
    FDEBUGx(("DXFEntityBase::getColor(%d) returns color (%5.3f %5.3f %5.3f), "
            "indices %d %d %d\n",
            colorNumber, color[0], color[1], color[2],
            a, b, c));
    return color;   
}

/*! Return OpenSG Node this entity type is currently working on, if any,
 *  otherwise return NullFC. To be overloaded where necessary!
 */

Node *DXFEntityBase::getNode(void)
{
    return NULL;
}

/*! Globally used read method, dispatches subsections of the file structure to
 * the respective derived classes.
 *
 *  \impl
 *  The method dispatch() assumes, that this method leaves with
 *  DXFEntityBase::_state set to DXFStateContinue or DXFStateError only.
 *
 *  \see dispatch()
 */

void DXFEntityBase::read(DXFEntityBase *parent)
{
    _parent = parent;
    _state = beginEntity();

    while(_state == DXFStateContinue || _state == DXFStateIgnore)
    {
        /*Int8 filestate = */DXFRecord::fetch();
//      if(filestate < 0) // DXFTypeEOF, DXFTypeIgnore, DXFTypeUnknown -> DXFRecord,
//                        // TODO
//      {
//          FWARNING(("DXF Loader: EOF error in line %d, reported by %s:%s, "
//                    "current record is (%d|%s). "
//                    "May have happened already before. Giving up! (return = %s)\n",
//                    DXFRecord::getLineNumber(),
//                    getEntityClassName(),
//                    getEntityTypeName(),
//                    DXFRecord::getGroupCode(),
//                    DXFRecord::getValueStr().c_str(), 
//                    DXFResultNames[_state]
//                   ));
//          _state = DXFStateError;
//          break;
//      }
        
        if(DXFRecord::getGroupCode() == 0)
        {
            dispatch(); // modifies _state
        }   
        else if( _state != DXFStateIgnore)
        {
            _state = evalRecord();

            if(_state == DXFStateUnknown)
            {
                FDEBUGx(( "DXFEntityBase[%s:%s]::read(): unknown "
                         "DXFRecord: %4d | '%s'\n",
                         getEntityClassName(),
                         getEntityTypeName(),
                         DXFRecord::getGroupCode(),
                         DXFRecord::getValueStr().c_str()
                       ));              
                _state = DXFStateContinue;  // we just ignore unknown records!
            }
        }
    }
    if(_state == DXFStateReturn)        // TODO: we have to call endEntity in
                                        // any case for clean ups?
        _state = endEntity();
    
    if(_state != DXFStateError)     // endEntity() returns DXFStateError or
                                    // DXFStateContinue! 
        _state = DXFStateContinue;
}


/*! Standard dispatch method. Dispatch following subsection of DXF file to
 * corresponding child entity class.
 *
 * \dev This method looks quite unstructured. This is due to the
 * inconsistencies in the DXF file format. But it works and makes adding new
 * enitity types quite simple. When you - why ever - have to reimplement this
 * method in a derived class (as currently in DXFBlocks) you have to make that
 * class a friend class of DXFEntityBase! \enddev
 *
 * \pre DXFRecord::getGroupCode() == 0
 */

void DXFEntityBase::dispatch(void)
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

    // Keyword EOF or END* for SECTIONs, TABLEs, BLOCKs, ...
    if( (!_parent && DXFRecord::getValueStr() == "EOF") ||
        ( _parent && DXFRecord::getValueStr() == _parent->getChildEndKeyword())
      )
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
    
    // Entities are never nested to themselves!
    // BUT: e.g. TABLE:LAYER and LAYER:LAYER are not distinguishable this
    // way. Thus we cannot do this test here!
//  if(DXFRecord::getValueStr() == getEntityTypeName())
//  {
//      DXFRecord::unget();
//      _state = DXFStateReturn;
//      FDEBUGx(("Line %d: %s:%s returns %s because of Record (%d|%s)\n",
//              DXFRecord::getLineNumber(),
//              getEntityClassName(),
//              getEntityTypeName(),
//              DXFResultNames[_state],
//              DXFRecord::getGroupCode(),
//              DXFRecord::getValueStr().c_str()
//             ));
//      return;
//  }

    // Is the current entity none of the highlevel ones like SECTIONs,
    // TABLEs, ...?
    if(_childBeginKeyword.empty())
    {
        // maintainance: only DXFile::_parent will be NULL and DXFFile has a
        // nonempty _childBeginKeyword, thus we can
        // ASSERT(_parent != NULL)

        itr = _childEntityTypes.find(DXFRecord::getValueStr());
        // known child entity type?
        if(itr != _childEntityTypes.end() )
        {
            // descent into the entity type hierarchy
            itr->second->read(this);
            // we just leave _state as it is, read() has set it to either
            // DXFStateError or DXFStateContinue
            FDEBUGx(("(%d)line %d: %s:%s returns %s because of Record (%d|%s)\n",
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
        // unknown child entity type!
        // Is there an explicit end keyword for the current entity?
        if(_parent->getChildEndKeyword().empty())
        {
            // No explicit end, thus the current record delimits the current
            // entity
            FDEBUGx(("DXF Loader: in line %d: "
                     "Entity type '%s' closes current entity '%s', returning!\n",
                     DXFRecord::getLineNumber(),
                     DXFRecord::getValueStr().c_str(),
                     getEntityTypeName()
                    ));
            DXFRecord::unget();
            _state = DXFStateReturn;
        }
        else
        {
            // Explicit end, thus we encountered an entity type not yet
            // implemented (an END* keyword would have been recognized already,
            // see above!): we ignore it!
            // TODO: this is error-prone. Wrong END* keywords will be
            // ignored too without rising an DXFStateError.
            FWARNING(("DXF Loader: in line %d: "
                      "Entity type '%s' not yet implemented. Ignoring!\n",
                      DXFRecord::getLineNumber(),
                      DXFRecord::getValueStr().c_str()));
            _state = DXFStateIgnore;
        }
        FDEBUGx(("(%d)line %d: %s:%s returns %s because of Record (%d|%s)\n",
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
    
    if(DXFRecord::getValueStr() == _childBeginKeyword
//      && _state != DXFStateIgnore // TODO: muss das hier dazu=?!?!?
      )
    {
        // We're inside a highlevel entity. The child entity type is determined
        // by a record with group code 2
        DXFRecord::fetch();
        if(DXFRecord::getGroupCode() != 2)
        {
            // Wrong group code, this record is not expected.
            FWARNING(("DXF Loader: syntax error in line %d: "
                      "group code is %d but must be 2!\n",
                      DXFRecord::getLineNumber(),
                      DXFRecord::getGroupCode()));
            _state = DXFStateError;
            FDEBUGx(("(%d)line %d: %s:%s returns %s because of Record (%d|%s)\n", __LINE__,
                    DXFRecord::getLineNumber(),
                    getEntityClassName(),
                    getEntityTypeName(),
                    DXFResultNames[_state],
                    DXFRecord::getGroupCode(),
                    DXFRecord::getValueStr().c_str()
                   ));
            return;
        }
#if 0 // DEBUG
        FDEBUGx(("DXFEntityBase[%s,%s]::dispatch: "
                "%s(%p)->_childEntityTypes.size() = %d\n",
                getEntityClassName(),
                getEntityTypeName(),
                getEntityTypeName(),
                this,   
                _childEntityTypes.size()
               ));
        for ( itr =  _childEntityTypes.begin();
              itr != _childEntityTypes.end();
              ++ itr)
        {
            FDEBUGx(("DXFEntityBase[%s,%s]::dispatch: "
                    "%s == %s(%p)!!!\n",
                    getEntityClassName(),
                    getEntityTypeName(),
                    itr->first.c_str(),
                    itr->second->getEntityTypeName(),
                    itr->second
                   ));
        }
#endif // DEBUG
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
        itr->second->read(this);
        FDEBUGx(("(%d)line %d: %s:%s returns %s because of Record (%d|%s)\n", __LINE__,
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
        FDEBUGx(("(%d)line %d: %s:%s returns %s because of Record (%d|%s)\n", __LINE__,
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
        FDEBUGx(("(%d)line %d: %s:%s returns %s because of Record (%d|%s)\n", __LINE__,
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
} // DXFEntityBase::dispatch()


/*! Evaluate records with globally used group codes:
 * - 5 -- Handle
 * - 70 -- Flags (some entities use this code differently)
 * - 100 -- Subclass marker (completely ignored)
 */

DXFResult DXFEntityBase::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
    switch( DXFRecord::getGroupCode() )
    {
        case 5:         // Handle
            _handle = DXFRecord::getValueStr();
            break;
        case 70:        // Flags (some entities use 70 groups differently)
            _flags = DXFRecord::getValueInt();
            break;
        case 100:       // Subclass marker, completely ignored
            break;
        default:
            state = DXFStateUnknown;
    }
    return state;   
}

/*! Initialize OpenSG data structures and other data structures for the next
 * DXF entity. To be overwritten by derived classes, which represent DXF
 * Entities!
 */
DXFResult DXFEntityBase::beginEntity(void)
{
    _handle = "";
    _flags = 0;

    return DXFStateContinue;
}

/*! Process data for entity and translate to OpenSG data structures. To be
 * overwritten by derived classes, which represent DXF Entities!
 */
DXFResult DXFEntityBase::endEntity(void)
{
    return DXFStateContinue;
}



/*================================ PRIVATE ================================*/

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Base class initialization. Never called directly since this class is pure
 * virtual.
 */

DXFEntityBase::DXFEntityBase(void) :
    _entityTypeName(""),
    _entityClassName(""),
    _childEntityTypes(),
    _childBeginKeyword(""),
    _childEndKeyword(""),
    _parent(NULL),
    _handle(""),
    _flags(0)
{
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */

DXFEntityBase::~DXFEntityBase(void)
{
}

void DXFEntityBase::cleanup(void)
{
    DXFEntityBase::_rootNodeP         = NULL;
    DXFEntityBase::_activeNodeP       = NULL;

    DXFEntityBase::_pointsP           = NULL;

    DXFEntityBase::_pointIndicesP     = NULL;

    DXFEntityBase::_faceLengthP       = NULL;

    DXFEntityBase::_faceTypeP         = NULL;

    
    if(DXFEntityBase::_linetypeMapP != NULL)
    {
        DXFEntityBase::_linetypeMapP->clear();

        delete DXFEntityBase::_linetypeMapP;
    }
    
    DXFEntityBase::_linetypeMapP      = NULL;

    if(DXFEntityBase::_layersMapP != NULL)
    {
        DXFEntityBase::_layersMapP->clear();

        delete DXFEntityBase::_layersMapP;
    }

    DXFEntityBase::_layersMapP        = NULL;

    if(DXFEntityBase::_blocksMapP != NULL)
    {
        DXFEntityBase::_blocksMapP->clear();

        delete DXFEntityBase::_blocksMapP;
    }

    DXFEntityBase::_blocksMapP        = NULL;
}
