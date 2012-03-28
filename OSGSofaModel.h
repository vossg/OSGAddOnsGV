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

#ifndef _OSGSOFAMODEL_H_
#define _OSGSOFAMODEL_H_

#include <vector>
#include <string>
#include <sofa/helper/gl/template.h>
#include <sofa/helper/gl/Texture.h>
#include <sofa/helper/OptionsGroup.h>
#include <sofa/core/visual/VisualModel.h>
#include <sofa/component/component.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/component/visualmodel/VisualModelImpl.h>

#include "OSGContribSOFADef.h"
#include "OSGTransform.h"
#include "OSGGeometry.h"
#include "OSGSwitch.h"
#include "OSGGeoProperties.h"
#include "OSGSimpleMaterial.h"
#include "OSGVariantMaterial.h"
#include "OSGMultiPassMaterial.h"

//#include "OSGSimpleTexturedMaterial.h"
//#include "OSGChunkMaterial.h"

#include "OSGPolygonChunk.h"
#include "OSGShadeModelChunk.h"
#include "OSGTwoSidedLightingChunk.h"
#include "OSGClipPlaneChunk.h"

#include "OSGTextureEnvChunk.h"
#include "OSGTextureObjChunk.h"
#include "OSGShaderProgramChunk.h"
#include "OSGImage.h"

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

//#define OSG2_SOFA_STANDALONE

#ifndef OSG2_SOFA_SIMU_THREAD

#define OSG2_NodeRecPtr NodeUnrecPtr
#define OSG2_GeoPnt3fPropertyRecPtr GeoPnt3fPropertyUnrecPtr 
#define OSG2_GeoVec3fPropertyRecPtr GeoVec3fPropertyUnrecPtr  
#define OSG2_GeoPnt2fPropertyRecPtr GeoPnt2fPropertyUnrecPtr
#define OSG2_GeometryRecPtr         GeometryUnrecPtr
#define OSG2_GeoUInt8PropertyRecPtr GeoUInt8PropertyUnrecPtr
#define OSG2_GeoUInt32PropertyRecPtr GeoUInt32PropertyUnrecPtr

typedef OSG::NodeUnrecPtr OSGSofaNodePtr;
typedef OSG::GeoPnt3fPropertyUnrecPtr OSGSofaGeoPnt3fPropertyPtr;
typedef OSG::GeoVec3fPropertyUnrecPtr OSGSofaGeoVec3fPropertyPtr;
typedef OSG::GeoPnt2fPropertyUnrecPtr OSGSofaGeoPnt2fPropertyPtr;
typedef OSG::GeometryUnrecPtr OSGSofaGeometryPtr;
typedef OSG::GeoUInt8PropertyUnrecPtr OSGSofaGeoUInt8PropertyPtr;
typedef OSG::GeoUInt32PropertyUnrecPtr OSGSofaGeoUInt32PropertyPtr;


#else

#define OSG2_NodeRecPtr NodeMTRecPtr
#define OSG2_GeoPnt3fPropertyRecPtr GeoPnt3fPropertyMTRecPtr 
#define OSG2_GeoVec3fPropertyRecPtr GeoVec3fPropertyMTRecPtr  
#define OSG2_GeoPnt2fPropertyRecPtr GeoPnt2fPropertyMTRecPtr
#define OSG2_GeometryRecPtr         GeometryMTRecPtr
#define OSG2_GeoUInt8PropertyRecPtr GeoUInt8PropertyMTRecPtr
#define OSG2_GeoUInt32PropertyRecPtr GeoUInt32PropertyMTRecPtr

typedef OSG::NodeMTRecPtr OSGSofaNodePtr;
typedef OSG::GeoPnt3fPropertyMTRecPtr OSGSofaGeoPnt3fPropertyPtr;
typedef OSG::GeoVec3fPropertyMTRecPtr OSGSofaGeoVec3fPropertyPtr;
typedef OSG::GeoPnt2fPropertyMTRecPtr OSGSofaGeoPnt2fPropertyPtr;
typedef OSG::GeometryMTRecPtr OSGSofaGeometryPtr;
typedef OSG::GeoUInt8PropertyMTRecPtr OSGSofaGeoUInt8PropertyPtr;
typedef OSG::GeoUInt32PropertyMTRecPtr OSGSofaGeoUInt32PropertyPtr;

#endif

