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

#if __GNUC__ >= 4 || __GNUC_MINOR__ >=3
//#pragma GCC diagnostic warning "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include "OSGSofaVariable.inl"
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace visualmodel
{

/** SINGLE INT VARIABLE **/
SOFA_DECL_CLASS(OSG2_IntVariable)
SOFA_DECL_CLASS(OSG2_Int2Variable)
SOFA_DECL_CLASS(OSG2_Int3Variable)
SOFA_DECL_CLASS(OSG2_Int4Variable)

//Register OSG2_IntVariable in the Object Factory
int OSG2_IntVariableClass = core::RegisterObject("OSG2_IntVariable")
.add< OSG2_IntVariable >()
;
//Register OSG2_Int2Variable in the Object Factory
int OSG2_Int2VariableClass = core::RegisterObject("OSG2_Int2Variable")
.add< OSG2_Int2Variable >()
;
//Register OSG2_Int3Variable in the Object Factory
int OSG2_Int3VariableClass = core::RegisterObject("OSG2_Int3Variable")
.add< OSG2_Int3Variable >()
;
//Register OSG2_Int4Variable in the Object Factory
int OSG2_Int4VariableClass = core::RegisterObject("OSG2_Int4Variable")
.add< OSG2_Int4Variable >()
;

/** SINGLE FLOAT VARIABLE **/

SOFA_DECL_CLASS(OSG2_FloatVariable)
SOFA_DECL_CLASS(OSG2_Float2Variable)
SOFA_DECL_CLASS(OSG2_Float3Variable)
SOFA_DECL_CLASS(OSG2_Float4Variable)

//Register OSG2_FloatVariable in the Object Factory
int OSG2_FloatVariableClass = core::RegisterObject("OSG2_FloatVariable")
.add< OSG2_FloatVariable >()
;
//Register OSG2_Float2Variable in the Object Factory
int OSG2_Float2VariableClass = core::RegisterObject("OSG2_Float2Variable")
.add< OSG2_Float2Variable >()
;
//Register OSG2_Float3Variable in the Object Factory
int OSG2_Float3VariableClass = core::RegisterObject("OSG2_Float3Variable")
.add< OSG2_Float3Variable >()
;
//Register OSG2_Float4Variable in the Object Factory
int OSG2_Float4VariableClass = core::RegisterObject("OSG2_Float4Variable")
.add< OSG2_Float4Variable >()
;

/** INT VECTOR VARIABLE **/
SOFA_DECL_CLASS(OSG2_IntVectorVariable)
SOFA_DECL_CLASS(OSG2_IntVector2Variable)
SOFA_DECL_CLASS(OSG2_IntVector3Variable)
SOFA_DECL_CLASS(OSG2_IntVector4Variable)

//Register OSG2_IntVectorVariable in the Object Factory
int OSG2_IntVectorVariableClass = core::RegisterObject("OSG2_IntVectorVariable")
.add< OSG2_IntVectorVariable >()
;

//Register OSG2_IntVector2Variable in the Object Factory
int OSG2_IntVector2VariableClass = core::RegisterObject("OSG2_IntVector2Variable")
.add< OSG2_IntVector2Variable >()
;

//Register OSG2_IntVector3Variable in the Object Factory
int OSG2_IntVector3VariableClass = core::RegisterObject("OSG2_IntVector3Variable")
.add< OSG2_IntVector3Variable >()
;

//Register OSG2_IntVector4Variable in the Object Factory
int OSG2_IntVector4VariableClass = core::RegisterObject("OSG2_IntVector4Variable")
.add< OSG2_IntVector4Variable >()
;


/** FLOAT VECTOR VARIABLE **/
SOFA_DECL_CLASS(OSG2_FloatVectorVariable)
SOFA_DECL_CLASS(OSG2_FloatVector2Variable)
SOFA_DECL_CLASS(OSG2_FloatVector3Variable)
SOFA_DECL_CLASS(OSG2_FloatVector4Variable)

//Register OSG2_FloatVectorVariable in the Object Factory
int OSG2_FloatVectorVariableClass = core::RegisterObject("OSG2_FloatVectorVariable")
.add< OSG2_FloatVectorVariable >()
;

//Register OSG2_FloatVector2Variable in the Object Factory
int OSG2_FloatVector2VariableClass = core::RegisterObject("OSG2_FloatVector2Variable")
.add< OSG2_FloatVector2Variable >()
;

//Register OSG2_FloatVector3Variable in the Object Factory
int OSG2_FloatVector3VariableClass = core::RegisterObject("OSG2_FloatVector3Variable")
.add< OSG2_FloatVector3Variable >()
;
//Register OSG2_FloatVector4Variable in the Object Factory
int OSG2_FloatVector4VariableClass = core::RegisterObject("OSG2_FloatVector4Variable")
.add< OSG2_FloatVector4Variable >()
;

/** FLOAT VECTOR VARIABLE **/
SOFA_DECL_CLASS(OSG2_Matrix2Variable)
SOFA_DECL_CLASS(OSG2_Matrix3Variable)
SOFA_DECL_CLASS(OSG2_Matrix4Variable)
SOFA_DECL_CLASS(OSG2_Matrix2x3Variable)
SOFA_DECL_CLASS(OSG2_Matrix3x2Variable)
SOFA_DECL_CLASS(OSG2_Matrix2x4Variable)
SOFA_DECL_CLASS(OSG2_Matrix4x2Variable)
SOFA_DECL_CLASS(OSG2_Matrix3x4Variable)
SOFA_DECL_CLASS(OSG2_Matrix4x3Variable)

//Register OSG2_Matrix2Variable in the Object Factory
int OSG2_Matrix2VariableClass = core::RegisterObject("OSG2_Matrix2Variable")
.add< OSG2_Matrix2Variable >()
;

//Register OSG2_Matrix3Variable in the Object Factory
int OSG2_Matrix3VariableClass = core::RegisterObject("OSG2_Matrix3Variable")
.add< OSG2_Matrix3Variable >()
;

//Register OSG2_Matrix4Variable in the Object Factory
int OSG2_Matrix4VariableClass = core::RegisterObject("OSG2_Matrix4Variable")
.add< OSG2_Matrix4Variable >()
;

//Register OSG2_Matrix2x3Variable in the Object Factory
int OSG2_Matrix2x3VariableClass = core::RegisterObject("OSG2_Matrix2x3Variable")
.add< OSG2_Matrix2x3Variable >()
;

//Register OSG2_Matrix3x2Variable in the Object Factory
int OSG2_Matrix3x2VariableClass = core::RegisterObject("OSG2_Matrix3x2Variable")
.add< OSG2_Matrix3x2Variable >()
;

//Register OSG2_Matrix2x4Variable in the Object Factory
int OSG2_Matrix2x4VariableClass = core::RegisterObject("OSG2_Matrix2x4Variable")
.add< OSG2_Matrix2x4Variable >()
;

//Register OSG2_Matrix4x2Variable in the Object Factory
int OSG2_Matrix4x2VariableClass = core::RegisterObject("OSG2_Matrix4x2Variable")
.add< OSG2_Matrix4x2Variable >()
;

//Register OSG2_Matrix2x4Variable in the Object Factory
int OSG2_Matrix3x4VariableClass = core::RegisterObject("OSG2_Matrix3x4Variable")
.add< OSG2_Matrix3x4Variable >()
;

//Register OSG2_Matrix4x3Variable in the Object Factory
int OSG2_Matrix4x3VariableClass = core::RegisterObject("OSG2_Matrix4x3Variable")
.add< OSG2_Matrix4x3Variable >()
;


OSG2_IntVariable::OSG2_IntVariable()
{

}

OSG2_Int2Variable::OSG2_Int2Variable()
{

}

OSG2_Int3Variable::OSG2_Int3Variable()
{

}

OSG2_Int4Variable::OSG2_Int4Variable()
{

}

void OSG2_IntVariable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;
    varChunk->addUniformVariable( id.getValue().c_str(), OSG::Int32(value.getValue()) );

    OSG::Int32 osgvalue;
    varChunk->getUniformVariable(id.getValue().c_str(), osgvalue);

std::cerr << "OSG2_IntVariable::initVisual" << std::endl;
std::cerr << "OSG2_IntVariable ::SOFA" << " " << indexShader.getValue() << " " << id.getValue().c_str() << " " << value.getValue() << std::endl;
std::cerr << "OSG2_IntVariable ::OPENSG" << " " << indexShader.getValue() << " " <<  id.getValue().c_str() << " " << osgvalue << std::endl;
std::cerr << "Shader : " << shader << std::endl;
std::cerr << "==========================" << std::endl;

}


