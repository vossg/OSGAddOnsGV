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

#ifndef _OSGCUDABUFFERPNT3FINTERPOLATOR_H_
#define _OSGCUDABUFFERPNT3FINTERPOLATOR_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGCudaBufferPnt3fInterpolatorBase.h"

OSG_BEGIN_NAMESPACE

/*! \brief CudaBufferPnt3fInterpolator class. See \ref
           PageSystemDynamicsCudaBufferPnt3fInterpolator for a description.
*/

class OSG_CONTRIBCOMPUTE_DLLMAPPING CudaBufferPnt3fInterpolator : 
    public CudaBufferPnt3fInterpolatorBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef CudaBufferPnt3fInterpolatorBase Inherited;
    typedef CudaBufferPnt3fInterpolator     Self;

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    virtual void changed(ConstFieldMaskArg whichField,
                         UInt32            origin,
                         BitVector         details    );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    virtual void dump(      UInt32     uiIndent = 0,
                      const BitVector  bvFlags  = 0) const;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in CoordinateInterpolatorBase.

    CallbackDrawTaskRefPtr _pTask;

    std::vector<float *> _vCudaValues;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    CudaBufferPnt3fInterpolator(void);
    CudaBufferPnt3fInterpolator(const CudaBufferPnt3fInterpolator &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~CudaBufferPnt3fInterpolator(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    void setupCuda   (HardwareContext *pContext, DrawEnv *pEnv);
    void computeCuda (HardwareContext *pContext, DrawEnv *pEnv);
    void shutdownCuda(HardwareContext *pContext, DrawEnv *pEnv);

    void resubmitCuda(HardwareContext *pContext, DrawEnv *pEnv);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    virtual void resolveLinks(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class CudaBufferPnt3fInterpolatorBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const CudaBufferPnt3fInterpolator &source);
};

typedef CudaBufferPnt3fInterpolator *CudaBufferPnt3fInterpolatorP;

OSG_END_NAMESPACE

#include "OSGCudaBufferPnt3fInterpolatorBase.inl"
#include "OSGCudaBufferPnt3fInterpolator.inl"

#endif /* _OSGCUDABUFFERPNT3FINTERPOLATOR_H_ */
