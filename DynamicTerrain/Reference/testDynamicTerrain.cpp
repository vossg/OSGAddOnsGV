// OpenSG Example: Cubes
//
// GLUT is used for window handling
#include <OSGGLUT.h>
// General OpenSG configuration, needed everywhere
#include <OSGAddOnsConfig.h>
// Methods to create simple geometry: boxes, spheres, tori etc.
#include <OSGSimpleGeometry.h>
// The GLUT-OpenSG connection class
#include <OSGGLUTWindow.h>
// A little helper to simplify terrainNode management and interaction
#include <OSGSimpleSceneManager.h>
#include <OSGSimpleGeometry.h>
#include <OSGImageFileHandler.h>
#include <OSGTextureChunk.h>

#include "OSGDynamicTerrain.h"
#include "OSGTerrainTools.h"

#include "OSGSinusHeightDataSource.h"
#include "OSGImageHeightDataSource.h"

// little helper to create a random number between a and b
#define random( a, b ) \
	( ( rand() / float(RAND_MAX) ) * ( (b) - (a) ) + (a) )

// Activate the OpenSG namespace
using namespace OSG;

// The SimpleSceneManager to manage simple applications
SimpleSceneManager *mgr;
NodePtr terrainNode;

DynamicTerrainPtr	terrainCore;

#ifdef _DEBUG
const bool testPerformance = false;
const bool testHugeTerrain = false;
#else
const bool testPerformance = true;
const bool testHugeTerrain = true;
#endif

bool wireframe = false;
bool showStats = false;

// forward declaration so we can have the interesting stuff upfront
int setupGLUT( int *argc, char *argv[] );
bool reloadShader();
TransformPtr markerTrans;

TextureChunkPtr makeTexture( const char* texname )
{
	ImagePtr image = ImageFileHandler::the().read(texname);
	TextureChunkPtr texChunkPtr   = TextureChunk::create();

	beginEditCP( texChunkPtr );
	{
		texChunkPtr->setImage( image );
		texChunkPtr->setWrapS( GL_CLAMP );
		texChunkPtr->setWrapT( GL_CLAMP );
		texChunkPtr->setWrapR( GL_CLAMP );

		texChunkPtr->setMinFilter( GL_LINEAR );
		texChunkPtr->setMagFilter( GL_LINEAR );
		texChunkPtr->setEnvMode( GL_MODULATE );
	}

	endEditCP( texChunkPtr );

	return texChunkPtr;
}

