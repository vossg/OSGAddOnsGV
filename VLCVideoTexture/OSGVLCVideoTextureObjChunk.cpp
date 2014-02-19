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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include "OSGConfig.h"

#include "OSGVLCVideoTextureObjChunk.h"
#include "OSGClusterServer.h"

OSG_BEGIN_NAMESPACE

namespace
{
    bool initVLCTexObjClusterServer(void)
    {
        fprintf(stderr, "initVLCTexObjClusterServer\n");

        FieldContainer          *pProtoFC    =
            VLCVideoTextureObjChunk::getClassType().getPrototype();

        VLCVideoTextureObjChunk *pVideoProto =
            dynamic_cast<VLCVideoTextureObjChunk*>(pProtoFC);

        if (pVideoProto != NULL)
        {
            pVideoProto->setIsMaster(false);
        }

        fprintf(stderr, "got %p/%p\n",
                pProtoFC,
                pVideoProto);

        return true;
    }

    bool regInitVLCTexObjClusterServer(void)
    {
        ClusterServer::addInitFunction(&initVLCTexObjClusterServer);

        return true;
    }

    StaticInitFuncWrapper oWrapper(&regInitVLCTexObjClusterServer);
}

// Documentation for this class is emitted in the
// OSGVLCVideoTextureObjChunkBase.cpp file.
// To modify it, please change the .fcd file (OSGVLCVideoTextureObjChunk.fcd)
// and regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void VLCVideoTextureObjChunk::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if (ePhase == TypeObject::SystemPost)
    {
    }
}


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

VLCVideoTextureObjChunk::VLCVideoTextureObjChunk(void) :
    Inherited()
{
    libvlc = NULL;
    vlcmediaplayer = NULL;
    needsUpdate = false;
    needsRestart = false;
    isSetToMute = false;
    vlcmediaplayer = NULL;
}

VLCVideoTextureObjChunk::VLCVideoTextureObjChunk(
    const VLCVideoTextureObjChunk &source) :

    Inherited(source)
{
    libvlc = NULL;
    vlcmediaplayer = NULL;
    needsUpdate = false;
    needsRestart = false;
    vlcmediaplayer = NULL;
}

VLCVideoTextureObjChunk::~VLCVideoTextureObjChunk(void)
{
    cleanVLC();
}

/*----------------------------- class specific ----------------------------*/

void VLCVideoTextureObjChunk::changed(ConstFieldMaskArg whichField,
                                      UInt32            origin,
                                      BitVector         details)
{

    Inherited::changed(whichField, origin, details);

    if (0x0000 != (whichField & UrlFieldMask)) {
        // init VLC internal structures
        if (getUrl().length() > 0) 
        {
            createVLCInstance();
        }
    }
    else
        if ((0x0000 != (whichField & WidthFieldMask)) ||
            (0x0000 != (whichField & HeightFieldMask)))
        {
            // reinit VLC internal structures
            if (getUrl().length() > 1)
            {
                resizeVideo();
            }
        }

    if (0x0000 != (whichField & StatusFieldMask)) {
        // update VLC        
    }

    if (0x0000 != (whichField & TimestampFieldMask)) {
        // update VLC
        if (getUrl().length() > 1)
        {
            if (!this->getIsMaster())
            {
                updateTime();
            }
        }
    }
}

void VLCVideoTextureObjChunk::dump(      UInt32    ,
                                   const BitVector ) const
{
    SLOG << "Dump VLCVideoTextureObjChunk NI" << std::endl;
}

void VLCVideoTextureObjChunk::frame(Time oTime, UInt32 uiFrame)
{
    // trigger update
    if (ctx.idstr != NULL)
    {
        if (needsUpdate)
        {
            ctx.lock->acquire();
            GLenum id = _sfGLId.getValue();
            Window::refreshGLObject(id);
            needsUpdate = false;
            ctx.lock->release();
            checkForSync();
        }
        if (needsRestart)
        {
            playAgain();
        }
    }
}


void VLCVideoTextureObjChunk::activate(DrawEnv *pEnv, UInt32 idx)
{



    Inherited::activate(pEnv, idx);
}

