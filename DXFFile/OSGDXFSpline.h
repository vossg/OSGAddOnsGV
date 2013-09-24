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

#ifndef _OSGDXFSpline_H_
#define _OSGDXFSpline_H_

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

class OSG_DXFFILE_DLLMAPPING DXFSpline : public DXFEntitiesEntry
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
    
    static const Char8 *getClassname(void) { return "DXFSpline"; }

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*---------------------------------------------------------------------*/
    /*! \name                    Destructor                                */
    /*! \{                                                                 */

    virtual ~DXFSpline(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Member                                  */
    /*! \{                                                                 */

    static DXFSpline *_the;

    // Data for Arc
    Real32   _elevation;        // 10, 20, 30 (only 30 has a meaning) TODO
	Real32   _radius;
	Real32   _startAngle;
	Real32   _endAngle;
	Pnt3f    _centerPoint;
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

    DXFSpline(void);

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
    DXFSpline(const DXFSpline &source);
    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const DXFSpline &source);
};

OSG_END_NAMESPACE

#endif /* _OSGDXFSpline_H_ */
