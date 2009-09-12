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

#ifndef _OSGBBQOUTOFCOREDATASOURCE_H_
#define _OSGBBQOUTOFCOREDATASOURCE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGBbqOutOfCoreDataSourceBase.h"
#include "OSGBbqResidualCompression.h"
#include "OSGBbqTerrainNode.h"


OSG_BEGIN_NAMESPACE

template<class HeightType, class HeightDeltaType, class TextureType>
class BbqOutOfCoreEngine : public BbqDataSourceEngine
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqDataSourceEngine Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqOutOfCoreEngine(void);
    virtual ~BbqOutOfCoreEngine(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */
    
    bool initialize(const std::string &szFilename, 
                          Real32       fHeightScale, 
                          Real32       fHeightOffset, 
                          Real32       fSampleSpacing);
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual bool onLoadNodeData(BbqTerrainNodeBase &oNode);

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    typedef BbqTerrainNode<HeightType, 
                           HeightDeltaType, 
                           TextureType    > BbqTerrNode;

    typedef BbqFileNode   <HeightType     > FileNode;

    BbqFileReader                _oInput;
    BbqFile::BbqFileHeader       _oHeader;
    std::vector<FileNode>        _oStaticNodeData;
    ResidualDecompressor         _oResidualDecompressor;
    std::vector<HeightDeltaType> _vResidualBuffer; // Int16

    /*==========================  PRIVATE  ================================*/

  private:
};

/*! \brief BbqOutOfCoreDataSource class. See \ref
           PageDrawableBbqOutOfCoreDataSource for a description.
*/

class OSG_DRAWABLE_DLLMAPPING BbqOutOfCoreDataSource : 
    public BbqOutOfCoreDataSourceBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqOutOfCoreDataSourceBase Inherited;
    typedef BbqOutOfCoreDataSource     Self;

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

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    BbqOutOfCoreDataSource(void);
    BbqOutOfCoreDataSource(const BbqOutOfCoreDataSource &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~BbqOutOfCoreDataSource(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    bool initialize(const std::string &szFilename, 
                          Real32       fHeightScale, 
                          Real32       fHeightOffset, 
                          Real32       fSampleSpacing);
    
    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class BbqOutOfCoreDataSourceBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const BbqOutOfCoreDataSource &source);
};

typedef BbqOutOfCoreDataSource *BbqOutOfCoreDataSourceP;

OSG_END_NAMESPACE

#include "OSGBbqOutOfCoreDataSourceBase.inl"
#include "OSGBbqOutOfCoreDataSource.inl"

#endif /* _OSGBBQOUTOFCOREDATASOURCE_H_ */
