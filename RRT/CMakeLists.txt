
IF(NOT OSGBUILD_RRT)
    RETURN()
ENDIF(NOT OSGBUILD_RRT)

IF(OSG_VERBOSE_SCAN)
    MESSAGE( STATUS "Looking at : ${CMAKE_CURRENT_SOURCE_DIR}")
ENDIF(OSG_VERBOSE_SCAN)

#################
# OSGContribRRT
#################

CLEAR_PROJECT(OSGContribRRT)

SET(OSGContribRRT_OSG_Dependencies OSGBase
                                   OSGSystem
                                   OSGGroup
                                   OSGDrawable
                                   OSGWindow
                                   OSGState)

SET(OSGContribRRT_Test_OSG_Dependencies OSGUtil OSGWindowGLUT)

SETUP_PROJECT(OSGContribRRT OSG_COMPILECONTRIBRRTLIB)

