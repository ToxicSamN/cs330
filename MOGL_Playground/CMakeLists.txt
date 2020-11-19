cmake_minimum_required(VERSION 3.16)
project(MOGL_Playground)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(MOGL_Playground main.cpp)

target_link_libraries(MOGL_Playground glew32)
target_link_libraries(MOGL_Playground opengl32)
target_link_libraries(MOGL_Playground glu32)
target_link_libraries(MOGL_Playground freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)
