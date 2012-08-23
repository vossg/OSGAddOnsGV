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

#include "OSGSofaTexture.h"
#include <sofa/core/visual/VisualParams.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/system/FileRepository.h>

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

SOFA_DECL_CLASS(OSGTexture)
SOFA_DECL_CLASS(OSGTexture2D)

// Register the OSGTexture* objects in the Object Factory
namespace
{
    int OSGTextureClass   = 
        core::RegisterObject("OSGTexture"  ).add<OSGTexture  >();
    
    int OSGTexture2DClass = 
        core::RegisterObject("OSGTexture2D").add<OSGTexture2D>();

    OSG::Image::PixelFormat convertSofaToOpenSG(
        helper::io::Image::ChannelFormat cf)
    {
        OSG::Image::PixelFormat pixelFormat = OSG::Image::OSG_INVALID_PF;

        switch(cf)
        {
            case helper::io::Image::L :   
                pixelFormat = OSG::Image::OSG_L_PF; 
                break;

            case helper::io::Image::LA :   
                pixelFormat = OSG::Image::OSG_LA_PF; 
                break;

            case helper::io::Image::R :   
                pixelFormat = OSG::Image::OSG_L_PF; 
                break;

            case helper::io::Image::RG :   
                pixelFormat = OSG::Image::OSG_LA_PF; 
                break;

            case helper::io::Image::RGB :   
                pixelFormat = OSG::Image::OSG_RGB_PF; 
                break;

            case helper::io::Image::RGBA :   
                pixelFormat = OSG::Image::OSG_RGBA_PF; 
                break;

            case helper::io::Image::BGR :   
                pixelFormat = OSG::Image::OSG_BGR_PF; 
                break;

            case helper::io::Image::BGRA :   
                pixelFormat = OSG::Image::OSG_BGRA_PF; 
                break;

            default: 
                break;
        };

        return pixelFormat;
    }

    OSG::Image::Type convertSofaToOpenSG(helper::io::Image::DataType dt)
    {
        OSG::Image::Type type = OSG::Image::OSG_INVALID_IMAGEDATATYPE;

        switch(dt)
        {
            case helper::io::Image::UNORM8 :   
                type = OSG::Image::OSG_UINT8_IMAGEDATA; 
                break;

            case helper::io::Image::UNORM16 :   
                type = OSG::Image::OSG_UINT16_IMAGEDATA; 
                break;

            case helper::io::Image::UINT32 :   
                type = OSG::Image::OSG_UINT32_IMAGEDATA; 
                break;

            case helper::io::Image::HALF :   
                type = OSG::Image::OSG_FLOAT16_IMAGEDATA; 
                break;

            case helper::io::Image::FLOAT :   
                type = OSG::Image::OSG_FLOAT32_IMAGEDATA; 
                break;

//            case helper::io::Image::UCOMPRESSED :   
//                type = OSG::Image::OSG_UINT8_IMAGEDATA; 
//                break;

            default: 
                break;
        };

        return type;
    }
}

unsigned short OSGTexture::MAX_NUMBER_OF_TEXTURE_UNIT = 1;

