cmake_minimum_required(VERSION 3.10)

project( Depixelize )

set(CMAKE_CXX_STANDARD 14)

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} )

add_subdirectory( src )

add_executable( Depix main.cpp )

target_link_libraries( Depix ${OpenCV_LIBS} srcDepix )

target_include_directories(Depix PUBLIC src)