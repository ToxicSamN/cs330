cmake_minimum_required(VERSION 3.16)
project(ModelViewProjection)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(ModelViewProjection Tutorial4-3.cpp)

target_link_libraries(ModelViewProjection glew32)
target_link_libraries(ModelViewProjection opengl32)
target_link_libraries(ModelViewProjection glu32)
target_link_libraries(ModelViewProjection freeglut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)