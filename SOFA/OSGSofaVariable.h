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

#ifndef _OSGSOFAVARIABLE_H_
#define _OSGSOFAVARIABLE_H_

#include <sofa/core/visual/VisualModel.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/helper/gl/template.h>


#include "OSGContribSOFADef.h"
#include "OSGSofaShader.h"


//#ifdef IN2_SOFA
//#define SOFA_OPENGL_VISUAL_API SOFA_EXPORT_DYNAMIC_LIBRARY
//#else
//#define SOFA_OPENGL_VISUAL_API  
//#endif

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

/**
 *  \brief Defines an uniform variable for a OSGShader.
 *
 *  This is an abstract class which pass a value to an uniform
 *  variable defined into the shader.
 *  At the moment, following types are supported :
 *   - int, ivec2, ivec3, ivec4;
 *   - float, vec2, vec3, vec4;
 *   - int[], ivec2[], ivec3[], ivec4[];
 *   - float[], vec2[], vec3[], vec4[];
 */

template<class DataTypes>
class OSGVariable : public core::visual::VisualModel, public OSGShaderElement
{
  public:
    SOFA_CLASS2(OSGVariable, core::visual::VisualModel, OSGShaderElement);
  protected:
    Data< DataTypes > value;

    OSGVariable(): value(initData(&value, DataTypes(), "value", "Set Uniform Value"))
        {
            addAlias(&value, "values"); // some variable types hold multiple values, so we authorize both names for this attribute
        }

    virtual ~OSGVariable(){}
  public:
    virtual void setValue( const DataTypes& v ) { value.setValue(v); system("pause"); }
    void init(){ OSGShaderElement::init(); }
    void initVisual(){ core::visual::VisualModel::initVisual(); }
    void pushValue() { initVisual(); }
    void reinit() { init(); initVisual(); }
};

