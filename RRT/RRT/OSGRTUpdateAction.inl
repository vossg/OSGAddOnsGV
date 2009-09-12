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


/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

template<typename DescT>
RTUpdateAction<DescT> *RTUpdateAction<DescT>::_pPrototype = NULL;

template<typename DescT>
std::vector<Action::Functor> *
    RTUpdateAction<DescT>::_vDefaultEnterFunctors = NULL;

template<typename DescT>
std::vector<Action::Functor> *
    RTUpdateAction<DescT>::_vDefaultLeaveFunctors = NULL;



/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/


/*-------------------------------------------------------------------------*\
 -  public                                                                 -
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*\
 -  protected                                                              -
\*-------------------------------------------------------------------------*/

template<typename DescT> inline
bool RTUpdateAction<DescT>::terminateEnter(void)
{
    FDEBUG_GV(("Terminate RTUpdateAction Enter\n"));

    delete _vDefaultEnterFunctors;

    _vDefaultEnterFunctors = NULL;

    return true;
}

template<typename DescT> inline
bool RTUpdateAction<DescT>::terminateLeave(void)
{
    FDEBUG_GV(("Terminate RTUpdateAction Leave\n"));

    delete _vDefaultLeaveFunctors;

    _vDefaultLeaveFunctors = NULL;

    return true;
}

template<typename DescT> inline
void RTUpdateAction<DescT>::registerEnterDefault(
    const FieldContainerType &type,
    const Action::Functor    &func)
{
    if(_vDefaultEnterFunctors == NULL)
    {
        _vDefaultEnterFunctors = new std::vector<Action::Functor>;

        addPostFactoryExitFunction(&RTUpdateAction::terminateEnter);
    }

    while(type.getId() >= _vDefaultEnterFunctors->size())
    {
        _vDefaultEnterFunctors->push_back(&Action::_defaultEnterFunction);
    }

    fprintf(stderr, "Register rtupdate for %s\n", type.getCName());

    (*_vDefaultEnterFunctors)[type.getId()] = func;
}

template<typename DescT> inline
void RTUpdateAction<DescT>::registerLeaveDefault(
    const FieldContainerType &type,
    const Action::Functor    &func)
{
    if(_vDefaultLeaveFunctors == NULL)
    {
        _vDefaultLeaveFunctors = new std::vector<Action::Functor>;

        addPostFactoryExitFunction(&RTUpdateAction::terminateLeave);
    }

    while(type.getId() >= _vDefaultLeaveFunctors->size())
    {
        _vDefaultLeaveFunctors->push_back(&Action::_defaultLeaveFunction);
    }

    FDEBUG_GV(("Register rtupdate leave for %s\n", type.getCName()));

    (*_vDefaultLeaveFunctors)[type.getId()] = func;
}

template<typename DescT> inline
void RTUpdateAction<DescT>::setPrototype(RTUpdateAction *pPrototype)
{
    _pPrototype = pPrototype;
}

template<typename DescT> inline
RTUpdateAction<DescT> *RTUpdateAction<DescT>::getPrototype(void)
{
    return _pPrototype;
}

template<typename DescT> inline
RTUpdateAction<DescT> *RTUpdateAction<DescT>::create(void)
{
    RTUpdateAction *returnValue;

    if(_pPrototype)
    {
        returnValue = new RTUpdateAction(*_pPrototype);
    }
    else
    {
        returnValue = new RTUpdateAction();
    }

    return returnValue;
}

/*-------------------------------------------------------------------------*\
 -  private                                                                -
\*-------------------------------------------------------------------------*/


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  public                                                                 -
\*-------------------------------------------------------------------------*/

template<typename DescT> inline
RTUpdateAction<DescT>::~RTUpdateAction(void)
{
}

template<typename DescT> inline
void RTUpdateAction<DescT>::dropGeometry(GeometryPtr pGeo)
{
    if(_pCurrentCache != NULL)
    {
//        _pCurrentCache->addGeometry(pGeo);
    }
}

template<typename DescT> inline
ActionBase::ResultE RTUpdateAction<DescT>::start(void)
{
    return Action::Continue;
}

template<typename DescT> inline
ActionBase::ResultE RTUpdateAction<DescT>::stop(ActionBase::ResultE res)
{
    return Action::Continue;
}


template<typename DescT> inline
void RTUpdateAction<DescT>::setRayTracer(RTRayTracer *pRayTracer)
{
    _pRayTracer = pRayTracer;
}

