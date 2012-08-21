# - Find SOFA Toolkit
# Find the native SOFA Toolkit includes and library
# This module defines
#  SOFA_INCLUDE_DIR, where to find cg.h, etc.
#  SOFA_LIBRARIES, the libraries needed to use Cg Toolkit.
#  SOFA_FOUND, If false, do not try to use Cg.
# also defined, but not for general use are
#  SOFA_LIBRARY, where to find the Cg Toolkit library.
##
# The following variables can be used to give CMake a hint where to find Cg:
# SOFA_ROOT
##

IF(WIN32)
  SET(SOFA_VERSION "_1_0")
ELSE()
  SET(SOFA_VERSION "")
ENDIF()

SET(SOFA_LIB_LIST sofagui
                  sofatree
                  sofacore 
                  sofahelper 
                  sofadefaulttype 
                  sofasimulation
                   
                  sofa_base_visual 
                  sofa_opengl_visual 
                  sofa_base_topology 
                  sofa_component 
                  sofa_graph_component
                  )

IF(APPLE)
#    FIND_LIBRARY(CG_LIBRARY Cg
#                 PATHS ${CG_ROOT} ENV CG_ROOT)
#    MARK_AS_ADVANCED (CG_LIBRARY)
#    SET(CG_LIBRARY_RELEASE ${CG_LIBRARY}/Cg)
#    SET(CG_LIBRARY_DEBUG ${CG_LIBRARY}/Cg)
#    SET(CG_GL_LIBRARY_RELEASE ${CG_LIBRARY}/Cg)
#    SET(CG_GL_LIBRARY_DEBUG ${CG_LIBRARY}/Cg)
#    SET(CG_INCLUDE_DIR ${CG_LIBRARY}/Headers)
#    SET(CG_LIBRARIES_FOUND TRUE)
ELSE(APPLE)

#    MESSAGE("FOOOO ROOT: ${SOFA_ROOT}")

    SET(SOFA_INCLUDE_DIRS)

    FIND_PATH(SOFA_FRAMEWORK_INCLUDE_DIR sofa/core/SofaLibrary.h
        PATHS ${SOFA_ROOT} ENV SOFA_ROOT
        PATH_SUFFIXES framework)

    IF(SOFA_FRAMEWORK_INCLUDE_DIR)
      LIST(APPEND SOFA_INCLUDE_DIRS ${SOFA_FRAMEWORK_INCLUDE_DIR})
    ENDIF(SOFA_FRAMEWORK_INCLUDE_DIR)

    FIND_PATH(SOFA_MODULES_INCLUDE_DIR sofa/component/init.h
        PATHS ${SOFA_ROOT} ENV SOFA_ROOT
        PATH_SUFFIXES modules)

    IF(SOFA_MODULES_INCLUDE_DIR)
      LIST(APPEND SOFA_INCLUDE_DIRS ${SOFA_MODULES_INCLUDE_DIR})
    ENDIF(SOFA_MODULES_INCLUDE_DIR)

    FIND_PATH(SOFA_APPS_INCLUDE_DIR sofa/gui/SofaGUI.h
        PATHS ${SOFA_ROOT} ENV SOFA_ROOT
        PATH_SUFFIXES applications)

    IF(SOFA_APPS_INCLUDE_DIR)
      LIST(APPEND SOFA_INCLUDE_DIRS ${SOFA_APPS_INCLUDE_DIR})
    ENDIF(SOFA_APPS_INCLUDE_DIR)

    FIND_PATH(SOFA_TINYXML_INCLUDE_DIR tinyxml.h
        PATHS ${SOFA_ROOT} ENV SOFA_ROOT
        PATH_SUFFIXES extlibs/tinyxml)

    IF(SOFA_TINYXML_INCLUDE_DIR)
      LIST(APPEND SOFA_INCLUDE_DIRS ${SOFA_TINYXML_INCLUDE_DIR})
    ENDIF(SOFA_TINYXML_INCLUDE_DIR)

#    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
#      SET(_OSG_CG_PATH_SUFFIXES lib64 lib.x64 lib)
#    ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
#      SET(_OSG_CG_PATH_SUFFIXES lib)
#    ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

     SET(SOFA_LIBS_FOUND TRUE)

     FOREACH(SOFA_LIB ${SOFA_LIB_LIST})
       STRING(TOUPPER ${SOFA_LIB} SOFA_LIB_UC)
