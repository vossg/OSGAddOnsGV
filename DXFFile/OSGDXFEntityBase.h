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

#ifndef _OSGDXFENTITYBASE_H_
#define _OSGDXFENTITYBASE_H_
#ifdef __sgi
#pragma once
#endif

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include "OSGBaseTypes.h"
#include "OSGDXFFileDef.h"

#include "OSGNode.h"

#include "OSGDXFRecord.h"

#include "OSGTypedGeoIntegralProperty.h"
#include "OSGTypedGeoVectorProperty.h"
#include "OSGLineChunk.h"
#include "OSGColor.h"

#include <map>

OSG_BEGIN_NAMESPACE

//---------------------------------------------------------------------------
//  Temporary for Debugging
//----------------------------------------------C-----------------------------

#define FDEBUGx(x)
#define FDEBUG0(x)

//---------------------------------------------------------------------------
//  Forward References
//---------------------------------------------------------------------------

class DXFEntityBase;

//---------------------------------------------------------------------------
//  Types
//---------------------------------------------------------------------------

/*! Codes for the current state of the DXF parser.
 */
enum DXFResult // TODO: Doku!
{
    DXFStateError    = -1,  //!< An unrecoverable error occurred during file parsing
    DXFStateContinue =  0,  //!< Read next record and proceed
    DXFStateReturn   =  1,  //!< Ascend in the entity type hierarchy
    DXFStateUnknown  =  2,  //!< (Currenty not used)
    DXFStateIgnore   =  3   //!< Ignore following records until something known is read
};

typedef std::map<std::string, DXFEntityBase *>    EntityTypeMap;
typedef std::map<std::string, NodeUnrecPtr>       StringToNodePtrMap;
typedef std::map<std::string, LineChunkUnrecPtr>  StringToLineChunkPtrMap;

//---------------------------------------------------------------------------
//  Class
//---------------------------------------------------------------------------

/*! \brief Baseclass for all entity types in the DXF file structure.
 */

class OSG_DXFFILE_DLLMAPPING DXFEntityBase
{
    /*==========================  PUBLIC  =================================*/

  public:
    
    /*---------------------------------------------------------------------*/
    /*! \name                      Helper                                  */
    /*! \{                                                                 */
    
    virtual Node *getNode(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Get                                     */
    /*! \{                                                                 */
    
    static const Char8 *getClassname      (void) { return "DXFEntityBase"; }
           const Char8 *getEntityTypeName (void);
           const Char8 *getEntityClassName(void);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    friend class DXFFile;
    // Add any class which overloads dispatch() here as friend!
    friend class DXFBlocks;
    
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    DXFEntityBase(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    Destructor                                */
    /*! \{                                                                 */

    virtual ~DXFEntityBase(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Set                                     */
    /*! \{                                                                 */

    void registerToParentEntityType(const Char8 *parentEntityTypeName);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Set                                     */
    /*! \{                                                                 */

    virtual void freeMemory(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Get                                     */
    /*! \{                                                                 */

    const std::string &getChildEndKeyword(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Helper                                  */
    /*! \{                                                                 */
    
    static Color3f getColor(Int16 colorNumber);
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Member                                  */
    /*! \{                                                                 */


    std::string         _entityTypeName;
    std::string         _entityClassName;

    EntityTypeMap       _childEntityTypes;
    std::string         _childBeginKeyword;
    std::string         _childEndKeyword;

    DXFEntityBase      *_parent; //< Pointer to parent entity type instance


    // Data for globally common group codes
    std::string _handle;        // 5   handle (except 105 for handle for DIMSTYLE)
    Int16       _flags;         // 70  <entity> flags

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Member                                  */
    /*! \{                                                                 */

    static Char8 const * const * const DXFResultNames;

    static UInt32                     _options;
    static DXFResult                  _state;

    // OpenSG Data
    static NodeUnrecPtr               _rootNodeP;
    static NodeUnrecPtr               _activeNodeP;

    static GeoPnt3dPropertyUnrecPtr   _pointsP;
    static MFPnt3d                   *_pointsF;
    static MFPnt3d::iterator          _pointsIt;

    static GeoUInt32PropertyUnrecPtr  _pointIndicesP;
    static MFUInt32                  *_pointIndicesF;
    static MFUInt32::iterator         _pointIndicesIt;

    static GeoUInt32PropertyUnrecPtr  _faceLengthP;
    static MFUInt32                  *_faceLengthF;
    static MFUInt32::iterator         _faceLengthIt;

    static GeoUInt8PropertyUnrecPtr   _faceTypeP;
    static MFUInt8                   *_faceTypeF;
    static MFUInt8::iterator          _faceTypeIt;
    
    static UInt32                     _uiNumFacePoints;
    static Int32                      _uiCurrentFaceType;

    static StringToLineChunkPtrMap   *_linetypeMapP;
    static StringToNodePtrMap        *_layersMapP;
    static StringToNodePtrMap        *_blocksMapP;
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Parsing                                  */
    /*! \{                                                                 */
    
    void read(DXFEntityBase *parent);
    virtual void dispatch(void);

    virtual DXFResult evalRecord(void);

    virtual DXFResult beginEntity(void);
    virtual DXFResult endEntity(void);

    void cleanup(void);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/
  private:

    /*!\brief prohibit default function (move to 'public' if needed) */
    DXFEntityBase(const DXFEntityBase &source);
    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const DXFEntityBase &source);
};

OSG_END_NAMESPACE

#endif /* _OSGDXFENTITYBASE_H_ */