// Initialize GLUT & OpenSG and set up the terrainNode
int main( int argc, char**argv )
{
	// OSG init
	osgInit(argc,argv);

	//osgLogP->setLogLevel( LOG_INFO );

	// GLUT init
	int winid = setupGLUT(&argc, argv);

	// the connection between GLUT and OpenSG
	GLUTWindowPtr gwin= GLUTWindow::create();
	gwin->setId(winid);
	gwin->init();

	// Create the DynamicTerrain Data:
	if( argc < 5 )
	{
		std::cerr << "usage: testDynamicTerrain heightmapfile texturefile heightscale sampleDistance [levelsize]" << std::endl;
		return 1;
	}  

	ImagePtr heightData = ImageFileHandler::the().read( argv[ 1 ] );

	if( heightData == NullFC )
	{
		std::cerr << "Could not open heightmap " << argv[ 1 ] << std::endl;
		return 2;
	}

	ImagePtr textureData = ImageFileHandler::the().read( argv[ 2 ] );

	if( textureData == NullFC )
	{
		std::cerr << "Could not open texture " << argv[ 2 ] << std::endl;
		return 3;
	}

	float heightScale = atof( argv[ 3 ] );
	float sampleDistance = atof( argv[ 4 ] );

	std::cout << "Setting HeightScale to " << heightScale << std::endl;
	std::cout << "Setting SampleDistance to " << sampleDistance << std::endl;

	int levelSize = 255;

	if( argc >= 5 )
	{
		levelSize = atoi( argv[ 5 ] );
		std::cout << "Setting LevelSize to " << levelSize << std::endl;
	}

	terrainCore = DynamicTerrain::create();

	beginEditCP( terrainCore );

	terrainCore->setLevelSize( levelSize );

	terrainCore->setSampleDistance( sampleDistance );
	terrainCore->setHeightDataScale( heightScale );

	terrainCore->setHeightData( heightData );		
	terrainCore->setTextureData( textureData );
	terrainCore->setMaterial( getDefaultMaterial() );	

	endEditCP( terrainCore );

	terrainNode = Node::create();
	
	beginEditCP( terrainNode );
	terrainNode->setCore( terrainCore );
	endEditCP( terrainNode );

	// create the SimpleSceneManager helper
	mgr = new SimpleSceneManager;

	// Create the Scenegraph:
	NodePtr sceneNode = Node::create();
	GroupPtr sceneCore = Group::create();

	// add the marker object:
	markerTrans = Transform::create();

	GeometryPtr markerGeometry = makeSphereGeo( 4, 5.0 );
	NodePtr markerNode = Node::create();

	SimpleMaterialPtr m1 = SimpleMaterial::create();

	// when everything is changed, not setting the mask is ok
	beginEditCP(m1);
	{
		m1->setAmbient      (Color3f(0.2,0.2,0.2));
		m1->setDiffuse      (Color3f(0.8,0.1,0.2));
		m1->setEmission     (Color3f(0.0,0.0,0.0));
		m1->setSpecular     (Color3f(1.0,1.0,1.0));
		m1->setShininess    (10);

		/*
		Transparent objects are rendered last and sorted from back to
		front, but only objectwise. Thus transparecy within an object is
		not handled correctly.
		*/
		m1->setTransparency (0);

		/*
		The SimpleGeometry objects do not contain colors, turn
		ColorMaterial off.
		*/
		m1->setColorMaterial(GL_NONE);
	}

    endEditCP ( m1 );
	
	beginEditCP( markerGeometry );
	markerGeometry->setMaterial( m1 );
	endEditCP( markerGeometry );

	beginEditCP( markerNode );
	markerNode->setCore( markerGeometry );
	endEditCP( markerNode );
	

	beginEditCP( sceneNode );
	sceneNode->setCore( sceneCore );
	sceneNode->addChild( terrainNode );
	sceneNode->addChild( markerNode );
	endEditCP( sceneNode );

	// tell the manager what to manage
	mgr->setWindow( gwin );
	mgr->setRoot  ( sceneNode );

	// show the whole terrainNode
	//mgr->showAll();

	WalkNavigator *walker = mgr->getNavigator()->getWalkNavigator();

	walker->setGroundDistance( 30 );
	walker->setPersonDimensions( 3, 1, 1 );

	mgr->setNavigationMode( Navigator::WALK );
	mgr->getNavigator()->setMotionFactor( 1.0f );
	mgr->getNavigator()->set( Pnt3f( 20, 10, 20 ), Pnt3f( 510, 8, 500 ), Vec3f( 0, 1, 0 ) );
	mgr->getCamera()->setFar( 6000.0f );

	// GLUT main loop
	glutMainLoop();

	return 0;
}

//
// GLUT callback functions
//

// redraw the window

int frameCount = 0;
Time lastLogTime = 0;
float logTimeDelta = 2.0f;

void display()
{	
	Time currentTime = getSystemTime();

	if( testPerformance )
	{
		if( currentTime - lastLogTime > logTimeDelta )
		{
			float fps = ( float )frameCount / logTimeDelta;

			//std::cout << "Current FPS = " << fps << " !\n";

			frameCount = 0;
			lastLogTime = currentTime;
		}	
	}

	// draw the terrainNode:
	mgr->redraw();

	frameCount++;

	glutPostRedisplay();
}

// react to size changes
void reshape( int w, int h )
{
	mgr->resize( w, h );
	glutPostRedisplay();
}

// react to mouse button presses
void mouse( int button, int state, int x, int y )
{
	if( state )
	{
		mgr->mouseButtonRelease( button, x, y );
	}
	else
	{
		mgr->mouseButtonPress( button, x, y );
	}

	glutPostRedisplay();
}

// react to mouse motions with pressed buttons
void motion( int x, int y )
{
	mgr->mouseMove( x, y );
	glutPostRedisplay();
}

