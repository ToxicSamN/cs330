# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2020.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2020.1.1\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/CubeIndices.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CubeIndices.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CubeIndices.dir/flags.make

CMakeFiles/CubeIndices.dir/Tutorial4-4.cpp.obj: CMakeFiles/CubeIndices.dir/flags.make
CMakeFiles/CubeIndices.dir/Tutorial4-4.cpp.obj: ../Tutorial4-4.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CubeIndices.dir/Tutorial4-4.cpp.obj"
	C:\TDM-GCC-64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\CubeIndices.dir\Tutorial4-4.cpp.obj -c "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices\Tutorial4-4.cpp"

CMakeFiles/CubeIndices.dir/Tutorial4-4.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CubeIndices.dir/Tutorial4-4.cpp.i"
	C:\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices\Tutorial4-4.cpp" > CMakeFiles\CubeIndices.dir\Tutorial4-4.cpp.i

CMakeFiles/CubeIndices.dir/Tutorial4-4.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CubeIndices.dir/Tutorial4-4.cpp.s"
	C:\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices\Tutorial4-4.cpp" -o CMakeFiles\CubeIndices.dir\Tutorial4-4.cpp.s

# Object files for target CubeIndices
CubeIndices_OBJECTS = \
"CMakeFiles/CubeIndices.dir/Tutorial4-4.cpp.obj"

# External object files for target CubeIndices
CubeIndices_EXTERNAL_OBJECTS =

CubeIndices.exe: CMakeFiles/CubeIndices.dir/Tutorial4-4.cpp.obj
CubeIndices.exe: CMakeFiles/CubeIndices.dir/build.make
CubeIndices.exe: CMakeFiles/CubeIndices.dir/linklibs.rsp
CubeIndices.exe: CMakeFiles/CubeIndices.dir/objects1.rsp
CubeIndices.exe: CMakeFiles/CubeIndices.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CubeIndices.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\CubeIndices.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CubeIndices.dir/build: CubeIndices.exe

.PHONY : CMakeFiles/CubeIndices.dir/build

CMakeFiles/CubeIndices.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\CubeIndices.dir\cmake_clean.cmake
.PHONY : CMakeFiles/CubeIndices.dir/clean

CMakeFiles/CubeIndices.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices\cmake-build-debug" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices\cmake-build-debug" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\CubeIndices\cmake-build-debug\CMakeFiles\CubeIndices.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/CubeIndices.dir/depend

