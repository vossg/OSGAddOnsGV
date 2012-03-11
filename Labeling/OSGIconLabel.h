/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2002 by the OpenSG Forum                   *
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

#ifndef _OSGICONLABEL_H_
#define _OSGICONLABEL_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGContribLabelingDef.h"
#include "OSGTextureObjChunk.h"
#include "OSGIconLabelBase.h"
#include <string>
#include <map>

OSG_BEGIN_NAMESPACE

/*! \brief IconLabel provides the projection of text (labels) onto the viewport.
    \ingroup GrpSystemNodeCoresMisc
*/

class OSG_CONTRIBLABELING_DLLMAPPING IconLabel : public IconLabelBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                     Modes                                    */
    /*! \{                                                                 */

    enum { Left=0, Center, Right } HorizontalAlignE;
    enum { Top=0,  Middle, Bottom } VerticalAlignE;


    /*---------------------------------------------------------------------*/
    /*! \name                       Sync                                   */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         detail);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Dump                                  */
    /*! \{                                                                 */

    virtual void dump(      UInt32    uiIndent = 0,
                      const BitVector bvFlags  = 0) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name           satisfying label rendering interface               */
    /*! \{                                                                 */

    virtual void onPreLabelRendering(void);
    virtual void onLabelRendering(DrawEnv *drawEnv);
    virtual void onPostLabelRendering(void);
    virtual Vec2f screenRect(void);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef IconLabelBase Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    IconLabel(void);
    IconLabel(const IconLabel &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~IconLabel(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Init                                       */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name              Intersect & Render                              */
    /*! \{                                                                 */

    Action::ResultE intersectEnter(Action *action);

    Action::ResultE renderEnter   (Action *action);
    Action::ResultE renderLeave   (Action *action);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name           label rendering helper functions                   */
    /*! \{                                                                 */

    TextureObjChunkRefPtr getTextureObjChunk(void);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:
    friend class FieldContainer;
    friend class IconLabelBase;

    /*---------------------------------------------------------------------*/

    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const IconLabel &source);
};

typedef IconLabel *IconLabelP;

OSG_END_NAMESPACE

#include "OSGIconLabelBase.inl"
#include "OSGIconLabel.inl"

#endif /* _OSGICONLABEL_H_ */
