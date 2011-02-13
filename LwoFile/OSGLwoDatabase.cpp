/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                Copyright (C) 2008 by the OpenSG Forum                     *
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

#include "OSGLwoDatabase.h"

#include "OSGNode.h"
#include "OSGGroup.h"

OSG_BEGIN_NAMESPACE

LwoDatabase::LwoDatabase(void) :
    _pCurrLayer            (NULL    ),
    _pCurrSubChunkContainer(NULL    ),
    _vLayerStore           (        ),
    _vGlobalTags           (        ),
    _vSurfs                (        ),
    _uiFileType            (Lwo2File)
{
}

LwoDatabase::~LwoDatabase(void)
{
    _pCurrLayer             = NULL;
    _pCurrSubChunkContainer = NULL;
    
    _vLayerStore.clear();
    _vGlobalTags.clear();
    _vSurfs     .clear();
}

bool LwoDatabase::read(std::istream &is)
{
    bool returnValue = true;

    fprintf(stderr, "Start to read lwo\n");

    LwoChunkHeader  oRHeader;

    returnValue = oRHeader.read(is);

    if(returnValue == false)
    {
        return returnValue;
    }

    LwoHeaderChunkRCPtr _pHeader = new LwoHeaderChunk(oRHeader);

    returnValue = _pHeader->read(is, *this);

    if(returnValue == false)
    {
        fprintf(stderr, "invalid header\n");

        return returnValue;
    }

    _uiFileType = _pHeader->getFileType();

    LwoChunkRCPtr pCurr = NULL;

    bool          rc    = true;

    returnValue = true;

    if(_uiFileType == LwobFile)
    {
        _pCurrLayer = 
            dynamic_pointer_cast<LwoLayrChunk>(LwoLayrChunk::create(oRHeader));

        _vLayerStore.push_back(_pCurrLayer);
    }

    while(rc == true)
    {
        rc = oRHeader.read(is);

        if(rc == true)
        {
            bool bNotHandled = true;

            if(_pCurrLayer                             != NULL && 
               _pCurrLayer->handle(is, *this, oRHeader) == true)
            {
                bNotHandled = false;
            }
            else if(_pCurrSubChunkContainer                   != NULL && 
                    _pCurrSubChunkContainer->handle( is, 
                                                    *this, 
                                                     oRHeader) == true)
            {
                bNotHandled = false;
            }
#if 0
            else if(pCurr != NULL && pCurr->handle( is, 
                                                   *this,
                                                    oRHeader) == true)
            {
                bNotHandled = false;
            }
#endif

            if(bNotHandled == true)
            {
                pCurr = LwoChunkFactory::the()->createChunk(oRHeader);

                if(pCurr != NULL)
                {
                    rc = pCurr->read(is, *this);

                    if(pCurr->isLayer() == true)
                    {
                        _pCurrLayer = dynamic_pointer_cast<LwoLayrChunk>(pCurr);

                        _vLayerStore.push_back(_pCurrLayer);
                    }
                    
                    if(pCurr->isSubChunkContainer() == true)
                    {
                        _pCurrSubChunkContainer = 
                            dynamic_pointer_cast<LwoSubChunkContainer>(pCurr);
                    }
                }
                else
                {
                    rc = false;
                }
            }
        }
    }

    pCurr = NULL;

    return returnValue;
}

NodeTransitPtr LwoDatabase::convert(void)
{
    NodeTransitPtr returnValue(NULL);

    if(_vLayerStore.size() == 0)
        return returnValue;

    returnValue = Node::create();

    returnValue->setCore(Group::create());

    std::vector<LwoLayrChunkRCPtr>::const_iterator lIt  = _vLayerStore.begin();
    std::vector<LwoLayrChunkRCPtr>::const_iterator lEnd = _vLayerStore.end  ();

    for(; lIt != lEnd; ++lIt)
    {
        NodeUnrecPtr pChild = (*lIt)->convertToNode(*this);

        if(pChild != NULL)
        {
            returnValue->addChild(pChild);
        }
    }


#if 0
    if(_pHeader == NULL)
        return returnValue;

//    _pHeader->dump(0);

    returnValue = _pHeader->convertToNode(*this);
#endif

    return returnValue;
}

LwoLayrChunk *LwoDatabase::getCurrentLayer(void)
{
    return _pCurrLayer;
}

LwoSurfChunk *LwoDatabase::getSurf(UInt32 uiIdx)
{
    if(uiIdx < _vSurfs.size())
    {
        return _vSurfs[uiIdx];
    }

    return NULL;
}

void LwoDatabase::addGlobalTags(LwoTagsChunk *pTags)
{
    if(pTags != NULL)
    {
        _vGlobalTags.push_back(pTags);
    }
}

Int32 LwoDatabase::getTagIndex(const std::string  &szName)
{
    Int32 returnValue = -1;

    for(UInt32 i = 0; i < _vGlobalTags.size(); ++i)
    {
        returnValue = _vGlobalTags[i]->getTagIndex(szName);

        if(returnValue != -1)
            break;
    }

    return returnValue;
}

void LwoDatabase::addSurf(LwoSurfChunk *pSurf)
{
    Int32 iIdx = this->getTagIndex(pSurf->getName());

#if 0
    fprintf(stderr, "Add surf %s (%d) %p\n", 
            pSurf->getName().c_str(),
            iIdx,
            this);
#endif

    if(iIdx != -1)
    {
        if(_vSurfs.size() <= UInt32(iIdx))
        {
            _vSurfs.resize(iIdx + 1, NULL);
        }

        _vSurfs[iIdx] = pSurf;
    }
}

UInt32 LwoDatabase::getFileType(void)
{
    return _uiFileType;
}

OSG_END_NAMESPACE
