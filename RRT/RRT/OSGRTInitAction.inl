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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

template<typename DescT>
RTInitAction<DescT> *RTInitAction<DescT>::_pPrototype = NULL;

template<typename DescT>
std::vector<Action::Functor> *
    RTInitAction<DescT>::_vDefaultEnterFunctors = NULL;

template<typename DescT>
std::vector<Action::Functor> *
    RTInitAction<DescT>::_vDefaultLeaveFunctors = NULL;


/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

template<typename DescT> inline
bool RTInitAction<DescT>::terminateEnter(void)
{
    FDEBUG_GV(("Terminate RTInitAction Enter\n"));

    delete _vDefaultEnterFunctors;

    _vDefaultEnterFunctors = NULL;

    return true;
}

template<typename DescT> inline
bool RTInitAction<DescT>::terminateLeave(void)
{
    FDEBUG_GV(("Terminate RTInitAction Leave\n"));

    delete _vDefaultLeaveFunctors;

    _vDefaultLeaveFunctors = NULL;

    return true;
}

template<typename DescT> inline
void RTInitAction<DescT>::registerEnterDefault(const FieldContainerType &type,
                                               const Action::Functor    &func)
{
    if(_vDefaultEnterFunctors == NULL)
    {
        _vDefaultEnterFunctors = new std::vector<Action::Functor>;

        addPostFactoryExitFunction(&RTInitAction::terminateEnter);
    }

    while(type.getId() >= _vDefaultEnterFunctors->size())
    {
        _vDefaultEnterFunctors->push_back(&Action::_defaultEnterFunction);
    }

    fprintf(stderr, "Register rtinit for %s\n", type.getCName());

    (*_vDefaultEnterFunctors)[type.getId()] = func;
}

template<typename DescT> inline
void RTInitAction<DescT>::registerLeaveDefault(const FieldContainerType &type,
                                               const Action::Functor    &func)
{
    if(_vDefaultLeaveFunctors == NULL)
    {
        _vDefaultLeaveFunctors = new std::vector<Action::Functor>;

        addPostFactoryExitFunction(&RTInitAction::terminateLeave);
    }

    while(type.getId() >= _vDefaultLeaveFunctors->size())
    {
        _vDefaultLeaveFunctors->push_back(&Action::_defaultLeaveFunction);
    }

    FDEBUG_GV(("Register rtinit leave for %s\n", type.getCName()));

    (*_vDefaultLeaveFunctors)[type.getId()] = func;
}

template<typename DescT> inline
void RTInitAction<DescT>::setPrototype(RTInitAction *pPrototype)
{
    _pPrototype = pPrototype;
}

template<typename DescT> inline
RTInitAction<DescT> *RTInitAction<DescT>::getPrototype(void)
{
    return _pPrototype;
}

template<typename DescT> inline
RTInitAction<DescT> *RTInitAction<DescT>::create(void)
{
    RTInitAction *returnValue;

    if(_pPrototype)
    {
        returnValue = new RTInitAction(*_pPrototype);
    }
    else
    {
        returnValue = new RTInitAction();
    }

    return returnValue;
}

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/


/*-------------------------------------------------------------------------*\
 -  public                                                                 -
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 -  protected                                                              -
\*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*\
 -  private                                                                -
\*-------------------------------------------------------------------------*/


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  public                                                                 -
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 -  protected                                                              -
\*-------------------------------------------------------------------------*/

