cmake_minimum_required(VERSION 3.16)
project(ModelMatrix)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(ModelMatrix main.cpp)

target_link_libraries(ModelMatrix glew32)
target_link_libraries(ModelMatrix opengl32)
target_link_libraries(ModelMatrix glu32)
target_link_libraries(ModelMatrix freeglut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)