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
include lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/depend.make

# Include the progress variables for this target.
include lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/progress.make

# Include the compile flags for this target's objects.
include lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/flags.make

lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o: lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/flags.make
lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o: ../lib/thrill/benchmarks/data/data_benchmark.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eagle/dev/thrill_tut/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/benchmarks/data && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o -c /home/eagle/dev/thrill_tut/lib/thrill/benchmarks/data/data_benchmark.cpp

lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/data_benchmark.dir/data_benchmark.cpp.i"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/benchmarks/data && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eagle/dev/thrill_tut/lib/thrill/benchmarks/data/data_benchmark.cpp > CMakeFiles/data_benchmark.dir/data_benchmark.cpp.i

lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/data_benchmark.dir/data_benchmark.cpp.s"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/benchmarks/data && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eagle/dev/thrill_tut/lib/thrill/benchmarks/data/data_benchmark.cpp -o CMakeFiles/data_benchmark.dir/data_benchmark.cpp.s

lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o.requires:

.PHONY : lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o.requires

lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o.provides: lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o.requires
	$(MAKE) -f lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/build.make lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o.provides.build
.PHONY : lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o.provides

lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o.provides.build: lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o


# Object files for target data_benchmark
data_benchmark_OBJECTS = \
"CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o"

# External object files for target data_benchmark
data_benchmark_EXTERNAL_OBJECTS =

lib/thrill/benchmarks/data/data_benchmark: lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o
lib/thrill/benchmarks/data/data_benchmark: lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/build.make
lib/thrill/benchmarks/data/data_benchmark: lib/thrill/thrill/libthrill.a
lib/thrill/benchmarks/data/data_benchmark: /usr/lib/x86_64-linux-gnu/libtbb.so
lib/thrill/benchmarks/data/data_benchmark: /usr/lib/x86_64-linux-gnu/libtbbmalloc.so
lib/thrill/benchmarks/data/data_benchmark: /usr/lib/x86_64-linux-gnu/libjemalloc.so
lib/thrill/benchmarks/data/data_benchmark: /usr/lib/x86_64-linux-gnu/libz.so
lib/thrill/benchmarks/data/data_benchmark: /usr/lib/x86_64-linux-gnu/libbz2.so
lib/thrill/benchmarks/data/data_benchmark: lib/thrill/extlib/libs3/libs3.a
lib/thrill/benchmarks/data/data_benchmark: /usr/lib/x86_64-linux-gnu/libxml2.so
lib/thrill/benchmarks/data/data_benchmark: /usr/lib/x86_64-linux-gnu/libcurl.so
lib/thrill/benchmarks/data/data_benchmark: /usr/lib/x86_64-linux-gnu/libssl.so
lib/thrill/benchmarks/data/data_benchmark: /usr/lib/x86_64-linux-gnu/libcrypto.so
lib/thrill/benchmarks/data/data_benchmark: lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eagle/dev/thrill_tut/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable data_benchmark"
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/benchmarks/data && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/data_benchmark.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/build: lib/thrill/benchmarks/data/data_benchmark

.PHONY : lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/build

lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/requires: lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/data_benchmark.cpp.o.requires

.PHONY : lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/requires

lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/clean:
	cd /home/eagle/dev/thrill_tut/build/lib/thrill/benchmarks/data && $(CMAKE_COMMAND) -P CMakeFiles/data_benchmark.dir/cmake_clean.cmake
.PHONY : lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/clean

lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/depend:
	cd /home/eagle/dev/thrill_tut/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eagle/dev/thrill_tut /home/eagle/dev/thrill_tut/lib/thrill/benchmarks/data /home/eagle/dev/thrill_tut/build /home/eagle/dev/thrill_tut/build/lib/thrill/benchmarks/data /home/eagle/dev/thrill_tut/build/lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/thrill/benchmarks/data/CMakeFiles/data_benchmark.dir/depend

