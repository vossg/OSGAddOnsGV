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

#ifndef _OSGDXFHatch_H_
#define _OSGDXFHatch_H_

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include "OSGBaseTypes.h"
#include "OSGDXFFileDef.h"

#include "OSGDXFEntityBase.h"
#include "OSGDXFEntitiesEntry.h"

OSG_BEGIN_NAMESPACE

//---------------------------------------------------------------------------
//  Forward References
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Types
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

/*! \brief Handles the POLYLINE entry in the DXF file strucure.
 */
struct DXFHatchBoundaryPathDataPolyLine
{
	Int32 isClosedFlag;
	Int32 numOFPolyVertices;
	Real32 startX;
	Real32 startY;
	Int32  bulge;
};
struct DXFHatchBoundaryPathDataLine
{
	Real32 startX;
	Real32 startY;
	Real32 endX;
	Real32 endY;
};
struct DXFHatchBoundaryPathDataArc
{
	Real32 centerX;
	Real32 centerY;
	Real32 radius;
	Real32 startAngle;
	Real32 endAngle;
	Int32  isCounterClockWise;
};
struct  DXFHatchBoundaryPathData
{
	
	//here put the codes for ellipse and spline
	 Int32 _pathTypeFlag;
	 Int32 _numOfEdge;
	 Int32 _edgeType_Or_hasBulgeFlag;
	 std::vector<DXFHatchBoundaryPathDataPolyLine> polyLineEdges;
	 std::vector<DXFHatchBoundaryPathDataLine> lineEdges;
	 std::vector<DXFHatchBoundaryPathDataArc> arcEdges;
	 DXFHatchBoundaryPathData(void);
};
struct DXFHatchPatternData
{
	Real32 patternLineAngle;
	Real32 patternBasePX;
	Real32 patternBasePY;
	Int32  patternLineOffsetX;
	Int32  patternLineOffsetY;
	Int32  numDashLenItems;
	Real32 dashLength;
};

class OSG_DXFFILE_DLLMAPPING DXFHatch : public DXFEntitiesEntry
{
    /*==========================  PUBLIC  =================================*/
  public:
    
    /*---------------------------------------------------------------------*/
    /*! \name                    Helper                                    */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Get                                     */
    /*! \{                                                                 */
    static const Char8 *getClassname(void) { return "DXFHatch"; }

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*---------------------------------------------------------------------*/
    /*! \name                    Destructor                                */
    /*! \{                                                                 */

    virtual ~DXFHatch(void);
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Member                                  */
    /*! \{                                                                 */

    static DXFHatch *_the;
    // Data for Arc
    Real32   _elevation;        // 10, 20, 30 (only 30 has a meaning) TODO
	Int32    _solidFillFlag;
	
	Int32   _numOfBoundaryPath;
	std::vector<DXFHatchBoundaryPathData> hatchBoundaryDataLoops;

	Int32  _numOfPatternLine;
	std::vector<DXFHatchPatternData> hatchPatternData;

	Int32   _numSeedPoints;
	Real32  _seedPntX;
	Real32  _seedPntY;
    // 70: Polyline flags are read to DXFEntitiesEntry::_flags
    // 66: Obsolete, (explicitly) ignored
    // Group codes implemented by DXFEntitiesEntry: 39, 210, 220, 230
    // Group codes not implemented: 40, 41, 71, 72, 73, 74, 75

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Parsing                                  */
    /*! \{                                                                 */
    
    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    typedef DXFEntitiesEntry Inherited;
    
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    DXFHatch(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Parsing                                  */
    /*! \{                                                                 */

    virtual DXFResult evalRecord (void);
    virtual DXFResult beginEntity(void);
    virtual DXFResult endEntity  (void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/

    /*!\brief prohibit default function (move to 'public' if needed) */
    DXFHatch(const DXFHatch &source);
    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const DXFHatch &source);
};

OSG_END_NAMESPACE

#endif /* _OSGDXFHatch_H_ */