#       MESSAGE("LOOKING FOR : ${SOFA_LIB} # ${SOFA_LIB_UC}")

       SET(${SOFA_LIB_UC}_NAMES_RELEASE ${SOFA_LIB}${SOFA_VERSION})
       FIND_LIBRARY(${SOFA_LIB_UC}_LIBRARY_RELEASE 
                    NAMES ${${SOFA_LIB_UC}_NAMES_RELEASE}
                    PATHS ${SOFA_ROOT} ENV SOFA_ROOT
                    PATH_SUFFIXES lib)
 
       SET(${SOFA_LIB_UC}_NAMES_DEBUG ${SOFA_LIB}${SOFA_VERSION}d)
       FIND_LIBRARY(${SOFA_LIB_UC}_LIBRARY_DEBUG 
                    NAMES ${${SOFA_LIB_UC}_NAMES_DEBUG}
                    PATHS ${SOFA_ROOT} ENV SOFA_ROOT
                    PATH_SUFFIXES lib)

       IF(NOT WIN32)
         IF(${SOFA_LIB_UC}_LIBRARY_DEBUG)
           MARK_AS_ADVANCED(${SOFA_LIB_UC}_LIBRARY_DEBUG)
         ENDIF()
       ENDIF(NOT WIN32)

       IF(${SOFA_LIB_UC}_LIBRARY_RELEASE)
         IF(SOFA_LIBS_FOUND)
           SET(SOFA_LIBS_FOUND TRUE)
         ELSE()
           SET(SOFA_LIBS_FOUND FALSE)
         ENDIF()
         MARK_AS_ADVANCED(${SOFA_LIB_UC}_LIBRARY_RELEASE)
       ELSE()
         IF(${SOFA_LIB_UC}_LIBRARY_DEBUG AND SOFA_LIBS_FOUND)
           SET(SOFA_LIBS_FOUND TRUE)
         ELSE()
           SET(SOFA_LIBS_FOUND FALSE)
         ENDIF()
       ENDIF()

#       MESSAGE("  GOT Rel : ${${SOFA_LIB_UC}_LIBRARY_RELEASE}")
#       MESSAGE("  GOT Dbg : ${${SOFA_LIB_UC}_LIBRARY_DEBUG}")
#       MESSAGE("  GOT SLF : ${SOFA_LIBS_FOUND}")
     ENDFOREACH()


ENDIF (APPLE)

SET(SOFA_DEFINES SOFA_DEV;
                 SOFA_QT4;
                 SOFA_GUI_QTVIEWER;
                 SOFA_GUI_GLUT;
                 FREEGLUT_LIB_PRAGMAS=0;
                 SOFA_SUPPORT_MAPPED_MASS;
                 SOFA_DUMP_VISITOR_INFO;
                 SOFA_HAVE_ZLIB;
                 SOFA_HAVE_PNG;
                 SOFA_HAVE_GLEW;
                 SOFA_HAVE_BOOST;
                 SOFA_HAVE_EIGEN2;
                 SOFA_QT4;
                 SOFA_XML_PARSER_TINYXML;
                 SOFA_GUI_QT;
                 EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET
    CACHE STRING "" FORCE)

#SOFA_HAVE_GLEW;SOFA_HAVE_ZLIB;SOFA_HAVE_PNG;
#                 SOFA_HAVE_EIGEN2;SOFA_SUPPORT_MAPPED_MASS;
#                 SOFA_DUMP_VISITOR_INFO;SOFA_GUI_GLUT;
#                 QT_XML_LIB;SOFA_QT4;SOFA_GUI_QTVIEWER;SOFA_DEBUG;
#                 SOFA_XML;SOFA_GUI_QT;SOFA_DEV;
#                 EIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET;
#                 SOFA_XML_PARSER_TINYXML;

MARK_AS_ADVANCED(SOFA_DEFINES)

# handle the QUIETLY and REQUIRED arguments and set CG_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SOFA
                                  DEFAULT_MSG 
                                  SOFA_LIBS_FOUND
                                  SOFA_INCLUDE_DIRS)

#MESSAGE("RESULT : fw  : ${SOFA_FRAMEWORK_INCLUDE_DIR}")
#MESSAGE("RESULT : mod : ${SOFA_MODULES_INCLUDE_DIR}")
#MESSAGE("RESULT : app : ${SOFA_APPS_INCLUDE_DIR}")
#MESSAGE("RESULT : inc : ${SOFA_INCLUDE_DIRS}")
