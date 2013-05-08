
MESSAGE(STATUS "processing OSGAddOns Package Prepare")
MESSAGE("")  

GET_FILENAME_COMPONENT(_CMAKE_CURRENT_LIST_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

CONFIGURE_FILE(
  ${_CMAKE_CURRENT_LIST_DIR}/OSGAddOnsConfig.h.cmake
  ${CMAKE_BINARY_DIR}/Source/Base/Base/OSGAddOnsConfig.h.tmp
  @ONLY                                                    )

EXECUTE_PROCESS(
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${CMAKE_BINARY_DIR}/Source/Base/Base/OSGAddOnsConfig.h.tmp"
            "${CMAKE_BINARY_DIR}/Source/Base/Base/OSGAddOnsConfig.h")
