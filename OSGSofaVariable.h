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
 *  \brief Defines an uniform variable for a OSG2_Shader.
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
class OSG2_Variable : public core::visual::VisualModel, public OSG2_ShaderElement
{
  public:
    SOFA_CLASS2(OSG2_Variable, core::visual::VisualModel, OSG2_ShaderElement);
  protected:
    Data< DataTypes > value;

    OSG2_Variable(): value(initData(&value, DataTypes(), "value", "Set Uniform Value"))
        {
            addAlias(&value, "values"); // some variable types hold multiple values, so we authorize both names for this attribute
        }

    virtual ~OSG2_Variable(){}
  public:
    virtual void setValue( const DataTypes& v ) { value.setValue(v); system("pause"); }
    void init(){ OSG2_ShaderElement::init(); }
    void initVisual(){ core::visual::VisualModel::initVisual(); }
    void pushValue() { initVisual(); }
    void reinit() { init(); initVisual(); }
};

/** SINGLE INT VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSG2_IntVariable : public OSG2_Variable< int>
{
  public:
    SOFA_CLASS(OSG2_IntVariable, OSG2_Variable< int>);

    OSG2_IntVariable();
    virtual ~OSG2_IntVariable() { }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Int2Variable : public OSG2_Variable<defaulttype::Vec<2, int> >
{

  public:
    SOFA_CLASS(OSG2_Int2Variable, SOFA_TEMPLATE(OSG2_Variable, SOFA_TEMPLATE2(defaulttype::Vec, 2, int)));

    OSG2_Int2Variable();
    virtual ~OSG2_Int2Variable(){ }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Int3Variable : public OSG2_Variable<defaulttype::Vec<3, int> >
{
  public:
    SOFA_CLASS(OSG2_Int3Variable, SOFA_TEMPLATE(OSG2_Variable, SOFA_TEMPLATE2(defaulttype::Vec, 3, int)));

    OSG2_Int3Variable();
    virtual ~OSG2_Int3Variable(){ }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Int4Variable : public OSG2_Variable<defaulttype::Vec<4, int> >
{
  public:
    SOFA_CLASS(OSG2_Int4Variable, SOFA_TEMPLATE(OSG2_Variable, SOFA_TEMPLATE2(defaulttype::Vec, 4, int)));

    OSG2_Int4Variable();
    virtual ~OSG2_Int4Variable(){ }

    void initVisual();
};

/** SINGLE FLOAT VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSG2_FloatVariable : public OSG2_Variable<float>
{
  public:
    SOFA_CLASS(OSG2_FloatVariable, OSG2_Variable<float>);

    OSG2_FloatVariable();
    virtual ~OSG2_FloatVariable() { }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Float2Variable : public OSG2_Variable<defaulttype::Vec2f>
{
  public:
    SOFA_CLASS(OSG2_Float2Variable, OSG2_Variable<defaulttype::Vec2f>);

    OSG2_Float2Variable();
    virtual ~OSG2_Float2Variable(){ }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Float3Variable : public OSG2_Variable<defaulttype::Vec3f>
{
  public:
    SOFA_CLASS(OSG2_Float3Variable, OSG2_Variable<defaulttype::Vec3f>);

    OSG2_Float3Variable();
    virtual ~OSG2_Float3Variable(){ }

    void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Float4Variable : public OSG2_Variable<defaulttype::Vec4f>
{
  public:
    SOFA_CLASS(OSG2_Float4Variable, OSG2_Variable<defaulttype::Vec4f>);

    OSG2_Float4Variable();
    virtual ~OSG2_Float4Variable(){ }

    void initVisual();
};

/** INT VECTOR VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSG2_IntVectorVariable : public OSG2_Variable<helper::vector<GLint> >
{
  public:
    SOFA_CLASS(OSG2_IntVectorVariable, OSG2_Variable<helper::vector<GLint> >);

    OSG2_IntVectorVariable();
    virtual ~OSG2_IntVectorVariable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_IntVector2Variable : public OSG2_IntVectorVariable
{

  public:
    SOFA_CLASS(OSG2_IntVector2Variable, OSG2_IntVectorVariable);

    OSG2_IntVector2Variable();
    virtual ~OSG2_IntVector2Variable(){ }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_IntVector3Variable : public OSG2_IntVectorVariable
{
  public:
    SOFA_CLASS(OSG2_IntVector3Variable, OSG2_IntVectorVariable);

    OSG2_IntVector3Variable();
    virtual ~OSG2_IntVector3Variable(){ }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_IntVector4Variable : public OSG2_IntVectorVariable
{
  public:
    SOFA_CLASS(OSG2_IntVector4Variable, OSG2_IntVectorVariable);

    OSG2_IntVector4Variable();
    virtual ~OSG2_IntVector4Variable(){ }

    virtual void init();
    virtual void initVisual();
};

/** FLOAT VECTOR VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSG2_FloatVectorVariable : public OSG2_Variable<helper::vector<float> >
{
  public:
    SOFA_CLASS(OSG2_FloatVectorVariable, OSG2_Variable<helper::vector<float> >);

    OSG2_FloatVectorVariable();
    virtual ~OSG2_FloatVectorVariable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_FloatVector2Variable : public OSG2_FloatVectorVariable
{
  public:
    SOFA_CLASS(OSG2_FloatVector2Variable,OSG2_FloatVectorVariable);

    OSG2_FloatVector2Variable();
    virtual ~OSG2_FloatVector2Variable(){ }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_FloatVector3Variable : public OSG2_FloatVectorVariable
{
  public:
    SOFA_CLASS(OSG2_FloatVector3Variable,OSG2_FloatVectorVariable);

    OSG2_FloatVector3Variable();
    virtual ~OSG2_FloatVector3Variable(){ }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_FloatVector4Variable : public OSG2_FloatVectorVariable
{
  public:
    SOFA_CLASS(OSG2_FloatVector4Variable,OSG2_FloatVectorVariable);

    OSG2_FloatVector4Variable();
    virtual ~OSG2_FloatVector4Variable(){ }

    virtual void init();
    virtual void initVisual();
};

/** Matrix VARIABLE **/
class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Matrix2Variable : public OSG2_Variable<helper::vector<float> >
{
  public:
    SOFA_CLASS(OSG2_Matrix2Variable,OSG2_Variable<helper::vector<float> >);

