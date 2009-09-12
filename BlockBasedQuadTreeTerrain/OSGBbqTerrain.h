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

#ifndef _OSGBBQTERRAIN_H_
#define _OSGBBQTERRAIN_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGNode.h"
#include "OSGBbqTerrainBase.h"
#include "OSGAction.h"

#include "OSGBbqTerrainEngine.h"
#include "OSGBbqOutOfCoreDataSourceEngine.h"

OSG_BEGIN_NAMESPACE

/*! \brief BbqTerrain class. See \ref
           PageDrawableBbqTerrain for a description.
*/

class OSG_DRAWABLE_DLLMAPPING BbqTerrain : public BbqTerrainBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqTerrainBase Inherited;
    typedef BbqTerrain     Self;

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

    virtual void fill(OSG::DrawableStatsAttachment *);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in BbqTerrainBase.

    // tmp
    BbqTerrainEngine             *terrain_;
    BbqOutOfCoreDataSourceEngine *outOfCoreDataSource_;
    BbqRenderOptions              terrainRenderOptions_;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    BbqTerrain(void);
    BbqTerrain(const BbqTerrain &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~BbqTerrain(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    Action::ResultE renderEnter(Action  *action  );
    Action::ResultE renderLeave(Action  *action  );

    void            execute    (DrawEnv *pDrawEnv);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class BbqTerrainBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const BbqTerrain &source);
};

typedef BbqTerrain *BbqTerrainP;

OSG_END_NAMESPACE

#include "OSGBbqTerrainBase.inl"
#include "OSGBbqTerrain.inl"

#endif /* _OSGBBQTERRAIN_H_ */
