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

#ifndef _OSGSOFAVISUALVISITOR_H_
#define _OSGSOFAVISUALVISITOR_H_

#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/ExecParams.h>
#include <sofa/simulation/common/Node.h>
#include <sofa/simulation/common/Visitor.h>
#include <sofa/core/visual/VisualModel.h>
#include <iostream>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>

#include "OSGContribSOFADef.h"

#include "OSGTransform.h"
#include "OSGShadowStage.h"

BEGIN_SOFA_SIM_NAMESPACE

class OSG_CONTRIBSOFA_DLLMAPPING OSGLightVisitor : public Visitor
{
  protected:

    OSG::NodeUnrecPtr        _pAttachNode;
    OSG::NodeUnrecPtr        _pParent;
    OSG::ShadowStageUnrecPtr _pShadowStage;

    bool                     _bTurnOffLights;
    
  public:

    OSGLightVisitor(const core::ExecParams *pParams);
    virtual ~OSGLightVisitor(void);

    bool             getTurnOffLights(void                     ) const;
    void             setTurnOffLights(bool                     );

    virtual void processVisualModel  (simulation  ::Node        *, 
                                      core::visual::VisualModel *pVisModel);

    virtual Result processNodeTopDown(simulation  ::Node        *pNode    );


    virtual const char *getClassName(void) const;


    OSG::Node        *getAttachNode     (void                     ) const;
    void              setAttachNode     (OSG::Node        *pNode  );

    OSG::Node        *getOSG2Parent     (void                     ) const;
    void              setOSG2Parent     (OSG::Node        *pParent);

    OSG::ShadowStage *getOSG2ShadowStage(void                     ) const;
    void              setOSG2ShadowStage(OSG::ShadowStage *pShadow);

  private:

    // prohibit default functions (move to 'public' if you need one)
    OSGLightVisitor(const OSGLightVisitor &source);
    void operator =(const OSGLightVisitor &source);
};

class  OSG_CONTRIBSOFA_DLLMAPPING OSGVisualUpdateVisitor : public Visitor
{
  protected:

    OSG::NodeUnrecPtr _pParent;
    OSG::NodeUnrecPtr _pChunkOverrideGroupNode;
    
  public:

    OSGVisualUpdateVisitor(const core::ExecParams *pParams);
    virtual ~OSGVisualUpdateVisitor(void);

    virtual void   processVisualModel(simulation  ::Node        *, 
                                      core::visual::VisualModel *pVisModel);
    virtual Result processNodeTopDown(simulation  ::Node        *pNode    );


    virtual const char *getClassName(void) const;


    void       setOSG2Parent            (OSG::Node *pParent);
    OSG::Node *getOSG2Parent            (void              ) const;

    void       setChunkOverrideGroupNode(OSG::Node *pGp    );
    OSG::Node *getChunkOverrideGroupNode(void              ) const;

  private:

    // prohibit default functions (move to 'public' if you need one)
    OSGVisualUpdateVisitor(const OSGVisualUpdateVisitor &source);
    void operator =(const OSGVisualUpdateVisitor &source);
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGVisualInitVisitor : public Visitor
{
  protected:

  public:

    OSGVisualInitVisitor(const core::ExecParams *pParams);
    virtual ~OSGVisualInitVisitor(void);

    virtual void   processVisualModel(simulation  ::Node        *, 
                                      core::visual::VisualModel *pVisModel);
    virtual Result processNodeTopDown(simulation  ::Node        *pNode    );


    virtual const char *getClassName(void) const;

  private:

    // prohibit default functions (move to 'public' if you need one)
    OSGVisualInitVisitor(const OSGVisualInitVisitor &source);
    void operator =(const OSGVisualInitVisitor &source);
};

END_SOFA_SIM_NAMESPACE

#include "OSGSofaVisualVisitor.inl"

#endif // _OSGSOFAVISUALVISITOR_H_
