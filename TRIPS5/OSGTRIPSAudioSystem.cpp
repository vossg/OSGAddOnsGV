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

#if __GNUC__ >= 4 || __GNUC_MINOR__ >=3
#pragma GCC diagnostic warning "-Wold-style-cast"
#endif

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include "OSGConfig.h"

#include "OSGTRIPSAudioSystem.h"
#include "OSGTRIPSSound.h"
#include "OSGNode.h"
#include "OSGCamera.h"

#include <fmod.hpp>

OSG_BEGIN_NAMESPACE

	FMOD::System* TRIPSAudioSystem::FMOD_System = NULL;

// Documentation for this class is emitted in the
// OSGTRIPSAudioSystemBase.cpp file.
// To modify it, please change the .fcd file (OSGTRIPSAudioSystem.fcd) and
// regenerate the base file.

/***************************************************************************\
*                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
*                           Class methods                                 *
\***************************************************************************/

void TRIPSAudioSystem::initMethod(InitPhase ePhase)
{
	Inherited::initMethod(ePhase);

	if(ePhase == TypeObject::SystemPost)
	{
		RenderAction::registerEnterDefault(
			getClassType(),
			reinterpret_cast<Action::Callback>(&TRIPSAudioSystem::renderActionEnter));		
	}
	
}


/***************************************************************************\
*                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
-  private                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

TRIPSAudioSystem::TRIPSAudioSystem(void) :
Inherited()
{
	init_failed=false;
}

TRIPSAudioSystem::TRIPSAudioSystem(const TRIPSAudioSystem &source) :
Inherited(source)
{
	init_failed=false;
}

TRIPSAudioSystem::~TRIPSAudioSystem(void)
{
	SINFO << "TRIPSAudioSystem Destructor" << std::endl;
	if(GlobalSystemState == OSG::Shutdown) {
	if (FMOD_System != NULL) {
		FMOD_System->close();
        FMOD_System->release();
        SLOG << "FMOD released" << std::endl;
		FMOD_System = NULL;
	}
	} else {
		SINFO << "keep FMOD" << std::endl;
	}
}



/*----------------------------- class specific ----------------------------*/

