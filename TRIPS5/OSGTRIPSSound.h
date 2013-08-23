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
 *                            Acknowledgements                               *
 *                                                                           *
 *   Thank You Tomas Neumann for providing the code for OpenSG!              *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

#ifndef _OSGTRIPSSOUND_H_
#define _OSGTRIPSSOUND_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGTRIPSSoundBase.h"

namespace FMOD {
	class Sound;
	class Channel;
}

OSG_BEGIN_NAMESPACE

/*! \brief TRIPSSound class. See \ref
           PageTRIPSTRIPSSound for a description.
*/


const std::string FMOD_ErrorString[] = {
  "FMOD_OK", 
  "FMOD_ERR_ALREADYLOCKED", 
  "FMOD_ERR_BADCOMMAND", 
  "FMOD_ERR_CDDA_DRIVERS", 
  "FMOD_ERR_CDDA_INIT", 
  "FMOD_ERR_CDDA_INVALID_DEVICE", 
  "FMOD_ERR_CDDA_NOAUDIO", 
  "FMOD_ERR_CDDA_NODEVICES", 
  "FMOD_ERR_CDDA_NODISC", 
  "FMOD_ERR_CDDA_READ", 
  "FMOD_ERR_CHANNEL_ALLOC", 
  "FMOD_ERR_CHANNEL_STOLEN", 
  "FMOD_ERR_COM", 
  "FMOD_ERR_DMA", 
  "FMOD_ERR_DSP_CONNECTION", 
  "FMOD_ERR_DSP_FORMAT", 
  "FMOD_ERR_DSP_NOTFOUND", 
  "FMOD_ERR_DSP_RUNNING", 
  "FMOD_ERR_DSP_TOOMANYCONNECTIONS", 
  "FMOD_ERR_FILE_BAD", 
  "FMOD_ERR_FILE_COULDNOTSEEK", 
  "FMOD_ERR_FILE_DISKEJECTED", 
  "FMOD_ERR_FILE_EOF", 
  "FMOD_ERR_FILE_NOTFOUND", 
  "FMOD_ERR_FILE_UNWANTED", 
  "FMOD_ERR_FORMAT", 
  "FMOD_ERR_HTTP", 
  "FMOD_ERR_HTTP_ACCESS", 
  "FMOD_ERR_HTTP_PROXY_AUTH", 
  "FMOD_ERR_HTTP_SERVER_ERROR", 
  "FMOD_ERR_HTTP_TIMEOUT", 
  "FMOD_ERR_INITIALIZATION", 
  "FMOD_ERR_INITIALIZED", 
  "FMOD_ERR_INTERNAL", 
  "FMOD_ERR_INVALID_ADDRESS", 
  "FMOD_ERR_INVALID_FLOAT", 
  "FMOD_ERR_INVALID_HANDLE", 
  "FMOD_ERR_INVALID_PARAM", 
  "FMOD_ERR_INVALID_POSITION", 
  "FMOD_ERR_INVALID_SPEAKER", 
  "FMOD_ERR_INVALID_SYNCPOINT", 
  "FMOD_ERR_INVALID_VECTOR", 
  "FMOD_ERR_MAXAUDIBLE", 
  "FMOD_ERR_MEMORY", 
  "FMOD_ERR_MEMORY_CANTPOINT", 
  "FMOD_ERR_MEMORY_SRAM", 
  "FMOD_ERR_NEEDS2D", 
  "FMOD_ERR_NEEDS3D", 
  "FMOD_ERR_NEEDSHARDWARE", 
  "FMOD_ERR_NEEDSSOFTWARE", 
  "FMOD_ERR_NET_CONNECT", 
  "FMOD_ERR_NET_SOCKET_ERROR", 
  "FMOD_ERR_NET_URL", 
  "FMOD_ERR_NET_WOULD_BLOCK", 
  "FMOD_ERR_NOTREADY", 
  "FMOD_ERR_OUTPUT_ALLOCATED", 
  "FMOD_ERR_OUTPUT_CREATEBUFFER", 
  "FMOD_ERR_OUTPUT_DRIVERCALL", 
  "FMOD_ERR_OUTPUT_ENUMERATION", 
  "FMOD_ERR_OUTPUT_FORMAT", 
  "FMOD_ERR_OUTPUT_INIT", 
  "FMOD_ERR_OUTPUT_NOHARDWARE", 
  "FMOD_ERR_OUTPUT_NOSOFTWARE", 
  "FMOD_ERR_PAN", 
  "FMOD_ERR_PLUGIN", 
  "FMOD_ERR_PLUGIN_INSTANCES", 
  "FMOD_ERR_PLUGIN_MISSING", 
  "FMOD_ERR_PLUGIN_RESOURCE", 
  "FMOD_ERR_PRELOADED", 
  "FMOD_ERR_PROGRAMMERSOUND", 
  "FMOD_ERR_RECORD", 
  "FMOD_ERR_REVERB_INSTANCE", 
  "FMOD_ERR_SUBSOUND_ALLOCATED", 
  "FMOD_ERR_SUBSOUND_CANTMOVE", 
  "FMOD_ERR_SUBSOUND_MODE", 
  "FMOD_ERR_SUBSOUNDS", 
  "FMOD_ERR_TAGNOTFOUND", 
  "FMOD_ERR_TOOMANYCHANNELS", 
  "FMOD_ERR_UNIMPLEMENTED", 
  "FMOD_ERR_UNINITIALIZED", 
  "FMOD_ERR_UNSUPPORTED", 
  "FMOD_ERR_UPDATE", 
  "FMOD_ERR_VERSION", 
  "FMOD_ERR_EVENT_FAILED", 
  "FMOD_ERR_EVENT_INFOONLY", 
  "FMOD_ERR_EVENT_INTERNAL", 
  "FMOD_ERR_EVENT_MAXSTREAMS", 
  "FMOD_ERR_EVENT_MISMATCH", 
  "FMOD_ERR_EVENT_NAMECONFLICT", 
  "FMOD_ERR_EVENT_NOTFOUND", 
  "FMOD_ERR_EVENT_NEEDSSIMPLE", 
  "FMOD_ERR_EVENT_GUIDCONFLICT", 
  "FMOD_ERR_EVENT_ALREADY_LOADED", 
  "FMOD_ERR_MUSIC_UNINITIALIZED", 
  "FMOD_ERR_MUSIC_NOTFOUND", 
  "FMOD_ERR_MUSIC_NOCALLBACK"
};

