#include <OSGAddOnsConfig.h>

#include <iostream>

#include <OSGGLUT.h>
#include <OSGNode.h>

#include "OSGTrackball.h"
#include "OSGLine.h"
#include "OSGPerspectiveCamera.h"
#include "OSGTransform.h"
#include "OSGRenderAction.h"
#include "OSGWindow.h"
#include "OSGSceneFileHandler.h"
#include "OSGSolidBackground.h"
#include "OSGGLUTWindow.h"
#include "OSGDirectionalLight.h"
#include "OSGSimpleGeometry.h"

#ifdef OLD_BBQ
#include "OSGBbqTerrainEngine.h"
#include "OSGBbqOutOfCoreDataSource.h"
#include "OSGBaseInitFunctions.h"

#include "OSGAlgorithmStage.h"
#include "OSGCallbackAlgorithm.h"
#endif

#include "OSGBbqTerrain.h"
#include "OSGBbqOutOfCoreDataSource.h"
#include "OSGMaterialGroup.h"
#include "OSGSimpleMaterial.h"

using namespace OSG;

RenderActionRefPtr rentravact = NULL;

NodeUnrecPtr  root = NULL;
NodeUnrecPtr  file = NULL;

PerspectiveCameraUnrecPtr cam = NULL;
ViewportUnrecPtr          vp  = NULL;
WindowUnrecPtr            win = NULL;

TransformUnrecPtr cam_trans   = NULL;
TransformUnrecPtr scene_trans = NULL;
TransformUnrecPtr ref_trans   = NULL;

#ifdef OLD_BBQ
BbqTerrainEngine       *terrain_              = NULL;
BbqOutOfCoreDataSource *outOfCoreDataSource_  = NULL;
BbqRenderOptions        terrainRenderOptions_;
#endif

BbqTerrainUnrecPtr             pTerrain = NULL;
BbqOutOfCoreDataSourceUnrecPtr pSource  = NULL;

Trackball tball;
Trackball tcamball;

Trackball *pActiveTBall = NULL;

int uiSize = 500;

float geoMorph = 0;

bool move_obj = false;
bool bAnimate = false;

int mouseb = 0;
int lastx  = 0;
int lasty  = 0;

Quaternion oldq;
Vec3f      oldv;

Matrix m1c;
Matrix m4c;

float fUnitScale  = 1.f;
float fUnitScale1 = 1.f;
Vec2f vUnitOffset(0.f, 0.f);

Real32 tScale = 1.f;
Real32 rScale = 1.f;

#ifdef OLD_BBQ
void terrainRenderCB(DrawEnv *pEnv)
{
    Vec3f camPos(m1c[3][0],
                 m1c[3][1],
                 m1c[3][2]);

    terrainRenderOptions_.showBoundingBoxes    = false;
    terrainRenderOptions_.useVboExtension      = true;
    terrainRenderOptions_.enableFrustumCulling = false;
    terrainRenderOptions_.screenSpaceError     = 5.f;
    terrainRenderOptions_.fovy                 = osgDegree2Rad(90);
    terrainRenderOptions_.screenSize.setValues(uiSize, uiSize);
    terrainRenderOptions_.pDrawEnv             = pEnv;
    terrainRenderOptions_.viewerpos[0]         = camPos[0];
    terrainRenderOptions_.viewerpos[1]         = camPos[1];
    terrainRenderOptions_.viewerpos[2]         = camPos[2];
    terrainRenderOptions_.geoMorphFactor       = geoMorph;

//    Vec3f camPos(0, 1000, 0);
    
//    fprintf(stderr, "camerapos %f %f %f\n", 
//           camPos[0],
//          camPos[1],
//          camPos[2]);

    terrain_->update(camPos);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadMatrixf(&(pEnv->getCameraViewing()[0][0]));

    terrain_->render(terrainRenderOptions_);

    glPopMatrix();

    terrain_->prefetchNodeData();
}
#endif

static float xPoints[][3] = 
{
//    {165.0, -35.0, 0.0},
//    {180.0, -50.0, 0.0}
//    {170.0, -40.0, 0.5},
    {171.5, -41.5, 0.5},
    {173.5, -43.5, 0.5}
};

