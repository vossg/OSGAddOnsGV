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


#include "OSGTextLabel.h"
#include "OSGDrawEnv.h"
#include "OSGVolume.h"
#include "OSGTextTXFFace.h"
#include "OSGTextLayoutParam.h"
#include "OSGTextLayoutResult.h"
#include "OSGTextTXFGlyph.h"
#include "OSGStatisticsDefaultFont.h" // FIXME: get rid of this
#include "OSGTextureObjChunk.h"

OSG_USING_NAMESPACE

// Documentation for this class is emited in the
// OSGTextLabelBase.cpp file.
// To modify it, please change the .fcd file (OSGTextLabel.fcd) and
// regenerate the base file.

/*-------------------------------------------------------------------------*/
/*                               Sync                                      */

void TextLabel::changed(ConstFieldMaskArg whichField, 
                    UInt32            origin,
                    BitVector         details)
{
    Inherited::changed(whichField, origin, details);
}

/*-------------------------------------------------------------------------*/
/*                                Dump                                     */

void TextLabel::dump(UInt32 uiIndent, const BitVector bvFlags ) const
{
   Inherited::dump(uiIndent, bvFlags);
}

/*-------------------------------------------------------------------------*/
/*                            Constructors                                 */

TextLabel::TextLabel(void) :
    Inherited(), 
    _face(NULL),
    _cachedTextLayoutParam(NULL),
    _cachedTextLayoutResult(NULL)
{
}

TextLabel::TextLabel(const TextLabel &source) :
    Inherited(source),
    _face(source._face),
    _cachedTextLayoutParam(NULL),
    _cachedTextLayoutResult(NULL)
{
    if (_face != NULL) addRef(_face);
}

/*-------------------------------------------------------------------------*/
/*                             Destructor                                  */

TextLabel::~TextLabel(void)
{
    if (_face != NULL) subRef(_face);

    assert(_cachedTextLayoutParam  == NULL);
    assert(_cachedTextLayoutResult == NULL);
}


/*-------------------------------------------------------------------------*/
/*                                Render                                   */

ActionBase::ResultE TextLabel::renderEnter(Action *action)
{
    RenderAction *pAction = 
        dynamic_cast<RenderAction *>(action);

    pAction->pushVisibility();

    return ActionBase::Continue;
}

ActionBase::ResultE TextLabel::renderLeave(Action *action)
{
    RenderAction *pAction = 
        dynamic_cast<RenderAction *>(action);

    pAction->popVisibility();

    return ActionBase::Continue;
}

/*-------------------------------------------------------------------------*/
/*                            Intersect                                    */

#ifndef OSG_WINCE
ActionBase::ResultE TextLabel::intersect(Action *action)
{
          IntersectAction *ia = dynamic_cast<IntersectAction *>(action);
    const BoxVolume       &bv = ia->getActNode()->getVolume();

    if(bv.isValid() && ! bv.intersect(ia->getLine()))
    {
        return Action::Skip;  //bv missed -> can not hit children
    }

    return ActionBase::Continue;
}
#endif

/*-------------------------------------------------------------------------*/
/*                                 Init                                    */

void TextLabel::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);

    if(ePhase == TypeObject::SystemPost)
    {
        RenderAction::registerEnterDefault(
            TextLabel::getClassType(), 
            reinterpret_cast<Action::Callback>(&TextLabel::renderEnter));

        RenderAction::registerLeaveDefault(
            TextLabel::getClassType(), 
            reinterpret_cast<Action::Callback>(&TextLabel::renderLeave));
#ifndef OSG_WINCE
        IntersectAction::registerEnterDefault(
            getClassType(),
            reinterpret_cast<Action::Callback>(&TextLabel::intersect));
#endif 
    }
}


/*-------------------------------------------------------------------------*/
/*                 implement label rendering interface                     */

void TextLabel::onPreLabelRendering(void)
{
    assert(_cachedTextLayoutParam  == NULL);
    assert(_cachedTextLayoutResult == NULL);

    updateFaceAndTexture();

    _cachedTextLayoutParam  = new TextLayoutParam;
    _cachedTextLayoutParam->spacing = 1.1;
    _cachedTextLayoutParam->majorAlignment = TextLayoutParam::ALIGN_BEGIN;
    _cachedTextLayoutParam->minorAlignment = TextLayoutParam::ALIGN_BEGIN;

    _cachedTextLayoutResult = new TextLayoutResult;

    // retrieve text
    std::vector<std::string> txt;
    txt.push_back(getText());
    _face->layout(txt, *_cachedTextLayoutParam, *_cachedTextLayoutResult);
}


