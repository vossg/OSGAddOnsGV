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

#ifndef _OSGSOFASHADER_H_
#define _OSGSOFASHADER_H_

#include <sofa/component/visualmodel/OglShader.h>


// OpenSG
#include "OSGContribSOFADef.h"
#include "OSGSimpleMaterial.h"
#include "OSGShaderProgramChunk.h"
#include "OSGShaderProgramVariableChunk.h"
#include "OSGTextureObjChunk.h"
#include "OSGImage.h"

//#ifdef IN2_SOFA
//#define SOFA_OPENGL_VISUAL_API SOFA_EXPORT_DYNAMIC_LIBRARY
//#else
//#define SOFA_OPENGL_VISUAL_API  
//#endif

OSG_BEGIN_NAMESPACE

class SimpleMaterial;

OSG_END_NAMESPACE

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

    /*
    class OSG2_Texture_Info 
    {
    public:
        OSG2_TextureInfo():_textureChunk(NULL), _textureUnit(0)  { }
        ~OSG2_TextureInfo() { _textureChunk = NULL; }
        OSG::UInt16 _textureUnit;
        OSG::TextureObjChunkRecPtr _textureChunk;
    
    };
    */
class OSG_CONTRIBSOFA_DLLMAPPING OSG2_ShaderGroup 
{

  public:
    OSG2_ShaderGroup():_shaderChunk(NULL), _shaderVarChunk(NULL)  { }
    ~OSG2_ShaderGroup() { _shaderChunk = NULL; _shaderVarChunk = NULL; _textureChunks.clear(); }
    OSG::ShaderProgramChunkRecPtr  _shaderChunk;
    OSG::ShaderProgramVariableChunkRecPtr _shaderVarChunk;

    typedef std::map<OSG::UInt16 , OSG::TextureObjChunkRecPtr> TextureChunkMap;
    typedef std::pair<OSG::UInt16 , OSG::TextureObjChunkRecPtr> TextureChunkPair;

    TextureChunkMap _textureChunks;
    std::string _vertexSource;
    std::string _fragmentSource;
    std::string _geometrySource;
    std::string _macros;
    OSG::UInt16 _textureUnit;

};


//class SOFA_OPENGL_VISUAL_API OSG2_Shader : public sofa::core::visual::Shader, public sofa::core::visual::VisualModel {

class OSG_CONTRIBSOFA_DLLMAPPING OSG2_Shader : public sofa::component::visualmodel::OglShader {

  public:
    SOFA_CLASS(OSG2_Shader, sofa::component::visualmodel::OglShader);
    //SOFA_CLASS(OSG2_Shader, sofa::core::visual::Shader);
    
  protected:
    OSG2_Shader();
    virtual ~OSG2_Shader();

    std::string loadFile(const std::string& file);
    void initFiles( void );
    void replaceProceduralTexture(std::string& source);
    std::string combineAll(std::string header, const std::string &shaderStage, std::string source);
    

  public:

    void init( void );
    void initVisual( void );

    void initShaders(unsigned int numberOfLights, bool softShadow);

    void start( void );
    void stop( void );
    bool isActive( void );

    void addDefineMacro(const unsigned int index, const std::string &name, const std::string &value);
    //void setTexture(const unsigned int index, const char* name, unsigned short unit, OSG::Image* image);
    
    void addTexture(const unsigned int index, const char* name, unsigned short unit, OSG::TextureObjChunk* texture);


    OSG::ShaderProgramChunkRecPtr getShaderChunk( unsigned int i) const 
        { return  ( ( i < _shaderGroups.size() ) ? _shaderGroups[i]._shaderChunk : NULL  );  }

    OSG::ShaderProgramVariableChunkRecPtr getShaderVariableChunk( unsigned int i) const 
        { return  ( ( i < _shaderGroups.size() ) ? _shaderGroups[i]._shaderVarChunk : NULL  );  }

    //OSG::TextureObjChunkRecPtr getShaderTextureObjChunk( unsigned int i) const 
    //{ return  ( ( i < _shaderGroups.size() ) ? _shaderGroups[i]._textureChunk : NULL  );  }

    void addToMaterial(unsigned int i, OSG::SimpleMaterial* mat) const;
  protected:

    //OpenSG
    std::vector<OSG::ShaderProgramChunkRecPtr> _shaderChunks;
    std::vector<OSG::ShaderProgramVariableChunkRecPtr> _shaderVariableChunks;
    std::vector<std::string> _vertexSource;
    std::vector<std::string> _fragmentSource;


    std::vector< OSG2_ShaderGroup > _shaderGroups;



};


/**
 *  \brief Abstract class which defines a element to be used with a OSG2_Shader.
 *
 *  This is only an partial implementation of the interface ShaderElement
 *  which adds a pointer to its corresponding shader (where it will be used)
 *  and the id (or name) of the element.
 */
class OSG_CONTRIBSOFA_DLLMAPPING OSG2_ShaderElement : public core::visual::ShaderElement {

  protected:
    ///Name of element (corresponding with the shader)
    Data<std::string> id;
    ///Name of element (corresponding with the shader)
    Data<unsigned int> indexShader;
    ///Shader to use the element with
    OSG2_Shader* shader;

  public:
    OSG2_ShaderElement();
    virtual ~OSG2_ShaderElement() { };
    virtual void init();
    const std::string getId() const {return id.getValue();};
    void setID( std::string str ) { *(id.beginEdit()) = str; id.endEdit();};
    void setIndexShader( unsigned int index) { *(indexShader.beginEdit()) = index; indexShader.endEdit();};

};

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFASHADOWSHADER_H_
