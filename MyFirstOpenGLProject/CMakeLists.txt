cmake_minimum_required(VERSION 3.16)
project(MyFirstOpenGLProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyFirstOpenGLProject MyFirstOpenGLProject.cpp)

target_link_libraries(MyFirstOpenGLProject opengl32)
target_link_libraries(MyFirstOpenGLProject glu32)
target_link_libraries(MyFirstOpenGLProject freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)