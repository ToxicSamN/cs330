cmake_minimum_required(VERSION 3.16)
project(Triangles)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(Triangles TriangleOGL.cpp)

target_link_libraries(Triangles opengl32)
target_link_libraries(Triangles glu32)
target_link_libraries(Triangles freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)