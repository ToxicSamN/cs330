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
CMAKE_SOURCE_DIR = "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Transform.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Transform.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Transform.dir/flags.make

CMakeFiles/Transform.dir/Transform.cpp.obj: CMakeFiles/Transform.dir/flags.make
CMakeFiles/Transform.dir/Transform.cpp.obj: ../Transform.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Transform.dir/Transform.cpp.obj"
	C:\TDM-GCC-64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Transform.dir\Transform.cpp.obj -c "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform\Transform.cpp"

CMakeFiles/Transform.dir/Transform.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Transform.dir/Transform.cpp.i"
	C:\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform\Transform.cpp" > CMakeFiles\Transform.dir\Transform.cpp.i

CMakeFiles/Transform.dir/Transform.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Transform.dir/Transform.cpp.s"
	C:\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform\Transform.cpp" -o CMakeFiles\Transform.dir\Transform.cpp.s

# Object files for target Transform
Transform_OBJECTS = \
"CMakeFiles/Transform.dir/Transform.cpp.obj"

# External object files for target Transform
Transform_EXTERNAL_OBJECTS =

Transform.exe: CMakeFiles/Transform.dir/Transform.cpp.obj
Transform.exe: CMakeFiles/Transform.dir/build.make
Transform.exe: CMakeFiles/Transform.dir/linklibs.rsp
Transform.exe: CMakeFiles/Transform.dir/objects1.rsp
Transform.exe: CMakeFiles/Transform.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Transform.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Transform.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Transform.dir/build: Transform.exe

.PHONY : CMakeFiles/Transform.dir/build

CMakeFiles/Transform.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Transform.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Transform.dir/clean

CMakeFiles/Transform.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform\cmake-build-debug" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform\cmake-build-debug" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\Transform\cmake-build-debug\CMakeFiles\Transform.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Transform.dir/depend

