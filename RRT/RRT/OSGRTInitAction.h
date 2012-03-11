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

#ifndef _OSGRTINITACTION_H_
#define _OSGRTINITACTION_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGBaseTypes.h"
#include "OSGAction.h"
#include "OSGContribRRTDef.h"

#include "OSGRTCacheAttachment.h"
#include "OSGStateOverridePool.h"

#include "OSGRTInfoAttachment.h"
#include "OSGTriangleIterator.h"

#include "OSGGeometry.h"
#include "OSGTransform.h"
#include "OSGComponentTransform.h"
#include "OSGMaterialGroup.h"
#include "OSGLightEngine.h"
#include "OSGPointLight.h"
#include "OSGDirectionalLight.h"
#include "OSGSpotLight.h"
#include "OSGVisitSubTree.h"
#include "OSGPrimeMaterial.h"

#include <stack>

OSG_BEGIN_NAMESPACE

/*! \brief Base class using the render action interface of window
 */

template<typename DescT>
class RTInitAction : public Action
{
    /*==========================  PUBLIC  =================================*/

  public:

    enum InitModes
    {
        AutoTriangulate = 0x0001
    };

    typedef          DescT                        Desc;

    typedef typename Desc::CacheAttachment        CacheAttachment;
    typedef typename CacheAttachment::ObjCPtr     CacheAttachmentPtr;
    typedef typename CacheAttachment::ObjUnrecPtr CacheAttachmentUnrecPtr;

    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual ~RTInitAction(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name               Rendering Environment                          */
    /*! \{                                                                 */

    void pushMatrix (const Matrix &matrix);
    void popMatrix  (      void          );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Culling                                    */
    /*! \{                                                                 */

    void overrideMaterial(Material *        pMaterial,
                          Node     * const  pNode    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Culling                                    */
    /*! \{                                                                 */

    void pushState  (void                             );
    void popState   (void                             );

    void addOverride(UInt32 uiSlot, StateChunk *pChunk);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Culling                                    */
    /*! \{                                                                 */

    Int32 allocateLightIndex(void);
    void  releaseLightIndex (void);

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

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Lights                                     */
    /*! \{                                                                 */

    static
    RTInitAction *create      (void                    );

    static
    void          setPrototype(RTInitAction *pPrototype);

    static
    RTInitAction *getPrototype(void                    );

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

    typedef std::pair<UInt32, Matrix>               MatrixStore;
    typedef std::vector<MatrixStore>                MatrixStack;

    typedef std::stack<StateOverride *>             OverrideStack;

    //-----------------------------------------------------------------------
    //   class variables
    //-----------------------------------------------------------------------

    static RTInitAction          *_pPrototype;

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

    RTInitAction(void);
    RTInitAction(const RTInitAction &source);

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

    UInt32                   _uiInitMode;
    CacheAttachmentUnrecPtr  _pCurrentCache;
    Node                    *_pCacheNode;

    UInt32                   _uiMatrixId;
    MatrixStore              _currMatrix;
    MatrixStack              _vMatrixStack;

    Material                *_pMaterial;
    Node                    *_pMaterialNode;

    Int32                    _iNextLightIndex;

    StateOverridePool       *_pStatePool;
    
    OverrideStack            _sStateOverrides;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/

    //-----------------------------------------------------------------------
    //   instance variables                                                  
    //-----------------------------------------------------------------------

    /*==========================  PRIVATE  ================================*/

  private:

    typedef Action Inherited;

    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const RTInitAction &source);
};

OSG_END_NAMESPACE

#define OSG_RTINITACTION_INST(DESC, NAME)                             \
                                                                      \
struct RTInitActionInit##NAME                                         \
{                                                                     \
    RTInitActionInit##NAME(void)                                      \
    {                                                                 \
        addPostFactoryInitFunction(RTInitActionInitialize< DESC >);   \
    }                                                                 \
};                                                                    \
                                                                      \
static RTInitActionInit##NAME initRTInitActionDummy##NAME;

#include "OSGRTInitAction.inl"

#endif /* _OSGRTINITACTION_H_ */
