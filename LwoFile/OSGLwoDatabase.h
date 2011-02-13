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

#ifndef _OSGLWODATABASE_H_
#define _OSGLWODATABASE_H_

#include <istream>

#include "OSGContribLwoFileDef.h"
#include "OSGContainerForwards.h"

#include "OSGLwoChunks.h"

#include "stack"

OSG_BEGIN_NAMESPACE

class OSG_CONTRIBLWOFILE_DLLMAPPING LwoDatabase
{
  protected:

    LwoLayrChunkRCPtr              _pCurrLayer;
    LwoSubChunkContainerRCPtr      _pCurrSubChunkContainer;

    std::vector<LwoLayrChunkRCPtr> _vLayerStore;
    std::vector<LwoTagsChunkRCPtr> _vGlobalTags;
    std::vector<LwoSurfChunkRCPtr> _vSurfs;

    UInt32                         _uiFileType;

  public:

    enum FileTypes
    {
        Lwo2File = 0x0001,
        LwobFile = 0x0002
    };

    LwoDatabase(void);
    ~LwoDatabase(void);

    bool           read   (std::istream &is);
    NodeTransitPtr convert(void            );

    LwoLayrChunk *getCurrentLayer(      void                );
    LwoSurfChunk *getSurf        (      UInt32        uiIdx );

    void          addGlobalTags  (      LwoTagsChunk *pTags );
    void          addSurf        (      LwoSurfChunk *pSurf );

    Int32         getTagIndex    (const std::string  &szName);
    UInt32        getFileType    (      void                );
};

OSG_END_NAMESPACE

#endif // _OSGLWODATABASE_H
