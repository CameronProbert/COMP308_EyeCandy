# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

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
CMAKE_COMMAND = /usr/pkg/bin/cmake

# The command to remove a file.
RM = /usr/pkg/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /u/students/burlinfran/git/COMP308_EyeCandy/work

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /u/students/burlinfran/git/COMP308_EyeCandy/build

# Include any dependencies generated for this target.
include src/CMakeFiles/FP.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/FP.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/FP.dir/flags.make

src/CMakeFiles/FP.dir/main.cpp.o: src/CMakeFiles/FP.dir/flags.make
src/CMakeFiles/FP.dir/main.cpp.o: /u/students/burlinfran/git/COMP308_EyeCandy/work/src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/u/students/burlinfran/git/COMP308_EyeCandy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/FP.dir/main.cpp.o"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && /bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FP.dir/main.cpp.o -c /u/students/burlinfran/git/COMP308_EyeCandy/work/src/main.cpp

src/CMakeFiles/FP.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FP.dir/main.cpp.i"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /u/students/burlinfran/git/COMP308_EyeCandy/work/src/main.cpp > CMakeFiles/FP.dir/main.cpp.i

src/CMakeFiles/FP.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FP.dir/main.cpp.s"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /u/students/burlinfran/git/COMP308_EyeCandy/work/src/main.cpp -o CMakeFiles/FP.dir/main.cpp.s

src/CMakeFiles/FP.dir/main.cpp.o.requires:

.PHONY : src/CMakeFiles/FP.dir/main.cpp.o.requires

src/CMakeFiles/FP.dir/main.cpp.o.provides: src/CMakeFiles/FP.dir/main.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/FP.dir/build.make src/CMakeFiles/FP.dir/main.cpp.o.provides.build
.PHONY : src/CMakeFiles/FP.dir/main.cpp.o.provides

src/CMakeFiles/FP.dir/main.cpp.o.provides.build: src/CMakeFiles/FP.dir/main.cpp.o


src/CMakeFiles/FP.dir/geometry.cpp.o: src/CMakeFiles/FP.dir/flags.make
src/CMakeFiles/FP.dir/geometry.cpp.o: /u/students/burlinfran/git/COMP308_EyeCandy/work/src/geometry.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/u/students/burlinfran/git/COMP308_EyeCandy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/FP.dir/geometry.cpp.o"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && /bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FP.dir/geometry.cpp.o -c /u/students/burlinfran/git/COMP308_EyeCandy/work/src/geometry.cpp

src/CMakeFiles/FP.dir/geometry.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FP.dir/geometry.cpp.i"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /u/students/burlinfran/git/COMP308_EyeCandy/work/src/geometry.cpp > CMakeFiles/FP.dir/geometry.cpp.i

src/CMakeFiles/FP.dir/geometry.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FP.dir/geometry.cpp.s"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /u/students/burlinfran/git/COMP308_EyeCandy/work/src/geometry.cpp -o CMakeFiles/FP.dir/geometry.cpp.s

src/CMakeFiles/FP.dir/geometry.cpp.o.requires:

.PHONY : src/CMakeFiles/FP.dir/geometry.cpp.o.requires

src/CMakeFiles/FP.dir/geometry.cpp.o.provides: src/CMakeFiles/FP.dir/geometry.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/FP.dir/build.make src/CMakeFiles/FP.dir/geometry.cpp.o.provides.build
.PHONY : src/CMakeFiles/FP.dir/geometry.cpp.o.provides

src/CMakeFiles/FP.dir/geometry.cpp.o.provides.build: src/CMakeFiles/FP.dir/geometry.cpp.o


src/CMakeFiles/FP.dir/scene.cpp.o: src/CMakeFiles/FP.dir/flags.make
src/CMakeFiles/FP.dir/scene.cpp.o: /u/students/burlinfran/git/COMP308_EyeCandy/work/src/scene.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/u/students/burlinfran/git/COMP308_EyeCandy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/FP.dir/scene.cpp.o"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && /bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/FP.dir/scene.cpp.o -c /u/students/burlinfran/git/COMP308_EyeCandy/work/src/scene.cpp

src/CMakeFiles/FP.dir/scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FP.dir/scene.cpp.i"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /u/students/burlinfran/git/COMP308_EyeCandy/work/src/scene.cpp > CMakeFiles/FP.dir/scene.cpp.i

src/CMakeFiles/FP.dir/scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FP.dir/scene.cpp.s"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /u/students/burlinfran/git/COMP308_EyeCandy/work/src/scene.cpp -o CMakeFiles/FP.dir/scene.cpp.s

src/CMakeFiles/FP.dir/scene.cpp.o.requires:

.PHONY : src/CMakeFiles/FP.dir/scene.cpp.o.requires

src/CMakeFiles/FP.dir/scene.cpp.o.provides: src/CMakeFiles/FP.dir/scene.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/FP.dir/build.make src/CMakeFiles/FP.dir/scene.cpp.o.provides.build
.PHONY : src/CMakeFiles/FP.dir/scene.cpp.o.provides

src/CMakeFiles/FP.dir/scene.cpp.o.provides.build: src/CMakeFiles/FP.dir/scene.cpp.o


# Object files for target FP
FP_OBJECTS = \
"CMakeFiles/FP.dir/main.cpp.o" \
"CMakeFiles/FP.dir/geometry.cpp.o" \
"CMakeFiles/FP.dir/scene.cpp.o"

# External object files for target FP
FP_EXTERNAL_OBJECTS =

bin/FP: src/CMakeFiles/FP.dir/main.cpp.o
bin/FP: src/CMakeFiles/FP.dir/geometry.cpp.o
bin/FP: src/CMakeFiles/FP.dir/scene.cpp.o
bin/FP: src/CMakeFiles/FP.dir/build.make
bin/FP: /lib64/libglut.so
bin/FP: bin/libglew.a
bin/FP: bin/libstb.a
bin/FP: /lib64/libXmu.so
bin/FP: /lib64/libXi.so
bin/FP: /lib64/libGLU.so
bin/FP: /lib64/libGL.so
bin/FP: /lib64/libGLU.so
bin/FP: /lib64/libGL.so
bin/FP: src/CMakeFiles/FP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/u/students/burlinfran/git/COMP308_EyeCandy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../bin/FP"
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/FP.dir/build: bin/FP

.PHONY : src/CMakeFiles/FP.dir/build

src/CMakeFiles/FP.dir/requires: src/CMakeFiles/FP.dir/main.cpp.o.requires
src/CMakeFiles/FP.dir/requires: src/CMakeFiles/FP.dir/geometry.cpp.o.requires
src/CMakeFiles/FP.dir/requires: src/CMakeFiles/FP.dir/scene.cpp.o.requires

.PHONY : src/CMakeFiles/FP.dir/requires

src/CMakeFiles/FP.dir/clean:
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build/src && $(CMAKE_COMMAND) -P CMakeFiles/FP.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/FP.dir/clean

src/CMakeFiles/FP.dir/depend:
	cd /u/students/burlinfran/git/COMP308_EyeCandy/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /u/students/burlinfran/git/COMP308_EyeCandy/work /u/students/burlinfran/git/COMP308_EyeCandy/work/src /u/students/burlinfran/git/COMP308_EyeCandy/build /u/students/burlinfran/git/COMP308_EyeCandy/build/src /u/students/burlinfran/git/COMP308_EyeCandy/build/src/CMakeFiles/FP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/FP.dir/depend
