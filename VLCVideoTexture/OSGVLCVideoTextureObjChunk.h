/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2013 by the OpenSG Forum                 *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 * contact: dirk@opensg.org, gerrit.voss@vossg.org, carsten_neumann@gmx.net  *
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

#ifndef _OSGVLCVIDEOTEXTUREOBJCHUNK_H_
#define _OSGVLCVIDEOTEXTUREOBJCHUNK_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGVLCVideoTextureObjChunkBase.h"
#include "OSGImage.h"
#include "OSGThread.h"

#include <vlc/vlc.h>

OSG_BEGIN_NAMESPACE

/*! \brief VLCVideoTextureObjChunk class. See \ref
           PageVLCVideoTextureVideoTextureObjChunk for a description.
*/


struct ctxStruct
{
    char                    *idstr;
    ImageRefPtr              img;        
    ExternalThreadRefPtr     videothread;      
    LockRefPtr               lock;
    VLCVideoTextureObjChunk *self;
};

static std::vector<ctxStruct*> allVideoTextures;


class OSG_VLCVIDEOTEXTURE_DLLMAPPING VLCVideoTextureObjChunk : 
    public VLCVideoTextureObjChunkBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef VLCVideoTextureObjChunkBase Inherited;
    typedef VLCVideoTextureObjChunk     Self;

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


    virtual void frame(Time oTime, UInt32 uiFrame);

    // update via static method
    static void triggerUpdate(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                       State                                  */
    /*! \{                                                                 */

    virtual void activate   (DrawEnv    *pEnv,
                             UInt32      index = 0);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                  Video Control                               */
    /*! \{                                                                 */

    bool isPlaying(void);
    void pause(void);
    void play(void);

    void checkForSync(void); 

    void updateTime(void);

    bool needsUpdate;

    /*! \}                                                                 */
    /*=========================  PROTECTED  ===============================*/

  protected:

    // Variables should all be in VideoTextureObjChunkBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    VLCVideoTextureObjChunk(void);
    VLCVideoTextureObjChunk(const VLCVideoTextureObjChunk &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~VLCVideoTextureObjChunk(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class VLCVideoTextureObjChunkBase;

    // internal image for video rendering
    ImageUnrecPtr img;

    // VLC
    libvlc_instance_t *libvlc;
    libvlc_media_player_t *vlcmediaplayer;
    
    ctxStruct ctx;
        
    bool createVLCInstance(libvlc_time_t start_time=0, bool play=true);
    void cleanVLC(void);
    void resizeVideo(void);


    OSG::TimeStamp lastSync;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const VLCVideoTextureObjChunk &source);
};

typedef VLCVideoTextureObjChunk *VLCVideoTextureObjChunkP;

OSG_END_NAMESPACE

#include "OSGVLCVideoTextureObjChunkBase.inl"
#include "OSGVLCVideoTextureObjChunk.inl"

#endif /* _OSGVLCVIDEOTEXTUREOBJCHUNK_H_ */
