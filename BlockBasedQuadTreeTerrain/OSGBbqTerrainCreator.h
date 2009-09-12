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


#ifndef _OSGBBQTERRAINCREATOR_H_
#define _OSGBBQTERRAINCREATOR_H_


#include "OSGBbqFile.h"
#include "OSGBbqNodeIterator.h"
#include "OSGTerrainHelper.h"

#include <stack>


OSG_BEGIN_NAMESPACE

class BbqCreateEngineBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqCreateEngineBase(void);
    virtual ~BbqCreateEngineBase(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual bool start(ImageBlockAccessor *pHeightFieldImage,
                       ImageBlockAccessor *pTextureImage,
                       BbqFileWriter      *pOutputFile,
                       Int32               iTileSize, 
                       Int32               iTextureSize     );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

            bool   isFinished (void) const;
    virtual void   step       (void)       = 0;
    
            Real32 getProgress(void) const;

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    BbqNodeId getChildNodeId    (BbqNodeId iId, 
                                 Int32     iChildIndex);
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */
  
    Int32               _iTileSize;
    Int32               _iTextureSize;
    Int32               _iLevelCount;
    Int32               _iTextureLeafLevel;

    Vec2i               _vHeightSampleCount;
    Vec2i               _vTextureSampleCount;

    ImageBlockAccessor *_pHeightFieldImage;
    ImageBlockAccessor *_pTextureImage;
        
    BbqFileWriter      *_pOutputFile;
    BbqNodeIterator     _oNodeIterator;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:
};

template<class HeightType, class TextureType>
class BbqCreateEngine : public BbqCreateEngineBase
{
    /*==========================  PUBLIC  =================================*/

  public:

    typedef BbqCreateEngineBase Inherited;

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqCreateEngine(void);
    virtual ~BbqCreateEngine(void);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual bool start(ImageBlockAccessor *pHeightFieldImage,
                       ImageBlockAccessor *pTextureImage,
                       BbqFileWriter      *pOutputFile,
                       Int32               iTileSize, 
                       Int32               iTextureSize     );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    virtual void step(void);

    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    struct BbqCreationNodeData
    {
        bool                heightDataValid;
        bool                textureDataValid;
        
        std::vector<HeightType > heightData;
        std::vector<TextureType> textureData;
    };

    struct BbqCreationNode
    {
        typedef BbqFileNode<HeightType> FileNode;

        Vec2i                           sampleOrigin;
        Vec2i                           sampleTarget;
        int                             treeLevel;
        int                             sampleStepSize;
        
        bool                            hasTextureData;
        
        Vec2f                           blockOrigin;
        float                           blockScale;
        
        //< maximum error of this node (in object space)
        Int32                           maxHeightError;     

        //< for bounding box construction   (todo: do we need these at all?)
        HeightType                      minHeightSample;    

        //< for bounding box construction
        HeightType                      maxHeightSample;    
        
        // this is the data pointer for the creation process.. 
        // the data is kept in memory as long as needed, but not longer

        BbqCreationNodeData                   *data;
        typename FileNode::TerrainDataPointer  fileDataPointer;    
    };

     /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    typedef 
     FixedAllocator< BbqCreationNodeData >   BbqCreationNodeDataAllocator;

    typedef typename
        BbqCreationNodeDataAllocator::StorageIt BbqCreationNodeDataAllocatorIt;


    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    BbqCreationNodeDataAllocator  _oCreationDataAllocator;
       
    std::vector<BbqCreationNode>  _vQuadtree;
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    bool      isLeafNodeId      (BbqNodeId iId        ) const;

    void      initializeNodesRec(BbqNodeId iId, 
                                 Int32     iX0, 
                                 Int32     iY0, 
                                 Int32     iX1, 
                                 Int32     iY1, 
                                 Int32     level      );

    void      processNode       (BbqNodeId iId        );
    
    

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void getTextureData(BbqCreationNode &oNode);
    void getHeightData (BbqCreationNode &oNode);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    void combineHeightData (BbqCreationNode &oNode, 
                            BbqCreationNode &oChildNode00, 
                            BbqCreationNode &oChildNode10, 
                            BbqCreationNode &oChildNode01, 
                            BbqCreationNode &oChildNode11);
    
    void combineTextureData(BbqCreationNode &oNode, 
                            BbqCreationNode &oChildNode00, 
                            BbqCreationNode &oChildNode10, 
                            BbqCreationNode &oChildNode01, 
                            BbqCreationNode &oChildNode11);
   
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */
   
    void subSampleHeightData (      HeightType *       pTarget, 
                              const HeightType * const pSource, 
                                    Int32              iX0, 
                                    Int32              iY0, 
                                    Int32              iSize );

    void subSampleTextureData(      TextureType *        pTarget, 
                              const TextureType * const  pSource, 
                                    Int32                iX0, 
                                    Int32                iY0, 
                                    Int32                iSize );
    
    
    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:
};


class BbqTerrainCreator
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqTerrainCreator(void);
    ~BbqTerrainCreator(void);
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    // todo: add an error report system (Returncodes/exceptions)
    // todo: put the parameters into a structure.. 

    bool  start(const std::string &inputHeightfield, 
                const std::string &inputTextureFilename, 
                const std::string &outputFilename, 
                      Int32        iTileSize, 
                      Int32        iTextureSize        );

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   Destructor                                 */
    /*! \{                                                                 */

    bool   isFinished (void) const;
    void   step       (void);
    
    Real32 getProgress(void) const;
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    BbqCreateEngineBase *_pEngine;
    ImageBlockAccessor   _oHeightFieldImage;
    ImageBlockAccessor   _oTextureImage;
    BbqFileWriter        _oOutputFile;

    /*! \}                                                                 */
    /*==========================  PRIVATE  ================================*/

  private:

};

    
OSG_END_NAMESPACE

#include "OSGBbqTerrainCreator.inl"

#endif // #ifdef BBQTERRAIN_INCLUDED


