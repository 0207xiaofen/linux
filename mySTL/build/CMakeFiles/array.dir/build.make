# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wangxian/桌面/up/mySTL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wangxian/桌面/up/mySTL/build

# Include any dependencies generated for this target.
include CMakeFiles/array.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/array.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/array.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/array.dir/flags.make

CMakeFiles/array.dir/test/test_array.cpp.o: CMakeFiles/array.dir/flags.make
CMakeFiles/array.dir/test/test_array.cpp.o: ../test/test_array.cpp
CMakeFiles/array.dir/test/test_array.cpp.o: CMakeFiles/array.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangxian/桌面/up/mySTL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/array.dir/test/test_array.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/array.dir/test/test_array.cpp.o -MF CMakeFiles/array.dir/test/test_array.cpp.o.d -o CMakeFiles/array.dir/test/test_array.cpp.o -c /home/wangxian/桌面/up/mySTL/test/test_array.cpp

CMakeFiles/array.dir/test/test_array.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/array.dir/test/test_array.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangxian/桌面/up/mySTL/test/test_array.cpp > CMakeFiles/array.dir/test/test_array.cpp.i

CMakeFiles/array.dir/test/test_array.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/array.dir/test/test_array.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangxian/桌面/up/mySTL/test/test_array.cpp -o CMakeFiles/array.dir/test/test_array.cpp.s

# Object files for target array
array_OBJECTS = \
"CMakeFiles/array.dir/test/test_array.cpp.o"

# External object files for target array
array_EXTERNAL_OBJECTS =

../TEST/array: CMakeFiles/array.dir/test/test_array.cpp.o
../TEST/array: CMakeFiles/array.dir/build.make
../TEST/array: CMakeFiles/array.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wangxian/桌面/up/mySTL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../TEST/array"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/array.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/array.dir/build: ../TEST/array
.PHONY : CMakeFiles/array.dir/build

CMakeFiles/array.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/array.dir/cmake_clean.cmake
.PHONY : CMakeFiles/array.dir/clean

CMakeFiles/array.dir/depend:
	cd /home/wangxian/桌面/up/mySTL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wangxian/桌面/up/mySTL /home/wangxian/桌面/up/mySTL /home/wangxian/桌面/up/mySTL/build /home/wangxian/桌面/up/mySTL/build /home/wangxian/桌面/up/mySTL/build/CMakeFiles/array.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/array.dir/depend

