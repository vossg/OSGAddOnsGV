/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *             Copyright (C) 2000-2007 by the OpenSG Forum                   *
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
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
 *                                Notes                                      *
 *                                                                           *
 * Implementation based on the original thesis work by Julien Koenen         *
 *                                                                           *
 *                                                                           *
\*---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------

#ifndef _OSGBBQDATASOURCEENGINE_H_
#define _OSGBBQDATASOURCEENGINE_H_

//-----------------------------------------------------------------------------

#include "OSGBbqNode.h"
#include "OSGBbqFile.h"

#include <vector>

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE
    
//-----------------------------------------------------------------------------


struct BbqDataSourceInformation
{
    Int32                  levelCount;
    Int32                  nodeCount;
    BbqFile::HeightFormat  heightFormat;
    Int32                  heightTileSize;
    Vec2i                  heightSampleCount;
    BbqFile::TextureFormat textureFormat;
    Int32                  textureTileSize;
    Vec2i                  textureSampleCount;
    
    // terrain scaling information:
    Real32                 heightScale;
    Real32                 heightOffset;
    Real32                 sampleSpacing;
};


//-----------------------------------------------------------------------------

struct BbqTerrainNode;

class BbqDataSourceEngine
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqDataSourceEngine(void);
    virtual ~BbqDataSourceEngine(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    const BbqDataSourceInformation &getInformation(void) const;
    

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    // load height and texture data: return false, if the data could not be
    // found/is invalid 
    // the returned data is valid until the next call to loadNodeData..
    // this loads the data synchronously.. you can use this method from a
    // separate thread to prevent any frame rate stalls.. 

    bool loadNodeData(BbqTerrainNode &oNode);
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    BbqDataSourceInformation _oInformation;
    
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

            void computeBoundingBox(BbqTerrainNode &oNode, 
                                    Real32          fMinHeightSample, 
                                    Real32          fMaxHeightSample);

    virtual bool onLoadNodeData    (BbqTerrainNode &oNode           ) = 0;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:
};

//-----------------------------------------------------------------------------

OSG_END_NAMESPACE

#endif // _OSGBBQDATASOURCEENGINE_H_

