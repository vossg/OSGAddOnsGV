
#include "OSGConfig.h"

#include <iostream>

#include "OSGGLUT.h"
#include "OSGSceneFileHandler.h"
#include "OSGWindow.h"
#include "OSGRenderAction.h"

#include "OSGDirectionalLight.h"
#include "OSGPointLight.h"
#include "OSGChunkOverrideGroup.h"
#include "OSGPolygonChunk.h"
#include "OSGSimpleGeometry.h"
#include "OSGMultiCore.h"
#include "OSGGLUTWindow.h"
#include "OSGSolidBackground.h"
#include "OSGPerspectiveCamera.h"
#include "OSGTransform.h"
#include "OSGTypedGeoIntegralProperty.h"
#include "OSGPrimeMaterial.h"
#include "OSGGeoFunctions.h"
#include "OSGPrimitiveIterator.h"
#include "OSGSimpleMaterial.h"

#include "OSGAlgorithmComputeElement.h"
#include "OSGSimpleCudaComputeAlgorithm.h"
#include "OSGComputeCallbackAlgorithm.h"

#include "OSGSimpleCompute.cuh"

#include "OSGTrackball.h"

#ifndef WIN32
#include <GL/glx.h>
#endif

#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>

#ifndef WIN32
#define PAR_DRAWER
#endif

#define USE_CALLBACK_ALGO

OSG::RenderAction *pRenAct = NULL;

OSG::Trackball     tball;

OSG::TransformRecPtr                    pCamTrans  = NULL;
OSG::WindowRecPtr                       pWin       = NULL;
OSG::PolygonChunkRecPtr                 pPoly      = NULL;
OSG::ChunkOverrideGroupRecPtr           pCOver     = NULL;
OSG::GeoPnt3fPropertyUnrecPtr           pnts       = NULL;
OSG::GeoVec3fPropertyUnrecPtr           norms      = NULL;

OSG::GeometryUnrecPtr                   pPlaneGeo  = NULL;
OSG::GeometryUnrecPtr                   pNormalGeo = NULL;
OSG::NodeUnrecPtr                       pNormalN   = NULL;

OSG::SimpleCudaComputeAlgorithmUnrecPtr pAlgo      = NULL;
OSG::ComputeCallbackAlgorithmUnrecPtr   pCBAlgo    = NULL;
OSG::AlgorithmComputeElementUnrecPtr    pAlgoElem  = NULL;

OSG::MultiCoreUnrecPtr                  pMultiCore = NULL;

OSG::Int32 mouseb      = 0;
OSG::Int32 lastx       = 0;
OSG::Int32 lasty       = 0;
bool       bPolyActive = false;

OSG::Real32 rIntensity = 0.02;
OSG::Real32 rScale     = 4.5;
OSG::Real32 rScaleStep = 0.1;
OSG::Real32 rScaleMax  = 50;

OSG::CallbackDrawTaskRefPtr pGeoUpdateCallback     = NULL;
OSG::CallbackDrawTaskRefPtr pGeoUpdateCudaCallback = NULL;

OSG::UInt32 uiUpdateMode = OSG::SimpleCudaComputeAlgorithm::UpdateOSG;

void updateGeoOSGMappedCallback(OSG::HardwareContext *pContext, 
                                OSG::DrawEnv         *pEnv)
{
//    fprintf(stderr, "updateGeoOSGMappedCallback\n");

    float *p = static_cast<float *>(pnts ->mapBuffer(GL_WRITE_ONLY_ARB, pEnv));
    float *n = static_cast<float *>(norms->mapBuffer(GL_WRITE_ONLY_ARB, pEnv));

    rScale += rScaleStep;

    if(rScale > rScaleMax)
    {
        rScale = rScaleMax;
        rScaleStep *= -1;
    }

    if(rScale < 1)
    {
        rScale = 1;
        rScaleStep *= -1;
    }

    for(OSG::UInt32 i = 0; i < pnts->size(); ++i)
    {
        OSG::Vec3f tmpVec(p[i * 3    ],
                          p[i * 3 + 1],
                          p[i * 3 + 2]);
        tmpVec[2] = 0.f;
        
        OSG::Real32 rDist = tmpVec.length();

        OSG::Real32 rBump = rIntensity * OSG::osgCos(rDist * rScale);

        p[i * 3 + 2] = rBump;

        rBump = rIntensity * (rScale/(rDist)) * -2 * -sin(rDist * rScale);

        tmpVec[0] = rBump * p[i * 3    ];
        tmpVec[1] = rBump * p[i * 3 + 1];
        tmpVec[2] = 1.f;
       
        tmpVec.normalize();

        n[i * 3    ] = tmpVec[0];
        n[i * 3 + 1] = tmpVec[1];
        n[i * 3 + 2] = tmpVec[2];
    }

    pnts ->unmapBuffer(pEnv);
    norms->unmapBuffer(pEnv);
}

