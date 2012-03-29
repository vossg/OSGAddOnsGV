#include <iostream>
#include <sstream>
#include <fstream>
#include <sofa/helper/ArgumentParser.h>
#include <sofa/simulation/common/xml/initXml.h>
#include <sofa/simulation/common/Node.h>
#include <sofa/helper/system/PluginManager.h>

#include <sofa/simulation/tree/TreeSimulation.h>
#include <sofa/component/init.h>
#include <sofa/component/misc/ReadState.h>
#include <sofa/component/misc/CompareState.h>
#include <sofa/helper/Factory.h>
#include <sofa/helper/BackTrace.h>
#include <sofa/helper/system/FileRepository.h>
#include <sofa/helper/system/SetDirectory.h>

#include <sofa/gui/PickHandler.h>


#include <sofa/helper/system/atomic.h>

// OpenSG 2
//#include "OSGSofaViewer.h"
#include "OSGSofaVisualModelPolicy.h"
#include "OSGSofaVisualVisitor.h"
#include "OSGSofaShadowGraph.h"
#include "OSGConfig.h"
#include "OSGGLUT.h"
#include "OSGGLUTWindow.h"
#include "OSGSimpleGeometry.h"
//#include "OSGQT4Window.h"
#include "OSGSimpleSceneManager.h"
#include "OSGThreadManager.h"
#include "OSGIntersectAction.h"
#include "OSGTriangleIterator.h"


using namespace sofa::gui;
using namespace sofa::simulation;

sofa::simulation::Node::SPtr sofa_root;

// The SimpleSceneManager to manage simple applications
OSG::SimpleSceneManagerRefPtr mgr = NULL;
sofa::gui::PickHandler        oPick;

enum ACTION_STATE { START_ACTION, DO_ACTION, END_ACTION };

// forward declaration so we can have the interesting stuff upfront
int setupGLUT( int *argc, char *argv[] );

//
// GLUT callback functions
//

// react to size changes
void reshape( int w, int h )
{
    if (!mgr) return;
    mgr->resize(w, h);
    glutPostRedisplay();
}

// redraw the window
void display( void )
{
    if (!mgr) return;
    if (!sofa_root) return;


    SReal dt = sofa_root->getDt();
    if (sofa_root->getAnimate())
    {
        sofa::simulation::getSimulation()->animate(sofa_root.get(), dt);
        sofa::simulation::getSimulation()->updateVisual(sofa_root.get());
    }

    

    mgr->redraw();
    glutPostRedisplay();
}

void startAction(int x, int y)
{
}

void doAction(int x, int y)
{
}

void endAction(int x, int y)
{
}






void sofaPick( int button, int state, int x, int y )
{

    OSG::UInt16 but = OSG::SimpleSceneManager::NoButton;

    OSG::Window*  win = mgr->getWindow();
    sofa::gui::MousePosition mousepos;
    OSG::Viewport* vp = (win)?win->getPort(0): NULL;
    if (!vp) return;

    mousepos.screenWidth = vp->calcPixelWidth();
    mousepos.screenHeight = vp->calcPixelHeight();
    mousepos.x = x;
    mousepos.y = y;



    OSG::Line l = mgr->calcViewRay( x, y);
    ::sofa::defaulttype::Vec3d pos, dir;
    OSG::Pnt3f p = l.getPosition();
    OSG::Vec3f d = l.getDirection();
    pos[0] = p[0]; 
    pos[1] = p[1]; 
    pos[2] = p[2]; 
    dir[0] = d[0]; 
    dir[1] = d[1]; 
    dir[2] = d[2]; 

    oPick.activateRay(mousepos.screenWidth,mousepos.screenHeight, sofa_root.get());
    oPick.updateRay(pos, dir);
    oPick.updateMouse2D(mousepos);

    switch (state)
    {
        case  GLUT_DOWN:
                if (button == GLUT_LEFT_BUTTON)
                {
                    oPick.handleMouseEvent(PRESSED, LEFT);
                }
                else if (button == GLUT_RIGHT_BUTTON) 
                {
                    oPick.handleMouseEvent(PRESSED, RIGHT);
                } 
                else if (button == GLUT_MIDDLE_BUTTON)
                {
                    oPick.handleMouseEvent(PRESSED, MIDDLE);
                }
                break;

        case GLUT_UP:

                if (button == GLUT_LEFT_BUTTON)
                {
                    oPick.handleMouseEvent(RELEASED, LEFT);
                }
                else if (button == GLUT_RIGHT_BUTTON) 
                {
                    oPick.handleMouseEvent(RELEASED, RIGHT);
                }
                else if (button == GLUT_MIDDLE_BUTTON)
                {
                    oPick.handleMouseEvent(RELEASED, MIDDLE);
                }
                break;
        
                default:
                break;
     }
     //moveRayPickInteractor(e->x(), e->y());
     l = mgr->calcViewRay( x, y);
     p = l.getPosition();
     d = l.getDirection();
     pos[0] = p[0]; 
     pos[1] = p[1]; 
     pos[2] = p[2]; 
     dir[0] = d[0]; 
     dir[1] = d[1]; 
     dir[2] = d[2]; 

     oPick.updateRay(pos, dir);

}


// react to mouse motions with pressed buttons
void simpleMotion( int x, int y )
{

    if (!mgr) return;

    oPick.deactivateRay();
    mgr->mouseMove(x, y);
    glutPostRedisplay();
}

void attachMotion( int x, int y )
{
    if (!mgr) return;

    if (sofa_root->getAnimate())
    {
        sofaPick(GLUT_LEFT_BUTTON, GLUT_DOWN, x , y);
    }
    glutPostRedisplay();
}

