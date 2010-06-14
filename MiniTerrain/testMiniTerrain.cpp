#if __GNUC__ >= 4 || __GNUC_MINOR__ >=3
#pragma GCC diagnostic warning "-Wold-style-cast"
#endif

#include <OSGConfig.h>

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
#include "OSGAlgorithmStage.h"
#include "OSGCallbackAlgorithm.h"

#include "mini/minitile.h"

using namespace OSG;

RenderAction *rentravact = NULL;

NodeUnrecPtr  root = NULL;
NodeUnrecPtr  file = NULL;

PerspectiveCameraUnrecPtr cam = NULL;
ViewportUnrecPtr          vp  = NULL;
WindowUnrecPtr            win = NULL;

TransformUnrecPtr cam_trans   = NULL;
TransformUnrecPtr scene_trans = NULL;

Trackball tball;

bool move_obj = false;

int mouseb = 0;
int lastx  = 0;
int lasty  = 0;

Quaternion oldq;
Vec3f      oldv;
Matrix     m1c;

bool bWireframe = false;

static minitile  *terrain;

void terrainRenderCB(DrawEnv *pEnv)
{
    const float res = 1.0E4f;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    Vec3f vTo(0.f, 0.f, -1.f);
    Vec3f vUp(0.f, 1.f,  0.f);

    Matrix mde = pEnv->getCameraViewing();

    glLoadMatrixf(&(mde[0][0]));

    vTo = m1c * vTo;
    vUp = m1c * vUp;

    if(bWireframe == true)
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);


    terrain->draw(res,
                  //exr,ey,ezr,
                  m1c[3][0], m1c[3][1], m1c[3][2],
                  //dxr,dy,dzr,
                  //mde[2][0], mde[2][1], mde[2][2],
                  vTo[0], vTo[1], vTo[2],
                  //ux,uy,uz,
                  //mde[1][0], mde[1][1], mde[1][2],
                  vUp[0], vUp[1], vUp[2],
                  90.f, //fovy,
                  (float) win->getWidth() / win->getHeight(),
                  0.1f, //nearp,
                  100000.f,
                  0); //farp);


    if(bWireframe == true)
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void display(void)
{
    Matrix m2, m3;
    Quaternion q1;

    m1c.setIdentity();

    tball.getRotation().getValue(m3);

    q1.setValue(m3);

    m1c.setRotate(q1);
    
    m2.setTranslate( tball.getPosition() );
    
    m1c.mult( m2 );

    m1c = tball.getFullTrackballMatrix();

    if(move_obj == true)
    {
        scene_trans->editSFMatrix()->setValue( m1c );
    }
    else
    {
        cam_trans->editSFMatrix()->setValue( m1c );
    }

    commitChanges();

    win->render(rentravact);
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
        tball.updateRotation( a, b, c, d );     
    }
    else if ( mouseb & ( 1 << GLUT_MIDDLE_BUTTON ) )
    {
        tball.updatePosition( a, b, c, d );     
    }
    else if ( mouseb & ( 1 << GLUT_RIGHT_BUTTON ) )
    {
        tball.updatePositionNeg( a, b, c, d );  
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
                tball.setAutoPosition(true);
                break;

            case GLUT_RIGHT_BUTTON:     
                tball.setAutoPositionNeg(true);
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
                tball.setAutoPosition(false);
                break;

            case GLUT_RIGHT_BUTTON:     
                tball.setAutoPositionNeg(false);
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
            delete rentravact;

            root = NULL;
            file = NULL;
            cam = NULL;
            vp  = NULL;
            win = NULL;
            cam_trans   = NULL;
            scene_trans = NULL;

            osgExit(); 
            exit(0);

        case 'a':   
            glDisable( GL_LIGHTING );
            std::cerr << "Lighting disabled." << std::endl;
            break;
        case 's':   
            glEnable( GL_LIGHTING );
            std::cerr << "Lighting enabled." << std::endl;
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
        case 'w':
            bWireframe = !bWireframe;
            break;

        case ' ':
        {
            Matrix     m;
            Quaternion q;
            Vec3f      v;

            q = oldq;
            v = oldv;

            oldq = tball.getRotation();
            oldv = tball.getPosition();

            move_obj = ! move_obj;
            if ( move_obj )
            {
                puts("moving object");
                tball.setMode( OSG::Trackball::OSGCamera );

            }
            else
            {
                puts("moving camera");
                tball.setMode( OSG::Trackball::OSGObject );
            }
            
            tball.setStartPosition( v, true );
            tball.setStartRotation( q, true );
        }
        break;
    }
}