//#define   NB_MAX_TEXTURES 16
 
BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

/**
 *  \brief Main class for rendering 3D model in SOFA.
 *
 *  This class implements VisuelModelImpl with rendering functions
 *  using OpenSG 2.0
 *
 */
class OSG_CONTRIBSOFA_DLLMAPPING OSGSofaModel : public VisualModelImpl
{
  public:
    SOFA_CLASS(OSGSofaModel, VisualModelImpl);

  protected:
    Data<bool> premultipliedAlpha, useVBO, writeZTransparent, alphaBlend, depthTest;
    Data<int> cullFace;
    
    //alpha blend function
    Data<sofa::helper::OptionsGroup> blendEquation;
    Data<sofa::helper::OptionsGroup> sourceFactor;
    Data<sofa::helper::OptionsGroup> destFactor;
    GLenum blendEq, sfactor, dfactor;

    // Open SG
    OSGSofaNodePtr _attachNode;
    OSG::SwitchUnrecPtr _switchCore;

    // shared across aspects
    OSGSofaGeoPnt3fPropertyPtr  _polygonPoints;
    OSGSofaGeoVec3fPropertyPtr  _vertexNormals;

    OSGSofaGeoPnt2fPropertyPtr  _texCoord2D;
    OSGSofaGeoVec3fPropertyPtr  _tangents;
    OSGSofaGeoVec3fPropertyPtr  _biTangents;
    bool _bTangents;
    

    // Chunks
    OSG::TextureObjChunkUnrecPtr _textureName;
    std::vector<OSG::TextureObjChunkUnrecPtr> _texMaterial;
    OSG::PolygonChunkUnrecPtr _polygonChunk;
    OSG::ShadeModelChunkUnrecPtr _shadeModelChunk;
    OSG::TwoSidedLightingChunkUnrecPtr _twoSidedLightingChunk;

    //OSG::ClipPlaneChunkUnrecPtr _clipPlaneChunk[6];



    helper::gl::Texture *tex; //this texture is used only if a texture name is specified in the scn

    bool canUseVBO, VBOGenDone, initDone, useTriangles, useQuads;
    unsigned int oldVerticesSize, oldTrianglesSize, oldQuadsSize;
    void internalDraw(const core::visual::VisualParams* vparams, bool transparent);

    void drawGroup(int ig, bool transparent);
    void drawGroups(bool transparent);


    // OpenSG

    bool createCommon( void );
    void createStateChunks( void );
    OSG::NodeTransitPtr createGroup( const FaceGroup& fg ,OSG::Geometry* geo= NULL);
    void createGroups( void );

    //virtual void pushTransformMatrix(float* matrix) { glPushMatrix(); glMultMatrixf(matrix); }
    //virtual void popTransformMatrix() { glPopMatrix(); }


    std::map<int, int> materialTextureIdMap; //link between a material and a texture


    GLenum getGLenum(const char* c ) const;

    OSG::Image::PixelFormat convertSofaToOpenSG(helper::io::Image::ChannelFormat cf);
    OSG::Image::Type   convertSofaToOpenSG(helper::io::Image::DataType dt);

    OSGSofaModel();

    ~OSGSofaModel();
  public:

    bool loadTexture(const std::string& filename);
    bool loadTextures() ;

    virtual void initVisual();

    virtual void init() { VisualModelImpl::init(); }

    //virtual void computeMesh();

    virtual void updateBuffers();

    void updateOptions(); 

    void createSubgraph();
    void updateVertices(); 
    void updateNormals(); 
    void updateTrianglesIndicesInGroup(const FaceGroup& fg, OSG::Geometry* geo);
    void updateTrianglesIndices();
    void updateQuadsIndicesInGroup(const FaceGroup& fg, OSG::Geometry* geo);
    void updateQuadsIndices();


  public:
    bool isUseTriangles()   { return useTriangles; }
    bool isUseQuads()   { return useQuads; }
    bool isUseVbo() { return useVBO.getValue(); }

    //helper::gl::Texture* getTex() const   { return tex; }

    // OpenSG
    OSG::Node* getAttachNode() const { return _attachNode; }



};

typedef sofa::defaulttype::Vec<3,GLfloat> GLVec3f;
typedef sofa::defaulttype::ExtVectorTypes<GLVec3f,GLVec3f> GLExtVec3fTypes;

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFAMODEL_H_