template<typename DescT> inline
RTInitAction<DescT>::RTInitAction(void) :
     Inherited      (               ),
    _uiInitMode     (AutoTriangulate),
    _pCurrentCache  (NULL           ),
    _pCacheNode     (NULL           ),
    _uiMatrixId     (0              ),
    _currMatrix     (               ),
    _vMatrixStack   (               ),
    _pMaterial      (NULL           ),
    _pMaterialNode  (NULL           ),
    _iNextLightIndex(0              ),
    _pStatePool     (               ),
    _sStateOverrides(               )
{
    if(_vDefaultEnterFunctors != NULL)
        _enterFunctors = *_vDefaultEnterFunctors;

    if(_vDefaultLeaveFunctors != NULL)
        _leaveFunctors = *_vDefaultLeaveFunctors;

    _nodeEnterCB = boost::bind(&RTInitAction::nodeEnter,
                               this, 
                               _1,
                               _2);

    _nodeLeaveCB = boost::bind(&RTInitAction::nodeExit,
                               this, 
                               _1,
                               _2);

    _pStatePool = new StateOverridePool;
}

template<typename DescT> inline
RTInitAction<DescT>::RTInitAction(const RTInitAction &source) :
     Inherited      (source            ),
    _uiInitMode     (source._uiInitMode),
    _pCurrentCache  (NULL              ),
    _pCacheNode     (NULL              ),
    _uiMatrixId     (0                 ),
    _currMatrix     (                  ),
    _vMatrixStack   (                  ),
    _pMaterial      (NULL              ),
    _pMaterialNode  (NULL              ),
    _iNextLightIndex(0                 ),
    _pStatePool     (                  ),
    _sStateOverrides(                  )
{
    if(_vDefaultEnterFunctors != NULL)
        _enterFunctors = *_vDefaultEnterFunctors;

    if(_vDefaultLeaveFunctors != NULL)
        _leaveFunctors = *_vDefaultLeaveFunctors;

    _pStatePool = new StateOverridePool;
}

template<typename DescT> inline
ActionBase::ResultE RTInitAction<DescT>::nodeEnter(Node   * const  pNode, 
                                                   Action *        pAction)
{
    RTInfoAttachment *pRTInfo = 
        dynamic_cast<RTInfoAttachment *>(
            pNode->findAttachment(RTInfoAttachment::getClassType()));

    if(pRTInfo != NULL)
    {
        if(_pCurrentCache == NULL)
        {
            _pCurrentCache = CacheAttachment::create();
            _pCacheNode    = pNode;

            _uiMatrixId = 1;
        
            _currMatrix.first = 1;
            _currMatrix.second.setIdentity();
            
            _vMatrixStack.clear();

            _pMaterial     = NULL;
            _pMaterialNode = NULL;

            _iNextLightIndex = 0;


            _pStatePool->freeAll();

            while(_sStateOverrides.empty() == false)
            {
                _sStateOverrides.pop();
            }
        }
    }

    return Action::Continue;
}

template<typename DescT> inline
ActionBase::ResultE RTInitAction<DescT>::nodeExit(Node   * const  pNode, 
                                                  Action *        pAction)
{
    if(pNode == _pCacheNode)
    {
        pNode->addAttachment(_pCurrentCache);

        _pCurrentCache->buildStructure();

        _pCurrentCache = NULL;
        _pCacheNode    = NULL;
    }

    return Action::Continue;
}

template<typename DescT> inline
RTInitAction<DescT>::~RTInitAction(void)
{
    delete _pStatePool;
}

template<typename DescT> inline
void RTInitAction<DescT>::pushMatrix(const Matrix &matrix)
{
    if(_pCurrentCache != NULL)
    {
        _vMatrixStack.push_back(_currMatrix);
        
        _currMatrix.first = ++_uiMatrixId;
        _currMatrix.second.mult(matrix);
    }
}

template<typename DescT> inline
void RTInitAction<DescT>::popMatrix(void)
{
    if(_pCurrentCache != NULL)
    {
        _currMatrix.first  = _vMatrixStack.back().first;
        _currMatrix.second = _vMatrixStack.back().second;
        
        _vMatrixStack.pop_back();
    }
}

