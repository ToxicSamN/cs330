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
CMAKE_SOURCE_DIR = "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Tutorial6.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Tutorial6.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Tutorial6.dir/flags.make

CMakeFiles/Tutorial6.dir/Tutorial6-1.cpp.obj: CMakeFiles/Tutorial6.dir/flags.make
CMakeFiles/Tutorial6.dir/Tutorial6-1.cpp.obj: ../Tutorial6-1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Tutorial6.dir/Tutorial6-1.cpp.obj"
	C:\TDM-GCC-64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Tutorial6.dir\Tutorial6-1.cpp.obj -c "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6\Tutorial6-1.cpp"

CMakeFiles/Tutorial6.dir/Tutorial6-1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Tutorial6.dir/Tutorial6-1.cpp.i"
	C:\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6\Tutorial6-1.cpp" > CMakeFiles\Tutorial6.dir\Tutorial6-1.cpp.i

CMakeFiles/Tutorial6.dir/Tutorial6-1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Tutorial6.dir/Tutorial6-1.cpp.s"
	C:\TDM-GCC-64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6\Tutorial6-1.cpp" -o CMakeFiles\Tutorial6.dir\Tutorial6-1.cpp.s

# Object files for target Tutorial6
Tutorial6_OBJECTS = \
"CMakeFiles/Tutorial6.dir/Tutorial6-1.cpp.obj"

# External object files for target Tutorial6
Tutorial6_EXTERNAL_OBJECTS =

Tutorial6.exe: CMakeFiles/Tutorial6.dir/Tutorial6-1.cpp.obj
Tutorial6.exe: CMakeFiles/Tutorial6.dir/build.make
Tutorial6.exe: CMakeFiles/Tutorial6.dir/linklibs.rsp
Tutorial6.exe: CMakeFiles/Tutorial6.dir/objects1.rsp
Tutorial6.exe: CMakeFiles/Tutorial6.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Tutorial6.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Tutorial6.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Tutorial6.dir/build: Tutorial6.exe

.PHONY : CMakeFiles/Tutorial6.dir/build

CMakeFiles/Tutorial6.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Tutorial6.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Tutorial6.dir/clean

CMakeFiles/Tutorial6.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6\cmake-build-debug" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6\cmake-build-debug" "C:\Users\Sammy\Google Drive\_SNHU_Course_Work\BSCS-Computer_Science\CS-330-J2956_Computer_Graphic_and_Visualization\code\cs330\Tutorial6\cmake-build-debug\CMakeFiles\Tutorial6.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Tutorial6.dir/depend

