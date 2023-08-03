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
include tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/compiler_depend.make

# Include the progress variables for this target.
include tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/progress.make

# Include the compile flags for this target's objects.
include tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/flags.make

tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/fft.cpp.o: tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/flags.make
tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/fft.cpp.o: /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/fftw/fft.cpp
tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/fft.cpp.o: tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/fft.cpp.o"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/jpeg-decoder/fftw && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/fft.cpp.o -MF CMakeFiles/decoder_fftw.dir/fft.cpp.o.d -o CMakeFiles/decoder_fftw.dir/fft.cpp.o -c /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/fftw/fft.cpp

tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/fft.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder_fftw.dir/fft.cpp.i"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/jpeg-decoder/fftw && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/fftw/fft.cpp > CMakeFiles/decoder_fftw.dir/fft.cpp.i

tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/fft.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder_fftw.dir/fft.cpp.s"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/jpeg-decoder/fftw && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/fftw/fft.cpp -o CMakeFiles/decoder_fftw.dir/fft.cpp.s

# Object files for target decoder_fftw
decoder_fftw_OBJECTS = \
"CMakeFiles/decoder_fftw.dir/fft.cpp.o"

# External object files for target decoder_fftw
decoder_fftw_EXTERNAL_OBJECTS =

tasks/jpeg-decoder/fftw/libdecoder_fftw.a: tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/fft.cpp.o
tasks/jpeg-decoder/fftw/libdecoder_fftw.a: tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/build.make
tasks/jpeg-decoder/fftw/libdecoder_fftw.a: tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libdecoder_fftw.a"
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/jpeg-decoder/fftw && $(CMAKE_COMMAND) -P CMakeFiles/decoder_fftw.dir/cmake_clean_target.cmake
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/jpeg-decoder/fftw && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/decoder_fftw.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/build: tasks/jpeg-decoder/fftw/libdecoder_fftw.a
.PHONY : tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/build

tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/clean:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/jpeg-decoder/fftw && $(CMAKE_COMMAND) -P CMakeFiles/decoder_fftw.dir/cmake_clean.cmake
.PHONY : tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/clean

tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/depend:
	cd /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/tasks/jpeg-decoder/fftw /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/jpeg-decoder/fftw /Users/timofeyzhamoidin/Stuff/HSE/cpp-advanced-hse/san_build/tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tasks/jpeg-decoder/fftw/CMakeFiles/decoder_fftw.dir/depend
