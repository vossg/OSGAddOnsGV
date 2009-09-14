#include <stdlib.h>
#include <stdio.h>
#include <OSGConfig.h>
#include <OSGQuaternion.h>
#include <OSGRenderAction.h>
#include <OSGMatrix.h>
#include <OSGMatrixUtility.h>
#include <OSGBackground.h>
#include <OSGForeground.h>
#include <OSGGrabForeground.h>
#include <OSGPolygonForeground.h>
#include <OSGPolygonBackground.h>
#include <OSGTextureGrabForeground.h>
#include <OSGFileGrabForeground.h>
#include <OSGImageForeground.h>
#include <OSGImage.h>
#include <OSGGeometry.h>
#include <OSGSimpleGeometry.h>
#include <OSGLight.h>
#include <OSGMaterialGroup.h>
#include "OSGPerspectiveShadowMapHandler.h"
#include "OSGShadowStage.h"
#include "OSGTreeHandler.h"
#include "OSGGLU.h"



#include "OSGRenderBuffer.h"
#include "OSGTextureBuffer.h"


OSG_BEGIN_NAMESPACE

#include "ShaderCode/OSGPerspectiveShadowMapShaderCode.cinl"

PerspectiveShadowMapHandler::PerspectiveShadowMapHandler(ShadowStage *source) :
    TreeHandler(source),
    _transforms(),
    _perspectiveLPM(),
    _perspectiveLVM(),
    _tiledeco(NULL),
    _blender(NULL),
    _matrixCam2(NULL),
    _colorMapImage(NULL),
    _shadowFactorMapImage(NULL),
    _shadowFactorMapImage2(NULL),
    _shadowCmat(NULL),
    _shadowSHL(NULL),
    _shadowCubeSHL(NULL),
    _shadowSHL2(NULL),
    _shadowSHL3(NULL),
    _shadowSHL4(NULL),
    _shadowSHL5(NULL),
    _shadowSHL6(NULL),
    _shadowSHL7(NULL),
    _firstRun(0),
    _initTexturesDone(false)
{

    _blender = BlendChunk::create();
    {
        _blender->setAlphaFunc(GL_GEQUAL);
        _blender->setAlphaValue(0.99);
    }

    _matrixCam2 = MatrixCamera::create();


    _colorMapO = TextureObjChunk::create();
    _colorMapImage = Image::create();

    _colorMapO->setImage(_colorMapImage);
    _colorMapO->setInternalFormat(GL_RGB);
    _colorMapO->setExternalFormat(GL_RGB);
    _colorMapO->setMinFilter(GL_NEAREST);
    _colorMapO->setMagFilter(GL_NEAREST);
    _colorMapO->setWrapS(GL_REPEAT);
    _colorMapO->setWrapT(GL_REPEAT);
    _colorMapO->setTarget(GL_TEXTURE_2D);


    _shadowFactorMapO = TextureObjChunk::create();
    _shadowFactorMapImage = Image::create();

    _shadowFactorMapO->setImage(_shadowFactorMapImage);
    _shadowFactorMapO->setInternalFormat(GL_RGB);
    _shadowFactorMapO->setExternalFormat(GL_RGB);
    _shadowFactorMapO->setMinFilter(GL_LINEAR);
    _shadowFactorMapO->setMagFilter(GL_LINEAR);
    _shadowFactorMapO->setWrapS(GL_REPEAT);
    _shadowFactorMapO->setWrapT(GL_REPEAT);
    _shadowFactorMapO->setTarget(GL_TEXTURE_2D);


    _shadowFactorMap2O = TextureObjChunk::create();
    _shadowFactorMapImage2 = Image::create();

    _shadowFactorMap2O->setImage(_shadowFactorMapImage2);
    _shadowFactorMap2O->setInternalFormat(GL_RGB);
    _shadowFactorMap2O->setExternalFormat(GL_RGB);
    _shadowFactorMap2O->setMinFilter(GL_LINEAR);
    _shadowFactorMap2O->setMagFilter(GL_LINEAR);
    _shadowFactorMap2O->setWrapS(GL_REPEAT);
    _shadowFactorMap2O->setWrapT(GL_REPEAT);
    _shadowFactorMap2O->setTarget(GL_TEXTURE_2D);


    //SHL Chunk 1
    _shadowSHL = SHLChunk::create();
    //_shadowSHL->readVertexProgram("Perspective_Shadow.vert");
    //_shadowSHL->readFragmentProgram("Perspective_Shadow.frag");
    _shadowSHL->setVertexProgram(_lisp_shadow_vp);
    _shadowSHL->setFragmentProgram(_lisp_shadow_fp);

    _shadowSHL2 = SHLChunk::create();
    //_shadowSHL2->readVertexProgram("PSM_Shadow2.vert");
    //_shadowSHL2->readFragmentProgram("PSM_Shadow2.frag");
    _shadowSHL2->setVertexProgram(_lisp_shadow2_vp);
    _shadowSHL2->setFragmentProgram(_lisp_shadow2_fp);

    _shadowSHL3 = SHLChunk::create();
    //_shadowSHL3->readVertexProgram("PSM_Shadow3.vert");
    //_shadowSHL3->readFragmentProgram("PSM_Shadow3.frag");
    _shadowSHL3->setVertexProgram(_lisp_shadow3_vp);
    _shadowSHL3->setFragmentProgram(_lisp_shadow3_fp);

    _shadowSHL4 = SHLChunk::create();
    //_shadowSHL4->readVertexProgram("PSM_Shadow4.vert");
    //_shadowSHL4->readFragmentProgram("PSM_Shadow4.frag");
    _shadowSHL4->setVertexProgram(_lisp_shadow4_vp);
    _shadowSHL4->setFragmentProgram(_lisp_shadow4_fp);

    _shadowSHL5 = SHLChunk::create();
    //_shadowSHL5->readVertexProgram("PSM_Shadow5.vert");
    //_shadowSHL5->readFragmentProgram("PSM_Shadow5.frag");
    _shadowSHL5->setVertexProgram(_lisp_shadow5_vp);
    _shadowSHL5->setFragmentProgram(_lisp_shadow5_fp);

    _shadowSHL6 = SHLChunk::create();
    //_shadowSHL6->readVertexProgram("PSM_Shadow6.vert");
    //_shadowSHL6->readFragmentProgram("PSM_Shadow6.frag");
    _shadowSHL6->setVertexProgram(_lisp_shadow6_vp);
    _shadowSHL6->setFragmentProgram(_lisp_shadow6_fp);

    _shadowSHL7 = SHLChunk::create();
    //_shadowSHL7->readVertexProgram("PSM_Shadow7.vert");
    //_shadowSHL7->readFragmentProgram("PSM_Shadow7.frag");
    _shadowSHL7->setVertexProgram(_lisp_shadow7_vp);
    _shadowSHL7->setFragmentProgram(_lisp_shadow7_fp);

    //SHL Chunk 2
    _combineSHL = SHLChunk::create();

    //_combineSHL->readVertexProgram("Perspective_Shadow_combine.vert");
    //_combineSHL->readFragmentProgram("Perspective_Shadow_combine.frag");
    _combineSHL->setVertexProgram(_shadow_combine_vp);
    _combineSHL->setFragmentProgram(_shadow_combine_fp);

    _combineDepth = DepthChunk::create();
        _combineDepth->setReadOnly(true);

    //SHL Chunk 3
    _shadowCubeSHL = SHLChunk::create();
    //_shadowCubeSHL->readVertexProgram("Std_ShadowCube.vert");
    //_shadowCubeSHL->readFragmentProgram("Std_ShadowCube.frag");
    _shadowCubeSHL->setVertexProgram(_lisp_shadowCube_vp);
    _shadowCubeSHL->setFragmentProgram(_lisp_shadowCube_fp);

    _shadowCmat = ChunkMaterial::create();

    //Combine Shader
    _combineCmat = ChunkMaterial::create();
    _combineCmat->addChunk(_combineSHL);
    _combineCmat->addChunk(_colorMapO);
    _combineCmat->addChunk(_shadowFactorMapO);
    _combineCmat->addChunk(_combineDepth);


    _transforms[0] = Matrix(1, 0, 0, 0,
                            0, -1, 0, 0,
                            0, 0, -1, 0,
                            0, 0, 0, 1);

    _transforms[1] = Matrix(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            0, 0, 0, 1);

    _transforms[2] = Matrix(1, 0, 0, 0,
                            0, 0, 1, 0,
                            0, -1, 0, 0,
                            0, 0, 0, 1);

    _transforms[3] = Matrix(1, 0, 0, 0,
                            0, 0, -1, 0,
                            0, 1, 0, 0,
                            0, 0, 0, 1);

    _transforms[4] = Matrix(0, 0, 1, 0,
                            0, 1, 0, 0,
                            -1, 0, 0, 0,
                            0, 0, 0, 1);

    _transforms[5] = Matrix(0, 0, -1, 0,
                            0, 1, 0, 0,
                            1, 0, 0, 0,
                            0, 0, 0, 1);

    PolygonChunkUnrecPtr pPoly = PolygonChunk::create();

    pPoly->setOffsetFill  (true                     );
    pPoly->setOffsetFactor(_shadowVP->getOffFactor());
    pPoly->setOffsetBias  (_shadowVP->getOffBias  ());

    _unlitMat->addChunk(pPoly);
}

