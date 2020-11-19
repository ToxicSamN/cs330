cmake_minimum_required(VERSION 3.16)
project(Transform)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(Transform Transform.cpp)

target_link_libraries(Transform opengl32)
target_link_libraries(Transform glu32)
target_link_libraries(Transform freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)