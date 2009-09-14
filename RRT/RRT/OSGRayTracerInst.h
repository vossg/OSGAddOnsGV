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

#ifndef _OSGRAYTRACERINST_H_
#define _OSGRAYTRACERINST_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRayTracer.h"

OSG_BEGIN_NAMESPACE

template<typename DescT>
class RTUpdateAction;

template<typename DescT>
class RTInitAction;

class RTCameraDecorator;

/*! \brief RTTarget class. See \ref
           PageContribRRTRTTarget for a description.
*/

template<typename DescT>
class RayTracerInst : public RayTracer
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RayTracerInst<DescT>         Self;

    typedef DescT                        Desc;

    typedef typename Desc::RTCache       RTCache;
    typedef typename Desc::Scene         Scene;
    typedef typename Desc::PacketManager PacketManager;

    struct RTActionDesc
    {
        typedef Self                           RTRayTracer;
        typedef typename Desc::CacheAttachment CacheAttachment;
    };


    typedef RTInitAction  <RTActionDesc> InitAction;
    typedef RTUpdateAction<RTActionDesc> UpdateAction;

    OSG_GEN_INTERNALPTR(Self);

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    OSG_FIELD_CONTAINER_TMPL_DECL;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void addCache(RTCache *pCache );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void init    (bool               bTiled        = false, 
                  bool               bSplitThreads = false);
    void trace   (RTCameraDecorator *pCam, 
                  bool               bTiled        = false);

    void finalize(DrawEnv           *pEnv                 );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    void setBackgroundColor(Real32 rR,
                            Real32 rG,
                            Real32 rB,
                            Real32 rA);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef RayTracer  Inherited;

    static TypeObject _type;

    Scene         *_pScene;
    PacketManager *_pPacketManager;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RayTracerInst(void);
    RayTracerInst(const RayTracerInst &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~RayTracerInst(void);

    /*! \}                                                                 */
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Init                                   */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       Sync                                   */
    /*! \{                                                                 */

    virtual void resolveLinks(void);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    
    /*!\brief prohibit default functions (move to 'public' if needed) */
    void operator =(const RayTracerInst &source);
};

OSG_END_NAMESPACE


#define OSG_RAYTRACERINST_INST(DESC)                                          \
template <>                                                                   \
RayTracerInst< DESC >::TypeObject                                             \
    RayTracerInst< DESC >::_type(                                             \
        Desc::getTypeName      (),                                            \
        Desc::getParentTypeName(),                                            \
        Desc::getGroupName     (),                                            \
        0,                                                                    \
        reinterpret_cast<PrototypeCreateF>(&Self::createEmpty),               \
        NULL,                                                                 \
        NULL,                                                                 \
        NULL,                                                                 \
        true,                                                                 \
        0);                                                                   \
                                                                              \
template<>                                                                    \
RayTracerInst< DESC >::TypeObject &                                           \
    RayTracerInst< DESC >::getClassType(void)                                 \
{                                                                             \
    return Self::_type;                                                       \
}                                                                             \
                                                                              \
template<>                                                                    \
RayTracerInst< DESC >::TypeObject &                                           \
    RayTracerInst< DESC >::getType(void)                                      \
{                                                                             \
    return Self::_type;                                                       \
}                                                                             \
                                                                              \
template<>                                                                    \
const RayTracerInst< DESC >::TypeObject &                                     \
    RayTracerInst< DESC >::getType(void) const                                \
{                                                                             \
    return Self::_type;                                                       \
}

#include "OSGRayTracerInst.inl"

#endif /* _OSGRAYTRACERINST_H_ */
