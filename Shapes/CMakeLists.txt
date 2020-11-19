cmake_minimum_required(VERSION 3.16)
project(Shapes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(Shapes Shapes.cpp)

target_link_libraries(Shapes opengl32)
target_link_libraries(Shapes glu32)
target_link_libraries(Shapes freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)