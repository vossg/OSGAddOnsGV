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

#ifndef _OSGBBQRENDERCACHE_H_
#define _OSGBBQRENDERCACHE_H_

//-----------------------------------------------------------------------------

#include "OSGBbqTerrainNode.h"

#include <vector>

//-----------------------------------------------------------------------------

OSG_BEGIN_NAMESPACE

//-----------------------------------------------------------------------------

template<typename T, BbqRenderCacheType CacheType> 
class BbqRenderCache
{
    /*==========================  PUBLIC  =================================*/

  public:

    /*---------------------------------------------------------------------*/
    /*! \name                    Constructor                               */
    /*! \{                                                                 */

    BbqRenderCache(void):
        _vEntries   (    ),
        _pFirstEntry(NULL),
        _pLastEntry (NULL)
    {
    }
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void setSize(Int32 size)
    {
        _vEntries.resize(size);
        
        _pLastEntry = NULL;
        
        for(Int32 i = 0; i < size; ++i)
        {
            Entry &entry = _vEntries[i];
            
            // put it into the lru list:
            entry.pPrev = _pLastEntry;
            
            if(_pLastEntry)
            {
                _pLastEntry->pNext = &entry;
            }
            else
            {
                _pFirstEntry = &entry;
            }
            
            _pLastEntry = &entry;
        }
    }

    void clear(void)
    {
        _vEntries.clear();

        _pLastEntry  = NULL;
        _pFirstEntry = NULL;
    }
    
    Int32 getSize() const
    {
        return Int32(_vEntries.size());
    }

    T &getEntry(Int32 i)
    {
        return _vEntries[i].oData;
    }
    
    T *getEntry(const BbqTerrainNodeBase *pNode)
    {
        assert(pNode);

        Entry *entry = static_cast<Entry *>(pNode->renderCache[CacheType]);
        
        if(entry && entry->pNode == pNode)
        {
            useEntry(entry);
            
            //verifyCache();
            
            return &(entry->oData);
        }
        else
        {
            return NULL;
        }
    }
    
    T *getNewEntry(const BbqTerrainNodeBase *pNode)
    {
        assert(pNode);
        
        //verifyCache();
        
        Entry *entry = _pFirstEntry;
        
        assert(entry);
        
        // this node is now cached:
        pNode->renderCache[CacheType] = entry;

        entry->pNode = pNode;
        
        useEntry(entry);
        
        assert(_pLastEntry == entry);
        
        //verifyCache();
        
        return &(entry->oData);
    }
    
    void verifyCache(void)
    {
#ifdef _DEBUG
        // count the entries:

        Entry *entry      = _pFirstEntry;
        Int32  entryCount = 0;
        
        while(entry)
        {
            entryCount++;
            
            Entry *nextEntry = entry->pNext;
            
            // verify the back link:
            if(nextEntry)
            {
                if(nextEntry->pPrev != entry)
                {
                    FLOG(("invalid render cache back link!\n"));
                }
            }
            else
            {
                if(_pLastEntry != entry)
                {
                    FLOG(("invalid render cache end!\n"));
                }
            }
            
            if(entry->pNode)
            {
                //if( entry->node->renderCache[ CacheType ] != entry )
                //{
                //  //doLogMessage( "invalid render cache node link!\n" );
                //}
            }
            
            entry = nextEntry;
        }
        
        if(entryCount != Int32(_vEntries.size()))
        {
            FLOG(("invalid render cache entry count = %i instead of %i!\n", 
                  entryCount, 
                  _vEntries.size()));
        }
#endif
    }
    
    /*! \}                                                                 */
    /*==========================  PROTECTRED  =============================*/

  protected:

    /*==========================  PRIVATE  ================================*/

  private:

    struct Entry
    {
        Entry(void):
            oData(    ),
            pNext(NULL),
            pPrev(NULL),
            pNode(NULL)
        {
        }

        Entry(const Entry &other):
            oData(other.oData),
            pNext(other.pNext),
            pPrev(other.pPrev),
            pNode(other.pNode) {}

              T                   oData;
              Entry              *pNext;
              Entry              *pPrev;
        const BbqTerrainNodeBase *pNode;

#ifdef WIN32
        const Entry &operator =(const Entry &other)
        {
            oData = other.oData;
            pNext = other.pNext;
            pPrev = other.pPrev;
            pNode = other.pNode;

            return *this;
        }
#endif

      private:

#ifndef WIN32
        void operator =(const Entry &other);
#endif
    };

    std::vector<Entry>  _vEntries;
    Entry              *_pFirstEntry;
    Entry              *_pLastEntry;
    
    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
    /*! \name                   formated output                            */
    /*! \{                                                                 */

    void useEntry(Entry *entry)
    {
        // use means put this item to the back of the list: (lru schema)
        if(entry != _pLastEntry)
        {
            // disconnect the entry from the list:
            if(entry == _pFirstEntry)
            {
                _pFirstEntry = entry->pNext;

                assert(_pFirstEntry);
            }
            
            if(entry->pNext)
            {
                entry->pNext->pPrev = entry->pPrev;
            }
            if(entry->pPrev)
            {
                entry->pPrev->pNext = entry->pNext;
            }
            
            // and put it in the end of the list:
            assert(_pLastEntry);
            
            entry->pNext = NULL;
            entry->pPrev = _pLastEntry;

            _pLastEntry->pNext = entry;
            _pLastEntry        = entry;         
        }
    }

    BbqRenderCache(const BbqRenderCache &other);
    void operator =(const BbqRenderCache &rhs);

    /*! \}                                                                 */
    /*---------------------------------------------------------------------*/
};


OSG_END_NAMESPACE

#endif // _OSGBBQRENDERCACHE_H_
