/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2013 by the OpenSG Forum                 *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 * contact: dirk@opensg.org, gerrit.voss@vossg.org, carsten_neumann@gmx.net  *
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

/*****************************************************************************\
 *****************************************************************************
 **                                                                         **
 **                  This file is automatically generated.                  **
 **                                                                         **
 **          Any changes made to this file WILL be lost when it is          **
 **           regenerated, which can become necessary at any time.          **
 **                                                                         **
 **     Do not change this file, changes should be done in the derived      **
 **     class BbqTerrain
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/


#ifndef _OSGBBQTERRAINBASE_H_
#define _OSGBBQTERRAINBASE_H_
#ifdef __sgi
#pragma once
#endif


#include "OSGAddOnsConfig.h"
#include "OSGContribBbqTerrainDef.h"

//#include "OSGBaseTypes.h"

#include "OSGStageDrawable.h" // Parent

#include "OSGNodeFields.h"              // Beacon type
#include "OSGBbqDataSourceFields.h"     // DataSource type
#include "OSGSysFields.h"               // MaxNumResidentNodes type

#include "OSGBbqTerrainFields.h"

OSG_BEGIN_NAMESPACE


class BbqTerrain;

//! \brief BbqTerrain Base Class.

class OSG_CONTRIBBBQTERRAIN_DLLMAPPING BbqTerrainBase : public StageDrawable
{
  public:

    typedef StageDrawable Inherited;
    typedef StageDrawable ParentContainer;

    typedef Inherited::TypeObject TypeObject;
    typedef TypeObject::InitPhase InitPhase;

    OSG_GEN_INTERNALPTR(BbqTerrain);

    /*==========================  PUBLIC  =================================*/

  public:

    enum
    {
        BeaconFieldId = Inherited::NextFieldId,
        DataSourceFieldId = BeaconFieldId + 1,
        MaxNumResidentNodesFieldId = DataSourceFieldId + 1,
        ScreenSpaceErrorFieldId = MaxNumResidentNodesFieldId + 1,
        EnableSkirtsFieldId = ScreenSpaceErrorFieldId + 1,
        ShowSwitchDistanceFieldId = EnableSkirtsFieldId + 1,
        NextFieldId = ShowSwitchDistanceFieldId + 1
    };

    static const OSG::BitVector BeaconFieldMask =
        (TypeTraits<BitVector>::One << BeaconFieldId);
    static const OSG::BitVector DataSourceFieldMask =
        (TypeTraits<BitVector>::One << DataSourceFieldId);
    static const OSG::BitVector MaxNumResidentNodesFieldMask =
        (TypeTraits<BitVector>::One << MaxNumResidentNodesFieldId);
    static const OSG::BitVector ScreenSpaceErrorFieldMask =
        (TypeTraits<BitVector>::One << ScreenSpaceErrorFieldId);
    static const OSG::BitVector EnableSkirtsFieldMask =
        (TypeTraits<BitVector>::One << EnableSkirtsFieldId);
    static const OSG::BitVector ShowSwitchDistanceFieldMask =
        (TypeTraits<BitVector>::One << ShowSwitchDistanceFieldId);
    static const OSG::BitVector NextFieldMask =
        (TypeTraits<BitVector>::One << NextFieldId);
        
    typedef SFUnrecNodePtr    SFBeaconType;
    typedef SFUnrecBbqDataSourcePtr SFDataSourceType;
    typedef SFUInt32          SFMaxNumResidentNodesType;
    typedef SFReal32          SFScreenSpaceErrorType;
    typedef SFBool            SFEnableSkirtsType;
    typedef SFBool            SFShowSwitchDistanceType;

    /*---------------------------------------------------------------------*/
    /*! \name                    Class Get                                 */
    /*! \{                                                                 */

    static FieldContainerType &getClassType   (void);
    static UInt32              getClassTypeId (void);
    static UInt16              getClassGroupId(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                FieldContainer Get                            */
    /*! \{                                                                 */

    virtual       FieldContainerType &getType         (void);
    virtual const FieldContainerType &getType         (void) const;

    virtual       UInt32              getContainerSize(void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    Field Get                                 */
    /*! \{                                                                 */

            const SFUnrecNodePtr      *getSFBeacon         (void) const;
                  SFUnrecNodePtr      *editSFBeacon         (void);
            const SFUnrecBbqDataSourcePtr *getSFDataSource     (void) const;
                  SFUnrecBbqDataSourcePtr *editSFDataSource     (void);

                  SFUInt32            *editSFMaxNumResidentNodes(void);
            const SFUInt32            *getSFMaxNumResidentNodes (void) const;

                  SFReal32            *editSFScreenSpaceError(void);
            const SFReal32            *getSFScreenSpaceError (void) const;

                  SFBool              *editSFEnableSkirts   (void);
            const SFBool              *getSFEnableSkirts    (void) const;

                  SFBool              *editSFShowSwitchDistance(void);
            const SFBool              *getSFShowSwitchDistance (void) const;


                  Node * getBeacon         (void) const;

                  BbqDataSource * getDataSource     (void) const;

                  UInt32              &editMaxNumResidentNodes(void);
                  UInt32               getMaxNumResidentNodes (void) const;

                  Real32              &editScreenSpaceError(void);
                  Real32               getScreenSpaceError (void) const;

                  bool                &editEnableSkirts   (void);
                  bool                 getEnableSkirts    (void) const;

                  bool                &editShowSwitchDistance(void);
                  bool                 getShowSwitchDistance (void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    Field Set                                 */
    /*! \{                                                                 */

            void setBeacon         (Node * const value);
            void setDataSource     (BbqDataSource * const value);
            void setMaxNumResidentNodes(const UInt32 value);
            void setScreenSpaceError(const Real32 value);
            void setEnableSkirts   (const bool value);
            void setShowSwitchDistance(const bool value);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                Ptr Field Set                                 */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                Ptr MField Set                                */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Binary Access                              */
    /*! \{                                                                 */

    virtual SizeT  getBinSize (ConstFieldMaskArg  whichField);
    virtual void   copyToBin  (BinaryDataHandler &pMem,
                               ConstFieldMaskArg  whichField);
    virtual void   copyFromBin(BinaryDataHandler &pMem,
                               ConstFieldMaskArg  whichField);


    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Construction                               */
    /*! \{                                                                 */

    static  BbqTerrainTransitPtr  create          (void);
    static  BbqTerrain           *createEmpty     (void);

    static  BbqTerrainTransitPtr  createLocal     (
                                               BitVector bFlags = FCLocal::All);

    static  BbqTerrain            *createEmptyLocal(
                                              BitVector bFlags = FCLocal::All);

    static  BbqTerrainTransitPtr  createDependent  (BitVector bFlags);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Copy                                   */
    /*! \{                                                                 */

    virtual FieldContainerTransitPtr shallowCopy     (void) const;
    virtual FieldContainerTransitPtr shallowCopyLocal(
                                       BitVector bFlags = FCLocal::All) const;
    virtual FieldContainerTransitPtr shallowCopyDependent(
                                                      BitVector bFlags) const;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    static TypeObject _type;

    static       void   classDescInserter(TypeObject &oType);
    static const Char8 *getClassname     (void             );

    /*---------------------------------------------------------------------*/
    /*! \name                      Fields                                  */
    /*! \{                                                                 */

    SFUnrecNodePtr    _sfBeacon;
    SFUnrecBbqDataSourcePtr _sfDataSource;
    SFUInt32          _sfMaxNumResidentNodes;
    SFReal32          _sfScreenSpaceError;
    SFBool            _sfEnableSkirts;
    SFBool            _sfShowSwitchDistance;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    BbqTerrainBase(void);
    BbqTerrainBase(const BbqTerrainBase &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~BbqTerrainBase(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     onCreate                                */
    /*! \{                                                                 */

    void onCreate(const BbqTerrain *source = NULL);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    Generic Field Access                      */
    /*! \{                                                                 */

    GetFieldHandlePtr  getHandleBeacon          (void) const;
    EditFieldHandlePtr editHandleBeacon         (void);
    GetFieldHandlePtr  getHandleDataSource      (void) const;
    EditFieldHandlePtr editHandleDataSource     (void);
    GetFieldHandlePtr  getHandleMaxNumResidentNodes (void) const;
    EditFieldHandlePtr editHandleMaxNumResidentNodes(void);
    GetFieldHandlePtr  getHandleScreenSpaceError (void) const;
    EditFieldHandlePtr editHandleScreenSpaceError(void);
    GetFieldHandlePtr  getHandleEnableSkirts    (void) const;
    EditFieldHandlePtr editHandleEnableSkirts   (void);
    GetFieldHandlePtr  getHandleShowSwitchDistance (void) const;
    EditFieldHandlePtr editHandleShowSwitchDistance(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Sync                                   */
    /*! \{                                                                 */

#ifdef OSG_MT_CPTR_ASPECT
    virtual void execSyncV(      FieldContainer    &oFrom,
                                 ConstFieldMaskArg  whichField,
                                 AspectOffsetStore &oOffsets,
                                 ConstFieldMaskArg  syncMode  ,
                           const UInt32             uiSyncInfo);

            void execSync (      BbqTerrainBase *pFrom,
                                 ConstFieldMaskArg  whichField,
                                 AspectOffsetStore &oOffsets,
                                 ConstFieldMaskArg  syncMode  ,
                           const UInt32             uiSyncInfo);
#endif

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Edit                                   */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Aspect Create                            */
    /*! \{                                                                 */

#ifdef OSG_MT_CPTR_ASPECT
    virtual FieldContainer *createAspectCopy(
                                    const FieldContainer *pRefAspect) const;
#endif

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Edit                                   */
    /*! \{                                                                 */
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Sync                                   */
    /*! \{                                                                 */

    virtual void resolveLinks(void);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:
    /*---------------------------------------------------------------------*/

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const BbqTerrainBase &source);
};

typedef BbqTerrainBase *BbqTerrainBaseP;

OSG_END_NAMESPACE

#endif /* _OSGBBQTERRAINBASE_H_ */
