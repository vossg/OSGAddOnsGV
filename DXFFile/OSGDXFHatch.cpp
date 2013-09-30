/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *               Copyright (C) 2000-2003 by the OpenSG Forum                 *
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
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
//  Includes
//---------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

#include "OSGConfig.h"

#include "OSGDXFHatch.h"
#include "OSGDXFHeader.h"
#include "OSGFieldContainer.h"
#include "OSGBaseFunctions.h"
#include "OSGGeoOptimization.h"
#include "OSGMaterialGroup.h"
#include "OSGMaterial.h"
#include "OSGSimpleMaterial.h"
OSG_USING_NAMESPACE


//THE HATCH PATTERN DATA

/***************************************************************************\
 *                            Description                                  *
\***************************************************************************/

/*! \class DXFHatch
    \ingroup GrpSystemFileIO

  Parses and evaluates the file section between the groups (0,POLYLINE) and
  (0,SEQEND). Exactly speaking any group with code 0 besides (0,VERTEX) ends
  the polyline.

*/

/***************************************************************************\
 *                               Types                                     *
\***************************************************************************/

/***************************************************************************\
 *                           Class variables                               *
\***************************************************************************/

/*! Pointer to singleton of this class. There is exactly one instance for each
 * derived class which can be instantiated.
 */
DXFHatch *DXFHatch::_the = new DXFHatch();

/* \var VARTYPE DXFHatch::_VARNAME
    variable documentation
 */

/***************************************************************************\
 *                           Class methods                                 *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/


/***************************************************************************\
 *                           Instance methods                              *
\***************************************************************************/

/*================================ PUBLIC =================================*/

/*=============================== PROTECTED ===============================*/

/*================================ PRIVATE ================================*/


/*! currently only reads the boundary data, not the pattern data yet
 */
