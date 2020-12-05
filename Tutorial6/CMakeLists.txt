cmake_minimum_required(VERSION 3.16)

set (thisProject Tutorial6)
project(${thisProject})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_OBJECT_PATH_MAX 350)

find_library(SOIL2 soil2-debug)

add_executable(${thisProject} Tutorial6-1.cpp)

include_directories(
        include
        lib
        C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib
)
link_directories(
        include
        C:\\TDM-GCC-64\\x86_64-w64-mingw32\\lib
)

target_link_libraries(${thisProject}
        glew32
        opengl32
        glu32
        freeglut
        ${SOIL2}
)