static float tStep = 0.0004;
static float t     = 0;

void display(void)
{
    Matrix m1, m2, m3;
    Quaternion q1;

    tball.getRotation().getValue(m3);

    q1.setValue(m3);

    m1.setRotate(q1);
    
    m2.setTranslate( tball.getPosition() );
    
    m1.mult( m2 );

    cam_trans->editSFMatrix()->setValue(m1);

    
#if 0
    m1c.setIdentity();

    Matrix m2c, m3c;
    Quaternion q1c;

    tcamball.getRotation().getValue(m3c);

    q1c.setValue(m3c);

    m1c.setRotate(q1c);
    
    m2c.setTranslate( tcamball.getPosition() );
    
    m1c.mult( m2c );
#else
    m1c.setIdentity();

    float fLat  = xPoints[0][1] + (xPoints[1][1] - xPoints[0][1]) * t;
    float fLong = xPoints[0][0] + (xPoints[1][0] - xPoints[0][0]) * t;

//    fprintf(stderr, "%f %f\n", fLat, fLong);

    if(bAnimate == true)
    {
        t += tStep;
        
        if(t > 1)
        {
            tStep = -tStep;
            t     = 1;
        }
        else if(t < 0)
        {
            tStep = -tStep;
            t     = 0;
        }
    }

    Pnt3f p1;

    projectPnt(p1, fLat, fLong, 50);

    m1c[3][0] = p1[0];
    m1c[3][1] = p1[1];
    m1c[3][2] = p1[2];

#endif

    scene_trans->editSFMatrix()->setValue(m1c);

    Vec3f x1(m1c[3][0],
             m1c[3][1],
             m1c[3][2]);
    Vec3f x2;

    backProjectPnt(x2, x1);


/*
    fprintf(stderr, "%f %f %f\n", 
            osgRad2Degree(x2[0]), 
            x2[2],
            osgRad2Degree(x2[1]));
 */

/* -285.728333 -285.728333 | 494.500488 494.500488 */

    const BbqDataSourceInformation &tInfo = 
        pSource->getInformation();

    m4c.setIdentity();

    m4c[3][0] = osgRad2Degree(x2[0]);
    m4c[3][1] = 0; //x2[2];
//    m4c[3][2] = -45.f - (osgRad2Degree(x2[1]) + 40.f);
//    m4c[3][2] = -(osgRad2Degree(x2[1]) + 40.f);
    m4c[3][2] = osgRad2Degree(x2[1]);

//    fprintf(stderr, "%f %f\n", 
//            -(osgRad2Degree(x2[1]) + 40.f),
//           -45.f - (osgRad2Degree(x2[1]) + 40.f));
//    (571.45666/ 5.f)
//   (- 285.728333 - (170.f * fUnitScale))

//    m4c[3][0] = m4c[3][0] * fUnitScale + vUnitOffset[0]; 
    m4c[3][0] = m4c[3][0] * tInfo.vScale[0] + tInfo.vOffset[0]; 

//    m4c[3][2] = (-m4c[3][2] - 40.f) * fUnitScale - 285.728333; 
//    m4c[3][2] = m4c[3][2] * fUnitScale1 + vUnitOffset[1];
    m4c[3][2] = m4c[3][2] * tInfo.vScale[1] + tInfo.vOffset[1];

    ref_trans->editSFMatrix()->setValue(m4c);

    commitChanges();

//    win->render(rentravact);

//    fprintf(stderr, "Frame\n==========================================\n");

    win->activate ();
    win->frameInit();    // query recently registered GL extensions
    
    win->renderAllViewports(rentravact);

    win->swap     ();
    win->frameExit();    // after frame cleanup: delete GL objects, if needed

}

void reshape( int w, int h )
{
    std::cerr << "Reshape: " << w << "," << h << std::endl;
    win->resize( w, h );
}


void animate(void)
{
    glutPostRedisplay();
}

// tballall stuff