PerspectiveShadowMapHandler::~PerspectiveShadowMapHandler(void)
{
    _tiledeco = NULL;

    _blender          = NULL;
    _shadowSHL        = NULL;
    _shadowSHL2       = NULL;
    _shadowSHL3       = NULL;
    _shadowSHL4       = NULL;
    _shadowSHL5       = NULL;
    _shadowSHL6       = NULL;
    _shadowSHL7       = NULL;
    _combineSHL       = NULL;
    _combineDepth     = NULL;
    _shadowCubeSHL    = NULL;
    _combineCmat      = NULL;
    _shadowCmat       = NULL;
    _matrixCam2       = NULL;
}

bool PerspectiveShadowMapHandler::initFBO(DrawEnv *pEnv)
{
    Int32   width  = pEnv->getPixelWidth();
    Int32   height = pEnv->getPixelHeight();

    if(width <= 0 || height <= 0)
        return false;

    if(_pFB != NULL)
        return true;

    Window *win = pEnv->getWindow();

    _width  = pEnv->getPixelWidth();
    _height = pEnv->getPixelHeight();

    _colorMapImage->set(GL_RGB, _width, _height);

    _shadowFactorMapImage->set(GL_RGB, _width, _height);

    _shadowFactorMapImage2->set(GL_RGB, _width, _height);

    commitChanges();


    _pFB = FrameBufferObject::create();
        
    _pFB->setSize(_width, _height);

    RenderBufferUnrecPtr pDepthRB = RenderBuffer::create();
        
    pDepthRB->setInternalFormat(GL_DEPTH_COMPONENT24);


    TextureBufferUnrecPtr pTexBuffer = TextureBuffer::create();

    pTexBuffer->setTexture(_colorMapO);

    _pFB->setColorAttachment(pTexBuffer, 0);


    pTexBuffer = TextureBuffer::create();

    pTexBuffer->setTexture(_shadowFactorMapO);

    _pFB->setColorAttachment(pTexBuffer, 1);
        

    pTexBuffer = TextureBuffer::create();
    
    pTexBuffer->setTexture(_shadowFactorMap2O);

    _pFB->setColorAttachment(pTexBuffer, 2);


    _pFB->setDepthAttachment(pDepthRB);

    commitChanges();

    return true;
}

void PerspectiveShadowMapHandler::reInit(DrawEnv *pEnv)
{
}

void PerspectiveShadowMapHandler::initTextures(DrawEnv *pEnv)
{
    _initTexturesDone = true;
}


