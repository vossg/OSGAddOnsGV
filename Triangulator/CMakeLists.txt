
IF(NOT OSGBUILD_TRIANGULATOR)
    RETURN()
ENDIF(NOT OSGBUILD_TRIANGULATOR)

IF(OSG_VERBOSE_SCAN)
    MESSAGE( STATUS "Looking at : ${CMAKE_CURRENT_SOURCE_DIR}")
ENDIF(OSG_VERBOSE_SCAN)

########################
# OSGContribTriangulator
########################

CLEAR_PROJECT(OSGContribTriangulator)

SET(OSGContribTriangulator_OSG_Dependencies OSGBase
                                            OSGSystem)

SETUP_PROJECT(OSGContribTriangulator OSG_COMPILECONTRIBTRIANGULATORLIB)