void removeMotion( int x, int y )
{
    if (!mgr) return;

    if (sofa_root->getAnimate())
    {
        sofaPick(GLUT_RIGHT_BUTTON, GLUT_DOWN, x , y);
    }
    glutPostRedisplay();
}


// react to mouse button presses
void mouse( int button, int state, int x, int y )
{
    if (!mgr) return;
    int currModifiers = glutGetModifiers();

    // normal trackball 
    if (currModifiers != GLUT_ACTIVE_SHIFT)
    {
        if (state)
            mgr->mouseButtonRelease(button, x, y);
        else
            mgr->mouseButtonPress(button, x, y);
        glutMotionFunc(simpleMotion);
        glutPostRedisplay();
        return;
    }

    if (currModifiers == GLUT_ACTIVE_SHIFT)
    {
        if (!sofa_root->getAnimate())
        {
            return;
        }

        sofaPick(button, state, x , y);
        if (state == GLUT_DOWN)
        {
            if (button == GLUT_LEFT_BUTTON)
            {
                glutMotionFunc(attachMotion);
            }
            else if (button == GLUT_RIGHT_BUTTON)
                glutMotionFunc(removeMotion);

        }
        else
        {
            glutMotionFunc(simpleMotion);
        }
        glutPostRedisplay();
        return;
    }
    else
    {
        glutMotionFunc(simpleMotion);
    }

    glutPostRedisplay();
}


// react to keys
void keyboard(unsigned char k, int x, int y)
{
    switch(k)
    {
        case 27:  
        {
            // clean up global variables
	        if (sofa_root)
		        getSimulation()->unload(sofa_root);
            sofa_root = NULL;

            mgr   = NULL;

        
            OSG::osgExit();
            exit(0);
        }
        break;

        case 'a':
        {
            sofa_root->setAnimate(!sofa_root->getAnimate());
        }
        break;

        case 'r':
        {
            getSimulation()->reset(sofa_root.get());
        }
        break;

        case 'q':
        {
            mgr->showAll();
        }
        break;

        case 's':
        {
            mgr->setStatistics(!mgr->getStatistics());
        }
        break;
    }
}

// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    
    int winid = glutCreateWindow("OpenSG with SOFA Application");
    
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(simpleMotion);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // call the redraw function whenever there's nothing else to do
    glutIdleFunc(display);
    return winid;
}



int main(int argc, char** argv)
{

    // OSG init
    OSG::osgInit(argc, argv);


	sofa::helper::BackTrace::autodump();

	sofa::core::ExecParams::defaultInstance()->setAspectID(0);

    // Load visual model policy to replace SOFA visual models with OpenSG
    sofa::gui::qt::viewer::OSGModelPolicy policy;
    policy.load();
  
	std::string fileName ;
	std::string sofaDataPath = "";

	std::vector<std::string> files;


	sofa::helper::parse(&files, "OpenSG with SOFA")
	.option(&sofaDataPath,'s',"sofapath","sofa path")
	(argc,argv);

    // GLUT init
    int winid = setupGLUT(&argc, argv);

    {    
        OSG::GLUTWindowRecPtr gwin = OSG::GLUTWindow::create();
        gwin->setGlutId(winid);
        gwin->init();

        // Create a new simulation
	    sofa::simulation::setSimulation(new sofa::simulation::tree::TreeSimulation());
 
        // Initialize sofa components    
	    sofa::component::init();

        // Initialize sofa XML
	    sofa::simulation::xml::initXml();
	
	    if (!files.empty())
		    fileName = files[0];


        if (sofaDataPath.empty()  == false)
        {
            sofa::helper::system::DataRepository.addFirstPath(sofaDataPath);
            sofa::helper::system::DataRepository.addFirstPath(sofaDataPath + "/examples");
            sofa::helper::system::DataRepository.addFirstPath(sofaDataPath + "/share");

        }




	    if (fileName.empty())
	    {
            fileName = "Demos/caduceus.scn";
	    }
		fileName = sofa::helper::system::DataRepository.getFile(fileName);

        sofa_root = sofa::core::objectmodel::SPtr_dynamic_cast<sofa::simulation::Node>
        ( sofa::simulation::getSimulation()->load(fileName.c_str()));

	    if (sofa_root==NULL)
	    {
		    sofa_root = sofa::simulation::getSimulation()->createNewGraph("");
    	}
        oPick.setPickingMethod(PickHandler::RAY_CASTING);

        RegisterOperation("Attach").add< sofa::gui::AttachOperation  >();
        RegisterOperation("Fix"   ).add< sofa::gui::FixOperation     >();
        RegisterOperation("Incise").add< sofa::gui::InciseOperation  >();
        RegisterOperation("Remove").add< sofa::gui::TopologyOperation>();

        //Add to each button of the mouse an operation
        oPick.changeOperation(sofa::gui::LEFT,   "Attach");
        oPick.changeOperation(sofa::gui::MIDDLE, "Incise");
        oPick.changeOperation(sofa::gui::RIGHT,  "Remove");

	    sofa::simulation::getSimulation()->init(sofa_root.get());

   
        oPick.init(sofa_root.get());



        mgr = OSG::SimpleSceneManager::create();
        mgr->setWindow(gwin);

        sofa::core::visual::VisualParams* vparams = sofa::core::visual::VisualParams::defaultInstance();
        sofa::simulation::OSGVisualInitVisitor init_visitor( vparams);
        sofa_root->execute(&init_visitor);
        
        sofa::simulation::getSimulation()->reset( sofa_root.get() );
        sofa::component::visualmodel::OSGSofaShadowGraph shadowGraph; 

   

        OSG::commitChanges();

        mgr->setRoot(shadowGraph.build(sofa_root, false).get());
        mgr->showAll();

    }
    
    glutMainLoop();

	return 0;


}

