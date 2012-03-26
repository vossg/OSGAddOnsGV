/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                 Copyright (C) 2012 by the OpenSG Forum                    *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 * contact: dirk            'at' opensg.org                                  * 
 *          gerrit.voss     'at' vossg.org                                   *
 *          carsten_neumann 'at' gmx.net                                     *
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

#ifndef _OSGSOFAVISUALMODELPOLICY_H_
#define _OSGSOFAVISUALMODELPOLICY_H_

//#include "initPlugin.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/core/visual/VisualParams.h>

#include "OSGContribSOFADef.h"
#include "OSGSofaDrawTool.h"

BEGIN_SOFA_GUI_QT_VIEWER_NAMESPACE

class OSG_CONTRIBSOFA_DLLMAPPING OSGVisualModelPolicy
{
  public:

    OSGVisualModelPolicy(core::visual::VisualParams *pVParams = 
                             core::visual::VisualParams::defaultInstance());
    virtual ~OSGVisualModelPolicy(void);

    virtual void load  (void) = 0;
    virtual void unload(void) = 0;

  protected:

    sofa::core::visual::VisualParams *_pVParams;

  private:

    // prohibit default functions (move to 'public' if you need one)
    OSGVisualModelPolicy(const OSGVisualModelPolicy &source);
    void operator =(const OSGVisualModelPolicy &source);
};


class OSG_CONTRIBSOFA_DLLMAPPING OSGModelPolicy : 
    public OSGVisualModelPolicy
{
  public:

    OSGModelPolicy(core::visual::VisualParams *pVParams = 
                       core::visual::VisualParams::defaultInstance());

    virtual ~OSGModelPolicy(void);

  protected:

    typedef std::map <std::string, std::string> AliasMap;
    typedef std::pair<std::string, std::string> AliasPair;

    sofa::core::ObjectFactory::ClassEntry *_pClassVisualModel_1;
    sofa::core::ObjectFactory::ClassEntry *_pClassVisualModel_2;
    sofa::core::ObjectFactory::ClassEntry *_pClassVisualModel_3;
    sofa::core::ObjectFactory::ClassEntry *_pClassVisualModel_4;
    sofa::core::ObjectFactory::ClassEntry *_pClassVisualModel_5;

    sofa::core::ObjectFactory::ClassEntry *_pShaderClass;
    sofa::core::ObjectFactory::ClassEntry *_pStateClass;
  
    //sofa::core::visual::DrawToolGL drawTool;
    // should we collect the draw calls? they are for debugging purposes after
    // all
    sofa::core::visual::OSGDrawTool        _oDrawTool;

   
    AliasMap                               _mShaderMap;
    AliasMap                               _mStateMap;

    void createStateAliases (void                                         );
    void createShaderAliases(void                                         );
    
    void aliasMap           (AliasMap                              &oAMap, 
                             sofa::core::ObjectFactory::ClassEntry *pEntry);
    
    void unAliasMap         (AliasMap                              &oAMap, 
                             sofa::core::ObjectFactory::ClassEntry *pEntry);

  public:

    virtual void load  (void);
    virtual void unload(void);

  private:

    // prohibit default functions (move to 'public' if you need one)
    OSGModelPolicy(const OSGModelPolicy &source);
    void operator =(const OSGModelPolicy &source);
};

END_SOFA_GUI_QT_VIEWER_NAMESPACE

#include "OSGSofaVisualModelPolicy.inl"

#endif // _OSGSOFAVISUALMODELPOLICY_H_