void TRIPSAudioSystem::changed(ConstFieldMaskArg whichField, 
	UInt32            origin,
	BitVector         details)
{
	Inherited::changed(whichField, origin, details);
      
   // remark:
   // loading of sound files is done here because of the original
   // idea to use a dedicated sound server

   if (getAudiomode() == UInt16(ACTIVE)) {

   FMOD_RESULT       result;

   if (0x0000 != (whichField & SoundlistFieldMask)) {
      SINFO << "Soundlist changed" << std::endl;
      for (UInt32 i=0; i<this->getMFSoundlist()->size32(); ++i) {	
				// check for magic number
				if (getSoundlist(i)->getSoundID() == -1234) {
					SINFO << "*TRIPS* new Sound found in Soundlist" << std::endl;
					TRIPSSoundRefPtr newSound = getSoundlist(i);

               FMOD::Channel *channel=0;
               FMOD::Sound *sample= NULL;
               FMOD_MODE   mode   = FMOD_HARDWARE | FMOD_3D | FMOD_LOOP_OFF;
               FMOD_CREATESOUNDEXINFO exinfo;						// length of memload sample

					if (newSound->getIs3D()) { // 3D Sound
                  
						   memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));                  					   
                     result=FMOD_System->createSound(newSound->getFilename().c_str(),mode, 0, &sample);

                     if (result != FMOD_OK) {
						      SFATAL << "*TRIPS* createSound " << newSound->getFilename() << " failed: " << FMOD_ErrorString[result] << std::endl;
						      newSound->setSoundID(-1);
						   } else {
                        newSound->setSoundID(i);

                        FMOD_VECTOR	f_posi,f_velo;
								Pnt3f	beaconpos;
								Vec3f	vel;

								vel.setValues(0,0,0);
								newSound->getBeacon()->getToWorld().mult(Pnt3f(0.0, 0.0, 0.0), beaconpos);

								f_posi.x = beaconpos[0];
								f_posi.y = beaconpos[1];
								f_posi.z = beaconpos[2];
								f_velo.x = vel[0];
								f_velo.y = vel[1];
								f_velo.z = vel[2];
								result = FMOD_System->playSound(FMOD_CHANNEL_FREE, sample, true, &channel);
                        if (result != FMOD_OK) {
		      	            SFATAL << "*TRIPS* play1 ERROR: " << FMOD_ErrorString[result] << std::endl;			                  
		                  }
                        result = sample->set3DMinMaxDistance(1.0f, 1000000000.0f);
                        if (result != FMOD_OK) {
		      	            SFATAL << "*TRIPS* set3DMinMaxDist ERROR: " << FMOD_ErrorString[result] << std::endl;			                  
		                  }
								result = channel->set3DAttributes(&f_posi,&f_velo);
                        if (result != FMOD_OK) {
		      	            SFATAL << "*TRIPS* set3DAttrib ERROR: " << FMOD_ErrorString[result] << std::endl;			                  
		                  }

                        SINFO << "*TRIPS* Beacon Pos: "<<  f_posi.x << " "
									<< f_posi.y << " "<<  f_posi.z << " "
									<< std::endl;
								SINFO << "*TRIPS* Loaded channel: "<< channel << std::endl;

								newSound->setSample	(sample);
								newSound->setChannel	(channel);
								newSound->setVelocity	(vel);
								SINFO << "*TRIPS* Volume: " << newSound->getSoundVolume() << std::endl;

                     }

               } else {                   // 2D Sound

                  memset(&exinfo, 0, sizeof(FMOD_CREATESOUNDEXINFO));                  					   
                  result=FMOD_System->createSound(newSound->getFilename().c_str(),mode, 0, &sample);

                  if (result != FMOD_OK) {
						   SFATAL << "*TRIPS* createSound " << newSound->getFilename() << " failed: " << FMOD_ErrorString[result] << std::endl;
						   newSound->setSoundID(-1);
						} else {
                     newSound->setSoundID(i);

                     result = FMOD_System->playSound(FMOD_CHANNEL_FREE, sample, true, &channel);
                     if (result != FMOD_OK) {
		      	         SFATAL << "*TRIPS* play1 ERROR: " << FMOD_ErrorString[result] << std::endl;			                  
		               }

                     newSound->setSample	(sample);
							newSound->setChannel	(channel);
                  }

               }
            }
      }      
   }


   //! Listenerupdate for 3D Update in each frame
	if (0x0000 != (whichField & ListenerpositionFieldMask)) {

      FMOD_VECTOR	f_posi,f_velo,f_testvelo,f_lookat,f_up;
      
      TimeStamp nowtime      = getTimeStamp();
      TimeStamp iCounterFreq = getTimeStampFreq();

      Real32 deltatime = Real32(nowtime - Real32(getLastframetime())) / 
          Real32(iCounterFreq);

      setLastframetime(Time(nowtime));
      

		if (getUsevelocity()) {

			setVelocity(getListenerposition() - getLastlistenerposition());
			setLastlistenerposition(getListenerposition());
			//		getVelocity() = (getVelocity() * 1); // timer.time();

			//		SLOG << "L-Velo: " << getVelocity() << std::endl;
			//		SLOG << "deltatime: " << deltatime << std::endl;
			f_testvelo.x = getVelocity()[0] / deltatime;
			f_testvelo.y = getVelocity()[1] / deltatime;
			f_testvelo.z = getVelocity()[2] / deltatime;
			if ((f_testvelo.x)>15)  f_velo.x= 15; else f_velo.x=f_testvelo.x;
			if ((f_testvelo.x)<-15) f_velo.x=-15;
			if ((f_testvelo.y)> 15) f_velo.y= 15; else f_velo.y=f_testvelo.y;
			if ((f_testvelo.y)<-15) f_velo.y=-15;
			if ((f_testvelo.z)>15)  f_velo.z= 15; else f_velo.z=f_testvelo.z;
			if ((f_testvelo.z)<-15) f_velo.z=-15;

		} else {
			f_velo.x=0; f_velo.y=0; f_velo.z=0;
		}

		f_posi.x = getListenerposition()[0];
		f_posi.y = getListenerposition()[1];
		f_posi.z = getListenerposition()[2];

      f_lookat.x = getListenerlookat()[0];
      f_lookat.y = getListenerlookat()[1];
      f_lookat.z = getListenerlookat()[2];

      f_up.x = getListenerup()[0];
      f_up.y = getListenerup()[1];
      f_up.z = getListenerup()[2];

      //SLOG << "pos: " << getListenerposition() << " lookat: " << getListenerlookat() << " up: " << getListenerup() << std::endl;

		result = FMOD_System->set3DListenerAttributes(0, &f_posi, &f_velo, &f_lookat, &f_up);			
      if (result != FMOD_OK) {
		   SFATAL << "*TRIPS* set3DListAtt ERROR: " << FMOD_ErrorString[result] << std::endl;			
		}

      for (UInt32 i=0; i<getMFSoundlist()->size32(); i++) {
                  
			if (getSoundlist(i)->getIs3D() && getSoundlist(i)->getSoundID()>-1)	{
				BoxVolume beaconBBox;
				getSoundlist(i)->getBeacon()->getWorldVolume(beaconBBox);
				Pnt3f	beaconpos;
				beaconBBox.getCenter(beaconpos);

				if (getSoundlist(i)->getUsevelocity())	{
					getSoundlist(i)->setVelocity(beaconpos - getSoundlist(i)->getLastsoundposition());
					getSoundlist(i)->setLastsoundposition( beaconpos );
					//				getSoundlist(i)->getVelocity() = 
					//					(getSoundlist(i)->getVelocity() * 1);
					f_testvelo.x = getSoundlist(i)->getVelocity()[0] / deltatime;
					f_testvelo.y = getSoundlist(i)->getVelocity()[1] / deltatime;
					f_testvelo.z = getSoundlist(i)->getVelocity()[2] / deltatime;
					
					if ((f_testvelo.x)>15)  f_velo.x= 15; else f_velo.x=f_testvelo.x;
					if ((f_testvelo.x)<-15) f_velo.x=-15;
					if ((f_testvelo.y)> 15) f_velo.y= 15; else f_velo.y=f_testvelo.y;
					if ((f_testvelo.y)<-15) f_velo.y=-15;
					if ((f_testvelo.z)>15)  f_velo.z= 15; else f_velo.z=f_testvelo.z;
					if ((f_testvelo.z)<-15) f_velo.z=-15;
					
				} else {
					f_velo.x=0; f_velo.y=0; f_velo.z=0;
				}
				
				f_posi.x = beaconpos[0];
				f_posi.y = beaconpos[1];
				f_posi.z = beaconpos[2];
				
				//SLOG << "S-Velo: " << getSoundlist(i)->getVelocity() << std::endl;
				            
				//FMOD_3D_SetAttributes(getSoundlist(i)->getChannel(), f_posi, f_velo);   

                        
            bool isPlay;
            getSoundlist(i)->getChannel()->isPlaying(&isPlay);
            if (isPlay) {
               result = getSoundlist(i)->getChannel()->set3DAttributes(&f_posi, &f_velo);
               if (result != FMOD_OK) {
                  char name[100];
                  int len=0;
                  (getSoundlist(i)->getSample())->getName(name, len);
		      	   SFATAL << "*TRIPS* Channel->set3DAttr ERROR: " << FMOD_ErrorString[result] << std::endl << "at " << name << std::endl;
		         }
            }
			} 
		}	
		//FMOD_Update();
      //SLOG << "FMOD update" << std::endl;
      result = FMOD_System->update();
      if (result != FMOD_OK) {
		   SFATAL << "*TRIPS* update ERROR: " << FMOD_ErrorString[result] << std::endl;			
		}		
	} // ListenerpositionFieldMask

   }
}

