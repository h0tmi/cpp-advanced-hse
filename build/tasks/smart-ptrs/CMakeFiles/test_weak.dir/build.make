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
include tasks/smart-ptrs/CMakeFiles/test_weak.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tasks/smart-ptrs/CMakeFiles/test_weak.dir/compiler_depend.make

# Include the progress variables for this target.
include tasks/smart-ptrs/CMakeFiles/test_weak.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/smart-ptrs/CMakeFiles/test_weak.dir/flags.make

tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test.cpp.o: tasks/smart-ptrs/CMakeFiles/test_weak.dir/flags.make
tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/smart-ptrs/weak/test.cpp
tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test.cpp.o: tasks/smart-ptrs/CMakeFiles/test_weak.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test.cpp.o -MF CMakeFiles/test_weak.dir/weak/test.cpp.o.d -o CMakeFiles/test_weak.dir/weak/test.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/smart-ptrs/weak/test.cpp

tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_weak.dir/weak/test.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/smart-ptrs/weak/test.cpp > CMakeFiles/test_weak.dir/weak/test.cpp.i

tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_weak.dir/weak/test.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/smart-ptrs/weak/test.cpp -o CMakeFiles/test_weak.dir/weak/test.cpp.s

tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test_shared.cpp.o: tasks/smart-ptrs/CMakeFiles/test_weak.dir/flags.make
tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test_shared.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/smart-ptrs/weak/test_shared.cpp
tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test_shared.cpp.o: tasks/smart-ptrs/CMakeFiles/test_weak.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test_shared.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test_shared.cpp.o -MF CMakeFiles/test_weak.dir/weak/test_shared.cpp.o.d -o CMakeFiles/test_weak.dir/weak/test_shared.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/smart-ptrs/weak/test_shared.cpp

tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test_shared.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_weak.dir/weak/test_shared.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/smart-ptrs/weak/test_shared.cpp > CMakeFiles/test_weak.dir/weak/test_shared.cpp.i

tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test_shared.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_weak.dir/weak/test_shared.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/smart-ptrs/weak/test_shared.cpp -o CMakeFiles/test_weak.dir/weak/test_shared.cpp.s

# Object files for target test_weak
test_weak_OBJECTS = \
"CMakeFiles/test_weak.dir/weak/test.cpp.o" \
"CMakeFiles/test_weak.dir/weak/test_shared.cpp.o"

# External object files for target test_weak
test_weak_EXTERNAL_OBJECTS =

test_weak: tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test.cpp.o
test_weak: tasks/smart-ptrs/CMakeFiles/test_weak.dir/weak/test_shared.cpp.o
test_weak: tasks/smart-ptrs/CMakeFiles/test_weak.dir/build.make
test_weak: libcontrib_catch_main.a
test_weak: allocations_checker/liballocations_checker.a
test_weak: tasks/smart-ptrs/CMakeFiles/test_weak.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../test_weak"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_weak.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/smart-ptrs/CMakeFiles/test_weak.dir/build: test_weak
.PHONY : tasks/smart-ptrs/CMakeFiles/test_weak.dir/build

tasks/smart-ptrs/CMakeFiles/test_weak.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs && $(CMAKE_COMMAND) -P CMakeFiles/test_weak.dir/cmake_clean.cmake
.PHONY : tasks/smart-ptrs/CMakeFiles/test_weak.dir/clean

tasks/smart-ptrs/CMakeFiles/test_weak.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/smart-ptrs /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/smart-ptrs/CMakeFiles/test_weak.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/smart-ptrs/CMakeFiles/test_weak.dir/depend

