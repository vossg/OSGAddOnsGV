//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:20:35
//
//	Description			:		OSGHeightDataUpdater.cpp
//
//-------------------------------------------------------------------------------------------------

#include "PrecompiledHeader.h"

#if 0 

#include "OSGHeightDataUpdater.h"
#include "OSGHeightDataSource.h"
#include "OSGDynamicTerrain.h"
#include "OSGTools.h"
#include "OSGDynamicTerrainLevel.h"

//todo: this belongs in the DynamicTerrain class (does it?)
//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	
	HeightDataUpdater::HeightDataUpdater()
	{
		sampleBudgetPerFrame_ = 0;
		terrain_	= NullFC;
		dataSource_	= 0;
	}


	//-------------------------------------------------------------------------------------------------


	HeightDataUpdater::~HeightDataUpdater()
	{
	}


	//-------------------------------------------------------------------------------------------------

	



}

//-------------------------------------------------------------------------------------------------

#endif // 0
