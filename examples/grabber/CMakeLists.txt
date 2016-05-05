cmake_minimum_required(VERSION 2.8.12)

project(grabber)
add_executable(grabber grabber.cpp)

IF(MSVC)
	add_definitions(-DWIN32)
	include_directories(${LIBWEBCAM_DIR})
	target_link_libraries(grabber optimized ${LIBWEBCAM_DIR}/build/Release/strmbase.lib)
	target_link_libraries(grabber optimized ${LIBWEBCAM_DIR}/build/Release/webcam.lib Winmm.lib)
	
	target_link_libraries(grabber debug ${LIBWEBCAM_DIR}/build/Debug/strmbasd.lib)
	target_link_libraries(grabber debug ${LIBWEBCAM_DIR}/build/Debug/webcam.lib Winmm.lib)
	
ELSE()
	find_package(libwebcam REQUIRED)
	target_link_libraries(grabber webcam)
ENDIF()
  