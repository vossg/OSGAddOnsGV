#include <OSGAddOnsConfig.h>

#include <iostream>

#include <OSGGLUT.h>

#include <OSGFieldContainerFactory.h>
#include <OSGVector.h>
#include <OSGQuaternion.h>
#include <OSGMatrix.h>
#include <OSGMatrixUtility.h>
#include <OSGBoxVolume.h>
#include <OSGLine.h>
#include <OSGNode.h>
#include <OSGGroup.h>
#include <OSGThread.h>
#include <OSGTransform.h>
#include <OSGAction.h>
#include <OSGRenderAction.h>
#include <OSGSimpleGeometry.h>
#include <OSGSceneFileHandler.h>
#include <OSGChunkOverrideGroup.h>
#include <OSGPolygonChunk.h>
#include <OSGDirectionalLight.h>

#include "OSGViewport.h"
#include "OSGCamera.h"
#include "OSGWindow.h"
#include "OSGGLUTWindow.h"
#include "OSGCamera.h"
#include "OSGPerspectiveCamera.h"
#include "OSGSolidBackground.h"
//#include "OSGUniformBackground.h"
#include "OSGOSGWriter.h"
#include "OSGChangeList.h"
#include "OSGIOStream.h"
#include "OSGGeoFunctions.h"
#include "OSGGraphOp.h"
#include "OSGGraphOpFactory.h"
#include "OSGMultiCore.h"

#include "OSGMaterialGroup.h"
#include "OSGChunkMaterial.h"
#include "OSGMaterialChunk.h"
#include "OSGSimpleSHLChunk.h"

#include "OSGTrackball.h"

using namespace OSG;

RenderActionRefPtr rentravact;

NodeUnrecPtr  root;

//NodeUnrecPtr  file;

PerspectiveCameraUnrecPtr cam;
ViewportUnrecPtr vp;
WindowUnrecPtr win;

TransformUnrecPtr cam_trans;
TransformUnrecPtr scene_trans;

PolygonChunkUnrecPtr pPoly;
bool                 bPolyActive = false;
ChunkOverrideGroupUnrecPtr pCOver;
SimpleSHLChunkUnrecPtr pSHL = NULL;

Trackball tball;

bool move_obj = false;

int mouseb = 0;
int lastx=0, lasty=0;

Quaternion oldq;
Vec3f      oldv;

float fMorph = 0.5f;


void display(void)
{
    Matrix m1, m2, m3;
    Quaternion q1;

    tball.getRotation().getValue(m3);

    q1.setValue(m3);

    m1.setRotate(q1);
    
//    std::cout << "TBROT" << std::endl << tball.getRotation() << endl;
//    std::cout << "M3" << std::endl << m3 << std::endl;
//    std::cout << "Q1" << std::endl << q1 << std::endl;
//    std::cout << "M1" << std::endl << m1 << std::endl;

//  m1.setRotate( tball.getRotation() );
    m2.setTranslate( tball.getPosition() );
    
//std::cout << "Pos: " << tball.getPosition() << ", Rot: " << tball.getRotation() << std::endl;

//    std::cout << tball.getRotation() << std::endl;

    m1.mult( m2 );

//    std::cerr << m1 << std::endl;
    
    m1 = tball.getFullTrackballMatrix();

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

//    win->render(renact);

//    std::cerr << "------------- AR START --------------" << std::endl;

//    Thread::getCurrentChangeList()->dump();
}

void reshape( int w, int h )
{
    std::cerr << "Reshape: " << w << "," << h << std::endl;
    win->resize( w, h );
}


void
animate(void)
{
    glutPostRedisplay();
}

// tballall stuff


