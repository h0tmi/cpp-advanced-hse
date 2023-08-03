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
include tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/compiler_depend.make

# Include the progress variables for this target.
include tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/flags.make

tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/huffman.cpp.o: tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/flags.make
tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/huffman.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/huffman/huffman.cpp
tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/huffman.cpp.o: tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/huffman.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/jpeg-decoder/huffman && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/huffman.cpp.o -MF CMakeFiles/decoder_huffman.dir/huffman.cpp.o.d -o CMakeFiles/decoder_huffman.dir/huffman.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/huffman/huffman.cpp

tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/huffman.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder_huffman.dir/huffman.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/jpeg-decoder/huffman && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/huffman/huffman.cpp > CMakeFiles/decoder_huffman.dir/huffman.cpp.i

tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/huffman.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder_huffman.dir/huffman.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/jpeg-decoder/huffman && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/huffman/huffman.cpp -o CMakeFiles/decoder_huffman.dir/huffman.cpp.s

# Object files for target decoder_huffman
decoder_huffman_OBJECTS = \
"CMakeFiles/decoder_huffman.dir/huffman.cpp.o"

# External object files for target decoder_huffman
decoder_huffman_EXTERNAL_OBJECTS =

tasks/jpeg-decoder/huffman/libdecoder_huffman.a: tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/huffman.cpp.o
tasks/jpeg-decoder/huffman/libdecoder_huffman.a: tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/build.make
tasks/jpeg-decoder/huffman/libdecoder_huffman.a: tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libdecoder_huffman.a"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/jpeg-decoder/huffman && $(CMAKE_COMMAND) -P CMakeFiles/decoder_huffman.dir/cmake_clean_target.cmake
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/jpeg-decoder/huffman && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/decoder_huffman.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/build: tasks/jpeg-decoder/huffman/libdecoder_huffman.a
.PHONY : tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/build

tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/jpeg-decoder/huffman && $(CMAKE_COMMAND) -P CMakeFiles/decoder_huffman.dir/cmake_clean.cmake
.PHONY : tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/clean

tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/huffman /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/jpeg-decoder/huffman /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/build/tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/jpeg-decoder/huffman/CMakeFiles/decoder_huffman.dir/depend

