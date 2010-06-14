//-------------------------------------------------------------------------------------------------
//
//	Author				:		Julien Koenen
//	Creation Date		:		15.12.2004 19:21:30
//
//	Description			:		QaebRenderer.h
//
//-------------------------------------------------------------------------------------------------

#if 0

//-------------------------------------------------------------------------------------------------

#ifndef QAEB_RENDERER_INCLUDED
#define QAEB_RENDERER_INCLUDED

//-------------------------------------------------------------------------------------------------
#include <OSGConfig.h>
#include <OSGContribDef.h>

#include <OSGFrustumVolume.h>
#include <OSGTextureChunkFields.h>
#include <OSGSHLChunk.h>
#include "OSGRectangle.h"
#include "OSGTypes.h"
#include "OSGGlslShader.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	class TerrainDataSource;
	class OpenGLContext;

	//-------------------------------------------------------------------------------------------------

	typedef Point< Int32, 2 > Pnt2i;	

	//-------------------------------------------------------------------------------------------------

	class OSG_CONTRIBLIB_DLLMAPPING QaebRenderer
	{
	public:		
		QaebRenderer();
		~QaebRenderer();

		bool						initialize( Window* window, float worldSpacing, int worldSampleCount, int levelSampleCount );
		void						shutdown();

	};


	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#endif // #ifdef QAEB_RENDERER_INCLUDED

//-------------------------------------------------------------------------------------------------

#endif // 0