class OSG_TRIPS_DLLMAPPING TRIPSSound : public TRIPSSoundBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

    typedef TRIPSSoundBase Inherited;
    typedef TRIPSSound     Self;

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

    // Variables should all be in TRIPSSoundBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    TRIPSSound(void);
    TRIPSSound(const TRIPSSound &source);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~TRIPSSound(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);


    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
public:

    /*! \name                       Play                                   */
    /*! \{                                                                 */

	 // some const for playing the sound in different ways
    enum PlayModeType {
      PAUSE		= -1,
      STOP		= 0,
      ONCE		= 1,
      EVERY		= 2,
      RANDOM	= 3,
	   UNPAUSE	= 4,
      LOOP		= 5,
      ONCEMORE = 8,
      ONCEANDKILL = 10
    };

    void play(PlayModeType mode);

    void setSample(FMOD::Sound *sample);
    void setChannel(FMOD::Channel *channel);

    FMOD::Sound * getSample();
    FMOD::Channel * getChannel();


    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

    friend class FieldContainer;
    friend class TRIPSSoundBase;

    FMOD::Sound *m_sample;
    FMOD::Channel *m_channel;


    // prohibit default functions (move to 'public' if you need one)
    void operator =(const TRIPSSound &source);
};

typedef TRIPSSound *TRIPSSoundP;

OSG_END_NAMESPACE

#include "OSGTRIPSSoundBase.inl"
#include "OSGTRIPSSound.inl"

#endif /* _OSGTRIPSSOUND_H_ */
