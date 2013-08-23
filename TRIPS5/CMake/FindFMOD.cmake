# - Locate FMOD library (By Matt Raykowski, OpenNeL Project http://www.opennel.org/)
# http://www.opennel.org/fisheye/browse/~raw,r=1353/NeL/trunk/nel/CMakeModules/FindFMOD.cmake
# (with permission to relicense as LGPL-with-staticlink-exemption by Matt Raykowski)
# This module defines
# FMOD_LIBRARY, the library to link against
# FMOD_FOUND, if false, do not try to link to FMOD
# FMOD_INCLUDE_DIR, where to find headers.

IF(FMOD_LIBRARY AND FMOD_INCLUDE_DIR)
# in cache already
SET(FMOD_FIND_QUIETLY TRUE)
ENDIF(FMOD_LIBRARY AND FMOD_INCLUDE_DIR)

FIND_PATH(FMOD_INCLUDE_DIR
          fmod.hpp
          PATHS $ENV{FMOD_DIR}/include
                $ENV{FMOD_DIR}/inc
                ${FMOD_DIR}/include
                ${FMOD_DIR}/inc
                /usr/local/include
                /usr/include
                /sw/include
                /opt/local/include
                /opt/csw/include
                /opt/include
                PATH_SUFFIXES fmod fmod3)

IF(WIN32)  
  IF(OSG_PLATFORM_64)
    SET(_FMOD_LIBNAMES fmodex64_vc.lib)
  ELSE(OSG_PLATFORM_64)
    SET(_FMOD_LIBNAMES fmodex_vc.lib)
  ENDIF()
ELSEIF(LINUX)
  IF(OSG_PLATFORM_64)
    SET(_FMOD_LIBNAMES libfmodex64.so)
  ENDIF()
ELSEIF(APPLE)
ENDIF()

FIND_LIBRARY(FMOD_LIBRARY
             NAMES ${_FMOD_LIBNAMES}
             PATHS $ENV{FMOD_DIR}/lib
                   ${FMOD_DIR}/lib
                   /usr/local/lib
                   /usr/lib
                   /usr/local/X11R6/lib
                   /usr/X11R6/lib
                   /sw/lib
                   /opt/local/lib
                   /opt/csw/lib
                   /opt/lib
                   /usr/freeware/lib64)

IF(FMOD_LIBRARY AND FMOD_INCLUDE_DIR)

  SET(FMOD_FOUND "YES")
  SET( FMOD_LIBRARIES ${FMOD_LIBRARY} )

  IF(NOT FMOD_FIND_QUIETLY)
    MESSAGE(STATUS "Found FMOD: ${FMOD_LIBRARY}")
  ENDIF(NOT FMOD_FIND_QUIETLY)

ELSE(FMOD_LIBRARY AND FMOD_INCLUDE_DIR)

  IF(NOT FMOD_FIND_QUIETLY)
    MESSAGE(STATUS "Warning: Unable to find FMOD!")
    MESSAGE(STATUS "set FMOD_DIR to point to the include/lib dir location!")
    SET(FMOD_DIR "FMOD_DIR-NOTFOUND" CACHE PATH "")
  ENDIF(NOT FMOD_FIND_QUIETLY)

ENDIF(FMOD_LIBRARY AND FMOD_INCLUDE_DIR)
