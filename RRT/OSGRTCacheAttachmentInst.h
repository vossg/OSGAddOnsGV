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

#ifndef _OSGRTCACHEATTACHMENTINST_H_
#define _OSGRRCACHEATTACHMENTINST_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTCacheAttachment.h"
#include "OSGGeometry.h"

OSG_BEGIN_NAMESPACE

/*! \brief RTCacheAttachmentInst class. See \ref
           PageContribRRTRayTracer for a description.
*/

template<typename DescT>
class  RTCacheAttachmentInst : public RTCacheAttachment
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTCacheAttachment        Inherited;
    typedef RTCacheAttachmentInst    Self;

    typedef DescT                    Desc;

    typedef typename Desc::RTCache   RTCache;

    typedef typename RTCache::SField RTCacheField;

    OSG_GEN_INTERNALPTR(Self);

    enum 
    { 
        CacheFieldId      = Inherited::NextFieldId, 
        NextFieldId       = CacheFieldId + 1
    };

    static const BitVector CacheFieldMask = Inherited::NextFieldMask;
    static const BitVector NextFieldMask  = CacheFieldMask << 1;

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    OSG_FIELD_CONTAINER_TMPL_DECL;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    void     addGeometry(GeometryPtr    pGeo,
                         Matrixr       &oMatrix,
                         State         *pState,
                         StateOverride *pStateOverride);
    
    RTCache *getCache   (void            );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void buildStructure(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details   );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

           RTCacheField _sfCache;

    static TypeObject   _type;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RTCacheAttachmentInst(void);
    RTCacheAttachmentInst(const RTCacheAttachmentInst &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~RTCacheAttachmentInst(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    virtual void resolveLinks(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    EditFieldHandlePtr editHandleCache(void);
    GetFieldHandlePtr  getHandleCache (void) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void classDescInserter(TypeObject &oType );
    static void initMethod       (InitPhase   ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const RTCacheAttachmentInst &source);
};

OSG_END_NAMESPACE

#define OSG_RTCACHEATTACHMENT_INST(DESC)                                      \
template <>                                                                   \
RTCacheAttachmentInst< DESC >::TypeObject                                     \
    RTCacheAttachmentInst< DESC >::_type(                                     \
        Desc::getTypeName      (),                                            \
        Desc::getParentTypeName(),                                            \
        Desc::getGroupName     (),                                            \
        0,                                                                    \
        (PrototypeCreateF) &Self::createEmpty,                                \
        NULL,                                                                 \
        (InitalInsertDescFunc) &Self::classDescInserter,                      \
        true,                                                                 \
        0);                                                                   \
                                                                              \
template<>                                                                    \
RTCacheAttachmentInst< DESC >::TypeObject &                                   \
    RTCacheAttachmentInst< DESC >::getClassType(void)                         \
{                                                                             \
    return Self::_type;                                                       \
}                                                                             \
                                                                              \
template<>                                                                    \
RTCacheAttachmentInst< DESC >::TypeObject &                                   \
    RTCacheAttachmentInst< DESC >::getType(void)                              \
{                                                                             \
    return Self::_type;                                                       \
}                                                                             \
                                                                              \
template<>                                                                    \
const RTCacheAttachmentInst< DESC >::TypeObject &                             \
    RTCacheAttachmentInst< DESC >::getType(void) const                        \
{                                                                             \
    return Self::_type;                                                       \
}


#include "OSGRTCacheAttachmentInst.inl"

#endif /* _OSGRTCACHEATTACHMENTINST_H_ */
