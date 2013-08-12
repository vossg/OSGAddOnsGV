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

#include "OSGTRIPSSound.h"
#include "OSGTRIPSAudioSystem.h"
#include "OSGNode.h"

#include <fmod.hpp>

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGTRIPSSoundBase.cpp file.
// To modify it, please change the .fcd file (OSGTRIPSSound.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void TRIPSSound::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
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

TRIPSSound::TRIPSSound(void) :
    Inherited()
{
}

TRIPSSound::TRIPSSound(const TRIPSSound &source) :
    Inherited(source)
{
}

TRIPSSound::~TRIPSSound(void)
{
}

/*----------------------------- class specific ----------------------------*/

void TRIPSSound::changed(ConstFieldMaskArg whichField, 
                            UInt32            origin,
                            BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void TRIPSSound::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump TRIPSSound NI" << std::endl;
}



void TRIPSSound::play(TRIPSSound::PlayModeType mode)
{
   SINFO << "Play TRIPSSound WIP" << std::endl;
   if ((this->getAudio()==NULL) || (this->getAudio()->getFMOD_SystemP()==NULL)) {
      SINFO << "TRIPSSound getAudio()==NULL" << std::endl;
      return;
   }
   switch (mode) {
   case ONCE: 
      this->getAudio()->getFMOD_SystemP()->playSound(FMOD_CHANNEL_FREE,m_sample,false,&m_channel);
      break;
   case STOP:
      if (m_channel != NULL) {
         m_channel->stop();
      }      
      break;
       default:
           break;
   }
   
   
}



void TRIPSSound::setSample(FMOD::Sound *sample)
{
   m_sample = sample;
}
void TRIPSSound::setChannel(FMOD::Channel *channel)
{
   m_channel = channel;
}


FMOD::Sound * TRIPSSound::getSample()
{
   return m_sample;
}
FMOD::Channel * TRIPSSound::getChannel()
{
   return m_channel;
}


OSG_END_NAMESPACE