DXFResult DXFHatch::evalRecord(void)
{
    DXFResult state = DXFStateContinue;
	int index = hatchBoundaryDataLoops.size();
	int edgeIndex=0;
	//std::cout << DXFRecord::getGroupCode() << "  " << DXFRecord::getLineNumber() << std::endl;
    switch( DXFRecord::getGroupCode() )
    {
        case 30:
            _elevation =     DXFRecord::getValueDbl();
            break;
        case 66:    // obsolete, ignore if present
            break;
		case 70:
			_solidFillFlag = DXFRecord::getValueInt();
			break;
		case 91:
			_numOfBoundaryPath = DXFRecord::getValueInt();
			std::cout << "91 here:   " << _numOfBoundaryPath<< std::endl;
			break;
		case 78:
			_numOfPatternLine = DXFRecord::getValueInt();
			std::cout << "78 here   " << DXFRecord::getLineNumber() << "  " << _numOfPatternLine<< std::endl;
			break;
	 //============= here begins the boundary path data
		case 92:
			if(hatchBoundaryDataLoops.size() < _numOfBoundaryPath)
			{
				DXFHatchBoundaryPathData tempPathData;
				hatchBoundaryDataLoops.push_back(tempPathData);
			}
			hatchBoundaryDataLoops[index]._pathTypeFlag = DXFRecord::getValueInt();
			break;
		case 93:
			hatchBoundaryDataLoops[index-1]._numOfEdge = DXFRecord::getValueInt();
			break;
		case 72:
			if(hatchBoundaryDataLoops[index-1]._pathTypeFlag ==2)
			{
				edgeIndex = hatchBoundaryDataLoops[index-1].polyLineEdges.size();
				DXFHatchBoundaryPathDataPolyLine polylineEdge;
				hatchBoundaryDataLoops[index-1].polyLineEdges.push_back(polylineEdge);
			}
			else
			{
			Int32 tempEdgeType = DXFRecord::getValueInt();
			edgeIndex = hatchBoundaryDataLoops[index-1].edges.size(); 
			switch (tempEdgeType)
				{
				case 1:
					DXFHatchBoundaryPathDataEdge lineEdge;
					lineEdge.edgeType_Or_hasBulgeFlag =tempEdgeType;
					hatchBoundaryDataLoops[index-1].edges.push_back(lineEdge);
					break;
				case 2:
					DXFHatchBoundaryPathDataEdge arcEdge;
					arcEdge.edgeType_Or_hasBulgeFlag =tempEdgeType;
					hatchBoundaryDataLoops[index-1].edges.push_back(arcEdge);
					break;
				case 3:
					break;
				case 4:
					break;
				}
			}
			break;
		case 10:
			if(hatchBoundaryDataLoops.size() >0)
			{
				if(_numSeedPoints>-1)
				{
					_seedPntX = DXFRecord::getValueDbl();
				}
				else if(hatchBoundaryDataLoops[index-1]._pathTypeFlag ==2)
				{
					edgeIndex = hatchBoundaryDataLoops[index-1].polyLineEdges.size() -1;
					hatchBoundaryDataLoops[index-1].polyLineEdges[edgeIndex].startX = DXFRecord::getValueDbl();
				}
				else
				{
					edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
					switch (hatchBoundaryDataLoops[index-1].edges[edgeIndex].edgeType_Or_hasBulgeFlag)
					{
					case 1:
						hatchBoundaryDataLoops[index-1].edges[edgeIndex].startX = DXFRecord::getValueDbl();
						break;
					case 2:
						hatchBoundaryDataLoops[index-1].edges[edgeIndex].startX = DXFRecord::getValueDbl();
						break;
					case 3:
						break;
					case 4:
						break;
					}
				}
			}
			break;
		case 20:
			if(hatchBoundaryDataLoops.size() > 0)
			{
				if(_numSeedPoints>-1)
				{
					_seedPntY = DXFRecord::getValueDbl();
				}
				else if(hatchBoundaryDataLoops[index-1]._pathTypeFlag ==2)
				{
					edgeIndex = hatchBoundaryDataLoops[index-1].polyLineEdges.size() -1;
					hatchBoundaryDataLoops[index-1].polyLineEdges[edgeIndex].startY = DXFRecord::getValueDbl();
				}
				else
				{
					edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
					switch (hatchBoundaryDataLoops[index-1].edges[edgeIndex].edgeType_Or_hasBulgeFlag)
					{
					case 1:
						hatchBoundaryDataLoops[index-1].edges[edgeIndex].startY = DXFRecord::getValueDbl();
						break;
					case 2:
						hatchBoundaryDataLoops[index-1].edges[edgeIndex].startY = DXFRecord::getValueDbl();
						break;
					case 3:
						break;
					case 4:
						break;
					}
				}
			}
			break;
		case 11:
			
			edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
			switch (hatchBoundaryDataLoops[index-1].edges[edgeIndex].edgeType_Or_hasBulgeFlag)
			{
			case 1:
				hatchBoundaryDataLoops[index-1].edges[edgeIndex].endX = DXFRecord::getValueDbl();
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			}
			break;
		case 21:
			
			edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
			switch (hatchBoundaryDataLoops[index-1].edges[edgeIndex].edgeType_Or_hasBulgeFlag)
			{
			case 1:
				edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
				hatchBoundaryDataLoops[index-1].edges[edgeIndex].endY = DXFRecord::getValueDbl();
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			}
			break;
		case 40:
			
			edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
			switch (hatchBoundaryDataLoops[index-1].edges[edgeIndex].edgeType_Or_hasBulgeFlag)
			{
			case 1:
				break;
			case 2:
				hatchBoundaryDataLoops[index-1].edges[edgeIndex].radius = DXFRecord::getValueDbl();
				break;
			case 3:
				break;
			case 4:
				break;
			}
			break;
		case 50:
			
			edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
			switch (hatchBoundaryDataLoops[index-1].edges[edgeIndex].edgeType_Or_hasBulgeFlag)
			{
			case 1:
				
				break;
			case 2:
				hatchBoundaryDataLoops[index-1].edges[edgeIndex].startAngle = DXFRecord::getValueDbl();
				break;
			case 3:
				break;
			case 4:
				break;
			}
			break;
		case 51:
			edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
			switch (hatchBoundaryDataLoops[index-1].edges[edgeIndex].edgeType_Or_hasBulgeFlag)
			{
			case 1:
				
				break;
			case 2:
				edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
				hatchBoundaryDataLoops[index-1].edges[edgeIndex].endAngle = DXFRecord::getValueDbl();
				break;
			case 3:
				break;
			case 4:
				break;
			}
			break;
		case 73:
			edgeIndex = hatchBoundaryDataLoops[index-1].edges.size() -1;
			switch (hatchBoundaryDataLoops[index-1].edges[edgeIndex].edgeType_Or_hasBulgeFlag)
			{
			case 1:
				break;
			case 2:
				hatchBoundaryDataLoops[index].edges[edgeIndex].isCounterClockWise = DXFRecord::getValueDbl();
				break;
			case 3:
				break;
			case 4:
				break;
			}
			break;
	//=================================================
		case 98:
			_numSeedPoints = DXFRecord::getValueInt();
			break;
        default:
            state = DXFStateUnknown;
    }
    if(state == DXFStateUnknown)
        state = Inherited::evalRecord();
    return state;
}

/*!  \todo
 *  check and make sure not to have bad memory leaks here!
 */
DXFResult DXFHatch::beginEntity(void)
{   
    DXFResult state;
    state = Inherited::beginEntity();
	_solidFillFlag = 1;
    beginGeometry();
    
    return state;
}

