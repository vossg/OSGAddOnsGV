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

#include "OSGDXFTables.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFTables
    \ingroup GrpSystemFileIO

  Parses and dispatches the file section between the groups (0,SECTION),
  (2,TABLES) and (0,ENDSEC). Tables are dispatched to dumb table classes like
  DXFTableLayer, which dispatches its LAYER entries to DXFLayer.

 */

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

#ifndef __sun
/*! \typedef DXFTableAppid
 * Handles the APPID table entry in the DXF file structure. Parses and
 * dispatches the file section between the groups (0,TABLE), (2,APPID) and
 * (0,ENDTAB).
 */
 extern const Char8 tableAppidName[] =         "APPID";
typedef DXFTable<tableAppidName>        DXFTableAppid;

/*! \typedef DXFTableBlock_record
 * Handles the BLOCK_RECORD table entry in the DXF file structure. Parses and
 * dispatches the file section between the groups (0,TABLE), (2,BLOCK_RECORD) and
 * (0,ENDTAB).
 */
extern const Char8 tableBlock_recordName[] =       "BLOCK_RECORD";
typedef DXFTable<tableBlock_recordName> DXFTableBlock_record;

/*! \typedef DXFTableDimstyle
 * Handles the DIMSTYLE table entry in the DXF file structure. Parses and
 * dispatches the file section between the groups (0,TABLE), (2,DIMSTYLE) and
 * (0,ENDTAB).
 */
extern const Char8 tableDimstyleName[] =           "DIMSTYLE";
typedef DXFTable<tableDimstyleName>     DXFTableDimstyle;

/*! \typedef DXFTableLayer
 * Handles the LAYER table entry in the DXF file structure. Parses and
 * dispatches the file section between the groups (0,TABLE), (2,LAYER) and
 * (0,ENDTAB).
 */
extern const Char8 tableLayerName[] =          "LAYER";
typedef DXFTable<tableLayerName>        DXFTableLayer;


/*! \typedef DXFTableLtype
 * Handles the LTYPE table entry in the DXF file structure. Parses and
 * dispatches the file section between the groups (0,TABLE), (2,LTYPE) and
 * (0,ENDTAB).
 */
extern const Char8 tableLtypeName[] =          "LTYPE";
typedef DXFTable<tableLtypeName>        DXFTableLtype;

/*! \typedef DXFTableStyle
 * Handles the STYLE table entry in the DXF file structure. Parses and
 * dispatches the file section between the groups (0,TABLE), (2,STYLE) and
 * (0,ENDTAB).
 */
extern const Char8 tableStyleName[] =          "STYLE";
typedef DXFTable<tableStyleName>        DXFTableStyle;

/*! \typedef DXFTableUcs
 * Handles the UCS table entry in the DXF file structure. Parses and
 * dispatches the file section between the groups (0,TABLE), (2,UCS) and
 * (0,ENDTAB).
 */
extern const Char8 tableUcsName[] =           "UCS";
typedef DXFTable<tableUcsName>          DXFTableUcs;

/*! \typedef DXFTableView
 * Handles the VIEW table entry in the DXF file structure. Parses and
 * dispatches the file section between the groups (0,TABLE), (2,VIEW) and
 * (0,ENDTAB).
 */
extern const Char8 tableViewName[] =               "VIEW";
typedef DXFTable<tableViewName>         DXFTableView;

/*! \typedef DXFTableVport
 * Handles the VPORT table entry in the DXF file structure. Parses and
 * dispatches the file section between the groups (0,TABLE), (2,VPORT) and
 * (0,ENDTAB).
 */
extern const Char8 tableVportName[] =          "VPORT";
typedef DXFTable<tableVportName>        DXFTableVport;


/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/*! Pointer to singleton of this class. There is exactly one instance for each
 * derived class which can be instantiated.
 */
DXFTables *DXFTables::_the = new DXFTables();

// HACK the irix doesn't like this need to fix it!
#if !defined(__sgi)

//DXFTableAppid      *DXFTableAppid::_the        = new DXFTableAppid();
//DXFTableBlock_record *DXFTableBlock_record::_the = new DXFTableBlock_record();
//DXFTableDimstyle   *DXFTableDimstyle::_the     = new DXFTableDimstyle();
template <>
DXFTableLayer        *DXFTableLayer::_the        = new DXFTableLayer();

template <>
DXFTableLtype        *DXFTableLtype::_the        = new DXFTableLtype();

//DXFTableStyle          *DXFTableStyle::_the        = new DXFTableStyle();
//DXFTableUcs        *DXFTableUcs::_the          = new DXFTableUcs();
//DXFTableView       *DXFTableView::_the         = new DXFTableView();
//DXFTableVport          *DXFTableVport::_the        = new DXFTableVport();

#endif

#endif

/* \var VARTYPE DXFTables::_VARNAME
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

/*! Evaluate records with TABLES group codes (currently none).
 */
DXFResult DXFTables::evalRecord(void)
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

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add TABLES entity (of class SECTION) to DXF hierarchy as child of FILE:FILE
 *  (see DXFFile).
 */
DXFTables::DXFTables(void) :
    Inherited()
{
    _entityTypeName    = "TABLES";
    _entityClassName   = "SECTION";

    _childBeginKeyword = "TABLE";
    _childEndKeyword   = "ENDTAB";  

    registerToParentEntityType("FILE:FILE");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFTables::~DXFTables(void)
{
}

