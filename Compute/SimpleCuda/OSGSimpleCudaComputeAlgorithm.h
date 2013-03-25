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

#ifndef _OSGSIMPLECUDACOMPUTEALGORITHM_H_
#define _OSGSIMPLECUDACOMPUTEALGORITHM_H_
#ifdef __sgi
#pragma once
#endif

#include <queue>

#include "OSGSimpleCudaComputeAlgorithmBase.h"

#ifdef OSG_WITH_CUDA
#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>
#endif

OSG_BEGIN_NAMESPACE

class DrawEnv;
class Action;

//! Stage
//! \ingroup GrpSystemNodeCoresMisc

class OSG_CONTRIBCOMPUTE_DLLMAPPING SimpleCudaComputeAlgorithm : 
    public SimpleCudaComputeAlgorithmBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    static const OSG::UInt32 UpdateOSG           = 0x0001;
    static const OSG::UInt32 UpdateOSGMapped     = 0x0002;
    static const OSG::UInt32 UpdateOSGMappedExt  = 0x0003;

    static const OSG::UInt32 UpdateOSGCuda       = 0x0004;
    static const OSG::UInt32 UpdateOSGCudaExt    = 0x0005;

    static const OSG::UInt32 UpdateOSGAlgo       = 0x0006;
    static const OSG::UInt32 UpdateOSGMappedAlgo = 0x0007;
    static const OSG::UInt32 UpdateOSGCudaAlgo   = 0x0008;


    static const OSG::UInt32 MaxUpdateMode       = 0x0009;

    /*---------------------------------------------------------------------*/
    /*! \name                       Sync                                   */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         detail);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Execute                                 */
    /*! \{                                                                 */

    virtual Action::ResultE renderEnter(Action          *pAction );
    virtual Action::ResultE renderLeave(Action          *pAction );
    virtual void            execute    (HardwareContext *pContext, 
                                        DrawEnv         *pEnv    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Execute                                 */
    /*! \{                                                                 */

    void incScale(Real32 rVal);
    void decScale(Real32 rVal);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                        Dump                                  */
    /*! \{                                                                 */

    virtual void dump(      UInt32    uiIndent = 0,
                      const BitVector bvFlags  = 0) const;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    typedef SimpleCudaComputeAlgorithmBase Inherited;

    cudaGraphicsResource_t cudaBufferResPos;
    cudaGraphicsResource_t cudaBufferResNorm;

    /*---------------------------------------------------------------------*/
    /*! \name                   Constructors                               */
    /*! \{                                                                 */

    SimpleCudaComputeAlgorithm(void);
    SimpleCudaComputeAlgorithm(const SimpleCudaComputeAlgorithm &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~SimpleCudaComputeAlgorithm(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Draw                                       */
    /*! \{                                                                 */

    void updateScale       (     void                     );

    void updateGeoOSG      (OSG::HardwareContext *pContext, 
                            OSG::DrawEnv         *pEnv    );

    void updateGeoOSGMapped(OSG::HardwareContext *pContext, 
                            OSG::DrawEnv         *pEnv    );

    void updateGeoOSGCuda  (OSG::HardwareContext *pContext, 
                            OSG::DrawEnv         *pEnv    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class SimpleCudaComputeAlgorithmBase;

    /*---------------------------------------------------------------------*/
    /*! \name                   thread local                               */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                    load thread                               */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/

    /*!\brief prohibit default function (move to 'public' if needed) */
    void operator =(const SimpleCudaComputeAlgorithm &source);
};

typedef SimpleCudaComputeAlgorithm *SimpleCudaComputeAlgorithmP;

OSG_END_NAMESPACE

#include "OSGSimpleCudaComputeAlgorithmBase.inl"
#include "OSGSimpleCudaComputeAlgorithm.inl"

#endif /* _OSGSIMPLECUDACOMPUTEALGORITHM_H_ */
