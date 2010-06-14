//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		OSGSinusHeightDataSource.h
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------

#ifndef OSG_SINUS_HEIGHT_DATA_SOURCE_INCLUDED
#define OSG_SINUS_HEIGHT_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGConfig.h>
#include <OSGContribDef.h>

#include <OSGImage.h>
#include "OSGHeightDataSource.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------


	class OSG_CONTRIBLIB_DLLMAPPING SinusHeightDataSource : public HeightDataSource
	{
	public:		
		explicit					SinusHeightDataSource( const Pnt2i& size );
		virtual						~SinusHeightDataSource();

		void						setFrequency( float freq );
		float						getFrequency() const;

		void						setHeightScale( float scale );
		float						getHeightScale() const;

		virtual const Pnt2i&		getSampleCount() const;
		virtual void				fillHeightData( GeometryClipmapLevel& level, const Rectangle2i& targetRect ) const;

	private:
		Pnt2i						size_;
		float						heightScale_;
		float						frequency_;

		float						getHeightSample( const Pnt2i& samplePos ) const;
	};
	
	
	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef OSG_SINUS_HEIGHT_DATA_SOURCE_INCLUDED

//-------------------------------------------------------------------------------------------------

