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
include tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/compiler_depend.make

# Include the progress variables for this target.
include tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/flags.make

tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/main.cpp.o: tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/flags.make
tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/main.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/move/rule-of-5/main.cpp
tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/main.cpp.o: tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/main.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/move/rule-of-5 && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/main.cpp.o -MF CMakeFiles/ctf_rule_of_5.dir/main.cpp.o.d -o CMakeFiles/ctf_rule_of_5.dir/main.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/move/rule-of-5/main.cpp

tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ctf_rule_of_5.dir/main.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/move/rule-of-5 && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/move/rule-of-5/main.cpp > CMakeFiles/ctf_rule_of_5.dir/main.cpp.i

tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ctf_rule_of_5.dir/main.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/move/rule-of-5 && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/move/rule-of-5/main.cpp -o CMakeFiles/ctf_rule_of_5.dir/main.cpp.s

# Object files for target ctf_rule_of_5
ctf_rule_of_5_OBJECTS = \
"CMakeFiles/ctf_rule_of_5.dir/main.cpp.o"

# External object files for target ctf_rule_of_5
ctf_rule_of_5_EXTERNAL_OBJECTS =

ctf_rule_of_5: tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/main.cpp.o
ctf_rule_of_5: tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/build.make
ctf_rule_of_5: tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../ctf_rule_of_5"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/move/rule-of-5 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ctf_rule_of_5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/build: ctf_rule_of_5
.PHONY : tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/build

tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/move/rule-of-5 && $(CMAKE_COMMAND) -P CMakeFiles/ctf_rule_of_5.dir/cmake_clean.cmake
.PHONY : tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/clean

tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/move/rule-of-5 /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/move/rule-of-5 /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/move/rule-of-5/CMakeFiles/ctf_rule_of_5.dir/depend