void motion(int x, int y)
{   
    Real32 w = win->getWidth(), h = win->getHeight();
    

    Real32  
        a = -2. * ( lastx / w - .5 ),
        b = -2. * ( .5 - lasty / h ),
        c = -2. * ( x / w - .5 ),
        d = -2. * ( .5 - y / h );
    
    if ( mouseb & ( 1 << GLUT_LEFT_BUTTON ) )
    {
        pActiveTBall->updateRotation( a, b, c, d );     
    }
    else if ( mouseb & ( 1 << GLUT_MIDDLE_BUTTON ) )
    {
        pActiveTBall->updatePosition( a, b, c, d );     
    }
    else if ( mouseb & ( 1 << GLUT_RIGHT_BUTTON ) )
    {
        pActiveTBall->updatePositionNeg( a, b, c, d );  
    }

    if(move_obj == true)
    {
/*
        fprintf(stderr, "%f %f %f\n",
                pActiveTBall->getPosition()[0],
                pActiveTBall->getPosition()[1],
                pActiveTBall->getPosition()[2]);
 */
    }

    lastx = x;
    lasty = y;
}

void mouse(int button, int state, int x, int y)
{
    if ( state == 0 )
    {
        switch ( button )
        {
            case GLUT_LEFT_BUTTON:  
                break;

            case GLUT_MIDDLE_BUTTON:
                pActiveTBall->setAutoPosition(true);
                break;

            case GLUT_RIGHT_BUTTON:     
                pActiveTBall->setAutoPositionNeg(true);
                break;
        }

        mouseb |= 1 << button;
    }
    else if ( state == 1 )
    {
        switch ( button )
        {
            case GLUT_LEFT_BUTTON:  break;

            case GLUT_MIDDLE_BUTTON:
                pActiveTBall->setAutoPosition(false);
                break;

            case GLUT_RIGHT_BUTTON:     
                pActiveTBall->setAutoPositionNeg(false);
                break;
        }       

        mouseb &= ~(1 << button);
    }
    lastx = x;
    lasty = y;
}

void vis(int visible)
{
    if (visible == GLUT_VISIBLE) 
    {
        glutIdleFunc(animate);
    } 
    else 
    {
        glutIdleFunc(NULL);
    }
}

void key(unsigned char key, int x, int y)
{
    switch ( key )
    {
        case 27:    
            rentravact = NULL;
#ifdef OLD_BBQ
            delete terrain_;
            delete outOfCoreDataSource_;
#endif

            root = NULL;
            file = NULL;
            cam = NULL;
            vp  = NULL;
            win = NULL;
            cam_trans   = NULL;
            scene_trans = NULL;
            ref_trans   = NULL;
            pTerrain = NULL;
            pSource  = NULL;


            osgExit(); 
            exit(0);
        case 'A':   
            glDisable( GL_LIGHTING );
            std::cerr << "Lighting disabled." << std::endl;
            break;
        case 'S':   
            glEnable( GL_LIGHTING );
            std::cerr << "Lighting enabled." << std::endl;
            break;
        case 'z':   glPolygonMode( GL_FRONT_AND_BACK, GL_POINT);
            std::cerr << "PolygonMode: Point." << std::endl;
            break;
        case 'x':   glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
            std::cerr << "PolygonMode: Line." << std::endl;
            break;
        case 'c':   glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            std::cerr << "PolygonMode: Fill." << std::endl;
            break;
        case 'r':   
        {
            std::cerr << "Sending ray through " << x << "," << y << std::endl;
            Line l;
            cam->calcViewRay( l, x, y, *vp );
            std::cerr << "From " << l.getPosition() << ", dir " 
                      << l.getDirection()
                      << std::endl;
        }
        break;

        case '<':
            m1c[3][1] -= 100;
            break;

        case '>':
            m1c[3][1] += 100;
            break;

        case 'd':
            pTerrain->setShowSwitchDistance( 
                !pTerrain->getShowSwitchDistance());
            break;

        case 'a':
            bAnimate = !bAnimate;
            break;

        case '[':
            pTerrain->setScreenSpaceError(
                pTerrain->getScreenSpaceError() - 0.01);

            fprintf(stderr, "spe : %f\n", pTerrain->getScreenSpaceError());
            break;

        case ']':
            pTerrain->setScreenSpaceError(
                pTerrain->getScreenSpaceError() + 0.01);

            fprintf(stderr, "spe : %f\n", pTerrain->getScreenSpaceError());
            break;

        case '+':
            tScale *= 2;
            fprintf(stderr, "ts %f\n", tScale);
            tball.setTranslationScale(tScale);
            break;

        case '-':
            tScale /= 2;
            fprintf(stderr, "ts %f\n", tScale);
            tball.setTranslationScale(tScale);
            break;

        case '\'':
            rScale *= 2;
            fprintf(stderr, "rs %f\n", rScale);
            tball.setRotationScale(rScale);
            break;

        case ';':
            rScale /= 2;
            fprintf(stderr, "rs %f\n", rScale);
            tball.setRotationScale(rScale);
            break;

        case ')':
            tStep *= 2;
            fprintf(stderr, "tst %f\n", tStep);
            break;

        case '(':
            tStep /= 2;
            fprintf(stderr, "tst %f\n", tStep);
            break;

#ifdef OLD_BBQ
        case 's':
            terrainRenderOptions_.showSkirts =
                !terrainRenderOptions_.showSkirts;
            break;
        case 'g':
            geoMorph = 1.f - geoMorph;
            fprintf(stderr, "geom : %f\n", geoMorph);
            break;
#endif

        case ' ':
        {
            move_obj = ! move_obj;

            if(move_obj == true)
            {
                pActiveTBall = &tcamball;
            }
            else
            {
                pActiveTBall = &tball;
            }
        }
        break;
    }
}


