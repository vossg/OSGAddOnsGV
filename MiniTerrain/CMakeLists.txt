
# CMake's ADD_SUBDIRECTORY only looks for CMakeLists.txt, so we include
# the real file here

IF(OSG_LIBMINI_FOUND)
  INCLUDE("CMakeLists.Lib.OSGContribMiniTerrain.txt")
ENDIF(OSG_LIBMINI_FOUND)
