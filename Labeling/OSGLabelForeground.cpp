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
//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>

#include <boost/bind.hpp>
#include <OSGImage.h>

#include <OSGViewport.h>

#include "OSGLabelForeground.h"

#include "OSGAction.h"
#include "OSGDrawEnv.h"
#include "OSGFrustumVolume.h"
#include "OSGSwitch.h"
#include "OSGLabel.h"
#include "OSGVisitSubTree.h"
#include "OSGTextureObjChunk.h"


OSG_USING_NAMESPACE

// Documentation for this class is emited in the
// OSGLabelForegroundBase.cpp file.
// To modify it, please change the .fcd file (OSGLabelForeground.fcd) and
// regenerate the base file.

/*----------------------- constructors & destructors ----------------------*/

LabelForeground::LabelForeground(void) :
    Inherited(),
    _cachedDrawEnv(NULL),
//    _cachedViewport(NULL),
    _matrixStack()
{
    TextureEnvChunkRefPtr texEnv = TextureEnvChunk::create();
    texEnv->setEnvMode(GL_MODULATE);
    this->setTextureEnvironment(texEnv);
}

/* */
LabelForeground::LabelForeground(const LabelForeground &source) :
    Inherited(source),
    _cachedDrawEnv(source._cachedDrawEnv),
//    _cachedViewport(source._cachedViewport),
    _matrixStack()
{
}

/* */
LabelForeground::~LabelForeground(void)
{
}

/*----------------------------- class specific ----------------------------*/

void LabelForeground::initMethod(InitPhase ePhase)
{
    Inherited::initMethod(ePhase);
}

/* */
void LabelForeground::changed(ConstFieldMaskArg whichField,
                                         UInt32            origin,
                                         BitVector         details    )
{
    Inherited::changed(whichField, origin, details);
}

/* */
void LabelForeground::dump(UInt32, const BitVector) const
{
    SLOG << "Dump LabelForeground NI" << std::endl;
}