void
motion(int x, int y)
{   
    Real32 w = win->getWidth(), h = win->getHeight();
    

    Real32  a = -2. * ( lastx / w - .5 ),
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

void
mouse(int button, int state, int x, int y)
{
    if ( state == 0 )
    {
        switch ( button )
        {
        case GLUT_LEFT_BUTTON:  break;
        case GLUT_MIDDLE_BUTTON:tball.setAutoPosition(true);
                                break;
        case GLUT_RIGHT_BUTTON:     tball.setAutoPositionNeg(true);
                                break;
        }
        mouseb |= 1 << button;
    }
    else if ( state == 1 )
    {
        switch ( button )
        {
        case GLUT_LEFT_BUTTON:  break;
        case GLUT_MIDDLE_BUTTON:tball.setAutoPosition(false);
                                break;
        case GLUT_RIGHT_BUTTON:     tball.setAutoPositionNeg(false);
                                break;
        }       
        mouseb &= ~(1 << button);
    }
    lastx = x;
    lasty = y;
}

void
vis(int visible)
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

            root = NULL;
//            file = NULL;
            cam = NULL;
            vp = NULL;
            win = NULL;
            cam_trans = NULL;
            scene_trans = NULL;
            pPoly = NULL;
            pCOver = NULL;
            pSHL = NULL;

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
        case 'z':   
            pPoly->setFrontMode(GL_POINT);
            pPoly->setBackMode(GL_POINT);
            std::cerr << "PolygonMode: Point." << std::endl;
            break;
        case 'x':   
            pPoly->setFrontMode(GL_LINE);
            pPoly->setBackMode(GL_LINE);
            std::cerr << "PolygonMode: Line." << std::endl;
            break;
        case 'c':   
            pPoly->setFrontMode(GL_FILL);
            pPoly->setBackMode(GL_FILL);
            std::cerr << "PolygonMode: Fill." << std::endl;
            break;
        case 'p':
        {
            if(bPolyActive == true)
            {
                OSG_ASSERT(pCOver->subChunk(pPoly) == true);
                bPolyActive = false;
            }
            else
            {
                OSG_ASSERT(pCOver->addChunk(pPoly) == true);
                bPolyActive = true;
            }
            break;
        }
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
        {
            if(fMorph > 0.f)
            {
                fMorph -= 0.01;
            }
            else
            {
                fMorph = 0.f;
            }
            pSHL->updateUniformVariable("fMorph", fMorph);
        }
        break;

        case '>':   
        {
            if(fMorph < 1.f)
            {
                fMorph += 0.01;
            }
            else
            {
                fMorph = 1.f;
            }
            pSHL->updateUniformVariable("fMorph", fMorph);
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
//                m = scene_trans->getSFMatrix()->getValue();
                tball.setMode( OSG::Trackball::OSGCamera );

            }
            else
            {
                puts("moving camera");
//                m = cam_trans->getSFMatrix()->getValue();
                tball.setMode( OSG::Trackball::OSGObject );
            }
            
//            q.setValue(m);
            tball.setStartPosition( v, true );
            tball.setStartRotation( q, true );

//            std::cout << q << std::endl;
//            std::cout << v << std::endl;

//            std::cout << " " << m[3][0] << " " << m[3][1] << " " << m[3][2] << std::endl;
            
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
    int winid = glutCreateWindow("OpenSG");
    glutKeyboardFunc(key);
    glutVisibilityFunc(vis);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);       
    glutMouseFunc(mouse);   
    glutMotionFunc(motion); 
    
    glutIdleFunc(display);  

    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
//    glEnable( GL_DEPTH_TEST );
//    glEnable( GL_LIGHTING );
//    glEnable( GL_LIGHT0 );
//    glFrontFace(GL_CW);
    

    // OSG

    SceneFileHandler::the()->print();

    // create the graph

    // beacon for camera and light  
    NodeUnrecPtr b1n = Node::create();
    GroupUnrecPtr b1 = Group::create();

    b1n->setCore( b1 );

    // transformation
    NodeUnrecPtr t1n = Node::create();
    TransformUnrecPtr t1 = Transform::create();

    t1n->setCore( t1 );
    t1n->addChild( b1n );

    cam_trans = t1;

    // light
    
    NodeUnrecPtr dlight = Node::create();
    DirectionalLightUnrecPtr dl = DirectionalLight::create();

    dlight->setCore( dl );
//    dlight->setCore( Group::create() );
    
    dl->setAmbient( .0, .0, .0, 1 );
    dl->setDiffuse( .8, .8, .8, .8 );
    dl->setDirection(0,0,1);
    dl->setBeacon( b1n);

    // root
    root = Node::create();
    GroupUnrecPtr gr1 = Group::create();

    root->setCore( gr1 );
    root->addChild( t1n );
    root->addChild( dlight );

    // Load the file

    NodeUnrecPtr file = NULL;
    
    if(argc > 1)
        file = SceneFileHandler::the()->read(argv[1], NULL);
    
    if ( file == NULL )
    {
        std::cerr << "Couldn't load file, ignoring" << std::endl;

        file = makeSphere(4, 2.0);

    }

#if 0
    GeometryPtr pGeo = cast_dynamic<GeometryPtr>(file->getCore());
    
    if(pGeo == NULL && file->getNChildren() != 0)
    {
        pGeo = cast_dynamic<GeometryPtr>(file->getChild(0)->getCore());
    }

    if(pGeo == NULL)
    {
        fprintf(stderr, "no geo\n");
    }

    GraphOp *op = GraphOpFactory::the()->create("Stripe");

    op->traverse(file);
