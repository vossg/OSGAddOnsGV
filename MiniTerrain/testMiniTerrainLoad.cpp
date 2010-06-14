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
#include "OSGMaterialGroup.h"
#include "OSGSimpleMaterial.h"

#include "mini/miniload.h"
#include "mini/miniio.h"
#include "mini/minicache.h"

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

static miniload  *terrain;

int request_callback(      int            col, 
                           int            row,
                     const unsigned char *mapfile, 
                           int            hlod,
                     const unsigned char *texfile,
                           int            tlod,
                     const unsigned char *fogfile,
                     void                *data,
                     databuf             *hfield,
                     databuf             *texture,
                     databuf             *fogmap)
{
#if 0
    fprintf(stderr, "%d %d\n", col, row);
    
    fprintf(stderr, "%d : %s\n", hlod, mapfile);
    fprintf(stderr, "%d : %s\n", tlod, texfile);
    fprintf(stderr, "%d : %s\n", 0, fogfile);
#endif

    if(hfield != NULL || texture != NULL || fogmap != NULL)
    {
        if(mapfile != NULL && hfield != NULL)
            hfield->loaddata((char *)mapfile);

        if(texfile != NULL && texture != NULL)
            texture->loaddata((char *)texfile);
    }
    else
    {
        int present = 1;
        
        if(mapfile != NULL) 
            present &= checkfile((char *) mapfile);

        if(texfile != NULL) 
            present &= checkfile((char *) texfile);

        if(fogfile != NULL) 
            present &= checkfile((char *) fogfile);
        
        return present;
    }

    return 1;
}

int hw_request_callback(      int            col,
                              int            row,
                        const unsigned char *mapfile,
                              int            hlod,
                        const unsigned char *texfile,
                              int            tlod,
                        const unsigned char *fogfile,
                              void          *data,
                              databuf       *hfield,
                              databuf       *texture,
                              databuf       *fogmap)
{
    int present;
    int sw_normals=0;

#if 0
    fprintf(stderr, "%d %d\n", col, row);
    
    fprintf(stderr, "%d : %s\n", hlod, mapfile);
    fprintf(stderr, "%d : %s\n", tlod, texfile);
    fprintf(stderr, "%d : %s\n", 0, fogfile);
#endif
    
    if(hfield!=NULL && texture!=NULL && fogmap!=NULL)
    {
        if(mapfile != NULL) 
            hfield->loadPNMdata((char *)mapfile);

        if(texfile != NULL)
            if(sw_normals == 0) 
                texture->loadPPMcompressed((char *)texfile);
//            else 
//                texture->loadPPMnormalized((char *)texfile,basepath4);

        if(fogfile != NULL) 
            fogmap->loadPNMdata((char *) fogfile);
    }
    else
    {
        present=1;

        if(mapfile != NULL) 
            present &= checkfile((char *)mapfile);

        if(texfile != NULL)
            present &= checkfile((char *)texfile);

        if(fogfile != NULL)
            present &= checkfile((char *)fogfile);

        return(present);
    }
    
    return(1);
}

void terrainRenderCB(DrawEnv *pEnv)
{
    const float res = 1.0E4f;

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

#if 1
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
                  0.f, //90.f, //fovy,
                  (float) win->getWidth() / win->getHeight(),
                  1000.f, //nearp,
                  100000000.f,
                  0); //farp);


    if(bWireframe == true)
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
#else

    float ex = 0.000000;
    float ey = 27.223131;
    float ez = 0.000000;
    
    float sina = 0.422618;
    float tanp = 0.000000;
    float cosa = 0.906308;

    static const float fovy=90.0f,nearp=0.1f,farp=100000.0f;
    static const float scale=1.f; //00.0f; // 1:100

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(ex,ey,-ez,ex+sina,ey-tanp,-ez-cosa,0.0f,1.0f,0.0f);

    // update vertex arrays
    terrain->draw(res,
                  ex,ey,-ez,
                  sina,-tanp,-cosa,
                  0.0f,1.0f,0.0f,
                  fovy,(float) win->getWidth() / win->getHeight(),
                  nearp/scale,farp/scale,
                  0 /*upd*/);
