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

#include "OSGSofaModel.h"
#include "OSGSofaShader.h"

#include "OSGImageFileHandler.h"
#include "OSGMaterialChunk.h"
#include "OSGDepthChunk.h"
#include "OSGNameAttachment.h"
#include "OSGGeoFunctions.h"

#include "OSGAction.h"

#include <sofa/core/visual/VisualParams.h>
#include <sofa/helper/system/gl.h>
#include <sofa/helper/system/glut.h>
#include <sofa/helper/gl/RAII.h>
#include <sofa/helper/vector.h>
#include <sofa/defaulttype/Quat.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sstream>
#include <string.h>


namespace sofa
{

namespace component
{

namespace visualmodel
{

using namespace sofa::defaulttype;
using namespace sofa::core::loader;

SOFA_DECL_CLASS(OSG2_Model)

int OSG2_ModelClass = core::RegisterObject("Visual Model for OpenSG 2.0")
.add< OSG2_Model >()
;


OSG2_Model::OSG2_Model()
: premultipliedAlpha(initData(&premultipliedAlpha, (bool) false, "premultipliedAlpha", "is alpha premultiplied ?"))
#ifndef SOFA_HAVE_GLEW
, useVBO(initData(&useVBO, (bool) false, "useVBO", "Use VBO for rendering"))
#else
, useVBO(initData(&useVBO, (bool) true, "useVBO", "Use VBO for rendering"))
#endif
, writeZTransparent(initData(&writeZTransparent, (bool) false, "writeZTransparent", "Write into Z Buffer for Transparent Object"))
, alphaBlend(initData(&alphaBlend, (bool) false, "alphaBlend", "Enable alpha blending"))
, depthTest(initData(&depthTest, (bool) true, "depthTest", "Enable depth testing"))
, cullFace(initData(&cullFace, (int) 0, "cullFace", "Face culling (0 = no culling, 1 = cull back faces, 2 = cull front faces)"))
, blendEquation( initData(&blendEquation, "blendEquation", "if alpha blending is enabled this specifies how source and destination colors are combined") )
, sourceFactor( initData(&sourceFactor, "sfactor", "if alpha blending is enabled this specifies how the red, green, blue, and alpha source blending factors are computed") )
, destFactor( initData(&destFactor, "dfactor", "if alpha blending is enabled this specifies how the red, green, blue, and alpha destination blending factors are computed") )

// OpenSG 
,_attachNode(NULL)
,_switchCore(NULL)
,_polygonPoints(NULL)
,_vertexNormals(NULL)
,_texCoord2D(NULL)
,_tangents(NULL)
,_biTangents(NULL)
,_textureName(NULL)
,_polygonChunk(NULL)
,_shadeModelChunk(NULL)
,_twoSidedLightingChunk(NULL)
, tex(NULL), canUseVBO(false), VBOGenDone(false), initDone(false), useTriangles(false), useQuads(false)
, oldVerticesSize(0), oldTrianglesSize(0), oldQuadsSize(0)
{

    sofa::helper::OptionsGroup* blendEquationOptions = blendEquation.beginEdit();
    blendEquationOptions->setNames(4,"GL_FUNC_ADD", "GL_FUNC_SUBTRACT", "GL_MIN", "GL_MAX"); // .. add other options
    blendEquationOptions->setSelectedItem(0);
    //this->f_printLog.setValue(true);
    blendEquation.endEdit();

    // alpha blend values
    sofa::helper::OptionsGroup* sourceFactorOptions = sourceFactor.beginEdit();
    sourceFactorOptions->setNames(4,"GL_ZERO", "GL_ONE", "GL_SRC_ALPHA", "GL_ONE_MINUS_SRC_ALPHA"); // .. add other options
    sourceFactorOptions->setSelectedItem(2);
    //this->f_printLog.setValue(true);
    sourceFactor.endEdit();

    sofa::helper::OptionsGroup* destFactorOptions = destFactor.beginEdit();
    destFactorOptions->setNames(4,"GL_ZERO", "GL_ONE", "GL_SRC_ALPHA", "GL_ONE_MINUS_SRC_ALPHA"); // .. add other options
    destFactorOptions->setSelectedItem(3);
    //this->f_printLog.setValue(true);
    destFactor.endEdit();
}

OSG2_Model::~OSG2_Model()
{
    if (tex!=NULL) delete tex;


    _attachNode = NULL;
    _polygonPoints = NULL;
    _vertexNormals = NULL;
    _texCoord2D = NULL;

    _textureName = NULL;

    //_texMaterial.clear();
    std::vector<OSG::TextureObjChunkRecPtr>().swap(_texMaterial);

    _polygonChunk = NULL;
    _shadeModelChunk = NULL;
    _twoSidedLightingChunk = NULL;
    
}

void OSG2_Model::internalDraw(const core::visual::VisualParams* vparams, bool transparent)
{
    m_vtexcoords.updateIfDirty();

    //updateBuffers();
    //updateModel();

    helper::ReadAccessor< Data< helper::vector<FaceGroup> > > groups = this->groups;
    
    if (vparams->displayFlags().getShowVisualModels()) 
    {
        if (_switchCore) 
        {
            if (_switchCore->getChoice() != OSG::Switch::ALL)
                _switchCore->setChoice(OSG::Switch::ALL);
        }
    }
    else
    {
        if (_switchCore) 
        {
            if (_switchCore->getChoice() != OSG::Switch::NONE)
                _switchCore->setChoice(OSG::Switch::NONE);
        }
    }

    if (vparams->displayFlags().getShowWireFrame())
    {
        if (_polygonChunk) 
        {
            if (_polygonChunk->getFrontMode() != GL_LINE) _polygonChunk->setFrontMode(GL_LINE);
            if (_polygonChunk->getBackMode() != GL_LINE) _polygonChunk->setBackMode(GL_LINE);
        }
    }
    else
    {
        if (_polygonChunk) 
        {
            if (_polygonChunk->getFrontMode() != GL_FILL) _polygonChunk->setFrontMode(GL_FILL);
            if (_polygonChunk->getBackMode() != GL_FILL) _polygonChunk->setBackMode(GL_FILL);
        }

    }            
            

}


bool OSG2_Model::loadTexture(const std::string& filename)
{

    OSG::ImageUnrecPtr image = OSG::Image::create();
    bool read = image->read(filename.c_str());
    if (!read)
        std::cerr << "OpenSG OSG2_Model_loadTextureSingle(" << this->getName() << "): couldn't create an image from file " << filename << std::endl;
    //helper::io::Image *img = helper::io::Image::Create(filename);
    //if (!img)
    //    return false;


    //helper::io::Image::ChannelFormat cf = img->getChannelFormat();
    //helper::io::Image::DataType dt = img->getDataType();

    //OSG::Image::PixelFormat pixelFormat = convertSofaToOpenSG( cf );
    //OSG::Image::Type type =  convertSofaToOpenSG( dt );

    //image->set(pixelFormat,img->getWidth(),img->getHeight(),img->getDepth(),
    //               1,1,0.0,img->getPixels(),type);

    _textureName = NULL;
    _textureName  = OSG::TextureObjChunk::create();
    _textureName->setImage(image);

    //tex = new helper::gl::Texture(img, true, true, false, srgbTexturing.getValue());
    return true;
}

OSG::Image::PixelFormat
OSG2_Model::convertSofaToOpenSG(helper::io::Image::ChannelFormat cf)
{
        OSG::Image::PixelFormat pixelFormat = OSG::Image::OSG_INVALID_PF;
        switch (cf)
        {
            case   helper::io::Image::L :   pixelFormat = OSG::Image::OSG_L_PF; break;
            case   helper::io::Image::LA :   pixelFormat = OSG::Image::OSG_LA_PF; break;
            case   helper::io::Image::R :   pixelFormat = OSG::Image::OSG_L_PF; break;
            case   helper::io::Image::RG :   pixelFormat = OSG::Image::OSG_LA_PF; break;
            case   helper::io::Image::RGB :   pixelFormat = OSG::Image::OSG_RGB_PF; break;
            case   helper::io::Image::RGBA :   pixelFormat = OSG::Image::OSG_RGBA_PF; break;
            case   helper::io::Image::BGR :   pixelFormat = OSG::Image::OSG_BGR_PF; break;
            case   helper::io::Image::BGRA :   pixelFormat = OSG::Image::OSG_BGRA_PF; break;
            default: break;
        };

        return pixelFormat;
}

OSG::Image::Type 
OSG2_Model::convertSofaToOpenSG(helper::io::Image::DataType dt)
{
      OSG::Image::Type type = OSG::Image::OSG_INVALID_IMAGEDATATYPE;
      switch (dt)
        {
            case   helper::io::Image::UNORM8 :   type = OSG::Image::OSG_UINT8_IMAGEDATA; break;
            case   helper::io::Image::UNORM16 :   type = OSG::Image::OSG_UINT16_IMAGEDATA; break;
            case   helper::io::Image::UINT32 :   type = OSG::Image::OSG_UINT32_IMAGEDATA; break;
            case   helper::io::Image::HALF :   type = OSG::Image::OSG_FLOAT16_IMAGEDATA; break;
            case   helper::io::Image::FLOAT :   type = OSG::Image::OSG_FLOAT32_IMAGEDATA; break;
            //case   helper::io::Image::UCOMPRESSED :   type = OSG::Image::OSG_UINT8_IMAGEDATA; break;
            default: break;
        };

        return type;
}



// a false result indicated problems during textures loading
bool OSG2_Model::loadTextures()
{
    bool result = true;

    //for output
    std::string str;

    //count the total number of activated textures
    std::vector<unsigned int> activatedTextures;
    for (unsigned int i = 0 ; i < this->materials.getValue().size() ; i++)
        if (this->materials.getValue()[i].useTexture && this->materials.getValue()[i].activated)
            activatedTextures.push_back(i);

//if (!activatedTextures.empty())
//std::cerr << "OSG2_Model : " << this->getName() << " has TEXTURES " << std::endl;

    for (std::vector< unsigned int>::iterator i = activatedTextures.begin() ; i < activatedTextures.end(); i++)
    {
        std::string textureFile(this->materials.getValue()[*i].textureFilename);

        if (!sofa::helper::system::DataRepository.findFile(textureFile))
        {
            textureFile = this->fileMesh.getFullPath();
            unsigned int position = textureFile.rfind("/");
            textureFile.replace (position+1,textureFile.length() - position, this->materials.getValue()[*i].textureFilename);

            if (!sofa::helper::system::DataRepository.findFile(textureFile))
            {
                if (i!=activatedTextures.begin()) std::cout << "\n" << std::flush;
                std::cerr   << "ERROR(OSG2_Model(" << this->getName() << "):Texture \"" << this->materials.getValue()[*i].textureFilename << "\" not found"
                        << " in material " << this->materials.getValue()[*i].name <<  std::endl;
                result = false;
                continue;
            }
        }

        //helper::io::Image *img = helper::io::Image::Create(textureFile);
        //if (!img)
        //{
        //   std::cerr << "ERROR(OSG2_Model(" << this->getName() << "): couldn't create an image from file " << this->materials.getValue()[*i].textureFilename << std::endl;
        //   result = false;
        //   continue;
        //}

        OSG::ImageUnrecPtr image = OSG::Image::create();

        bool read = image->read(textureFile.c_str());
        if (read)
            std::cerr << "OpenSG FILEIO(OSG2_Model(" << this->getName() << "): created an image from file " << this->materials.getValue()[*i].textureFilename << std::endl;


        // temp hack to use sofa io to read png

        //helper::io::Image::ChannelFormat cf = img->getChannelFormat();
        //helper::io::Image::DataType dt = img->getDataType();

        //OSG::Image::PixelFormat pixelFormat = convertSofaToOpenSG( cf );
        //OSG::Image::Type type =  convertSofaToOpenSG( dt );

        //image->set(pixelFormat,img->getWidth(),img->getHeight(),img->getDepth(),
        //           1,1,0.0,img->getPixels(),type);

        materialTextureIdMap.insert(std::pair<int, int>(*i,_texMaterial.size()));


        // OpenSG

        OSG::TextureObjChunkRecPtr matTex  = OSG::TextureObjChunk::create();
        matTex->setImage(image);
        _texMaterial.push_back(matTex);

        /****OUTPUT****/
        //clear the previous line
        std::cout << std::string(str.length(), '\b');

        str.clear();

        std::stringstream out; out << _texMaterial.size() << "/" << activatedTextures.size() << " textures loaded for OSG2_Model " << this->getName();
        str += out.str();

        std::cout << str << std::flush;
    }
    std::cout << std::endl;

    if (_texMaterial.size() != activatedTextures.size())
        std::cerr << "ERROR(OSG2_Model(" << this->getName()<< ")) " << (activatedTextures.size() - _texMaterial.size()) << " textures couldn't be loaded" <<  std::endl;



    /**********************************************
     * Load textures for bump mapping
     *********************************************/
//
//    for (unsigned int i = 0 ; i < this->materials.getValue().size() ; i++)
//    {
//       //we count only the bump texture with an activated material
//       if (this->materials.getValue()[i].useBumpMapping && this->materials.getValue()[i].activated)
//       {
//            std::string textureFile(this->materials.getValue()[i].bumpTextureFilename);
//
//            if (!sofa::helper::system::DataRepository.findFile(textureFile))
//            {
//                textureFile = this->fileMesh.getFullPath();
//                unsigned int position = textureFile.rfind("/");
//                textureFile.replace (position+1,textureFile.length() - position, this->materials.getValue()[i].bumpTextureFilename);
////                std::cout << "Loading texture: " << textureFile << std::endl;
//
//                if (!sofa::helper::system::DataRepository.findFile(textureFile))
//                {
//                    std::cout <<  std::endl;
//                    serr << "Texture \"" << this->materials.getValue()[i].bumpTextureFilename << "\" not found"
//                            << " in material " << this->materials.getValue()[i].name << " for OSG2_Model " << this->name
//                            << "(\""<< this->fileMesh.getFullPath() << "\")" << sendl;
//                    break;
//                }
//            }
//
//            helper::io::Image *img = helper::io::Image::Create(textureFile);
//            if (!img)
//            {
//                std::cout <<  std::endl;
//               std::cerr << "Error:OSG2_Model:loadTextures: couldn't create an image from file " << this->materials.getValue()[i].bumpTextureFilename << std::endl;
//               return false;
//            }
//            helper::gl::Texture * text = new helper::gl::Texture(img, true, true, false, srgbTexturing.getValue());
//            materialTextureIdMap.insert(std::pair<int, int>(i,textures.size()));
//            textures.push_back( text );
//
//            std::cout << "\r\033[K" << i+1 << "/" << this->materials.getValue().size() << " textures loaded for bump mapping for OSG2_Model " << this->getName()
//                    << "(loading "<<textureFile << ")"<< std::flush;
//       }
//    }
//    std::cout << "\r\033[K" << std::flush;
    return result;
}

void OSG2_Model::initVisual()
{
    initTextures();

    createStateChunks();


    createSubgraph();



//const ResizableExtVector<Coord>& vertices = this->getVertices();
//serr << "OSG2_Model:: " << this->getName()  << " Vertices " << vertices.size() << sendl;

//const ResizableExtVector<Coord>& verticesAgain = this->m_positions.getValue();

//serr << "OSG2_Model::AfterUpdateVisual " << this->getName()  << " M_Positions " << verticesAgain.size() << sendl;
//if (isUsingTopology())
//{
//    serr << "OSG2_Model:: IS USING TOPOLOGY vertices\n" << this->m_vertices.getValue().size() << sendl;
//    serr << "OSG2_Model:: IS USING TOPOLOGY triangles\n" << this->m_triangles.getValue().size()<< sendl;
//    serr << "OSG2_Model:: IS USING TOPOLOGY quads\n" << this->m_quads.getValue().size() << sendl;
//}

    if ( alphaBlend.getValue() )
    {
        blendEq = getGLenum( blendEquation.getValue().getSelectedItem().c_str() );
        sfactor = getGLenum( sourceFactor.getValue().getSelectedItem().c_str() );
        dfactor = getGLenum( destFactor.getValue().getSelectedItem().c_str() );
    }


    initDone = true;

    updateBuffers();

}


void OSG2_Model::createSubgraph()
{
    _switchCore = OSG::Switch::create();
    _switchCore->setChoice(OSG::Switch::ALL);

    _attachNode = OSG::makeNodeFor(_switchCore);

    OSG::setName( _attachNode, this->getName().c_str() );
}


void OSG2_Model::updateVertices()
{
    if (!_polygonPoints) return;

    const ResizableExtVector<Coord>& vertices = this->getVertices();
    OSG::GeoPnt3fProperty::StoredFieldType *posfield = _polygonPoints->editFieldPtr();
    OSG::GeoPnt3fProperty::StoredFieldType::iterator it = posfield->begin();
     
    Coord p;
    for (unsigned int i=0; i < vertices.size(); ++i)
    {
        p = vertices[i];
        (*it) = OSG::Pnt3f(p[0],p[1],p[2]); 
        it++;
    }
}

void OSG2_Model::updateNormals()
{
    if (!_vertexNormals) return;

    OSG::GeoVec3fProperty::StoredFieldType *normfield = _vertexNormals->editFieldPtr();
   
    OSG::GeoVec3fProperty::StoredFieldType::iterator nit = normfield->begin();
     
    const ResizableExtVector<Coord>& vnormals = this->getVnormals();

    Deriv n;
    for (unsigned int i=0; i < vnormals.size(); ++i)
    {
        n = vnormals[i];
        (*nit) = OSG::Vec3f(n[0],n[1],n[2]); 
        nit++;
    }

}

void OSG2_Model::updateTrianglesIndicesInGroup(const FaceGroup& fg, OSG::Geometry* geo)
{
    int numTriInGroup = fg.nbt;
    if (numTriInGroup <= 0) return;

    if (!geo) return;

    //std::cerr << "OSG2_Model::updateTrianglesIndicesInGroup() " << std::endl;

    OSG::OSG2_GeometryRecPtr geoCore = geo;

    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
#if 0  // GV unused
    const ResizableExtVector<Quad>& quads = this->getQuads();
    const ResizableExtVector<Coord>& vertices = this->getVertices();

    const ResizableExtVector<Coord>& vnormals = this->getVnormals();

    const ResizableExtVector<TexCoord>& vtexcoords = this->getVtexcoords();
#endif

    OSG::OSG2_GeoUInt8PropertyRecPtr polyTypes 
    = dynamic_cast<OSG::GeoUInt8Property*>(geoCore->getTypes());
    OSG::OSG2_GeoUInt32PropertyRecPtr polyLens 
    = dynamic_cast<OSG::GeoUInt32Property*>(geoCore->getLengths());

    OSG::OSG2_GeoUInt32PropertyRecPtr polyIndices
    = dynamic_cast<OSG::GeoUInt32Property*>(geoCore->getIndex( OSG::Geometry::PositionsIndex));
    OSG::OSG2_GeoUInt32PropertyRecPtr normIndices
    = dynamic_cast<OSG::GeoUInt32Property*>(geoCore->getIndex( OSG::Geometry::NormalsIndex));

    if (polyTypes->size() == 0 ) return;
    if (polyLens->size() == 0 ) return;
    if (polyIndices->size() == 0 ) return;
    if (normIndices->size() == 0 ) return;
    // foreach triangle in polyTypes
    OSG::UInt8 typeval;
    OSG::UInt32 lenval;
    OSG::UInt32 index = 0;
    OSG::UInt32 last = fg.tri0 + numTriInGroup;
#if 0 // GV unused
    OSG::UInt32 cur_tri = fg.tri0;
#endif
    Triangle t;
    
    polyLens->getValue(lenval,0);

    for (OSG::UInt32 i=0; i < polyTypes->size(); ++i)
    {
        polyTypes->getValue(typeval,i);
        polyLens->getValue(lenval,i);

        //std::cerr << "LEN VAL : " << lenval <<  "num : " << numTriInGroup << std::endl;

        if (typeval != GL_TRIANGLES && typeval != GL_TRIANGLE_STRIP &&
            typeval != GL_TRIANGLE_FAN) 
        {
            index += lenval;
            continue;
        }

        for (OSG::UInt32 j= fg.tri0; j < last; ++j, index +=3 )
        {
            // triangle found
            t = triangles[j];
            polyIndices->setValue(t[0], index);
            polyIndices->setValue(t[1], index+1);
            polyIndices->setValue(t[2], index+2);
        }
    }
    if (_tangents)
    {

        OSG::calcVertexTangentsProp(geoCore, OSG::Geometry::TexCoordsIndex, 
        OSG::Geometry::NormalsIndex, 
        OSG::Geometry::TexCoords1Index, 
        OSG::Geometry::TexCoords2Index); 
    }

}

void OSG2_Model::updateTrianglesIndices()
{
    if (!useTriangles) return;
    if (!_attachNode) return;

    helper::ReadAccessor< Data< helper::vector<FaceGroup> > > groups = this->groups;

    //if (_attachNode->getNChildren() != groups.size()) return;

    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    const ResizableExtVector<Quad>& quads = this->getQuads();
#if 0 // GV unused
    const ResizableExtVector<Coord>& vertices = this->getVertices();
#endif

//std::cerr << "OSG2_Model::updateTrianglesIndices" << std::endl;
    if (groups.empty())
    {

//serr << "OSG2_Model:: " << this->getName()  << " updateTrianglesIndices Single"  << sendl;

        FaceGroup fg;
        fg.nbt = triangles.size();
        fg.nbq = quads.size();
        fg.tri0 = 0;
        fg.quad0 = 0;
        fg.materialId = -1;

        //if (_attachNode) 
        {
            // fresh _attachNode
            if (_attachNode->getNChildren() <= 0)
            {
            }
            else 
            {
                OSG::NodeCoreUnrecPtr nc = _attachNode->getChild(0)->getCore();
                OSG::OSG2_GeometryRecPtr geocore 
                = OSG::dynamic_pointer_cast <OSG::Geometry>(nc);
                updateTrianglesIndicesInGroup( fg, geocore);

            }

        }
        return;
    }
    // number of groups the same
    OSG::NodeCoreUnrecPtr geonode;
    OSG::OSG2_GeometryRecPtr geocore; 
    for (unsigned int i=0; i < groups.size() ; ++i)
    {
          geonode = _attachNode->getChild(i)->getCore();
          OSG::OSG2_GeometryRecPtr geocore 
          = OSG::dynamic_pointer_cast <OSG::Geometry>(geonode);
          updateTrianglesIndicesInGroup(this->groups.getValue()[i], geocore);
    }
}

void OSG2_Model::updateQuadsIndicesInGroup(const FaceGroup& fg, OSG::Geometry* geo)
{
    int numQuadInGroup = fg.nbq;
    if (numQuadInGroup <= 0) return;

    if (!geo) return;

    //std::cerr << "OSG2_Model::updateQuadsIndicesInGroup() " << std::endl;

    OSG::OSG2_GeometryRecPtr geoCore = geo;

    const ResizableExtVector<Quad>& quads = this->getQuads();

#if 0 // GV unsused
    const ResizableExtVector<TexCoord>& vtexcoords = this->getVtexcoords();
#endif

    OSG::OSG2_GeoUInt8PropertyRecPtr polyTypes 
    = dynamic_cast<OSG::GeoUInt8Property*>(geoCore->getTypes());
    OSG::OSG2_GeoUInt32PropertyRecPtr polyLens 
    = dynamic_cast<OSG::GeoUInt32Property*>(geoCore->getLengths());

    OSG::OSG2_GeoUInt32PropertyRecPtr polyIndices
    = dynamic_cast<OSG::GeoUInt32Property*>(geoCore->getIndex( OSG::Geometry::PositionsIndex));
    OSG::OSG2_GeoUInt32PropertyRecPtr normIndices
    = dynamic_cast<OSG::GeoUInt32Property*>(geoCore->getIndex( OSG::Geometry::NormalsIndex));

    if (polyTypes->size() == 0 ) return;
    if (polyLens->size() == 0 ) return;
    if (polyIndices->size() == 0 ) return;

    // foreach quad in polyTypes
    OSG::UInt8 typeval;
    OSG::UInt32 lenval;
    OSG::UInt32 index = 0;
    OSG::UInt32 last = fg.quad0 + numQuadInGroup;
    Quad t;
    
    polyLens->getValue(lenval,0);

    for (OSG::UInt32 i=0; i < polyTypes->size(); ++i)
    {
        polyTypes->getValue(typeval,i);
        polyLens->getValue(lenval,i);

        //std::cerr << "LEN VAL : " << lenval <<  "num : " << numQuadInGroup << std::endl;

        if (typeval != GL_QUADS) 
        {
            index += lenval;
            continue;
        }

        for (OSG::UInt32 j= fg.quad0; j < last; ++j, index +=4 )
        {
            // quad found
            t = quads[j];
            polyIndices->setValue(t[0], index);
            polyIndices->setValue(t[1], index+1);
            polyIndices->setValue(t[2], index+2);
            polyIndices->setValue(t[3], index+3);
        }
    }
    if (_tangents)
    {

        OSG::calcVertexTangentsProp(geoCore, OSG::Geometry::TexCoordsIndex, 
        OSG::Geometry::NormalsIndex, 
        OSG::Geometry::TexCoords1Index, 
        OSG::Geometry::TexCoords2Index); 
    }

}


void OSG2_Model::updateQuadsIndices()
{
    if (!useQuads) return;
    if (!_attachNode) return;

    helper::ReadAccessor< Data< helper::vector<FaceGroup> > > groups = this->groups;

    //if (_attachNode->getNChildren() != groups.size()) return;

//std::cerr << "OSG2_Model::updateQuadsIndices" << std::endl;

    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    const ResizableExtVector<Quad>& quads = this->getQuads();
#if 0 // GV unused
    const ResizableExtVector<Coord>& vertices = this->getVertices();
#endif

    if (groups.empty())
    {

//serr << "OSG2_Model:: " << this->getName()  << " updateQuadsIndices Single"  << sendl;

        FaceGroup fg;
        fg.nbt = triangles.size();
        fg.nbq = quads.size();
        fg.tri0 = 0;
        fg.quad0 = 0;
        fg.materialId = -1;

        //if (_attachNode) 
        {
            // fresh _attachNode
            if (_attachNode->getNChildren() <= 0)
            {
            }
            else 
            {
                OSG::NodeCoreUnrecPtr nc = _attachNode->getChild(0)->getCore();
                OSG::OSG2_GeometryRecPtr geocore 
                = OSG::dynamic_pointer_cast <OSG::Geometry>(nc);
                updateQuadsIndicesInGroup( fg, geocore);

            }

        }
        return;
    }
    // number of groups the same
    OSG::NodeCoreUnrecPtr geonode;
    OSG::OSG2_GeometryRecPtr geocore; 
    for (unsigned int i=0; i < groups.size() ; ++i)
    {
          geonode = _attachNode->getChild(i)->getCore();
          OSG::OSG2_GeometryRecPtr geocore 
          = OSG::dynamic_pointer_cast <OSG::Geometry>(geonode);
          updateQuadsIndicesInGroup(this->groups.getValue()[i], geocore);
    }

}

// this is called if there are changes
void OSG2_Model::updateBuffers()
{

    // false if initVisual has not been called 
    if (!initDone) return;



    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    const ResizableExtVector<Quad>& quads = this->getQuads();
    const ResizableExtVector<Coord>& vertices = this->getVertices();

    // vertices are computed, not loaded from file
    // vertices have not been generated
    //if (!_polygonPoints || vertices.size() <= 0) 
    
    bool  createVertices = false;
    bool  createTriangles = false;
    bool  createQuads = false;

    if (!_polygonPoints) 
    {
        std::cerr  << "OSG2_Model : " << "FIRST TIME CREATE" << sendl; 
        createGroups();
        return;
    }
    if ( oldVerticesSize != vertices.size() )
    {
        createVertices = true;
    }
    if (useTriangles)
    {
        if (oldTrianglesSize != triangles.size())
            createTriangles = true;
    }
    else if (triangles.size() > 0)
        createTriangles = true;

    if (useQuads)
    {
        if (oldQuadsSize != quads.size())
            createQuads = true;
    }
    else if (quads.size() > 0)
        createQuads = true;

    if (createVertices || createTriangles || createQuads)
    {
        //serr << "OSG2_Model : " << "VERTEX SIZE has CHANGED" << sendl; 
        //serr << "OSG2_Model : " << "VERTEX SIZE " << vertices.size() <<  " " << oldVerticesSize << sendl; 
        //serr << "OSG2_Model : " << "QUAD SIZE " << quads.size() <<  " " << oldQuadsSize << sendl; 
        //serr << "OSG2_Model : " << "TRI SIZE " << triangles.size() << " " << oldTrianglesSize  << sendl; 
        createGroups();
        return;
    }


    updateVertices();
    updateNormals();
    updateTrianglesIndices();
    updateQuadsIndices();


    //OSG::commitChanges();
    
}

bool OSG2_Model::createCommon( void )
{

    const ResizableExtVector<Coord>& vertices = this->getVertices();
    if (vertices.size() <= 0) return false;

    const ResizableExtVector<Coord>& vnormals = this->getVnormals();
    const ResizableExtVector<TexCoord>& vtexcoords = this->getVtexcoords();
    const ResizableExtVector<Coord>& vtangents= this->getVtangents();
    const ResizableExtVector<Coord>& vbitangents= this->getVbitangents();
    bool hasTangents = vtangents.size() && vbitangents.size() && m_computeTangents.getValue();

    hasTangents = m_computeTangents.getValue();

    if (!_tangents)
    {
        if (hasTangents)
        {
            _tangents    =  OSG::GeoVec3fProperty::create();
            _biTangents    =  OSG::GeoVec3fProperty::create();
        }      
    }
    else
    {
        if (hasTangents)
        {
            _tangents->clear();
            _biTangents->clear();
        }      
    }
    /*
    // If using SOFA tangents
    if (hasTangents)
    {
        
        Deriv d;
        for (int i=0; i< vtangents.size(); ++i)
        {
            d = vtangents[i];
            _tangents->addValue(OSG::Vec4f(d[0],d[1],d[2],d[3]));
            d = vbitangents[i];
            _biTangents->addValue(OSG::Vec4f(d[0],d[1],d[2], d[3]));

        }
    }
    */

    

    if (!_polygonPoints) 
        _polygonPoints =  OSG::GeoPnt3fProperty::create();
    else
        _polygonPoints->clear();

    if (vnormals.size() <= 0) 
    {
        computeNormals();
    }

    if (!_vertexNormals)
    {
        if (vnormals.size() > 0)
            _vertexNormals =  OSG::GeoVec3fProperty::create();
    }
    else
        _vertexNormals->clear();

    if (!_texCoord2D)
    {
        if (vtexcoords.size() > 0)
        _texCoord2D    =  OSG::GeoPnt2fProperty::create();
    }
    else
        _texCoord2D->clear();

    Coord p;
    
    for (unsigned int i=0; i< vertices.size(); ++i)
    {
        p = vertices[i];
        _polygonPoints->addValue(OSG::Pnt3f(p[0],p[1],p[2]));
    }


    Deriv d;
    for (unsigned int i=0; i< vnormals.size(); ++i)
    {
        d = vnormals[i];
        _vertexNormals->addValue(OSG::Vec3f(d[0],d[1],d[2]));

    }
    TexCoord t;
    for (unsigned int i=0; i< vtexcoords.size(); ++i)
    {
        t = vtexcoords[i];
        _texCoord2D->addValue(OSG::Pnt2f(t[0],t[1]));

    }
    return true;
}

void OSG2_Model::createStateChunks( void )
{
    if (!_polygonChunk)
    {
        _polygonChunk = OSG::PolygonChunk::create();
        //_polygonChunk->setCullFace(GL_BACK);
        _polygonChunk->setCullFace(GL_NONE);
        
        // artifacts when this is true ????
        //_polygonChunk->setSmooth(true);
        //_polygonChunk->setSmooth(false);
            //_polygonChunk->setFrontMode(GL_LINE);
            //_polygonChunk->setBackMode(GL_LINE);
            //_polygonChunk->setBackMode(GL_POINT);
    }
    if (!_shadeModelChunk)
    {
        _shadeModelChunk = OSG::ShadeModelChunk::create();
        _shadeModelChunk->setShadeModel(GL_SMOOTH);
    }
    if (!_twoSidedLightingChunk)
    {
        _twoSidedLightingChunk = OSG::TwoSidedLightingChunk::create();
        _twoSidedLightingChunk->setIgnore(false);
//_twoSidedLightingChunk->setIgnore(true);
    }
}



OSG::NodeTransitPtr OSG2_Model::createGroup(const FaceGroup& fg, OSG::Geometry* geo)
{
    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    const ResizableExtVector<Quad>& quads = this->getQuads();
#if 0 // GV unused
    const ResizableExtVector<Coord>& vertices = this->getVertices();
#endif
    const ResizableExtVector<Coord>& vnormals = this->getVnormals();

#if 0 // GV unused
    const ResizableExtVector<TexCoord>& vtexcoords = this->getVtexcoords();
#endif

    OSG::OSG2_GeoUInt8PropertyRecPtr polyTypes;
    OSG::OSG2_GeoUInt32PropertyRecPtr polyLens;

    OSG::OSG2_GeoUInt32PropertyRecPtr polyIndices;
    OSG::OSG2_GeoUInt32PropertyRecPtr normIndices;

    OSG::OSG2_GeometryRecPtr geoCore;
    if (geo)
    {
        geoCore = geo;

        polyTypes = dynamic_cast<OSG::GeoUInt8Property*>(geoCore->getTypes());
        polyLens = dynamic_cast<OSG::GeoUInt32Property*>(geoCore->getLengths());

        polyTypes->clear();
        polyLens->clear();


        //polyIndices = geo->get();
        //normIndices = geo->getLengths();
        polyIndices = dynamic_cast<OSG::GeoUInt32Property*>(geoCore->getIndex( OSG::Geometry::PositionsIndex));
        normIndices = dynamic_cast<OSG::GeoUInt32Property*>(geoCore->getIndex( OSG::Geometry::NormalsIndex));
        polyIndices->clear();
        normIndices->clear();
    }
    else
    {
        geoCore = OSG::Geometry::create();

        polyTypes = OSG::GeoUInt8Property::create();
        polyLens  = OSG::GeoUInt32Property::create();
        polyIndices = OSG::GeoUInt32Property::create();
        normIndices = OSG::GeoUInt32Property::create();
    }

    int numTriInGroup = fg.nbt;
    int numQuadInGroup = fg.nbq;

    if (numTriInGroup > 0)
    {
        polyTypes->addValue(GL_TRIANGLES);
        polyLens->addValue(numTriInGroup * 3);
        int last = fg.tri0 + numTriInGroup;

        Triangle t;

        for (int i = fg.tri0; i < last; ++i)
        {
            t = triangles[i];
            polyIndices->push_back(t[0]);
            polyIndices->push_back(t[1]);
            polyIndices->push_back(t[2]);


        }
    }
    if (numQuadInGroup > 0)
    {
        polyTypes->addValue(GL_QUADS);
        //polyTypes->addValue(GL_LINES_ADJACENCY_EXT);
        polyLens->addValue(numQuadInGroup * 4);

        int last = fg.quad0 + numQuadInGroup;

        Quad q;

        for (int i = fg.quad0; i < last; ++i)
        {
            q = quads[i];
            polyIndices->push_back(q[0]);
            polyIndices->push_back(q[1]);
            polyIndices->push_back(q[2]);
            polyIndices->push_back(q[3]);
        }
    }


    if (!geo)
    {
        geoCore->setTypes(polyTypes);
        geoCore->setLengths(polyLens);

        geoCore->setProperty(_polygonPoints, OSG::Geometry::PositionsIndex);
        geoCore->setIndex   (polyIndices,   OSG::Geometry::PositionsIndex); 

        geoCore->setProperty(_vertexNormals,  OSG::Geometry::NormalsIndex  );
        geoCore->setIndex   (polyIndices,   OSG::Geometry::NormalsIndex); 
    }

    if (vnormals.size() <= 0) OSG::calcVertexNormals(geoCore);

    // has tex coords
    if (_texCoord2D)
    {
        //geoCore->setTexCoords(_texCoord2D);
        geoCore->setProperty(_texCoord2D,  OSG::Geometry::TexCoordsIndex  );
        geoCore->setIndex   (polyIndices,   OSG::Geometry::TexCoordsIndex); 
    }
    // has tangents
    // tangents in texture unit 1
    // bitangents in texture unit 2
    if (_tangents)
    {

        //geoCore->setTexCoords1(_tangents);
        geoCore->setProperty(_tangents,  OSG::Geometry::TexCoords1Index  );
        geoCore->setIndex   (polyIndices,   OSG::Geometry::TexCoords1Index); 

        //geoCore->setTexCoords2(_biTangents);
        geoCore->setProperty(_biTangents,  OSG::Geometry::TexCoords2Index  );
        geoCore->setIndex   (polyIndices,   OSG::Geometry::TexCoords2Index); 

        //OSG::calcVertexTangents(geoCore, OSG::Geometry::TexCoordsIndex, 
        //OSG::Geometry::TexCoords1Index, OSG::Geometry::TexCoords2Index); 

        OSG::calcVertexTangentsProp(geoCore, OSG::Geometry::TexCoordsIndex, 
        OSG::Geometry::NormalsIndex, 
        OSG::Geometry::TexCoords1Index, 
        OSG::Geometry::TexCoords2Index); 
    }


    Material mat;
    if (fg.materialId < 0) mat = this->material.getValue();
    else
        mat = this->materials.getValue()[fg.materialId];

    OSG::SimpleMaterialRecPtr simpleMaterial = OSG::SimpleMaterial::create();

    //OSG::ChunkMaterialRecPtr chunkMaterial = OSG::ChunkMaterial::create();
    //OSG::MaterialChunkRecPtr matChunk = OSG::MaterialChunk::create();
    
    Vec4f ambient = mat.useAmbient?mat.ambient:Vec4f();
    Vec4f diffuse = mat.useDiffuse?mat.diffuse:Vec4f();
    Vec4f specular = mat.useSpecular?mat.specular:Vec4f();
    Vec4f emissive = mat.useEmissive?mat.emissive:Vec4f();
    float shininess = mat.useShininess?mat.shininess:45;
    float transparency = 0.f;

    if (shininess <= 0.0f)
    {
        specular.clear();
        shininess = 1;
    }
    bool isTransparent = (mat.useDiffuse && mat.diffuse[3] < 1.0);

    if (isTransparent) 
    {
        emissive[3] = 0; 
        ambient[3] = 0; 
        specular[3] = 0;
        transparency = 1.f - mat.diffuse[3];
        //emissive.clear();
        //ambient.clear();
        //specular.clear();
    }

    //shininess /= 128.f;
    //shininess = 1.f;
    if (shininess > 128.f) shininess = 128.f;
    
    //matChunk->setDiffuse( OSG::Color4f(diffuse[0], diffuse[1], diffuse[2], diffuse[3]) );
    //matChunk->setAmbient( OSG::Color4f(ambient[0], ambient[1], ambient[2], ambient[3]) );
    //matChunk->setEmission( OSG::Color4f(emissive[0], emissive[1], emissive[2], emissive[3]) );
    //matChunk->setSpecular( OSG::Color4f(specular[0], specular[1], specular[2], specular[3]) );
    //matChunk->setShininess( shininess );
    //matChunk->setColorMaterial( GL_NONE );
    //matChunk->setLit(true);
    //chunkMaterial->addChunk( matChunk );
    

    simpleMaterial->setDiffuse(OSG::Color3f(diffuse[0],diffuse[1],diffuse[2]));
    simpleMaterial->setAmbient(OSG::Color3f(ambient[0],ambient[1],ambient[2]));
    simpleMaterial->setEmission(OSG::Color3f(emissive[0],emissive[1],emissive[2]));
    simpleMaterial->setSpecular(OSG::Color3f(specular[0],specular[1],specular[2]));
    simpleMaterial->setShininess(shininess);
    simpleMaterial->setTransparency(transparency);
    //std::cerr << "TRANSPARENCY : " << simpleMaterial->getTransparency() <<  "is: " << simpleMaterial->isTransparent() << std::endl;
    //simpleMaterial->setLit(true);
    simpleMaterial->setColorMaterial(GL_NONE);

    sofa::core::objectmodel::BaseContext* context = this->getContext();
    OSG2_Shader* shader = context->core::objectmodel::BaseContext::get<OSG2_Shader>();
    // ASSUME first shader first

    if (!_textureName && mat.useTexture && mat.activated)
    {
        int indexInTextureArray = materialTextureIdMap[fg.materialId];
        
        OSG::TextureEnvChunkRecPtr texEnv = OSG::TextureEnvChunk::create();
        texEnv->setEnvMode(GL_MODULATE);

        _texMaterial[indexInTextureArray]->setMinFilter(GL_LINEAR);
        _texMaterial[indexInTextureArray]->setMagFilter(GL_LINEAR);

        simpleMaterial->addChunk(_texMaterial[indexInTextureArray], 0);

        if (shader) shader->addToMaterial( 0, simpleMaterial );
        simpleMaterial->addChunk(texEnv);
        
        //chunkMaterial->addChunk(_texMaterial[indexInTextureArray]);
        //chunkMaterial->addChunk(texEnv);
    }
    else
    {

        if (_textureName) 
        {
            OSG::TextureEnvChunkRecPtr texEnv = OSG::TextureEnvChunk::create();
            texEnv->setEnvMode(GL_MODULATE);
            _textureName->setMinFilter(GL_LINEAR);
            _textureName->setMagFilter(GL_LINEAR);
            //texEnv->setEnvMode(GL_DECAL);
            serr << "OSG2_Model " << this->getName()  << " using _texture_name" << sendl;
            simpleMaterial->addChunk(_textureName, 0);
            if (shader) shader->addToMaterial( 0, simpleMaterial );
            simpleMaterial->addChunk(texEnv);

            //chunkMaterial->addChunk(_textureName);
            //chunkMaterial->addChunk(texEnv);
        }
        else
        {
            // Find attached shader
            if (shader)
            {
                //if (shader->getShaderTextureObjChunk(0))
            //serr << "OSG2_Model OglTexture found"  << sendl;
                OSG::TextureEnvChunkRecPtr texEnv = OSG::TextureEnvChunk::create();
                texEnv->setEnvMode(GL_MODULATE);
//serr << "OSG2_Model::addMaterial "  << shader  << sendl;
                shader->addToMaterial( 0, simpleMaterial );
                simpleMaterial->addChunk(texEnv);

                //chunkMaterial->addChunk( shader->getShaderTextureObjChunk(0), 1);
                //chunkMaterial->addChunk( shader->getShaderChunk(0) );
                //chunkMaterial->addChunk( shader->getShaderVariableChunk(0) );
            }
        }
                
    }

    if (_polygonChunk) simpleMaterial->addChunk(_polygonChunk);
    if (_shadeModelChunk) simpleMaterial->addChunk(_shadeModelChunk);
    if (_twoSidedLightingChunk) simpleMaterial->addChunk(_twoSidedLightingChunk);
    //if (_shaderChunk) simpleMaterial->addChunk(_shaderChunk);


    OSG::DepthChunkUnrecPtr depth = OSG::DepthChunk::create();
    depth->setEnable(true);
    depth->setReadOnly(false);
    simpleMaterial->addChunk(depth);

    //simpleMaterial->setSortKey(0);

    //OSG::VariantMaterialRecPtr variantMaterial = OSG::VariantMaterial::create();
    OSG::MultiPassMaterialRecPtr multiPassMaterial = OSG::MultiPassMaterial::create();

    if (isTransparent)
    {
        OSG::BlendChunkUnrecPtr blend = OSG::BlendChunk::create();
        simpleMaterial->addChunk(blend);

        OSG::SimpleMaterialRecPtr simpleMaterial2 
        = OSG::dynamic_pointer_cast<OSG::SimpleMaterial>(OSG::deepClone(simpleMaterial));

        
        // Material 1
        OSG::StateChunkUnrecPtr statechunk
        = simpleMaterial->find(OSG::BlendChunk::getClassType());
        OSG::BlendChunkUnrecPtr blendCur 
        = OSG::dynamic_pointer_cast <OSG::BlendChunk>(statechunk);
        if (blendCur) 
        {
            blendCur->setSrcFactor(GL_ZERO);
            blendCur->setDestFactor(GL_ONE_MINUS_SRC_ALPHA);
            blendCur->setEquation(GL_FUNC_ADD);
            blendCur->setIgnore(false);
        }
        statechunk
        = simpleMaterial->find(OSG::DepthChunk::getClassType());
        OSG::DepthChunkUnrecPtr depthCur
        = OSG::dynamic_pointer_cast <OSG::DepthChunk>(statechunk);
        if (depthCur)
        {
            //if (writeZTransparent.getValue())
            //    depthCur->setReadOnly(false);
            //else
            // disable depth write             
            depthCur->setReadOnly(true);

            // enable depth test
            depthCur->setEnable(true);

        }

        // Material 2
        statechunk
        = simpleMaterial2->find(OSG::BlendChunk::getClassType());
        blendCur 
        = OSG::dynamic_pointer_cast <OSG::BlendChunk>(statechunk);
        if (blendCur) 
        {
            if (premultipliedAlpha.getValue())
            {
                blendCur->setSrcFactor(GL_ONE);
                blendCur->setDestFactor(GL_ONE);
            }
            else
            {
                blendCur->setSrcFactor(GL_SRC_ALPHA);
                blendCur->setDestFactor(GL_ONE);
            }               
            blendCur->setEquation(GL_FUNC_ADD);
            blendCur->setIgnore(false);

            if (alphaBlend.getValue())
            {
                // depth mask false
                blendCur->setEquation( blendEq );
                blendCur->setSrcFactor( sfactor );
                blendCur->setDestFactor( dfactor );
               
            }

        }
        statechunk
        = simpleMaterial2->find(OSG::DepthChunk::getClassType());
        depthCur
        = OSG::dynamic_pointer_cast <OSG::DepthChunk>(statechunk);

        if (depthCur)
        {
            // disable depth testing
            if (!depthTest.getValue())
            {
                depthCur->setEnable(false);
            }
            else
            {
                std::cerr << "Depth Enable : " << this->getName() << std::endl;
                depthCur->setEnable(true);
            }


            depthCur->setReadOnly(true);
            //depthCur->setReadOnly(false);
            

            //if (alphaBlend.getValue())
            //{
            //    depthCur->setReadOnly(true);
            //}


        }

        //statechunk
        //= simpleMaterial2->find(OSG::PolygonChunk::getClassType());
        //OSG::PolygonChunkUnrecPtr  polygonCur
        //= OSG::dynamic_pointer_cast <OSG::PolygonChunk>(statechunk);

        //if (polygonCur) polygonCur->setFrontMode(GL_LINE);
        //if (polygonCur) polygonCur->setBackMode(GL_LINE);
        //if (!depth) system("pause");

        

        // simpleMaterial then simpleMaterial2
        //variantMaterial->addMaterial(simpleMaterial, 10);
        //variantMaterial->addMaterial(simpleMaterial2, 0);
        
        multiPassMaterial->addMaterial(simpleMaterial);
        multiPassMaterial->addMaterial(simpleMaterial2);
        multiPassMaterial->setTransparencyMode
        (OSG::Material::TransparencyForceTransparent);
        
        geoCore->setMaterial(multiPassMaterial);

    }
    else
    {

        geoCore->setMaterial(simpleMaterial);
    }

    
    //if (_polygonChunk) chunkMaterial->addChunk(_polygonChunk);
    //if (_shadeModelChunk) chunkMaterial->addChunk(_shadeModelChunk);
    //if (_twoSidedLightingChunk) chunkMaterial->addChunk(_twoSidedLightingChunk);
    //if (_shaderChunk) chunkMaterial->addChunk(_shaderChunk);
    //geoCore->setMaterial(chunkMaterial);


    if (geo) return OSG::NodeTransitPtr(NULL);

    OSG::OSG2_NodeRecPtr geo_node = OSG::makeNodeFor(geoCore);
    return OSG::NodeTransitPtr(geo_node);

}

void OSG2_Model::createGroups( void )
{
    if (!_attachNode) return;

    if (!createCommon()) return;
    //std::cerr << "OSG2_Model : going to create GROUPS" << std::endl;

    helper::ReadAccessor< Data< helper::vector<FaceGroup> > > groups = this->groups;

    const ResizableExtVector<Triangle>& triangles = this->getTriangles();
    const ResizableExtVector<Quad>& quads = this->getQuads();
    const ResizableExtVector<Coord>& vertices = this->getVertices();

    useQuads = useTriangles = false;

    if (quads.size() > 0) useQuads = true;
    if (triangles.size() > 0) useTriangles = true;

    oldVerticesSize = vertices.size();
    oldTrianglesSize = triangles.size();
    oldQuadsSize  = quads.size();

   
    if (groups.empty())
    {

//serr << "OSG2_Model:: " << this->getName()  << " createGroups Single"  << sendl;

        FaceGroup fg;
        fg.nbt = triangles.size();
        fg.nbq = quads.size();
        fg.tri0 = 0;
        fg.quad0 = 0;
        fg.materialId = -1;

        //if (_attachNode) 
        {
            // fresh _attachNode
            if (_attachNode->getNChildren() <= 0)
            {
                OSG::OSG2_NodeRecPtr node = createGroup( fg);
                _attachNode->addChild(node);
            }
            else 
            {
                OSG::NodeCoreUnrecPtr nc = _attachNode->getChild(0)->getCore();
                OSG::OSG2_GeometryRecPtr geocore 
                = OSG::dynamic_pointer_cast <OSG::Geometry>(nc);
                OSG::OSG2_NodeRecPtr node = createGroup( fg, geocore);

                // should not be true
                if (node)
                    _attachNode->replaceChild(0, node);    
            }

        }

        return;
    }
    // number of groups have change.
    // to reuse or not reuse
    if (_attachNode->getNChildren() != groups.size())
    {
        _attachNode->clearChildren();
        for (unsigned int i=0; i < groups.size() ; ++i)
        {
            OSG::OSG2_NodeRecPtr node = createGroup( this->groups.getValue()[i]);
            _attachNode->addChild(node);
        }
        return;

    }
    // number of groups the same
    OSG::NodeCoreUnrecPtr geonode;
    OSG::OSG2_GeometryRecPtr geocore; 
    for (unsigned int i=0; i < groups.size() ; ++i)
    {
          geonode = _attachNode->getChild(i)->getCore();
          OSG::OSG2_GeometryRecPtr geocore 
          = OSG::dynamic_pointer_cast <OSG::Geometry>(geonode);
          OSG::OSG2_NodeRecPtr node = createGroup(this->groups.getValue()[i], geocore);

          // should not be true
          if (node)
            _attachNode->replaceChild(0, node);    
    }

    
}


GLenum OSG2_Model::getGLenum(const char* c ) const
{

    if ( strcmp( c, "GL_ZERO") == 0)
    {
        return GL_ZERO;
    }
    else if  ( strcmp( c, "GL_ONE") == 0)
    {
        return GL_ONE;
    }
    else if (strcmp( c, "GL_SRC_ALPHA") == 0 )
    {
        return GL_SRC_ALPHA;
    }
    else if (strcmp( c, "GL_ONE_MINUS_SRC_ALPHA") == 0 )
    {
        return GL_ONE_MINUS_SRC_ALPHA;
    }
    // .... add ohter OGL symbolic constants
    // glBlendEquation Value
    else if  ( strcmp( c, "GL_FUNC_ADD") == 0)
    {
        return GL_FUNC_ADD;
    }
    else if (strcmp( c, "GL_FUNC_SUBTRACT") == 0 )
    {
        return GL_FUNC_SUBTRACT;
    }
    else if (strcmp( c, "GL_MAX") == 0 )
    {
        return GL_MAX;
    }
    else if (strcmp( c, "GL_MIN") == 0 )
    {
        return GL_MIN;
    }

    else
    {
        // error: not valid
        std::cerr   << " OSG2_Model - not valid or not supported openGL enum value: " << c ;
        return GL_ZERO;
    }


}

OSG::Action::ResultE enter(OSG::Node* node)
{
    SLOG << "entering " << node << OSG::endLog;
    return OSG::Action::Continue;
}

void OSG2_Model::updateOptions()
{
    if (!_attachNode) return;
    traverse(_attachNode, enter);
    
}


} // namespace visualmodel

} // namespace component

} // namespace sofa