void TRIPSAudioSystem::dump(      UInt32    ,
	const BitVector ) const
{
	SLOG << "Dump TRIPSAudioSystem NI" << std::endl;
}


void TRIPSAudioSystem::adjustVolume(Volume &volume)
{
    volume.setValid();

    // Prevent clipping of the TRIPSAudioSystem so that the pre/post
    // render callbacks are executed each frame:
#if 0 // TODO: warum funktioniert das nicht?
    volume.setEmpty();
    volume.setInfinite();
    //volume.setStatic();
#else
    volume.extendBy(Pnt3f(-100000,-100000,-100000));
    volume.extendBy(Pnt3f(100000,100000,100000));
   // volume.setEmpty();
#endif
    SINFO << "adjustVolume TRIPSAudioSystem done" << std::endl;
}


/*! \brief Registers the per frame function
*/
Action::ResultE TRIPSAudioSystem::renderActionEnter(Action *action)
{
	RenderAction  *ra = dynamic_cast<RenderAction *>(action);
   	
	if ((FMOD_System != NULL) && (getAudiomode() == UInt16(ACTIVE))) {      
		//SLOG << "TRIPSAudioSystem::renderActionEnter NI OK" << std::endl;

      Pnt3f eyepos;
	   Vec3f look, up;
      OSG::Camera* cam = ra->getCamera();
	
	   cam->getBeacon()->getToWorld().mult(Pnt3f(0.0, 0.0, 0.0), eyepos); //?? multFull?
	   cam->getBeacon()->getToWorld().mult(Vec3f(0.0, 0.0, -1.0), look);
	   cam->getBeacon()->getToWorld().mult(Vec3f(0.0, -1.0, 0.0), up);

      look.normalize();
      up.normalize();

      setListenerlookat(look);
      setListenerup(up);
      setListenerposition(eyepos);

      //SINFO << "renderActionEnter: " << eyepos  << std::endl;
      

   } else {
		SINFO << "TRIPSAudioSystem::renderActionEnter NULL" << std::endl;
		if (!init_failed) init_FMOD();
	}


	return Action::Continue;
}