void updateGeoOSGCudaCallback(OSG::HardwareContext *pContext, 
                              OSG::DrawEnv         *pEnv)
{
    rScale += rScaleStep;

    if(rScale > rScaleMax)
    {
        rScale = rScaleMax;
        rScaleStep *= -1;
    }

    if(rScale < 1)
    {
        rScale = 1;
        rScaleStep *= -1;
    }

    cudaError_t cRes;
    OSG::Window *pWin  = pEnv->getWindow();

    pWin->validateGLObject(pnts->getGLId(), pEnv);

    OSG::UInt32 uiPntsVBOId = pWin->getGLObjectId(pnts->getGLId());

    cRes = cudaGLRegisterBufferObject(uiPntsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "RegisterRes %d\n", OSG::UInt32(cRes));
    }

    void *pMappedPoints = NULL;

    cRes = cudaGLMapBufferObject(&pMappedPoints, uiPntsVBOId);	 

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "MappedPtr, Res : %d\n", 
                OSG::UInt32(cRes));

    }


    pWin->validateGLObject(norms->getGLId(), pEnv);

    OSG::UInt32 uiNormsVBOId = pWin->getGLObjectId(norms->getGLId());

    cRes = cudaGLRegisterBufferObject(uiNormsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "RegisterRes %d\n", OSG::UInt32(cRes));
    }

    void *pMappedNormals = NULL;

    cRes = cudaGLMapBufferObject(&pMappedNormals, uiNormsVBOId);	 

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "MappedPtr, Res : %d\n", 
                OSG::UInt32(cRes));

    }

    OSGCUDA::simpleComputeUpdateGeo(rScale,
                                    rIntensity,
                                    pnts->size(),
                                    pMappedPoints,
                                    pMappedNormals);

    cRes = cudaGLUnmapBufferObject(uiPntsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnmapRes %d\n", OSG::UInt32(cRes));
    }

    cRes = cudaGLUnregisterBufferObject(uiPntsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnRegisterRes %d\n", OSG::UInt32(cRes));
    }


    cRes = cudaGLUnmapBufferObject(uiNormsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnmapRes %d\n", OSG::UInt32(cRes));
    }

    cRes = cudaGLUnregisterBufferObject(uiNormsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnRegisterRes %d\n", OSG::UInt32(cRes));
    }
}

void updateGeoCallback(OSG::HardwareContext *pContext, OSG::DrawEnv *pEnv)
{
    if(uiUpdateMode == OSG::SimpleCudaComputeAlgorithm::UpdateOSGMapped)
    {
        updateGeoOSGMappedCallback(pContext, pEnv);
    }
    else if(uiUpdateMode == OSG::SimpleCudaComputeAlgorithm::UpdateOSGCuda)
    {
        updateGeoOSGCudaCallback(pContext, pEnv);
    }
}

