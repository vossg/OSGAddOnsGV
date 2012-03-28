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

//Work in Progress - Visualization not working

#include "OSGSofaPointSplatModel.h"
#include <map>
#include <sofa/helper/gl/template.h>
#include <sofa/core/ObjectFactory.h>

#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/behavior/BaseMechanicalState.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/core/topology/TopologyChange.h>

#include <sofa/core/loader/VoxelLoader.h>

#include <sofa/core/visual/VisualParams.h>

#include <sofa/component/topology/TopologyData.inl>

#include "OSGImage.h"

namespace sofa
{

namespace component
{

namespace visualmodel
{
  
SOFA_DECL_CLASS(OSGPointSplatModel)

int PointSplatModelClass = core::RegisterObject("A simple visualization for a cloud of points.")
.add< OSGPointSplatModel >()
//.addAlias("PointSplat")
;

using namespace sofa::defaulttype;
using namespace sofa::core::topology;

OSGPointSplatModel::OSGPointSplatModel() //const std::string &name, std::string filename, std::string loader, std::string textureName)
: radius(initData(&radius, 1.0f, "radius", "Radius of the spheres.")),
textureSize(initData(&textureSize, 32, "textureSize", "Size of the billboard texture.")),
alpha(initData(&alpha, 1.0f, "alpha", "Opacity of the billboards. 1.0 is 100% opaque.")),
color(initData(&color, std::string("white"), "color", "Billboard color.")),
_topology(NULL),
_mstate(NULL),
texture_data(NULL),
pointData(initData(&pointData, "pointData", "scalar field modulating point colors"))
,_particles(NULL)
,_particlePoints(NULL)
,_texture(NULL)
,_simpleTexMaterial(NULL)
,_attachNode(NULL)
{
    std::cerr << "OSGPointSplateModel::CONSTRUCTOR" << std::endl;
}

OSGPointSplatModel::~OSGPointSplatModel()
{
    if(texture_data != NULL)
        delete [] texture_data;

    _particles = NULL;
    _particlePoints = NULL;
    _simpleTexMaterial = NULL;
}

void OSGPointSplatModel::init()
{
    getContext()->get(_topology);
    if(_topology)
        _mstate = dynamic_cast<core::behavior::BaseMechanicalState*>(_topology->getContext()->getMechanicalState());
    else
        getContext()->get(_mstate);

    VisualModel::init();

    core::loader::VoxelLoader *loader;
    getContext()->get(loader);

    if (_topology) std::cerr << "OSGPointSplatModel topology " << std::endl;
    if (loader) std::cerr << "OSGPointSplatModel loader " << std::endl;
    if (_mstate) std::cerr << "OSGPointSplatModel mstate " << std::endl;
    if(loader && _mstate)
    {
    std::cerr << "Splat::init" << std::endl;
        unsigned int nbPoints = _mstate->getSize();

        const helper::vector<unsigned int> idxInRegularGrid = loader->getHexaIndicesInGrid();


        if(idxInRegularGrid.size() == nbPoints)
        {
::system("pause");
            const unsigned char *imageData = loader->getData();

            helper::vector<unsigned char>* pData = pointData.beginEdit();
            for(unsigned int i=0; i<nbPoints; ++i)
                (*pData).push_back(imageData[idxInRegularGrid[i]]);
            pointData.endEdit();


            

        }
    }

    std::cerr << "Splat::To reinit" << std::endl;

    reinit();

    updateVisual();
}

void OSGPointSplatModel::reinit()
{
    if(texture_data != NULL)
        delete [] texture_data;

    unsigned int texture_size = textureSize.getValue();
    unsigned int half_texture_size = texture_size >> 1;
    texture_data = new unsigned char [texture_size * texture_size];

    for(unsigned int i=0; i<half_texture_size; ++i)
        for(unsigned int j=0; j<half_texture_size; ++j)
        {
            const float x = i / (float) half_texture_size;
            const float y = j / (float) half_texture_size;
            const float dist = sqrt(x*x + y*y);
            const float value = cos(3.141592f/2.0f * dist);
            const unsigned char texValue = (value < 0.0f) ? 0 : (unsigned char) (255.0f * alpha.getValue() * value);

            texture_data[half_texture_size + i + (half_texture_size + j) * texture_size] = texValue;
            texture_data[half_texture_size + i + (half_texture_size - 1 - j) * texture_size] = texValue;
            texture_data[half_texture_size - 1 - i + (half_texture_size + j) * texture_size] = texValue;
            texture_data[half_texture_size - 1 - i + (half_texture_size - 1 - j) * texture_size] = texValue;
        }



    OSG::ImageUnrecPtr image = OSG::Image::create();
    image->set(OSG::Image::OSG_A_PF, texture_size, texture_size,1,
               1,1,0.0, texture_data, OSG::Image::OSG_UINT8_IMAGEDATA);

    if (!_texture) _texture = OSG::TextureObjChunk::create();
    _texture->setImage(image);
    _texture->setWrapS(GL_REPEAT);
    _texture->setWrapT(GL_REPEAT);
    _texture->setMagFilter(GL_LINEAR);
    _texture->setMinFilter(GL_LINEAR);

    if (!_particles) _particles = OSG::Particles::create();

    //if (!_simpleTexMaterial) _simpleTexMaterial = OSG::SimpleTexturedMaterial::create();
    if (!_simpleTexMaterial) _simpleTexMaterial = OSG::SimpleMaterial::create();
    _simpleTexMaterial->setLit(false);

    OSG::TextureEnvChunkUnrecPtr texEnv = OSG::TextureEnvChunk::create();
    texEnv->setEnvMode(GL_REPLACE);


    OSG::BlendChunkUnrecPtr blend = OSG::BlendChunk::create();
    blend->setSrcFactor(GL_SRC_ALPHA);
    blend->setDestFactor(GL_ONE_MINUS_SRC_ALPHA);

    _simpleTexMaterial->addChunk(_texture);
    _simpleTexMaterial->addChunk(texEnv);
    //_simpleTexMaterial->addChunk(blend);

    //_simpleTexMaterial->setEnvMode(GL_REPLACE);
    //_simpleTexMaterial->setMinFilter(GL_LINEAR);
    //_simpleTexMaterial->setMagFilter(GL_LINEAR);
    //_simpleTexMaterial->setImage(image);

    _particles->setMaterial(_simpleTexMaterial);

    /*
    int slot(0);
    OSG::StateChunkUnrecPtr statechunk = _simpleTexMaterial->find( OSG::TextureObjChunk::getClassType(), slot);
    OSG::TextureObjChunkUnrecPtr tex  = 
    OSG::dynamic_pointer_cast <OSG::TextureObjChunk>(statechunk);


    if (tex)
    {
::system("pause");
        tex->setWrapS(GL_REPEAT);
        tex->setWrapT(GL_REPEAT);
    }
    statechunk = _simpleTexMaterial->find( OSG::BlendChunk::getClassType(), slot);
    OSG::BlendChunkUnrecPtr blender  = 
    OSG::dynamic_pointer_cast <OSG::BlendChunk>(statechunk);
    if (blender)
    {
::system("pause");
        blender->setSrcFactor(GL_SRC_ALPHA);
        blender->setDestFactor(GL_ONE_MINUS_SRC_ALPHA);
    }
    */
    setColor(color.getValue());

    //_simpleTexMaterial->setColorMaterial(GL_AMBIENT_AND_DIFFUSE);
    //_simpleTexMaterial->setDiffuse(OSG::Color3f(this->r, this->g, this->b));
    _simpleTexMaterial->setDiffuse(OSG::Color3f(1,1,1));

    //if (this->a > 0.01f) _simpleTexMaterial->setTransparency(1.f - a);


    _attachNode = OSG::makeNodeFor(_particles);
    


}

void OSGPointSplatModel::initVisual()
{

            //_particles =  OSG::Particles::create();
            _particlePoints =  OSG::GeoPnt3fProperty::create();
/*
    _particlePoints->addValue( OSG::Pnt3f( -.5, -.5, -0.5) );
    _particlePoints->addValue( OSG::Pnt3f(  .5, -.5, -0.5) );
    _particlePoints->addValue( OSG::Pnt3f(  .5,  .5, -0.5) );
    _particlePoints->addValue( OSG::Pnt3f( -.5,  .5, -0.5) );
    _particlePoints->addValue( OSG::Pnt3f( -.5, -.5,  0.5) );
    _particlePoints->addValue( OSG::Pnt3f(  .5, -.5,  0.5) );
    _particlePoints->addValue( OSG::Pnt3f(  .5,  .5,  0.5) );
    _particlePoints->addValue( OSG::Pnt3f( -.5,  .5,  0.5) );
*/
     unsigned int nbPoints = _mstate->getSize();
 std::cerr << "Splat::initVisual " << nbPoints << std::endl;
            for (unsigned int i=0; i< nbPoints ;i++)
            {
                _particlePoints->addValue( OSG::Pnt3f(
                                           (Real)_mstate->getPX(i),
                                           (Real)_mstate->getPY(i),
                                           (Real)_mstate->getPZ(i)) );
            }
    _particleColors = OSG::GeoColor3fProperty::create();

    OSG::Color3f color(this->r, this->g, this->b);
    _particleColors->addValue(color);

    //_particleColors->addValue(OSG::Color3f(255, 255, 255));
    //_particleColors->addValue(OSG::Color3f(0, 255, 255));
    //_particleColors->addValue(OSG::Color3f(255, 0, 255));
    //_particleColors->addValue(OSG::Color3f(255, 255, 0));
    //_particleColors->addValue(OSG::Color3f(255, 0, 0));
    //_particleColors->addValue(OSG::Color3f(0, 255, 0));
    //_particleColors->addValue(OSG::Color3f(255, 0, 0));
    //_particleColors->addValue(OSG::Color3f(0, 255, 0));


    _particles->setColors(_particleColors);

    OSG::MFVec3f* size = const_cast<OSG::MFVec3f*>(_particles->getMFSizes());
    size->push_back(OSG::Vec3f(50,50,50));




            _particles->setPositions(_particlePoints);
            _particles->setMode(OSG::Particles::ViewerQuads);
            //_particles->setMode(OSG::Particles::ModeE::Points);
}

void OSGPointSplatModel::updateVisual()
{

//return;

    unsigned int nbPoints = _mstate->getSize();

std::cerr << "Splat::updateVisual " << nbPoints << std::endl;

    _particlePoints = NULL;
    _particlePoints = OSG::GeoPnt3fProperty::create();

        

    for (unsigned int i=0; i< nbPoints ;i++)
    {
        _particlePoints->addValue( OSG::Pnt3f(
                                   (Real)_mstate->getPX(i),
                                   (Real)_mstate->getPY(i),
                                   (Real)_mstate->getPZ(i)) );
    }
    _particles->setPositions(_particlePoints);

}

void OSGPointSplatModel::drawTransparent(const core::visual::VisualParams* vparams)
{
return;
    std::cerr << "DRAWTRANSPARENT POINTSPLAT" << std::endl;    

    if(!vparams->displayFlags().getShowVisualModels()) return;

    const helper::vector<unsigned char>& pData = pointData.getValue();

    glPushAttrib(GL_ENABLE_BIT);

    glDisable(GL_LIGHTING);

//  glClearColor (0, 0, 0, 0);
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glEnable (GL_TEXTURE_2D);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
//  glDepthMask(GL_FALSE);
//  glDisable(GL_DEPTH_TEST);

    GLuint  texture;
    unsigned int texture_size = (unsigned) textureSize.getValue();

    glGenTextures (1, &texture);
    glBindTexture (GL_TEXTURE_2D, texture);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_ALPHA, texture_size, texture_size, 0,
        GL_ALPHA, GL_UNSIGNED_BYTE, texture_data);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    VecCoord    vertices;

