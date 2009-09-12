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

#ifndef _OSGRTTRIANGLEACC_H_
#define _OSGRTTRIANGLEACC_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTSingleRayPacket.h"
#include "OSGRTHitPacket.h"

OSG_BEGIN_NAMESPACE

/*! \brief RTTarget class. See \ref
           PageContribRRTRTTarget for a description.
*/

class OSG_CONTRIBRRT_DLLMAPPING RTTriAccelBarycentric
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    RTTriAccelBarycentric(void);
    RTTriAccelBarycentric(const RTTriAccelBarycentric &source);

    ~RTTriAccelBarycentric(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void setup(Pnt3f  A, 
               Pnt3f  B, 
               Pnt3f  C, 
               UInt32 uiObjId,
               UInt32 uiTriId);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void intersect      (RTRayPacket     &oRay, 
                         RTHitPacket     &oHit,
                         UInt32           uiCacheId);


    void intersect      (RTRaySIMDPacket &oRay, 
                         RTHitSIMDPacket &oHit,
                         UInt32           uiCacheId,
                         UInt32           uiActive  );

#ifndef OSG_SIMD_RAYPACKET_DEBUG
    void intersectSingle(Vec3f           *vRayDirs,
                         Pnt3f           &vOrigin,
                         RTHitSIMDPacket &oHit,
                         UInt32           uiCacheId,
                         UInt32           uiActive  );
#else
    void intersectSingle(RTRaySIMDPacket &oRay, 
                         RTHitSIMDPacket &oHit,
                         UInt32           uiCacheId,
                         UInt32           uiActive  );
#endif

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void putToStream(OutStream &str) const;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTTriAccelBarycentric &source);

    bool operator ==(const RTTriAccelBarycentric &source) const;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in RTTargetBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    Real32 _nU;
    Real32 _nV;
    Real32 _nD;
    UInt32 _uiProj;

    Real32 _bNU;
    Real32 _bNV;
    Real32 _bD;
    UInt32 _uiObjId;

    Real32 _cNU;
    Real32 _cNV;
    Real32 _cD;
    UInt32 _uiTriId;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    // prohibit default functions (move to 'public' if you need one)
};


class OSG_CONTRIBRRT_DLLMAPPING RTTriAccelBarycentricVer1
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    RTTriAccelBarycentricVer1(void);
    RTTriAccelBarycentricVer1(const RTTriAccelBarycentricVer1 &source);

    ~RTTriAccelBarycentricVer1(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void setup(Pnt3f  A, 
               Pnt3f  B, 
               Pnt3f  C, 
               UInt32 uiObjId,
               UInt32 uiTriId);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void intersect(RTRayPacket     &oRay, 
                   RTHitPacket     &oHit,
                   UInt32           uiCacheId);

    void intersect(RTRaySIMDPacket &oRay, 
                   RTHitSIMDPacket &oHit,
                   UInt32           uiCacheId,
                   UInt32           uiActive  );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTTriAccelBarycentricVer1 &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in RTTargetBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    Pnt3f _a;
    Pnt3f _b;
    Pnt3f _c;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    UInt32 _uiObjId;
    UInt32 _uiTriId;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    // prohibit default functions (move to 'public' if you need one)
};


class OSG_CONTRIBRRT_DLLMAPPING RTTriAccelBarycentricVer2
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    RTTriAccelBarycentricVer2(void);
    RTTriAccelBarycentricVer2(const RTTriAccelBarycentricVer2 &source);

    ~RTTriAccelBarycentricVer2(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void setup(Pnt3f  A, 
               Pnt3f  B, 
               Pnt3f  C, 
               UInt32 uiObjId,
               UInt32 uiTriId);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void intersect(RTRayPacket     &oRay, 
                   RTHitPacket     &oHit,
                   UInt32           uiCacheId);

    void intersect(RTRaySIMDPacket &oRay, 
                   RTHitSIMDPacket &oHit,
                   UInt32           uiCacheId,
                   UInt32           uiActive );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTTriAccelBarycentricVer2 &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in RTTargetBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    Pnt3f _a;
    Pnt3f _b;
    Pnt3f _c;

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    UInt32 _uiObjId;
    UInt32 _uiTriId;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    // prohibit default functions (move to 'public' if you need one)
};

OSG_END_NAMESPACE

#include "OSGRTTriangleAccel.inl"

#endif /* _OSGRTTRIANGLEACC_H_ */