void updateGeoOSG(void)
{
    OSG::GeoPnt3fProperty::StoredFieldType *p  = pnts ->editFieldPtr();
    OSG::GeoVec3fProperty::StoredFieldType *n  = norms->editFieldPtr();

    rScale += rScaleStep;

    if(rScale > rScaleMax)
    {
        rScale = rScaleMax;
        rScaleStep *= -1;
    }

    if(rScale < 1)
    {
        rScale = 1;
        rScaleStep *= -1;
    }

    for(OSG::UInt32 i = 0; i < p->size(); ++i)
    {
        OSG::Vec3f tmpVec = (*p)[i].subZero();

        tmpVec[2] = 0.f;
        
        OSG::Real32 rDist = tmpVec.length();

        OSG::Real32 rBump = rIntensity * OSG::osgCos(rDist * rScale);

        (*p)[i][2] = rBump;

        rBump = rIntensity * (rScale/(rDist)) * -2 * -sin(rDist * rScale);

        tmpVec[0] = rBump * (*p)[i][0];
        tmpVec[1] = rBump * (*p)[i][1];
        tmpVec[2] = 1.f;
       

        tmpVec.normalize();

        (*n)[i] = tmpVec;
    }
}

void updateGeoOSGMapped(OSG::HardwareContext *pContext, 
                        OSG::DrawEnv         *pEnv    )
{
    float *p = static_cast<float *>(pnts ->mapBuffer(GL_WRITE_ONLY_ARB, pEnv));
    float *n = static_cast<float *>(norms->mapBuffer(GL_WRITE_ONLY_ARB, pEnv));

    rScale += rScaleStep;

    if(rScale > rScaleMax)
    {
        rScale = rScaleMax;
        rScaleStep *= -1;
    }

    if(rScale < 1)
    {
        rScale = 1;
        rScaleStep *= -1;
    }

    for(OSG::UInt32 i = 0; i < pnts->size(); ++i)
    {
        OSG::Vec3f tmpVec(p[i * 3    ],
                          p[i * 3 + 1],
                          p[i * 3 + 2]);
        tmpVec[2] = 0.f;
        
        OSG::Real32 rDist = tmpVec.length();

        OSG::Real32 rBump = rIntensity * OSG::osgCos(rDist * rScale);

        p[i * 3 + 2] = rBump;

        rBump = rIntensity * (rScale/(rDist)) * -2 * -sin(rDist * rScale);

        tmpVec[0] = rBump * p[i * 3    ];
        tmpVec[1] = rBump * p[i * 3 + 1];
        tmpVec[2] = 1.f;
       
        tmpVec.normalize();

        n[i * 3    ] = tmpVec[0];
        n[i * 3 + 1] = tmpVec[1];
        n[i * 3 + 2] = tmpVec[2];
    }

    pnts ->unmapBuffer(pEnv);
    norms->unmapBuffer(pEnv);
}

void updateGeoOSGCuda(OSG::HardwareContext *pContext, 
                      OSG::DrawEnv         *pEnv)
{
    rScale += rScaleStep;

    if(rScale > rScaleMax)
    {
        rScale = rScaleMax;
        rScaleStep *= -1;
    }

    if(rScale < 1)
    {
        rScale = 1;
        rScaleStep *= -1;
    }

    cudaError_t cRes;
    OSG::Window *pWin  = pEnv->getWindow();

    pWin->validateGLObject(pnts->getGLId(), pEnv);

    OSG::UInt32 uiPntsVBOId = pWin->getGLObjectId(pnts->getGLId());

    cRes = cudaGLRegisterBufferObject(uiPntsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "RegisterRes %d\n", OSG::UInt32(cRes));
    }

    void *pMappedPoints = NULL;

    cRes = cudaGLMapBufferObject(&pMappedPoints, uiPntsVBOId);	 

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "MappedPtr, Res : %d\n", 
                OSG::UInt32(cRes));

    }


    pWin->validateGLObject(norms->getGLId(), pEnv);

    OSG::UInt32 uiNormsVBOId = pWin->getGLObjectId(norms->getGLId());

    cRes = cudaGLRegisterBufferObject(uiNormsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "RegisterRes %d\n", OSG::UInt32(cRes));
    }

    void *pMappedNormals = NULL;

    cRes = cudaGLMapBufferObject(&pMappedNormals, uiNormsVBOId);	 

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "MappedPtr, Res : %d\n", 
                OSG::UInt32(cRes));

    }

    OSGCUDA::simpleComputeUpdateGeo(rScale,
                                    rIntensity,
                                    pnts->size(),
                                    pMappedPoints,
                                    pMappedNormals);

    cRes = cudaGLUnmapBufferObject(uiPntsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnmapRes %d\n", OSG::UInt32(cRes));
    }

    cRes = cudaGLUnregisterBufferObject(uiPntsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnRegisterRes %d\n", OSG::UInt32(cRes));
    }


    cRes = cudaGLUnmapBufferObject(uiNormsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnmapRes %d\n", OSG::UInt32(cRes));
    }

    cRes = cudaGLUnregisterBufferObject(uiNormsVBOId);

    if(cRes != cudaSuccess)
    {
        fprintf(stderr, "UnRegisterRes %d\n", OSG::UInt32(cRes));
    }
}