OSGTexture::OSGTexture(void) :

    // 3rd parameter in initData is a keyword in scn files!
    // do not change!
    textureUnit            (
        initData(&textureUnit, 
                  1, 
                  "textureUnit", 
                  "Set the texture unit")),

    enabled                (
        initData(&enabled, 
                  bool(true), 
                  "enabled", 
                  "enabled ?")),

    repeat                 (
        initData(&repeat, 
                  bool(false),
                  "repeat", 
                  "Repeat Texture ?")),

    linearInterpolation    (
        initData(&linearInterpolation, 
                  bool(true),
                  "linearInterpolation", 
                  "Interpolate Texture ?")),

    generateMipmaps        (
        initData(&generateMipmaps, 
                  bool(false), 
                  "generateMipmaps", 
                  "Generate mipmaps ?")),

    srgbColorspace         (
        initData(&srgbColorspace, 
                  bool(false),
                  "srgbColorspace", 
                  "SRGB colorspace ?")),

    minLod                 (
        initData(&minLod, 
                  float(-1000.f), 
                  "minLod", 
                  "Minimum mipmap lod ?")),

    maxLod                 (
        initData(&maxLod, 
                  float(1000.f), 
                  "maxLod", 
                  "Maximum mipmap lod ?")),

    proceduralTextureWidth (
        initData(&proceduralTextureWidth, 
                  uint32_t(0), 
                  "proceduralTextureWidth", 
                  "Width of procedural Texture")),

    proceduralTextureHeight(
        initData(&proceduralTextureHeight, 
                  uint32_t(0), 
                  "proceduralTextureHeight", 
                  "Height of procedural Texture")),

    proceduralTextureNbBits(
        initData(&proceduralTextureNbBits, 
                  uint32_t(1), 
                  "proceduralTextureNbBits", 
                  "Nb bits per color")),

    proceduralTextureData  (
        initData(&proceduralTextureData, 
                  "proceduralTextureData", 
                  "Data of procedural Texture ")),

    _szTextureFilename     (
        initData(&_szTextureFilename, 
                   std::string(""),
                   "textureFilename", 
                   "Texture Filename")),

    _szCubemapFilenamePosX (
        initData(&_szCubemapFilenamePosX, 
                   std::string(""), 
                   "cubemapFilenamePosX", 
                   "Texture filename of positive-X cubemap face")),

    _szCubemapFilenamePosY (
        initData(&_szCubemapFilenamePosY, 
                   std::string(""), 
                   "cubemapFilenamePosY", 
                   "Texture filename of positive-Y cubemap face")),

    _szCubemapFilenamePosZ (
        initData(&_szCubemapFilenamePosZ, 
                   std::string(""), 
                   "cubemapFilenamePosZ", 
                   "Texture filename of positive-Z cubemap face")),

    _szCubemapFilenameNegX (
        initData(&_szCubemapFilenameNegX, 
                   std::string(""), 
                   "cubemapFilenameNegX", 
                   "Texture filename of negative-X cubemap face")),

    _szCubemapFilenameNegY (
        initData(&_szCubemapFilenameNegY, 
                   std::string(""), 
                   "cubemapFilenameNegY", 
                   "Texture filename of negative-Y cubemap face")),

    _szCubemapFilenameNegZ (
        initData(&_szCubemapFilenameNegZ, 
                   std::string(""), 
                   "cubemapFilenameNegZ", 
                   "Texture filename of negative-Z cubemap face")),

    _pTexture   (NULL),
    _pImage     (NULL),
    _pOSGImage  (NULL),
    _pOSGTexture(NULL)
{

}

OSGTexture::~OSGTexture(void)
{
    // error in original OglTexture ?
    //if (!texture) delete texture;
  
    _pOSGImage = NULL;
    
    delete _pImage;
}

void OSGTexture::setActiveTexture(unsigned short unit)
{
    //glActiveTexture(GL_TEXTURE0 + unit);
}