    if (_mstate)
    {
        vertices.resize(_mstate->getSize());

        for (unsigned int i=0;i<vertices.size();i++)
        {
            vertices[i][0] = (Real)_mstate->getPX(i);
            vertices[i][1] = (Real)_mstate->getPY(i);
            vertices[i][2] = (Real)_mstate->getPZ(i);
        }
    }


    glPushMatrix ();

    float mat[16];
    glGetFloatv( GL_MODELVIEW_MATRIX, mat );

    Coord vRight( mat[0], mat[4], mat[8] );
    Coord vUp( mat[1], mat[5], mat[9] );
    Coord vView(vRight.cross(vUp));

    std::multimap< Real, unsigned int, std::less<Real> >    mapPnt;

    for(unsigned int i=0; i<vertices.size(); ++i)
    {
        const Coord&    vertex = vertices[i];

        Real dist(vertex * vView);

        mapPnt.insert(std::pair<Real, unsigned int>(dist, i));
    }
    
    for(std::multimap<Real, unsigned int>::const_iterator it = mapPnt.begin();
        it != mapPnt.end(); ++it)
    {
        int i = (*it).second;

        const Coord& center = vertices[i];

        const float qsize = radius.getValue();

        // Now, build a quad around the center point based on the vRight 
        // and vUp vectors. This will guarantee that the quad will be 
        // orthogonal to the view.
        Coord vPoint0(center + ((-vRight - vUp) * qsize));
        Coord vPoint1(center + (( vRight - vUp) * qsize));
        Coord vPoint2(center + (( vRight + vUp) * qsize));
        Coord vPoint3(center + ((-vRight + vUp) * qsize));
    
        // TODO: modulate color by data
        float m = 1.0f;

        if(!pData.empty())
        {
            m = 0.5f + 2.0f * pData[i] / 255.0f;
        }

        glColor4f (m*r, m*g, m*b, a);

        glBegin( GL_QUADS );
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f((float) vPoint0[0], (float) vPoint0[1], (float) vPoint0[2]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f((float) vPoint1[0], (float) vPoint1[1], (float) vPoint1[2]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f((float) vPoint2[0], (float) vPoint2[1], (float) vPoint2[2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f((float) vPoint3[0], (float) vPoint3[1], (float) vPoint3[2]);
        glEnd();
    }

    // restore the previously stored modelview matrix
    glPopMatrix ();

    // glDepthMask(GL_TRUE);
    // glEnable(GL_DEPTH_TEST);

    glPopAttrib();
}

//void OSGPointSplatModel::handleTopologyChange() 
//{
//  std::list<const TopologyChange *>::const_iterator itBegin=_topology->beginChange();
//  std::list<const TopologyChange *>::const_iterator itEnd=_topology->endChange();
//
//  while( itBegin != itEnd )
//  {
//      core::topology::TopologyChangeType changeType = (*itBegin)->getChangeType();
//
//      switch( changeType )
//      {
//          case core::topology::POINTSREMOVED:
//          {
//
//              break;
//          }
//
//          default:
//              break;
//      }; // switch( changeType )
//
//      ++itBegin;
//  } // while( changeIt != last; )
//
//}

void OSGPointSplatModel::setColor(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

static int hexval(char c)
{
    if (c>='0' && c<='9') return c-'0';
    else if (c>='a' && c<='f') return (c-'a')+10;
    else if (c>='A' && c<='F') return (c-'A')+10;
    else return 0;
}

void OSGPointSplatModel::setColor(std::string color)
{
    if (color.empty()) return;
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
    if (color[0]>='0' && color[0]<='9')
    {
        sscanf(color.c_str(),"%f %f %f %f", &r, &g, &b, &a);
    }
    else if (color[0]=='#' && color.length()>=7)
    {
        r = (hexval(color[1])*16+hexval(color[2]))/255.0f;
        g = (hexval(color[3])*16+hexval(color[4]))/255.0f;
        b = (hexval(color[5])*16+hexval(color[6]))/255.0f;
        if (color.length()>=9)
            a = (hexval(color[7])*16+hexval(color[8]))/255.0f;
    }
    else if (color[0]=='#' && color.length()>=4)
    {
        r = (hexval(color[1])*17)/255.0f;
        g = (hexval(color[2])*17)/255.0f;
        b = (hexval(color[3])*17)/255.0f;
        if (color.length()>=5)
            a = (hexval(color[4])*17)/255.0f;
    }
    else if (color == "white")    { r = 1.0f; g = 1.0f; b = 1.0f; }
    else if (color == "black")    { r = 0.0f; g = 0.0f; b = 0.0f; }
    else if (color == "red")      { r = 1.0f; g = 0.0f; b = 0.0f; }
    else if (color == "green")    { r = 0.0f; g = 1.0f; b = 0.0f; }
    else if (color == "blue")     { r = 0.0f; g = 0.0f; b = 1.0f; }
    else if (color == "cyan")     { r = 0.0f; g = 1.0f; b = 1.0f; }
    else if (color == "magenta")  { r = 1.0f; g = 0.0f; b = 1.0f; }
    else if (color == "yellow")   { r = 1.0f; g = 1.0f; b = 0.0f; }
    else if (color == "gray")     { r = 0.5f; g = 0.5f; b = 0.5f; }
    else
    {
        serr << "Unknown color "<<color<<sendl;
        return;
    }
    setColor(r,g,b,a);
}

} // namespace visualmodel

} // namespace component

} // namespace sofa

