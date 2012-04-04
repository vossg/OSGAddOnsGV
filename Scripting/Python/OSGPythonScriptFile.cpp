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

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include "OSGConfig.h"
#include "OSGSceneFileHandler.h"

#include "OSGPythonScriptFile.h"

OSG_BEGIN_NAMESPACE

// Documentation for this class is emitted in the
// OSGPythonScriptFileBase.cpp file.
// To modify it, please change the .fcd file (OSGPythonScriptFile.fcd) and
// regenerate the base file.

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

void PythonScriptFile::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
#ifdef WIN32_X
        typedef OSGSceneFileType::PostLoadingDispatcher<
                  PythonScriptFile> PLDPythonScriptFile;

        OSGSceneFileType::the().registerEndNodeCallback(
            PythonScriptFile::getClassType(),
            boost::bind(&PLDPythonScriptFile::dispatch,
                        PLDPythonScriptFile(), _1)); 
#endif
    }
}

void PythonScriptFile::postOSGLoading(FileContextAttachment * const pContext)
{
    Inherited::postOSGLoading(pContext);

    if(_sfScriptUrl.getValue().empty() == false)
    {
        std::string szFilenameResolved =
            SceneFileHandler::the()->getPathHandler()->findFile(
                _sfScriptUrl.getValue().c_str());

        if(szFilenameResolved.empty() == false)
        {
            std::string szProgram;
            if(readProgram(szProgram, szFilenameResolved.c_str()))
            {
                Inherited::setScript(szProgram);
            }
        }
        else
        {
            std::cerr << "PythonScriptFile: cannot find file "
                      << _sfScriptUrl.getValue() << std::endl;
        }
    }
}

bool PythonScriptFile::readProgram(std::string  &szTarget,
                                   const Char8  *szFilename)
{
    std::ifstream s(szFilename);

    if(s.good())
    {
        return readProgram(szTarget, s);
    }
    else
    {
        FWARNING(("PythonScriptFile::readProgram: couldn't open '%s' "
                  "for reading!\n", szFilename));

        return false;
    }

}

bool PythonScriptFile::readProgram(std::string  &szTarget,
                                   std::istream &iStream)
{
#define BUFSIZE 200

    szTarget.erase();

    Char8 buf[BUFSIZE];

    if(!iStream.good())
    {
        FWARNING(("PythonScriptFile::readProgram: stream is not good!\n"));

        return false;
    }

    do
    {
        iStream.read(buf, BUFSIZE);

        szTarget.append(buf, iStream.gcount());
    }
    while(!iStream.eof());

    return true;
}

/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*-------------------------------------------------------------------------*\
 -  private                                                                 -
\*-------------------------------------------------------------------------*/

/*----------------------- constructors & destructors ----------------------*/

PythonScriptFile::PythonScriptFile(void) :
    Inherited()
{
}

PythonScriptFile::PythonScriptFile(const PythonScriptFile &source) :
    Inherited(source)
{
}

PythonScriptFile::~PythonScriptFile(void)
{
}

/*----------------------------- class specific ----------------------------*/

void PythonScriptFile::changed(ConstFieldMaskArg whichField, 
                            UInt32            origin,
                            BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

void PythonScriptFile::dump(      UInt32    ,
                         const BitVector ) const
{
    SLOG << "Dump PythonScriptFile NI" << std::endl;
}

OSG_END_NAMESPACE