void OSGTexture::init(void)
{
    if(_szTextureFilename.getFullPath().empty())
    {
        if(_szCubemapFilenamePosX.getFullPath().empty() &&
           _szCubemapFilenamePosY.getFullPath().empty() &&
           _szCubemapFilenamePosZ.getFullPath().empty() &&
           _szCubemapFilenameNegX.getFullPath().empty() &&
           _szCubemapFilenameNegY.getFullPath().empty() &&
           _szCubemapFilenameNegZ.getFullPath().empty())
        {
            // "Procedural" texture (actually inline texture data inside the
            // scene file). 

            unsigned int height = proceduralTextureHeight.getValue();
            unsigned int width  = proceduralTextureWidth .getValue();
            unsigned int nbb    = proceduralTextureNbBits.getValue();

            helper::vector<unsigned int> textureData = 
                proceduralTextureData.getValue();

            if(height > 0 && width > 0 && !textureData.empty())
            {
                //Init texture
                _pImage = new helper::io::Image();

                _pImage->init(height, width, nbb);

                //Make texture
                unsigned char* data = _pImage->getPixels();

                for(unsigned int i = 0; 
                                 i < textureData.size() && 
                                 i < height * width * (nbb / 8); 
                               ++i                             )
                {
                    data[i] = (unsigned char)textureData[i];
                }

                for(unsigned int i = textureData.size(); 
                                 i < height * width * (nbb / 8); 
                               ++i                             )
                {
                    data[i] = 0;
                }

                OSG::Image::Type type = OSG::Image::OSG_UINT8_IMAGEDATA;

                if(nbb == 16) 
                    type =  OSG::Image::OSG_UINT16_IMAGEDATA;
                else if(nbb == 32) 
                    type =  OSG::Image::OSG_UINT32_IMAGEDATA;

                _pOSGImage = OSG::Image::create();

                _pOSGImage->set( OSG::Image::OSG_L_PF,
                                _pImage->getWidth(),
                                _pImage->getHeight(),
                                 1,
                                 1,
                                 1,
                                 0.0,
                                _pImage->getPixels(), 
                                 type);

                _pOSGImage->setDimension(2);
                
//                _image->set(OSG::Image::OSG_RGB_PF,
//                            img->getWidth(),
//                            img->getHeight(),
//                            1,
//                            1,
//                            1,
//                            0.0,
//                            img->getPixels(), 
//                            OSG::Image::OSG_UINT32_IMAGEDATA );

//                _image->write("PerlinNoise.png");
            }
        }
        else
        {
            // A cubemap with faces stored in separate files.
            std::string filename[6] =
            {
                _szCubemapFilenamePosX.getFullPath(),
                _szCubemapFilenameNegX.getFullPath(),
                _szCubemapFilenamePosY.getFullPath(),
                _szCubemapFilenameNegY.getFullPath(),
                _szCubemapFilenamePosZ.getFullPath(),
                _szCubemapFilenameNegZ.getFullPath()
            };

            delete _pImage;

            _pImage = NULL;

            helper::io::Image *tmp = NULL;

            for(unsigned i = 0; i < 6; i++)
            {
                if(!filename[i].empty())
                {
                    delete tmp;

                    tmp = helper::io::Image::Create(filename[i].c_str());

                    if(tmp->getTextureType() != helper::io::Image::TEXTURE_2D)
                    {
                        std::cerr << "OSGTexture: invalid texture type in " 
                                  << filename[i] 
                                  << std::endl;
                        continue;
                    }

                    if(_pImage == NULL)
                    {
                        _pImage = new helper::io::Image();

                        _pImage->init(tmp->getWidth(), 
                                      tmp->getHeight(), 
                                      0, 
                                      1, 
                                      tmp->getDataType(), 
                                      tmp->getChannelFormat());
                        
                        memset(_pImage->getPixels(), 
                                0, 
                               _pImage->getImageSize());
                    }
                    else
                    {
                        if(_pImage->getWidth () != _pImage->getWidth () ||
                           _pImage->getHeight() != _pImage->getHeight()  )
                        {
                            std::cerr << "OSGTexture: inconsistent cubemap "
                                      << "dimensions in " 
                                      << filename[i] 
                                      << std::endl;

                            continue;
                        }
                        
                        if(_pImage->getDataType() != _pImage->getDataType())
                        {
                            std::cerr << "OSGTexture: inconsistent cubemap "
                                      << "datatype in "
                                      << filename[i] << std::endl;

                            continue;
                        }

                        if(_pImage->getChannelFormat() != 
                           _pImage->getChannelFormat()   )
                        {
                            std::cerr << "OSGTexture: inconsistent cubemap "
                                      << "channel format in " 
                                      << filename[i] << std::endl;

                            continue;
                        }
                    }

                    memcpy(_pImage->getCubeMipmapPixels(i, 0), 
                            tmp->getPixels(), 
                            tmp->getImageSize());
                }
            }

            delete tmp;
        }
    }
    else
    {
        // Ordinary texture.
//        img = helper::io::Image::Create(
//            textureFilename.getFullPath().c_str());

        _pOSGImage = OSG::Image::create();

        _pOSGImage->read(_szTextureFilename.getFullPath().c_str());
std::cerr  << "_pOSGIMAGE : " << _szTextureFilename.getFullPath().c_str() << "\n";
    }

    OSGShaderElement::init();
}

/* 
   OSG::ImageRefPtr image = OSG::Image::create();
    
   OSG::UChar8 data[] = {  0xff, 0xff, 0xff,  0x80, 0x00, 0x00,
   0x80, 0x00, 0x00,  0xff, 0xff, 0xff };
    
   image->set( OSG::Image::OSG_RGB_PF, 2, 2, 1, 1, 1, 0, data );
 */


