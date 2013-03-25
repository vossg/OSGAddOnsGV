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

#ifndef _OSGLABELFOREGROUND_H_
#define _OSGLABELFOREGROUND_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGLabelForegroundBase.h"
#include "OSGAction.h"

OSG_BEGIN_NAMESPACE

class TextLayoutResult;

/*! \brief Simple Statistics Foreground class. See \ref
    PageSystemWindowForegroundStatisticsSimple for a description.
*/

class OSG_CONTRIBLABELING_DLLMAPPING LabelForeground : public LabelForegroundBase
{
  public:

    typedef LabelForegroundBase                    Inherited;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Draw                                    */
    /*! \{                                                                 */

    virtual void draw(DrawEnv  *pEnv);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name               Convenience Functions                          */
    /*! \{                                                                 */

    Action::ResultE traverseEnter(Node * const node);
    Action::ResultE traverseLeave(Node * const node, Action::ResultE res);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in LabelForegroundBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    LabelForeground(void);
    LabelForeground(const LabelForeground &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~LabelForeground(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Init                                   */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    DrawEnv*             _cachedDrawEnv;
//    Viewport*            _cachedViewport;
    std::vector<Matrix>  _matrixStack;

    friend class FieldContainer;
    friend class LabelForegroundBase;

    void drawCharacters(const TextLayoutResult&  layoutResult,
                              class TextTXFFace* face) const;

    void drawLabel(Node* labelNode) const;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const LabelForeground &source);
};

typedef LabelForeground       *LabelForegroundP;

OSG_END_NAMESPACE

#include "OSGLabelForegroundBase.inl"
#include "OSGLabelForeground.inl"

#endif /* _OSGLABELFOREGROUND_H_ */
