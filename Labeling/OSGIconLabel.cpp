/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2002 by the OpenSG Forum                   *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 *   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
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

#include <cstdlib>
#include <cstdio>

#include <OSGGL.h>

#ifndef OSG_WINCE
#include <OSGIntersectAction.h>
#endif

#include <OSGRenderAction.h>


#include "OSGIconLabel.h"
#include "OSGDrawEnv.h"
#include "OSGVolume.h"

OSG_USING_NAMESPACE

static std::map<ImageRefPtr,TextureObjChunkRefPtr> _globalIconLabelImg2texObj;
static std::map<std::string,TextureObjChunkRefPtr> _globalIconLabelFile2texObj;

static TextureObjChunkRefPtr _globalIconLabelDefaultTexObj = NULL;

static TextureObjChunkTransitPtr createTexObjChunk(Image* img);

static void ensureIconLabelGlobals (void);
static bool destroyIconLabelGlobals(void);

// Documentation for this class is emited in the
// OSGIconLabelBase.cpp file.
// To modify it, please change the .fcd file (OSGIconLabel.fcd) and
// regenerate the base file.

/*-------------------------------------------------------------------------*/
/*                               Sync                                      */

void IconLabel::changed(ConstFieldMaskArg whichField, 
                    UInt32            origin,
                    BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

/*-------------------------------------------------------------------------*/
/*                                Dump                                     */

void IconLabel::dump(UInt32 uiIndent, const BitVector bvFlags ) const
{
   Inherited::dump(uiIndent, bvFlags);
}

/*-------------------------------------------------------------------------*/
/*                            Constructors                                 */

IconLabel::IconLabel(void) : Inherited()
{
    ensureIconLabelGlobals();
}

IconLabel::IconLabel(const IconLabel &source) : Inherited(source)
{
    ensureIconLabelGlobals();
}

/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

IconLabel::~IconLabel(void)
{
}


/*-------------------------------------------------------------------------*/
/*                                Render                                   */

ActionBase::ResultE IconLabel::renderEnter(Action *action)
{
    RenderAction *pAction = 
        dynamic_cast<RenderAction *>(action);

    pAction->pushVisibility();

    return ActionBase::Continue;
}

ActionBase::ResultE IconLabel::renderLeave(Action *action)
{
    RenderAction *pAction = 
        dynamic_cast<RenderAction *>(action);

    pAction->popVisibility();

    return ActionBase::Continue;
}

/*-------------------------------------------------------------------------*/
/*                            Intersect                                    */

#ifndef OSG_WINCE
ActionBase::ResultE IconLabel::intersect(Action *action)
{
          IntersectAction *ia = dynamic_cast<IntersectAction *>(action);
    const DynamicVolume   &dv = ia->getActNode()->getVolume();

    if(dv.isValid() && ! dv.intersect(ia->getLine()))
    {
        return Action::Skip;  //bv missed -> can not hit children
    }

    return ActionBase::Continue;
}
#endif

/*-------------------------------------------------------------------------*/
/*                                 Init                                    */

void IconLabel::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
        RenderAction::registerEnterDefault(
            IconLabel::getClassType(), 
            reinterpret_cast<Action::Callback>(&IconLabel::renderEnter));

        RenderAction::registerLeaveDefault(
            IconLabel::getClassType(), 
            reinterpret_cast<Action::Callback>(&IconLabel::renderLeave));
#ifndef OSG_WINCE
        IntersectAction::registerEnterDefault(
            getClassType(),
            reinterpret_cast<Action::Callback>(&IconLabel::intersect));
#endif 
    }
}

void IconLabel::onPreLabelRendering(void)
{
    TextureObjChunkRefPtr texObj = getTextureObjChunk(); assert(texObj);

    if (this->getTextureObject() != texObj) this->setTextureObject(texObj);
}