// ----------------------------------------------------------------------------------------
static void *vlc_lock(void *data, void **p_pixels)
{
    ctxStruct *_ctx = static_cast<ctxStruct *>(data);
    if (_ctx->idstr == NULL)
    {
        *p_pixels = NULL;
        return NULL;
    }

    if (_ctx->videothread == NULL)
    {
        std::string name = std::string("THREAD__") + std::string(_ctx->idstr);
        _ctx->videothread = ExternalThread::get(name.c_str(), true);
        _ctx->videothread->initialize(0);  //using the same aspect -> no mem copy         
    }

    _ctx->lock->acquire();

    *p_pixels = static_cast<void *>(_ctx->img->editData());
    return NULL; /* picture identifier, not needed here */
}

static void vlc_unlock(void *data, void *id, void *const *p_pixels)
{
    ctxStruct *_ctx = static_cast<ctxStruct *>(data);
    if (_ctx->idstr == NULL) return;

    _ctx->lock->release();
    _ctx->self->needsUpdate = true;

    assert(id == NULL); // picture identifier, not needed here
}

static void vlc_display(void *data, void *id)
{
    // VLC wants to display the video
    //    ctxStruct *_ctx = static_cast<ctxStruct *>(data);
    assert(id == NULL);
}


static void vlc_event(const libvlc_event_t* _event, void *data)
{
    ctxStruct *_ctx = static_cast<ctxStruct *>(data);
    if (_ctx->idstr == NULL) return;

    if (_event->type == libvlc_MediaPlayerEndReached)
    {
        _ctx->self->needsRestart = true;
    }
}

static std::string getVideoID()
{
    static int number = 0;
    number++;
    std::stringstream ss;
    ss << number;
    return ss.str();
}

// ----------------------------------------------------------------------------------------


bool VLCVideoTextureObjChunk::createVLCInstance(libvlc_time_t start_time,
                                                bool          play)
{
    libvlc_media_t *m;

    char const *vlc_argv[] =
    {
        "-I", "dumy",      // No special interface
        "--ignore-config", // Don't use VLC's config        
        "--quiet",         // deactivates VLCs console outputs
        "--no-xlib",       // tell VLC to not use Xlib                   
        "--no-video-title-show", // no titles
    };

    int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

    // Initialize libVLC   
    if (libvlc == NULL)
    {
        libvlc = libvlc_new(vlc_argc, vlc_argv);
    }
    else
    {
        if (vlcmediaplayer != NULL)
        {
            ctx.idstr = NULL;
            libvlc_media_player_stop(vlcmediaplayer);
            libvlc_media_player_release(vlcmediaplayer);
            vlcmediaplayer = NULL;
            if (ctx.videothread != NULL) {
                ctx.videothread->runFunction(NULL, NULL); // reset thread structure: isInititialized=false
            }
            ctx.videothread = NULL;
        }
    }
    if (libvlc == NULL) 
    {
        SFATAL << "libvlc creation failed (plugin folder?)" << std::endl;
        return false;
    }
    m = libvlc_media_new_path(libvlc, getUrl().c_str());

    vlcmediaplayer = libvlc_media_player_new_from_media(m);

    libvlc_media_release(m);

    ctx.idstr = strdup((std::string("video") + getVideoID()).c_str());
    ctx.videothread = NULL;
    ctx.img = Image::create();
    ctx.img->set(OSG::Image::OSG_BGR_PF, getWidth(), getHeight());
    ctx.lock = Lock::create();

    this->setImage(ctx.img);
    this->setTarget(GL_TEXTURE_2D);
    ctx.self = this;


    vlceventmgr = libvlc_media_player_event_manager(vlcmediaplayer);
    // attach event to defined event handler callback
    libvlc_event_attach(vlceventmgr, libvlc_MediaPlayerEndReached, vlc_event, &ctx);

    libvlc_video_set_callbacks(vlcmediaplayer, vlc_lock, vlc_unlock, vlc_display, &ctx);
    libvlc_video_set_format(vlcmediaplayer, "RV24", getWidth(), getHeight(), getWidth() * 3);
    if (getIsMaster() == false) 
    {
        libvlc_audio_set_mute(vlcmediaplayer, 1);

        SLOG << "audiotracks:" << libvlc_audio_get_track_count(vlcmediaplayer) << std::endl;
        libvlc_audio_set_track(vlcmediaplayer, -1); // this may only work when media is playing!
        needsUpdate = true;


        SLOG << "not master, setting audio to mute!" << std::endl;
    }
    else SLOG << "this is handled as master!" << std::endl;

#if 0
    {
        SLOG << "available filters:" << std::endl;
        libvlc_module_description_t *list = libvlc_video_filter_list_get(libvlc);
        libvlc_module_description_t *iter = list;
        while (iter != NULL) {
            SLOG << iter->psz_name << std::endl;
            iter = iter->p_next;
        }
        libvlc_module_description_list_release(list);
    }
#endif


    libvlc_media_player_play(vlcmediaplayer);
    libvlc_media_player_set_time(vlcmediaplayer, start_time);
    if (!play) libvlc_media_player_pause(vlcmediaplayer); //not working?

    // add to static list
    //allVideoTextures.push_back(&ctx);

    lastSync = OSG::getTimeStamp();

    return true;
}


