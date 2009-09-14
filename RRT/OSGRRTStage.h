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

#ifndef _OSGRRTSTAGE_H_
#define _OSGRRTSTAGE_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRRTStageBase.h"
#include "OSGBarrier.h"
#include "OSGCamera.h"
#include "OSGTextureObjChunk.h"
#include "OSGRTCameraDecorator.h"

#include "OSGRRTDefinitions.h"

OSG_BEGIN_NAMESPACE

class Background;

/*! \brief RRTStage class. See \ref
           PageContribRRTRRTStage for a description.
*/

class OSG_CONTRIBRRT_DLLMAPPING RRTStage : public RRTStageBase
{
  protected:
    
  public:

    /*==========================  PUBLIC  =================================*/

    typedef RayTracerInst<RRT::SinglePacketDesc> SinglePacketRayTracer;
    typedef RayTracerInst<RRT::SIMDPacketDesc  > SIMDPacketRayTracer;

  public:


//    typedef SinglePacketRayTracer   ActiveRayTracer;
    typedef SIMDPacketRayTracer                   ActiveRayTracer;

    typedef RefCountPtr<ActiveRayTracer , 
                        UnrecordedRefCountPolicy> ActiveRayTracerUnrecPtr;

    typedef RRTStageBase                          Inherited;
    typedef RRTStage                              Self;

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details   );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Dump                                  */
    /*! \{                                                                 */

    void run        (CameraP     pCam,
                     Background *pBackground,
                     Node       *pRoot      );

    void postProcess(DrawEnv    *pEnv       );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void setSize(UInt32 uiWidth, UInt32 uiHeight);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in RRTStageBase.

    bool                    _bInitialized;
    ActiveRayTracerUnrecPtr _pRayTracer;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RRTStage(void);
    RRTStage(const RRTStage &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~RRTStage(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                MT Construction                               */
    /*! \{                                                                 */

    void onCreate       (const RRTStage *source = NULL);

    void onCreateAspect (const RRTStage *createAspect,
                         const RRTStage *source = NULL);
    
    void onDestroy      (      UInt32    uiContainerId);

    void onDestroyAspect(      UInt32    uiContainerId,
                               UInt32    uiAspect     );
    
    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class RRTStageBase;
    friend class FieldContainer;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const RRTStage &source);
};

typedef RRTStage *RRTStageP;

OSG_END_NAMESPACE

#include "OSGRRTStageBase.inl"
#include "OSGRRTStage.inl"

#endif /* _OSGRRTSTAGE_H_ */
