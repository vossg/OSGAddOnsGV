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

#ifndef _OSGRTSINGLERAYPACKET_H_
#define _OSGRTSINGLERAYPACKET_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGRTRayPacket.h"

OSG_BEGIN_NAMESPACE

class OSG_CONTRIBRRT_DLLMAPPING RTSingleRayPacketInfo : public RTRayPacketInfo
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTRayPacketInfo       Inherited;
    typedef RTSingleRayPacketInfo Self;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RTSingleRayPacketInfo(void);
    RTSingleRayPacketInfo(const RTSingleRayPacketInfo &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    ~RTSingleRayPacketInfo(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    void   setXY(UInt32 uiX, 
                 UInt32 uiY);
    
    UInt32 getX (void      );
    UInt32 getY (void      );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void dump(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTSingleRayPacketInfo &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    UInt32 _uiX;
    UInt32 _uiY;

  private:

};


template<class DescT>
class RTCacheRefRayPacketMixin : public RTRayPacket
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef          RTRayPacket        Inherited;
    typedef typename DescT::RTCacheNode CacheNode;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RTCacheRefRayPacketMixin(void);
    RTCacheRefRayPacketMixin(const RTCacheRefRayPacketMixin &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    ~RTCacheRefRayPacketMixin(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    void       setCacheNode(const CacheNode *pNode);
    CacheNode *getCacheNode(      void       );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTCacheRefRayPacketMixin &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    const CacheNode *_pCacheNode;

    /*==========================  PRIVATE  ================================*/

  private:
};

/*! \brief RTTarget class. See \ref
           PageContribRRTRTTarget for a description.
*/

template<class ParentT>
class RTSingleRayPacket : public ParentT 
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef RTSingleRayPacket Self;
    typedef ParentT           Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    RTSingleRayPacket(void);
    RTSingleRayPacket(const RTSingleRayPacket &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    ~RTSingleRayPacket(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Sync                                    */
    /*! \{                                                                 */

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void dump(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                     Output                                   */
    /*! \{                                                                 */

    void operator =(const RTSingleRayPacket &source);

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:
};

OSG_END_NAMESPACE

#include "OSGRTSingleRayPacket.inl"

#endif /* _OSGRTSINGLERAYPACKET_H_ */
