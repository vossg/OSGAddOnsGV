/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2006 by the OpenSG Forum                 *
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

#ifndef _OSGBBQDATASOURCE_H_
#define _OSGBBQDATASOURCE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGBbqDataSourceBase.h"
#include "OSGBbqNode.h"
#include "OSGBbqFile.h"

OSG_BEGIN_NAMESPACE

struct BbqDataSourceInformation
{
    Int32                  levelCount;
    Int32                  nodeCount;
    BbqFile::HeightFormat  heightFormat;
    Int32                  heightTileSize;
    Vec2i                  heightSampleCount;
    Image::Type            heightType;
    Image::Type            textureType;
    BbqFile::TextureFormat textureFormat;
    Int32                  textureTileSize;
    Vec2i                  textureSampleCount;
    
    // terrain scaling information:
    Real32                 heightScale;
    Real32                 heightOffset;
    Real32                 sampleSpacing;

    // terrain global grid bbox
    Vec2f                  gridBBoxMin;
    Vec2f                  gridBBoxMax;

    //geographic referencing:
    UInt32                 uiDatum;
    Vec2f                  vEllipsoidAxis;
    Vec2f                  vOrigin;
    Vec2f                  vPixelSize;

    // geo ref to grid mapping
    Vec2f                  vScale;
    Vec2f                  vOffset;
};

struct BbqTerrainNodeBase;


class BbqDataSourceEngine
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqDataSourceEngine(void);
    virtual ~BbqDataSourceEngine(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    const BbqDataSourceInformation &getInformation(void) const;
          Image::Type               getHeightType (void) const;
          Image::Type               getTextureType(void) const;
          bool                      hasGeoRef     (void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void computeBoundingBox(BbqTerrainNodeBase &oNode, 
                            Real32              fMinHeightSample, 
                            Real32              fMaxHeightSample);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual bool onLoadNodeData(BbqTerrainNodeBase &oNode) = 0;

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    BbqDataSourceInformation _oInformation;

    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */
    
    void initSourceInformation(BbqFile::BbqFileHeader &oHeader, 
                               bool                    bIgnoreGeoRef,
                               Real32                  fHeightScale, 
                               Real32                  fHeightOffset, 
                               Real32                  fSampleSpacing);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:
};

/*! \brief BbqDataSource class. See \ref
           PageDrawableBbqDataSource for a description.
*/

class OSG_DRAWABLE_DLLMAPPING BbqDataSource : public BbqDataSourceBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqDataSourceBase Inherited;
    typedef BbqDataSource     Self;

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    const BbqDataSourceInformation &getInformation(void) const;
          Image::Type               getHeightType (void) const;
          Image::Type               getTextureType(void) const;
          bool                      hasGeoRef     (void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // load height and texture data: return false, if the data could not be
    // found/is invalid 
    // the returned data is valid until the next call to loadNodeData..
    // this loads the data synchronously.. you can use this method from a
    // separate thread to prevent any frame rate stalls.. 

    bool loadNodeData(BbqTerrainNodeBase &oNode);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in BbqDataSourceBase.

    BbqDataSourceEngine      *_pEngine;
    BbqDataSourceInformation  _oDefaultInfo;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    BbqDataSource(void);
    BbqDataSource(const BbqDataSource &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~BbqDataSource(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */


    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class BbqDataSourceBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const BbqDataSource &source);
};

typedef BbqDataSource *BbqDataSourceP;

OSG_END_NAMESPACE

#include "OSGBbqDataSourceBase.inl"
#include "OSGBbqDataSource.inl"

#endif /* _OSGBBQDATASOURCE_H_ */