template<typename DescT> inline
typename RTUpdateAction<DescT>::RTRayTracer *
    RTUpdateAction<DescT>::getRayTracer(void)
{
    return _pRayTracer;
}


/*-------------------------------------------------------------------------*\
 -  protected                                                              -
\*-------------------------------------------------------------------------*/

template<typename DescT> inline
RTUpdateAction<DescT>::RTUpdateAction(void) :
     Inherited    (    ),
    _pRayTracer   (NULL),
    _pCurrentCache(NULL),
    _pCacheNode   (NULL)
{
    if(_vDefaultEnterFunctors != NULL)
        _enterFunctors = *_vDefaultEnterFunctors;

    if(_vDefaultLeaveFunctors != NULL)
        _leaveFunctors = *_vDefaultLeaveFunctors;

    _nodeEnterCB = boost::bind(&RTUpdateAction::nodeEnter,
                               this, 
                               _1,
                               _2);

    _nodeLeaveCB = boost::bind(&RTUpdateAction::nodeExit,
                               this, 
                               _1,
                               _2);
}

template<typename DescT> inline
RTUpdateAction<DescT>::RTUpdateAction(const RTUpdateAction &source) :
     Inherited    (source),
    _pRayTracer   (NULL  ),
    _pCurrentCache(NULL  ),
    _pCacheNode   (NULL  )
{
    if(_vDefaultEnterFunctors != NULL)
        _enterFunctors = *_vDefaultEnterFunctors;

    if(_vDefaultLeaveFunctors != NULL)
        _leaveFunctors = *_vDefaultLeaveFunctors;
}

template<typename DescT> inline
ActionBase::ResultE RTUpdateAction<DescT>::nodeEnter(NodePtrConstArg  pNode, 
                                                     Action          *pAction)
{
    CacheAttachmentPtr pRTCacheAtt = 
        dynamic_cast<CacheAttachmentPtr>(
            pNode->findAttachment(CacheAttachment::getClassType()));

    if(pRTCacheAtt != NULL)
    {
        _pRayTracer->addCache(pRTCacheAtt->getCache());

        return Action::Skip;
    }

    return Action::Continue;
}

template<typename DescT> inline
ActionBase::ResultE RTUpdateAction<DescT>::nodeExit (NodePtrConstArg  pNode, 
                                                     Action          *pAction)
{
    return Action::Continue;
}

/*-------------------------------------------------------------------------*\
 -  private                                                                -
\*-------------------------------------------------------------------------*/



template<typename DescT> inline
ActionBase::ResultE GeometryRTUpdateEnter(const NodeCorePtr &pCore,
                                                Action      *action)
{
#if 0
    fprintf(stderr, "Enter GeoRTInit %p\n", &(*pCore));

    GeometryPtr   pGeo    = dynamic_cast<GeometryPtr   >(pCore);
    RTInitAction *pRTInit = dynamic_cast<RTInitAction *>(action);

    if(pRTInit != NULL)
    {
        pRTInit->dropGeometry(pGeo);
    }
#endif

    return ActionBase::Continue;
}

template<typename DescT> inline
ActionBase::ResultE GeometryRTUpdateLeave(const NodeCorePtr &pCore,
                                                Action      *action)
{
#if 0
    fprintf(stderr, "Leave GeoRTInit %p\n", &(*pCore));
#endif

    return ActionBase::Continue;
}


template<typename DescT> inline
ActionBase::ResultE VisitSubTreeRTUpdateEnter(const NodeCorePtr &pCore,
                                                    Action      *action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Enter Group %p\n", &(*pCore)));
#endif
//    RenderAction *a = dynamic_cast<RenderAction *>(action);

    VisitSubTreePtr pVisit  = dynamic_cast<VisitSubTreePtr>(pCore);

    action->useNodeList();
    
    action->addNode(pVisit->getSubTreeRoot());

    return ActionBase::Continue;
}


template<typename DescT> inline
bool RTUpdateActionInitialize(void)
{
#if 0
    RTInitAction::registerEnterDefault(
        Geometry::getClassType(), 
        GeometryRTInitEnter);

    RTInitAction::registerLeaveDefault( 
        Geometry::getClassType(), 
        GeometryRTInitLeave);
#endif

    RTUpdateAction<DescT>::registerEnterDefault(
        VisitSubTree::getClassType(), 
        VisitSubTreeRTUpdateEnter<DescT>);

    return true;
}

OSG_END_NAMESPACE
