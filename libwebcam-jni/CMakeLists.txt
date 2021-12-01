cmake_minimum_required(VERSION 3.2.2)

project(webcamjni VERSION ${CMAKE_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 11)

find_package(JNI)
find_package(JPEG REQUIRED)

if (JNI_FOUND)
    message (STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
    message (STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
else()
    message (STATUS "Can't find JNI")
endif()

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/libwebcam
        src/main/cpp
        ${JNI_INCLUDE_DIRS})

# build application's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

file(GLOB JNI_SRC "src/main/cpp/*.cpp")

add_library(webcamjni SHARED ${JNI_SRC})

set_target_properties(webcamjni PROPERTIES
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${PROJECT_VERSION_MAJOR})

target_link_libraries(webcamjni webcam jpeg)
