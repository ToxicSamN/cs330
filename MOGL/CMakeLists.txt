cmake_minimum_required(VERSION 3.16)
project(MOGL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(MOGL MOGL.cpp)

target_link_libraries(MOGL glew32)
target_link_libraries(MOGL opengl32)
target_link_libraries(MOGL glu32)
target_link_libraries(MOGL freeglut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)