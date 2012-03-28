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
SOFA_DECL_CLASS(OSGIntVariable)
SOFA_DECL_CLASS(OSGInt2Variable)
SOFA_DECL_CLASS(OSGInt3Variable)
SOFA_DECL_CLASS(OSGInt4Variable)

//Register OSGIntVariable in the Object Factory
int OSGIntVariableClass = core::RegisterObject("OSGIntVariable")
.add< OSGIntVariable >()
;
//Register OSGInt2Variable in the Object Factory
int OSGInt2VariableClass = core::RegisterObject("OSGInt2Variable")
.add< OSGInt2Variable >()
;
//Register OSGInt3Variable in the Object Factory
int OSGInt3VariableClass = core::RegisterObject("OSGInt3Variable")
.add< OSGInt3Variable >()
;
//Register OSGInt4Variable in the Object Factory
int OSGInt4VariableClass = core::RegisterObject("OSGInt4Variable")
.add< OSGInt4Variable >()
;

/** SINGLE FLOAT VARIABLE **/

SOFA_DECL_CLASS(OSGFloatVariable)
SOFA_DECL_CLASS(OSGFloat2Variable)
SOFA_DECL_CLASS(OSGFloat3Variable)
SOFA_DECL_CLASS(OSGFloat4Variable)

//Register OSGFloatVariable in the Object Factory
int OSGFloatVariableClass = core::RegisterObject("OSGFloatVariable")
.add< OSGFloatVariable >()
;
//Register OSGFloat2Variable in the Object Factory
int OSGFloat2VariableClass = core::RegisterObject("OSGFloat2Variable")
.add< OSGFloat2Variable >()
;
//Register OSGFloat3Variable in the Object Factory
int OSGFloat3VariableClass = core::RegisterObject("OSGFloat3Variable")
.add< OSGFloat3Variable >()
;
//Register OSGFloat4Variable in the Object Factory
int OSGFloat4VariableClass = core::RegisterObject("OSGFloat4Variable")
.add< OSGFloat4Variable >()
;

/** INT VECTOR VARIABLE **/
SOFA_DECL_CLASS(OSGIntVectorVariable)
SOFA_DECL_CLASS(OSGIntVector2Variable)
SOFA_DECL_CLASS(OSGIntVector3Variable)
SOFA_DECL_CLASS(OSGIntVector4Variable)

//Register OSGIntVectorVariable in the Object Factory
int OSGIntVectorVariableClass = core::RegisterObject("OSGIntVectorVariable")
.add< OSGIntVectorVariable >()
;

//Register OSGIntVector2Variable in the Object Factory
int OSGIntVector2VariableClass = core::RegisterObject("OSGIntVector2Variable")
.add< OSGIntVector2Variable >()
;

//Register OSGIntVector3Variable in the Object Factory
int OSGIntVector3VariableClass = core::RegisterObject("OSGIntVector3Variable")
.add< OSGIntVector3Variable >()
;

//Register OSGIntVector4Variable in the Object Factory
int OSGIntVector4VariableClass = core::RegisterObject("OSGIntVector4Variable")
.add< OSGIntVector4Variable >()
;


/** FLOAT VECTOR VARIABLE **/
SOFA_DECL_CLASS(OSGFloatVectorVariable)
SOFA_DECL_CLASS(OSGFloatVector2Variable)
SOFA_DECL_CLASS(OSGFloatVector3Variable)
SOFA_DECL_CLASS(OSGFloatVector4Variable)

//Register OSGFloatVectorVariable in the Object Factory
int OSGFloatVectorVariableClass = core::RegisterObject("OSGFloatVectorVariable")
.add< OSGFloatVectorVariable >()
;

//Register OSGFloatVector2Variable in the Object Factory
int OSGFloatVector2VariableClass = core::RegisterObject("OSGFloatVector2Variable")
.add< OSGFloatVector2Variable >()
;

