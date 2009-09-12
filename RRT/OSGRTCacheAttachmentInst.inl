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

OSG_BEGIN_NAMESPACE

/*---------------------------------------------------------------------*/


template<typename DescT> inline
void RTCacheAttachmentInst<DescT>::addGeometry(GeometryPtr    pGeo,
                                               Matrixr       &oMatrix,
                                               State         *pState,
                                               StateOverride *pStateOverride)
{
    if(_pCache == NULL)
    {
        _pCache = new RTCache;
        
        OSG::addRef(_pCache);
    }

    _pCache->addGeometry(pGeo, oMatrix, pState, pStateOverride);
}

template<typename DescT> inline
typename RTCacheAttachmentInst<DescT>::RTCache *
    RTCacheAttachmentInst<DescT>::getCache(void)
{
    return _pCache;
}

/*---------------------------------------------------------------------*/

template<typename DescT> inline
void RTCacheAttachmentInst<DescT>::buildStructure(void)
{
    if(_pCache != NULL)
    {
        _pCache->buildStructure();
    }
}


/*---------------------------------------------------------------------*/

template<typename DescT> inline
void RTCacheAttachmentInst<DescT>::changed(ConstFieldMaskArg whichField,
                                           UInt32            origin,
                                           BitVector         details   )
{
    Inherited::changed(whichField, origin, details);
}

template<typename DescT> inline
void RTCacheAttachmentInst<DescT>::dump(      UInt32    ,
                                        const BitVector ) const
{
    SLOG << "Dump RTCacheAttachment NI" << std::endl;
}

/*---------------------------------------------------------------------*/

OSG_FIELD_CONTAINER_TMPL_NO_TYPE_DEF  (RTCacheAttachmentInst, DescT)
OSG_RC_GET_STATIC_TYPE_ID_INL_TMPL_DEF(RTCacheAttachmentInst, DescT)

template<typename DescT> inline
RTCacheAttachmentInst<DescT>::RTCacheAttachmentInst(void) :
     Inherited(    ),
    _pCache   (NULL)
{
}

template<typename DescT> inline
RTCacheAttachmentInst<DescT>::RTCacheAttachmentInst(
    const RTCacheAttachmentInst &source) :

     Inherited(source),
    _pCache   (NULL  )
{
}

template<typename DescT> inline
RTCacheAttachmentInst<DescT>::~RTCacheAttachmentInst(void)
{
}


template<typename DescT> inline
void RTCacheAttachmentInst<DescT>::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
    }
}

template<typename DescT> inline
void RTCacheAttachmentInst<DescT>::resolveLinks(void)
{
    Inherited::resolveLinks();

    OSG::subRef(_pCache);
}

/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/

OSG_END_NAMESPACE
