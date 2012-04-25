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

#include "OSGSofaShader.h"
#include "OSGSimpleMaterial.h"

#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/system/FileRepository.h>

#include <sofa/component/visualmodel/LightManager.h>


namespace sofa
{

namespace component
{

namespace visualmodel
{


SOFA_DECL_CLASS(OSGShader)

//Register OSGShader in the Object Factory
int OSGShaderClass = core::RegisterObject("OSGShader")
.add< OSGShader >()
;

// TODO: geometry shaders

OSGShader::OSGShader()
{


}

OSGShader::~OSGShader()
{
    if (!_shaderChunks.empty())
    {
        //_shaderChunks.clear();
        std::vector<OSG::ShaderProgramChunkUnrecPtr>().swap(_shaderChunks);
    }
    if (!_shaderVariableChunks.empty())
    {
        //_shaderVariableChunks.clear();
        std::vector<OSG::ShaderProgramVariableChunkUnrecPtr>().swap(_shaderVariableChunks);
    }
    if (!_vertexSource.empty())
    {
        //_vertexSource.clear();
        std::vector<std::string>().swap(_vertexSource);
    }
    if (!_fragmentSource.empty())
    {
        //_fragmentSource.clear();
        std::vector<std::string>().swap(_fragmentSource);
    }

    if (!_shaderGroups.empty())
    {
        //_fragmentSource.clear();
        std::vector<OSGShaderGroup>().swap(_shaderGroups);
    }

}

std::string OSGShader::loadFile(const std::string& file)
{
    std::ifstream fin(file.c_str());

    // Make sure we opened the file correctly
    if(!fin)
        return "";

    std::string strLine = "";
    std::string strText = "";

    while(std::getline(fin, strLine))
    {
        strText = strText + "\n" + strLine;
    }

    fin.close();

    return strText;
}

void OSGShader::initFiles( void )
{
    std::string tempStr = vertFilename.getFullPath();
    std::string file;
    const std::string SEPARATOR = ";";
    size_t pos = 0;
    size_t oldPos = 0;

    pos = tempStr.find(SEPARATOR, oldPos);

    while (pos != std::string::npos)
    {
        file = tempStr.substr( oldPos, pos - oldPos);

        if (!helper::system::DataRepository.findFile(file))
        {
            serr << "OSGShader : vertex shader file " << file <<" was not found." << sendl;
            return;
        }
        vertexFilenames.push_back( file );

        oldPos = pos + SEPARATOR.size();
        pos = tempStr.find(SEPARATOR, oldPos + SEPARATOR.size());
    }

    file = tempStr.substr( oldPos );

    if (!helper::system::DataRepository.findFile(file))
    {
        serr << "OSGShader : vertex shader file " << file <<" was not found." << sendl;
        return;
    }
    vertexFilenames.push_back( file );

    ///Fragment filenames parsing
    pos = oldPos = 0;
        tempStr = fragFilename.getFullPath();

    pos = tempStr.find(SEPARATOR, oldPos);

    while (pos != std::string::npos)
    {
        file = tempStr.substr( oldPos, pos - oldPos );

        if (!helper::system::DataRepository.findFile(file))
        {
            serr << "OSGShader : fragment shader file " << file <<" was not found." << sendl;
            return;
        }
        fragmentFilenames.push_back( file );

        oldPos = pos + SEPARATOR.size();
        pos = tempStr.find(SEPARATOR, oldPos + SEPARATOR.size());
    }

    file = tempStr.substr( oldPos );
    if (!helper::system::DataRepository.findFile(file))
    {
        serr << "OSGShader : fragment shader file " << file <<" was not found." << sendl;
        return;
    }
    fragmentFilenames.push_back( file );


    ///Geometry filenames parsing
    pos = oldPos = 0;
        tempStr = geoFilename.getFullPath();

    if (geoFilename.getValue() == "" )
    {

        if (fragmentFilenames.size() != vertexFilenames.size())
        {
            serr << "OSGShader : The number of Vertex shaders is different from the number of Fragment Shaders." << sendl;
            return;
        }

    }
    else
    {
        pos = tempStr.find(SEPARATOR, oldPos);

        while (pos != std::string::npos)
        {
            pos = tempStr.find(SEPARATOR, oldPos);
            if (pos != std::string::npos)
            {
                file = tempStr.substr( oldPos, pos - oldPos );

                if (!helper::system::DataRepository.findFile(file))
                {
                        serr << "OSGShader : geometry shader file " << file <<" was not found." << sendl;
                        return;
                }
                geometryFilenames.push_back( file );
            }
            oldPos = pos + SEPARATOR.size();
            pos = tempStr.find(SEPARATOR, oldPos + SEPARATOR.size());
        }

        file = tempStr.substr( oldPos );
        if (!helper::system::DataRepository.findFile(file))
        {
                serr << "OSGShader : geometry shader file " << file <<" was not found." << sendl;
                return;
        }
        geometryFilenames.push_back( file );


        if (fragmentFilenames.size() != vertexFilenames.size() && geometryFilenames.size() !=  vertexFilenames.size())
        {
            serr << "OSGShader : The number of indicated shaders is not coherent (not the same number for each triplet." << sendl;
            return;
        }


        hasGeometryShader = true;
    }
    
    
}

void OSGShader::replaceProceduralTexture(std::string& source)
{
    // should affect only generalRendering.frag;
    std::string search = "uniform sampler2D perlinPermutationsTexture";
    std::string replace = "uniform sampler1D perlinPermutationsTexture";

    std::string::size_type found_start = source.find( search );

    if (found_start == std::string::npos) return;

    source.replace(found_start, search.length(), replace);


    search = "getPerlinValue";
    std::string::size_type aa = found_start;
    found_start = source.find( search , aa);

    if (found_start == std::string::npos) return;


    search = "{";
    aa = found_start;
    found_start = source.find( search , aa);
    search = "}";
    std::string::size_type found_end = source.find( search , found_start);

    std::string temp = source.substr(found_start , found_end - found_start + 1);

    std::cerr << "EXTRACT " << found_start << std::endl;
    std::cerr << temp.c_str() << std::endl;
    std::cerr << "================ " << std::endl;
    //system("pause");



    source.replace(found_end,1," ");

    std::string perlinFunc =
    "{\n";
    perlinFunc += "    return int(texture1D(perlinPermutationsTexture, float((i & 255)/256.0))*256);\n";
    perlinFunc += "    //return int(texture1D(perlinPermutationsTexture, float((i & 255)/256.0))*256);\n";
    perlinFunc += "    //return int(texture1D(perlinPermutationsTexture, float((i & 255)/256.0))*256);\n";
    perlinFunc += "    //return int(texture1D(perlinPermutationsTexture, float((i & 255)/256.0))*256);\n";
    perlinFunc += "}\n";

    size_t expect_end = found_start + perlinFunc.length();
#if 0 // GV unused
    size_t final_end = found_end;
#endif

    if (expect_end > found_end) 
    {
        source.insert(found_end-1, expect_end-found_end +1, ' '); 
    }
    else if (expect_end < found_end) 
    {
        perlinFunc.append(found_end-expect_end+1,' ');
    }
    source.replace( found_start, perlinFunc.length(), perlinFunc );


    temp = source.substr(0 , found_end+500);

    std::cerr << "=====FINAL========= " << std::endl;
    std::cerr << temp.c_str() << std::endl;
    std::cerr << "================ " << std::endl;
    //system("pause");


}

std::string OSGShader::combineAll(std::string header, const std::string &shaderStage, std::string source)
{

    std::size_t spos = 1;
    // Skip #version
    if (source.size() > spos + 8 && (source.substr(spos,8)).compare(std::string("#version")) == 0)
    {
        spos = source.find('\n', spos+8);
        spos++;
    }


    // Skip #extension strings
    while (spos != std::string::npos && source.size() > spos + 10 && source.substr(spos,10) == std::string("#extension"))
    {
        spos = source.find('\n', spos+10);
        spos++;
    }

    header += "#define " + shaderStage + '\n';
    //header += "#define " +  '\n';

    if (spos == 0)
        source = header + source;
    else if (spos == std::string::npos)
        source = source + "\n" + header;
    else
        source = source.substr(0, spos) + header + source.substr(spos);
    return source;
}


void OSGShader::init( void )
{
std::cerr << "OSGShader ::init" << std::endl;
    
    initFiles();

    // vertexFilenames  & fragmentFilenames &  & geometryFilenames should contain the full path names 
    // got to make sure that vertexFilenames match fragmentFilenames
    _shaderGroups.resize(vertexFilenames.size());


    //bool geometryShader = geometryFilenames.size() > 0;

    if (vertexFilenames.size() != fragmentFilenames.size())
    {
        std::cerr << "OSGShader: Vertex Shaders != Fragment Shaders" << std::endl;
    }

    for (unsigned int i=0; i < _shaderGroups.size(); ++i)
    {
        _shaderGroups[i]._shaderChunk = OSG::ShaderProgramChunk::create();

        _shaderGroups[i]._vertexSource = loadFile( vertexFilenames[i].c_str() );
        //std::cerr << "OSG2ShaderProgram vertex read :: " << vertexFilenames[i] << std::endl;

        _shaderGroups[i]._fragmentSource = loadFile( fragmentFilenames[i].c_str() );
        //std::cerr << "OSG2ShaderProgram fragment read :: " << fragmentFilenames[i] << std::endl;

        if (hasGeometryShader)
        {
            _shaderGroups[i]._geometrySource = loadFile(geometryFilenames[i].c_str() );
        std::cerr << "OSG2ShaderProgram geometry read :: " << geometryFilenames[i] << std::endl;
        }

    }

    //passive.setValue(false);
    //turnOn.setValue(true);
}

void OSGShader::initShaders(unsigned int  numberOfLights , bool softShadow)
{
    //std::cerr << "OSGShader::initShaders" << std::endl;
    //system("pause");
}

void OSGShader::initVisual( void )
{
    //OglShader::initVisual();
   
    // By now Macros are collected
    // safe point to construct 
    std::cerr << "OSGShader::initVisual" << std::endl;
    //system("pause");

#ifdef GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXTX
        GLint maxV;
        glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &maxV);