int main (int argc, char **argv)
{
    osgInit(argc,argv);
    
    // GLUT init

    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize    (uiSize, uiSize);
    glutInitWindowPosition(100,100);


    int winid = glutCreateWindow("OpenSG");
    glutKeyboardFunc(key);
    glutVisibilityFunc(vis);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);       
    glutMouseFunc(mouse);   
    glutMotionFunc(motion); 
    
    glutIdleFunc(display);  

    // OSG

    SceneFileHandler::the()->print();

    // create the graph

    // beacon for camera and light  
    NodeUnrecPtr  b1n = Node::create();
    GroupUnrecPtr b1  = Group::create();

    b1n->setCore( b1 );

    // transformation
    NodeUnrecPtr      t1n = Node::create();
    TransformUnrecPtr t1  = Transform::create();

    t1n->setCore (t1 );
    t1n->addChild(b1n);

    cam_trans = t1;

    // light
    
    NodeUnrecPtr             dlight = Node::create();
    DirectionalLightUnrecPtr dl     = DirectionalLight::create();

    {
        dlight->setCore(dl);
        
        dl->setAmbient( .0, .0, .0, 1 );
        dl->setDiffuse( .8, .8, .8, .8 );
        dl->setDirection(0,0,1);
        dl->setBeacon( b1n);
    }

    // root
    root              = Node:: create();
    GroupUnrecPtr gr1 = Group::create();

    root->setCore (gr1   );
    root->addChild(t1n   );
    root->addChild(dlight);

    // Load the file

    NodeUnrecPtr file = NULL;
    
    if(argc > 1)
    {
        file = SceneFileHandler::the()->read(argv[1], NULL);
    }

    if(file == NULL)
    {
        std::cerr << "Couldn't load file, ignoring" << std::endl;

        file = makeSphere(4, 20.0);
    }

    Thread::getCurrentChangeList()->commitChanges();

    file->updateVolume();


    Vec3f min,max;
    file->getVolume().getBounds(min, max);
    
    std::cout << "Volume: from " << min << " to " << max << std::endl;

                 scene_trans = Transform::create();
    NodeUnrecPtr sceneTrN    = Node::create();

    sceneTrN->setCore (scene_trans);
    sceneTrN->addChild(file       );


                 ref_trans = Transform::create();
    NodeUnrecPtr refTrN    = Node::create();

    refTrN->setCore (ref_trans);
    refTrN->addChild(makeSphere(4, 20.0));