void OSG2_Int2Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFInt32 int2;
    int2.resize(2);
    int2[0] =  value.getValue()[0];
    int2[1] =  value.getValue()[1];

    varChunk->addUniformVariable( id.getValue().c_str()
    //, OSG::MFInt32 < value.getValue()[0], value.getValue()[1] >) ;
    , int2 ) ;
}

void OSG2_Int3Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFInt32 int3;
    int3.resize(3);
    int3[0] =  value.getValue()[0];
    int3[1] =  value.getValue()[1];
    int3[2] =  value.getValue()[2];

    varChunk->addUniformVariable( id.getValue().c_str()
    , int3 ) ;
}

void OSG2_Int4Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFInt32 int4;
    int4.resize(4);
    int4[0] =  value.getValue()[0];
    int4[1] =  value.getValue()[1];
    int4[2] =  value.getValue()[2];
    int4[3] =  value.getValue()[3];

    varChunk->addUniformVariable( id.getValue().c_str()
    , int4 ) ;
}


OSG2_FloatVariable::OSG2_FloatVariable()
{

}

OSG2_Float2Variable::OSG2_Float2Variable()
{

}

OSG2_Float3Variable::OSG2_Float3Variable()
{

}

OSG2_Float4Variable::OSG2_Float4Variable()
{

}

