cmake_minimum_required(VERSION 2.8)

project(kalman-tracker)

set(CMAKE_BUILD_TYPE Debug)

find_package( OpenCV REQUIRED )



set(SRC_PATH source )

#########################################################
# Executable
add_executable(track-1of3  ${SRC_PATH}/track-1of3.cpp )
target_link_libraries( track-1of3 ${OpenCV_LIBS} )

add_executable(track-yellow  ${SRC_PATH}/track-yellow.cpp )
target_link_libraries(track-yellow  ${OpenCV_LIBS} )