        //if (geometryVerticesOut.getValue() < 0 || geometryVerticesOut.getValue() > maxV)
        //{
        //  for (unsigned int i=0 ; i<vertexFilenames.size() ; i++)
        //      geometryVerticesOut.setValue(3);
        //}
        if (geometryVerticesOut.getValue() < 0 || geometryVerticesOut.getValue() > maxV)
        {
            geometryVerticesOut.setValue(3);
        }
#endif
    // this is where the actual shader program is created
    for (unsigned int i=0; i < _shaderGroups.size(); ++i)
    {

        _shaderGroups[i]._vertexSource = loadFile( vertexFilenames[i].c_str() );
        OSG::ShaderProgramUnrecPtr vp = OSG::ShaderProgram::createVertexShader();
        std::string finalprogram = combineAll(_shaderGroups[i]._macros, "VertexShader",  _shaderGroups[i]._vertexSource);
        vp->setProgram( finalprogram );
        //std::cerr << "=======VERTEX SHADER ===========\n" << finalprogram << std::endl;
        _shaderGroups[i]._shaderChunk->addShader(vp);

        OSG::ShaderProgramUnrecPtr fp = OSG::ShaderProgram::createFragmentShader();
        replaceProceduralTexture(_shaderGroups[i]._fragmentSource);
        finalprogram = combineAll(_shaderGroups[i]._macros, "FragmentShader",  _shaderGroups[i]._fragmentSource);

        fp->setProgram( finalprogram );
        _shaderGroups[i]._shaderChunk->addShader(fp);

        _shaderGroups[i]._shaderVarChunk =  OSG::ShaderProgramVariableChunk::create();


        if (!hasGeometryShader) continue;

        OSG::ShaderProgramUnrecPtr gp = OSG::ShaderProgram::createGeometryShader();
        finalprogram = combineAll(_shaderGroups[i]._macros, "GeometryShader",  _shaderGroups[i]._geometrySource);
        
       
        if (geometryInputType.getValue() != -1)
            gp->setProgramParameter(GL_GEOMETRY_INPUT_TYPE_EXT, 
                                    geometryInputType.getValue()); 
        if (geometryOutputType.getValue() != -1)
            gp->setProgramParameter(GL_GEOMETRY_OUTPUT_TYPE_EXT, 
                                    geometryOutputType.getValue()); 
        gp->setProgramParameter(GL_GEOMETRY_VERTICES_OUT_EXT, 
                                    geometryVerticesOut.getValue()); 

        gp->setProgram( _shaderGroups[i]._geometrySource );
        //gp->setProgram( finalprogram );
        _shaderGroups[i]._shaderChunk->addShader(gp);

        

    }
}


void OSGShader::start( void )
{
//std::cerr << "OSGShader ::start" << std::endl;
    //OglShader::start();

}

void OSGShader::stop( void )
{
//std::cerr << "OSGShader ::stop" << std::endl;
    //OglShader::stop();

}

bool OSGShader::isActive( void )
{
    return true;

    //return !passive.getValue();
}


void OSGShader::addDefineMacro(const unsigned int index, const std::string &name, const std::string &value)
{
    std::cerr << "OSGShader::addDefineMacro" << std::endl;
    //shaderVector[index]->AddDefineMacro(name, value);
    //_macros.
    if ( index > _shaderGroups.size() ) return;
    _shaderGroups[index]._macros += "#define " + name + " " + value + "\n";
    std::cerr <<  _shaderGroups[index]._macros << std::endl;
}


/*
static void light0Active(OSG::DrawEnv *pEnv, OSG::Int32 &iLoc)
{
    if(iLoc != -1)
    {
        OSGGETGLFUNC_EXT(glUniform1i,
                         osgGlUniform1i,
                         OSG::ShaderProgram::getFuncIdUniform1i());

        osgGlUniform1i(iLoc, 
                       GLint(pEnv->getLightState() & 0x0001));
    }
}

void OSGShader::setTexture(const unsigned int index, const char* name, unsigned short unit, OSG::Image* image)
{
    if ( index > _shaderGroups.size() ) return;

    _shaderGroups[index]._textureChunk = OSG::TextureObjChunk::create();
    _shaderGroups[index]._textureChunk->setImage(image);
    _shaderGroups[index]._textureChunk->setMinFilter(GL_NEAREST);
    _shaderGroups[index]._textureChunk->setMagFilter(GL_NEAREST);
    _shaderGroups[index]._textureChunk->setWrapS(GL_REPEAT);
    _shaderGroups[index]._textureChunk->setWrapT(GL_REPEAT);
    //_shaderGroups[index]._textureChunk->setWrapR(GL_REPEAT);

    //OSG::ImageUnrecPtr img = _shaderGroups[index]._textureChunk->getImage();
    //img->write("TEXTURE_PERLIN.png");

    bool success = _shaderGroups[index]._shaderVarChunk->addUniformVariable( name, OSG::UInt16(unit) );
    _shaderGroups[index]._textureUnit = OSG::UInt16(unit);
    // Is procedural variable what I think it is ??????
    //OSG::Int32 texunit = unit;
    //bool success = _shaderGroups[index]._shaderVarChunk->addProceduralVariable( name, &light0Active );
if (success)
{
std::cerr << "TEXTURE UNIT : " << unit << " NAME " << name << std::endl;
    //system("pause");    
}

}
*/

void OSGShader::addTexture(const unsigned int index, const char* name, unsigned short unit, OSG::TextureObjChunk* texture)
{
    if ( index > _shaderGroups.size() ) return;
    std::cerr << "OSGShader::addTexture IN" << std::endl;
    _shaderGroups[index]._textureChunks.
    insert(OSGShaderGroup::TextureChunkPair(OSG::UInt16(unit), texture));
    
    /*bool success = */_shaderGroups[index]._shaderVarChunk->addUniformVariable( name, OSG::UInt16(unit) );

}

void OSGShader::addToMaterial(unsigned int i, OSG::SimpleMaterial* mat) const
{
    if (mat)
    {
        //mat->addChunk( getShaderTextureObjChunk(i), _shaderGroups[i]._textureUnit);
        //mat->addChunk( getShaderTextureObjChunk(i), 0);
        //mat->addChunk( getShaderTextureObjChunk(i), 1);

        mat->addChunk( getShaderChunk(i) );
        mat->addChunk( getShaderVariableChunk(i) );
        
        const OSGShaderGroup::TextureChunkMap& amap 
        = _shaderGroups[i]._textureChunks;
        if ( amap.size() == 0 ) return;

        OSGShaderGroup::TextureChunkMap::const_iterator itr = amap.begin();

        for (; itr != amap.end(); ++itr )
        {
        //OSG::TextureObjChunkUnrecPtr cog 
        //= OSG::dynamic_pointer_cast <OSG::TextureObjChunk>(itr->second);
        //if (cog) exit(1);

//std::cerr << "OSGShader:: addToMaterial : " << mat << " "  << i << " " << itr->first <<  " " << itr->second << "Shader " << this << std::endl;
//OSG::StateChunk *oldTEX  = mat->find(OSG::TextureObjChunk::getClassType(), itr->first);
   //if(oldTEX != NULL) 
   {
       //mat->subChunk(oldTEX);
   }


            //mat->addChunk(itr->second, 0);
            mat->addChunk(itr->second, itr->first);
            //if (itr->second->getImage()) exit(1);
        }
    }

}


OSGShaderElement::OSGShaderElement()
: id(initData(&id, std::string(""), "id", "Set an ID name"))
, indexShader(initData(&indexShader, (unsigned int) 0, "indexShader", "Set the index of the desired shader you want to apply this parameter"))
{

}

void OSGShaderElement::init()
{
    sofa::core::objectmodel::BaseContext* context = this->getContext();
    shader = context->core::objectmodel::BaseContext::get<OSGShader>();

    if (!shader)
    {
        serr << "OSGShaderElement: shader not found "<< sendl;
        return;
    }
    if (id.getValue().empty())
        id.setValue(this->getName());
}



}//namespace visualmodel

} //namespace component

} //namespace sofa