#ifdef OLD_BBQ
    AlgorithmStageUnrecPtr pAlgoStage = AlgorithmStage::create();

    pAlgoStage->setInheritedTarget(true);
    pAlgoStage->setProjectionMode(AlgorithmStage::Ignore);
    pAlgoStage->setCopyViewing(true);

    CallbackAlgorithmUnrecPtr pAlgo = CallbackAlgorithm::create();


    pAlgo->setCallback(&terrainRenderCB, "");

    pAlgoStage->setAlgorithm(pAlgo);
#endif

    pSource = BbqOutOfCoreDataSource::create();

//    pSource->setFilename("data/ps_com.bbq");
    pSource->setFilename("data/ps.bbq");
//    pSource->setFilename("/home/gerrit/mtmp/ps.bbq");

    pSource->setHeightScale  (6500.0f);
    pSource->setHeightOffset (0.0f  );
    pSource->setSampleSpacing(1.0f  );

    pTerrain = BbqTerrain::create();

    pTerrain->setBeacon    (sceneTrN);
    pTerrain->setDataSource(pSource );

    pTerrain->setScreenSpaceError(6.0);

    NodeUnrecPtr pAlgoNode = Node::create();

    pAlgoNode->setCore(pTerrain);

    dlight->addChild(pAlgoNode);

    dlight->addChild(sceneTrN);
    dlight->addChild(refTrN  );

    OSG::commitChanges();

    const BbqDataSourceInformation &tInfo = 
        pSource->getInformation();

    fprintf(stderr, "%d %d\n",
            tInfo.heightSampleCount[0],
            tInfo.heightSampleCount[1]);

    fprintf(stderr, "min %f %f\n",
            tInfo.vOrigin[0],
            tInfo.vOrigin[1]);

    fprintf(stderr, "max %f %f\n",
            tInfo.vOrigin[0] + 
                tInfo.heightSampleCount[0] * tInfo.vPixelSize[0],
            tInfo.vOrigin[1] + 
                tInfo.heightSampleCount[1] * tInfo.vPixelSize[1]);

    fprintf(stderr, "ts: %f\n", tInfo.sampleSpacing);


    fprintf(stderr, "info Bbox min %f %f\n", 
            tInfo.gridBBoxMin[0],
            tInfo.gridBBoxMin[1]);

    fprintf(stderr, "info Bbox max %f %f\n", 
            tInfo.gridBBoxMax[0],
            tInfo.gridBBoxMax[1]);

    fUnitScale  = tInfo.sampleSpacing / tInfo.vPixelSize[0];
    fUnitScale1 = tInfo.sampleSpacing / tInfo.vPixelSize[1];

    //(- 285.728333 - (170.f * fUnitScale)
    
    vUnitOffset[0] = tInfo.gridBBoxMin[0] - (tInfo.vOrigin[0] * fUnitScale);
    //m4c[3][2] = (-m4c[3][2] - 40.f) * fUnitScale - 285.728333; 

    vUnitOffset[1] = tInfo.gridBBoxMin[1] - (tInfo.vOrigin[1] * fUnitScale1);

#if 1
    NodeUnrecPtr geoRef = Node::create();
    
    geoRef->setCore(Group::create());

    NodeUnrecPtr           pEll     = Node::create();
    MaterialGroupUnrecPtr  pEllMatG = MaterialGroup::create();
    SimpleMaterialUnrecPtr pEllMat  = SimpleMaterial::create();

    pEllMat->editDiffuse().setValuesRGB(0.3, 0.3, 0.3);

    pEllMatG->setMaterial(pEllMat);

    pEll->setCore(pEllMatG);

    pEll->addChild(makeLatLongEllipsoid(18, 
                                        36, 
                                        6378.137 * 0.95,
                                        6356.7523142 * 0.95));
    

    geoRef->addChild(pEll);
    
    geoRef->addChild(makeEllipsoidAxis(18, 
                                       36, 
                                       6378.137 * 1.05,
                                       6356.7523142 * 1.05));