void TRIPSAudioSystem::init_FMOD()
{
	SINFO << "TRIPSAudioSystem::init_FMOD" << std::endl;
	if (FMOD_System == NULL) {
		SINFO << "FMOD_System init" << std::endl;
		FMOD_RESULT  result;
//		unsigned int version;

		// init FMOD system
		if (FMOD::System_Create(&FMOD_System) != FMOD_OK) {
			SFATAL << "*TRIPS* something went wrong while creating the FMOD system" << std::endl;			
			FMOD_System=NULL;
			init_failed = true;
			return;
		} 

      readIniFile(); // reading standards from .ini file


      if (getAudiomode() == UInt16(ACTIVE)) {
         SLOG << "*TRIPS* is in ACTIVE Mode" << std::endl;

         // SetOutput

         switch (getAudiodriver()) 
         {
         case FMOD_OUTPUTTYPE_NOSOUND:
            SLOG << "*TRIPS* Setting Driver to No-Sound...";
            if (FMOD_System->setOutput(FMOD_OUTPUTTYPE_NOSOUND) == FMOD_OK) 
               SLOG << "*TRIPS* Setting Driver succesful "<< std::endl; 
            else {
               SLOG << "*TRIPS* Setting Driver failed!" << std::endl;
               FMOD_System->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
            }
            break;
         case FMOD_OUTPUTTYPE_WINMM:
            SLOG << "*TRIPS* Setting Driver to Windows Multimedia Driver" << std::endl;
            if (FMOD_System->setOutput(FMOD_OUTPUTTYPE_WINMM) == FMOD_OK) 
               SLOG << "*TRIPS* Setting Driver succesful "<< std::endl;
            else {
               SLOG << "*TRIPS* Setting Driver failed!" << std::endl;
               FMOD_System->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
            }
            break;
         case FMOD_OUTPUTTYPE_DSOUND:
            SLOG << "*TRIPS* Setting Driver to Direct Sound" << std::endl;
            if (FMOD_System->setOutput(FMOD_OUTPUTTYPE_DSOUND) == FMOD_OK)
               SLOG << "*TRIPS* Setting Driver succesful "<< std::endl; 
            else {
               SLOG << "*TRIPS* Setting Driver failed!" << std::endl;
               FMOD_System->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
            }
            break;		
         case FMOD_OUTPUTTYPE_ASIO:
            SLOG << "*TRIPS* Setting Driver to ASIO" << std::endl;
            if (FMOD_System->setOutput(FMOD_OUTPUTTYPE_ASIO) == FMOD_OK) 
               SLOG << "*TRIPS* Setting Driver succesful "<< std::endl; 
            else {
               SLOG << "*TRIPS* Setting Driver failed!" << std::endl;
               FMOD_System->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
            }
            break;
         case FMOD_OUTPUTTYPE_OSS:
            SLOG << "*TRIPS* Setting Driver to OSS (Open Sound System)" << std::endl;
            if (FMOD_System->setOutput(FMOD_OUTPUTTYPE_OSS) == FMOD_OK) 
               SLOG << "*TRIPS* Setting Driver succesful "<< std::endl; 
            else {
               SLOG << "*TRIPS* Setting Driver failed!" << std::endl;
               FMOD_System->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
            }
            break;
         case FMOD_OUTPUTTYPE_ESD:
            SLOG << "*TRIPS* Setting Driver to ESD (Enlightment Sound Daemon)" << std::endl;
            if (FMOD_System->setOutput(FMOD_OUTPUTTYPE_ESD) == FMOD_OK) 
               SLOG << "*TRIPS* Setting Driver succesful "<< std::endl; 
            else {
               SLOG << "*TRIPS* Setting Driver failed!" << std::endl;
               FMOD_System->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
            }
            break;
         case FMOD_OUTPUTTYPE_ALSA:
            SLOG << "*TRIPS* Setting Driver to Alsa" << std::endl;
            if (FMOD_System->setOutput(FMOD_OUTPUTTYPE_ALSA) == FMOD_OK) 
               SLOG << "*TRIPS* Setting Driver succesful "<< std::endl;
            else {
               SLOG << "*TRIPS* Setting Driver failed!" << std::endl;
               FMOD_System->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
            }
            break;
         default:
            SFATAL << "*TRIPS* Unknown Audiodriver choosen! Setting to default" << std::endl;
            FMOD_System->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
            break;
         }


         // SETUP SPEAKERS
         FMOD_System->setSpeakerMode(FMOD_SPEAKERMODE(getSpeakermode()));
         switch (getSpeakermode()) 
         {
         case FMOD_SPEAKERMODE_MONO:
            SLOG << "*TRIPS* Setting Speaker Mode to Mono Output (1)" << std::endl;
            break;
         case FMOD_SPEAKERMODE_STEREO:
            SLOG << "*TRIPS* Setting Speaker Mode to Stereo Output (2)" << std::endl;
            break;		
         case FMOD_SPEAKERMODE_QUAD:
            SLOG << "*TRIPS* Setting Speaker Mode to Quad Output (4) " << std::endl;
            break;
         case FMOD_SPEAKERMODE_SURROUND:
            SLOG << "*TRIPS* Setting Speaker Mode to Surround Output (4) " << std::endl;
            break;
         case FMOD_SPEAKERMODE_5POINT1:
            SLOG << "*TRIPS* Setting Speaker Mode to 5.1 (6)" << std::endl;
            break;
         case FMOD_SPEAKERMODE_7POINT1:
            SLOG << "*TRIPS* Setting Speaker Mode to 7.1 (8)" << std::endl;
            break;
         default:
            SFATAL << "*TRIPS* unknown Speakermode! Setting STEREO" << std::endl;
            FMOD_System->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
            break;
         }

         // SETUP Environment
         //		FMOD_REVERB_PROPERTIES arena = FMOD_PRESET_ARENA;
         FMOD_REVERB_PROPERTIES hall = FMOD_PRESET_CONCERTHALL;
         //		FMOD_REVERB_PROPERTIES water = FMOD_PRESET_UNDERWATER;
         FMOD_System->setReverbProperties(&hall);

         SLOG << "**** TRIPS-Audio setup done ****" << std::endl<< std::endl;

         FMOD_System->setDriver(0); //was: -1 ??

         // INITIALIZE
         result = FMOD_System->init(64, FMOD_INITFLAGS(FMOD_INIT_NORMAL), 0);
         if (result != FMOD_OK) {
            SFATAL << "*TRIPS* Init ERROR: " << FMOD_ErrorString[result] << std::endl;            
            FMOD_System=NULL;            
         }
      } else {
         SLOG << "*TRIPS* is in MUTE Mode" << std::endl;
      }

      		
	}

	if (FMOD_System == NULL) {
		init_failed = true;
	}
}