template<typename DescT> inline
void RTInitAction<DescT>::overrideMaterial(Material *       pMaterial,
                                           Node     * const pNode    )
{
    if(_pCurrentCache != NULL)
    {
        if(_pMaterialNode == NULL)
        {
            _pMaterial     = pMaterial;
            _pMaterialNode = pNode;
        }
        else if(_pMaterialNode == pNode)
        {
            if(pMaterial == NULL)
            {
                _pMaterial     = NULL;
                _pMaterialNode = NULL;
            }
            else
            {
                fprintf(stderr, "strange reset with non null mat\n");
            }
        }
    }
}

template<typename DescT> inline
void RTInitAction<DescT>::pushState(void)
{
    if(_pCurrentCache != NULL)
    {
        StateOverride *pNewState = _pStatePool->create();
    
        if(_sStateOverrides.empty() == false)
            pNewState->fillFrom(_sStateOverrides.top());
        
        _sStateOverrides.push(pNewState);
    }
}

template<typename DescT> inline
void RTInitAction<DescT>::popState(void)
{
    if(_pCurrentCache != NULL)
    {
        _sStateOverrides.pop();
    }
}

template<typename DescT> inline
void RTInitAction<DescT>::addOverride(UInt32 uiSlot, StateChunk *pChunk)
{
    if(_pCurrentCache != NULL)
    {
        if(_sStateOverrides.empty() == true)
        {
            StateOverride *pNewState = _pStatePool->create();
            
            _sStateOverrides.push(pNewState);
        }
        
        _sStateOverrides.top()->addOverride(uiSlot, pChunk);
    }
}

template<typename DescT> inline
Int32 RTInitAction<DescT>::allocateLightIndex(void)
{
    if(_pCurrentCache != NULL)
    {
        Int32 returnValue = _iNextLightIndex++;

        if(_iNextLightIndex > 7)
        {
            return -_iNextLightIndex;
        }
        else
        {
            return returnValue;
        }
    }
    else
    {
        return -1;
    }
}

template<typename DescT> inline
void RTInitAction<DescT>::releaseLightIndex(void)
{
    if(_pCurrentCache != NULL)
    {
        --_iNextLightIndex;
    }
}

inline
bool isTriangulated(Geometry *pGeo)
{
    bool returnValue = false;

    if(pGeo == NULL)
        return returnValue;

    GeoIntegralProperty *pTypes = pGeo->getTypes();

    if(pTypes == NULL)
        return returnValue;

    returnValue = true;

    for(UInt32 i = 0; i < pTypes->size(); ++i)
    {
        UInt32 uiType = pTypes->getValue(i);

        if(uiType != GL_TRIANGLES)
        {
            fprintf(stderr, "Got %d %d\n", i, uiType);

            returnValue = false;
            break;
        }
    }

    return returnValue;
}

inline
void triangulateSimple(Geometry *pGeo)
{
    if(pGeo == NULL)
        return;

    Geometry::IndexBag sourceIndexBag = pGeo->getUniqueIndexBag();

    std::vector<GeoIntegralPropertyUnrecPtr> targetIndexBag;

    for(UInt32 i = 0; i < sourceIndexBag.size(); ++i)
    {
        targetIndexBag.push_back(
            dynamic_pointer_cast<GeoIntegralProperty>(
                sourceIndexBag[i].first->shallowCopy()));

        targetIndexBag.back()->clear();
    }

    UInt32 uiTriCount = 0;

    for(TriangleIterator t  = pGeo->beginTriangles(); 
                         t != pGeo->endTriangles  ();
                       ++t)
    {
        for(UInt32 i = 0; i < sourceIndexBag.size(); ++i)
        {
            Int32 iIdx;

            for(UInt32 j = 0; j < 3; ++j)
            {
                iIdx = t.getPropertyIndex(sourceIndexBag[i].second[0],
                                          j);

                targetIndexBag[i]->push_back(iIdx);
            }
        }


        ++uiTriCount;
    }

    for(UInt32 i = 0; i < sourceIndexBag.size(); ++i)
    {
        for(UInt32 j = 0; j < sourceIndexBag[i].second.size(); ++j)
        {
            pGeo->setIndex(targetIndexBag[i], 
                           sourceIndexBag[i].second[j]);
        }
    }


    GeoIntegralProperty *pTypes   = pGeo->getTypes  ();
    GeoIntegralProperty *pLengths = pGeo->getLengths();
    
    pTypes->clear();
    pTypes->push_back(GL_TRIANGLES);

    pLengths->clear();
    pLengths->push_back(uiTriCount * 3);
}

