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
include lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/depend.make

# Include the progress variables for this target.
include lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/progress.make

# Include the compile flags for this target's objects.
include lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/flags.make

lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o: lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/flags.make
lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o: ../lib/thrill/tests/data/multiplexer_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eagle/dev/thrill_tut/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/tests && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o -c /home/eagle/dev/thrill_tut/lib/thrill/tests/data/multiplexer_test.cpp

lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.i"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eagle/dev/thrill_tut/lib/thrill/tests/data/multiplexer_test.cpp > CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.i

lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.s"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eagle/dev/thrill_tut/lib/thrill/tests/data/multiplexer_test.cpp -o CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.s

lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o.requires:

.PHONY : lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o.requires

lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o.provides: lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o.requires
	$(MAKE) -f lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/build.make lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o.provides.build
.PHONY : lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o.provides

lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o.provides.build: lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o


# Object files for target data_multiplexer_test
data_multiplexer_test_OBJECTS = \
"CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o"

# External object files for target data_multiplexer_test
data_multiplexer_test_EXTERNAL_OBJECTS =

lib/thrill/tests/data_multiplexer_test: lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o
lib/thrill/tests/data_multiplexer_test: lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/build.make
lib/thrill/tests/data_multiplexer_test: lib/thrill/thrill/libthrill.a
lib/thrill/tests/data_multiplexer_test: lib/thrill/extlib/googletest/googletest/libgtest.a
lib/thrill/tests/data_multiplexer_test: lib/thrill/extlib/googletest/googletest/libgtest_main.a
lib/thrill/tests/data_multiplexer_test: /usr/lib/x86_64-linux-gnu/libtbb.so
lib/thrill/tests/data_multiplexer_test: /usr/lib/x86_64-linux-gnu/libtbbmalloc.so
lib/thrill/tests/data_multiplexer_test: /usr/lib/x86_64-linux-gnu/libjemalloc.so
lib/thrill/tests/data_multiplexer_test: /usr/lib/x86_64-linux-gnu/libz.so
lib/thrill/tests/data_multiplexer_test: /usr/lib/x86_64-linux-gnu/libbz2.so
lib/thrill/tests/data_multiplexer_test: lib/thrill/extlib/libs3/libs3.a
lib/thrill/tests/data_multiplexer_test: /usr/lib/x86_64-linux-gnu/libxml2.so
lib/thrill/tests/data_multiplexer_test: /usr/lib/x86_64-linux-gnu/libcurl.so
lib/thrill/tests/data_multiplexer_test: /usr/lib/x86_64-linux-gnu/libssl.so
lib/thrill/tests/data_multiplexer_test: /usr/lib/x86_64-linux-gnu/libcrypto.so
lib/thrill/tests/data_multiplexer_test: lib/thrill/extlib/googletest/googletest/libgtest.a
lib/thrill/tests/data_multiplexer_test: lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eagle/dev/thrill_tut/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable data_multiplexer_test"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/data_multiplexer_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/build: lib/thrill/tests/data_multiplexer_test

.PHONY : lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/build

lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/requires: lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/data/multiplexer_test.cpp.o.requires

.PHONY : lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/requires

lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/clean:
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/tests && $(CMAKE_COMMAND) -P CMakeFiles/data_multiplexer_test.dir/cmake_clean.cmake
.PHONY : lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/clean

lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/depend:
	cd /home/eagle/dev/thrill_tut/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eagle/dev/thrill_tut /home/eagle/dev/thrill_tut/lib/thrill/tests /home/eagle/dev/thrill_tut/build /home/eagle/dev/thrill_tut/build/lib/thrill/tests /home/eagle/dev/thrill_tut/build/lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/thrill/tests/CMakeFiles/data_multiplexer_test.dir/depend

