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
CMAKE_BINARY_DIR = /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build

# Include any dependencies generated for this target.
include tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/compiler_depend.make

# Include the progress variables for this target.
include tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/flags.make

tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/run.cpp.o: tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/flags.make
tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/run.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/fast-queue/run.cpp
tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/run.cpp.o: tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/run.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/lock-free/fast-queue && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/run.cpp.o -MF CMakeFiles/bench_fast_queue.dir/run.cpp.o.d -o CMakeFiles/bench_fast_queue.dir/run.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/fast-queue/run.cpp

tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/run.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bench_fast_queue.dir/run.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/lock-free/fast-queue && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/fast-queue/run.cpp > CMakeFiles/bench_fast_queue.dir/run.cpp.i

tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/run.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bench_fast_queue.dir/run.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/lock-free/fast-queue && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/fast-queue/run.cpp -o CMakeFiles/bench_fast_queue.dir/run.cpp.s

# Object files for target bench_fast_queue
bench_fast_queue_OBJECTS = \
"CMakeFiles/bench_fast_queue.dir/run.cpp.o"

# External object files for target bench_fast_queue
bench_fast_queue_EXTERNAL_OBJECTS =

bench_fast_queue: tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/run.cpp.o
bench_fast_queue: tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/build.make
bench_fast_queue: libbenchmark.a
bench_fast_queue: tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bench_fast_queue"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/lock-free/fast-queue && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bench_fast_queue.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/build: bench_fast_queue
.PHONY : tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/build

tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/lock-free/fast-queue && $(CMAKE_COMMAND) -P CMakeFiles/bench_fast_queue.dir/cmake_clean.cmake
.PHONY : tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/clean

tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/lock-free/fast-queue /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/lock-free/fast-queue /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/lock-free/fast-queue/CMakeFiles/bench_fast_queue.dir/depend