//Register OSGFloatVector3Variable in the Object Factory
int OSGFloatVector3VariableClass = core::RegisterObject("OSGFloatVector3Variable")
.add< OSGFloatVector3Variable >()
;
//Register OSGFloatVector4Variable in the Object Factory
int OSGFloatVector4VariableClass = core::RegisterObject("OSGFloatVector4Variable")
.add< OSGFloatVector4Variable >()
;

/** FLOAT VECTOR VARIABLE **/
SOFA_DECL_CLASS(OSGMatrix2Variable)
SOFA_DECL_CLASS(OSGMatrix3Variable)
SOFA_DECL_CLASS(OSGMatrix4Variable)
SOFA_DECL_CLASS(OSGMatrix2x3Variable)
SOFA_DECL_CLASS(OSGMatrix3x2Variable)
SOFA_DECL_CLASS(OSGMatrix2x4Variable)
SOFA_DECL_CLASS(OSGMatrix4x2Variable)
SOFA_DECL_CLASS(OSGMatrix3x4Variable)
SOFA_DECL_CLASS(OSGMatrix4x3Variable)

//Register OSGMatrix2Variable in the Object Factory
int OSGMatrix2VariableClass = core::RegisterObject("OSGMatrix2Variable")
.add< OSGMatrix2Variable >()
;

//Register OSGMatrix3Variable in the Object Factory
int OSGMatrix3VariableClass = core::RegisterObject("OSGMatrix3Variable")
.add< OSGMatrix3Variable >()
;

//Register OSGMatrix4Variable in the Object Factory
int OSGMatrix4VariableClass = core::RegisterObject("OSGMatrix4Variable")
.add< OSGMatrix4Variable >()
;

//Register OSGMatrix2x3Variable in the Object Factory
int OSGMatrix2x3VariableClass = core::RegisterObject("OSGMatrix2x3Variable")
.add< OSGMatrix2x3Variable >()
;

//Register OSGMatrix3x2Variable in the Object Factory
int OSGMatrix3x2VariableClass = core::RegisterObject("OSGMatrix3x2Variable")
.add< OSGMatrix3x2Variable >()
;

//Register OSGMatrix2x4Variable in the Object Factory
int OSGMatrix2x4VariableClass = core::RegisterObject("OSGMatrix2x4Variable")
.add< OSGMatrix2x4Variable >()
;

//Register OSGMatrix4x2Variable in the Object Factory
int OSGMatrix4x2VariableClass = core::RegisterObject("OSGMatrix4x2Variable")
.add< OSGMatrix4x2Variable >()
;

//Register OSGMatrix2x4Variable in the Object Factory
int OSGMatrix3x4VariableClass = core::RegisterObject("OSGMatrix3x4Variable")
.add< OSGMatrix3x4Variable >()
;

//Register OSGMatrix4x3Variable in the Object Factory
int OSGMatrix4x3VariableClass = core::RegisterObject("OSGMatrix4x3Variable")
.add< OSGMatrix4x3Variable >()
;


OSGIntVariable::OSGIntVariable()
{

}

OSGInt2Variable::OSGInt2Variable()
{

}

OSGInt3Variable::OSGInt3Variable()
{

}

OSGInt4Variable::OSGInt4Variable()
{

}

void OSGIntVariable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;
    varChunk->addUniformVariable( id.getValue().c_str(), OSG::Int32(value.getValue()) );

    OSG::Int32 osgvalue;
    varChunk->getUniformVariable(id.getValue().c_str(), osgvalue);

std::cerr << "OSGIntVariable::initVisual" << std::endl;
std::cerr << "OSGIntVariable ::SOFA" << " " << indexShader.getValue() << " " << id.getValue().c_str() << " " << value.getValue() << std::endl;
std::cerr << "OSGIntVariable ::OPENSG" << " " << indexShader.getValue() << " " <<  id.getValue().c_str() << " " << osgvalue << std::endl;
std::cerr << "Shader : " << shader << std::endl;
std::cerr << "==========================" << std::endl;

}


