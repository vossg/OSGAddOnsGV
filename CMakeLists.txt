CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(OpenSGAddOns)

IF(NOT _OSGINITCACHELOADED AND EXISTS ${CMAKE_SOURCE_DIR}/CMakeCacheInitial.txt)
  INCLUDE(${CMAKE_SOURCE_DIR}/CMakeCacheInitial.txt)
  SET(_OSGINITCACHELOADED TRUE CACHE INTERNAL "")
ENDIF(NOT _OSGINITCACHELOADED AND EXISTS ${CMAKE_SOURCE_DIR}/CMakeCacheInitial.txt)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${OpenSG_DIR}/share/OpenSG/cmake")

INCLUDE(ExternalProjectHelper)

SET(OSG_COMPONENTS OSGBase OSGSystem OSGState OSGGroup OSGDrawable OSGWindow
                   OSGText OSGUtil OSGWindowGLUT OSGFileIO OSGImageFileIO
                   OSGDynamics OSGContribComputeBase)

IF(UNIX)
  IF(APPLE)
    LIST(APPEND OSG_COMPONENTS OSGWindowCarbon)
  ELSE(APPLE)
    LIST(APPEND OSG_COMPONENTS OSGWindowX)
  ENDIF(APPLE)
ENDIF(UNIX)

IF(WIN32)
  LIST(APPEND OSG_COMPONENTS OSGWindowWIN32)
ENDIF(WIN32)

OSG_INIT(${OSG_COMPONENTS})

IF(WIN32)
  OSG_CONFIGURE_GLEW()
ENDIF(WIN32)

##############################################################################
# MAIN BUILD STEPS
##############################################################################

OSG_COLLECT_LIBS()
