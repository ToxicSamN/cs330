cmake_minimum_required(VERSION 3.16)
project(SammyShuck_FinalProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(SammyShuck_FinalProject SammyShuck_FinalProject_WIP.cpp)

target_link_libraries(SammyShuck_FinalProject glew32)
target_link_libraries(SammyShuck_FinalProject opengl32)
target_link_libraries(SammyShuck_FinalProject glu32)
target_link_libraries(SammyShuck_FinalProject freeglut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)
