# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /home/mary/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4323.58/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/mary/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4323.58/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Solver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Solver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Solver.dir/flags.make

CMakeFiles/Solver.dir/src/Polynomial.cpp.o: CMakeFiles/Solver.dir/flags.make
CMakeFiles/Solver.dir/src/Polynomial.cpp.o: ../src/Polynomial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Solver.dir/src/Polynomial.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Solver.dir/src/Polynomial.cpp.o -c /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/src/Polynomial.cpp

CMakeFiles/Solver.dir/src/Polynomial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Solver.dir/src/Polynomial.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/src/Polynomial.cpp > CMakeFiles/Solver.dir/src/Polynomial.cpp.i

CMakeFiles/Solver.dir/src/Polynomial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Solver.dir/src/Polynomial.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/src/Polynomial.cpp -o CMakeFiles/Solver.dir/src/Polynomial.cpp.s

CMakeFiles/Solver.dir/test/Test.cpp.o: CMakeFiles/Solver.dir/flags.make
CMakeFiles/Solver.dir/test/Test.cpp.o: ../test/Test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Solver.dir/test/Test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Solver.dir/test/Test.cpp.o -c /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/test/Test.cpp

CMakeFiles/Solver.dir/test/Test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Solver.dir/test/Test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/test/Test.cpp > CMakeFiles/Solver.dir/test/Test.cpp.i

CMakeFiles/Solver.dir/test/Test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Solver.dir/test/Test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/test/Test.cpp -o CMakeFiles/Solver.dir/test/Test.cpp.s

# Object files for target Solver
Solver_OBJECTS = \
"CMakeFiles/Solver.dir/src/Polynomial.cpp.o" \
"CMakeFiles/Solver.dir/test/Test.cpp.o"

# External object files for target Solver
Solver_EXTERNAL_OBJECTS =

Solver: CMakeFiles/Solver.dir/src/Polynomial.cpp.o
Solver: CMakeFiles/Solver.dir/test/Test.cpp.o
Solver: CMakeFiles/Solver.dir/build.make
Solver: lib/google-test/googlemock/gtest/libgtestd.a
Solver: lib/google-test/googlemock/gtest/libgtest_maind.a
Solver: lib/google-test/googlemock/libgmockd.a
Solver: lib/google-test/googlemock/libgmock_maind.a
Solver: lib/google-test/googlemock/libgmockd.a
Solver: lib/google-test/googlemock/gtest/libgtestd.a
Solver: CMakeFiles/Solver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Solver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Solver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Solver.dir/build: Solver

.PHONY : CMakeFiles/Solver.dir/build

CMakeFiles/Solver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Solver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Solver.dir/clean

CMakeFiles/Solver.dir/depend:
	cd /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/cmake-build-debug /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/cmake-build-debug /media/mary/Data/git/cpp-hw-fall-2018/QuadraticSolver/cmake-build-debug/CMakeFiles/Solver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Solver.dir/depend