bool reloadShader()
{
	const char* shaderFilename = "cpu_terrain_shader.glsl";

	std::string shaderTextBuffer;

	if( !loadTextFile( shaderFilename, shaderTextBuffer ) )
	{
		return false;
	}

	const char* vertexShaderTag = "[VertexShader]";

	std::string::size_type startOfVertexShader = shaderTextBuffer.find( vertexShaderTag, 0 );

	if( startOfVertexShader == std::string::npos )
	{
		PWARNING << "[DynamicTerrain] " << shaderFilename << "(0) : error : Missing VertexShader-Tag (" << vertexShaderTag << ")\n";
		return false;
	}

	const char* fragmentShaderTag = "[FragmentShader]";

	std::string::size_type startOfFragmentShader = shaderTextBuffer.find( fragmentShaderTag, startOfVertexShader );

	if( startOfFragmentShader == std::string::npos )
	{
		PWARNING << "[DynamicTerrain] " << shaderFilename << "(0) : error : Missing FragmentShader-Tag (" << fragmentShaderTag << ")\n";
		return false;
	}

	assert( startOfFragmentShader > startOfVertexShader );

	const size_t vertexShaderSize = startOfFragmentShader - startOfVertexShader - strlen( vertexShaderTag );
	const size_t fragmentShaderSize = shaderTextBuffer.size() - startOfFragmentShader - strlen( fragmentShaderTag );

	startOfVertexShader		+= strlen( vertexShaderTag );
	startOfFragmentShader	+= strlen( fragmentShaderTag );

	std::string vertexShaderTextBuffer( shaderTextBuffer, startOfVertexShader, vertexShaderSize );
	std::string fragmentShaderTextBuffer( shaderTextBuffer, startOfFragmentShader, fragmentShaderSize );

	beginEditCP( terrainCore, DynamicTerrain::CpuFragmentProgramFieldMask | DynamicTerrain::CpuVertexProgramFieldMask );
	terrainCore->setCpuVertexProgram( vertexShaderTextBuffer );
	terrainCore->setCpuFragmentProgram( fragmentShaderTextBuffer );
	endEditCP( terrainCore );

	return true;
}

// react to keys
void keyboard(unsigned char k, int x, int y)
{
	GLint current = GL_LINE;

	switch( k )
	{
	case 27:  
		exit(1);

	case 'a':   
		mgr->setHighlight( terrainNode );
		break;

	case 's':
		showStats = !showStats;
		mgr->setStatistics( showStats );
		break;

	case 'w': 
		wireframe = !wireframe;

		// todo: this is a hack! no direct manipulation of GL states allowed.. (but this is just a simple test app so i do it anyway)
		if( wireframe )
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} 
		else 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		break;

	case ' ':
		terrainCore->setDisableUpdate( !terrainCore->getDisableUpdate() );
		//updateTerrain = !updateTerrain;
		break;

	case 'l':   
		mgr->useOpenSGLogo();
		break;

	case 'f':   
		mgr->setNavigationMode(Navigator::FLY);
		break;

	case 't':   
		mgr->setNavigationMode(Navigator::TRACKBALL);
		break;

	case '1':
		mgr->getCamera()->setFar( 20000.0f );		
		break;

	case '2':
		mgr->getNavigator()->setMotionFactor( 1000.0f );
		break;

	case 'x':  
		mgr->getNavigator()->set(Pnt3f(500,10,500),Pnt3f(510,8,500),Vec3f(0,1,0));
		break;

	case 'c': 
		mgr->getNavigator()->set( Pnt3f(0,0,2),Pnt3f(2,2,1),Vec3f(0,1,0));
		break;

	case 'd': 
		terrainCore->setShowTransitionRegions( !terrainCore->getShowTransitionRegions() );
		break;

	case 'v': 
		terrainCore->setUseVboExtension( !terrainCore->getUseVboExtension() );
		if( terrainCore->getUseVboExtension() )
		{
			std::cout << "Enabled Vbo Extension" << std::endl;
		}
		else
		{
			std::cout << "Disabled Vbo Extension" << std::endl;
		}

		break;

	case 'r':
		reloadShader();		
		break;

	case 'i':
		{
			Line l = mgr->calcViewRay( x, y );

			std::cerr << "From "  << l.getPosition () 
				<< ", dir " << l.getDirection() << std::endl;

			IntersectAction *act = IntersectAction::create();

			act->setLine(l);
			act->apply(terrainNode);

			// did we hit something?
			if( act->didHit() )
			{
				// yes!! print and highlight it
				std::cerr << " object " << act->getHitObject  () 
					<< " tri "    << act->getHitTriangle() 
					<< " at "     << act->getHitPoint   () << std::endl;

				// position the ball on the intersection point
			}
			else
			{
				// no, get rid of the triangle and highlight.
				std::cerr << " nothing hit " << std::endl;
			}
		}
		break;
	}
	glutPostRedisplay();
}

// setup the GLUT library which handles the windows for us
int setupGLUT(int *argc, char *argv[])
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	int winid = glutCreateWindow("OpenSG");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);

	return winid;
}
