cmake_minimum_required(VERSION 3.16)
project(SammyShuck__wk1_PracticeActivity1_3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_OBJECT_PATH_MAX 350)

add_executable(SammyShuck__wk1_PracticeActivity1_3 SammyShuck__wk1_PA1-3.cpp)

target_link_libraries(SammyShuck__wk1_PracticeActivity1_3 opengl32)
target_link_libraries(SammyShuck__wk1_PracticeActivity1_3 glu32)
target_link_libraries(SammyShuck__wk1_PracticeActivity1_3 freeGlut)

SET(SOIL SOIL)
link_directories(C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib\\SOIL)