void TRIPSAudioSystem::playStream(std::string filename, bool loop)
{
	//SLOG << "playStream " << filename.c_str() << std::endl;
	if (FMOD_System == NULL) {
		if (!init_failed) init_FMOD(); // unlikely
		if (FMOD_System == NULL) return;
	}

	FMOD_RESULT  result;
	FMOD::Sound* new_sound=NULL;
	FMOD::Channel *channel = 0;
	result = FMOD_System->createStream(filename.c_str(),FMOD_DEFAULT,0,&new_sound);

	if (result != FMOD_OK) {
		SFATAL << "*TRIPS* createStream ERROR: " << FMOD_ErrorString[result] << std::endl;			         
	}

	result = FMOD_System->playSound(FMOD_CHANNEL_FREE, new_sound, false, &channel);
    if (result != FMOD_OK) {
		SFATAL << "*TRIPS* Play ERROR: " << FMOD_ErrorString[result] << std::endl;			         
	}

    if (channel!=NULL) {
       if (loop) result = channel->setMode(FMOD_LOOP_NORMAL);
       else result = channel->setMode(FMOD_LOOP_OFF);
      if (result != FMOD_OK) {
		   SFATAL << "*TRIPS* set to loop ERROR: " << FMOD_ErrorString[result] << std::endl;			         
	   }
      result = channel->setVolume(0.5f);
      if (result != FMOD_OK) {
		   SFATAL << "*TRIPS* set volume ERROR: " << FMOD_ErrorString[result] << std::endl;			         
	   }

    } else {
       SFATAL << "*TRIPS* no channel" << std::endl;			         
    }
	
}