template<typename DescT> inline
void RTInitAction<DescT>::dropGeometry(Geometry *pGeo)
{
    if(_pCurrentCache != NULL)
    {
        StateOverride *pOverride = NULL;

        if(_sStateOverrides.empty() == false)
        {
            pOverride = _sStateOverrides.top();
        }

        Material *pMat = _pMaterial;

        if(pMat == NULL)
        {
            pMat = pGeo->getMaterial();
        }

        if(pMat == NULL)
            return;

        PrimeMaterial *pPrimeMat = pMat->finalize(0x0000);

        if(isTriangulated(pGeo) == false)
        {
            fprintf(stderr, "found untriangulated geometry\n");

            if(0x0000 != (_uiInitMode & AutoTriangulate))
            {
                fprintf(stderr, "triangulate\n");

                triangulateSimple(pGeo);


                _pCurrentCache->addGeometry(pGeo, 
                                            _currMatrix.second,
                                             pPrimeMat->getState(0),
                                             pOverride);
            }
        }
        else
        {
            _pCurrentCache->addGeometry( pGeo, 
                                        _currMatrix.second,
                                         pPrimeMat->getState(0), 
                                         pOverride);
        }
    }
}

template<typename DescT> inline
ActionBase::ResultE RTInitAction<DescT>::start(void)
{
    return Action::Continue;
}

template<typename DescT> inline
ActionBase::ResultE RTInitAction<DescT>::stop(ActionBase::ResultE res)
{
    return Action::Continue;
}



