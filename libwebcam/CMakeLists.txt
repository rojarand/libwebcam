cmake_minimum_required(VERSION 3.2.2)

project(libwebcam)

set(CMAKE_CXX_STANDARD 11)

# Set the compiled file to the TARGET variable.
set(TARGET "libwebcam")
set(LIB_NAME "webcam")
set(LIB_MODE "STATIC")# "SHARED", "STATIC"

# Collect sources into the variable
file (GLOB_RECURSE DEVICE_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/device/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/device/*.cpp")
file (GLOB_RECURSE ERROR_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/error/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/error/*.cpp")
file (GLOB_RECURSE IMAGE_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/image/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/image/*.cpp")
file (GLOB_RECURSE INFO_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/info/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/info/*.cpp")	  
file (GLOB_RECURSE UTILS_SOURCES
      "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp")
file (GLOB INCLUDER
      "${CMAKE_CURRENT_SOURCE_DIR}/webcam.h")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

source_group("device" FILES ${DEVICE_SOURCES})
source_group("error" FILES ${ERROR_SOURCES})
source_group("image" FILES ${IMAGE_SOURCES})
source_group("info" FILES ${INFO_SOURCES})
source_group("utils" FILES ${UTILS_SOURCES})

add_library(${LIB_NAME} ${LIB_MODE}
	${DEVICE_SOURCES}
	${ERROR_SOURCES}
	${IMAGE_SOURCES}
	${INFO_SOURCES}
	${UTILS_SOURCES}
	${INCLUDER})
	

install(TARGETS ${LIB_NAME} DESTINATION lib ARCHIVE DESTINATION lib)


# Here we define the include paths that will be used by our clients.
# BUILD_INTERFACE will be used when this library is consumed in source form.
# INSTALL_INTERFACE will be used when this library is consumed in binary form.
#target_include_directories(webcam PUBLIC
#	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${LIB_SOURCE_DIR}>
#	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
#	$<INSTALL_INTERFACE:include/${LIB_SOURCE_DIR}>
#)

# Install the header files. The DESTINATION should match with the INSTALL_INTERFACE above.
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_SOURCE_DIR}/ DESTINATION include/${LIB_SOURCE_DIR} PATTERN "*.h")

# Install the compiled file and associate the metadata with an export name.
#install(TARGETS ${LIB_NAME} DESTINATION lib/ EXPORT ${TARGET}-config)

# Install the generated metadata targets file.
# Note: If we had multiple targets to export we would have to generate a config file.
# However, since we only export a single target we can use directly the targets file.
#install(EXPORT ${TARGET}-config DESTINATION lib/cmake/${TARGET})
