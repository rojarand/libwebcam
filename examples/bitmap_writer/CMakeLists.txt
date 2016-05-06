cmake_minimum_required(VERSION 2.8.12)

project(bitmap_writer)
add_executable(bitmap_writer bitmap_writer.cpp)

IF(MSVC)
	add_definitions(-DWIN32)
	include_directories(${LIBWEBCAM_DIR})
	target_link_libraries(bitmap_writer optimized ${LIBWEBCAM_DIR}/build/Release/strmbase.lib)
	target_link_libraries(bitmap_writer optimized ${LIBWEBCAM_DIR}/build/Release/webcam.lib Winmm.lib)
	
	target_link_libraries(bitmap_writer debug ${LIBWEBCAM_DIR}/build/Debug/strmbasd.lib)
	target_link_libraries(bitmap_writer debug ${LIBWEBCAM_DIR}/build/Debug/webcam.lib Winmm.lib)
	
ELSE()
	set(CMAKE_CXX_FLAGS "-std=c++0x")
	find_package(libwebcam REQUIRED)
	target_link_libraries(bitmap_writer webcam)
ENDIF()
  