#if 0
    NodeUnrecPtr           pEllSeg     = Node::create();
    MaterialGroupUnrecPtr  pEllSegMatG = MaterialGroup::create();
    SimpleMaterialUnrecPtr pEllSegMat  = SimpleMaterial::create();

    pEllSegMat->editDiffuse().setValuesRGB(1.0, 0.0, 0.0);
    

    pEllSegMatG->setMaterial(pEllSegMat);

    pEllSeg->setCore(pEllSegMatG);

    pEllSeg->addChild(makeLatLongEllipsoidSeg(18, 
                                              36, 
                                              6378.137 * 1.01,
                                              6356.7523142 * 1.01,
                                              osgDegree2Rad(-50.f),
                                              osgDegree2Rad(165.f),
                                              osgDegree2Rad(-35.f),
                                              osgDegree2Rad(180.f)));

    geoRef->addChild(pEllSeg);
#endif


    NodeUnrecPtr pCorner0 = Node::create();
    NodeUnrecPtr pCorner1 = Node::create();
    NodeUnrecPtr pCorner2 = Node::create();
    NodeUnrecPtr pCorner3 = Node::create();

    TransformUnrecPtr pCornerTr0 = Transform::create();
    TransformUnrecPtr pCornerTr1 = Transform::create();
    TransformUnrecPtr pCornerTr2 = Transform::create();
    TransformUnrecPtr pCornerTr3 = Transform::create();

    pCorner0->setCore(pCornerTr0);
    pCorner1->setCore(pCornerTr1);
    pCorner2->setCore(pCornerTr2);
    pCorner3->setCore(pCornerTr3);

    pCorner0->addChild(makeLatLongSphere(10, 10, 100.f));
    pCorner1->addChild(makeLatLongSphere(10, 10, 100.f));
    pCorner2->addChild(makeLatLongSphere(10, 10, 100.f));
    pCorner3->addChild(makeLatLongSphere(10, 10, 100.f));

    Pnt3f trpos(0.f, 0.f, 0.f);

    projectPnt(trpos, -50.f, 165, 10);

    pCornerTr0->editMatrix()[3][0] = trpos[0];
    pCornerTr0->editMatrix()[3][1] = trpos[1];
    pCornerTr0->editMatrix()[3][2] = trpos[2];

    projectPnt(trpos, -35.f, 165, 10);

    pCornerTr1->editMatrix()[3][0] = trpos[0];
    pCornerTr1->editMatrix()[3][1] = trpos[1];
    pCornerTr1->editMatrix()[3][2] = trpos[2];

    projectPnt(trpos, -50.f, 180, 10);

    pCornerTr2->editMatrix()[3][0] = trpos[0];
    pCornerTr2->editMatrix()[3][1] = trpos[1];
    pCornerTr2->editMatrix()[3][2] = trpos[2];

    projectPnt(trpos, -35.f, 180, 10);

    pCornerTr3->editMatrix()[3][0] = trpos[0];
    pCornerTr3->editMatrix()[3][1] = trpos[1];
    pCornerTr3->editMatrix()[3][2] = trpos[2];

    geoRef->addChild(pCorner0);
    geoRef->addChild(pCorner1);
    geoRef->addChild(pCorner2);
    geoRef->addChild(pCorner3);

    dlight->addChild(geoRef);
#endif

    Pnt3f x1;
    Pnt3f x2;

    projectPnt(x1, 170.0,               40.0, 0);
    projectPnt(x2, 170.000833333333333, 40.0, 0);

    Vec3f v1 = x1 - x2;

    fprintf(stderr, "l:%f\n", v1.length());

    // Camera
    
    cam = PerspectiveCamera::create();
    {
        cam->setBeacon( b1n );
        cam->setFov( osgDegree2Rad( 90 ) );
        cam->setNear( 0.1 );
        cam->setFar( 120000 );
    }

    // Background
    SolidBackgroundUnrecPtr bkgnd = SolidBackground::create();
    {
        bkgnd->setColor(Color3f(0,0,1));
    }

    // Viewport
    vp = Viewport::create();
    {
        vp->setCamera( cam );
        vp->setBackground( bkgnd );
        vp->setRoot( root );
        vp->setSize( 0,0, 1,1 );
    }


    // Window
    GLUTWindowUnrecPtr gwin;

    GLint glvp[4];

    glGetIntegerv(GL_VIEWPORT, glvp);

    gwin = GLUTWindow::create();
    {
        gwin->setGlutId(winid);
        gwin->setSize( glvp[2], glvp[3] );
        
        win = gwin;

        win->addPort( vp );

        win->init();
    }

    // Action
    rentravact = RenderAction::create();

    rentravact->setFrustumCulling(false);

