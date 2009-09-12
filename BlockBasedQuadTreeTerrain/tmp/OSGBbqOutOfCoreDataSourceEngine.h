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

#ifndef _OSGOUTOFCOREBBQDATASOURCEENGINE_H_
#define _OSGOUTOFCOREBBQDATASOURCEENGINE_H_

//-----------------------------------------------------------------------------

#include "OSGBbqDataSourceEngine.h"
#include "OSGBbqFile.h"
#include "OSGBbqResidualCompression.h"

#include "OSGVector.h"

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------

struct BbqTerrainNode;

class BbqOutOfCoreDataSourceEngine : public BbqDataSourceEngine
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqOutOfCoreDataSourceEngine(void);
    virtual ~BbqOutOfCoreDataSourceEngine(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    bool initialize(const std::string &szFilename, 
                          Real32       fHeightScale, 
                          Real32       fHeightOffset, 
                          Real32       fSampleSpacing);
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    virtual bool onLoadNodeData(BbqTerrainNode &oNode);
    
    /*==========================  PRIVATE  ================================*/

  private:

    BbqFileReader            _oInput;
    BbqFile::BbqFileHeader   _oHeader;
    std::vector<BbqFileNode> _oStaticNodeData;
    ResidualDecompressor     _oResidualDecompressor;
    std::vector<Int16>       _vResidualBuffer;
};

OSG_END_NAMESPACE

#endif // #ifdef OUTOFCOREBBQDATASOURCEENGINE_INCLUDED

