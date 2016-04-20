cmake_minimum_required(VERSION 2.8.12)

project(libwebcam)

#include(../CMakeInclude.txt)

# Set the compiled file to the TARGET variable.
set(TARGET "libwebcam")
set(LIB_SOURCE_DIR "libwebcam")
set(LIB_NAME "webcam")
set(LIB_MODE "STATIC")# "SHARED", "STATIC"

# Collect sources into the variable
file (GLOB_RECURSE DEVICE_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/device/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/device/*.cpp")
file (GLOB_RECURSE ENUMERATOR_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/enumerator/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/enumerator/*.cpp")
file (GLOB_RECURSE ERROR_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/error/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/error/*.cpp")
file (GLOB_RECURSE IMAGE_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/image/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/image/*.cpp")
file (GLOB_RECURSE INFO_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/info/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/info/*.cpp")	  
file (GLOB_RECURSE UTILS_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/utils/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/utils/*.cpp")


source_group("device" FILES ${DEVICE_SOURCES})
source_group("enumerator" FILES ${ENUMERATOR_SOURCES})
source_group("error" FILES ${ERROR_SOURCES})
source_group("image" FILES ${IMAGE_SOURCES})
source_group("info" FILES ${INFO_SOURCES})
source_group("utils" FILES ${UTILS_SOURCES})

add_library(${LIB_NAME} ${LIB_MODE}
	${DEVICE_SOURCES}
	${ENUMERATOR_SOURCES}
	${ERROR_SOURCES}
	${IMAGE_SOURCES}
	${INFO_SOURCES}
	${UTILS_SOURCES})
	
#IF (WIN32)
IF(MSVC)
  # set stuff for windows
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} WIN32")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  add_definitions(-DWIN32)
  add_definitions(-DUNICODE)
  add_definitions(-D_UNICODE)
  #include(FindDirectShow.cmake)
  #let user to configure baseclass dir in gui mode
  set(DIRECTSHOW_BASECLASS_DIR "C:/Program Files/Microsoft SDKs/Windows/v7.1/Samples/multimedia/directshow/baseclasses" CACHE PATH "Path to directshow baseclasses folder")
  #set(DIRECTSHOW_BASECLASS_DIR "C:/Program Files/Microsoft SDKs/Windows/v7.1/Samples/multimedia/directshow/baseclasses")
  include_directories(${DIRECTSHOW_BASECLASS_DIR})
  include_external_msproject(BaseClasses "${DIRECTSHOW_BASECLASS_DIR}/BaseClasses.vcxproj")
  
  MESSAGE("LIBRARY MODE: ${LIB_MODE}")
  IF(${LIB_MODE} STREQUAL "SHARED")
	#link_directories("C:/Program Files/Microsoft SDKs/Windows/v7.1/Samples/multimedia/directshow/baseclasses/Debug")
	target_link_libraries(${LIB_NAME} debug ${DIRECTSHOW_BASECLASS_DIR}/Debug/strmbasd.lib Winmm.lib)
	target_link_libraries(${LIB_NAME} optimized ${DIRECTSHOW_BASECLASS_DIR}/Release/strmbase.lib Winmm.lib)
  ENDIF()
ELSE()
  # set stuff for other systems
  #add_compile_options(-std=c++11)
	set(CMAKE_CXX_FLAGS "-std=c++0x")
ENDIF()


# Here we define the include paths that will be used by our clients.
# BUILD_INTERFACE will be used when this library is consumed in source form.
# INSTALL_INTERFACE will be used when this library is consumed in binary form.
target_include_directories(webcam PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LIB_SOURCE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
	$<INSTALL_INTERFACE:include/${LIB_SOURCE_DIR}>
)

# Install the header files. The DESTINATION should match with the INSTALL_INTERFACE above.
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_SOURCE_DIR}/ DESTINATION include/${LIB_SOURCE_DIR} PATTERN "*.h")

# Install the compiled file and associate the metadata with an export name.
install(TARGETS ${LIB_NAME} DESTINATION lib/ EXPORT ${TARGET}-config)

# Install the generated metadata targets file.
# Note: If we had multiple targets to export we would have to generate a config file.
# However, since we only export a single target we can use directly the targets file.
install(EXPORT ${TARGET}-config DESTINATION lib/cmake/${TARGET})