/** SINGLE INT VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSGIntVariable : public OSGVariable< int>
{
  public:
    SOFA_CLASS(OSGIntVariable, OSGVariable< int>);

    OSGIntVariable();
    virtual ~OSGIntVariable() { }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGInt2Variable : public OSGVariable<defaulttype::Vec<2, int> >
{

  public:
    SOFA_CLASS(OSGInt2Variable, SOFA_TEMPLATE(OSGVariable, SOFA_TEMPLATE2(defaulttype::Vec, 2, int)));

    OSGInt2Variable();
    virtual ~OSGInt2Variable(){ }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGInt3Variable : public OSGVariable<defaulttype::Vec<3, int> >
{
  public:
    SOFA_CLASS(OSGInt3Variable, SOFA_TEMPLATE(OSGVariable, SOFA_TEMPLATE2(defaulttype::Vec, 3, int)));

    OSGInt3Variable();
    virtual ~OSGInt3Variable(){ }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGInt4Variable : public OSGVariable<defaulttype::Vec<4, int> >
{
  public:
    SOFA_CLASS(OSGInt4Variable, SOFA_TEMPLATE(OSGVariable, SOFA_TEMPLATE2(defaulttype::Vec, 4, int)));

    OSGInt4Variable();
    virtual ~OSGInt4Variable(){ }

    void initVisual();
};

/** SINGLE FLOAT VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSGFloatVariable : public OSGVariable<float>
{
  public:
    SOFA_CLASS(OSGFloatVariable, OSGVariable<float>);

    OSGFloatVariable();
    virtual ~OSGFloatVariable() { }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGFloat2Variable : public OSGVariable<defaulttype::Vec2f>
{
  public:
    SOFA_CLASS(OSGFloat2Variable, OSGVariable<defaulttype::Vec2f>);

    OSGFloat2Variable();
    virtual ~OSGFloat2Variable(){ }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGFloat3Variable : public OSGVariable<defaulttype::Vec3f>
{
  public:
    SOFA_CLASS(OSGFloat3Variable, OSGVariable<defaulttype::Vec3f>);

    OSGFloat3Variable();
    virtual ~OSGFloat3Variable(){ }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGFloat4Variable : public OSGVariable<defaulttype::Vec4f>
{
  public:
    SOFA_CLASS(OSGFloat4Variable, OSGVariable<defaulttype::Vec4f>);

    OSGFloat4Variable();
    virtual ~OSGFloat4Variable(){ }

    void initVisual();
};

/** INT VECTOR VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSGIntVectorVariable : public OSGVariable<helper::vector<GLint> >
{
  public:
    SOFA_CLASS(OSGIntVectorVariable, OSGVariable<helper::vector<GLint> >);

    OSGIntVectorVariable();
    virtual ~OSGIntVectorVariable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGIntVector2Variable : public OSGIntVectorVariable
{

  public:
    SOFA_CLASS(OSGIntVector2Variable, OSGIntVectorVariable);

    OSGIntVector2Variable();
    virtual ~OSGIntVector2Variable(){ }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGIntVector3Variable : public OSGIntVectorVariable
{
  public:
    SOFA_CLASS(OSGIntVector3Variable, OSGIntVectorVariable);

    OSGIntVector3Variable();
    virtual ~OSGIntVector3Variable(){ }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGIntVector4Variable : public OSGIntVectorVariable
{
  public:
    SOFA_CLASS(OSGIntVector4Variable, OSGIntVectorVariable);

    OSGIntVector4Variable();
    virtual ~OSGIntVector4Variable(){ }

    virtual void init();
    virtual void initVisual();
};

/** FLOAT VECTOR VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSGFloatVectorVariable : public OSGVariable<helper::vector<float> >
{
  public:
    SOFA_CLASS(OSGFloatVectorVariable, OSGVariable<helper::vector<float> >);

    OSGFloatVectorVariable();
    virtual ~OSGFloatVectorVariable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGFloatVector2Variable : public OSGFloatVectorVariable
{
  public:
    SOFA_CLASS(OSGFloatVector2Variable,OSGFloatVectorVariable);

    OSGFloatVector2Variable();
    virtual ~OSGFloatVector2Variable(){ }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGFloatVector3Variable : public OSGFloatVectorVariable
{
  public:
    SOFA_CLASS(OSGFloatVector3Variable,OSGFloatVectorVariable);

    OSGFloatVector3Variable();
    virtual ~OSGFloatVector3Variable(){ }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGFloatVector4Variable : public OSGFloatVectorVariable
{
  public:
    SOFA_CLASS(OSGFloatVector4Variable,OSGFloatVectorVariable);

    OSGFloatVector4Variable();
    virtual ~OSGFloatVector4Variable(){ }

    virtual void init();
    virtual void initVisual();
};

/** Matrix VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSGMatrix2Variable : public OSGVariable<helper::vector<float> >
{
  public:
    SOFA_CLASS(OSGMatrix2Variable,OSGVariable<helper::vector<float> >);

    Data<bool> transpose;

    OSGMatrix2Variable();
    virtual ~OSGMatrix2Variable() { }

    virtual void setTranspose( const bool& v ) { transpose.setValue(v); }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGMatrix3Variable : public OSGMatrix2Variable
{
  public:
    SOFA_CLASS(OSGMatrix3Variable,OSGMatrix2Variable);

    OSGMatrix3Variable();
    virtual ~OSGMatrix3Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGMatrix4Variable : public OSGMatrix2Variable
{
  public:
    SOFA_CLASS(OSGMatrix4Variable,OSGMatrix2Variable);

    OSGMatrix4Variable();
    virtual ~OSGMatrix4Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGMatrix2x3Variable : public OSGMatrix2Variable
{
  public:
    SOFA_CLASS(OSGMatrix2x3Variable,OSGMatrix2Variable);

    OSGMatrix2x3Variable();
    virtual ~OSGMatrix2x3Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGMatrix3x2Variable : public OSGMatrix2Variable
{
  public:
    SOFA_CLASS(OSGMatrix3x2Variable,OSGMatrix2Variable);

    OSGMatrix3x2Variable();
    virtual ~OSGMatrix3x2Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGMatrix2x4Variable : public OSGMatrix2Variable
{
  public:
    SOFA_CLASS(OSGMatrix2x4Variable,OSGMatrix2Variable);

    OSGMatrix2x4Variable();
    virtual ~OSGMatrix2x4Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGMatrix4x2Variable : public OSGMatrix2Variable
{
  public:
    SOFA_CLASS(OSGMatrix4x2Variable,OSGMatrix2Variable);

    OSGMatrix4x2Variable();
    virtual ~OSGMatrix4x2Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGMatrix3x4Variable : public OSGMatrix2Variable
{
  public:
    SOFA_CLASS(OSGMatrix3x4Variable,OSGMatrix2Variable);

    OSGMatrix3x4Variable();
    virtual ~OSGMatrix3x4Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGMatrix4x3Variable : public OSGMatrix2Variable
{
  public:
    SOFA_CLASS(OSGMatrix4x3Variable,OSGMatrix2Variable);

    OSGMatrix4x3Variable();
    virtual ~OSGMatrix4x3Variable() { }

    virtual void init();
    virtual void initVisual();
};

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFAVARIABLE_H_