#endif
//   createOptimizedPrimitives(pGeo);
//    createSharedIndex(pGeo);
    
    Thread::getCurrentChangeList()->commitChanges();

//    file->dump();

#if 0
    char *outFileName = "/tmp/foo1.osg";

    OSG::IndentFileOutStream outFileStream(outFileName);

    if( !outFileStream )
    {
        std::cerr << "Can not open output stream to file: "
                  << outFileName << std::endl;
        return -1;
    }

    std::cerr << "STARTING PRINTOUT:" << std::endl;
    OSGWriter writer( outFileStream, 4 );

    writer.write( file );

    outFileStream.close();

    OSG::SceneFileHandler::the()->write(file, "/tmp/foo.osb");
#endif    

    file->updateVolume();

//    subRefCP(file);

//    return 0;


    Vec3f min,max;
    file->getVolume().getBounds( min, max );
    

    std::cout << "Volume: from " << min << " to " << max << std::endl;


//    NodePtr pChunkOverNode = Node::create();
  

//    pChunkOverNode->setCore(pCOver);
//    pChunkOverNode->addChild(file);

    MultiCoreUnrecPtr pMCore = MultiCore::create();

    pCOver      = ChunkOverrideGroup::create();
    scene_trans = Transform::create();

    pMCore->addCore(scene_trans);
    pMCore->addCore(pCOver     );

    NodeUnrecPtr sceneTrN = Node::create();

    sceneTrN->setCore(pMCore);
    sceneTrN->addChild(file);

    MaterialGroupUnrecPtr pMG = MaterialGroup::create();

    ChunkMaterialUnrecPtr pChunkMat = ChunkMaterial::create();

    pMG->setMaterial(pChunkMat);
    pMCore->addCore(pMG);


    MaterialChunkUnrecPtr pMatChunk = MaterialChunk::create();

    pMatChunk->editDiffuse().setValuesRGBA(0.8, 0.8, 0.8, 1.0);
    
    pChunkMat->addChunk(pMatChunk);

    pSHL = SimpleSHLChunk::create();
    
    if(!pSHL->readVertexProgram("m.vp"))
        fprintf(stderr, "Couldn't read vertex program 'm.vp'\n");

    pSHL->addUniformVariable("fMorph", 0.5f);

    pChunkMat->addChunk(pSHL);

    dlight->addChild(sceneTrN);

    std::cerr << "Tree: " << std::endl;
//  root->dump();

    // Camera
    
    cam = PerspectiveCamera::create();

    cam->setBeacon( b1n );
    cam->setFov( osgDegree2Rad( 90 ) );
    cam->setNear( 0.1 );
    cam->setFar( 100000 );

    // Background
    SolidBackgroundUnrecPtr bkgnd = SolidBackground::create();

    bkgnd->setColor(Color3f(0,0,1));
    
    // Viewport

    vp = Viewport::create();

    vp->setCamera( cam );
    vp->setBackground( bkgnd );
    vp->setRoot( root );
    vp->setSize( 0,0, 1,1 );

//    vp->dump();

    // Window
    std::cout << "GLUT winid: " << winid << std::endl;

    GLUTWindowUnrecPtr gwin;

    GLint glvp[4];

    glGetIntegerv( GL_VIEWPORT, glvp );

    gwin = GLUTWindow::create();
    gwin->setGlutId(winid);
    gwin->setSize( glvp[2], glvp[3] );

    win = gwin;

    win->addPort( vp );

    win->init();

    // Action
    
    rentravact = RenderAction::create();
//    renact->setFrustumCulling(false);


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

    tball.setMode( Trackball::OSGObject );
    tball.setStartPosition( pos, true );
    tball.setSum( true );
    tball.setTranslationMode( Trackball::OSGFree );
    tball.setTranslationScale(scale);
    tball.setRotationCenter(tCenter);

//    fprintf(stderr, "%d\n", MFNodePtr          ::getClassType().getId());
//    fprintf(stderr, "%d\n", MFFieldContainerPtr::getClassType().getId());

//    MFNodePtr          ::getClassType().dump();
//    MFFieldContainerPtr::getClassType().dump();

    // run...
    
    pPoly = PolygonChunk::create();

    pCOver->subChunk(pPoly);

//    OSG::addRef(pPoly);

#if 0
    GroupNodePtr pGr = GroupNodePtr::create();

    NodePtr  pN;
    GroupPtr pG;

    fprintf(stderr, "A\n");

    if(pGr == NULL)
    {
    }

    fprintf(stderr, "B\n");

    if(pGr == pN)
    {
    }

    fprintf(stderr, "C\n");

    if(pGr == pG)
    {
    }

    pGr = NULL;
#endif

    glutMainLoop();
    
    return 0;
}