void display(void)
{
    OSG::Matrix m1;
   
    m1 = tball.getFullTrackballMatrix();

    pCamTrans->editSFMatrix()->setValue(m1);

    if(uiUpdateMode == OSG::SimpleCudaComputeAlgorithm::UpdateOSG)
    {
        updateGeoOSG();
        updateVertexNormalsGeo(pNormalGeo,
                               0.01,
                               pPlaneGeo);
    }
    else if(uiUpdateMode == OSG::SimpleCudaComputeAlgorithm::UpdateOSGMappedExt)
    {
#ifdef PAR_DRAWER
# ifndef WIN32
		glXMakeCurrent(static_cast<OSG::XWindow *>(pWin.get())->getDisplay(),
                       None, 
                       NULL);
# else
		wglMakeCurrent(NULL, NULL);
# endif
#endif
        if(pGeoUpdateCallback == NULL)
        {
            pGeoUpdateCallback = 
                new OSG::CallbackDrawTask(updateGeoOSGMapped,
                                          OSG::CallbackDrawTask::Callback);
        }
        
        pWin->queueTask(pGeoUpdateCallback);
    }
    else if(uiUpdateMode == OSG::SimpleCudaComputeAlgorithm::UpdateOSGCudaExt)
    {
#ifdef PAR_DRAWER
# ifndef WIN32
		glXMakeCurrent(static_cast<OSG::XWindow *>(pWin.get())->getDisplay(),
                       None, 
                       NULL);
# else
		wglMakeCurrent(NULL, NULL);
# endif
#endif
        if(pGeoUpdateCudaCallback == NULL)
        {
            pGeoUpdateCudaCallback = 
                new OSG::CallbackDrawTask(updateGeoOSGCuda,
                                          OSG::CallbackDrawTask::Callback);
        }
        
        pWin->queueTask(pGeoUpdateCudaCallback);
    }

    OSG::commitChanges();

    pWin->render(pRenAct);
}

void reshape(int w, int h)
{
    std::cerr << "Reshape: " << w << "," << h << std::endl;

    pWin->resize( w, h );
}


void animate(void)
{
    glutPostRedisplay();
}

// tballall stuff


