cmake_minimum_required(VERSION 3.16)
project(SammyShuck__wk3_3_1PA4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(SammyShuck__wk3_3_1PA4 SammyShuck_wk3_PA3-1.cpp)

target_link_libraries(SammyShuck__wk3_3_1PA4 glew32)
target_link_libraries(SammyShuck__wk3_3_1PA4 opengl32)
target_link_libraries(SammyShuck__wk3_3_1PA4 glu32)
target_link_libraries(SammyShuck__wk3_3_1PA4 freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)
