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

#ifndef _OSGSOFATEXTURE_H_
#define _OSGSOFATEXTURE_H_

#include <sofa/core/visual/VisualModel.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/helper/gl/template.h>
#include <sofa/helper/gl/GLSLShader.h>
#include <sofa/helper/gl/Texture.h>
#include "OSGSofaShader.h"
#include <sofa/core/objectmodel/DataFileName.h>

#include "OSGContribSOFADef.h"
#include "OSGImage.h"
#include "OSGTextureObjChunk.h"

BEGIN_SOFA_CMP_VISMODEL_NAMESPACE

/**
 *  \brief Defines an uniform sampler (texture) for a OSGShader.
 *
 *  This is an abstract class which passes a texture id to an uniform
 *  sampler variable defined into the shader and load the image into OpenGL.
 *  At the moment, only texture2D is supported.
 */

class OSG_CONTRIBSOFA_DLLMAPPING OSGTexture : public core::visual::VisualModel, 
                                              public OSGShaderElement
{

  public:

    SOFA_CLASS2(OSGTexture, core::visual::VisualModel, OSGShaderElement);

  protected:

    Data<int         >                     textureUnit;
    Data<bool        >                     enabled;
    Data<bool        >                     repeat;
    Data<bool        >                     linearInterpolation;
    Data<bool        >                     generateMipmaps;
    Data<bool        >                     srgbColorspace;
    Data<float       >                     minLod;
    Data<float       >                     maxLod;
    Data<unsigned int>                     proceduralTextureWidth;
    Data<unsigned int>                     proceduralTextureHeight;
    Data<unsigned int>                     proceduralTextureNbBits;
    Data< helper::vector<unsigned int> >   proceduralTextureData;

    sofa::core::objectmodel::DataFileName _szTextureFilename;
    sofa::core::objectmodel::DataFileName _szCubemapFilenamePosX;
    sofa::core::objectmodel::DataFileName _szCubemapFilenamePosY;
    sofa::core::objectmodel::DataFileName _szCubemapFilenamePosZ;
    sofa::core::objectmodel::DataFileName _szCubemapFilenameNegX;
    sofa::core::objectmodel::DataFileName _szCubemapFilenameNegY;
    sofa::core::objectmodel::DataFileName _szCubemapFilenameNegZ;

    helper::gl::Texture                   *_pTexture;
    helper::io::Image                     *_pImage;

    OSG::ImageUnrecPtr                     _pOSGImage;
    OSG::TextureObjChunkUnrecPtr           _pOSGTexture;

  public:

    static unsigned short MAX_NUMBER_OF_TEXTURE_UNIT;

  protected:

    OSGTexture(void);
    virtual ~OSGTexture(void);

  public:

    virtual void        init          (void                        );

            void        initVisual    (                            );
            void        reinit        (void                        );
            void        fwdDraw       (core::visual::VisualParams *);
            void        bwdDraw       (core::visual::VisualParams *);

            std::string getTextureName(void                        );
            void        getTextureUnit(void                        );

            void        bind          (void                        );
            void        unbind        (void                        );

    ///Utility function to set current active texture
    static void setActiveTexture(unsigned short unit);

  private:

    // prohibit default functions (move to 'public' if you need one)
    OSGTexture(const OSGTexture &source);
    void operator =(const OSGTexture &source);
};

class OSG_CONTRIBSOFA_DLLMAPPING OSGTexture2D : public OSGTexture
{
  public:

    SOFA_CLASS(OSGTexture2D, OSGTexture);

  private:

    sofa::core::objectmodel::DataFileName _szTexture2DFilename;

  public:

    OSGTexture2D(void);
    virtual ~OSGTexture2D(void);

    virtual void init(void);

    // prohibit default functions (move to 'public' if you need one)
    OSGTexture2D(const OSGTexture2D &source);
    void operator =(const OSGTexture2D &source);
};

END_SOFA_CMP_VISMODEL_NAMESPACE

#endif // _OSGSOFATEXTURE_H_
