cmake_minimum_required(VERSION 2.8.12)

project(enumerator)
add_executable(enumerator enumerator.cpp)

IF(MSVC)
	#set(WEBCAM_PROJ_DIR "" CACHE PATH "Path to libwebcam Visual Studio project folder")
	#include_external_msproject(libwebcam "${WEBCAM_PROJ_DIR}/webcam.vcxproj")
	include_directories(${LIBWEBCAM_DIR})
	target_link_libraries(enumerator optimized ${LIBWEBCAM_DIR}/build/Release/strmbase.lib)
	target_link_libraries(enumerator optimized ${LIBWEBCAM_DIR}/build/Release/webcam.lib Winmm.lib)
	
	target_link_libraries(enumerator debug ${LIBWEBCAM_DIR}/build/Debug/strmbasd.lib)
	target_link_libraries(enumerator debug ${LIBWEBCAM_DIR}/build/Debug/webcam.lib Winmm.lib)
	
ELSE()
	set(CMAKE_CXX_FLAGS "-std=c++0x")
ENDIF()
  