void OSGTexture::initVisual(void)
{
    std::cerr << "OSGTexture::initVisual " << std::endl;

//     if(shader != NULL) 
//         std::cerr << "OSGTexture:: HAS SHADER " << std::endl;
// 
    //GLint maxTextureUnits;
    //glGetIntegerv(GL_MAX_TEXTURE_UNITS, &maxTextureUnits);
    //MAX_NUMBER_OF_TEXTURE_UNIT = maxTextureUnits;

// TEMP cklee
    MAX_NUMBER_OF_TEXTURE_UNIT = 8;

    if(textureUnit.getValue() > MAX_NUMBER_OF_TEXTURE_UNIT)
    {
        serr << "Unit Texture too high ; set it at the unit texture n°1" 
             << sendl;

        textureUnit.setValue(1);
    }

/*
    if (!img)
    {
        serr << "OSGTexture: Error : OSGTexture file " 
             << textureFilename.getFullPath() 
             << " not found." << sendl;
        return;
    }
*/

    //texture = new helper::gl::Texture(img, 
    //                                  repeat.getValue(), 
    //                                  linearInterpolation.getValue(),
    //                                  generateMipmaps.getValue(), 
    //                                  srgbColorspace.getValue(),
    //                                  minLod.getValue(), maxLod.getValue());
    //texture->init();
    //setActiveTexture(textureUnit.getValue());

    std::cerr << "repeat " << repeat.getValue()              << std::endl;
    std::cerr << "linear " << linearInterpolation.getValue() << std::endl;
    std::cerr << "generateMips "    << generateMipmaps.getValue() << std::endl;
    std::cerr << "sRGBColorspace "  << srgbColorspace             << std::endl;
    std::cerr << "minLod " << minLod.getValue() << std::endl;
    std::cerr << "maxLod " << maxLod.getValue() << std::endl;

    //system("pause");

    _pOSGTexture = OSG::TextureObjChunk::create();

std::cerr  << "_pOSGIMAGE : " << _szTextureFilename.getFullPath().c_str() << "\n";
    _pOSGTexture->setImage(_pOSGImage);

/*
    static bool once = false;
    if (!once)
    {
        _pOSGImage->write("DAMNBIG.png");
        once = true;
    }
*/

    if(repeat.getValue() == true) 
    {
        _pOSGTexture->setWrapS(GL_REPEAT);
        _pOSGTexture->setWrapT(GL_REPEAT);
    }
    else 
    {
        _pOSGTexture->setWrapS(GL_CLAMP);
        _pOSGTexture->setWrapT(GL_CLAMP);
    }

    if(linearInterpolation.getValue() == true) 
    {
        //_pOSGTexture->setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
        //_pOSGTexture->setMinFilter(GL_LINEAR_MIPMAP_LINEAR);
        _pOSGTexture->setMagFilter(GL_LINEAR);
        _pOSGTexture->setMinFilter(GL_LINEAR);
    }
    else
    {
        _pOSGTexture->setMagFilter(GL_NEAREST);
        _pOSGTexture->setMinFilter(GL_NEAREST);
    }


//    shader->setTexture(indexShader.getValue(), 
//                       id.getValue().c_str(), 
//                       textureUnit.getValue(), 
//                       _pOSGImage);

//     std::cerr << "OSGTexture:: addTexture : " 
//               << indexShader.getValue() << " " 
//               << textureUnit.getValue() << " " 
//               << _pOSGTexture 
//               <<  "Shader " << shader << std::endl;

//     shader->addTexture( indexShader.getValue(), 
//                         id.getValue().c_str(), 
//                         textureUnit.getValue(), 
//                        _pOSGTexture           );

    OSGShader* curShader = NULL;

    for(std::set<OSGShader*>::iterator it = shaders.begin(), 
                      iend = shaders.end(); it!=iend; ++it)
    {
        curShader = *it;
        if (curShader == NULL) continue;
        curShader->addTexture( indexShader.getValue(), 
                        id.getValue().c_str(), 
                        textureUnit.getValue(), 
                       _pOSGTexture           );
    }
    
    std::cerr << "OUT OSGTexture::initVisual " << std::endl;
}

void OSGTexture::reinit(void)
{
    if(textureUnit.getValue() > MAX_NUMBER_OF_TEXTURE_UNIT)
    {
        serr << "Unit Texture too high ; set it at the unit texture n°1" 
             << sendl;

        textureUnit.setValue(1);
    }
}

void OSGTexture::fwdDraw(core::visual::VisualParams *)
{
    return;

    if(enabled.getValue() == true)
    {
        setActiveTexture(textureUnit.getValue());

        bind();

        setActiveTexture(0);
    }
}

void OSGTexture::bwdDraw(core::visual::VisualParams *)
{
    return;

    if(enabled.getValue() == true)
    {
        setActiveTexture(textureUnit.getValue());

        unbind();

        setActiveTexture(0) ;
    }
}

void OSGTexture::bind(void)
{
    return;

    //if (!texture) initVisual();
    //texture->bind();
    //glEnable(texture->getTarget());
}

void OSGTexture::unbind(void)
{
    return;

    //texture->unbind();
    //glDisable(texture->getTarget());
}

///////////////////////////////////////////////////////////////////////////////

OSGTexture2D::OSGTexture2D(void) :
    _szTexture2DFilename(initData(&_szTexture2DFilename, 
                                    std::string(""), 
                                    "_szTexture2DFilename", 
                                    "Texture2D Filename"))
{
    std::cerr << "OSGTexture2D is deprecated. Please use OSGTexture "
              << "instead." << std::endl;
}

OSGTexture2D::~OSGTexture2D(void)
{
}

void OSGTexture2D::init(void)
{
    _szTextureFilename.setValue(_szTexture2DFilename.getValue());

    OSGTexture::init();
}

END_SOFA_CMP_VISMODEL_NAMESPACE
