# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /star/u/truhlar/star-upcDstRP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /star/u/truhlar/star-upcDstRP/build

# Include any dependencies generated for this target.
include CMakeFiles/AnalysisOld.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/AnalysisOld.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AnalysisOld.dir/flags.make

CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o: CMakeFiles/AnalysisOld.dir/flags.make
CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o: ../src/AnalysisOld.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /star/u/truhlar/star-upcDstRP/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o"
	/usr/bin/g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o -c /star/u/truhlar/star-upcDstRP/src/AnalysisOld.cxx

CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.i"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /star/u/truhlar/star-upcDstRP/src/AnalysisOld.cxx > CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.i

CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.s"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /star/u/truhlar/star-upcDstRP/src/AnalysisOld.cxx -o CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.s

CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o.requires:
.PHONY : CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o.requires

CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o.provides: CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o.requires
	$(MAKE) -f CMakeFiles/AnalysisOld.dir/build.make CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o.provides.build
.PHONY : CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o.provides

CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o.provides.build: CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o

# Object files for target AnalysisOld
AnalysisOld_OBJECTS = \
"CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o"

# External object files for target AnalysisOld
AnalysisOld_EXTERNAL_OBJECTS =

AnalysisOld: CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o
AnalysisOld: CMakeFiles/AnalysisOld.dir/build.make
AnalysisOld: libstar-upc.so
AnalysisOld: CMakeFiles/AnalysisOld.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable AnalysisOld"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AnalysisOld.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AnalysisOld.dir/build: AnalysisOld
.PHONY : CMakeFiles/AnalysisOld.dir/build

CMakeFiles/AnalysisOld.dir/requires: CMakeFiles/AnalysisOld.dir/src/AnalysisOld.cxx.o.requires
.PHONY : CMakeFiles/AnalysisOld.dir/requires

CMakeFiles/AnalysisOld.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AnalysisOld.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AnalysisOld.dir/clean

CMakeFiles/AnalysisOld.dir/depend:
	cd /star/u/truhlar/star-upcDstRP/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /star/u/truhlar/star-upcDstRP /star/u/truhlar/star-upcDstRP /star/u/truhlar/star-upcDstRP/build /star/u/truhlar/star-upcDstRP/build /star/u/truhlar/star-upcDstRP/build/CMakeFiles/AnalysisOld.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/AnalysisOld.dir/depend

