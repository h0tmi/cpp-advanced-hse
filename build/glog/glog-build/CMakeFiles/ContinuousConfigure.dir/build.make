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

# Utility rule file for ContinuousConfigure.

# Include any custom commands dependencies for this target.
include glog/glog-build/CMakeFiles/ContinuousConfigure.dir/compiler_depend.make

# Include the progress variables for this target.
include glog/glog-build/CMakeFiles/ContinuousConfigure.dir/progress.make

glog/glog-build/CMakeFiles/ContinuousConfigure:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/glog/glog-build && /opt/homebrew/Cellar/cmake/3.26.4/bin/ctest -D ContinuousConfigure

ContinuousConfigure: glog/glog-build/CMakeFiles/ContinuousConfigure
ContinuousConfigure: glog/glog-build/CMakeFiles/ContinuousConfigure.dir/build.make
.PHONY : ContinuousConfigure

# Rule to build all files generated by this target.
glog/glog-build/CMakeFiles/ContinuousConfigure.dir/build: ContinuousConfigure
.PHONY : glog/glog-build/CMakeFiles/ContinuousConfigure.dir/build

glog/glog-build/CMakeFiles/ContinuousConfigure.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/glog/glog-build && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousConfigure.dir/cmake_clean.cmake
.PHONY : glog/glog-build/CMakeFiles/ContinuousConfigure.dir/clean

glog/glog-build/CMakeFiles/ContinuousConfigure.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/glog/glog-src /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/glog/glog-build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/glog/glog-build/CMakeFiles/ContinuousConfigure.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glog/glog-build/CMakeFiles/ContinuousConfigure.dir/depend

