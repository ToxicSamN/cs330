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
CMAKE_SOURCE_DIR = "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Quad.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Quad.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Quad.dir/flags.make

CMakeFiles/Quad.dir/Quad.cpp.obj: CMakeFiles/Quad.dir/flags.make
CMakeFiles/Quad.dir/Quad.cpp.obj: ../Quad.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Quad.dir/Quad.cpp.obj"
	C:\TDM-GCC-64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Quad.dir\Quad.cpp.obj -c "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad\Quad.cpp"

CMakeFiles/Quad.dir/Quad.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Quad.dir/Quad.cpp.i"
	C:\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad\Quad.cpp" > CMakeFiles\Quad.dir\Quad.cpp.i

CMakeFiles/Quad.dir/Quad.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Quad.dir/Quad.cpp.s"
	C:\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad\Quad.cpp" -o CMakeFiles\Quad.dir\Quad.cpp.s

# Object files for target Quad
Quad_OBJECTS = \
"CMakeFiles/Quad.dir/Quad.cpp.obj"

# External object files for target Quad
Quad_EXTERNAL_OBJECTS =

Quad.exe: CMakeFiles/Quad.dir/Quad.cpp.obj
Quad.exe: CMakeFiles/Quad.dir/build.make
Quad.exe: CMakeFiles/Quad.dir/linklibs.rsp
Quad.exe: CMakeFiles/Quad.dir/objects1.rsp
Quad.exe: CMakeFiles/Quad.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Quad.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Quad.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Quad.dir/build: Quad.exe

.PHONY : CMakeFiles/Quad.dir/build

CMakeFiles/Quad.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Quad.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Quad.dir/clean

CMakeFiles/Quad.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad\cmake-build-debug" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad\cmake-build-debug" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Quad\cmake-build-debug\CMakeFiles\Quad.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Quad.dir/depend