void IconLabel::onLabelRendering(DrawEnv *drawEnv)
{
    Vec2f  ss             = screenRect();
    Real32 posLeft        =   0;
    Real32 posTop         =   0;
    Real32 posRight       =   ss.x();
    Real32 posBottom      = - ss.y();
    Real32 texCoordLeft   = 0.0;
    Real32 texCoordTop    = 1.0;
    Real32 texCoordRight  = 1.0;
    Real32 texCoordBottom = 0.0;

    TextureObjChunkRefPtr texObj = getTextureObjChunk(); assert(texObj);

    texObj->activate(drawEnv);

    glBegin(GL_QUADS);
        // lower left corner
        glTexCoord2f(texCoordLeft, texCoordBottom);
        glVertex2f(posLeft, posBottom);

        // lower right corner
        glTexCoord2f(texCoordRight, texCoordBottom);
        glVertex2f(posRight, posBottom);

        // upper right corner
        glTexCoord2f(texCoordRight, texCoordTop);
        glVertex2f(posRight, posTop);

        // upper left corner
        glTexCoord2f(texCoordLeft, texCoordTop);
        glVertex2f(posLeft, posTop);
    glEnd();

    texObj->deactivate(drawEnv);
}

void IconLabel::onPostLabelRendering(void)
{
}


Vec2f IconLabel::screenRect(void)
{
    TextureObjChunkRefPtr texObj = getTextureObjChunk(); assert(texObj);
    Real32 ww = getSize().x();
    Real32 hh = getSize().y();

    if (ww <= 1 && hh <= 1) {
        Image* img = texObj->getImage();

        if (ww <= 0) ww = 1.0;
        if (hh <= 0) hh = 1.0;

        ww *= img->getWidth();
        hh *= img->getHeight();
    }

    return Vec2f(ww, hh);
}

/* the 'image' property is more dominant than the 'filename' property */
TextureObjChunkRefPtr IconLabel::getTextureObjChunk(void)
{
    ImageRefPtr           img    = getImage();
    std::string           file   = getFilename();
    TextureObjChunkRefPtr texObj = NULL;

    if (img != NULL)
        texObj = _globalIconLabelImg2texObj[img];
    else if (!file.empty())
        texObj = _globalIconLabelFile2texObj[file];

    if (texObj != NULL) return texObj; // return cached texture object chunk

    if (img != NULL) {
        _globalIconLabelImg2texObj[img] = createTexObjChunk(img);
        texObj                          = _globalIconLabelImg2texObj[img];
    }
    else if (!file.empty()) {
        img = Image::create();
        if (img->read(file.c_str())) {
            _globalIconLabelFile2texObj[file] = createTexObjChunk(img);
            texObj = _globalIconLabelFile2texObj[file];
        }
        else {
            std::cerr << "WARNING: IconLabel: could not read image from file"
                      << file << std::endl;
        }
    }

    if (texObj != NULL) return texObj; // return new texture object chunk


    assert(_globalIconLabelDefaultTexObj);

    return _globalIconLabelDefaultTexObj;
}


// --- static/global functions ---------------------------------------------

TextureObjChunkTransitPtr createTexObjChunk(Image* img)
{
    TextureObjChunkTransitPtr texObj = TextureObjChunk::create();

    texObj->setImage(img);
    texObj->setMinFilter(GL_LINEAR);
    texObj->setMagFilter(GL_LINEAR);
    texObj->setWrapS(GL_REPEAT);
    texObj->setWrapT(GL_REPEAT);

    return texObj;
}

void ensureIconLabelGlobals(void)
{
    if (_globalIconLabelDefaultTexObj != NULL) return;

    UChar8 imgdata[] = {  255,0,0,  0,255,0,  0,0,255, 255,0,255 };

    ImageRefPtr img = Image::create();
    img->set(Image::OSG_RGB_PF, 2, 2, 1, 1, 1, 0, imgdata );

    _globalIconLabelDefaultTexObj = createTexObjChunk(img);

    addPreFactoryExitFunction(destroyIconLabelGlobals);
}

bool destroyIconLabelGlobals(void)
{
    _globalIconLabelDefaultTexObj = NULL;
    _globalIconLabelImg2texObj .clear();
    _globalIconLabelFile2texObj.clear();

    return true;
}
