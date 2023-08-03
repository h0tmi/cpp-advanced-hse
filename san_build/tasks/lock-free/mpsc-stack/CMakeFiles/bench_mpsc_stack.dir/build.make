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
include tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/compiler_depend.make

# Include the progress variables for this target.
include tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/flags.make

tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.o: tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/flags.make
tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/mpsc-stack/mpsc_stack_bench.cpp
tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.o: tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/lock-free/mpsc-stack && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.o -MF CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.o.d -o CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/mpsc-stack/mpsc_stack_bench.cpp

tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/lock-free/mpsc-stack && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/mpsc-stack/mpsc_stack_bench.cpp > CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.i

tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/lock-free/mpsc-stack && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/mpsc-stack/mpsc_stack_bench.cpp -o CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.s

# Object files for target bench_mpsc_stack
bench_mpsc_stack_OBJECTS = \
"CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.o"

# External object files for target bench_mpsc_stack
bench_mpsc_stack_EXTERNAL_OBJECTS =

bench_mpsc_stack: tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/mpsc_stack_bench.cpp.o
bench_mpsc_stack: tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/build.make
bench_mpsc_stack: libbenchmark.a
bench_mpsc_stack: tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bench_mpsc_stack"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/lock-free/mpsc-stack && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bench_mpsc_stack.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/build: bench_mpsc_stack
.PHONY : tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/build

tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/lock-free/mpsc-stack && $(CMAKE_COMMAND) -P CMakeFiles/bench_mpsc_stack.dir/cmake_clean.cmake
.PHONY : tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/clean

tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/mpsc-stack /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/lock-free/mpsc-stack /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/lock-free/mpsc-stack/CMakeFiles/bench_mpsc_stack.dir/depend

