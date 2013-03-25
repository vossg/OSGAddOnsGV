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

#ifndef _OSGBUFFERPNT3FINTERPOLATOR_H_
#define _OSGBUFFERPNT3FINTERPOLATOR_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGBufferPnt3fInterpolatorBase.h"

OSG_BEGIN_NAMESPACE

/*! \brief BufferPnt3fInterpolator class. See \ref
           PageSystemDynamicsBufferPnt3fInterpolator for a description.
*/

class OSG_CONTRIBCOMPUTE_DLLMAPPING BufferPnt3fInterpolator : 
    public BufferPnt3fInterpolatorBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef BufferPnt3fInterpolatorBase Inherited;
    typedef BufferPnt3fInterpolator     Self;

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
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in CoordinateInterpolatorBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    BufferPnt3fInterpolator(void);
    BufferPnt3fInterpolator(const BufferPnt3fInterpolator &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~BufferPnt3fInterpolator(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class BufferPnt3fInterpolatorBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const BufferPnt3fInterpolator &source);
};

typedef BufferPnt3fInterpolator *BufferPnt3fInterpolatorP;

OSG_END_NAMESPACE

#include "OSGBufferPnt3fInterpolatorBase.inl"
#include "OSGBufferPnt3fInterpolator.inl"

#endif /* _OSGBUFFERPNT3FINTERPOLATOR_H_ */
