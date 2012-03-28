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

#if __GNUC__ >= 4 || __GNUC_MINOR__ >=3
//#pragma GCC diagnostic warning "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

#include "OSGSofaVisualModelPolicy.h"

BEGIN_SOFA_GUI_QT_VIEWER_NAMESPACE

OSGModelPolicy::OSGModelPolicy(core::visual::VisualParams *pVParams) :
     OSGVisualModelPolicy(pVParams),

    _pClassVisualModel_1 (NULL    ),
    _pClassVisualModel_2 (NULL    ),
    _pClassVisualModel_3 (NULL    ),
    _pClassVisualModel_4 (NULL    ),
    _pClassVisualModel_5 (NULL    ),

    _pShaderClass        (NULL    ),
    _pStateClass         (NULL    ),
  
    _oDrawTool           (        ),
   
    _mShaderMap          (        ),
    _mStateMap           (        )
{
}


OSGModelPolicy::~OSGModelPolicy(void) 
{
    _mShaderMap.clear();
    _mStateMap .clear();
}

void OSGModelPolicy::createStateAliases(void)
{
    _mStateMap.insert(AliasPair("ClipPlane", "OSGClipPlane"));
}

void OSGModelPolicy::createShaderAliases(void)
{
    _mShaderMap.insert(AliasPair("OglShader", 
                                 "OSGShader"              ));
    _mShaderMap.insert(AliasPair("OglShaderDefineMacro", 
                                 "OSGShaderDefineMacro"   ));
    _mShaderMap.insert(AliasPair("OglTexture",        
                                 "OSGTexture"             ));
    _mShaderMap.insert(AliasPair("OglTexture2D",      
                                 "OSGTexture2D"           ));
    
    _mShaderMap.insert(AliasPair("OglIntVariable",    
                                 "OSGIntVariable"         ));
    _mShaderMap.insert(AliasPair("OglInt2Variable",   
                                 "OSGInt2Variable"        ));
    _mShaderMap.insert(AliasPair("OglInt3Variable",   
                                 "OSGInt3Variable"        ));
    _mShaderMap.insert(AliasPair("OglInt4Variable",   
                                 "OSGInt4Variable"        ));
    
    _mShaderMap.insert(AliasPair("OglFloatVariable",  
                                 "OSGFloatVariable"       ));
    _mShaderMap.insert(AliasPair("OglFloat2Variable", 
                                 "OSGFloat2Variable"      ));
    _mShaderMap.insert(AliasPair("OglFloat3Variable", 
                                 "OSGFloat3Variable"      ));
    _mShaderMap.insert(AliasPair("OglFloat4Variable", 
                                 "OSGFloat4Variable"      ));
    
    _mShaderMap.insert(AliasPair("OglIntVectorVariable", 
                                 "OSGIntVectorVariable"   ));
    _mShaderMap.insert(AliasPair("OglIntVector2Variable", 
                                 "OSGIntVector2Variable"  ));
    _mShaderMap.insert(AliasPair("OglIntVector3Variable", 
                                 "OSGIntVector3Variable"  ));
    _mShaderMap.insert(AliasPair("OglIntVector4Variable", 
                                 "OSGIntVector4Variable"  ));
    
    _mShaderMap.insert(AliasPair("OglFloatVectorVariable", 
                                 "OSGFloatVectorVariable" ));
    _mShaderMap.insert(AliasPair("OglFloatVector2Variable", 
                                 "OSGFloatVector2Variable"));
    _mShaderMap.insert(AliasPair("OglFloatVector3Variable", 
                                 "OSGFloatVector3Variable"));
    _mShaderMap.insert(AliasPair("OglFloatVector4Variable", 
                                 "OSGFloatVector4Variable"));
    
    _mShaderMap.insert(AliasPair("OglMatrix2Variable", 
                                 "OSGMatrix2Variable"     ));
    _mShaderMap.insert(AliasPair("OglMatrix3Variable", 
                                 "OSGMatrix3Variable"     ));
    _mShaderMap.insert(AliasPair("OglMatrix4Variable", 
                                 "OSGMatrix4Variable"     ));
    
    _mShaderMap.insert(AliasPair("OglMatrix2x3Variable", 
                                 "OSGMatrix2x3Variable"   ));
    _mShaderMap.insert(AliasPair("OglMatrix3x2Variable", 
                                 "OSGMatrix3x2Variable"   ));
    
    _mShaderMap.insert(AliasPair("OglMatrix2x4Variable", 
                                 "OSGMatrix2x4Variable"   ));
    _mShaderMap.insert(AliasPair("OglMatrix4x2Variable", 
                                 "OSGMatrix4x2Variable"   ));
    
    _mShaderMap.insert(AliasPair("OglMatrix3x4Variable", 
                                 "OSGMatrix3x4Variable"   ));
    _mShaderMap.insert(AliasPair("OglMatrix4x3Variable", 
                                 "OSGMatrix4x3Variable"   ));
}


void OSGModelPolicy::load(void)
{
    std::cerr << "OSGModelPolicy LOADING" << std::endl;
    
    // Replace all visual models with OSGSofaModel
    sofa::core::ObjectFactory::AddAlias( "VisualModel", 
                                         "OSGSofaModel", 
                                         true,
                                        &_pClassVisualModel_1   );

    sofa::core::ObjectFactory::AddAlias( "OglModel", 
                                         "OSGSofaModel", 
                                         true,
                                        &_pClassVisualModel_2   );

    sofa::core::ObjectFactory::AddAlias( "OglTetrahedralModel", 
                                         "OSGTetrahedralModel", 
                                         true,
                                        &_pClassVisualModel_3   );

    sofa::core::ObjectFactory::AddAlias( "PointSplatModel", 
                                         "OSGPointSplatModel", 
                                         true,
                                        &_pClassVisualModel_4   );

    sofa::core::ObjectFactory::AddAlias( "PointSplat", 
                                         "OSGPointSplatModel", 
                                         true,
                                        &_pClassVisualModel_5   );
    
    createStateAliases ();
    createShaderAliases();
    
    aliasMap(_mStateMap,  _pStateClass );
    aliasMap(_mShaderMap, _pShaderClass);
    
    _pVParams->drawTool() = &_oDrawTool;
}

void OSGModelPolicy::unload(void)
{
    std::cerr << "OSGModelPolicy UNLOADING" << std::endl;
    
    // Restore all visual models to original
    sofa::core::ObjectFactory::ResetAlias("VisualModel", 
                                          _pClassVisualModel_1);
    sofa::core::ObjectFactory::ResetAlias("OglModel", 
                                          _pClassVisualModel_2);
    sofa::core::ObjectFactory::ResetAlias("OglTetrahedralModel", 
                                          _pClassVisualModel_3);
    sofa::core::ObjectFactory::ResetAlias("PointSplatModel", 
                                          _pClassVisualModel_4);
    sofa::core::ObjectFactory::ResetAlias("PointSplat", 
                                          _pClassVisualModel_5);

//    sofa::core::ObjectFactory::ResetAlias("OglShadowShader", 
//                                          _pClassVisualModel);
    
    unAliasMap(_mStateMap,  _pStateClass);
    unAliasMap(_mShaderMap, _pShaderClass);
    
    _pVParams->drawTool() = NULL;
}

END_SOFA_GUI_QT_VIEWER_NAMESPACE