void VLCVideoTextureObjChunk::cleanVLC()
{
    if (libvlc == NULL) return;
    // Stop stream and clean up libVLC
    ctx.idstr = NULL;

    libvlc_media_player_stop(vlcmediaplayer);

    libvlc_media_player_release(vlcmediaplayer);
    libvlc_release(libvlc);

    libvlc = NULL;
}

void VLCVideoTextureObjChunk::resizeVideo()
{
    if ((libvlc == NULL) || (vlcmediaplayer == NULL)) return;
    libvlc_time_t current_time = 0;
    bool play = true;

    if (libvlc_media_player_is_playing(vlcmediaplayer) != 1) play = false;

    current_time = libvlc_media_player_get_time(vlcmediaplayer);
    SINFO << "restart video at " << current_time << std::endl;

    createVLCInstance(current_time, play);
}

//
bool VLCVideoTextureObjChunk::isPlaying()
{
    if ((libvlc == NULL) || (vlcmediaplayer == NULL)) return false;
    if (libvlc_media_player_is_playing(vlcmediaplayer) == 1) return true;
    return false;
}

void VLCVideoTextureObjChunk::pause(void)
{
    if ((libvlc == NULL) || (vlcmediaplayer == NULL)) return;
    libvlc_media_player_pause(vlcmediaplayer);
}

void VLCVideoTextureObjChunk::play(void)
{
    if ((libvlc == NULL) || (vlcmediaplayer == NULL)) return;
    libvlc_media_player_play(vlcmediaplayer);
}

void VLCVideoTextureObjChunk::playAgain(void)
{
    if ((libvlc == NULL) || (vlcmediaplayer == NULL)) return;
    libvlc_media_player_stop(vlcmediaplayer);
    libvlc_media_player_play(vlcmediaplayer);
    needsRestart = false;
}

void VLCVideoTextureObjChunk::checkForSync(void)
{
    if ((libvlc == NULL) || (vlcmediaplayer == NULL)) return;
    if (libvlc_media_player_is_playing(vlcmediaplayer) != 1) return;

    // only the master sets the timestamp
    if (getIsMaster() == true)
    {
        OSG::TimeStamp now = OSG::getTimeStamp();
        if (OSG::getTimeStampMsecs(now - lastSync) > getUpdatefreq())
        {
            lastSync = now;
            setTimestamp(libvlc_media_player_get_time(vlcmediaplayer) + NETWORK_LATENCY);
            commitChanges();
        }
    }    
}

void VLCVideoTextureObjChunk::updateTime(void)
{
    if ((libvlc == NULL) || (vlcmediaplayer == NULL)) return;
    if (libvlc_media_player_is_playing(vlcmediaplayer) != 1) return;
    // only update if > MIN_TIME_DIFF ms offset
    long test = labs(libvlc_media_player_get_time(vlcmediaplayer) - this->getTimestamp());
    if (test > MIN_TIME_DIFF)
    {
        libvlc_media_player_set_time(vlcmediaplayer, this->getTimestamp());
        SLOG << "resyncing video: " << test << std::endl;

        if (!isSetToMute) 
        {
            libvlc_audio_set_track(vlcmediaplayer, -1);            
            isSetToMute = true;
        }

    }
}



OSG_END_NAMESPACE
