cmake_minimum_required(VERSION 3.16)
project(Polygon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(Polygon Polygon.cpp)

target_link_libraries(Polygon opengl32)
target_link_libraries(Polygon glu32)
target_link_libraries(Polygon freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)