void OSGInt2Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFInt32 int2;
    int2.resize(2);
    int2[0] =  value.getValue()[0];
    int2[1] =  value.getValue()[1];

    varChunk->addUniformVariable( id.getValue().c_str()
    //, OSG::MFInt32 < value.getValue()[0], value.getValue()[1] >) ;
    , int2 ) ;
}

void OSGInt3Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    OSG::MFInt32 int3;
    int3.resize(3);
    int3[0] =  value.getValue()[0];
    int3[1] =  value.getValue()[1];
    int3[2] =  value.getValue()[2];

    varChunk->addUniformVariable( id.getValue().c_str()
    , int3 ) ;
}

void OSGInt4Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
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


OSGFloatVariable::OSGFloatVariable()
{

}

OSGFloat2Variable::OSGFloat2Variable()
{

}

OSGFloat3Variable::OSGFloat3Variable()
{

}

OSGFloat4Variable::OSGFloat4Variable()
{

}

void OSGFloatVariable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;
    varChunk->addUniformVariable( id.getValue().c_str(), OSG::Real32(value.getValue()) );

    std::string osgid;
    OSG::Real32 osgvalue;
    varChunk->getUniformVariable(id.getValue().c_str(), osgvalue);

std::cerr << "OSGFloatVariable::initVisual" << std::endl;
std::cerr << "OSGFloatVariable ::SOFA" << " " << indexShader.getValue() << " " << id.getValue().c_str() << " " << value.getValue() << std::endl;
std::cerr << "OSGFloatVariable ::OPENSG" << " " << indexShader.getValue() << " " <<  id.getValue().c_str() << " " << osgvalue << std::endl;
std::cerr << "Shader : " << shader << std::endl;
std::cerr << "==========================" << std::endl;

    //shader->setFloat(indexShader.getValue(), id.getValue().c_str(), value.getValue());
}


void OSGFloat2Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    
    varChunk->addUniformVariable( id.getValue().c_str()
    , OSG::Vec2f( value.getValue()[0],  value.getValue()[1]) ) ;
    

}

void OSGFloat3Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
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

std::cerr << "OSGFloat3_Variable::initVisual" << std::endl;
std::cerr << "OSGFloat3_Variable ::OPENSG" << " " << indexShader.getValue() << " " <<  id.getValue().c_str() << " " << osgvalue << std::endl;
std::cerr << "Shader : " << shader << std::endl;
std::cerr << "==========================" << std::endl;
}

void OSGFloat4Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;


    varChunk->addUniformVariable( id.getValue().c_str(), 
    OSG::Vec4f(value.getValue()[0], value.getValue()[1], value.getValue()[2], value.getValue()[3]) );

    //system("pause");
}


OSGIntVectorVariable::OSGIntVectorVariable()
{

}

OSGIntVector2Variable::OSGIntVector2Variable()
{

}

OSGIntVector3Variable::OSGIntVector3Variable()
{

}

OSGIntVector4Variable::OSGIntVector4Variable()
{

}


void OSGIntVectorVariable::init()
{
    OSGVariable<helper::vector<GLint> >::init();
}

void OSGIntVector2Variable::init()
{
    OSGIntVectorVariable::init();
    helper::vector<GLint> temp = value.getValue();
    if (value.getValue().size() %2 != 0)
    {
        serr << "The number of values is not even ; padding with one zero" << sendl;
        temp.push_back(0);
        value.setValue(temp);
    }
}

void OSGIntVector3Variable::init()
{
    OSGIntVectorVariable::init();
    helper::vector<GLint> temp = value.getValue();
    if (value.getValue().size() %3 != 0)
    {
        serr << "The number of values is not a multiple of 3 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %3 != 0) temp.push_back(0);
        value.setValue(temp);
    }
}

