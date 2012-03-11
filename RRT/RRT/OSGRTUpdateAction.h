/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *           Copyright (C) 2003 by the OpenSG Forum                          *
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

#ifndef _OSGRTUPDATEACTION_H_
#define _OSGRTUPDATEACTION_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGBaseTypes.h"
#include "OSGAction.h"
#include "OSGContribRRTDef.h"
#include "OSGGeometry.h"
#include "OSGVisitSubTree.h"

#include "OSGRTCacheAttachment.h"

OSG_BEGIN_NAMESPACE

class RayTracer;

/*! \brief Base class using the render action interface of window
 */

template<typename DescT>
class RTUpdateAction : public Action
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef          DescT                    Desc;

    typedef typename Desc::CacheAttachment    CacheAttachment;
    typedef typename CacheAttachment::ObjCPtr CacheAttachmentPtr;

    typedef typename Desc::RTRayTracer        RTRayTracer;

    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTUpdateAction(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name               Rendering Environment                          */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Culling                                    */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Lights                                     */
    /*! \{                                                                 */

    void dropGeometry(Geometry *pGeo);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Lights                                     */
    /*! \{                                                                 */

    virtual Action::ResultE start(void               );
    virtual Action::ResultE stop (Action::ResultE res);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Lights                                     */
    /*! \{                                                                 */

    void         setRayTracer(RTRayTracer *pRayTracer);
    RTRayTracer *getRayTracer(void                 );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Lights                                     */
    /*! \{                                                                 */

    static
    RTUpdateAction *create      (void                      );

    static
    void            setPrototype(RTUpdateAction *pPrototype);

    static
    RTUpdateAction *getPrototype(void                      );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Lights                                     */
    /*! \{                                                                 */

    static void registerEnterDefault (const FieldContainerType &type,
                                      const Action::Functor    &func      );

    static void registerLeaveDefault (const FieldContainerType &type,
                                      const Action::Functor    &func      );
    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    //-----------------------------------------------------------------------
    //   class variables
    //-----------------------------------------------------------------------

    static RTUpdateAction        *_pPrototype;

    static std::vector<Functor>  *_vDefaultEnterFunctors;
    static std::vector<Functor>  *_vDefaultLeaveFunctors;


    //-----------------------------------------------------------------------
    //   class functions
    //-----------------------------------------------------------------------

    static bool terminateEnter(void);
    static bool terminateLeave(void);

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    RTUpdateAction(void);
    RTUpdateAction(const RTUpdateAction &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Internal updates                           */
    /*! \{                                                                 */

    Action::ResultE nodeEnter(Node * const pNode, Action *pAction);
    Action::ResultE nodeExit (Node * const pNode, Action *pAction);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Internal updates                           */
    /*! \{                                                                 */

    RTRayTracer        *_pRayTracer;
    CacheAttachmentPtr  _pCurrentCache;
    Node               *_pCacheNode;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/

    //-----------------------------------------------------------------------
    //   instance variables                                                  
    //-----------------------------------------------------------------------

    /*==========================  PRIVATE  ================================*/

  private:

    typedef Action Inherited;

    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const RTUpdateAction &source);
};

OSG_END_NAMESPACE

#define OSG_RTUPDATEACTION_INST(DESC, NAME)                           \
                                                                      \
struct RTUpdateActionInit##NAME                                       \
{                                                                     \
    RTUpdateActionInit##NAME(void)                                    \
    {                                                                 \
        addPostFactoryInitFunction(RTUpdateActionInitialize< DESC >); \
    }                                                                 \
};                                                                    \
                                                                      \
static RTUpdateActionInit##NAME initRTUpdateActionDummy##NAME;

#include "OSGRTUpdateAction.inl"

#endif /* _OSGRTUPDATEACTION_H_ */