/*! Create OpenSG polyline or surface geometry from VERTEX data. 
 *
 *  If the POLYLINE entity represents a polyline (not a mesh) and there is a
 *  Geometry with the same color and line settings in the layer, the polyline
 *  will be added to it's core as one GL_LINE_STRIP primitive whereas meshes
 *  always are created as a new Geometry Node (Done in
 *  DXFEntitiesEntry::flushGeometry)
 *
 *  \todo
 *  color and line settings are not checked yet)
 */

DXFResult DXFHatch::endEntity(void)
{	
	_faceTypeP->clear();
	_faceLengthP->clear();

	OSG::Color3f color;
	StringToNodePtrMap::iterator itr = _layersMapP->find(this->_layerName);
	if(itr!= _layersMapP->end())
	{
		OSG::NodeRefPtr layerNodePtr = itr->second;
		OSG::MaterialGroupRefPtr mgrp = dynamic_cast<OSG::MaterialGroup*>(layerNodePtr->getCore());
		OSG::SimpleMaterialRefPtr mat = dynamic_cast<OSG::SimpleMaterial*>(mgrp->getMaterial());
		color =	mat->getDiffuse();
	}
	for(int i=0;i<this->hatchBoundaryDataLoops.size();i++)
	{
		DXFHatchBoundaryPathData& pathData = hatchBoundaryDataLoops.at(i);
		
		if(pathData.polyLineEdges.size()>0)
		{
			for(int j=0;j<pathData.polyLineEdges.size();j++)
			{
				Pnt3f startp(pathData.polyLineEdges.at(j).startX,
					pathData.polyLineEdges.at(j).startY,
					_elevation);
				_pointsP->push_back(startp);
			}
			_faceTypeP->push_back(GL_LINE_STRIP);
			_faceLengthP->push_back(pathData.polyLineEdges.size());
		}
		int verticesNum =0;
		if(pathData.edges.size()>0)
		{
			for(int j=0;j<pathData.edges.size();j++)
			{
				if(pathData.edges[j].edgeType_Or_hasBulgeFlag == 1)
				{
					Pnt3f endp(pathData.edges.at(j).endX,
						pathData.edges.at(j).endY,
						_elevation);
					_pointsP->push_back(endp);
					std::cout << endp << std::endl;
					verticesNum ++;
				}
				else if(pathData.edges[j].edgeType_Or_hasBulgeFlag == 2)
				{
					float angleBase = DXFHeader::getAngBase();
					int dir = DXFHeader::getAngDir();
					double x, y, z;
					Real32 tempStartAngle = pathData.edges.at(j).startAngle;
					Real32 tempEndAngle = pathData.edges.at(j).endAngle;
					Real32 tempRadius = pathData.edges.at(j).radius;
					if(dir) //clockwise 
					{
						tempStartAngle= 360 - tempStartAngle;
						tempEndAngle  =  360 - tempEndAngle;
					}
					tempStartAngle += 360;
					tempEndAngle += 360;
					while(tempStartAngle > tempEndAngle)
					{
						tempEndAngle += 360;
					}
					x= pathData.edges.at(j).startX + tempRadius * cos(osgDegree2Rad(tempStartAngle));
					y= pathData.edges.at(j).startY + tempRadius * sin(osgDegree2Rad(tempStartAngle));
					z = 0;
					//ignore the first one as it is the end point of the last edge
					//only when it it j==0, add it
					if(j==0)
					{
						_pointsP->push_back(OSG::Pnt3f(x,y,z));
						verticesNum ++;
						std::cout << x << "  " << y << "  " << z << std::endl;
					}
					int div = 10;
					int num = abs(tempEndAngle - tempStartAngle) / div;
					for(int i=1;i<num-1;i++)
					{
						x= pathData.edges.at(j).startX  + tempRadius * cos(osgDegree2Rad(tempStartAngle + div * i));
						y= pathData.edges.at(j).startY + tempRadius * sin(osgDegree2Rad(tempStartAngle + div * i));
						z = 0;
						_pointsP->push_back(OSG::Pnt3f(x,y,z));
						verticesNum ++;
						std::cout << x << " " << y << "  " << z << std::endl;
					}
					x= pathData.edges.at(j).startX  + tempRadius * cos(osgDegree2Rad(tempEndAngle));
					y= pathData.edges.at(j).startY  + tempRadius * sin(osgDegree2Rad(tempEndAngle));
					z = 0;
					_pointsP->push_back(OSG::Pnt3f(x,y,z));
					std::cout << x << "  " << y << "  " << z << std::endl;
					verticesNum ++;
				}
			}
		}
			_faceTypeP->push_back(GL_POLYGON);
			_faceLengthP->push_back(verticesNum);
	}

	flushGeometry(true);
	endGeometry();

    _elevation = 0;
	_solidFillFlag = 1;
	_numOfBoundaryPath = 0;
	hatchBoundaryDataLoops.clear();
	_numOfPatternLine =0;
	hatchPatternData.clear();
	_numSeedPoints=-1;
	_seedPntX = 0;
	_seedPntY = 0;
    return DXFStateContinue;
}