void OSGIntVector4Variable::init()
{
    OSGIntVectorVariable::init();
    helper::vector<GLint> temp = value.getValue();

    if (value.getValue().size() %4 != 0)
    {
        serr << "The number of values is not a multiple of 4 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %4 != 0) temp.push_back(0);
        value.setValue(temp);
    }
}

void OSGIntVectorVariable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;

    varChunk->addUniformVariable( id.getValue().c_str(), OSG::Int32(value.getValue()[0]) );

    //shader->setIntVector(indexShader.getValue(), id.getValue().c_str(), value.getValue().size(), &(value.getValue()[0]));
}

void OSGIntVector2Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
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

void OSGIntVector3Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
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

void OSGIntVector4Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
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


OSGFloatVectorVariable::OSGFloatVectorVariable()
{

}

OSGFloatVector2Variable::OSGFloatVector2Variable()
{

}

OSGFloatVector3Variable::OSGFloatVector3Variable()
{

}

OSGFloatVector4Variable::OSGFloatVector4Variable()
{

}


void OSGFloatVectorVariable::init()
{
    OSGVariable<helper::vector<float> >::init();
}

void OSGFloatVector2Variable::init()
{
    OSGFloatVectorVariable::init();
    helper::vector<float> temp = value.getValue();
    if (value.getValue().size() %2 != 0)
    {
        serr << "The number of values is not even ; padding with one zero" << sendl;
        temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSGFloatVector3Variable::init()
{
    OSGFloatVectorVariable::init();
    helper::vector<float> temp = value.getValue();

    if (value.getValue().size() %3 != 0)
    {
        serr << "The number of values is not a multiple of 3 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %3 != 0) temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSGFloatVector4Variable::init()
{
    OSGFloatVectorVariable::init();
    helper::vector<float> temp = value.getValue();
    std::cerr << this->getName() << " " << value.getValue().size() << std::endl;
    if (value.getValue().size() %4 != 0)
    {
        serr << "The number of values is not a multiple of 4 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %4 != 0) temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSGFloatVectorVariable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;
    varChunk->addUniformVariable( id.getValue().c_str(), 
    OSG::Real32(value.getValue()[0]) );

    //shader->setFloatVector(indexShader.getValue(), id.getValue().c_str(), value.getValue().size(), &(value.getValue()[0]));
}

void OSGFloatVector2Variable::initVisual()
{
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
    if (!varChunk) return;
    varChunk->addUniformVariable( id.getValue().c_str(), 
    OSG::Vec2f(value.getValue()[0],value.getValue()[1] ) );

    //shader->setFloatVector2(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/2, &(value.getValue()[0]));
}

void OSGFloatVector3Variable::initVisual()
{
    std::cerr << "IN OSGFloatVector3Variable::initVisual()" << std::endl;
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
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

    std::cerr << "OUT OSGFloatVector3Variable::initVisual()" << std::endl;

    //shader->setFloatVector3(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/3, &(value.getValue()[0]));
}

void OSGFloatVector4Variable::initVisual()
{
    std::cerr << "IN OSGFloatVector4Variable::initVisual() " << id.getValue().c_str() << std::endl;
    OSG::ShaderProgramVariableChunk* varChunk = shader->getShaderVariableChunk(indexShader.getValue());
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

    std::cerr << "OUT OSGFloatVector4Variable::initVisual()" << std::endl;
}


//// Matrix /////
OSGMatrix2Variable::OSGMatrix2Variable()
{

}

void OSGMatrix2Variable::init()
{
     OSGVariable<helper::vector<float> >::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %4 != 0)
     {
        serr << "The number of values is not a multiple of 4 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %4 != 0) temp.push_back(0.0);
        value.setValue(temp);
     }
}

void OSGMatrix2Variable::initVisual()
{
    shader->setMatrix2(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/4, transpose.getValue(), &(value.getValue()[0]));
}

OSGMatrix3Variable::OSGMatrix3Variable()
{

}

void OSGMatrix3Variable::init()
{
    OSGMatrix2Variable::init();

    helper::vector<float> temp = value.getValue();

    if (value.getValue().size() %9 != 0)
    {
        serr << "The number of values is not a multiple of 9 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %9 != 0) temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSGMatrix3Variable::initVisual()
{
    shader->setMatrix3(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/9, transpose.getValue(), &(value.getValue()[0]));
}

OSGMatrix4Variable::OSGMatrix4Variable()
{

}

void OSGMatrix4Variable::init()
{
    OSGMatrix2Variable::init();

    helper::vector<float> temp = value.getValue();

    if (value.getValue().size() %16 != 0)
    {
        serr << "The number of values is not a multiple of 16 ; padding with zero(s)" << sendl;
        while (value.getValue().size() %16 != 0) temp.push_back(0.0);
        value.setValue(temp);
    }
}

void OSGMatrix4Variable::initVisual()
{
    shader->setMatrix4(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/16, transpose.getValue(), &(value.getValue()[0]));
}

OSGMatrix2x3Variable::OSGMatrix2x3Variable()
{

}

void OSGMatrix2x3Variable::init()
{
    OSGMatrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %6 != 0)
     {
             serr << "The number of values is not a multiple of 6 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %6 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSGMatrix2x3Variable::initVisual()
{
    shader->setMatrix2x3(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/6, transpose.getValue(), &(value.getValue()[0]));
}


OSGMatrix3x2Variable::OSGMatrix3x2Variable()
{

}

void OSGMatrix3x2Variable::init()
{
    OSGMatrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %6 != 0)
     {
             serr << "The number of values is not a multiple of 6 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %6 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSGMatrix3x2Variable::initVisual()
{
    shader->setMatrix3x2(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/6, transpose.getValue(), &(value.getValue()[0]));
}

OSGMatrix2x4Variable::OSGMatrix2x4Variable()
{

}

void OSGMatrix2x4Variable::init()
{
    OSGMatrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %8 != 0)
     {
             serr << "The number of values is not a multiple of 8 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %8 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSGMatrix2x4Variable::initVisual()
{
    shader->setMatrix2x4(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/8, transpose.getValue(), &(value.getValue()[0]));
}

OSGMatrix4x2Variable::OSGMatrix4x2Variable()
{

}

void OSGMatrix4x2Variable::init()
{
    OSGMatrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %8 != 0)
     {
             serr << "The number of values is not a multiple of 8 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %8 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSGMatrix4x2Variable::initVisual()
{
    //shader->setMatrix4x2(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/8, transpose.getValue(), &(value.getValue()[0]));
}

OSGMatrix3x4Variable::OSGMatrix3x4Variable()
{

}

void OSGMatrix3x4Variable::init()
{
    OSGMatrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %12 != 0)
     {
             serr << "The number of values is not a multiple of 12 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %12 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSGMatrix3x4Variable::initVisual()
{
    //shader->setMatrix3x4(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/12, transpose.getValue(), &(value.getValue()[0]));
}

OSGMatrix4x3Variable::OSGMatrix4x3Variable()
{

}

void OSGMatrix4x3Variable::init()
{
    OSGMatrix2Variable::init();

     helper::vector<float> temp = value.getValue();

     if (value.getValue().size() %12 != 0)
     {
             serr << "The number of values is not a multiple of 12 ; padding with zero(s)" << sendl;
            while (value.getValue().size() %12 != 0)
                 temp.push_back(0.0);
            value.setValue(temp);
     }
}

void OSGMatrix4x3Variable::initVisual()
{
    //shader->setMatrix4x3(indexShader.getValue(), id.getValue().c_str(), value.getValue().size()/12, transpose.getValue(), &(value.getValue()[0]));
}

} // namespace visual

} // namespace component

} // namespace sofa
