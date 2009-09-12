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

/*****************************************************************************\
 *****************************************************************************
 **                                                                         **
 **                  This file is automatically generated.                  **
 **                                                                         **
 **          Any changes made to this file WILL be lost when it is          **
 **           regenerated, which can become necessary at any time.          **
 **                                                                         **
 *****************************************************************************
\*****************************************************************************/


#ifndef _OSGRTCACHEATTACHMENTFIELDS_H_
#define _OSGRTCACHEATTACHMENTFIELDS_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGConfig.h"
#include "OSGContribRRTDef.h"

#include "OSGFieldContainerFields.h"
#include "OSGFieldContainerPtrSField.h"
#include "OSGFieldContainerPtrMField.h"


OSG_BEGIN_NAMESPACE

class RTCacheAttachment;

#if !defined(OSG_DO_DOC) // created as a dummy class, remove to prevent doubles
//! RTCacheAttachmentPtr

OSG_GEN_CONTAINERPTR(RTCacheAttachment);

#endif

#if !defined(OSG_DO_DOC) || (OSG_DOC_LEVEL >= 3)
/*! \ingroup GrpContribRRTFieldTraits
 */
#if !defined(OSG_DOC_DEV_TRAITS)
/*! \hideinhierarchy */
#endif

template <>
struct FieldTraits<RTCacheAttachmentPtr> :
    public FieldTraitsFCPtrBase<RTCacheAttachmentPtr>
{
  private:

    static DataType             _type;

  public:

    typedef FieldTraits<RTCacheAttachmentPtr>  Self;

    enum                        { Convertible = NotConvertible };

    static OSG_CONTRIBRRT_DLLMAPPING DataType &getType(void);

    template<typename RefCountPolicy> inline
    static const Char8    *getSName     (void);

//    static const char *getSName(void) { return "SFRTCacheAttachmentPtr"; }
    template<typename RefCountPolicy> inline
    static const Char8    *getMName     (void);

//    static const char *getMName(void) { return "MFRTCacheAttachmentPtr"; }
};

template<> inline
const Char8 *FieldTraits<RTCacheAttachmentPtr, 0>::getSName<RecordedRefCountPolicy>(void)
{
    return "SFRecRTCacheAttachmentPtr"; 
}

template<> inline
const Char8 *FieldTraits<RTCacheAttachmentPtr, 0>::getSName<UnrecordedRefCountPolicy>(void)
{
    return "SFUnrecRTCacheAttachmentPtr"; 
}

template<> inline
const Char8 *FieldTraits<RTCacheAttachmentPtr, 0>::getSName<WeakRefCountPolicy>(void)
{
    return "SFWeakRTCacheAttachmentPtr"; 
}

template<> inline
const Char8 *FieldTraits<RTCacheAttachmentPtr, 0>::getSName<NoRefCountPolicy>(void)
{
    return "SFUnrefdRTCacheAttachmentPtr"; 
}

template<> inline
const Char8 *FieldTraits<RTCacheAttachmentPtr, 0>::getMName<RecordedRefCountPolicy>(void)
{
    return "MFRecRTCacheAttachmentPtr"; 
}

template<> inline
const Char8 *FieldTraits<RTCacheAttachmentPtr, 0>::getMName<UnrecordedRefCountPolicy>(void)
{
    return "MFUnrecRTCacheAttachmentPtr"; 
}

template<> inline
const Char8 *FieldTraits<RTCacheAttachmentPtr, 0>::getMName<WeakRefCountPolicy>(void)
{
    return "MFWeakRTCacheAttachmentPtr"; 
}

template<> inline
const Char8 *FieldTraits<RTCacheAttachmentPtr, 0>::getMName<NoRefCountPolicy>(void)
{
    return "MFUnrefdRTCacheAttachmentPtr"; 
}

#if !defined(OSG_DOC_DEV_TRAITS)
/*! \class  FieldTraitsTemplateBase<RTCacheAttachmentPtr, 0>
    \hideinhierarchy
 */
#endif

#endif // !defined(OSG_DO_DOC) || (OSG_DOC_LEVEL >= 3)


#if !defined(OSG_DO_DOC) || defined(OSG_DOC_FIELD_TYPEDEFS)
/*! \ingroup GrpContribRRTFieldSingle */

typedef FieldContainerPtrSField<RTCacheAttachmentPtr,
                                RecordedRefCountPolicy  > SFRecRTCacheAttachmentPtr;
typedef FieldContainerPtrSField<RTCacheAttachmentPtr,
                                UnrecordedRefCountPolicy> SFUnrecRTCacheAttachmentPtr;
typedef FieldContainerPtrSField<RTCacheAttachmentPtr,
                                WeakRefCountPolicy      > SFWeakRTCacheAttachmentPtr;
typedef FieldContainerPtrSField<RTCacheAttachmentPtr,
                                NoRefCountPolicy        > SFUncountedRTCacheAttachmentPtr;
#endif


#if !defined(OSG_DO_DOC) || defined(OSG_DOC_FIELD_TYPEDEFS)
/*! \ingroup GrpContribRRTFieldMulti */

typedef FieldContainerPtrMField<RTCacheAttachmentPtr,
                                RecordedRefCountPolicy  > MFRecRTCacheAttachmentPtr;
typedef FieldContainerPtrMField<RTCacheAttachmentPtr,
                                UnrecordedRefCountPolicy> MFUnrecRTCacheAttachmentPtr;
typedef FieldContainerPtrMField<RTCacheAttachmentPtr,
                                WeakRefCountPolicy      > MFWeakRTCacheAttachmentPtr;
typedef FieldContainerPtrMField<RTCacheAttachmentPtr,
                                NoRefCountPolicy        > MFUncountedRTCacheAttachmentPtr;
#endif


OSG_END_NAMESPACE

#endif /* _OSGRTCACHEATTACHMENTFIELDS_H_ */
