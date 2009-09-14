// OpenSG Tutorial Example: Loading
//
// This example shows how to load a scene file using OpenSG.
// The supported formats right now are VRML97, OBJ, OFF and RAW, so just
// calling this program with a scene file as a parameter should load the scene
// file.
//

// Headers
#include <OSGGLUT.h>
#include <OSGConfig.h>

#include <OSGSimpleGeometry.h>
#include <OSGGLUTWindow.h>
#include <OSGSimpleSceneManager.h>
#include <OSGAction.h>
#include <OSGTransform.h>
#include <OSGCoredNodePtr.h>

#include <OSGLabelForeground.h>
#include <OSGTextLabel.h>
#include <OSGIconLabel.h>

// New Headers

// The SimpleSceneManager to manage simple applications
OSG::SimpleSceneManager*   g_mgr             = NULL;
OSG::GLUTWindowRefPtr      g_win             = NULL;
OSG::NodeRefPtr            g_scene           = NULL;
OSG::LabelForegroundRefPtr g_labelForeground = NULL;

// forward declaration so we can have the interesting stuff upfront
int  setupGLUT( int *argc, char *argv[] );
void onExit   ( void );

OSG::LabelTransitPtr createTextLabel(int idx)
{
    OSG::TextLabelRefPtr textLabel = OSG::TextLabel::create();
    std::string txt;

    switch (idx) {
        case 0:  txt = "label 0";  break;
        case 1:  txt = "label 1";  break;
        case 2:  txt = "label 2";  break;
        case 3:  txt = "label 3";  break;
        default: txt = "label";
    }

    textLabel->setText(txt);

    return OSG::LabelTransitPtr(textLabel);
}

OSG::LabelTransitPtr createIconLabel(void)
{
    OSG::IconLabelRefPtr iconLabel = OSG::IconLabel::create();

#if 0
    iconLabel->setSize(OSG::Vec2f(64,64));
#else
    iconLabel->setSize(OSG::Vec2f(0.2,0.2));
#endif

#if 0
    // FIXME: find better image example + check that in...
    iconLabel->setFilename("/home/spindler/Projects/Tonky/Data/paul.jpg");
#else
    std::cerr << "specify IcoenLabel.image" << std::endl;
    OSG::ImageRefPtr img = OSG::Image::create();
    img->read("/home/spindler/Projects/Tonky/Data/paul.jpg");
    iconLabel->setImage(img);
#endif
    return OSG::LabelTransitPtr(iconLabel);
}

void updateLabelParams(OSG::Label* label, int idx)
{
    OSG::Real32  orient    = 0;
    OSG::Vec2f   pixOffset = OSG::Vec2f(0,0);
    OSG::Color4f bgColor   = OSG::Color4f(OSG::osgRand(), 
                                          OSG::osgRand(), 
                                          OSG::osgRand(), 
                                          OSG::osgRand());

    OSG::Vec2f  marg(0,0);

    switch (idx) {
        case 0: orient    = 45.0;  bgColor = OSG::Color4f(0,0,0,0); break;
        case 1: orient    = 15.0;  marg = OSG::Vec2f(2,2); break;
        case 2: orient    = 45.0;  marg = OSG::Vec2f(4,4); break;
        case 3: orient    = 85.0;  marg = OSG::Vec2f(20,20);  break;
        default: ;
    }
    label->setOrientation(orient);
    label->setPixelOffset(pixOffset);
    label->setBorderColor(OSG::Color4f(OSG::osgRand(), 
                                       OSG::osgRand(), 
                                       OSG::osgRand(), 1.0));
    label->setBgColor(bgColor);
    label->setBorderOffset(OSG::Vec2f(0,0));
    label->setShadowOffset(OSG::Vec2f(4,4));
    label->setMargin(marg);
}

OSG::NodeTransitPtr createLabeledTorus(OSG::Vec3f trans, int idx)
{
    OSG::NodeTransitPtr  node  = OSG::Node::create();
    OSG::TransformRefPtr xform = OSG::Transform::create();
    OSG::Matrix          mat;

    // --- setup transform ------------------
    mat.setIdentity();
    mat.setTranslate(trans);
    xform->setMatrix(mat);
    node->setCore(xform);


    // --- setup label ----------------------
    OSG::NodeRefPtr  labelNode = OSG::Node::create();
    OSG::LabelRefPtr label     = 
        (idx) ? createTextLabel(idx) : createIconLabel();

    updateLabelParams(label, idx);
    labelNode->setCore(label);

    // --- add torus ------------------------
    labelNode->addChild(OSG::makeTorus(.5, 2, 16, 16));

    node->addChild(labelNode);
    return node;
}

