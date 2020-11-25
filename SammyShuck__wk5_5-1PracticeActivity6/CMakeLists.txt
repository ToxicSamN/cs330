cmake_minimum_required(VERSION 3.16)
project(SammyShuck__wk5_PracticeActivity6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(SammyShuck__wk5_PracticeActivity6 SammyShuck_wk5_PA6.cpp)

target_link_libraries(SammyShuck__wk5_PracticeActivity6 glew32)
target_link_libraries(SammyShuck__wk5_PracticeActivity6 opengl32)
target_link_libraries(SammyShuck__wk5_PracticeActivity6 glu32)
target_link_libraries(SammyShuck__wk5_PracticeActivity6 freeglut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)