    Data<bool> transpose;

    OSG2_Matrix2Variable();
    virtual ~OSG2_Matrix2Variable() { }

    virtual void setTranspose( const bool& v ) { transpose.setValue(v); }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Matrix3Variable : public OSG2_Matrix2Variable
{
  public:
    SOFA_CLASS(OSG2_Matrix3Variable,OSG2_Matrix2Variable);

    OSG2_Matrix3Variable();
    virtual ~OSG2_Matrix3Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Matrix4Variable : public OSG2_Matrix2Variable
{
  public:
    SOFA_CLASS(OSG2_Matrix4Variable,OSG2_Matrix2Variable);

    OSG2_Matrix4Variable();
    virtual ~OSG2_Matrix4Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Matrix2x3Variable : public OSG2_Matrix2Variable
{
  public:
    SOFA_CLASS(OSG2_Matrix2x3Variable,OSG2_Matrix2Variable);

    OSG2_Matrix2x3Variable();
    virtual ~OSG2_Matrix2x3Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Matrix3x2Variable : public OSG2_Matrix2Variable
{
  public:
    SOFA_CLASS(OSG2_Matrix3x2Variable,OSG2_Matrix2Variable);

    OSG2_Matrix3x2Variable();
    virtual ~OSG2_Matrix3x2Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Matrix2x4Variable : public OSG2_Matrix2Variable
{
  public:
    SOFA_CLASS(OSG2_Matrix2x4Variable,OSG2_Matrix2Variable);

    OSG2_Matrix2x4Variable();
    virtual ~OSG2_Matrix2x4Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Matrix4x2Variable : public OSG2_Matrix2Variable
{
  public:
    SOFA_CLASS(OSG2_Matrix4x2Variable,OSG2_Matrix2Variable);

    OSG2_Matrix4x2Variable();
    virtual ~OSG2_Matrix4x2Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Matrix3x4Variable : public OSG2_Matrix2Variable
{
  public:
    SOFA_CLASS(OSG2_Matrix3x4Variable,OSG2_Matrix2Variable);

    OSG2_Matrix3x4Variable();
    virtual ~OSG2_Matrix3x4Variable() { }

    virtual void init();
    virtual void initVisual();
};

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Matrix4x3Variable : public OSG2_Matrix2Variable
{
  public:
    SOFA_CLASS(OSG2_Matrix4x3Variable,OSG2_Matrix2Variable);

    OSG2_Matrix4x3Variable();
    virtual ~OSG2_Matrix4x3Variable() { }

    virtual void init();
    virtual void initVisual();
};

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFAVARIABLE_H_
