cmake_minimum_required(VERSION 3.6)
project(voxels)
#need 14 for variable templates
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(glfw)

find_package(OpenGL REQUIRED)

set(SOURCE_FILES main.cpp)
add_executable(voxels ${SOURCE_FILES})

target_link_libraries(voxels glfw ${GLFW_LIBRARY} ${OPENGL_LIBRARY})