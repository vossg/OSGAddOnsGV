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

#include "OSGDXFHeader.h"

OSG_USING_NAMESPACE

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFHeader
    \ingroup GrpSystemFileIO

  Parses the file section between the groups (0,SECTION), (2,HEADER) and
  (0,ENDSEC). As far as implemented, DXF header variables are evaluated.

  Currently theres only an output to show the AutoCAD release version (header
  variable $ACADVER).
 
  \todo Check, which other variables are relevant for the OpenSG import - and
  then implement them...

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

Real32 DXFHeader::_angBase = 0;
Int32  DXFHeader::_angDir  = 0;
Int32  DXFHeader::_ceColor = 0;
DXFHeader *DXFHeader::_the = new DXFHeader();

/* \var VARTYPE DXFHeader::_VARNAME
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

/*! Evaluate HEADER variables. Only a few are implemented exemplarically. The
 * others are unknown and will be ignored by the parser.
 */

DXFResult DXFHeader::evalRecord(void)
{
    DXFResult state = DXFStateUnknown;  

    if(DXFRecord::getGroupCode() == 9)
    {
        _headerVariable = DXFRecord::getValueStr();
        state = DXFStateContinue;
    }
    else
    {
        if(_headerVariable == "$ACADVER")
        {
            if(DXFRecord::getGroupCode() == 1)
            {
                FINFO(("DXF Loader: AutoCAD File Version '%s'\n",
                       DXFRecord::getValueStr().c_str()));
                // TODO: check, whether version is implemented...
                state = DXFStateContinue;
            }
        }
		else if(_headerVariable == "$ANGBASE")
		{
			_angBase = DXFRecord::getValueDbl();
		}
		else if(_headerVariable == "$ANGDIR")
		{
			_angDir = DXFRecord::getValueInt();
		}
		else if(_headerVariable == "$CECOLOR")
		{
			_ceColor = DXFRecord::getValueInt();
		}
//      else if(_headerVariable == "$EXTMIN")
//      {
//          // Fill Pnt3f extmin with values from 3 records containing
//          // coordinate data 
//          state = DXFStateContinue;
//      }
//      else if ...
    }
    if(state == DXFStateUnknown)
        state = Inherited::evalRecord();
    return state;
}

/*! \todo
 *  Any global preparations needed here?
 */

DXFResult DXFHeader::beginEntity(void)
{
    DXFResult state;

    state = Inherited::beginEntity();

    return state;
}

/*! \todo
 * There might be work for correctly interpret the HEADER section.
 */

DXFResult DXFHeader::endEntity(void)
{
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add SECTION:HEADER entity to DXF hierarchy as child of FILE:FILE (see
 *  DXFFile).
 */
DXFHeader::DXFHeader(void) :
     Inherited     (  ),
    _headerVariable("")
{
    _entityClassName = "SECTION";   
    _entityTypeName  = "HEADER";

    registerToParentEntityType("FILE:FILE");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */

DXFHeader::~DXFHeader(void)
{
}

