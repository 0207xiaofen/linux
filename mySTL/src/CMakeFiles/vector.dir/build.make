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
include CMakeFiles/vector.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/vector.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/vector.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vector.dir/flags.make

CMakeFiles/vector.dir/test/test_vector.cpp.o: CMakeFiles/vector.dir/flags.make
CMakeFiles/vector.dir/test/test_vector.cpp.o: ../test/test_vector.cpp
CMakeFiles/vector.dir/test/test_vector.cpp.o: CMakeFiles/vector.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangxian/桌面/up/mySTL/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/vector.dir/test/test_vector.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/vector.dir/test/test_vector.cpp.o -MF CMakeFiles/vector.dir/test/test_vector.cpp.o.d -o CMakeFiles/vector.dir/test/test_vector.cpp.o -c /home/wangxian/桌面/up/mySTL/test/test_vector.cpp

CMakeFiles/vector.dir/test/test_vector.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vector.dir/test/test_vector.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangxian/桌面/up/mySTL/test/test_vector.cpp > CMakeFiles/vector.dir/test/test_vector.cpp.i

CMakeFiles/vector.dir/test/test_vector.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vector.dir/test/test_vector.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangxian/桌面/up/mySTL/test/test_vector.cpp -o CMakeFiles/vector.dir/test/test_vector.cpp.s

CMakeFiles/vector.dir/alloc.cpp.o: CMakeFiles/vector.dir/flags.make
CMakeFiles/vector.dir/alloc.cpp.o: alloc.cpp
CMakeFiles/vector.dir/alloc.cpp.o: CMakeFiles/vector.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wangxian/桌面/up/mySTL/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/vector.dir/alloc.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/vector.dir/alloc.cpp.o -MF CMakeFiles/vector.dir/alloc.cpp.o.d -o CMakeFiles/vector.dir/alloc.cpp.o -c /home/wangxian/桌面/up/mySTL/src/alloc.cpp

CMakeFiles/vector.dir/alloc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vector.dir/alloc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wangxian/桌面/up/mySTL/src/alloc.cpp > CMakeFiles/vector.dir/alloc.cpp.i

CMakeFiles/vector.dir/alloc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vector.dir/alloc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wangxian/桌面/up/mySTL/src/alloc.cpp -o CMakeFiles/vector.dir/alloc.cpp.s

# Object files for target vector
vector_OBJECTS = \
"CMakeFiles/vector.dir/test/test_vector.cpp.o" \
"CMakeFiles/vector.dir/alloc.cpp.o"

# External object files for target vector
vector_EXTERNAL_OBJECTS =

../TEST/vector: CMakeFiles/vector.dir/test/test_vector.cpp.o
../TEST/vector: CMakeFiles/vector.dir/alloc.cpp.o
../TEST/vector: CMakeFiles/vector.dir/build.make
../TEST/vector: CMakeFiles/vector.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wangxian/桌面/up/mySTL/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../TEST/vector"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vector.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vector.dir/build: ../TEST/vector
.PHONY : CMakeFiles/vector.dir/build

CMakeFiles/vector.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vector.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vector.dir/clean

CMakeFiles/vector.dir/depend:
	cd /home/wangxian/桌面/up/mySTL/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wangxian/桌面/up/mySTL /home/wangxian/桌面/up/mySTL /home/wangxian/桌面/up/mySTL/src /home/wangxian/桌面/up/mySTL/src /home/wangxian/桌面/up/mySTL/src/CMakeFiles/vector.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vector.dir/depend

