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
include tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/compiler_depend.make

# Include the progress variables for this target.
include tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/flags.make

tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.o: tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/flags.make
tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/scheme/basic/repl/main.cpp
tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.o: tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/scheme/basic && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.o -MF CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.o.d -o CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/scheme/basic/repl/main.cpp

tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/scheme/basic && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/scheme/basic/repl/main.cpp > CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.i

tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/scheme/basic && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/scheme/basic/repl/main.cpp -o CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.s

# Object files for target scheme_basic_repl
scheme_basic_repl_OBJECTS = \
"CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.o"

# External object files for target scheme_basic_repl
scheme_basic_repl_EXTERNAL_OBJECTS =

scheme_basic_repl: tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/repl/main.cpp.o
scheme_basic_repl: tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/build.make
scheme_basic_repl: tasks/scheme/basic/libscheme_basic.a
scheme_basic_repl: tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../scheme_basic_repl"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/scheme/basic && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/scheme_basic_repl.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/build: scheme_basic_repl
.PHONY : tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/build

tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/scheme/basic && $(CMAKE_COMMAND) -P CMakeFiles/scheme_basic_repl.dir/cmake_clean.cmake
.PHONY : tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/clean

tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/scheme/basic /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/scheme/basic /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/scheme/basic/CMakeFiles/scheme_basic_repl.dir/depend

