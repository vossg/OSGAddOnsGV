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

// Activate the OpenSG namespace
OSG_USING_NAMESPACE

// The SimpleSceneManager to manage simple applications
SimpleSceneManager*   g_mgr             = NULL;
GLUTWindowRefPtr      g_win             = NULL;
NodeRefPtr            g_scene           = NULL;
LabelForegroundRefPtr g_labelForeground = NULL;

// forward declaration so we can have the interesting stuff upfront
int  setupGLUT( int *argc, char *argv[] );
void onExit   ( void );

LabelTransitPtr createTextLabel(int idx)
{
    TextLabelRefPtr textLabel = TextLabel::create();
    std::string txt;

    switch (idx) {
        case 0:  txt = "label 0";  break;
        case 1:  txt = "label 1";  break;
        case 2:  txt = "label 2";  break;
        case 3:  txt = "label 3";  break;
        default: txt = "label";
    }

    textLabel->setText(txt);

    return LabelTransitPtr(textLabel);
}

LabelTransitPtr createIconLabel(void)
{
    IconLabelRefPtr iconLabel = IconLabel::create();

#if 0
    iconLabel->setSize(Vec2f(64,64));
#else
    iconLabel->setSize(Vec2f(0.2,0.2));
#endif

#if 0
    // FIXME: find better image example + check that in...
    iconLabel->setFilename("/home/spindler/Projects/Tonky/Data/paul.jpg");
#else
    std::cerr << "specify IcoenLabel.image" << std::endl;
    ImageRefPtr img = Image::create();
    img->read("/home/spindler/Projects/Tonky/Data/paul.jpg");
    iconLabel->setImage(img);
#endif
    return LabelTransitPtr(iconLabel);
}

void updateLabelParams(Label* label, int idx)
{
    Real32  orient    = 0;
    Vec2f   pixOffset = Vec2f(0,0);
    Color4f bgColor   = Color4f(osgRand(), osgRand(), osgRand(), osgRand());

    Vec2f  marg(0,0);

    switch (idx) {
        case 0: orient    = 45.0;  bgColor = Color4f(0,0,0,0); break;
        case 1: orient    = 15.0;  marg = Vec2f(2,2); break;
        case 2: orient    = 45.0;  marg = Vec2f(4,4); break;
        case 3: orient    = 85.0;  marg = Vec2f(20,20);  break;
        default: ;
    }
    label->setOrientation(orient);
    label->setPixelOffset(pixOffset);
    label->setBorderColor(Color4f(osgRand(), osgRand(), osgRand(), 1.0));
    label->setBgColor(bgColor);
    label->setBorderOffset(Vec2f(0,0));
    label->setShadowOffset(Vec2f(4,4));
    label->setMargin(marg);
}

NodeTransitPtr createLabeledTorus(Vec3f trans, int idx)
{
    NodeTransitPtr  node  = Node::create();
    TransformRefPtr xform = Transform::create();
    Matrix          mat;

    // --- setup transform ------------------
    mat.setIdentity();
    mat.setTranslate(trans);
    xform->setMatrix(mat);
    node->setCore(xform);


    // --- setup label ----------------------
    NodeRefPtr  labelNode = Node::create();
    LabelRefPtr label     = (idx) ? createTextLabel(idx) : createIconLabel();

    updateLabelParams(label, idx);
    labelNode->setCore(label);

    // --- add torus ------------------------
    labelNode->addChild(makeTorus(.5, 2, 16, 16));

    node->addChild(labelNode);
    return node;
}

NodeTransitPtr createLabeledScene(void)
{
    NodeTransitPtr scene = Node::create();
    scene->setCore(Group::create());

#if 0
    scene->addChild(createLabeledTorus(Vec3f( 1, 3, 0), 0));
    scene->addChild(createLabeledTorus(Vec3f( 2, 3, 0), 0));
    scene->addChild(createLabeledTorus(Vec3f( 4, 3, 0), 0));
#endif
    scene->addChild(createLabeledTorus(Vec3f( 3, 3, 0), 0));
    scene->addChild(createLabeledTorus(Vec3f( 3,-3, 0), 1));
    scene->addChild(createLabeledTorus(Vec3f(-3, 3, 0), 2));
    scene->addChild(createLabeledTorus(Vec3f(-3,-3, 0), 3));

    return scene;
}

NodeTransitPtr createScene(void)
{
    // create the scene:
    TransformTransitPtr xform = Transform::create();

    Matrix mat;
    mat.setTranslate(Vec3f(2,0,0));
    xform->setMatrix(mat);

    NodeTransitPtr scene = Node::create();
    scene->addChild(createLabeledScene());
    scene->setCore(xform);

    OSG::commitChanges();

    return scene;
}


// Initialize GLUT & OpenSG and set up the scene
int main(int argc, char **argv)
{
    // OSG init
    osgInit(argc,argv);

    // GLUT init
    int winid = setupGLUT(&argc, argv);

    // the connection between GLUT and OpenSG
    g_win= GLUTWindow::create();
    g_win->setGlutId(winid);
    g_win->init();

    g_scene = createScene();

    // create the SimpleSceneManager helper
    g_mgr = new SimpleSceneManager;
    g_mgr->setUseTraversalAction(true);

    // tell the manager what to manage
    g_mgr->setWindow(g_win );
    g_mgr->setRoot  (g_scene);

    // add LabelForeground!!!
    g_labelForeground = LabelForeground::create();
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
    Thread::getCurrentChangeList()->clear();
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
            g_mgr->setNavigationMode(Navigator::FLY);
        break;
        case 't':
            g_mgr->setNavigationMode(Navigator::TRACKBALL);
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