TRIPSSoundTransitPtr TRIPSAudioSystem::addSound(std::string filename,
		                           NodeRefPtr beacon, 
								         bool usevelocity,
								         bool is3D)
{
   if (FMOD_System==NULL) return TRIPSSoundTransitPtr(NULL);
   TRIPSSoundUnrecPtr newSound;
   newSound = TRIPSSound::create();

   newSound->setFilename	(filename);
   newSound->setBeacon		(beacon);
   newSound->setIs3D		(is3D);
   newSound->setUsevelocity(usevelocity);
   newSound->setAudio(this);	
   newSound->setSoundID	(-1234); // magic number
   newSound->setSoundVolume		(255);
   
   this->editMFSoundlist()->push_back(newSound);

   return TRIPSSoundTransitPtr(newSound);
}


FMOD::System* TRIPSAudioSystem::getFMOD_SystemP()
{
   return FMOD_System;
}





Int32 TRIPSAudioSystem::readIniFile(void)
{
	const int buf = 150;
	char datline[buf];
	std::string inifile;
	std::string command, value;
	Vec3f offset;
	
	inifile = getInifilename();
	if (inifile.length()<1) inifile="trips.ini";
	std::ifstream ini(inifile.c_str());
	if (ini.fail()) {
		SFATAL << "Cannot find " << inifile.c_str() << ", trying trips.ini ..." << std::endl;
		inifile="trips.ini";
		std::ifstream ini(inifile.c_str());
	}
	
	if (ini.fail()) { 
		SFATAL << "Cannot find " << inifile.c_str() << "! Must be in same dir as application" << std::endl;
		return 0;
	}
	while (!ini.eof()){
		ini >> command;
		if (command=="#") ini.getline(datline, buf) ;

		/*if (command=="AutoInit") { // ??
			ini >> value;
			if (value == "TRUE")    {setAutoinit(true);}
			if (value == "FALSE")   {setAutoinit(false);}
			ini.getline(datline, buf) ;
		}*/

		if (command=="AudioMode") {
			ini >> value;
			if (value == "ACTIVE") {setAudiomode(ACTIVE);}
			if (value == "MUTE")   {setAudiomode(MUTE);}
			ini.getline(datline, buf) ;
		}
		
		if (command=="AudioDriver") {
			ini >> value;
			setAudiodriver(-1);
			if (value == "FMOD_OUTPUTTYPE_NOSOUND") setAudiodriver(FMOD_OUTPUTTYPE_NOSOUND);
			// Windows
			if (value == "FMOD_OUTPUTTYPE_WINMM")  setAudiodriver(FMOD_OUTPUTTYPE_WINMM);
			if (value == "FMOD_OUTPUTTYPE_DSOUND") setAudiodriver(FMOD_OUTPUTTYPE_DSOUND);			
			if (value == "FMOD_OUTPUTTYPE_ASIO")   setAudiodriver(FMOD_OUTPUTTYPE_ASIO);
			// Linux
			if (value == "FMOD_OUTPUTTYPE_OSS")    setAudiodriver(FMOD_OUTPUTTYPE_OSS);
			if (value == "FMOD_OUTPUTTYPE_ESD")    setAudiodriver(FMOD_OUTPUTTYPE_ESD);
			if (value == "FMOD_OUTPUTTYPE_ALSA")   setAudiodriver(FMOD_OUTPUTTYPE_ALSA);
			ini.getline(datline, buf) ;
		}
		
		if (command=="SpeakerMode") {
			ini >> value;
			setSpeakermode(-1);
			if (value == "FMOD_SPEAKERMODE_MONO")         setSpeakermode(FMOD_SPEAKERMODE_MONO);
			if (value == "FMOD_SPEAKERMODE_STEREO")       setSpeakermode(FMOD_SPEAKERMODE_STEREO);			
			if (value == "FMOD_SPEAKERMODE_QUAD")         setSpeakermode(FMOD_SPEAKERMODE_QUAD);
			if (value == "FMOD_SPEAKERMODE_SURROUND")     setSpeakermode(FMOD_SPEAKERMODE_SURROUND);			
			if (value == "FMOD_SPEAKERMODE_5POINT1")      setSpeakermode(FMOD_SPEAKERMODE_5POINT1);
         if (value == "FMOD_SPEAKERMODE_7POINT1")      setSpeakermode(FMOD_SPEAKERMODE_7POINT1);
			ini.getline(datline, buf) ;
		}
		
		if (command=="ListenerOffset"){
			ini >> offset[0];
			ini >> offset[1];
			ini >> offset[2];
			setListeneroffset(offset);
			ini.getline(datline, buf) ;
		}
		
	}
	ini.close();
	return 1;
}



OSG_END_NAMESPACE