void TextLabel::onLabelRendering(DrawEnv *drawEnv)
{
    TextureObjChunkRefPtr texObj = this->getTextureObject();
    assert(_cachedTextLayoutParam);
    assert(_cachedTextLayoutResult);
    assert(texObj);

    Real32 scale   = 1 / _face->getScale();
    Real32 size    = _face->getParam().size;

    glTranslatef(0.5 * size,  -0.5 * size, 0.0);
    glScalef(scale, scale, 1);
    texObj->activate(drawEnv);
    drawCharacters();
    texObj->deactivate(drawEnv);
}


void TextLabel::onPostLabelRendering(void)
{
    assert(_cachedTextLayoutParam);
    assert(_cachedTextLayoutResult);

    delete _cachedTextLayoutParam;  _cachedTextLayoutParam  = NULL;
    delete _cachedTextLayoutResult; _cachedTextLayoutResult = NULL;
}


Vec2f TextLabel::screenRect(void)
{
    assert(_cachedTextLayoutParam);
    assert(_cachedTextLayoutResult);

    Real32 scale = 1 / _face->getScale();
    Real32 size  = _face->getParam().size;
    Real32 ww    = _cachedTextLayoutResult->textBounds.x() * scale + size;
    Real32 hh    = _cachedTextLayoutResult->textBounds.y() * scale + size;

    return Vec2f(ww, hh);
}

/*-------------------------------------------------------------------------*/
/*                    label rendering helper functions                     */

void TextLabel::updateFaceAndTexture(void)
{
    if (_face != NULL) {
        std::string fam = getFamily();

        if (_face->getParam().size == getSize() && 
            _face->getFamily() == fam || fam.empty()) // FIXME: what about ""?
        {
            return; // nothing to be done
        }

        subRef(_face);
        _face = NULL;
    }

    // Create the font
    std::string family = getFamily();

    if (family.empty()) family = "SANS";

    TextTXFParam param;
    param.size = static_cast<UInt32>(getSize());
    _face      = TextTXFFace::create(family, TextFace::STYLE_PLAIN, param);

    if (_face != NULL)
    {
        TextureObjChunkRefPtr texObj = TextureObjChunk::create();

        Image* texture = _face->getTexture();
        texObj->setImage(texture);
        texObj->setWrapS(GL_CLAMP);
        texObj->setWrapT(GL_CLAMP);
        texObj->setMinFilter(GL_LINEAR);
        texObj->setMagFilter(GL_LINEAR);

        this->setTextureObject(texObj);
    }


    // We failed to create the font - fallback to the default font
    if (_face == NULL)
    {
        // FIXME: create TextLabelDefaultFont!!!
        _face     = StatisticsDefaultFont::the()->getFace();
        this->setTextureObject(StatisticsDefaultFont::the()->getTexture());
    }

    // Increment reference counters
    addRef(_face);
}


void TextLabel::drawCharacters(void)
{
    assert(_cachedTextLayoutResult);

    glBegin(GL_QUADS);

    UInt32 i, numGlyphs = _cachedTextLayoutResult->getNumGlyphs();
    for(i = 0; i < numGlyphs; ++i)
    {
        const TextTXFGlyph &glyph =
            _face->getTXFGlyph(_cachedTextLayoutResult->indices[i]);

        Real32 width  = glyph.getWidth();
        Real32 height = glyph.getHeight();
        // No need to draw invisible glyphs
        if ((width <= 0.f) || (height <= 0.f))
            continue;

        // Calculate coordinates
        const Vec2f &pos      = _cachedTextLayoutResult->positions[i];
        Real32 posLeft        = pos.x();
        Real32 posTop         = pos.y();
        Real32 posRight       = pos.x() + width;
        Real32 posBottom      = pos.y() - height;
        Real32 texCoordLeft   = glyph.getTexCoord(TextTXFGlyph::COORD_LEFT);
        Real32 texCoordTop    = glyph.getTexCoord(TextTXFGlyph::COORD_TOP);
        Real32 texCoordRight  = glyph.getTexCoord(TextTXFGlyph::COORD_RIGHT);
        Real32 texCoordBottom = glyph.getTexCoord(TextTXFGlyph::COORD_BOTTOM);

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
    }

    glEnd();
}