/*-------------------------------------------------------------------------*/

/*------------------------- constructors ----------------------------------*/

/*! Add ENTITY:POLYLINE entity type to DXF hierarchy as child of
 *  SECTION:ENTITIES (see DXFEntities) and BLOCKSENTRY:BLOCK (see DXFBlock).
 */
DXFHatch::DXFHatch(void) :
    Inherited(),
    _elevation(0.0),
	_solidFillFlag(1)
{
    _entityTypeName    = "HATCH";

    registerToParentEntityType("SECTION:ENTITIES");
    registerToParentEntityType("BLOCKSENTRY:BLOCK");
}

/*-------------------------- destructor -----------------------------------*/

/*! Does nothing.
 */
DXFHatch::~DXFHatch(void)
{
}

//======================================================================================
//the HATCH BOUNDARY PATH DATA

//DXFResult DXFHatchBoundaryPathData::evalRecord (void)
//{
//	DXFResult state = DXFStateContinue;
//	switch(DXFRecord::getGroupCode())
//	{
//	case 92:
//		_pathTypeFlag = DXFRecord::getValueInt();
//		break;
//	case 93:
//		_numOfEdge = DXFRecord::getValueInt();
//		break;
//	case 72:
//		if(_pathTypeFlag == 2)
//		{
//			_hasBulgeFlag = DXFRecord::getValueInt();
//			polyLineEdgePnts.push_back(tempStartPnt);
//		}
//		else
//		{
//			_edgeType = DXFRecord::getValueInt();
//			if(_edgeType == 1)
//			{
//				lineEdgePnts.push_back(tempStartPnt);
//				lineEdgePnts.push_back(tempEndPnt);
//			}
//			else if(_edgeType == 2)
//			{
//				float angleBase = DXFHeader::getAngBase();
//				int dir = DXFHeader::getAngDir();
//				double x, y, z;
//				if(dir) //clockwise
//				{
//					tempStartAngle= 360 - tempStartAngle;
//					tempEndAngle  =  360 - tempEndAngle;
//				}
//				tempStartAngle += 360;
//				tempEndAngle += 360;
//				while(tempStartAngle > tempEndAngle)
//				{
//					tempEndAngle += 360;
//				}
//				x= tempStartPnt[0] + tempRadius * cos(osgDegree2Rad(tempStartAngle));
//				y= tempStartPnt[1] + tempRadius * sin(osgDegree2Rad(tempStartAngle));
//				z = 0;
//				ArcLEdgePnts.push_back(OSG::Pnt3f(x,y,z));
//				int div = 10;
//				int num = abs(tempEndAngle - tempStartAngle) / div;
//
//				for(int i=1;i<num-1;i++)
//				{
//					x= tempStartPnt[0] + tempRadius * cos(osgDegree2Rad(tempStartAngle + div * i));
//					y= tempStartPnt[1] + tempRadius * sin(osgDegree2Rad(tempStartAngle + div * i));
//					z = 0;
//					ArcLEdgePnts.push_back(OSG::Pnt3f(x,y,z));
//				}
//				x= tempStartPnt[0] + tempRadius * cos(osgDegree2Rad(tempEndAngle));
//				y= tempStartPnt[1] + tempRadius * sin(osgDegree2Rad(tempEndAngle));
//				z = 0;
//				ArcLEdgePnts.push_back(OSG::Pnt3f(x,y,z));
//			}
//			else if(_edgeType ==3)
//			{
//			}
//			else if(_edgeType == 4)
//			{
//			}
//		}
//		
//		resetTempVariable();
//		break;
//	case 10:
//		tempStartPnt[0]=DXFRecord::getValueDbl();
//		break;
//	case 20:
//		tempStartPnt[1]=DXFRecord::getValueDbl();
//		break;
//	case 11:
//		tempEndPnt[0]=DXFRecord::getValueDbl();
//		break;
//	case 21:
//		tempEndPnt[1]=DXFRecord::getValueDbl();
//		break;
//	case 40:
//		tempRadius = DXFRecord::getValueDbl();
//		break;
//	case 50:
//		tempStartAngle = DXFRecord::getValueDbl();
//		break;
//	case 51:
//		tempEndAngle = DXFRecord::getValueDbl();
//		break;
//	case 73:
//		tempIsCounterClockWiseFlag = DXFRecord::getValueInt();
//		break;
//	default:
//		state = DXFStateUnknown;
//	}
//	if(state == DXFStateUnknown)
//	{
//		state = Inherited::evalRecord();
//	}
//	return state;
//}
DXFHatchBoundaryPathData::DXFHatchBoundaryPathData()
{
	_pathTypeFlag = 0;
	_numOfEdge = 0;
}
