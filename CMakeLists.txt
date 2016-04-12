cmake_minimum_required(VERSION 2.8.12)

project(libwebcam)

#include(../CMakeInclude.txt)

# Add all the source files to the SOURCES variable.
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/libwebcam/*.cpp")

# Set the compiled file to the TARGET variable.
set(TARGET "libwebcam")

add_library(webcam SHARED ${SOURCES})

#add_compile_options(-std=c++11)
SET(CMAKE_CXX_FLAGS "-std=c++0x")

# Here we define the include paths that will be used by our clients.
# BUILD_INTERFACE will be used when this library is consumed in source form.
# INSTALL_INTERFACE will be used when this library is consumed in binary form.
target_include_directories(webcam PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libwebcam>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
	$<INSTALL_INTERFACE:include/${TARGET}>
)

# Install the header files. The DESTINATION should match with the INSTALL_INTERFACE above.
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}/ DESTINATION include/${TARGET} PATTERN "*.h")

# Install the compiled file and associate the metadata with an export name.
install(TARGETS webcam DESTINATION lib/ EXPORT ${TARGET}-config)

# Install the generated metadata targets file.
# Note: If we had multiple targets to export we would have to generate a config file.
# However, since we only export a single target we can use directly the targets file.
install(EXPORT ${TARGET}-config DESTINATION lib/cmake/${TARGET})
