cmake_minimum_required(VERSION 3.16)
project(SammyShuck__wk4_4_1PA5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(SammyShuck__wk4_4_1PA5 SammyShuck_wk4_PA4-1.cpp)

target_link_libraries(SammyShuck__wk4_4_1PA5 glew32)
target_link_libraries(SammyShuck__wk4_4_1PA5 opengl32)
target_link_libraries(SammyShuck__wk4_4_1PA5 glu32)
target_link_libraries(SammyShuck__wk4_4_1PA5 freeglut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)