cmake_minimum_required(VERSION 3.16)
project(Demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(Demo main.cpp)

target_link_libraries(Demo opengl32)
target_link_libraries(Demo glu32)
target_link_libraries(Demo freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)