void OSG2_FloatVariable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;
    varChunk->addUniformVariable( id.getValue().c_str(), OSG::Real32(value.getValue()) );

    std::string osgid;
    OSG::Real32 osgvalue;
    varChunk->getUniformVariable(id.getValue().c_str(), osgvalue);

std::cerr << "OSG2_FloatVariable::initVisual" << std::endl;
std::cerr << "OSG2_FloatVariable ::SOFA" << " " << indexShader.getValue() << " " << id.getValue().c_str() << " " << value.getValue() << std::endl;
std::cerr << "OSG2_FloatVariable ::OPENSG" << " " << indexShader.getValue() << " " <<  id.getValue().c_str() << " " << osgvalue << std::endl;
std::cerr << "Shader : " << shader << std::endl;
std::cerr << "==========================" << std::endl;

    //shader->setFloat(indexShader.getValue(), id.getValue().c_str(), value.getValue());
}


void OSG2_Float2Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    
    varChunk->addUniformVariable( id.getValue().c_str()
    , OSG::Vec2f( value.getValue()[0],  value.getValue()[1]) ) ;
    

}

void OSG2_Float3Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFReal32 float3;
    float3.resize(3);
    float3[0] =  value.getValue()[0];
    float3[1] =  value.getValue()[1];
    float3[2] =  value.getValue()[2];


    varChunk->addUniformVariable( id.getValue().c_str(), 
    OSG::Vec3f(value.getValue()[0], value.getValue()[1], value.getValue()[2], value.getValue()[3]) );

    OSG::Vec3f osgvalue;
    varChunk->getUniformVariable(id.getValue().c_str(), osgvalue);

