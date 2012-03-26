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

#ifndef _OSGSOFATETRAHEDRALMODEL_H_
#define _OSGSOFATETRAHEDRALMODEL_H_

//#include <sofa/core/visual/VisualModel.h>
//#include <sofa/core/topology/BaseMeshTopology.h>
//#include <sofa/core/behavior/MechanicalState.h>
//#include <sofa/component/component.h>
//#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/component/visualmodel/OglTetrahedralModel.h>

#include "OSGContribSOFADef.h"
#include "OSGTransform.h"

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

/**
 *  \brief Render 3D models with tetrahedra.
 *
 *  This is a basic class using tetrehedra for the rendering
 *  instead of common triangles. It loads its data with
 *  a BaseMeshTopology and a MechanicalState.
 *  This rendering is only available with Nvidia's >8 series
 *  and Ati's >2K series.
 *
 */

template<class DataTypes>
class OSG2_TetrahedralModel : public core::visual::VisualModel
{
  public:
    SOFA_CLASS(OSG2_TetrahedralModel, core::visual::VisualModel);
    //SOFA_CLASS(OSG2_TetrahedralModel, core::visual::VisualModel);

    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::VecCoord VecCoord;

  private:
    core::behavior::MechanicalState<DataTypes>* nodes;
    core::topology::BaseMeshTopology* topo;

    Data<bool> depthTest;
    Data<bool> blending;

    OSG::NodeRecPtr _attachNode;

  protected:
    OSG2_TetrahedralModel();
    virtual ~OSG2_TetrahedralModel();
  public:
    void init();
    void initVisual();
    void computeBBox(const core::ExecParams *);

    // OpenSG
    OSG::NodeRecPtr getAttachNode() const { return _attachNode; }

    virtual std::string getTemplateName() const
        {
            return templateName(this);
        }

    static std::string templateName(const OSG2_TetrahedralModel<DataTypes>* = NULL)
        {
            return DataTypes::Name();
        }
        
    /// Pre-construction check method called by ObjectFactory.
    /// Check that DataTypes matches the MechanicalState.
    template<class T>
    static bool canCreate(T*& obj, core::objectmodel::BaseContext* context, core::objectmodel::BaseObjectDescription* arg)
        {
            if (dynamic_cast<core::behavior::MechanicalState<DataTypes>*>(context->getMechanicalState()) == NULL)
                return false;
            return core::objectmodel::BaseObject::canCreate(obj, context, arg);
        }

};

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFATETRAHEDRALMODEL_H_
