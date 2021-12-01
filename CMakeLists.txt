cmake_minimum_required(VERSION 3.2.2)

project(libwebcam_all VERSION 1.0.1)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Remove any ambiguity about what mode it is in
IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message("-- Debug Mode --")
ELSE()
    message("-- Release Mode --")
ENDIF()


# Options. Turn on with 'cmake -Dmyvarname=ON'.
option(test "Build all tests." ON) # Makes boolean 'test' available.
enable_testing()

add_subdirectory(libwebcam)
add_subdirectory(libwebcam-jni)

#add_subdirectory(gtest)
