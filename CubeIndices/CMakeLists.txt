cmake_minimum_required(VERSION 3.16)
project(CubeIndices)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OBJECT_PATH_MAX 350)

 add_executable(CubeIndices Tutorial4-4.cpp)
#add_executable(CubeIndices Tutorial4-4_verbatim.cpp)

target_link_libraries(CubeIndices glew32)
target_link_libraries(CubeIndices opengl32)
target_link_libraries(CubeIndices glu32)
target_link_libraries(CubeIndices freeglut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)