std::cerr << "OSG2_Float3_Variable::initVisual" << std::endl;
std::cerr << "OSG2_Float3_Variable ::OPENSG" << " " << indexShader.getValue() << " " <<  id.getValue().c_str() << " " << osgvalue << std::endl;
std::cerr << "Shader : " << shader << std::endl;
std::cerr << "==========================" << std::endl;
}

void OSG2_Float4Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;


    varChunk->addUniformVariable( id.getValue().c_str(), 
    OSG::Vec4f(value.getValue()[0], value.getValue()[1], value.getValue()[2], value.getValue()[3]) );

    //system("pause");
}


OSG2_IntVectorVariable::OSG2_IntVectorVariable()
{

}

OSG2_IntVector2Variable::OSG2_IntVector2Variable()
{

}

OSG2_IntVector3Variable::OSG2_IntVector3Variable()
{

}

OSG2_IntVector4Variable::OSG2_IntVector4Variable()
{

}


void OSG2_IntVectorVariable::init()
{
    OSG2_Variable<helper::vector<GLint> >::init();
}

void OSG2_IntVector2Variable::init()
{
    OSG2_IntVectorVariable::init();
    helper::vector<GLint> temp = value.getValue();
    if (value.getValue().size() %2 != 0)
    {
        serr << "The number of values is not even ; padding with one zero" << sendl;
        temp.push_back(0);
        value.setValue(temp);
    }
}

void OSG2_IntVector3Variable::init()
{
    OSG2_IntVectorVariable::init();
    helper::vector<GLint> temp = value.getValue();
    if (value.getValue().size() %3 != 0)
    {
        serr << "The number of values is not a multiple of 3 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %3 != 0) temp.push_back(0);
        value.setValue(temp);
    }
}

void OSG2_IntVector4Variable::init()
{
    OSG2_IntVectorVariable::init();
    helper::vector<GLint> temp = value.getValue();

    if (value.getValue().size() %4 != 0)
    {
        serr << "The number of values is not a multiple of 4 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %4 != 0) temp.push_back(0);
        value.setValue(temp);
    }
}

void OSG2_IntVectorVariable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    varChunk->addUniformVariable( id.getValue().c_str(), OSG::Int32(value.getValue()[0]) );

    //shader->setIntVector(indexShader.getValue(), id.getValue().c_str(), value.getValue().size(), &(value.getValue()[0]));
}

void OSG2_IntVector2Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFInt32 int2;
    int2.resize(2);
    int2[0] =  value.getValue()[0];
    int2[1] =  value.getValue()[1];

    varChunk->addUniformVariable( id.getValue().c_str()
    //, OSG::MFInt32 < value.getValue()[0], value.getValue()[1] >) ;
    , int2 ) ;
    
    //shader->setIntVector2(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/2, &(value.getValue()[0]));
}

void OSG2_IntVector3Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFInt32 int3;
    int3.resize(3);
    int3[0] =  value.getValue()[0];
    int3[1] =  value.getValue()[1];
    int3[2] =  value.getValue()[2];

    varChunk->addUniformVariable( id.getValue().c_str()
    , int3 ) ;

    //shader->setIntVector3(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/3, &(value.getValue()[0]));
}

void OSG2_IntVector4Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFInt32 int4;
    int4.resize(4);
    int4[0] =  value.getValue()[0];
    int4[1] =  value.getValue()[1];
    int4[2] =  value.getValue()[2];
    int4[3] =  value.getValue()[3];

    varChunk->addUniformVariable( id.getValue().c_str()
    , int4 ) ;

    //shader->setIntVector4(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/4, &(value.getValue()[0]));
}


OSG2_FloatVectorVariable::OSG2_FloatVectorVariable()
{

}

OSG2_FloatVector2Variable::OSG2_FloatVector2Variable()
{

}

OSG2_FloatVector3Variable::OSG2_FloatVector3Variable()
{

}

