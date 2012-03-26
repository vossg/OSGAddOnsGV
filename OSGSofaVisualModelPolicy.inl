/*---------------------------------------------------------------------------*\
 *                                OpenSG                                     *
 *                                                                           *
 *                                                                           *
 *                 Copyright (C) 2012 by the OpenSG Forum                    *
 *                                                                           *
 *                            www.opensg.org                                 *
 *                                                                           *
 * contact: dirk            'at' opensg.org                                  * 
 *          gerrit.voss     'at' vossg.org                                   *
 *          carsten_neumann 'at' gmx.net                                     *
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

BEGIN_SOFA_GUI_QT_VIEWER_NAMESPACE

inline
OSGVisualModelPolicy::OSGVisualModelPolicy(
    core::visual::VisualParams *pVParams) :

    _pVParams(pVParams)
{
}

inline
OSGVisualModelPolicy::~OSGVisualModelPolicy(void) 
{
}




inline
void OSGModelPolicy::aliasMap(AliasMap                              &oAMap, 
                              sofa::core::ObjectFactory::ClassEntry *pEntry)
{
    if(oAMap.size() == 0) 
        return;
    
    AliasMap::iterator itr;

    for(itr = oAMap.begin(); itr != oAMap.end(); ++itr)
    {
        sofa::core::ObjectFactory::AddAlias( itr->first .c_str(),
                                             itr->second.c_str(), 
                                             true, 
                                            &pEntry              );
        
    }
    
}

inline
void OSGModelPolicy::unAliasMap(AliasMap                              &oAMap, 
                                sofa::core::ObjectFactory::ClassEntry *pEntry)
{
    if(oAMap.size() == 0)
        return;
    
    AliasMap::iterator itr;

    for(itr = oAMap.begin(); itr != oAMap.end(); ++itr)
    {
        sofa::core::ObjectFactory::ResetAlias(itr->first.c_str(), pEntry);
    }
}
    
END_SOFA_GUI_QT_VIEWER_NAMESPACE