#endif

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void display(void)
{
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

void reshape(int w, int h)
{
    std::cerr << "Reshape: " << w << "," << h << std::endl;

    win->resize(w, h);
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
    
    if(mouseb & (1 << GLUT_LEFT_BUTTON))
    {
        tball.updateRotation(a, b, c, d);     
    }
    else if(mouseb & (1 << GLUT_MIDDLE_BUTTON))
    {
        tball.updatePosition(a, b, c, d);     
    }
    else if(mouseb & (1 << GLUT_RIGHT_BUTTON))
    {
        tball.updatePositionNeg(a, b, c, d);  
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
    if(visible == GLUT_VISIBLE) 
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
    switch(key)
    {
        case 27:    
            win = NULL;
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
            
            tball.setStartPosition(v, true);
            tball.setStartRotation(q, true);
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
#if 0
        std::cerr << "Couldn't load file, ignoring" << std::endl;

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
                                            6378137. * 0.95,
                                            6356752.3142 * 0.95));
    

        geoRef->addChild(pEll);
    
        geoRef->addChild(makeEllipsoidAxis(18, 
                                           36, 
                                           6378137. * 1.05,
                                           6356752.3142 * 1.05));

        file = geoRef; //makeSphere(4, 2.0);
#endif

        file = makeSphere(4, 2.0);
    }

    Thread::getCurrentChangeList()->commitChanges();

    file->updateVolume();


    Vec3f min,max;
    file->getVolume().getBounds(min, max);
    
    std::cout << "Volume: from " << min << " to " << max << std::endl;

/*
    min[0] = -500;
    min[1] = 0;
    min[2] = -500;

    max[0] = 500;
    max[1] = 100;
    max[2] = 500;
 */

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
        cam->setNear( 1000 );
        cam->setFar( 100000000 );
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

    int cols = 1;
    int rows = 1;
    
    terrain = new miniload(); 

    terrain->configure_mipmaps(0);

    terrain->setloader(hw_request_callback,
                       NULL);

#if 1
    static const char *abasepath1 = 
        "/home/gerrit/SoftwareBuild/Hawaii/data/HawaiiTileset/tiles";
    static const char *abasepath2 =
        "/home/gerrit/SoftwareBuild/Hawaii/data/HawaiiTileset/landsat";

    static const float viewx = 0.f; //-560309.0f;
    static const float viewy = 0.f; //72234.0f;

    // vertical exaggeration
    static const float exaggeration=1.5f; // exaggeration=150%

    float tscale = 1.f; //00.0f; // 1:100

    // number of columns and rows
    cols = 32;
    rows = 24;

    // approximate value of one arc-second in meters
    static float arcsec[3];

    // resampling output parameters
    static float outparams[5];

    std::string szTilePath;
    std::string szLandSatPath;

    if(argc > 1)
    {
        fprintf(stderr, "loading from %s\n", argv[1]);

        szTilePath   .assign(argv[1]);
        szLandSatPath.assign(argv[1]);

        szTilePath    += "/data/HawaiiTileset/tiles";
        szLandSatPath += "/data/HawaiiTileset/landsat";

        abasepath1 = szTilePath   .c_str();
        abasepath2 = szLandSatPath.c_str();
    }


    // load resampled tiles
    terrain->load(cols,rows, // number of columns and rows

                  abasepath1,
                  abasepath2,
                  NULL, // directories for tiles and textures (and no fogmaps)

                  -viewx,-viewy, // horizontal offset in arc-seconds
                  0.0f, // no vertical offset
                  1.0f,1.0f,1.0f, // no horizontal stretching
                  exaggeration,tscale, // vertical exaggeration and global scale
                  0.0f,0.0f, // no fog parameters required
                  0.0f, // choose default minimum resolution
                  0.0f, // disable base offset safety
                  outparams, // geometric output parameters
                  arcsec); // one arcsec in meters
#endif

#if 1
    fprintf(stderr, "%f %f %f\n",
            terrain->getminitile()->getcenterx(),
            terrain->getminitile()->getcentery(),
            terrain->getminitile()->getcenterz());

    fprintf(stderr, "%f %f \n",
            terrain->getminitile()->getsizex(),
            terrain->getminitile()->getsizez());
#endif            

    max[0] = terrain->getminitile()->getcenterx();
    max[1] = terrain->getminitile()->getcentery();
    max[2] = terrain->getminitile()->getcenterz();

    min[0] = 
        terrain->getminitile()->getcenterx() -
        (terrain->getminitile()->getsizex() / 2.f);

    min[1] = terrain->getminitile()->getcentery();

    min[2] = 
        terrain->getminitile()->getcenterz() -
        (terrain->getminitile()->getsizez() / 2.f);

    // tball
    Vec3f pos;

    pos.setValues(min[0] + ((max[0] - min[0]) * 0.5), 
                  min[1] + ((max[1] - min[1]) * 0.5), 
                  max[2] + ( max[2] - min[2] ) * 1.5 );
    
    float scale = (max[2] - min[2] + max[1] - min[1] + max[0] - min[0]) / 6;

    //scale = 100;

    Pnt3f tCenter(min[0] + (max[0] - min[0]) / 2,
                  min[1] + (max[1] - min[1]) / 2,
                  min[2] + (max[2] - min[2]) / 2);


    fprintf(stderr, "Startpos : %f %f %f\n", 
            pos[0], pos[1], pos[2]);
    fprintf(stderr, "tcenter  : %f %f %f\n", 
            tCenter[0], tCenter[1], tCenter[2]);

    tball.setMode            (Trackball::OSGObject);
    tball.setStartPosition   (pos, true           );
    tball.setSum             (true                );
    tball.setTranslationMode (Trackball::OSGFree  );
    tball.setTranslationScale(scale               );
    tball.setRotationCenter  (tCenter             );

    return 0;
}

int main(int argc, char **argv)
{
    doMain(argc, argv);

    // run...
    glutMainLoop();
    
    return 0;
}

