# CMake's ADD_SUBDIRECTORY only looks for CMakeLists.txt, so we include
# the real file here

#INCLUDE("CMakeLists.Lib.OSGContribSOFA.txt")

######################################################################
## Project specific stuff - defines name, headers, sources and dependencies
######################################################################
## Add a sub-project
## project_add( <projectName> [LIBS | APPS | PLUGINS] [VERBOSE] )
project_add(OSGSOFA LIBS)


list(APPEND ${PROJECT_NAME}_HEADERS
#Your .h headers list here
    OSGContribSOFADef.h
    OSGSofaClipPlane.h
    OSGSofaClipPlane.inl
    OSGSofaDrawTool.h
    OSGSofaDrawTool.inl
    OSGSofaLightManager.h
    OSGSofaModel.h
    OSGSofaPointSplatModel.h
    OSGSofaShadowGraph.h
    OSGSofaTetrahedralModel.h
    OSGSofaTetrahedralModel.inl
    OSGSofaTexture.h
    OSGSofaVariable.h
    OSGSofaVariable.inl
    OSGSofaVisualModelPolicy.h
    OSGSofaVisualModelPolicy.inl
    OSGSofaVisualVisitor.h
    OSGSofaVisualVisitor.inl
)

list(APPEND ${PROJECT_NAME}_SOURCES
#Your .cpp sources list here
    OSGSofaClipPlane.cpp
    OSGSofaDrawTool.cpp
    OSGSofaLightManager.cpp
    OSGSofaModel.cpp
    OSGSofaPointSplatModel.cpp
    OSGSofaShader.cpp
    OSGSofaShaderMacro.cpp
    OSGSofaShadowGraph.cpp
    OSGSofaTetrahedralModel.cpp
    OSGSofaTexture.cpp
    OSGSofaVariable.cpp
    OSGSofaVisualModelPolicy.cpp
    OSGSofaVisualVisitor.cpp
)

add_library(${PROJECT_NAME}
		${${PROJECT_NAME}_HEADERS}
        ${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME} 
#Your depends libs list here	
    ${OpenSG_LIBRARIES}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${SOFA_LIBRARIES}
)

## Deal with output install path and version for this target
## PROJECT(<projectName> <target>)
project_install(${PROJECT_NAME} ${PROJECT_NAME})