/*! Draw the statistics lines.
*/
void LabelForeground::draw(DrawEnv *pEnv)
{
    if(getActive() == false) return;

    Node *root = _sfRoot.getValue();

    if (root == NULL) return; // nothing to do;

    Real32  pw = Real32(pEnv->getPixelWidth ());
    Real32  ph = Real32(pEnv->getPixelHeight());

    if (pw < 1 || ph < 1) return;

    _cachedDrawEnv  = pEnv;
//    _cachedViewport = pPort;

    glPushAttrib(GL_LIGHTING_BIT | GL_POLYGON_BIT | GL_DEPTH_BUFFER_BIT | 
                 GL_COLOR_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Set viewport. We want to map one unit to one pixel on the
    // screen. Some sources in the internet say that we should
    // add an offset of -0.375 to prevent rounding errors. Don't
    // know if that is true, but it seems to work.
    glOrtho(0 - 0.375, pw - 0.375, 0 - 0.375, ph - 0.375, 0, 1);

    // --- draw labels ------------------------------------------
//    std::cerr << "== start ===================================" << std::endl;

    assert(_matrixStack.size() == 0);
    ::traverse(root,
               boost::bind(&LabelForeground::traverseEnter, this, _1),
               boost::bind(&LabelForeground::traverseLeave, this, _1, _2) );
    assert(_matrixStack.size() == 0);

//    std::cerr << "== end =====================================" << std::endl;

    // ----------------------------------------------

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();


//    _cachedViewport = NULL;
    _cachedDrawEnv  = NULL;
}

// requires a node with a label as core
void LabelForeground::drawLabel(Node* labelNode) const
{
    Label* label = dynamic_cast<Label*>(labelNode->getCore());
    assert(label);

    // ignore unimportant labels...
    if (getImportanceThreshold() > label->getImportance()) return;

    label->onPreLabelRendering();

    Vec2f   marg     = label->getMargin();
    Vec2f   bOffset  = label->getBorderOffset();
    Color4f bColor   = label->getBorderColor();
    Vec2f   sOffset  = label->getShadowOffset();

    // ------------------------------------------------------------------
    // --- project from world to screen and draw label if necessary
    Pnt3f  screenPos;
    Matrix toScreen, toWorld = labelNode->getToWorld();

    // --- proper handling of VisitSubTrees...
    if (_matrixStack.size() > 0) {
        Matrix mat = _matrixStack.back();
        mat.mult(toWorld);
        toWorld = mat;
    }

    labelNode->getVolume().getCenter(screenPos);
    screenPos += label->getPosition().subZero();

//  _cachedViewport->getCamera()->getWorldToScreen(toScreen, *_cachedViewport);

    toScreen = _cachedDrawEnv->getWorldToScreen();
    toWorld.mult(screenPos, screenPos);
    toScreen.multFull(screenPos, screenPos);

    screenPos[0] = (screenPos[0] + 1.0) /2;
    screenPos[1] = (screenPos[1] + 1.0) /2;

    // ------------------------------------------------------------------
    // --- draw label ---------------------------------------------------
    Vec2f  screenRect = label->screenRect();
    Real32 pixWidth  = screenRect.x() + 2.0 * marg.x();
    Real32 pixHeight = screenRect.y() + 2.0 * marg.y();

    // Let's do some simple form of layout management
    Real32 pw     = Real32(_cachedDrawEnv->getPixelWidth ());
    Real32 ph     = Real32(_cachedDrawEnv->getPixelHeight());
    Real32 orthoX = pw * screenPos.x();
    Real32 orthoY = ph * screenPos.y(); 

    switch ( label->getHorizontalAlign() )
    {
        case Label::Right:   orthoX -= pixWidth;       break;
        case Label::Center:  orthoX -= pixWidth * 0.5; break;
        case Label::Left:
        default:      break;
    }

    switch ( label->getVerticalAlign() )
    {
        case Label::Bottom: orthoY += pixHeight;       break;
        case Label::Middle: orthoY += pixHeight * 0.5; break;
        case Label::Top:
        default:
            break;
    }
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glTranslatef(orthoX, orthoY, 0.0);

    glTranslatef(pixWidth*0.5,  -pixHeight*0.5, 0);
    glRotatef(label->getOrientation(), 0.0, 0.0, 1.0); // apply orientation
    glTranslatef(-pixWidth*0.5, +pixHeight*0.5, 0);

    glTranslatef(label->getPixelOffset().x(), label->getPixelOffset().y(), 0);

    // draw background
    glColor4fv(label->getBgColor().getValuesRGBA());
    glBegin(GL_QUADS);
        glVertex2f(0,        -pixHeight);
        glVertex2f(pixWidth, -pixHeight);
        glVertex2f(pixWidth, 0);
        glVertex2f(0, 0);
    glEnd();

    // draw border
    if(bColor.alpha() >= 0.0f)
    {
        glColor4fv(bColor.getValuesRGBA());
        glBegin(GL_LINE_LOOP);
            glVertex2f(bOffset.x(),             -pixHeight+1+ bOffset.y());
            glVertex2f(pixWidth-1- bOffset.x(), -pixHeight+1+ bOffset.y());
            glVertex2f(pixWidth-1- bOffset.x(),           -1- bOffset.y());
            glVertex2f(bOffset.x(),                       -1- bOffset.y());
        glEnd();
    }


    glTranslatef(marg.x(), -marg.y(), 0.0);

    TextureEnvChunkRefPtr texEnv = this->getTextureEnvironment();

    texEnv->activate(_cachedDrawEnv);

    // --- draw label shadow -----------------------------------
    glColor4fv(label->getShadowColor().getValuesRGBA());
    glPushMatrix();
    glTranslatef(sOffset.x(), sOffset.y(), 0);
    label->onLabelRendering(_cachedDrawEnv);
    glPopMatrix();

    // --- draw label content (text or icon) -------------------
    glColor4fv(label->getColor().getValuesRGBA());
    label->onLabelRendering(_cachedDrawEnv);

    texEnv->deactivate(_cachedDrawEnv);

    glPopMatrix();

    label->onPostLabelRendering();
}

Action::ResultE LabelForeground::traverseEnter(Node * const node)
{
    NodeCore* core      = node->getCore();
    Label*    labelCore = dynamic_cast<Label*>(core);

    if (labelCore) drawLabel(node); // only call for label nodes


    Switch* switchCore = dynamic_cast<Switch*>(core);

    if (switchCore && switchCore->getChoice() != Switch::ALL)
        return Action::Skip;

    VisitSubTree* visitSubTreeCore = dynamic_cast<VisitSubTree*>(core);

    if (visitSubTreeCore && visitSubTreeCore->getSubTreeRoot()) {
        _matrixStack.push_back(node->getToWorld());
        ::traverse(visitSubTreeCore->getSubTreeRoot(),
                   boost::bind(&LabelForeground::traverseEnter,this,_1),
                   boost::bind(&LabelForeground::traverseLeave,this,_1,_2));
        _matrixStack.pop_back();
        return Action::Continue;
    }

    return Action::Continue;
}

Action::ResultE LabelForeground::traverseLeave(Node * const node,
                                               Action::ResultE res)
{
    // nothing to do (could be removed)
    return res;
}
