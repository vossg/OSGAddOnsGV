/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *           Copyright (C) 2000,2001,2002 by the OpenSG Forum                *
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
#include <boost/bind.hpp>

#include "OSGConfig.h"


#include "OSGRenderAction.h"

#include "OSGMaterialDrawable.h"
#include "OSGMatrixUtility.h"

#include "OSGRRTStage.h"

#define OSG_DUMP_TRAVERSAL

OSG_BEGIN_NAMESPACE

ActionBase::ResultE RRTStageRenderEnter(NodeCore * const pCore,
                                        Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Enter RRTStage %p\n", &(*pCore)));
#endif


    RenderAction *a      = dynamic_cast<RenderAction *>(action);

    RRTStage     *pStage = dynamic_cast<RRTStage *>(pCore);


    a->pushPartition(0, RenderPartition::SimpleCallback);
    {
        Viewport          *pPort    = a->getViewport();
        RenderPartition   *pPart    = a->getActivePartition();
        FrameBufferObject *pTarget  = pStage->getRenderTarget();
        
#ifdef OSG_DEBUGX
        std::string szMessage("PostProcessPartition\n");
        pPart->setDebugString(szMessage          );
#endif

        pPart->setRenderTarget(pTarget);

        if(pPort != NULL)
        {
//            pPart->setViewport(pPort         );
            pPart->setWindow  (a->getWindow());
            
            pPart->calcViewportDimension(pPort->getLeft  (),
                                         pPort->getBottom(),
                                         pPort->getRight (),
                                         pPort->getTop   (),
                                         
                                         a->getWindow()->getWidth (),
                                         a->getWindow()->getHeight());
            
            Matrix m, t;
            
            m.setIdentity();
            t.setIdentity();
            
            MatrixOrthogonal( m,
                              0.f, 1.f,
                              0.f, 1.f,
                              -1.f, 1.f);
            
            pPart->setupProjection(m, t);
            
            RenderPartition::SimpleDrawCallback f;
            
            pStage->run(a->getCamera(),
                        a->getBackground(),
                        a->getActNode());

            f = boost::bind(&RRTStage::postProcess, pStage, _1);
            
            pPart->dropFunctor(f);
        }
    }
    a->popPartition();

    return Action::Skip;
}

ActionBase::ResultE RRTStageRenderLeave(NodeCore * const pCore,
                                        Action   *       action)
{
#ifdef OSG_DUMP_TRAVERSAL
    FDEBUG_GV(("Leave RRTStage %p\n", &(*pCore)));
#endif

    return Action::Skip;
}


/*-------------------------------------------------------------------------*/
/*                                   init                                  */

bool RRTActionInitialize(void)
{
    RenderAction::registerEnterDefault(
        RRTStage::getClassType(), 
              RRTStageRenderEnter);

    RenderAction::registerLeaveDefault( 
        RRTStage::getClassType(), 
              RRTStageRenderLeave);

    return true;
}

struct RRTActionInit
{
    RRTActionInit(void) 
    { 
        addPostFactoryInitFunction(RRTActionInitialize); 
    }
};

static RRTActionInit initTestDummy;

OSG_END_NAMESPACE





/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

/*-------------------------------------------------------------------------*/
/*                             Assignment                                  */

/*-------------------------------------------------------------------------*/
/*                             Comparison                                  */

/*-------------------------------------------------------------------------*/
/*                              cvs id's                                   */
