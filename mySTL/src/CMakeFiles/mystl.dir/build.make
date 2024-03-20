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
CMAKE_BINARY_DIR = /home/wangxian/桌面/up/mySTL/src

# Include any dependencies generated for this target.
include CMakeFiles/mystl.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/mystl.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/mystl.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mystl.dir/flags.make

CMakeFiles/mystl.dir/algorithm.cpp.o: CMakeFiles/mystl.dir/flags.make
CMakeFiles/mystl.dir/algorithm.cpp.o: algorithm.cpp
CMakeFiles/mystl.dir/algorithm.cpp.o: CMakeFiles/mystl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangxian/桌面/up/mySTL/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/mystl.dir/algorithm.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mystl.dir/algorithm.cpp.o -MF CMakeFiles/mystl.dir/algorithm.cpp.o.d -o CMakeFiles/mystl.dir/algorithm.cpp.o -c /home/wangxian/桌面/up/mySTL/src/algorithm.cpp

CMakeFiles/mystl.dir/algorithm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mystl.dir/algorithm.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangxian/桌面/up/mySTL/src/algorithm.cpp > CMakeFiles/mystl.dir/algorithm.cpp.i

CMakeFiles/mystl.dir/algorithm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mystl.dir/algorithm.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangxian/桌面/up/mySTL/src/algorithm.cpp -o CMakeFiles/mystl.dir/algorithm.cpp.s

CMakeFiles/mystl.dir/alloc.cpp.o: CMakeFiles/mystl.dir/flags.make
CMakeFiles/mystl.dir/alloc.cpp.o: alloc.cpp
CMakeFiles/mystl.dir/alloc.cpp.o: CMakeFiles/mystl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangxian/桌面/up/mySTL/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/mystl.dir/alloc.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/mystl.dir/alloc.cpp.o -MF CMakeFiles/mystl.dir/alloc.cpp.o.d -o CMakeFiles/mystl.dir/alloc.cpp.o -c /home/wangxian/桌面/up/mySTL/src/alloc.cpp

CMakeFiles/mystl.dir/alloc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mystl.dir/alloc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangxian/桌面/up/mySTL/src/alloc.cpp > CMakeFiles/mystl.dir/alloc.cpp.i

CMakeFiles/mystl.dir/alloc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mystl.dir/alloc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangxian/桌面/up/mySTL/src/alloc.cpp -o CMakeFiles/mystl.dir/alloc.cpp.s

# Object files for target mystl
mystl_OBJECTS = \
"CMakeFiles/mystl.dir/algorithm.cpp.o" \
"CMakeFiles/mystl.dir/alloc.cpp.o"

# External object files for target mystl
mystl_EXTERNAL_OBJECTS =

../lib/libmystl.so: CMakeFiles/mystl.dir/algorithm.cpp.o
../lib/libmystl.so: CMakeFiles/mystl.dir/alloc.cpp.o
../lib/libmystl.so: CMakeFiles/mystl.dir/build.make
../lib/libmystl.so: CMakeFiles/mystl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wangxian/桌面/up/mySTL/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../lib/libmystl.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mystl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mystl.dir/build: ../lib/libmystl.so
.PHONY : CMakeFiles/mystl.dir/build

CMakeFiles/mystl.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mystl.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mystl.dir/clean

CMakeFiles/mystl.dir/depend:
	cd /home/wangxian/桌面/up/mySTL/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wangxian/桌面/up/mySTL /home/wangxian/桌面/up/mySTL /home/wangxian/桌面/up/mySTL/src /home/wangxian/桌面/up/mySTL/src /home/wangxian/桌面/up/mySTL/src/CMakeFiles/mystl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mystl.dir/depend

