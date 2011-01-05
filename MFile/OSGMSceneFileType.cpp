/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2002 by the OpenSG Forum                   *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 *   contact: dirk@opensg.org, gerrit.voss@vossg.org, jbehr@zgdv.de          *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                License                                    *
 *                                                                           *
 * This library is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU Library General Public License as published    *
 * by the Free Software Foundation, version 2.                               *
 *                                                                           *
 * This library is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Library General Public License for more details.                          *
 *                                                                           *
 * You should have received a copy of the GNU Library General Public         *
 * License along with this library; if not, write to the Free Software       *
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.                 *
 *                                                                           *
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Changes                                    *
 *                                                                           *
 * 2002/10/16: added transparency path. Thanks to Franck Sourdin             *
 *            (sourdin@ai.cluny.ensam.fr) for it!                            *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

//-------------------------------
//  Includes
//-------------------------------

#include <cstdlib>
#include <cstdio>

#include "OSGConfig.h"

#include <OSGGL.h>

#include <iostream>
#include <fstream>
#include <vector>

#include "OSGLog.h"

#include "OSGNode.h"
#include "OSGGeometry.h"
#include "OSGTypedGeoVectorProperty.h"
#include "OSGTypedGeoIntegralProperty.h"
#ifndef OSG_EMBEDDED
#include "OSGGeoFunctions.h"
#endif
#include "OSGSimpleTexturedMaterial.h"
#ifndef OSG_EMBEDDED
#include "OSGImageFileHandler.h"
#include "OSGPathHandler.h"
#endif
#include "OSGGroup.h"
#ifndef OSG_EMBEDDED
#include "OSGSceneFileHandler.h"
#endif
#include "OSGTriangleIterator.h"

#include "OSGMSceneFileType.h"

OSG_USING_NAMESPACE


/*! \class OSG::MSceneFileType
    \ingroup GrpSystemFileIO

 */

#if defined(OSG_WIN32_ICL) && !defined(OSG_CHECK_FIELDSETARG)
#pragma warning (disable : 383)
#endif

const Char8 *MSceneFileType::_suffixA[] =  { "m" };

MSceneFileType  MSceneFileType::_the(_suffixA,
                                     sizeof(_suffixA),
                                     false,
                                     10,
                                     (SceneFileType::OSG_READ_SUPPORTED |
                                      SceneFileType::OSG_WRITE_SUPPORTED));