OSG2_FloatVector4Variable::OSG2_FloatVector4Variable()
{

}


void OSG2_FloatVectorVariable::init()
{
    OSG2_Variable<helper::vector<float> >::init();
}

void OSG2_FloatVector2Variable::init()
{
    OSG2_FloatVectorVariable::init();
    helper::vector<float> temp = value.getValue();
    if (value.getValue().size() %2 != 0)
    {
        serr << "The number of values is not even ; padding with one zero" << sendl;
        temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSG2_FloatVector3Variable::init()
{
    OSG2_FloatVectorVariable::init();
    helper::vector<float> temp = value.getValue();

    if (value.getValue().size() %3 != 0)
    {
        serr << "The number of values is not a multiple of 3 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %3 != 0) temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSG2_FloatVector4Variable::init()
{
    OSG2_FloatVectorVariable::init();
    helper::vector<float> temp = value.getValue();
    std::cerr << this->getName() << " " << value.getValue().size() << std::endl;
    if (value.getValue().size() %4 != 0)
    {
        serr << "The number of values is not a multiple of 4 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %4 != 0) temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSG2_FloatVectorVariable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;
    varChunk->addUniformVariable( id.getValue().c_str(), 
    OSG::Real32(value.getValue()[0]) );

    //shader->setFloatVector(indexShader.getValue(), id.getValue().c_str(), value.getValue().size(), &(value.getValue()[0]));
}

void OSG2_FloatVector2Variable::initVisual()
{
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;
    varChunk->addUniformVariable( id.getValue().c_str(), 
    OSG::Vec2f(value.getValue()[0],value.getValue()[1] ) );

    //shader->setFloatVector2(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/2, &(value.getValue()[0]));
}

void OSG2_FloatVector3Variable::initVisual()
{
    std::cerr << "IN OSG2_FloatVector3Variable::initVisual()" << std::endl;
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFVec3f vectormap;
    vectormap.resize(value.getValue().size() / 3);
    helper::vector<float> temp = value.getValue();
    OSG::UInt32 count = 0;
    for (OSG::UInt32 i=0; i < vectormap.size() ; ++i, ++count)
    {
        vectormap[i][0] = temp[count];
        vectormap[i][1] = temp[++count];
        vectormap[i][2] = temp[++count];
    }

    varChunk->addUniformVariable( id.getValue().c_str(), 
    vectormap);

    std::cerr << "OUT OSG2_FloatVector3Variable::initVisual()" << std::endl;

    //shader->setFloatVector3(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/3, &(value.getValue()[0]));
}

void OSG2_FloatVector4Variable::initVisual()
{
    std::cerr << "IN OSG2_FloatVector4Variable::initVisual() " << id.getValue().c_str() << std::endl;
    OSG::ShaderProgramVariableChunkRecPtr varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFVec4f vectormap;
    vectormap.resize(value.getValue().size() / 4);
    helper::vector<float> temp = value.getValue();
    OSG::UInt32 count = 0;
    for (OSG::UInt32 i=0; i < vectormap.size() ; i++, ++count)
    {
        vectormap[i][0] = temp[count];
        vectormap[i][1] = temp[++count];
        vectormap[i][2] = temp[++count];
        vectormap[i][3] = temp[++count];
    }

    //OSG::MFReal32 allfloats;
    //allfloats.resize(value.getValue().size());
    //for (OSG::UInt32 i=0; i < allfloats.size() ; ++i) allfloats[i] = temp[i];



    varChunk->addUniformVariable( id.getValue().c_str(), 
    //allfloats);
    vectormap);

    std::cerr << "OUT OSG2_FloatVector4Variable::initVisual()" << std::endl;
}


//// Matrix /////
OSG2_Matrix2Variable::OSG2_Matrix2Variable()
{

}

void OSG2_Matrix2Variable::init()
{
     OSG2_Variable<helper::vector<float> >::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %4 != 0)
     {
        serr << "The number of values is not a multiple of 4 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %4 != 0) temp.push_back(0.0);
        value.setValue(temp);
     }
}

void OSG2_Matrix2Variable::initVisual()
{
    shader->setMatrix2(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/4, transpose.getValue(), &(value.getValue()[0]));
}

OSG2_Matrix3Variable::OSG2_Matrix3Variable()
{

}

void OSG2_Matrix3Variable::init()
{
    OSG2_Matrix2Variable::init();

    helper::vector<float> temp = value.getValue();

    if (value.getValue().size() %9 != 0)
    {
        serr << "The number of values is not a multiple of 9 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %9 != 0) temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSG2_Matrix3Variable::initVisual()
{
    shader->setMatrix3(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/9, transpose.getValue(), &(value.getValue()[0]));
}

OSG2_Matrix4Variable::OSG2_Matrix4Variable()
{

}

void OSG2_Matrix4Variable::init()
{
    OSG2_Matrix2Variable::init();

    helper::vector<float> temp = value.getValue();

    if (value.getValue().size() %16 != 0)
    {
        serr << "The number of values is not a multiple of 16 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %16 != 0) temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSG2_Matrix4Variable::initVisual()
{
    shader->setMatrix4(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/16, transpose.getValue(), &(value.getValue()[0]));
}

OSG2_Matrix2x3Variable::OSG2_Matrix2x3Variable()
{

}

void OSG2_Matrix2x3Variable::init()
{
    OSG2_Matrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %6 != 0)
     {
             serr << "The number of values is not a multiple of 6 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %6 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSG2_Matrix2x3Variable::initVisual()
{
    shader->setMatrix2x3(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/6, transpose.getValue(), &(value.getValue()[0]));
}


OSG2_Matrix3x2Variable::OSG2_Matrix3x2Variable()
{

}

void OSG2_Matrix3x2Variable::init()
{
    OSG2_Matrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %6 != 0)
     {
             serr << "The number of values is not a multiple of 6 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %6 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSG2_Matrix3x2Variable::initVisual()
{
    shader->setMatrix3x2(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/6, transpose.getValue(), &(value.getValue()[0]));
}

OSG2_Matrix2x4Variable::OSG2_Matrix2x4Variable()
{

}

void OSG2_Matrix2x4Variable::init()
{
    OSG2_Matrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %8 != 0)
     {
             serr << "The number of values is not a multiple of 8 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %8 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSG2_Matrix2x4Variable::initVisual()
{
    shader->setMatrix2x4(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/8, transpose.getValue(), &(value.getValue()[0]));
}

OSG2_Matrix4x2Variable::OSG2_Matrix4x2Variable()
{

}

void OSG2_Matrix4x2Variable::init()
{
    OSG2_Matrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %8 != 0)
     {
             serr << "The number of values is not a multiple of 8 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %8 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSG2_Matrix4x2Variable::initVisual()
{
    //shader->setMatrix4x2(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/8, transpose.getValue(), &(value.getValue()[0]));
}

OSG2_Matrix3x4Variable::OSG2_Matrix3x4Variable()
{

}

void OSG2_Matrix3x4Variable::init()
{
    OSG2_Matrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %12 != 0)
     {
             serr << "The number of values is not a multiple of 12 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %12 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSG2_Matrix3x4Variable::initVisual()
{
    //shader->setMatrix3x4(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/12, transpose.getValue(), &(value.getValue()[0]));
}

OSG2_Matrix4x3Variable::OSG2_Matrix4x3Variable()
{

}

void OSG2_Matrix4x3Variable::init()
{
    OSG2_Matrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %12 != 0)
     {
             serr << "The number of values is not a multiple of 12 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %12 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSG2_Matrix4x3Variable::initVisual()
{
    //shader->setMatrix4x3(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/12, transpose.getValue(), &(value.getValue()[0]));
}

} // namespace visual

} // namespace component

} // namespace sofa