void motion(int x, int y)
{   
    OSG::Real32 w = pWin->getWidth(), h = pWin->getHeight();
    

    OSG::Real32  a = -2. * ( lastx / w - .5 ),
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
    if(state == 0)
    {
        switch(button)
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
    else if(state == 1)
    {
        switch(button)
        {
            case GLUT_LEFT_BUTTON:  
                break;

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
    switch(key)
    {
        case 27:    

            pWin                   = NULL;
            pCamTrans              = NULL;
            pPoly                  = NULL;
            pCOver                 = NULL;

            pnts                   = NULL;
            norms                  = NULL;

            pPlaneGeo              = NULL;
            pNormalGeo             = NULL;
            pNormalN               = NULL;

            pGeoUpdateCallback     = NULL;
            pGeoUpdateCudaCallback = NULL;

            pAlgo                  = NULL;
            pCBAlgo                = NULL;
            pAlgoElem              = NULL;

            pMultiCore             = NULL;
            
            delete pRenAct;

            OSG::osgExit(); 

            exit(0);
            break;

        case 'v':
            pRenAct->setVolumeDrawing(!pRenAct->getVolumeDrawing());
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

        case 's':
            rScale -= 0.1f;

            pAlgo->decScale(0.1f);

            fprintf(stderr, "s: %f | i: %f\n",
                    rScale,
                    rIntensity);
            break;
        case 'S':
            rScale += 0.1f;

            pAlgo->incScale(0.1f);

            fprintf(stderr, "s: %f | i: %f\n",
                    rScale,
                    rIntensity);
            break;

        case 'i':
            rIntensity -= 0.01;

            pAlgo->setIntensity(rIntensity);

            fprintf(stderr, "s: %f | i: %f\n",
                    rScale,
                    rIntensity);
            break;
        case 'I':
            rIntensity += 0.01;

            pAlgo->setIntensity(rIntensity);

            fprintf(stderr, "s: %f | i: %f\n",
                    rScale,
                    rIntensity);
            break;
        case 'n':
            pNormalN->setTravMask(~pNormalN->getTravMask());
            break;

        case 'm':
            ++uiUpdateMode;

            if(uiUpdateMode == OSG::SimpleCudaComputeAlgorithm::MaxUpdateMode)
            {
                uiUpdateMode = 0;
            }

            pAlgo->setMode(uiUpdateMode);

            switch(uiUpdateMode)
            {
                case 0:
                    pAlgoElem->setAlgorithm(pCBAlgo);
                    fprintf(stderr, "Updates disables\n");
                    break;
                case 1:
                    fprintf(stderr, "UpdateOSG\n");
                    break;
                case 2:
                    fprintf(stderr, "UpdateOSGMapped\n");
                    break;
                case 3:
                    fprintf(stderr, "UpdateOSGMappedExt\n");
                    break;
                case 4:
                    fprintf(stderr, "UpdateOSGCuda\n");
                    break;
                case 5:
                    fprintf(stderr, "UpdateOSGCudaExt\n");
                    break;

                case 6:
                    pAlgoElem->setAlgorithm(pAlgo);
                    fprintf(stderr, "UpdateOSGAlgo\n");
                    break;
                case 7:
                    fprintf(stderr, "UpdateOSGMappedAlgo\n");
                    break;
                case 8:
                    fprintf(stderr, "UpdateOSGCudaAlgo\n");
                    break;
            }
            break;
    }
}

OSG::NodeTransitPtr initPlane(void)
{
    OSG::NodeTransitPtr returnValue(NULL);


    OSG::Real32 xsize = 1.f;
    OSG::Real32 ysize = 1.f;
    
    OSG::UInt16 hor   = 100;
    OSG::UInt16 vert  = 100;

                                   pnts  = OSG::GeoPnt3fProperty ::create();
                                   norms = OSG::GeoVec3fProperty ::create();
    OSG::GeoVec2fPropertyUnrecPtr  tex   = OSG::GeoVec2fProperty ::create();
    OSG::GeoUInt32PropertyUnrecPtr index = OSG::GeoUInt32Property::create();
    OSG::GeoUInt32PropertyUnrecPtr lens  = OSG::GeoUInt32Property::create();
    OSG::GeoUInt8PropertyUnrecPtr  types = OSG::GeoUInt8Property ::create();    

    pnts ->setVboUsage(GL_DYNAMIC_DRAW_ARB);
    norms->setVboUsage(GL_DYNAMIC_DRAW_ARB);

    OSG::UInt16 x,y;
    OSG::Real32 xstep,ystep;

    xstep=xsize / hor;
    ystep=ysize / vert;

    // calc the vertices

    OSG::GeoPnt3fProperty::StoredFieldType *p  = pnts ->editFieldPtr();
    OSG::GeoVec3fProperty::StoredFieldType *n  = norms->editFieldPtr();
    OSG::GeoVec2fProperty::StoredFieldType *tx = tex ->editFieldPtr();

    for(y = 0; y <= vert; y++)
    {
        for(x = 0; x <= hor; x++)
        {
            p->push_back(OSG::Pnt3f(x * xstep - xsize / 2, 
                                    y * ystep - ysize / 2, 
                                    0));

            n->push_back(OSG::Vec3f(0, 0, 1));

            tx->push_back(OSG::Vec2f(x / OSG::Real32(hor ), 
                                     y / OSG::Real32(vert)));
        }
    }

    // create the faces

    OSG::GeoUInt32Property::StoredFieldType *i = index->editFieldPtr();
    OSG::GeoUInt32Property::StoredFieldType *l = lens ->editFieldPtr();
    OSG::GeoUInt8Property ::StoredFieldType  *t = types->editFieldPtr();

    for(y = 0; y < vert; y++)
    {
        t->push_back(GL_TRIANGLE_STRIP);
        l->push_back(2 * (hor + 1));

        for(x = 0; x <= hor; x++)
        {
            i->push_back((y + 1) * (hor + 1) + x);
            i->push_back( y      * (hor + 1) + x);
        }
    }

    // create the geometry

    OSG::SimpleMaterialUnrecPtr pMat = OSG::SimpleMaterial::create();

    pMat->setDiffuse(OSG::Color3f(0.8, 0.8, 0.8));
    pMat->setAmbient(OSG::Color3f(0.1, 0.1, 0.1));

    OSG::GeometryUnrecPtr pGeo = OSG::Geometry::create();

    pGeo->setMaterial (pMat );
    pGeo->setPositions(pnts );
    pGeo->setNormals  (norms);
    pGeo->setTexCoords(tex  );
    pGeo->setIndices  (index);
    pGeo->setTypes    (types);
    pGeo->setLengths  (lens );

    pGeo->setDlistCache(false);
    
    pPlaneGeo = pGeo;


    pMultiCore = OSG::MultiCore              ::create();
    pAlgoElem  = OSG::AlgorithmComputeElement::create();

    pAlgo   = OSG::SimpleCudaComputeAlgorithm::create();
    pCBAlgo = OSG::ComputeCallbackAlgorithm  ::create();

    pCBAlgo->setCallback(&updateGeoCallback, "");

    OSG::GroupUnrecPtr pTmpGroup = OSG::Group::create();

    pAlgoElem->setAlgorithm(pCBAlgo);

    pMultiCore->addCore(pAlgoElem);
    pMultiCore->addCore(pGeo     );

    pAlgo->setTarget(pGeo);

    returnValue = OSG::Node::create();

    returnValue->setCore(pMultiCore);

    return returnValue;
}

int init(int argc, char **argv)
{
    OSG::osgInit(argc,argv);
       
    OSG::setVBOUsageOnPropertyProtos(true);


    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    int winid = glutCreateWindow("OpenSG");

    glutKeyboardFunc  (key    );
    glutVisibilityFunc(vis    );
    glutReshapeFunc   (reshape);
    glutDisplayFunc   (display);       
    glutMouseFunc     (mouse  );   
    glutMotionFunc    (motion ); 
    glutIdleFunc      (display);  

//    glEnable(GL_CULL_FACE);

    // OSG

    OSG::SceneFileHandler::the()->print();

    // create the graph

    // beacon for camera and light  
    OSG::NodeUnrecPtr  b1n = OSG::Node ::create();
    OSG::GroupUnrecPtr b1  = OSG::Group::create();

    b1n->setCore(b1);

    // transformation
    OSG::NodeUnrecPtr      t1n = OSG::Node     ::create();
    OSG::TransformUnrecPtr t1  = OSG::Transform::create();

    t1n->setCore (t1 );

    t1n->addChild(b1n);

    pCamTrans = t1;

    // light
    OSG::NodeUnrecPtr             pLightN = OSG::Node            ::create();
#ifdef DLIGHT
    OSG::DirectionalLightUnrecPtr pLight  = OSG::DirectionalLight::create();

    pLightN->setCore(pLight);
    
    pLight->setAmbient  (0.0,  0.0,  0.0,  1  );
    pLight->setDiffuse  (0.8f, 0.8f, 0.8f, 1.f);
    pLight->setDirection(0,    0,    1        );
    pLight->setBeacon   (b1n                  );
#else
    OSG::PointLightUnrecPtr pLight = OSG::PointLight::create();

    pLightN->setCore(pLight);
    
    pLight->setAmbient (0.0,  0.0,  0.0,  1  );
    pLight->setDiffuse (0.8f, 0.8f, 0.8f, 1.f);
    pLight->setPosition(0,    0,    0        );
    pLight->setBeacon  (b1n                  );
#endif

    // root
    OSG::NodeUnrecPtr  root = OSG::Node::create();

    OSG::GroupUnrecPtr gr1  = OSG::Group::create();

    root->setCore(gr1);

    root->addChild(t1n    );
    root->addChild(pLightN);

    // Load the file

    OSG::NodeUnrecPtr file = NULL;
    
    if(argc > 1)
    {
        file = OSG::SceneFileHandler::the()->read(argv[1], NULL);
    }

    if(file == NULL)
    {
        std::cerr << "Couldn't load file, ignoring" << std::endl;

        file = initPlane();
    }

   
    OSG::Thread::getCurrentChangeList()->commitChanges();

    file->updateVolume();


    OSG::Vec3f min,max;

    file->getVolume().getBounds(min, max);
    

    std::cout << "Volume: from " << min << " to " << max << std::endl;


    OSG::MultiCoreUnrecPtr pMCore      = OSG::MultiCore::create();

    pCOver                             = OSG::ChunkOverrideGroup::create();

    OSG::TransformRecPtr   scene_trans = OSG::Transform         ::create();

    pMCore->addCore(scene_trans);
    pMCore->addCore(pCOver     );

    OSG::NodeUnrecPtr sceneTrN = OSG::Node::create();

    sceneTrN->setCore(pMCore);
    sceneTrN->addChild(file);

    pNormalN = calcVertexNormalsGeo(pPlaneGeo, 
                                    0.01     );

    pNormalGeo = dynamic_cast<OSG::Geometry *>(pNormalN->getCore());

    pNormalN->setTravMask(~pNormalN->getTravMask());

    sceneTrN->addChild(pNormalN);

    pLightN->addChild(sceneTrN);


    // Camera
    OSG::PerspectiveCameraUnrecPtr cam = OSG::PerspectiveCamera::create();

    cam->setBeacon(b1n                   );
    cam->setFov   (OSG::osgDegree2Rad(90));
    cam->setNear  (0.1f                  );
    cam->setFar   (10000                 );

    // Background
    OSG::SolidBackgroundUnrecPtr bkgnd = OSG::SolidBackground::create();

    bkgnd->setColor(OSG::Color3f(1,0,0));
    
    // Viewport
    OSG::ViewportUnrecPtr vp = OSG::Viewport::create();

    vp->setCamera    (cam       );
    vp->setBackground(bkgnd     );
    vp->setRoot      (root      );
    vp->setSize      (0, 0, 1, 1);

    // Window
    std::cout << "GLUT winid: " << winid << std::endl;

    OSG::GLUTWindowUnrecPtr gwin = OSG::GLUTWindow::create();

    gwin->setGlutId(winid);

    pWin = gwin;

    pWin->addPort(vp);

#ifdef PAR_DRAWER
    pWin->setPartitionDrawMode(OSG::Window::ParallelPartitionDraw);
    pWin->setKeepContextActive(false                             );
#endif

    pWin->init();

    // Action
    
    pRenAct = OSG::RenderAction::create();

    pRenAct->setCorrectTwoSidedLighting(true);

    // tball

    OSG::Vec3f pos;

    pos.setValues(min[0] + ((max[0] - min[0]) * 0.5), 
                  min[1] + ((max[1] - min[1]) * 0.5), 
                  max[2] + ( max[2] - min[2] ) * 1.5 );
    
    if(pos[2] < OSG::Eps)
        pos[2] += 3;

    float scale = (max[2] - min[2] + max[1] - min[1] + max[0] - min[0]) / 6;

    OSG::Pnt3f tCenter(min[0] + (max[0] - min[0]) / 2,
                       min[1] + (max[1] - min[1]) / 2,
                       min[2] + (max[2] - min[2]) / 2);


    fprintf(stderr, "Startpos : %f %f %f\n", pos[0], pos[1], pos[2]);

    tball.setMode            (OSG::Trackball::OSGObject);
    tball.setStartPosition   (pos, true                );
    tball.setSum             (true                     );
    tball.setTranslationMode (OSG::Trackball::OSGFree  );
    tball.setTranslationScale(scale                    );
    tball.setRotationCenter  (tCenter                  );

    // run...
    
    pPoly = OSG::PolygonChunk::create();

    return 0;
}

int main (int argc, char **argv)
{
    init(argc, argv);

    glutMainLoop();
    
    return 0;
}

