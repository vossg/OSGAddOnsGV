/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                 Copyright (C) 2003 by the OpenSG Forum                    *
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

#ifndef _OSGCONTRIBSOFADEF_H_
#define _OSGCONTRIBLSOFADEF_H_
#ifdef __sgi
#pragma once
#endif

//---------------------------------------------------------------------------
//  Defines
//---------------------------------------------------------------------------

#if defined(WIN32)
#   ifdef OSG_COMPILECONTRIBSOFALIB
#       define OSG_CONTRIBSOFA_DLLMAPPING     __declspec(dllexport)
#       define OSG_CONTRIBSOFA_DLLTMPLMAPPING __declspec(dllexport)
#       define OSG_CONTRIBSOFA_EXPIMP_TMPL      
#   else
#       define OSG_CONTRIBSOFA_DLLMAPPING     __declspec(dllimport)
#       define OSG_CONTRIBSOFA_DLLTMPLMAPPING __declspec(dllimport)
#       define OSG_CONTRIBSOFA_EXPIMP_TMPL      extern
#   endif
#else
#define OSG_CONTRIBSOFA_DLLMAPPING
#define OSG_CONTRIBSOFA_DLLTMPLMAPPING
#endif


#define BEGIN_SOFA_CORE_TOPO_NAMESPACE \
    namespace sofa { namespace core { namespace topology {
#define END_SOFA_CORE_TOPO_NAMESPACE } } }

#define BEGIN_SOFA_CORE_BEHAVIOR_NAMESPACE \
    namespace sofa { namespace core{ namespace behavior {
#define END_SOFA_CORE_BEHAVIOR_NAMESPACE } } } 

#define BEGIN_SOFA_CORE_VISUAL_NAMESPACE \
    namespace sofa { namespace core { namespace visual {
#define END_SOFA_CORE_VISUAL_NAMESPACE } } }

#define BEGIN_SOFA_SIM_NAMESPACE namespace sofa { namespace simulation {
#define END_SOFA_SIM_NAMESPACE } }

#define BEGIN_SOFA_CMP_VISMODEL_NAMESPACE \
    namespace sofa { namespace component { namespace visualmodel {
#define END_SOFA_CMP_VISMODEL_NAMESPACE } } }

#define BEGIN_SOFA_GUI_QT_VIEWER_NAMESPACE \
    namespace sofa { namespace gui { namespace qt { namespace viewer {
#define END_SOFA_GUI_QT_VIEWER_NAMESPACE } } } }

#endif /* _OSGCONTRIBSOFADEF_H_ */
