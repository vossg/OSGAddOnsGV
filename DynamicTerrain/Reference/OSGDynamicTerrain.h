/*---------------------------------------------------------------------------*\
*                                OpenSG                                     *
*                                                                           *
*                                                                           *
*               Copyright (C) 2000-2002 by the OpenSG Forum                 *
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

//-------------------------------------------------------------------------------------------------

#ifndef _OSGTERRAIN_H_
#define _OSGTERRAIN_H_
#ifdef __sgi
#pragma once
#endif

//-------------------------------------------------------------------------------------------------

#include <OSGConfig.h>
#include <OSGContribDef.h>

#include <OSGDynamicTerrainBase.h>
#include "OSGRectangle.h"
#include "OSGGeometryClipmaps.h"
#include "OSGImageHeightDataSource.h"
#include "OSGImageTextureDataSource.h"

//-------------------------------------------------------------------------------------------------

namespace OSG
{

	//-------------------------------------------------------------------------------------------------

	class HeightDataSource;
	class TextureDataSource;

	//-------------------------------------------------------------------------------------------------

	class OSG_CONTRIBLIB_DLLMAPPING DynamicTerrain : public DynamicTerrainBase
	{
	private:

		typedef DynamicTerrainBase Inherited;

		/*==========================  PUBLIC  =================================*/
	public:

		// action handlers:
		virtual Action::ResultE		drawPrimitives( DrawActionBase * action );
		virtual void				adjustVolume( Volume & volume );
		virtual Action::ResultE		intersectEnter(Action* action );

		/*---------------------------------------------------------------------*/
		/*! \name                      Sync                                    */
		/*! \{                                                                 */

		virtual void				changed( BitVector whichField, UInt32 origin );

		/*! \}                                                                 */
		/*---------------------------------------------------------------------*/
		/*! \name                     Output                                   */
		/*! \{                                                                 */

		virtual void				dump( UInt32 uiIndent = 0, const BitVector bvFlags = 0 ) const;

		/*! \}                                                                 */
		/*=========================  PROTECTED  ===============================*/
	protected:
		// Variables should all be in TerrainBase.

		/*---------------------------------------------------------------------*/
		/*! \name                  Constructors                                */
		/*! \{                                                                 */

		DynamicTerrain();
		DynamicTerrain( const DynamicTerrain& source );

		/*! \}                                                                 */
		/*---------------------------------------------------------------------*/
		/*! \name                   Destructors                                */
		/*! \{                                                                 */
		virtual						~DynamicTerrain(); 

		/*! \}                                                                 */

		/*==========================  PRIVATE  ================================*/
	private:
		friend class FieldContainer;
		friend class DynamicTerrainBase;

		TextureDataSource*			getTextureSource();

		static void					initMethod();

		// private 'stateless' data (this data is not replicated to any other thread/node. So we need
		// to be careful what we store in here. (all data here should be determined through the 
		// values of the Fields. 

		// Image Data sources: (this fields are used if the compressed data fields contain no data:)
		ImageHeightDataSource		imageHeightSource_;
		ImageTextureDataSource		imageTextureSource_;

		TextureChunkPtr				globalTexture_;
		
		// use this source if the field CompressedHeightData contains valid data:
		//CompressedHeightDataSource	compressedHeightSource_;

		// the clipmap object: (this stores the clipmaps)
		GeometryClipmaps			geoClipmaps_;
		
		bool						needInitialize_;

		Pnt2f						getWorldOffset() const;
		Pnt2f						getWorldSize() const;
		WorldTransformation			getWorldTransform();
		
		// prohibit default functions (move to 'public' if you need one)
		void operator =(const DynamicTerrain &source);
	};


	//-------------------------------------------------------------------------------------------------

	typedef DynamicTerrain* DynamicTerrainP;

	//-------------------------------------------------------------------------------------------------

}

//-------------------------------------------------------------------------------------------------

#include <OSGDynamicTerrainBase.inl>
#include <OSGDynamicTerrain.inl>

#define OSGTERRAIN_HEADER_CVSID "@(#)$Id: FCTemplate_h.h,v 1.23 2005/03/05 11:27:26 dirk Exp $"

//-------------------------------------------------------------------------------------------------

#endif /* _OSGTERRAIN_H_ */

//-------------------------------------------------------------------------------------------------
