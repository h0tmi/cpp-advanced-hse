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
include tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/compiler_depend.make

# Include the progress variables for this target.
include tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/flags.make

tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/test.cpp.o: tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/flags.make
tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/test.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/meta/constexpr-map/test.cpp
tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/test.cpp.o: tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/test.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/meta/constexpr-map && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/test.cpp.o -MF CMakeFiles/test_constexpr_map.dir/test.cpp.o.d -o CMakeFiles/test_constexpr_map.dir/test.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/meta/constexpr-map/test.cpp

tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_constexpr_map.dir/test.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/meta/constexpr-map && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/meta/constexpr-map/test.cpp > CMakeFiles/test_constexpr_map.dir/test.cpp.i

tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_constexpr_map.dir/test.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/meta/constexpr-map && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/meta/constexpr-map/test.cpp -o CMakeFiles/test_constexpr_map.dir/test.cpp.s

# Object files for target test_constexpr_map
test_constexpr_map_OBJECTS = \
"CMakeFiles/test_constexpr_map.dir/test.cpp.o"

# External object files for target test_constexpr_map
test_constexpr_map_EXTERNAL_OBJECTS =

test_constexpr_map: tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/test.cpp.o
test_constexpr_map: tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/build.make
test_constexpr_map: libcontrib_catch_main.a
test_constexpr_map: tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../test_constexpr_map"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/meta/constexpr-map && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_constexpr_map.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/build: test_constexpr_map
.PHONY : tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/build

tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/meta/constexpr-map && $(CMAKE_COMMAND) -P CMakeFiles/test_constexpr_map.dir/cmake_clean.cmake
.PHONY : tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/clean

tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/meta/constexpr-map /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/meta/constexpr-map /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/meta/constexpr-map/CMakeFiles/test_constexpr_map.dir/depend
