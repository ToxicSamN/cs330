cmake_minimum_required(VERSION 3.16)
project(Tutorial5_1)

set(CMAKE_CXX_STANDARD 11)

add_executable(Tutorial5_1 Tutorial5-1.cpp)

target_link_libraries(Tutorial5_1 glew32)
target_link_libraries(Tutorial5_1 opengl32)
target_link_libraries(Tutorial5_1 glu32)
target_link_libraries(Tutorial5_1 freeglut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)