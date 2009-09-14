
IF(NOT OSGBUILD_LABELLING)
    RETURN()
ENDIF(NOT OSGBUILD_LABELLING)

IF(OSG_VERBOSE_SCAN)
    MESSAGE( STATUS "Looking at : ${CMAKE_CURRENT_SOURCE_DIR}")
ENDIF(OSG_VERBOSE_SCAN)

####################
# OSGContribLabeling
####################

CLEAR_PROJECT(OSGContribLabeling)

SET(OSGContribLabeling_OSG_Dependencies OSGBase
                                        OSGSystem
                                        OSGState
                                        OSGText
                                        OSGDrawable
                                        OSGGroup
                                        OSGUtil)

SET(OSGContribLabeling_Test_OSG_Dependencies OSGWindow OSGWindowGLUT)

SETUP_PROJECT(OSGContribLabeling OSG_COMPILECONTRIBLABELINGLIB)