OSG::NodeTransitPtr createLabeledScene(void)
{
    OSG::NodeTransitPtr scene = OSG::Node::create();
    scene->setCore(OSG::Group::create());

#if 0
    scene->addChild(createLabeledTorus(OSG::Vec3f( 1, 3, 0), 0));
    scene->addChild(createLabeledTorus(OSG::Vec3f( 2, 3, 0), 0));
    scene->addChild(createLabeledTorus(OSG::Vec3f( 4, 3, 0), 0));
#endif
    scene->addChild(createLabeledTorus(OSG::Vec3f( 3, 3, 0), 0));
    scene->addChild(createLabeledTorus(OSG::Vec3f( 3,-3, 0), 1));
    scene->addChild(createLabeledTorus(OSG::Vec3f(-3, 3, 0), 2));
    scene->addChild(createLabeledTorus(OSG::Vec3f(-3,-3, 0), 3));

    return scene;
}

OSG::NodeTransitPtr createScene(void)
{
    // create the scene:
    OSG::TransformTransitPtr xform = OSG::Transform::create();

    OSG::Matrix mat;
    mat.setTranslate(OSG::Vec3f(2,0,0));
    xform->setMatrix(mat);

    OSG::NodeTransitPtr scene = OSG::Node::create();
    scene->addChild(createLabeledScene());
    scene->setCore(xform);

    OSG::commitChanges();

    return scene;
}


// Initialize GLUT & OpenSG and set up the scene
int main(int argc, char **argv)
{
    // OSG init
    OSG::osgInit(argc,argv);

    // GLUT init
    int winid = setupGLUT(&argc, argv);

    // the connection between GLUT and OpenSG
    g_win= OSG::GLUTWindow::create();
    g_win->setGlutId(winid);
    g_win->init();

    g_scene = createScene();

    // create the SimpleSceneManager helper
    g_mgr = new OSG::SimpleSceneManager;
    g_mgr->setUseTraversalAction(true);

    // tell the manager what to manage
    g_mgr->setWindow(g_win );
    g_mgr->setRoot  (g_scene);

    // add LabelForeground!!!
    g_labelForeground = OSG::LabelForeground::create();
    g_mgr->addForeground(g_labelForeground);

    // show the whole scene
    g_mgr->showAll();

    // GLUT main loop
    glutMainLoop();

    return 0;
}

//
// GLUT callback functions
//

// redraw the window
void display(void)
{
    g_mgr->idle();
    g_mgr->redraw();
    OSG::Thread::getCurrentChangeList()->clear();
}

// react to size changes
void reshape(int w, int h)
{
    g_mgr->resize(w, h);
    glutPostRedisplay();
}

// react to mouse button presses
void mouse(int button, int state, int x, int y)
{

    if (state)
        g_mgr->mouseButtonRelease(button, x, y);
    else
        g_mgr->mouseButtonPress(button, x, y);
        
    glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion(int x, int y)
{

    g_mgr->mouseMove(x, y);
    glutPostRedisplay();
}

// react to keys
void keyboard(unsigned char k, int , int )
{
    switch(k)
    {
        case 27:
            onExit();
            OSG::osgExit();
            exit(0);
        break;
        case 'f':
            g_mgr->setNavigationMode(OSG::Navigator::FLY);
        break;
        case 't':
            g_mgr->setNavigationMode(OSG::Navigator::TRACKBALL);
        break;
        case 'q':
            g_mgr->setStatistics(true);
        break;
        case 'w':
            g_mgr->setStatistics(false);
        break;
        case 'r':
        {
            bool useTrav = !g_mgr->getUseTraversalAction();
            g_mgr->setUseTraversalAction(useTrav);
            printf("Using %s action.\n", useTrav ? "render traversal" : "render");
        }
        break;
    }
}

// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    int winid = glutCreateWindow("OpenSG");
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    return winid;
}

void onExit(void)
{
    delete g_mgr;

    g_labelForeground = NULL;
    g_scene           = NULL;
    g_win             = NULL;
}
