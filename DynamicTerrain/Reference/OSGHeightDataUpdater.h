//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		OSGHeightDataUpdater.h
//
//-------------------------------------------------------------------------------------------------

#if 0 

// todo: rename the class to TerrainUpdater??

//-------------------------------------------------------------------------------------------------

#ifndef HEIGHT_DATA_UPDATER_INCLUDED
#define HEIGHT_DATA_UPDATER_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGConfig.h>
#include <OSGContribDef.h>

#include "OSGTypes.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	class HeightDataSource;
	class DynamicTerrain;
	class DynamicTerrainLevel;
	typedef FCPtr< MaterialDrawablePtr, DynamicTerrain >	TerrainPtr;
	typedef FCPtr< FieldContainerPtr, DynamicTerrainLevel > DynamicTerrainLevelPtr;

	//-------------------------------------------------------------------------------------------------


	class OSG_CONTRIBLIB_DLLMAPPING HeightDataUpdater
	{
	public:		
		HeightDataUpdater();
		~HeightDataUpdater();

		void				initialize( TerrainPtr terrain, HeightDataSource* dataSource, int levelSampleCount );
		void				update( const Pnt3f& viewerPosition );

	private:
		Pnt2i				worldPosToSamplePos( const Pnt2f& worldPos ) const;
		int					updateLevel( DynamicTerrainLevelPtr level, const DynamicTerrainLevelPtr parentLevel, const Pnt2i& newOrigin );
		int					updateLevelBlock( DynamicTerrainLevelPtr level, const DynamicTerrainLevelPtr parentLevel, const Rectangle2i& targetRect );

		TerrainPtr			terrain_;
		HeightDataSource*	dataSource_;
		int					sampleBudgetPerFrame_;
		int					lastUpdateSampleCount_;
	};


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef HEIGHT_DATA_UPDATER_INCLUDED

//-------------------------------------------------------------------------------------------------

#endif  // 0