/*-------------------------------------------------------------------------*\
 -  private                                                                -
\*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

template<typename DescT> inline
ActionBase::ResultE GeometryRTInitEnter(NodeCore * const pCore,
                                        Action   *       action)
{
    fprintf(stderr, "Enter GeoRTInit %p\n", &(*pCore));

    Geometry            *pGeo    = dynamic_cast<Geometry            *>(pCore);
    RTInitAction<DescT> *pRTInit = dynamic_cast<RTInitAction<DescT> *>(action);

    if(pRTInit != NULL)
    {
        pRTInit->dropGeometry(pGeo);
    }

    return ActionBase::Continue;
}

inline
ActionBase::ResultE GeometryRTInitLeave(NodeCore * const pCore,
                                        Action   *       action)
{
    fprintf(stderr, "Leave GeoRTInit %p\n", &(*pCore));

    return ActionBase::Continue;
}


template<typename DescT> inline
ActionBase::ResultE TransformRTInitEnter(NodeCore * const pCore,
                                         Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Enter Transform %p\n", &(*pCore)));
#endif

    Transform *pThis = dynamic_cast<Transform *>(pCore);

    RTInitAction<DescT> *pRTInit = dynamic_cast<RTInitAction<DescT> *>(action);

    pRTInit->pushMatrix(pThis->getMatrix());

    return ActionBase::Continue;
}

template<typename DescT> inline
ActionBase::ResultE TransformRTInitLeave(NodeCore * const pCore,
                                         Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Leave Transform %p\n", &(*pCore)));
#endif

    RTInitAction<DescT> *pRTInit = dynamic_cast<RTInitAction<DescT> *>(action);

    pRTInit->popMatrix();

    return ActionBase::Continue;
}

template<typename DescT> inline
ActionBase::ResultE MaterialGroupRTInitEnter(NodeCore * const pCore,
                                             Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Enter MaterialGroup %p\n", &(*pCore)));
#endif

    RTInitAction<DescT> *pRTInit   = 
        dynamic_cast<RTInitAction<DescT> *>(action);

    MaterialGroup       *pMatGroup = dynamic_cast<MaterialGroup *>(pCore);

    if(pRTInit                  != NULL && 
       pMatGroup                != NULL &&
       pMatGroup->getMaterial() != NULL  )
    {
        pRTInit->overrideMaterial(pMatGroup->getMaterial(), 
                                  pRTInit->getActNode()   );
    }

    return ActionBase::Continue;
}

template<typename DescT> inline
ActionBase::ResultE MaterialGroupRTInitLeave(NodeCore * const pCore,
                                             Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Leave MaterialGroup %p\n", &(*pCore)));
#endif

    RTInitAction<DescT> *pRTInit = dynamic_cast<RTInitAction<DescT> *>(action);

    if(pRTInit != NULL)
    {
        pRTInit->overrideMaterial(NULL, pRTInit->getActNode());
    }

    return ActionBase::Continue;
}

template<typename DescT> inline
Action::ResultE LightRTInitEnter(NodeCore                * const pCore,
                                 LightEngine::LightTypeE         eType,
                                 RTInitAction<DescT>     *       action)
{    
    Action::ResultE  r           = Action::Continue;
    Light           *pLight      = dynamic_cast<Light *>(pCore);


    StateChunk      *pChunk      = pLight->getChunk();
        
    UInt32           uiSlot      = pChunk->getClassId();
        
    Int32            iLightIndex = action->allocateLightIndex();
        
    action->pushState();
 
    fprintf(stderr, "InitLight\n");
       
    if(iLightIndex >= 0)
    {
        action->addOverride(uiSlot + iLightIndex, pChunk);
    }

    return r;
}

template<typename DescT> inline
Action::ResultE LightRTInitLeave(NodeCore                * const pCore,
                                 LightEngine::LightTypeE         eType,
                                 Action                  *       action)
{
    Action::ResultE      r       = Action::Continue;
    RTInitAction<DescT> *pRTInit = dynamic_cast<RTInitAction<DescT> *>(action);

//    Light               *pLight  = dynamic_cast<Light *>(pCore);

    pRTInit->releaseLightIndex();
    pRTInit->popState();

    return r;
}

template<typename DescT> inline
Action::ResultE DirectionalLightRTInitEnter(NodeCore * const pCore,
                                            Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Enter DirLight %p\n", &(*pCore)));
#endif

    RTInitAction<DescT> *pRTInit = dynamic_cast<RTInitAction<DescT> *>(action);

    return LightRTInitEnter<DescT>(pCore, LightEngine::Directional, pRTInit);
}

template<typename DescT> inline
Action::ResultE DirectionalLightRTInitLeave(NodeCore * const pCore,
                                            Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Leave DirLight %p\n", &(*pCore)));
#endif

    return LightRTInitLeave<DescT>(pCore, LightEngine::Directional, action);
}

template<typename DescT> inline
Action::ResultE PointLightRTInitEnter(NodeCore * const pCore,
                                      Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Enter PointLight %p\n", &(*pCore)));
#endif

    RTInitAction<DescT> *pRTInit = dynamic_cast<RTInitAction<DescT> *>(action);

    return LightRTInitEnter<DescT>(pCore,
                                   LightEngine::Point,
                                   pRTInit);
}

template<typename DescT> inline
Action::ResultE PointLightRTInitLeave(NodeCore * const pCore,
                                      Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Leave PointLight %p\n", &(*pCore)));
#endif

    return LightRTInitLeave<DescT>(pCore, 
                                   LightEngine::Point,
                                   action);
}

template<typename DescT> inline
Action::ResultE SpotLightRTInitEnter(NodeCore * const pCore,
                                     Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Enter SpotLight %p\n", &(*pCore)));
#endif

    RTInitAction<DescT> *pRTInit = dynamic_cast<RTInitAction<DescT> *>(action);

    return LightRTInitEnter<DescT>(pCore,
                                   LightEngine::Spot,
                                   pRTInit);
}

template<typename DescT> inline
Action::ResultE SpotLightRTInitLeave(NodeCore * const pCore,
                                     Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Leave SpotLight %p\n", &(*pCore)));
#endif

    return LightRTInitLeave<DescT>(pCore, 
                                   LightEngine::Spot,
                                   action);
}


template<typename DescT> inline
ActionBase::ResultE GroupRTInitEnter(NodeCore * const pCore,
                                     Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Enter Group %p\n", &(*pCore)));
#endif
    return ActionBase::Continue;
}

template<typename DescT> inline
ActionBase::ResultE GroupRTInitLeave(NodeCore * const pCore,
                                     Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Leave Group %p\n", &(*pCore)));
#endif

    return ActionBase::Continue;
}

template<typename DescT> inline
ActionBase::ResultE VisitSubTreeRTInitEnter(NodeCore * const pCore,
                                            Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Enter Group %p\n", &(*pCore)));
#endif
//    RenderAction *a = dynamic_cast<RenderAction *>(action);

    VisitSubTree *pVisit  = dynamic_cast<VisitSubTree *>(pCore);

    action->useNodeList();
    
    action->addNode(pVisit->getSubTreeRoot());

    return ActionBase::Continue;
}


template<typename DescT> inline
bool RTInitActionInitialize(void)
{
    RTInitAction<DescT>::registerEnterDefault(
        Geometry::getClassType(), 
        GeometryRTInitEnter<DescT>);

    RTInitAction<DescT>::registerLeaveDefault( 
        Geometry::getClassType(), 
        GeometryRTInitLeave);

    RTInitAction<DescT>::registerEnterDefault(
        Transform::getClassType(), 
        TransformRTInitEnter<DescT>);

    RTInitAction<DescT>::registerLeaveDefault(
        Transform::getClassType(), 
        TransformRTInitLeave<DescT>);

    RTInitAction<DescT>::registerEnterDefault(
        ComponentTransform::getClassType(), 
        TransformRTInitEnter<DescT>);

    RTInitAction<DescT>::registerLeaveDefault(
        ComponentTransform::getClassType(), 
        TransformRTInitLeave<DescT>);

    RTInitAction<DescT>::registerEnterDefault(
        MaterialGroup::getClassType(), 
        MaterialGroupRTInitEnter<DescT>);

    RTInitAction<DescT>::registerLeaveDefault(
        MaterialGroup::getClassType(), 
        MaterialGroupRTInitLeave<DescT>);

    RTInitAction<DescT>::registerEnterDefault( 
        DirectionalLight::getClassType(), 
        DirectionalLightRTInitEnter<DescT>);

    RTInitAction<DescT>::registerLeaveDefault(
        DirectionalLight::getClassType(), 
        DirectionalLightRTInitLeave<DescT>);

    RTInitAction<DescT>::registerEnterDefault( 
        PointLight::getClassType(), 
        PointLightRTInitEnter<DescT>);

    RTInitAction<DescT>::registerLeaveDefault( 
        PointLight::getClassType(), 
        PointLightRTInitLeave<DescT>);


    RTInitAction<DescT>::registerEnterDefault(
        SpotLight::getClassType(), 
        SpotLightRTInitEnter<DescT>);

    RTInitAction<DescT>::registerLeaveDefault( 
        SpotLight::getClassType(), 
        SpotLightRTInitLeave<DescT>);


    RTInitAction<DescT>::registerEnterDefault(
        Group::getClassType(), 
        GroupRTInitEnter<DescT>);

    RTInitAction<DescT>::registerLeaveDefault(
        Group::getClassType(), 
        GroupRTInitLeave<DescT>);


    RTInitAction<DescT>::registerEnterDefault(
        VisitSubTree::getClassType(), 
        VisitSubTreeRTInitEnter<DescT>);


    return true;
}



OSG_END_NAMESPACE
