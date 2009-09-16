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

#include "OSGImage.h"
#include "OSGPolygonForeground.h"
#include "OSGSimpleMaterial.h"
#include "OSGVisitSubTree.h"

#include "OSGFBOViewport.h"
#include "OSGFrameBufferObject.h"
#include "OSGRenderBuffer.h"
#include "OSGTextureBuffer.h"

#include "OSGTextureObjChunk.h"
#include "OSGTextureEnvChunk.h"

#include "OSGRRTStage.h"
#include "OSGRTInfoAttachment.h"

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

TextureObjChunkUnrecPtr tx1o   = NULL;
TextureEnvChunkUnrecPtr tx1e   = NULL;
RRTStageUnrecPtr        pStage = NULL;


void display(void)
{
    Matrix m1, m2, m3;
    Quaternion q1;

    tball.getRotation().getValue(m3);

    q1.setValue(m3);

    m1.setRotate(q1);
    
    m2.setTranslate( tball.getPosition() );
    
    m1.mult( m2 );

    if(move_obj == true)
    {
        scene_trans->editSFMatrix()->setValue( m1 );
    }
    else
    {
        cam_trans->editSFMatrix()->setValue( m1 );
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

            root        = NULL;
            file        = NULL;
            cam         = NULL;
            vp          = NULL;
            win         = NULL;
            cam_trans   = NULL;
            scene_trans = NULL;
            tx1o        = NULL;
            tx1e        = NULL;
            pStage      = NULL;

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

        case '1':
        {
            if(pStage != NULL)
            {
                pStage->setSize(63, 63);
            }
        }
        break;
        
        case '2':
        {
            if(pStage != NULL)
            {
                pStage->setSize(64, 64);
            }
        }
        break;

        case '3':
        {
            if(pStage != NULL)
            {
                pStage->setSize(128, 128);
            }
        }
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


int doMain (int argc, char **argv)
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

    fprintf(stderr, "create b1n: %p\n", b1n.get());

    b1n->setCore( b1 );

    // transformation
    NodeUnrecPtr      t1n = Node::create();
    TransformUnrecPtr t1  = Transform::create();

    fprintf(stderr, "create t1n: %p\n", t1n.get());

    t1n->setCore (t1 );
    t1n->addChild(b1n);

    cam_trans = t1;

    // light
    
    NodeUnrecPtr             dlight = Node::create();
    DirectionalLightUnrecPtr dl     = DirectionalLight::create();

    {
        dlight->setCore(dl);
        
//        dlight->setCore(Group::create());
        
        dl->setAmbient( .0, .0, .0, 1 );
        dl->setDiffuse( .8, .8, .8, .8 );
        dl->setDirection(1,0,0);
//        dl->setBeacon( b1n);
    }

    fprintf(stderr, "create dlight: %p\n", dlight.get());

    // root
    root              = Node:: create();
    GroupUnrecPtr gr1 = Group::create();

    root->setCore (gr1   );
    root->addChild(t1n   );
    root->addChild(dlight);

    fprintf(stderr, "create root: %p\n", root.get());

    // Load the file

    NodeUnrecPtr file = NULL;
    
    if(argc > 1)
    {
        file = SceneFileHandler::the()->read(argv[1], NULL);
    }

    if(file == NULL)
    {
        std::cerr << "Couldn't load file, ignoring" << std::endl;

        file = makeSphere(4, 2.0);
    }

    fprintf(stderr, "create file: %p\n", file.get());

    Thread::getCurrentChangeList()->commitChanges();

    file->updateVolume();


    Vec3f min,max;
    file->getVolume().getBounds(min, max);
    
    std::cout << "Volume: from " << min << " to " << max << std::endl;

                 scene_trans = Transform::create();
    NodeUnrecPtr sceneTrN    = Node::create();

    sceneTrN->setCore (scene_trans);
    sceneTrN->addChild(file       );

    fprintf(stderr, "create sceneTrN: %p\n", sceneTrN.get());


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


    tx1o = TextureObjChunk::create();
    tx1e = TextureEnvChunk::create();


#if 0
    UChar8 imgdata[] =
    {  
        64,64,64, 128,128,128, 192,192,192, 255,255,255 
    };
#endif

    ImageUnrecPtr pImg = Image::create();

    pImg->set(Image::OSG_RGB_PF, 128, 128);

    tx1o->setImage    (pImg      ); 
    tx1o->setMinFilter(GL_LINEAR );
    tx1o->setMagFilter(GL_LINEAR );
    tx1o->setWrapS    (GL_CLAMP );
    tx1o->setWrapT    (GL_CLAMP );
    tx1e->setEnvMode  (GL_REPLACE);

    SimpleMaterialUnrecPtr mat = SimpleMaterial::create();
    
    mat->setDiffuse(Color3f(1,1,1));
    mat->setLit    (false         );
    mat->addChunk  (tx1o          );
    mat->addChunk  (tx1e          );

    PolygonForegroundUnrecPtr pFG =  PolygonForeground::create();

    pFG->setMaterial(mat);

    MFPnt2f *pPos = pFG->editMFPositions();
    MFVec3f *pTex = pFG->editMFTexCoords();

    pPos->push_back(Pnt2f(0.0f, 0.0f));
    pPos->push_back(Pnt2f(0.3f, 0.0f));
    pPos->push_back(Pnt2f(0.3f, 0.3f));
    pPos->push_back(Pnt2f(0.0f, 0.3f));

    pTex->push_back(Vec3f(0.0f, 0.0f, 0.0f));
    pTex->push_back(Vec3f(1.0f, 0.0f, 0.0f));
    pTex->push_back(Vec3f(1.0f, 1.0f, 0.0f));
    pTex->push_back(Vec3f(0.0f, 1.0f, 0.0f));

    vp->addForeground(pFG);



    // RRT

    FrameBufferObjectUnrecPtr pFBO = FrameBufferObject::create();

    TextureBufferUnrecPtr pTexBuffer   = TextureBuffer::create();
    RenderBufferUnrecPtr  pDepthBuffer = RenderBuffer ::create();

    pDepthBuffer->setInternalFormat(GL_DEPTH_COMPONENT24   );

    pTexBuffer->setTexture(tx1o);

    pFBO->setSize(128, 128);
    
    pFBO->setColorAttachment(pTexBuffer, 0);
    pFBO->setDepthAttachment(pDepthBuffer );

    pFBO->editMFDrawBuffers()->clear();
    pFBO->editMFDrawBuffers()->push_back(GL_COLOR_ATTACHMENT0_EXT);



    NodeUnrecPtr pStageNode = Node::create();

    fprintf(stderr, "create stageNode: %p\n", pStageNode.get());

    pStage = RRTStage::create();

    pStage->setRenderTarget (pFBO);
    pStage->setTextureTarget(tx1o);
  
    pStage->setSplitThreads(false);
//    pStage->setTiled       (true);
    pStage->setTiled       (false);
    pStage->setWidth       (128  );
    pStage->setHeight      (128  );


//    pStage->setRayTracingRoot(file);



    RTInfoAttachmentUnrecPtr pRTInfo = RTInfoAttachment::create();

    file->addAttachment(pRTInfo);



    VisitSubTreeUnrecPtr pVisit     = VisitSubTree::create();
    NodeUnrecPtr         pVisitNode = Node::create();

    fprintf(stderr, "create pVisitNode: %p\n", pVisitNode.get());

    pVisit    ->setSubTreeRoot(dlight);
//    pVisit    ->setSubTreeRoot(file);
    pVisitNode->setCore       (pVisit);


    pStageNode->setCore (pStage    );
    pStageNode->addChild(pVisitNode);


    root->addChild(pStageNode);


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

    fprintf(stderr, "create stageNode: %p %d %d\n", 
            pStageNode.get(),
            pStageNode->getRefCount(),
            pStageNode->getWeakRefCount());

   
    return 0;
}

int main (int argc, char **argv)
{
    doMain(argc, argv);

    // run...
    glutMainLoop();

    return 0;
}