NodeTransitPtr MSceneFileType::read(std::istream &is, const Char8 *) const
{
    NodeTransitPtr rootPtr(NULL);


    if(is)
    {
              Char8  szTmpBuf[1024];
        const Char8 *szDelim = " \t=(){}";

        Pnt3f vPos;
        Vec3f vNormal;

        GeoPnt3fPropertyUnrecPtr  coordPtr       = GeoPnt3fProperty ::create();
        GeoVec3fPropertyUnrecPtr  normalPtr      = GeoVec3fProperty ::create();

        GeoPnt3fPropertyUnrecPtr  oCoordPtr      = GeoPnt3fProperty ::create();
        GeoVec3fPropertyUnrecPtr  oNormalPtr     = GeoVec3fProperty ::create();

        GeoUInt32PropertyUnrecPtr indexPtr       = GeoUInt32Property::create();

        GeoUInt32PropertyUnrecPtr lensPtr        = GeoUInt32Property::create();
        GeoUInt8PropertyUnrecPtr  typePtr        = GeoUInt8Property ::create();

        bool   onlyTris   = true;
        UInt32 uiTriCount = 0;
        bool   haveOGeo   = false;

        while(!is.eof())
        {
            is.getline(szTmpBuf, 1024);

//            fprintf(stderr, "%s\n", szTmpBuf);

            if(szTmpBuf[0] == 'V')
            {
                // Vertex
                Char8 *cIt = strtok(szTmpBuf, szDelim);

                // Vertex Id
                cIt = strtok(NULL, szDelim);

                // pos.x
                cIt = strtok(NULL, szDelim);

                vPos[0] = TypeTraits<Pnt3f::ValueType>::getFromCString(cIt);

                // pos.y
                cIt = strtok(NULL, szDelim);

                vPos[1] = TypeTraits<Pnt3f::ValueType>::getFromCString(cIt);

                // pos.z
                cIt = strtok(NULL, szDelim);

                vPos[2] = TypeTraits<Pnt3f::ValueType>::getFromCString(cIt);

                coordPtr->addValue(vPos);

                // More ? (e.g. normal)
                cIt = strtok(NULL, szDelim);
                
                if(cIt != NULL && cIt[0] == 'n')
                {
                    // normal.x
                    cIt = strtok(NULL, szDelim);
                    
                    vNormal[0] = 
                        TypeTraits<Vec3f::ValueType>::getFromCString(cIt);
                    
                    // normal.y
                    cIt = strtok(NULL, szDelim);
                    
                    vNormal[1] = 
                        TypeTraits<Vec3f::ValueType>::getFromCString(cIt);
                    
                    // normal.z
                    cIt = strtok(NULL, szDelim);
                    
                    vNormal[2] = 
                        TypeTraits<Vec3f::ValueType>::getFromCString(cIt);

                    normalPtr->addValue(vNormal);

                    // More ? (e.g. opos)
                    cIt = strtok(NULL, szDelim);
                }

                if(cIt != NULL && cIt[0] == 'O' && cIt[1] == 'p')
                {
                    // pos.x
                    cIt = strtok(NULL, szDelim);
                    
                    vPos[0] = TypeTraits<Pnt3f::ValueType>::getFromCString(cIt);
                    
                    // pos.y
                    cIt = strtok(NULL, szDelim);
                    
                    vPos[1] = TypeTraits<Pnt3f::ValueType>::getFromCString(cIt);
                    
                    // pos.z
                    cIt = strtok(NULL, szDelim);
                    
                    vPos[2] = TypeTraits<Pnt3f::ValueType>::getFromCString(cIt);
                    
                    oCoordPtr->addValue(vPos);

                    haveOGeo = true;

                    // More ? (e.g. onorm)
                    cIt = strtok(NULL, szDelim);
                }


                if(cIt != NULL && cIt[0] == 'O' && cIt[1] == 'n')
                {
                    cIt = strtok(NULL, szDelim);
                    
                    vNormal[0] = 
                        TypeTraits<Vec3f::ValueType>::getFromCString(cIt);
                    
                    // normal.y
                    cIt = strtok(NULL, szDelim);
                    
                    vNormal[1] = 
                        TypeTraits<Vec3f::ValueType>::getFromCString(cIt);
                    
                    // normal.z
                    cIt = strtok(NULL, szDelim);
                    
                    vNormal[2] = 
                        TypeTraits<Vec3f::ValueType>::getFromCString(cIt);

                    oNormalPtr->addValue(vNormal);

                    // More ? (e.g. opos)
                    cIt = strtok(NULL, szDelim);
                }

#if 0
                fprintf(stderr, "%f %f %f | %f %f %f\n",
                        vPos[0],
                        vPos[1],
                        vPos[2],
                        vNormal[0],
                        vNormal[1],
                        vNormal[2]);
#endif
            }
            else if(szTmpBuf[0] == 'F')
            {
                UInt32 uiVCount   = 0;
                UInt32 uiIdx;

                // Vertex
                Char8 *cIt = strtok(szTmpBuf, szDelim);

                // Vertex Id
                cIt = strtok(NULL, szDelim);

                // idx 1
                cIt = strtok(NULL, szDelim);

                while(cIt != NULL)
                {
                    uiIdx = TypeTraits<UInt32>::getFromCString(cIt);

                    --uiIdx;

                    indexPtr->push_back(uiIdx);
                    
                    ++uiVCount;

                    cIt = strtok(NULL, szDelim);
                }

//                fprintf(stderr, "VC :%d\n", uiVCount);

                switch(uiVCount)
                {
                    case 1:
                        lensPtr->push_back(1        );
                        typePtr->push_back(GL_POINTS);
                        onlyTris = false;
                        break;
                    case 2:
                        lensPtr->push_back(2       );
                        typePtr->push_back(GL_LINES);
                        onlyTris = false;
                        break;
                    case 3:
                        lensPtr->push_back(3           );
                        typePtr->push_back(GL_TRIANGLES);
                        ++uiTriCount;
                        break;
                    case 4:
                        lensPtr->push_back(4       );
                        typePtr->push_back(GL_QUADS);
                        onlyTris = false;
                        break;
                    default:
                        lensPtr->push_back(uiVCount  );
                        typePtr->push_back(GL_POLYGON);
                        onlyTris = false;
                        break;
                            
                            
                };
            }
        }

        rootPtr = Node::create();
        
        GeometryUnrecPtr pGeo = Geometry::create();

        pGeo->setMaterial(getDefaultMaterial());

        rootPtr->setCore(pGeo);

        if(haveOGeo == true)
        {
            pGeo->setPositions(oCoordPtr );
            pGeo->setNormals  (oNormalPtr);

            pGeo->setTexCoords (coordPtr );
            pGeo->setTexCoords1(normalPtr);
        }
        else
        {
            pGeo->setPositions(coordPtr );
            pGeo->setNormals  (normalPtr);
        }

        pGeo->setIndex(indexPtr, Geometry::PositionsIndex);
        pGeo->setIndex(indexPtr, Geometry::NormalsIndex  );

        if(haveOGeo == true)
        {
            pGeo->setIndex(indexPtr, Geometry::TexCoordsIndex);
            pGeo->setIndex(indexPtr, Geometry::TexCoords1Index);
        }

        if(onlyTris == true)
        {
            lensPtr->clear();
            typePtr->clear();

            lensPtr->push_back(3 * uiTriCount);
            typePtr->push_back(GL_TRIANGLES  );
        }

        pGeo->setLengths(lensPtr);
        pGeo->setTypes  (typePtr);
    }


#if 0
    NodePtr rootPtr, nodePtr;
    std::string elem;
    std::map<std::string, DataElem>::const_iterator elemI;
    Vec3f vec3r;
    Pnt3f pnt3r;
    Vec2f vec2r;
    Real32 x,y,z;
    GeoPnt3rPropertyPtr coordPtr    = GeoPnt3rProperty::create();
    GeoVec2rPropertyPtr texCoordPtr = GeoVec2rProperty::create();
    GeoVec3rPropertyPtr normalPtr   = GeoVec3rProperty::create();
    GeometryPtr geoPtr;
    GeoIntegralPropertyPtr posIndexPtr, texIndexPtr, normalIndexPtr;
    GeoIntegralPropertyPtr lensPtr;
    GeoIntegralPropertyPtr typePtr;
    DataElem dataElem;
    Char8 strBuf[8192], *token, *nextToken;
    Int32 strBufSize = sizeof(strBuf)/sizeof(Char8);
    Int32 index, posIndex = 0, indexType;
    Int32 i,j,n,primCount[3];
    std::list<Mesh> meshList;
    std::map<std::string, SimpleTexturedMaterialPtr> mtlMap;
    std::map<std::string, SimpleTexturedMaterialPtr>::iterator mtlI;
    Mesh emptyMesh;
    Face emptyFace;
    TiePoint  emptyTie;
    Int32 indexMask, meshIndexMask;
    std::list<Face>::iterator faceI;
    std::list<Mesh>::iterator meshI;
    bool isSingleIndex;

    // create the first mesh entry
    meshList.push_back(emptyMesh);
    meshI = meshList.begin();

    if(is)
    {
        primCount[0] = 0;
        primCount[1] = 0;
        primCount[2] = 0;

        for(is >> elem; is.eof() == false; is >> elem)
        {
            if(elem[0] == '#' || elem[0] == '$')
            {
                is.ignore(INT_MAX, '\n');
            }
            else
            {
#ifndef OSG_EMBEDDED
				SceneFileHandler::the()->updateReadProgress();
#endif
                elemI = _dataElemMap.find(elem);
                dataElem = ((elemI == _dataElemMap.end()) ?
                        UNKNOWN_DE : elemI->second );
                switch (dataElem)
                {
                    case OBJECT_DE:
                    case GROUP_DE:
                    case SMOOTHING_GROUP_DE:
                        is.ignore(INT_MAX, '\n');
                    break;
                    case VERTEX_DE:
                        primCount[0]++;
                        is >> x >> y >> z;
                        pnt3r.setValues(x,y,z);
                        coordPtr->addValue(pnt3r);
                    break;
                    case VERTEX_TEXTURECOORD_DE:
                        primCount[1]++;
                        is >> x >> y;
                        vec2r.setValues(x,y);
                        texCoordPtr->addValue(vec2r);
                    break;
                    case VERTEX_NORMAL_DE:
                        primCount[2]++;
                        is >> x >> y >> z;
                        vec3r.setValues(x,y,z);
                        normalPtr->addValue(vec3r);
                    break;
                    case LIB_MTL_DE:
                        is >> elem;
                        readMTL ( elem.c_str(), mtlMap );
                        is.ignore(INT_MAX, '\n');
                    break;
                    case USE_MTL_DE:
                        is >> elem;
                        if (meshI->faceList.empty() == false)
                        {
                            meshList.push_front(emptyMesh);
                            meshI = meshList.begin();
                        }
                        mtlI = mtlMap.find(elem);
                        if (mtlI == mtlMap.end())
                        {
                            FFATAL (("Unkown mtl %s\n", elem.c_str()));
                        }
                        else
                        {
                            meshI->mtlPtr = mtlI->second;
                        }
                    break;
                    case FACE_DE:
                        meshI->faceList.push_front(emptyFace);
                        faceI = meshI->faceList.begin();
                        is.get(strBuf,strBufSize);
                        token = strBuf;
                        indexType = 0;
                        while(token && *token)
                        {
                            for (; *token == '/'; token++)
                                indexType++;
                            for (; isspace(*token); token++)
                                indexType = 0;
                            index = strtol(token, &nextToken, 10);
                            if (token == nextToken)
                                break;
                            if (indexType == 0)
                                faceI->tieVec.push_back(emptyTie);
                            if (index >= 0)
                                index--;
                            else
                                index =  primCount[indexType] + index;
                            faceI->tieVec.back().index[indexType] = index;
                            token = nextToken;
                        }
                    break;
                    case UNKNOWN_DE:
                    default:
                        FWARNING (( "Unkown obj data elem: %s\n", elem.c_str()));
                        is.ignore(INT_MAX, '\n');
                    break;
                }
            }
        }

#if 0
        std::cerr << "------------------------------------------------" << std::endl;
        i = 0;
        for (meshI = meshList.begin(); meshI != meshList.end(); meshI++)
        {
            std::cerr << "Mesh " << i << " faceCount :"
                      << meshI->faceList.size() << std::endl;
            j = 0 ;
            for ( faceI = meshI->faceList.begin(); faceI != meshI->faceList.end();
                  faceI++)
            std::cerr << "MESH " <<  i << "face: " << j++ << "tie num: "
                      << faceI->tieVec.size() << std::endl;
            i++;
        }
        std::cerr << "------------------------------------------------" << std::endl;
#endif

        // create Geometry objects
        for (meshI = meshList.begin(); meshI != meshList.end(); meshI++)
        {
            geoPtr   = Geometry::create();
            posIndexPtr = NULL;
            texIndexPtr = NULL;
            normalIndexPtr = NULL;
            lensPtr  = GeoUIntProperty::create();
            typePtr  = GeoUInt8Property::create();

            // create and check mesh index mask
            meshIndexMask = 0;
            isSingleIndex = true;
            if ( meshI->faceList.empty() == false)
            {
                for ( faceI = meshI->faceList.begin();
                  faceI != meshI->faceList.end(); faceI++)
                {
                    indexMask = 0;
                    n = faceI->tieVec.size();
                    for (i = 0; i < n; i++)
                    {
                        for (j = 0; j < 3; j++)
                        {
                            if ((index = (faceI->tieVec[i].index[j])) >= 0)
                            {
                                indexMask |= (1 << j);
                                if (j)
                                    isSingleIndex &= (posIndex == index);
                                else
                                    posIndex = index;
                            }
                        }
                    }
                    if (meshIndexMask == 0)
                    {
                        meshIndexMask = indexMask;
                    }
                    else if (meshIndexMask != indexMask)
                    {
                        // consider this real-world example:
                       // [...]
                       // f 1603//1747 1679//1744 1678//1743
                       // s 1
                       // f 9/1/10 5/2/9 1680/3/1748 1681/4/174
                       // [...]
                       // Some faces contain texture coords and others do not.
                       // The old version did just skip this geometry.
                       // This version should continue if there's at least
                       // the vertex index
                       // I've seen the change in the maskIndex only after a smooth group,
                       // so it's perhaps smarter to not ignore the smooth group further up in this code
                       if( !(indexMask & 1) )  
                       {
                         // if there are vertex indices there's no reason to get in here
                          FFATAL (( "IndexMask unmatch, can not create geo\n"));
                          meshIndexMask = 0;
                          break;
                       }
                       else
                       {
                         // consider the minimum similarities of mesh masks
                         meshIndexMask &= indexMask;
                       }
                    }
                }
            }
            else
            {
                FWARNING (("Mesh with empty faceList\n"));
            }

            // fill the geo properties
            if (meshIndexMask)
            {
                geoPtr->setPositions ( coordPtr );
                posIndexPtr = GeoUIntProperty::create();
                if(!isSingleIndex)
                    geoPtr->setIndex(posIndexPtr, Geometry::PositionsIndex);
                geoPtr->setLengths   ( lensPtr );
                geoPtr->setTypes     ( typePtr );

                if ( (meshIndexMask & 2) && texCoordPtr->size() > 0 )
                {
                    geoPtr->setTexCoords ( texCoordPtr );
                    texIndexPtr = GeoUIntProperty::create();
                    if(!isSingleIndex)
                        geoPtr->setIndex(texIndexPtr, Geometry::TexCoordsIndex);
                }
                else
                {
                    geoPtr->setTexCoords ( NULL );
                }

                if ( (meshIndexMask & 4) && normalPtr->size() > 0 )
                {
                    geoPtr->setNormals   ( normalPtr );
                    normalIndexPtr = GeoUIntProperty::create();
                    if(!isSingleIndex)
                        geoPtr->setIndex(normalIndexPtr, Geometry::NormalsIndex);
                }
                else
                {
                    geoPtr->setNormals   ( NULL );
                }

                if (meshI->mtlPtr == NULL)
                {
                    meshI->mtlPtr = SimpleTexturedMaterial::create();
                    meshI->mtlPtr->setDiffuse( Color3r( .8f, .8f, .8f ) );
                    meshI->mtlPtr->setSpecular( Color3r( 1.f, 1.f, 1.f ) );
                    meshI->mtlPtr->setShininess( 20.f );
                }
                geoPtr->setMaterial  ( meshI->mtlPtr );

                for ( faceI = meshI->faceList.begin();
                      faceI != meshI->faceList.end(); faceI++)
                {
                    n = faceI->tieVec.size();
    
                    // add the lens entry
                    lensPtr->push_back(n);
    
                    // add the type entry
                    typePtr->push_back(GL_POLYGON);
    
                    // create the index values
                    for (i = 0; i < n; i++)
                    {
                        if (isSingleIndex)
                        {
                            posIndexPtr->push_back(faceI->tieVec[i].index[0]);
                        }
                        else
                        {
                            posIndexPtr->push_back(faceI->tieVec[i].index[0]);
                            if(texIndexPtr != NULL)
                                texIndexPtr->push_back(faceI->tieVec[i].index[1]);
                            if(normalIndexPtr != NULL)
                                normalIndexPtr->push_back(faceI->tieVec[i].index[2]);
                        }
                    }
                }
    
                if(isSingleIndex)
				{
                    geoPtr->setIndex(posIndexPtr, Geometry::PositionsIndex);
                    geoPtr->setIndex(posIndexPtr, Geometry::NormalsIndex  );
                    geoPtr->setIndex(posIndexPtr, Geometry::TexCoordsIndex);
				}

                // need to port the geometry functions ...
#if 0
                createSharedIndex( geoPtr );
#endif
    
                // check if we have normals
                // need to port the geometry functions ...

#ifndef OSG_EMBEDDED
				if(geoPtr->getNormals() == NULL)
                    calcVertexNormals(geoPtr);
#endif

                // create and link the node
                nodePtr = Node::create();
                nodePtr->setCore( geoPtr );
    
                if (meshList.size() > 1)
                {
                    if (rootPtr == NULL)
                    {
                        rootPtr = Node::create();
                        rootPtr->setCore ( Group::create() );
                        rootPtr->addChild(nodePtr);
                    }
                    else
                    {
                        rootPtr->addChild(nodePtr);
                    }
                }
                else
                {
                    rootPtr = nodePtr;
                }
            }
        }
    }

#ifndef OSG_EMBEDDED
	SceneFileHandler::the()->updateReadProgress(100);
#endif
#endif

    return rootPtr;
}

bool MSceneFileType::write(Node         * const  node, 
                           std::ostream         &os,
                           Char8          const *fileNameOrExtension) const
{
    return true;
}

MSceneFileType::MSceneFileType(const Char8  *suffixArray[],
                                     UInt16  suffixByteCount,
                                     bool    override,
                                     UInt32  overridePriority,
                                     UInt32  flags) :
    SceneFileType(suffixArray,
                  suffixByteCount,
                  override,
                  overridePriority,
                  flags)

{
}


MSceneFileType::MSceneFileType(const MSceneFileType &obj) :
    SceneFileType(obj)
{
}


MSceneFileType &MSceneFileType::the(void)
{
    return _the;
}

MSceneFileType::~MSceneFileType(void)
{
    return;
}

const Char8 *MSceneFileType::getName(void) const
{
    return "M Geometry";
}

