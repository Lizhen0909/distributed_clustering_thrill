# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eagle/dev/thrill_tut

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eagle/dev/thrill_tut/build

# Include any dependencies generated for this target.
include lib/thrill/examples/sleep/CMakeFiles/sleep.dir/depend.make

# Include the progress variables for this target.
include lib/thrill/examples/sleep/CMakeFiles/sleep.dir/progress.make

# Include the compile flags for this target's objects.
include lib/thrill/examples/sleep/CMakeFiles/sleep.dir/flags.make

lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o: lib/thrill/examples/sleep/CMakeFiles/sleep.dir/flags.make
lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o: ../lib/thrill/examples/sleep/sleep.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eagle/dev/thrill_tut/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/examples/sleep && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sleep.dir/sleep.cpp.o -c /home/eagle/dev/thrill_tut/lib/thrill/examples/sleep/sleep.cpp

lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sleep.dir/sleep.cpp.i"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/examples/sleep && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eagle/dev/thrill_tut/lib/thrill/examples/sleep/sleep.cpp > CMakeFiles/sleep.dir/sleep.cpp.i

lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sleep.dir/sleep.cpp.s"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/examples/sleep && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eagle/dev/thrill_tut/lib/thrill/examples/sleep/sleep.cpp -o CMakeFiles/sleep.dir/sleep.cpp.s

lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o.requires:

.PHONY : lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o.requires

lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o.provides: lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o.requires
	$(MAKE) -f lib/thrill/examples/sleep/CMakeFiles/sleep.dir/build.make lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o.provides.build
.PHONY : lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o.provides

lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o.provides.build: lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o


# Object files for target sleep
sleep_OBJECTS = \
"CMakeFiles/sleep.dir/sleep.cpp.o"

# External object files for target sleep
sleep_EXTERNAL_OBJECTS =

lib/thrill/examples/sleep/sleep: lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o
lib/thrill/examples/sleep/sleep: lib/thrill/examples/sleep/CMakeFiles/sleep.dir/build.make
lib/thrill/examples/sleep/sleep: lib/thrill/thrill/libthrill.a
lib/thrill/examples/sleep/sleep: /usr/lib/x86_64-linux-gnu/libtbb.so
lib/thrill/examples/sleep/sleep: /usr/lib/x86_64-linux-gnu/libtbbmalloc.so
lib/thrill/examples/sleep/sleep: /usr/lib/x86_64-linux-gnu/libjemalloc.so
lib/thrill/examples/sleep/sleep: /usr/lib/x86_64-linux-gnu/libz.so
lib/thrill/examples/sleep/sleep: /usr/lib/x86_64-linux-gnu/libbz2.so
lib/thrill/examples/sleep/sleep: lib/thrill/extlib/libs3/libs3.a
lib/thrill/examples/sleep/sleep: /usr/lib/x86_64-linux-gnu/libxml2.so
lib/thrill/examples/sleep/sleep: /usr/lib/x86_64-linux-gnu/libcurl.so
lib/thrill/examples/sleep/sleep: /usr/lib/x86_64-linux-gnu/libssl.so
lib/thrill/examples/sleep/sleep: /usr/lib/x86_64-linux-gnu/libcrypto.so
lib/thrill/examples/sleep/sleep: lib/thrill/examples/sleep/CMakeFiles/sleep.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eagle/dev/thrill_tut/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sleep"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/examples/sleep && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sleep.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/thrill/examples/sleep/CMakeFiles/sleep.dir/build: lib/thrill/examples/sleep/sleep

.PHONY : lib/thrill/examples/sleep/CMakeFiles/sleep.dir/build

lib/thrill/examples/sleep/CMakeFiles/sleep.dir/requires: lib/thrill/examples/sleep/CMakeFiles/sleep.dir/sleep.cpp.o.requires

.PHONY : lib/thrill/examples/sleep/CMakeFiles/sleep.dir/requires

lib/thrill/examples/sleep/CMakeFiles/sleep.dir/clean:
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/examples/sleep && $(CMAKE_COMMAND) -P CMakeFiles/sleep.dir/cmake_clean.cmake
.PHONY : lib/thrill/examples/sleep/CMakeFiles/sleep.dir/clean

lib/thrill/examples/sleep/CMakeFiles/sleep.dir/depend:
	cd /home/eagle/dev/thrill_tut/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eagle/dev/thrill_tut /home/eagle/dev/thrill_tut/lib/thrill/examples/sleep /home/eagle/dev/thrill_tut/build /home/eagle/dev/thrill_tut/build/lib/thrill/examples/sleep /home/eagle/dev/thrill_tut/build/lib/thrill/examples/sleep/CMakeFiles/sleep.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/thrill/examples/sleep/CMakeFiles/sleep.dir/depend