int doMain(int argc, char **argv)
{
    osgInit(argc,argv);
    
    // GLUT init

    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
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
    
#if 0
    if(argc > 1)
    {
        file = SceneFileHandler::the()->read(argv[1], NULL);
    }
#endif

    if(file == NULL)
    {
//        std::cerr << "Couldn't load file, ignoring" << std::endl;

        file = makeSphere(4, 2.0);
    }

    Thread::getCurrentChangeList()->commitChanges();

    file->updateVolume();


    Vec3f min,max;
    file->getVolume().getBounds(min, max);
    
    std::cout << "Volume: from " << min << " to " << max << std::endl;

    min[0] = -500;
    min[1] = 0;
    min[2] = -500;

    max[0] = 500;
    max[1] = 100;
    max[2] = 500;

            scene_trans = Transform::create();
    NodeUnrecPtr sceneTrN    = Node::create();

    sceneTrN->setCore (scene_trans);
    sceneTrN->addChild(file       );

    AlgorithmStageUnrecPtr pAlgoStage = AlgorithmStage::create();

    pAlgoStage->setInheritedTarget(true);
    pAlgoStage->setProjectionMode (AlgorithmStage::Ignore);
    pAlgoStage->setCopyViewing    (true);

    CallbackAlgorithmUnrecPtr pAlgo = CallbackAlgorithm::create();

    pAlgo->setCallback(&terrainRenderCB, "");

    pAlgoStage->setAlgorithm(pAlgo);

    NodeUnrecPtr pAlgoNode = Node::create();

    pAlgoNode->setCore(pAlgoStage);

    sceneTrN->addChild(pAlgoNode);

    dlight->addChild(sceneTrN);

    // Camera
    
    cam = PerspectiveCamera::create();
    {
        cam->setBeacon( b1n );
        cam->setFov( osgDegree2Rad( 90 ) );
        cam->setNear( 0.1 );
        cam->setFar( 100000 );
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
        win->activate();
    }

    // Action
    rentravact = RenderAction::create();
    
    rentravact->setFrustumCulling(false);

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
    tball.setTranslationScale(scale               );
    tball.setRotationCenter  (tCenter             );

    const unsigned char hfield [] = "data/Yukon.map.pgm";
    const unsigned char texture[] = "data/Yukon.texmap.ppm";
    const unsigned char fogmap [] = "data/Yukon.fogmap.pgm";

    const unsigned char *hfields  = hfield;
    const unsigned char *textures = texture;
    const unsigned char *fogmaps  = fogmap;

    std::string szHField;
    std::string szTexture;
    std::string szFogMap;

    if(argc > 1)
    {
        fprintf(stderr, "loading from %s\n", argv[1]);

        szHField .assign(argv[1]);
        szTexture.assign(argv[1]);
        szFogMap .assign(argv[1]);

        szHField  += "/data/Yukon.map.pgm";
        szTexture += "/data/Yukon.texmap.ppm";
        szFogMap  += "/data/Yukon.fogmap.pgm";

        hfields  = reinterpret_cast<const unsigned char *>(szHField .c_str());
        textures = reinterpret_cast<const unsigned char *>(szTexture.c_str());
        fogmaps  = reinterpret_cast<const unsigned char *>(szFogMap .c_str());
    }
    else
    {
        hfields  = hfield;
        textures = texture;
        fogmaps  = fogmap;
    }
    
    const int cols = 1;
    const int rows = 1;

    const float xdim = 1000.0f;
    const float zdim = 1000.0f;

    scale=0.33f;

    const float lambda   = 0.15f;
    const float displace = 1.0f;
    
    const float emission    = 0.01f;
    const float attenuation = 0.33f;
    
    const float fogR = 0.75f;
    const float fogG = 0.75f;
    const float fogB = 1.0f;
    
    terrain = new minitile(&hfields,
                           &textures,
                           cols,rows,
                           xdim,zdim,scale,
                           0.0f,0.0f,0.0f,
                           &fogmaps,
                           lambda,displace,emission,
                           attenuation,
                           fogR,fogG,fogB);


    fprintf(stderr, "%f %f %f\n",
            terrain->getcenterx(),
            terrain->getcentery(),
            terrain->getcenterz());

    fprintf(stderr, "%f %f \n",
            terrain->getsizex(),
            terrain->getsizez());
 
    return 0;
}

int main(int argc, char **argv)
{
    doMain(argc, argv);

    // run...
    glutMainLoop();
    
    return 0;
}

