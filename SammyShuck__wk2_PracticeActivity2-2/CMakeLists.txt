cmake_minimum_required(VERSION 3.16)
project(SammyShuck__wk2_PA2_2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(SammyShuck__wk2_PA2_2 SammyShuck__wk2_PA2-2.cpp)

target_link_libraries(SammyShuck__wk2_PA2_2 opengl32)
target_link_libraries(SammyShuck__wk2_PA2_2 glu32)
target_link_libraries(SammyShuck__wk2_PA2_2 freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)