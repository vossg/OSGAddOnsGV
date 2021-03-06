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

#ifndef _OSGTRIPSAUDIOSYSTEM_H_
#define _OSGTRIPSAUDIOSYSTEM_H_
#ifdef __sgi
#pragma once
#endif

#include "OSGTRIPSAudioSystemBase.h"
#include "OSGRenderAction.h"
#include "OSGTRIPSSound.h"

namespace FMOD {
	class System;
}

OSG_BEGIN_NAMESPACE

/*! \brief TRIPSAudioSystem class. See \ref
           PageTRIPSTRIPSAudioSystem for a description.
*/

class OSG_TRIPS_DLLMAPPING TRIPSAudioSystem : public TRIPSAudioSystemBase
{
  protected:

    /*==========================  PUBLIC  =================================*/

  public:

     enum AudioModeType 
	  {
		  MUTE,
		  ACTIVE
	  };


    typedef TRIPSAudioSystemBase Inherited;
    typedef TRIPSAudioSystem     Self;

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

    // Variables should all be in TRIPSAudioSystemBase.

    /*---------------------------------------------------------------------*/
    /*! \name                  Constructors                                */
    /*! \{                                                                 */

    TRIPSAudioSystem(void);
    TRIPSAudioSystem(const TRIPSAudioSystem &source);



    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructors                                */
    /*! \{                                                                 */

    virtual ~TRIPSAudioSystem(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                      Init                                    */
    /*! \{                                                                 */

    static void initMethod(InitPhase ePhase);

	 /*! \}                                                                */
    /*---------------------------------------------------------------------*/
    /*! \name                      constant updates                        */
    /*! \{                                                                 */
	
    Action::ResultE renderActionEnter(Action *action);
	

	 /*! \}                                                                */

    void adjustVolume(Volume &volume);
    
public:

	/*---------------------------------------------------------------------*/
    /*! \name                      FMOD calls                              */
    /*! \{                                                                 */

   void playStream(std::string filename, bool loop=false);


	TRIPSSoundTransitPtr addSound(std::string filename,
		                           NodeRefPtr beacon, 
								         bool usevelocity,
								         bool is3D);

   FMOD::System* getFMOD_SystemP();

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:


    static FMOD::System *FMOD_System;
		

	bool init_failed;
	void init_FMOD();

   Int32 readIniFile(void);



    friend class FieldContainer;
    friend class TRIPSAudioSystemBase;

    // prohibit default functions (move to 'public' if you need one)
    void operator =(const TRIPSAudioSystem &source);
};

typedef TRIPSAudioSystem *TRIPSAudioSystemP;

OSG_END_NAMESPACE

#include "OSGTRIPSAudioSystemBase.inl"
#include "OSGTRIPSAudioSystem.inl"

#endif /* _OSGTRIPSAUDIOSYSTEM_H_ */
