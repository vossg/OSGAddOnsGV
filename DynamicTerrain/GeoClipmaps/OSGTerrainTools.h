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

#ifndef _OSGTERRAINTOOLS_H_
#define _OSGTERRAINTOOLS_H_

#include <string>
#include "OSGVector.h"
#include "OSGMatrix.h"
#include "OSGColor.h"
#include "OSGTerrainTypes.h"
#include "OSGContribDynamicTerrainDef.h"

OSG_BEGIN_NAMESPACE


unsigned int		getNextPowerOf2( unsigned int x );
void				drawBox( const Pnt3f& min, const Pnt3f& max );
OSG_CONTRIBDYNAMICTERRAIN_DLLMAPPING
bool				loadTextFile( const char* filename, std::string& text );


template< typename T > T	clamp( T x, T min, T max )
{
    assert( min <= max );
    if( x < min )
    {
        return min;
    }
    else if( x > max )
    {
        return max;
    }
    else
		{
			return x;
		}
}

OSG_END_NAMESPACE

#endif // #ifdef _OSGTERRAINTOOLS_H_