#ifdef OLD_BBQ
    outOfCoreDataSource_ = new BbqOutOfCoreDataSource();

    bool rc = outOfCoreDataSource_->initialize( 
//        "data/ps.bbq", 
        "data/ps_com.bbq", 
        650.0f, 
        0.0f, 
        1.0f );

    fprintf(stderr, "ds::init %d\n", rc);

    const BbqDataSourceInformation &bbqInfo = 
        outOfCoreDataSource_->getInformation();
   

    fprintf(stderr, "%d %d\n",
            bbqInfo.levelCount,
            bbqInfo.nodeCount);

    fprintf(stderr, "%d %d %d\n",
            bbqInfo.heightTileSize,
            bbqInfo.heightSampleCount[0],
            bbqInfo.heightSampleCount[1]);

    fprintf(stderr, "%f %f %f\n",
            bbqInfo.heightScale,
            bbqInfo.heightOffset,
            bbqInfo.sampleSpacing);

    

    const int maxResidentNodeCount = 5000;

    terrain_ = new BbqTerrainEngine();

    rc = terrain_->initialize(outOfCoreDataSource_, maxResidentNodeCount);

    fprintf(stderr, "t::init %d\n", rc);

    terrainRenderOptions_.showSkirts           = false;
    terrainRenderOptions_.showSwitchDistance   = true;
#endif


    // tball
    Vec3f pos;

    pos.setValues(min[0] + ((max[0] - min[0]) * 0.5), 
                  min[1] + ((max[1] - min[1]) * 0.5), 
                  max[2] + ( max[2] - min[2] ) * 1.5 );
    
    float scale = (max[2] - min[2] + max[1] - min[1] + max[0] - min[0]) / 6;

    Pnt3f tCenter(min[0] + (max[0] - min[0]) / 2,
                  min[1] + (max[1] - min[1]) / 2,
                  min[2] + (max[2] - min[2]) / 2);

    fprintf(stderr, "Startpos : %f %f %f\n", pos[0], pos[1], pos[2]);

    tball.setMode            (Trackball::OSGObject);
    tball.setStartPosition   (pos, true           );
    tball.setSum             (true                );
    tball.setTranslationMode (Trackball::OSGFree  );
    tball.setTranslationScale(scale * 400         );
    tball.setRotationCenter  (tCenter             );

    tScale = scale * 400;

    fprintf(stderr, "tscale %f\n", tScale);

//    pos.setValues(0, 400, 0);

    projectPnt(pos, -42.5, 172.5, 10);

    tCenter.setValues(min[0] + (max[0] - min[0]) / 2,
                      min[1] + (max[1] - min[1]) / 2,
                      min[2] + (max[2] - min[2]) / 2);
    
    fprintf(stderr, "Startpos : %f %f %f\n", pos[0], pos[1], pos[2]);

    tcamball.setMode            (Trackball::OSGObject);
    tcamball.setStartPosition   (pos, true           );
    tcamball.setSum             (true                );
    tcamball.setTranslationMode (Trackball::OSGFree  );
    tcamball.setTranslationScale(100                 );
    tcamball.setRotationCenter  (tCenter             );

    m1c.setIdentity();

    m1c[3][1] = 5000;

    m4c.setIdentity();

    m4c[3][1] = 5000;


    pActiveTBall = &tball;

    commitChanges();

    // run...
    glutMainLoop();
    
    return 0;
}

