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

#include "OSGRRTExternalInclude.h"

using namespace OSG;

NodeUnrecPtr pStageNode = NULL;

RenderActionRefPtr rentravact = NULL;

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




int doMain (int argc, char **argv)
{
    osgInit(argc,argv);
    
    // GLUT init

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

        file = makeSphere(4, 2.0);
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


    dlight->addChild(sceneTrN);

    // Camera
    
    cam = PerspectiveCamera::create();
    {
        cam->setBeacon( b1n );
        cam->setFov( osgDegree2Rad( 90 ) );
        cam->setNear( 0.1 );
        cam->setFar( 100000 );
    }

    RTCameraDecoratorUnrecPtr pCamDeco = RTCameraDecorator::create();

    pCamDeco->setDecoratee(cam);

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



    pStageNode = Node::create();

    pStage = RRTStage::create();

    pStage->setRenderTarget (pFBO);
//    pStage->setTextureTarget(tx1o);
  
    pStage->setSplitThreads(false );
//    pStage->setTiled       (true);
    pStage->setTiled       (false);
    pStage->setWidth       (128  );
    pStage->setHeight      (128  );


//    pStage->setRayTracingRoot(file);



    RTInfoAttachmentUnrecPtr pRTInfo = RTInfoAttachment::create();

    file->addAttachment(pRTInfo);



    VisitSubTreeUnrecPtr pVisit     = VisitSubTree::create();
    NodeUnrecPtr         pVisitNode = Node::create();

//    pVisit    ->setSubTreeRoot(dlight);
    pVisit    ->setSubTreeRoot(file);
    pVisitNode->setCore       (pVisit);


    pStageNode->setCore (pStage    );
    pStageNode->addChild(pVisitNode);


    root->addChild(pStageNode);

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


    RRTStage::ActiveRayTracerUnrecPtr pRayTracer = 
        RRTStage::ActiveRayTracer::create();


    pRayTracer->setResolution(128, 128);

    RTImageTargetUnrecPtr pImgTarget = RTImageTarget::create();

    pRayTracer->setTarget(pImgTarget);

    pRayTracer->setRayTracingRoot(pStageNode);

    pRayTracer->init(false, false);
                     /*_sfTiled.getValue(), _sfSplitThreads.getValue()*/

    pRayTracer->trace(pCamDeco, false /*_sfTiled.getValue()*/);

    pRayTracer->finalize(NULL);

    // run...
//    glutMainLoop();
    
    rentravact  = NULL;

    pStageNode  = NULL;
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
    pRayTracer  = NULL;

    return 0;
}

int main (int argc, char **argv)
{
    doMain(argc, argv);

    osgExit();

    return 0;
}