void PerspectiveShadowMapHandler::calcPerspectiveSpot(Matrix &_LPM, 
                                                      Matrix &_LVM,
                                                      UInt32   num,
                                                      DrawEnv *pEnv)
{

    Matrix  CPM, CVM;
#if 0
    _shadowVP->getCamera()->getViewing(CVM, _shadowVP->getParent()->getWidth(),
                                       _shadowVP->getParent()->getHeight());
    _shadowVP->getCamera()->getProjection(CPM,
                                          _shadowVP->getParent()->getWidth(),
                                          _shadowVP->getParent()->getHeight());
#endif
    pEnv->getAction()->getCamera()->getViewing(
        CVM, 
        pEnv->getWindow()->getWidth(),
        pEnv->getWindow()->getHeight());

    pEnv->getAction()->getCamera()->getProjection(
        CPM,
        pEnv->getWindow()->getWidth(),
        pEnv->getWindow()->getHeight());

    Matrix  LPM, LVM;
#if 0
    _shadowVP->_lightCameras[num]->getViewing(LVM,
                                              _shadowVP->getParent()->getWidth
                                              (),
                                              _shadowVP->getParent()->getHeight
                                              ());
    _shadowVP->_lightCameras[num]->getProjection(LPM,
                                                 _shadowVP->getParent
                                                 ()->getWidth(),
                                                 _shadowVP->getParent
                                                 ()->getHeight());
#endif
    _shadowVP->_lightCameras[num]->getViewing(LVM,
                                              pEnv->getWindow()->getWidth
                                              (),
                                              pEnv->getWindow()->getHeight
                                              ());
    _shadowVP->_lightCameras[num]->getProjection(LPM,
                                                 pEnv->getWindow
                                                 ()->getWidth(),
                                                 pEnv->getWindow
                                                 ()->getHeight());

    Matrix  LPVM;
    LPVM = LPM;
    LPVM.mult(LVM);

    Matrix  eyeProjView = CPM;
    eyeProjView.mult(CVM);
    Matrix  invEyeProjView = eyeProjView;
    invEyeProjView.invert();

    //Scene AABox Extrempunkte holen (Weltkoordinaten)
    Pnt3f   sceneMax = _shadowVP->getLightRoot(num)->getVolume().getMax();
    Pnt3f   sceneMin = _shadowVP->getLightRoot(num)->getVolume().getMin();

    Pnt3f   bb0(sceneMin[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb1(sceneMax[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb2(sceneMax[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb3(sceneMin[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb4(sceneMin[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb5(sceneMax[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb6(sceneMax[0], sceneMax[1], sceneMax[2]);
    Pnt3f   bb7(sceneMin[0], sceneMax[1], sceneMax[2]);

    //LightPos holen
    Pnt3f   lPos(0,0,0);

    bool    isDirect = false;
    bool    useStd = false;

    if(_shadowVP->_lights[num].second->getType() == DirectionalLight::getClassType())
        isDirect = true;

    if(!isDirect)
    {
        if(_shadowVP->_lights[num].second->getType() == PointLight::getClassType())
        {
            PointLight *tmpPoint;
            tmpPoint = dynamic_cast<PointLight *>(_shadowVP->_lights[num].second.get());
	
            lPos = tmpPoint->getPosition();
	
            if(tmpPoint->getBeacon() != NULL)
            {
                Matrix  m = tmpPoint->getBeacon()->getToWorld();
                m.mult(lPos, lPos);
            }
        }
	
        if(_shadowVP->_lights[num].second->getType() == SpotLight::getClassType())
        {
            SpotLight *tmpSpot;
            tmpSpot = dynamic_cast<SpotLight *>(_shadowVP->_lights[num].second.get());
            lPos = tmpSpot->getPosition();
            if(tmpSpot->getBeacon() != NULL)
            {
                Matrix  m = tmpSpot->getBeacon()->getToWorld();
                m.mult(lPos, lPos);
            }
        }
	
        //Wenn Lichtposition innerhalb der Scene-BB ist standard Shadow Mapping verwenden
        if(lPos[0] > sceneMin[0] && lPos[1] > sceneMin[1] &&
           lPos[2] > sceneMin[2] && lPos[0] < sceneMax[0] &&
           lPos[1] < sceneMax[1] && lPos[2] < sceneMax[2])
        {
            _LPM = LPM;
            _LVM = LVM;
            useStd = true;
        }
            //beleuchtet das Spotlight die komplette Szene? Wenn nicht -> standard Shadow Mapping
        else if(_shadowVP->_lights[num].second->getType() == SpotLight::getClassType
                () && !bbInsideFrustum(sceneMin, sceneMax, LPVM))
        {
            _LPM = LPM;
            _LVM = LVM;
            useStd = true;
        }
    }

    if(!useStd)
    {

        //Lichtrichtung im Post-Perspektivischen Raum des Lichtes
        Vec3f   lightDir(0,0,1);

        //Kamerapunkt holen
        Pnt3f   eyePos(0,0,0);
#if 0
        Matrix  m2 = _shadowVP->getCamera()->getBeacon()->getToWorld();
#endif
        Matrix  m2 = pEnv->getAction()->getCamera()->getBeacon()->getToWorld();

        m2.mult(eyePos, eyePos);
        
        LPVM.multFull(eyePos, eyePos);

        //Ist die Kamera hinter der Lichtquelle?
        bool    cBehindL = false;
        if(eyePos[2] < -1)
            cBehindL = true;
        else if(eyePos[2] > 1)
            cBehindL = true;

        if(cBehindL)
        {
            //Trick: Near = -Far / Far = Near
            //Gibt leider Probleme im Grenzbereich ...
            /*double tmpNear;
               tmpNear = _shadowVP->_lightCameras[num]->getNear();
               _shadowVP->_lightCameras[num]->setNear(-_shadowVP->_lightCameras[num]->getFar());
               _shadowVP->_lightCameras[num]->setFar(tmpNear);
               _shadowVP->_lightCameras[num]->getProjection(LPM,_shadowVP->getParent()->getWidth(),_shadowVP->getParent()->getHeight());
               LPVM = LPM;
               LPVM.mult(LVM);
               lightDir = Vec3f(0,0,-1);*/

            //Standard Shadow Mapping benutzen
            _LVM = LVM;
            _LPM = LPM;
        }
        else
        {
            //ViewDir
            Vec3f               viewDir(0,0,-1);
#if 0
            Matrix              m3 = _shadowVP->getCamera()->getBeacon
                ()->getToWorld();
#endif
            Matrix              m3 = pEnv->getAction()->getCamera()->getBeacon
                ()->getToWorld();
            m3  .mult(viewDir, viewDir);
            LPVM.mult(viewDir, viewDir);
            viewDir.normalize();

            //Body berechnen
            std::vector<Pnt3f>  points;
            calcHull2(&points, invEyeProjView, sceneMin, sceneMax, LPVM);

            //Kopie erstellen
            std::vector<Pnt3f>  pointsCopy;
            for(UInt32 i = 0;i < points.size();i++)
                pointsCopy.push_back(points[i]);

            bool                correctAngle = false;
            if((osgAbs(viewDir[0]) - osgAbs(lightDir[0])) < 0.01 &&
               (osgAbs(viewDir[1]) - osgAbs(lightDir[1])) < 0.01 &&
               (osgAbs(viewDir[2]) - osgAbs(lightDir[2])) < 0.01)
            {
                viewDir[0] += 0.01;
                correctAngle = true;
            }

            double              dotProd = viewDir.dot(lightDir);
            double              sinGamma;
            sinGamma = sqrt(1.0 - dotProd * dotProd);
            if(correctAngle)
                viewDir[0] -= 0.01;

            //up berechnen
            Vec3f               up99;
            calcUpVec2(up99, viewDir, lightDir);

            Matrix              lView22;
            MatrixLookAt(lView22, eyePos, eyePos + lightDir, up99);
            lView22.invert();

            // Punkte ins neue KKS transformieren
            for(UInt32 i = 0;i < points.size();i++)
            {
                Pnt3f   tmpPnt(points[i][0], points[i][1], points[i][2]);
                lView22.multFull(tmpPnt, tmpPnt);
                points[i] = tmpPnt;
            }

            // BBox um Punkte bestimmen
            Pnt3f               min99, max99;
            calcCubicHull2(min99, max99, &points);

            Real32              factor = 1.0 / sinGamma;

            Real32              nearDist = 1.0 - sinGamma;
            if(nearDist < 0.01)
                nearDist = 0.01;

            Real32              z_n = factor * nearDist;
            Real32              d = osgAbs(max99[1] - min99[1]);
            Real32              z_f = z_n + d * sinGamma;
            Real32              n = (z_n + sqrt(z_f * z_n)) / sinGamma;
            Real32              f = n + d;

            Pnt3f               pos99;

            pos99[0] = eyePos[0] + (up99[0] * -(n - nearDist));
            pos99[1] = eyePos[1] + (up99[1] * -(n - nearDist));
            pos99[2] = eyePos[2] + (up99[2] * -(n - nearDist));

            Matrix              lView222;
            MatrixLookAt(lView222, pos99, pos99 + lightDir, up99);
            lView222.invert();

            // Lisp-Matrix erstellen
            Matrix              lispMtx99(1,0,0,0,
                                          0,(f + n) / (f - n),0,-2 * f * n /
                                          (f - n),
                                          0,0,1,0,
                                          0,1,0,0);

            for(UInt32 i = 0;i < pointsCopy.size();i++)
            {
                Pnt3f   tmpPnt = pointsCopy[i];
                lView222 .multFull(tmpPnt, tmpPnt);
                lispMtx99.multFull(tmpPnt, tmpPnt);
                pointsCopy[i] = tmpPnt;
            }

            calcCubicHull2(min99, max99, &pointsCopy);

            Matrix              lProj;
            scaleTranslateToFit2(lProj, min99, max99);

            Matrix              rh2lh;
            rh2lh.setIdentity();
            rh2lh.setScale(1.0, 1.0, -1.0);

            lProj.mult(lispMtx99);
            lProj.multLeft(rh2lh);

            points.clear();
            pointsCopy.clear();

            _LPM = lProj;
            _LPM.mult(lView222);
            _LPM.mult(LPM);
            _LVM = LVM;
        }
    }
}

void PerspectiveShadowMapHandler::calcHull2(std::vector<Pnt3f> *points,
                                            Matrix invEyeProjMatrix, 
                                            Pnt3f sceneMin,
                                            Pnt3f sceneMax, 
                                            Matrix LPVM)
{
    //Die Funktion clippt das Kamera Frustum mit der Szenen Bounding Box und liefert eine Punktmenge zurck

    //Unit Cube
    Pnt3f   vf0(-1,-1,-1);
    Pnt3f   vf1(1,-1,-1);
    Pnt3f   vf2(1,1,-1);
    Pnt3f   vf3(-1,1,-1);
    Pnt3f   vf4(-1,-1,1);
    Pnt3f   vf5(1,-1,1);
    Pnt3f   vf6(1,1,1);
    Pnt3f   vf7(-1,1,1);

    //transform to World Coordinates
    invEyeProjMatrix.multFull(vf0, vf0);
    invEyeProjMatrix.multFull(vf1, vf1);
    invEyeProjMatrix.multFull(vf2, vf2);
    invEyeProjMatrix.multFull(vf3, vf3);
    invEyeProjMatrix.multFull(vf4, vf4);
    invEyeProjMatrix.multFull(vf5, vf5);
    invEyeProjMatrix.multFull(vf6, vf6);
    invEyeProjMatrix.multFull(vf7, vf7);

    //Scene Bounding Box Points
    Pnt3f   bb0(sceneMin[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb1(sceneMax[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb2(sceneMax[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb3(sceneMin[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb4(sceneMin[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb5(sceneMax[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb6(sceneMax[0], sceneMax[1], sceneMax[2]);
    Pnt3f   bb7(sceneMin[0], sceneMax[1], sceneMax[2]);

    //Build Planes of BBox
    Plane   bboxSides[6];
    //Front
    bboxSides[0] = Plane(bb0, bb1, bb2);
    //Back
    bboxSides[1] = Plane(bb7, bb6, bb5);
    //Left
    bboxSides[2] = Plane(bb0, bb3, bb7);
    //Right
    bboxSides[3] = Plane(bb2, bb1, bb5);
    //Top
    bboxSides[4] = Plane(bb6, bb7, bb3);
    //Bottom
    bboxSides[5] = Plane(bb5, bb1, bb0);

    //Build Planes of View Frustum
    //Front
    Pnt3f   vfI[6][20];
    Pnt3f   vfO[20];
    UInt32  outCount;

    //Define View Frustum as Clip Polygons
    //Front
    vfI[0][0] = vf0;
    vfI[0][1] = vf1;
    vfI[0][2] = vf2;
    vfI[0][3] = vf3;
    //Back
    vfI[1][0] = vf4;
    vfI[1][1] = vf5;
    vfI[1][2] = vf6;
    vfI[1][3] = vf7;
    //Left
    vfI[2][0] = vf0;
    vfI[2][1] = vf4;
    vfI[2][2] = vf7;
    vfI[2][3] = vf3;
    //Right
    vfI[3][0] = vf1;
    vfI[3][1] = vf5;
    vfI[3][2] = vf6;
    vfI[3][3] = vf2;
    //Top
    vfI[4][0] = vf3;
    vfI[4][1] = vf2;
    vfI[4][2] = vf6;
    vfI[4][3] = vf7;
    //Bottom
    vfI[5][0] = vf0;
    vfI[5][1] = vf1;
    vfI[5][2] = vf5;
    vfI[5][3] = vf4;

    //Punktgroesse merken
    UInt32  vfSize[6];
    for(UInt32 i = 0;i < 6;i++)
        vfSize[i] = 4;

    UInt32  sum = 0;
    Pnt3f   allPoints[100];

    // speichert die anzahl der durchgefhrten Clips
    UInt32  sumClip = 0;

    for(UInt32 i = 0;i < 6;i++)
    {
        //Kamerafrustum
        outCount = vfSize[i];

        for(UInt32 j = 0;j < 6;j++)
        {
            //BBox
            if(vfSize[i] != 0)
            {
                outCount = bboxSides[j].clip(vfI[i], vfO, vfSize[i]);
                sumClip++;
                vfSize[i] = outCount;

                for(UInt32 k = 0;k < outCount;k++)
                {
                    vfI[i][k] = vfO[k];
                }
            }
        }

        for(UInt32 k = 0;k < vfSize[i];k++)
        {
            points->push_back(vfO[k]);
        }
    }

    if(pntInFrontOf(vf0, vf1, vf2, bb0) && pntInFrontOf(vf7, vf6, vf5, bb0) &&
       pntInFrontOf(vf0, vf3, vf7, bb0) && pntInFrontOf(vf2, vf1, vf5, bb0) &&
       pntInFrontOf(vf6, vf7, vf3, bb0) && pntInFrontOf(vf5, vf1, vf0, bb0))
        points->push_back(bb0);
    if(pntInFrontOf(vf0, vf1, vf2, bb1) && pntInFrontOf(vf7, vf6, vf5, bb1) &&
       pntInFrontOf(vf0, vf3, vf7, bb1) && pntInFrontOf(vf2, vf1, vf5, bb1) &&
       pntInFrontOf(vf6, vf7, vf3, bb1) && pntInFrontOf(vf5, vf1, vf0, bb1))
        points->push_back(bb1);
    if(pntInFrontOf(vf0, vf1, vf2, bb2) && pntInFrontOf(vf7, vf6, vf5, bb2) &&
       pntInFrontOf(vf0, vf3, vf7, bb2) && pntInFrontOf(vf2, vf1, vf5, bb2) &&
       pntInFrontOf(vf6, vf7, vf3, bb2) && pntInFrontOf(vf5, vf1, vf0, bb2))
        points->push_back(bb2);
    if(pntInFrontOf(vf0, vf1, vf2, bb3) && pntInFrontOf(vf7, vf6, vf5, bb3) &&
       pntInFrontOf(vf0, vf3, vf7, bb3) && pntInFrontOf(vf2, vf1, vf5, bb3) &&
       pntInFrontOf(vf6, vf7, vf3, bb3) && pntInFrontOf(vf5, vf1, vf0, bb3))
        points->push_back(bb3);
    if(pntInFrontOf(vf0, vf1, vf2, bb4) && pntInFrontOf(vf7, vf6, vf5, bb4) &&
       pntInFrontOf(vf0, vf3, vf7, bb4) && pntInFrontOf(vf2, vf1, vf5, bb4) &&
       pntInFrontOf(vf6, vf7, vf3, bb4) && pntInFrontOf(vf5, vf1, vf0, bb4))
        points->push_back(bb4);
    if(pntInFrontOf(vf0, vf1, vf2, bb5) && pntInFrontOf(vf7, vf6, vf5, bb5) &&
       pntInFrontOf(vf0, vf3, vf7, bb5) && pntInFrontOf(vf2, vf1, vf5, bb5) &&
       pntInFrontOf(vf6, vf7, vf3, bb5) && pntInFrontOf(vf5, vf1, vf0, bb5))
        points->push_back(bb5);
    if(pntInFrontOf(vf0, vf1, vf2, bb6) && pntInFrontOf(vf7, vf6, vf5, bb6) &&
       pntInFrontOf(vf0, vf3, vf7, bb6) && pntInFrontOf(vf2, vf1, vf5, bb6) &&
       pntInFrontOf(vf6, vf7, vf3, bb6) && pntInFrontOf(vf5, vf1, vf0, bb6))
        points->push_back(bb6);
    if(pntInFrontOf(vf0, vf1, vf2, bb7) && pntInFrontOf(vf7, vf6, vf5, bb7) &&
       pntInFrontOf(vf0, vf3, vf7, bb7) && pntInFrontOf(vf2, vf1, vf5, bb7) &&
       pntInFrontOf(vf6, vf7, vf3, bb7) && pntInFrontOf(vf5, vf1, vf0, bb7))
        points->push_back(bb7);

    //Body in LK transformieren
    for(UInt32 i = 0;i < points->size();i++)
    {
        Pnt3f   tmpPnt((*points)[i]);
        LPVM.multFull(tmpPnt, tmpPnt);
        (*points)[i] = tmpPnt;
    }

    UInt32  sumPoints = points->size();
    for(UInt32 i = 0;i < sumPoints;i++)
    {
        Pnt3f   pntCut2 = (*points)[i];
        pntCut2[2] = -1.0;
        points->push_back(pntCut2);
    }
}

void PerspectiveShadowMapHandler::calcBodyVec(Vec3f &dir, Pnt3f eyePos,
                                              std::vector<Pnt3f> *points)
{
    for(UInt32 i = 0;i < points->size();i++)
    {
        Vec3f   p;
        for(UInt32 j = 0;j < 3;j++)
        {
            p[j] = (*points)[i][j] - eyePos[j];
        }
        dir[0] += p[0];
        dir[1] += p[1];
        dir[2] += p[2];
    }
    dir.normalize();
}

void PerspectiveShadowMapHandler::printMatrix(Matrix m)
{
    printf("Printing Matrix...\n");
    Real32  *mPtr = m.getValues();
    printf(
        " %f, %f, %f, %f \n %f, %f, %f, %f \n %f, %f, %f, %f \n %f, %f, %f, %f \n", mPtr[0], mPtr[1], mPtr[2], mPtr[3], mPtr[4], mPtr[5], mPtr[6], mPtr[7], mPtr[8], mPtr[9], mPtr[10], mPtr[11], mPtr[12], mPtr[13], mPtr[14], mPtr[15]);
}

void PerspectiveShadowMapHandler::printPoint(Pnt3f p)
{
    printf("Printing Point...\n");
    printf("%f, %f, %f\n", p.x(), p.y(), p.z());
}

void PerspectiveShadowMapHandler::calcUpVec2(Vec3f &up, Vec3f viewDir, Vec3f lightDir)
{
    Vec3f   left;
    left = lightDir.cross(viewDir);
    up = left.cross(lightDir);
    up.normalize();
}

bool PerspectiveShadowMapHandler::pntInFrontOf(Pnt3f p1, Pnt3f p2, Pnt3f p3, Pnt3f p)
{
    Vec3f   dir1, dir2, testdir, norm;
    dir1 = p2 - p1;
    dir2 = p3 - p2;
    dir1.normalize();
    dir2.normalize();
    norm = dir1.cross(dir2);
    norm.normalize();
    testdir = p2 - p;
    testdir.normalize();
    if(testdir.dot(norm) < 0)
        return false;
    else
        return true;
}

void PerspectiveShadowMapHandler::calcCubicHull2(Pnt3f &min, Pnt3f &max,
                                                 std::vector<Pnt3f> *points)
{
    min = Pnt3f(1000000.0, 1000000.0, 1000000.0);
    max = Pnt3f(-1000000.0, -1000000.0, -1000000.0);

    for(UInt32 i = 1;i < points->size();i++)
    {
        for(UInt32 j = 0;j < 3;j++)
        {
            if((*points)[i][j] < min[j])
                min[j] = (*points)[i][j];
            if((*points)[i][j] > max[j])
                max[j] = (*points)[i][j];
        }
    }
}

void PerspectiveShadowMapHandler::scaleTranslateToFit2(Matrix &mat, 
                                                       const Pnt3f vMin,
                                                       const Pnt3f vMax)
{
    Real32  output[16];
    output[ 0] = 2 / (vMax[0] - vMin[0]);
    output[ 4] = 0;
    output[ 8] = 0;
    output[12] = -(vMax[0] + vMin[0]) / (vMax[0] - vMin[0]);

    output[ 1] = 0;
    output[ 5] = 2 / (vMax[1] - vMin[1]);
    output[ 9] = 0;
    output[13] = -(vMax[1] + vMin[1]) / (vMax[1] - vMin[1]);

    output[ 2] = 0;
    output[ 6] = 0;
    output[10] = 2 / (vMax[2] - vMin[2]);
    output[14] = -(vMax[2] + vMin[2]) / (vMax[2] - vMin[2]);

    output[ 3] = 0;
    output[ 7] = 0;
    output[11] = 0;
    output[15] = 1;
    mat.setValueTransposed(output[0], output[1], output[2], output[3],
                           output[4], output[5], output[6], output[7],
                           output[8], output[9], output[10], output[11],
                           output[12], output[13], output[14], output[15]);
}

bool PerspectiveShadowMapHandler::bbInsideFrustum(Pnt3f sceneMin, 
                                                  Pnt3f sceneMax,
                                                  Matrix LPVM)
{
    Matrix  iLPVM(LPVM);
    iLPVM.invert();

    LPVM.multFull(sceneMin, sceneMin);
    LPVM.multFull(sceneMax, sceneMax);

    //Scene Bounding Box Points
    Pnt3f   bb0(sceneMin[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb1(sceneMax[0], sceneMin[1], sceneMin[2]);
    Pnt3f   bb2(sceneMax[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb3(sceneMin[0], sceneMax[1], sceneMin[2]);
    Pnt3f   bb4(sceneMin[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb5(sceneMax[0], sceneMin[1], sceneMax[2]);
    Pnt3f   bb6(sceneMax[0], sceneMax[1], sceneMax[2]);
    Pnt3f   bb7(sceneMin[0], sceneMax[1], sceneMax[2]);

    bool    isIn[8];
    for(UInt32 i = 0;i < 8;i++)
        isIn[i] = false;

    if(bb0[0] < 1 && bb0[0] > -1 && bb0[1] < 1 && bb0[1] > -1 && bb0[2] < 1 &&
       bb0[2] > -1)
        isIn[0] = true;
    if(bb1[0] < 1 && bb1[0] > -1 && bb1[1] < 1 && bb1[1] > -1 && bb1[2] < 1 &&
       bb1[2] > -1)
        isIn[1] = true;
    if(bb2[0] < 1 && bb2[0] > -1 && bb2[1] < 1 && bb2[1] > -1 && bb2[2] < 1 &&
       bb2[2] > -1)
        isIn[2] = true;
    if(bb3[0] < 1 && bb3[0] > -1 && bb3[1] < 1 && bb3[1] > -1 && bb3[2] < 1 &&
       bb3[2] > -1)
        isIn[3] = true;
    if(bb4[0] < 1 && bb4[0] > -1 && bb4[1] < 1 && bb4[1] > -1 && bb4[2] < 1 &&
       bb4[2] > -1)
        isIn[4] = true;
    if(bb5[0] < 1 && bb5[0] > -1 && bb5[1] < 1 && bb5[1] > -1 && bb5[2] < 1 &&
       bb5[2] > -1)
        isIn[5] = true;
    if(bb6[0] < 1 && bb6[0] > -1 && bb6[1] < 1 && bb6[1] > -1 && bb6[2] < 1 &&
       bb6[2] > -1)
        isIn[6] = true;
    if(bb7[0] < 1 && bb7[0] > -1 && bb7[1] < 1 && bb7[1] > -1 && bb7[2] < 1 &&
       bb7[2] > -1)
        isIn[7] = true;

    if(isIn[0] && isIn[1] && isIn[2] && isIn[3] && isIn[4] && isIn[5] &&
       isIn[6] && isIn[7])
        return true;
    else
        return false;
}


void PerspectiveShadowMapHandler::createShadowMapsFBO(DrawEnv *pEnv, 
                                                      RenderAction *pTmpAction)
{
#ifdef SHADOWCHECK
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glShadeModel(GL_FLAT);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_TRUE);
#endif

    // disable all lights more speed
    std::vector<bool> lightStates;
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // store old states.
        lightStates.push_back(_shadowVP->_lights[i].second->getOn());
        _shadowVP->_lights[i].second->setOn(false);
    }

    // deactivate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            exnode->setTravMask(0);
    }

    RenderAction *a = dynamic_cast<RenderAction *>(pEnv->getAction());

    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        if(_shadowVP->_lightStates[i])
        {
            if(_shadowVP->getGlobalShadowIntensity() != 0.0 ||
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
            {
                //------Setting up Window to fit size of ShadowMap-------------

                if(_shadowVP->_lights[i].second->getType() != 
                                             PointLight::getClassType() || 
                   !_shadowVP->_realPointLight[i])
                {

                    _matrixCam2->setProjectionMatrix(_perspectiveLPM[i]);
                    _matrixCam2->setModelviewMatrix(_perspectiveLVM[i]);

                    a->pushPartition();
                    {
                        RenderPartition   *pPart    = a->getActivePartition();

                        pPart->setRenderTarget(_shadowVP->_vTexChunks[i].pFBO);

                        pPart->setWindow  (a->getWindow());

                        pPart->calcViewportDimension(0.f,
                                                     0.f,
                                                     _shadowVP->getMapSize(),
                                                     _shadowVP->getMapSize(),
                                                 
                                                     _shadowVP->getMapSize(),
                                                     _shadowVP->getMapSize() );


                        Matrix m, t;
                    
                        // set the projection
                        _matrixCam2->getProjection          (
                            m, 
                            pPart->getViewportWidth (), 
                            pPart->getViewportHeight());
                        
                        _matrixCam2->getProjectionTranslation(
                            t, 
                            pPart->getViewportWidth (), 
                            pPart->getViewportHeight());
                        
                        pPart->setupProjection(m, t);
                        
                        _matrixCam2->getViewing(
                            m, 
                            pPart->getViewportWidth (),
                            pPart->getViewportHeight());
                        
                        
                        pPart->setupViewing(m);
                        
                        pPart->setNear     (_matrixCam2->getNear());
                        pPart->setFar      (_matrixCam2->getFar ());
                        
                        pPart->calcFrustum();

                        pPart->setBackground(_pClearBackground);

                        Node *light  = _shadowVP->_lights[i].first;
                        Node *parent =  light->getParent();

                        if(parent != NULL)
                        {
                            a->pushMatrix(parent->getToWorld());
                        }
                        
                        
                        a->overrideMaterial(_unlitMat, a->getActNode());
                        a->recurse(light);
                        a->overrideMaterial( NULL,       a->getActNode());

                        if(parent != NULL)
                        {
                            a->popMatrix();
                        }
                    }
                    a->popPartition();

                }
                else
                {
                    MatrixCameraDecoratorUnrecPtr    deco =
                        MatrixCameraDecorator::create();

                    for(UInt32 j = 0;j < 6;j++)
                    {
                        //Offset berechnen
                        UInt32  xOffset, yOffset;
                        if(j == 0)
                        {
                            xOffset = 0;
                            yOffset = 0;
                        }
                        else if(j == 1)
                        {
                            xOffset = _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 2)
                        {
                            xOffset = 2 * _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 3)
                        {
                            xOffset = 3 * _PLMapSize;
                            yOffset = 0;
                        }
                        else if(j == 4)
                        {
                            xOffset = 0;
                            yOffset = _PLMapSize;
                        }
                        else
                        {
                            xOffset = _PLMapSize;
                            yOffset = _PLMapSize;
                        }

                        deco->setDecoratee(_shadowVP->_lightCameras[i]);
                        deco->setPreProjection(_transforms[j]);

                        a->pushPartition();
                        {
                            RenderPartition   *pPart = a->getActivePartition();

                            pPart->setRenderTarget(
                                _shadowVP->_vTexChunks[i].pFBO);

                            pPart->setWindow  (a->getWindow());

                            pPart->calcViewportDimension(
                                xOffset, 
                                yOffset,
                                xOffset + _PLMapSize,
                                yOffset + _PLMapSize,
                                                 
                                _shadowVP->getMapSize(),
                                _shadowVP->getMapSize() );


                            Matrix m, t;
                    
                            // set the projection
                            deco->getProjection          (
                                m, 
                                pPart->getViewportWidth (), 
                                pPart->getViewportHeight());
                            
                            deco->getProjectionTranslation(
                                t, 
                                pPart->getViewportWidth (), 
                                pPart->getViewportHeight());
                            
                            pPart->setupProjection(m, t);
                            
                            deco->getViewing(
                                m, 
                                pPart->getViewportWidth (),
                                pPart->getViewportHeight());
                            
                            pPart->setupViewing(m);
                        
                            pPart->setNear     (
                                deco->getNear());
                            pPart->setFar      (
                                deco->getFar ());
                        
                            pPart->calcFrustum();
                            
                            pPart->setBackground(_pClearBackground);
                            
                            Node *light  = _shadowVP->_lights[i].first;
                            Node *parent =  light->getParent();
                            
                            if(parent != NULL)
                            {
                                a->pushMatrix(parent->getToWorld());
                            }
                            
                            
                            a->overrideMaterial(_unlitMat, a->getActNode());
                            a->recurse(light);
                            a->overrideMaterial( NULL,       a->getActNode());
                            
                            if(parent != NULL)
                            {
                                a->popMatrix();
                            }
                        }
                        a->popPartition();

                    }
                }
            }
        }
    }

    //-------Restoring old states of Window and Viewport----------

    // enable all lights.
    for(UInt32 i = 0;i < _shadowVP->_lights.size();++i)
    {
        // restore old states.
        _shadowVP->_lights[i].second->setOn(lightStates[i]);
    }

    // activate exclude nodes:
    for(UInt32 i = 0;i < _shadowVP->getMFExcludeNodes()->size();++i)
    {
        Node *exnode = _shadowVP->getExcludeNodes(i);
        if(exnode != NULL)
            if(_shadowVP->_excludeNodeActive[i])
                exnode->setTravMask(TypeTraits<UInt32>::BitsSet);
    }

#ifdef SHADOWCHECK
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
#endif
}



void PerspectiveShadowMapHandler::createColorMapFBO(DrawEnv *pEnv,
                                                    RenderAction *pTmpAction)
{
    RenderAction *a = dynamic_cast<RenderAction *>(pEnv->getAction());

    a->pushPartition((RenderPartition::CopyWindow      |
                      RenderPartition::CopyViewing     |
                      RenderPartition::CopyProjection  |
                      RenderPartition::CopyFrustum     |
                      RenderPartition::CopyNearFar     |
                      RenderPartition::CopyViewportSize),
                     RenderPartition::StateSorting);
    {
        RenderPartition *pPart = a->getActivePartition();

        pPart->setRenderTarget(_pFB);
        pPart->setDrawBuffer  (GL_COLOR_ATTACHMENT0_EXT);

        Node *parent = _shadowVP->getSceneRoot()->getParent();

        if(parent != NULL)
        {
            a->pushMatrix(parent->getToWorld());
        }
        
        pPart->setBackground(a->getBackground());

        a->recurse(_shadowVP->getSceneRoot());

        if(parent != NULL)
        {
            a->popMatrix();
        }
    }
    a->popPartition();

}


void PerspectiveShadowMapHandler::createShadowFactorMapFBO(
    DrawEnv *pEnv,
    RenderAction *pTmpAction)
{
    _activeFactorMap = 0;

    //Finde alle aktiven Lichtquellen
    Real32              activeLights = 0;
    if(_shadowVP->getGlobalShadowIntensity() != 0.0)
    {
        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            if(_shadowVP->_lightStates[i] != 0)
                activeLights++;
        }
    }
    else
    {
        for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
        {
            if(_shadowVP->_lightStates[i] != 0 &&
               _shadowVP->_lights[i].second->getShadowIntensity() != 0.0)
                activeLights++;
        }
    }


    bool bCA1Cleared = false;
    bool bCA2Cleared = false;

    RenderAction *a = dynamic_cast<RenderAction *>(pEnv->getAction());

    //Zuerst alle echte Pointlights
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
               _shadowVP->_realPointLight[i])
            {
                Real32  shadowIntensity;
                if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                {
                    shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                       activeLights);
                }
                else
                {
                    shadowIntensity =
                        (_shadowVP->_lights[i].second->getShadowIntensity() /
                         activeLights);
                }

                Matrix  LVM, LPM, CVM;

                _shadowVP->_lightCameras[i]->getViewing(
                    LVM,
                    pEnv->getPixelWidth(), 
                    pEnv->getPixelHeight());

                _shadowVP->_lightCameras[i]->getProjection(
                    LPM,
                    pEnv->getPixelWidth(),
                    pEnv->getPixelHeight());

#if 0
                _shadowVP->getCamera()->getViewing(
                    CVM,
                    _shadowVP->getPixelWidth(),
                    _shadowVP->getPixelHeight());
#endif

                pTmpAction->getCamera()->getViewing(
                    CVM,
                    pEnv->getPixelWidth(),
                    pEnv->getPixelHeight());

                Matrix  iCVM = CVM;
                iCVM.invert();
				
                Real32  texFactor;
                if(_shadowVP->_lights[i].second->getType() == 
                                               PointLight::getClassType() || 
                   _shadowVP->_lights[i].second->getType() ==
                                               SpotLight::getClassType()   )
                {
                    texFactor = Real32(_width) / Real32(_height);
                }
                else
                {
                    texFactor = 1.0;
                }

                Matrix  shadowMatrix = LPM;
                shadowMatrix.mult(LVM);
                shadowMatrix.mult(iCVM);

                Real32  xFactor = 1.0;
                Real32  yFactor = 1.0;
	
#if 0
                Matrix  m = 
                    pEnv->getAction()->getCamera()->getBeacon()->getToWorld();
#endif

                Matrix  m = pTmpAction->getCamera()->getBeacon()->getToWorld();

                Matrix  shadowMatrixOP = LVM;
                shadowMatrix.mult(iCVM);

                Matrix  shadowMatrixA = LPM;
                shadowMatrixA.mult(_transforms[0]);
                shadowMatrixA.mult(LVM);
                shadowMatrixA.mult(iCVM);

                Matrix  shadowMatrixB = LPM;
                shadowMatrixB.mult(_transforms[1]);
                shadowMatrixB.mult(LVM);
                shadowMatrixB.mult(iCVM);

                Matrix  shadowMatrixC = LPM;
                shadowMatrixC.mult(_transforms[2]);
                shadowMatrixC.mult(LVM);
                shadowMatrixC.mult(iCVM);

                Matrix  shadowMatrixD = LPM;
                shadowMatrixD.mult(_transforms[3]);
                shadowMatrixD.mult(LVM);
                shadowMatrixD.mult(iCVM);

                Matrix  shadowMatrixE = LPM;
                shadowMatrixE.mult(_transforms[4]);
                shadowMatrixE.mult(LVM);
                shadowMatrixE.mult(iCVM);

                Matrix  shadowMatrixF = LPM;
                shadowMatrixF.mult(_transforms[5]);
                shadowMatrixF.mult(LVM);
                shadowMatrixF.mult(iCVM);

                _shadowCubeSHL->addUniformVariable("shadowMap", 0);
                _shadowCubeSHL->addUniformVariable("oldFactorMap", 1);
                _shadowCubeSHL->addUniformVariable("firstRun", _firstRun);
                _shadowCubeSHL->addUniformVariable("intensity",
                                                    shadowIntensity);
                _shadowCubeSHL->addUniformVariable("texFactor", texFactor);
                _shadowCubeSHL->addUniformVariable("lightPMA", shadowMatrixA);
                _shadowCubeSHL->addUniformVariable("lightPMB", shadowMatrixB);
                _shadowCubeSHL->addUniformVariable("lightPMC", shadowMatrixC);
                _shadowCubeSHL->addUniformVariable("lightPMD", shadowMatrixD);
                _shadowCubeSHL->addUniformVariable("lightPME", shadowMatrixE);
                _shadowCubeSHL->addUniformVariable("lightPMF", shadowMatrixF);
                _shadowCubeSHL->addUniformVariable("lightPMOP",
                                                    shadowMatrixOP);
                _shadowCubeSHL->addUniformVariable("KKtoWK", m);
                _shadowCubeSHL->addUniformVariable("xFactor",
                                                    Real32(xFactor));
                _shadowCubeSHL->addUniformVariable("yFactor",
                                                    Real32(yFactor));

                _shadowCmat->clearChunks();
                _shadowCmat->addChunk(_shadowCubeSHL);
                _shadowCmat->addChunk(_shadowVP->_vTexChunks[i].pTexO);
                _shadowCmat->addChunk(_shadowVP->_vTexChunks[i].pTexE);

                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                GLenum  *buffers = NULL;
                buffers = new GLenum[1];
                if(_activeFactorMap == 0)
                    buffers[0] = GL_COLOR_ATTACHMENT1_EXT;
                else
                    buffers[0] = GL_COLOR_ATTACHMENT2_EXT;

                a->pushPartition((RenderPartition::CopyWindow      |
                                  RenderPartition::CopyViewing     |
                                  RenderPartition::CopyProjection  |
                                  RenderPartition::CopyFrustum     |
                                  RenderPartition::CopyNearFar     |
                                  RenderPartition::CopyViewportSize),
                                 RenderPartition::StateSorting);
                {
                    RenderPartition *pPart = a->getActivePartition();

                    pPart->setRenderTarget(_pFB);
                    pPart->setDrawBuffer  (*buffers);

                    Node *light  = _shadowVP->_lights[i].first;
                    Node *parent =  light->getParent();

                    if(parent != NULL)
                    {
                        a->pushMatrix(parent->getToWorld());
                    }

                    if(_activeFactorMap == 0 && bCA1Cleared == false)
                    {
                        pPart->setBackground(_pClearBackground);
                        bCA1Cleared = true;
                    }
                    else if(bCA2Cleared == false)
                    {
                        pPart->setBackground(_pClearBackground);
                        bCA2Cleared = true;
                    }
                             
                    a->overrideMaterial(_shadowCmat, a->getActNode());
                    a->recurse(light);
                    a->overrideMaterial( NULL,       a->getActNode());

                    if(parent != NULL)
                    {
                        a->popMatrix();
                    }
                }
                a->popPartition();

                delete[] buffers;
                _firstRun = 0;
                if(_activeFactorMap == 0)
                    _activeFactorMap = 1;
                else
                    _activeFactorMap = 0;
            }
        }
    }

    std::vector<Real32> shadowIntensityF;
    std::vector<Real32> mapFactorF;
    std::vector<Matrix> shadowMatrixF;

    UInt32              lightCounter = 0;

    Real32              xFactor = 1.0;
    Real32              yFactor = 1.0;
	
    //Jetzt alle normalen Lichtquellen
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0)
        {
            if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                _shadowVP->_lights[i].second->getShadowIntensity() != 0.0) &&
               !_shadowVP->_realPointLight[i])
            {

                Real32  shadowIntensity;
                if(_shadowVP->getGlobalShadowIntensity() != 0.0)
                    shadowIntensity = (_shadowVP->getGlobalShadowIntensity() /
                                       activeLights);
                else
                    shadowIntensity =
                        (_shadowVP->_lights[i].second->getShadowIntensity() /
                         activeLights);
                shadowIntensityF.push_back(shadowIntensity);
				
                Matrix  LVM, LPM, CVM;
                LVM = _perspectiveLVM[i];
                LPM = _perspectiveLPM[i];

#if 0
                _shadowVP->getCamera()->getViewing(CVM,
                                                   _shadowVP->getPixelWidth(),
                                                   _shadowVP->getPixelHeight
                                                   ());
#endif
                pTmpAction->getCamera()->getViewing(CVM,
                                                   pEnv->getPixelWidth(),
                                                   pEnv->getPixelHeight
                                                   ());
                Matrix  iCVM = CVM;
                iCVM.invert();

                Matrix  shadowMatrix = LPM;
                shadowMatrix.mult(LVM);
                shadowMatrix.mult(iCVM);
                shadowMatrixF.push_back(shadowMatrix);

                Real32  mapFactor;
                mapFactor = Real32(_shadowVP->getMapSize()) /
                    Real32(_shadowVP->_shadowImages[i]->getWidth());
                mapFactorF.push_back(mapFactor);
                lightCounter++;
            }
        }
    }

    if(lightCounter != 0)
    {
        GLenum  *buffers = NULL;
        buffers = new GLenum[1];
        if(_activeFactorMap == 0)
            buffers[0] = GL_COLOR_ATTACHMENT1_EXT;
        else
            buffers[0] = GL_COLOR_ATTACHMENT2_EXT;

        UInt32  renderTimes = 1;
        if(lightCounter > 7)
            renderTimes = UInt32(ceil(Real32(lightCounter) / 7.0f));

        for(UInt32 i = 0;i < renderTimes;i++)
        {

            UInt32  lightOffset = lightCounter - (i * 7);
		
            //clear chunk and add Textures
            _shadowCmat->clearChunks();
	
            UInt32  lightNum = 0;
            for(UInt32 j = 0;j < _shadowVP->_lights.size();j++)
            {
                if(_shadowVP->_lightStates[j] != 0)
                {
                    if((_shadowVP->getGlobalShadowIntensity() != 0.0 ||
                        _shadowVP->_lights[j].second->getShadowIntensity() != 0.0) &&
                       !_shadowVP->_realPointLight[j])
                    {
                        if(lightNum >= (i * 7) && lightNum < ((i + 1) * 7))
                        {
                            _shadowCmat->addChunk(
                                _shadowVP->_vTexChunks[j].pTexO);
                            _shadowCmat->addChunk(
                                _shadowVP->_vTexChunks[j].pTexE);
                        }
                        lightNum++;
                    }
                }
            }

            if(lightOffset == 1)
            {
                _shadowCmat->addChunk(_shadowSHL);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL->addUniformVariable("oldFactorMap", 1);
                _shadowSHL->addUniformVariable("shadowMap", 0);
                _shadowSHL->addUniformVariable("firstRun", _firstRun);
                _shadowSHL->addUniformVariable("intensity",
                                                shadowIntensityF[0 + (i * 7)]);
                _shadowSHL->addUniformVariable("lightPM", shadowMatrixF[0 +
                                                (i * 7)]);
                _shadowSHL->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL->addUniformVariable("mapFactor",
                                                Real32(mapFactorF[0 +
                                                       (i * 7)]));
            }	

            else if(lightOffset == 2)
            {
                _shadowCmat->addChunk(_shadowSHL2);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL2->addUniformVariable("oldFactorMap", 2);
                _shadowSHL2->addUniformVariable("shadowMap1", 0);
                _shadowSHL2->addUniformVariable("shadowMap2", 1);
                _shadowSHL2->addUniformVariable("firstRun", _firstRun);
                _shadowSHL2->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL2->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL2->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL2->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL2->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
            }

            else if(lightOffset == 3)
            {
                _shadowCmat->addChunk(_shadowSHL3);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL3->addUniformVariable("oldFactorMap", 3);
                _shadowSHL3->addUniformVariable("shadowMap1", 0);
                _shadowSHL3->addUniformVariable("shadowMap2", 1);
                _shadowSHL3->addUniformVariable("shadowMap3", 2);
                _shadowSHL3->addUniformVariable("firstRun", _firstRun);
                _shadowSHL3->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL3->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL3->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL3->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL3->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL3->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
            }

            else if(lightCounter == 4)
            {
                _shadowCmat->addChunk(_shadowSHL4);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL4->addUniformVariable("oldFactorMap", 4);
                _shadowSHL4->addUniformVariable("shadowMap1", 0);
                _shadowSHL4->addUniformVariable("shadowMap2", 1);
                _shadowSHL4->addUniformVariable("shadowMap3", 2);
                _shadowSHL4->addUniformVariable("shadowMap4", 3);
                _shadowSHL4->addUniformVariable("firstRun", _firstRun);
                _shadowSHL4->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL4->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL4->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL4->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL4->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL4->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL4->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
            }

            else if(lightCounter == 5)
            {
                _shadowCmat->addChunk(_shadowSHL5);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL5->addUniformVariable("oldFactorMap", 5);
                _shadowSHL5->addUniformVariable("shadowMap1", 0);
                _shadowSHL5->addUniformVariable("shadowMap2", 1);
                _shadowSHL5->addUniformVariable("shadowMap3", 2);
                _shadowSHL5->addUniformVariable("shadowMap4", 3);
                _shadowSHL5->addUniformVariable("shadowMap5", 4);
                _shadowSHL5->addUniformVariable("firstRun", _firstRun);
                _shadowSHL5->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("intensity5",
                                                 shadowIntensityF[4 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("lightPM5", shadowMatrixF[4 +
                                                 (i * 7)]);
                _shadowSHL5->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL5->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL5->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
                _shadowSHL5->addUniformVariable("mapFactor5",
                                                 Real32(mapFactorF[4 + (i *
                                                                        7)]));
            }

            else if(lightCounter == 6)
            {
                _shadowCmat->addChunk(_shadowSHL6);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL6->addUniformVariable("oldFactorMap", 6);
                _shadowSHL6->addUniformVariable("shadowMap1", 0);
                _shadowSHL6->addUniformVariable("shadowMap2", 1);
                _shadowSHL6->addUniformVariable("shadowMap3", 2);
                _shadowSHL6->addUniformVariable("shadowMap4", 3);
                _shadowSHL6->addUniformVariable("shadowMap5", 4);
                _shadowSHL6->addUniformVariable("shadowMap6", 5);
                _shadowSHL6->addUniformVariable("firstRun", _firstRun);
                _shadowSHL6->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity5",
                                                 shadowIntensityF[4 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("intensity6",
                                                 shadowIntensityF[5 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM5", shadowMatrixF[4 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("lightPM6", shadowMatrixF[5 +
                                                 (i * 7)]);
                _shadowSHL6->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL6->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL6->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor5",
                                                 Real32(mapFactorF[4 + (i *
                                                                        7)]));
                _shadowSHL6->addUniformVariable("mapFactor6",
                                                 Real32(mapFactorF[5 + (i *
                                                                        7)]));
            }

            else
            {
                _shadowCmat->addChunk(_shadowSHL7);
                if(_activeFactorMap == 0)
                {
                    _shadowCmat->addChunk(_shadowFactorMap2O);
                }
                else
                {
                    _shadowCmat->addChunk(_shadowFactorMapO);
                }

                _shadowSHL7->addUniformVariable("oldFactorMap", 7);
                _shadowSHL7->addUniformVariable("shadowMap1", 0);
                _shadowSHL7->addUniformVariable("shadowMap2", 1);
                _shadowSHL7->addUniformVariable("shadowMap3", 2);
                _shadowSHL7->addUniformVariable("shadowMap4", 3);
                _shadowSHL7->addUniformVariable("shadowMap5", 4);
                _shadowSHL7->addUniformVariable("shadowMap6", 5);
                _shadowSHL7->addUniformVariable("shadowMap7", 6);
                _shadowSHL7->addUniformVariable("firstRun", _firstRun);
                _shadowSHL7->addUniformVariable("intensity1",
                                                 shadowIntensityF[0 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity2",
                                                 shadowIntensityF[1 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity3",
                                                 shadowIntensityF[2 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity4",
                                                 shadowIntensityF[3 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity5",
                                                 shadowIntensityF[4 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity6",
                                                 shadowIntensityF[5 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("intensity7",
                                                 shadowIntensityF[6 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM1", shadowMatrixF[0 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM2", shadowMatrixF[1 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM3", shadowMatrixF[2 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM4", shadowMatrixF[3 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM5", shadowMatrixF[4 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM6", shadowMatrixF[5 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("lightPM7", shadowMatrixF[6 +
                                                 (i * 7)]);
                _shadowSHL7->addUniformVariable("xFactor", Real32(xFactor));
                _shadowSHL7->addUniformVariable("yFactor", Real32(yFactor));
                _shadowSHL7->addUniformVariable("mapFactor1",
                                                 Real32(mapFactorF[0 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor2",
                                                 Real32(mapFactorF[1 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor3",
                                                 Real32(mapFactorF[2 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor4",
                                                 Real32(mapFactorF[3 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor5",
                                                 Real32(mapFactorF[4 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor6",
                                                 Real32(mapFactorF[5 + (i *
                                                                        7)]));
                _shadowSHL7->addUniformVariable("mapFactor7",
                                                 Real32(mapFactorF[6 + (i *
                                                                        7)]));
            }


            a->pushPartition((RenderPartition::CopyWindow      |
                              RenderPartition::CopyViewing     |
                              RenderPartition::CopyProjection  |
                              RenderPartition::CopyFrustum     |
                              RenderPartition::CopyNearFar     |
                              RenderPartition::CopyViewportSize),
                             RenderPartition::StateSorting);
            {
                RenderPartition *pPart = a->getActivePartition();
                
                pPart->setRenderTarget(_pFB);
                pPart->setDrawBuffer  (*buffers);
                
                Node *light  = _shadowVP->_lights[i].first;
                Node *parent =  light->getParent();
                
                if(parent != NULL)
                {
                    a->pushMatrix(parent->getToWorld());
                }
                
                if(_activeFactorMap == 0 && bCA1Cleared == false)
                {
                    pPart->setBackground(_pClearBackground);
                    bCA1Cleared = true;
                }
                else if(bCA2Cleared == false)
                {
                    pPart->setBackground(_pClearBackground);
                    bCA2Cleared = true;
                }

                a->overrideMaterial(_shadowCmat, a->getActNode());
                a->recurse(light);
                a->overrideMaterial( NULL,       a->getActNode());

                if(parent != NULL)
                {
                    a->popMatrix();
                }
            }
            a->popPartition();

            _firstRun = 0;
            if(_activeFactorMap == 0)
                _activeFactorMap = 1;
            else
                _activeFactorMap = 0;
        }

        delete[] buffers;
    }
    _firstRun = 0;
    shadowIntensityF.clear();
    mapFactorF.clear();
    shadowMatrixF.clear();
}


void PerspectiveShadowMapHandler::render(DrawEnv      *pEnv,
                                         RenderAction *pTmpAction)
{
    Window  *win = pEnv->getWindow();
    initialize(win);

    glPushAttrib(GL_ENABLE_BIT);

    if(!_initTexturesDone)
        initTextures(pEnv);

    if(!initFBO(pEnv))
        printf("ERROR with FBOBJECT\n");


    GLfloat globalAmbient[] =
    {
        0.0, 0.0, 0.0, 1.0
    };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        glBindTexture(GL_TEXTURE_2D,
                      pEnv->getWindow()->getGLObjectId(
                          _shadowVP->_vTexChunks[i].pTexO->getGLId()));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB,
                        GL_COMPARE_R_TO_TEXTURE_ARB);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB,
                        GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB,
                        GL_LUMINANCE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    if(_shadowVP->getMapSize() / 4 > _maxPLMapSize)
        _PLMapSize = _maxPLMapSize;
    else
        _PLMapSize = _shadowVP->getMapSize() / 4;

    _firstRun = 1;
			
#if 0
    if(_shadowVP->getPixelWidth() != _width ||
       _shadowVP->getPixelHeight() != _height)
#endif
        if(pEnv->getPixelWidth() != _width ||
           pEnv->getPixelHeight() != _height)
        {
#if 0
            _width = _shadowVP->getPixelWidth();
            _height = _shadowVP->getPixelHeight();
#endif
            _width  = pEnv->getPixelWidth();
            _height = pEnv->getPixelHeight();

            _colorMapImage->set(GL_RGB, _width, _height);
                
            _shadowFactorMapImage->set(GL_RGB, _width, _height);
            
            _shadowFactorMapImage2->set(GL_RGB, _width, _height);
		
            reInit(pEnv);
        }

    //Matrizen fr alle Lichter berechnen
    for(UInt32 i = 0;i < _shadowVP->_lights.size();i++)
    {
        if(_shadowVP->_lightStates[i] != 0 &&
           (_shadowVP->_lights[i].second->getShadowIntensity() != 0.0 ||
            _shadowVP->getGlobalShadowIntensity() != 0.0))
        {
            Matrix  _LPM, _LVM;
            calcPerspectiveSpot(_LPM, _LVM, i, pEnv);
            _perspectiveLPM.push_back(_LPM);
            _perspectiveLVM.push_back(_LVM);
			
        }
        else
        {
            Matrix  _LPM, _LVM;
            _LPM.setIdentity();
            _LVM.setIdentity();
            _perspectiveLPM.push_back(_LPM);
            _perspectiveLVM.push_back(_LVM);
        }
    }
    
    if(_shadowVP->getMapAutoUpdate() == true ||
       _shadowVP->_trigger_update    == true  )
    {
        createColorMapFBO(pEnv, pTmpAction);
        
        
        //deactivate transparent Nodes
        for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
        {
            _shadowVP->_transparent[t]->setTravMask(0);
        }


        createShadowMapsFBO(pEnv, pTmpAction);

        
        // switch on all transparent geos
        for(UInt32 t = 0;t < _shadowVP->_transparent.size();++t)
        {
            _shadowVP->_transparent[t]->setTravMask(
                TypeTraits<UInt32>::BitsSet);
        }


        createShadowFactorMapFBO(pEnv, pTmpAction);
        
        _shadowVP->_trigger_update = false;
    }
    
    setupDrawCombineMap2(pEnv->getAction());

    
    glPopAttrib();
    
    _perspectiveLPM.clear();
    _perspectiveLVM.clear();
}

OSG_END_NAMESPACE
