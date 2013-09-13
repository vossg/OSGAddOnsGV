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

#ifndef _OSGDXFENTITIESENTRY_H_
#define _OSGDXFENTITIESENTRY_H_

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include "OSGBaseTypes.h"
#include "OSGDXFFileDef.h"

#include "OSGDXFEntityBase.h"

#include "OSGMatrix.h"  // for arbitraryAxisAlgorithm
#include "OSGVector.h"  // for arbitraryAxisAlgorithm

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

/*! \brief Base class for all entries inside a ENTITIES entity.
 */

class OSG_DXFFILE_DLLMAPPING DXFEntitiesEntry : public DXFEntityBase
{
    /*==========================  PUBLIC  =================================*/

  public:
    
    /*---------------------------------------------------------------------*/
    /*! \name                      Get                                     */
    /*! \{                                                                 */
    
    static  const Char8 *getClassname(void) { return "DXFEntitiesEntry"; }

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Set                                     */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    DXFEntitiesEntry(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    Destructor                                */
    /*! \{                                                                 */

    virtual ~DXFEntitiesEntry(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Helper                                  */
    /*! \{                                                                 */

    static Matrix arbitraryAxisAlgorithm(const Vec3f &normal); // TODO:
                                                               // woanders
                                                               // sammeln? 

    Node      *getParentNode(void             );
    DXFResult  beginGeometry(void             );
    DXFResult  endGeometry  (void             );
    void       flushGeometry(bool forceNewNode);


    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Member                                  */
    /*! \{                                                                 */

    // Data for common group codes for entities
    std::string _layerName;             // 8
    std::string _linetypeName;          // 6
    Int16       _colorNumber;           // 62
    Real32      _linetypeScale;         // 48
    Real32      _thickness;             // 39
    Vec3f       _extrusionDirection;    // 210, 220, 230
    // OpenSG data

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Parsing                                  */
    /*! \{                                                                 */
    
    virtual DXFResult evalRecord (void);
    virtual DXFResult beginEntity(void);
//  virtual DXFResult endEntity(void);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    typedef DXFEntityBase Inherited;
    
    /*!\brief prohibit default function (move to 'public' if needed) */
    DXFEntitiesEntry(const DXFEntitiesEntry &source);
    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const DXFEntitiesEntry &source);
};

OSG_END_NAMESPACE

#endif /* _OSGDXFENTITIESENTRY_H_ */
