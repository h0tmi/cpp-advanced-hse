# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build

# Include any dependencies generated for this target.
include tasks/unassigned/list/CMakeFiles/test_list.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tasks/unassigned/list/CMakeFiles/test_list.dir/compiler_depend.make

# Include the progress variables for this target.
include tasks/unassigned/list/CMakeFiles/test_list.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/unassigned/list/CMakeFiles/test_list.dir/flags.make

tasks/unassigned/list/CMakeFiles/test_list.dir/test.cpp.o: tasks/unassigned/list/CMakeFiles/test_list.dir/flags.make
tasks/unassigned/list/CMakeFiles/test_list.dir/test.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/unassigned/list/test.cpp
tasks/unassigned/list/CMakeFiles/test_list.dir/test.cpp.o: tasks/unassigned/list/CMakeFiles/test_list.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/unassigned/list/CMakeFiles/test_list.dir/test.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/unassigned/list && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/unassigned/list/CMakeFiles/test_list.dir/test.cpp.o -MF CMakeFiles/test_list.dir/test.cpp.o.d -o CMakeFiles/test_list.dir/test.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/unassigned/list/test.cpp

tasks/unassigned/list/CMakeFiles/test_list.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_list.dir/test.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/unassigned/list && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/unassigned/list/test.cpp > CMakeFiles/test_list.dir/test.cpp.i

tasks/unassigned/list/CMakeFiles/test_list.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_list.dir/test.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/unassigned/list && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/unassigned/list/test.cpp -o CMakeFiles/test_list.dir/test.cpp.s

# Object files for target test_list
test_list_OBJECTS = \
"CMakeFiles/test_list.dir/test.cpp.o"

# External object files for target test_list
test_list_EXTERNAL_OBJECTS =

test_list: tasks/unassigned/list/CMakeFiles/test_list.dir/test.cpp.o
test_list: tasks/unassigned/list/CMakeFiles/test_list.dir/build.make
test_list: libcontrib_catch_main.a
test_list: tasks/unassigned/list/CMakeFiles/test_list.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../test_list"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/unassigned/list && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_list.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/unassigned/list/CMakeFiles/test_list.dir/build: test_list
.PHONY : tasks/unassigned/list/CMakeFiles/test_list.dir/build

tasks/unassigned/list/CMakeFiles/test_list.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/unassigned/list && $(CMAKE_COMMAND) -P CMakeFiles/test_list.dir/cmake_clean.cmake
.PHONY : tasks/unassigned/list/CMakeFiles/test_list.dir/clean

tasks/unassigned/list/CMakeFiles/test_list.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/unassigned/list /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/unassigned/list /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/unassigned/list/CMakeFiles/test_list.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/unassigned/list/CMakeFiles/test_list.dir/depend

