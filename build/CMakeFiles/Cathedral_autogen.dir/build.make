# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/nolanlarison/cathedral

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/nolanlarison/cathedral/build

# Utility rule file for Cathedral_autogen.

# Include any custom commands dependencies for this target.
include CMakeFiles/Cathedral_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Cathedral_autogen.dir/progress.make

CMakeFiles/Cathedral_autogen: Cathedral_autogen/timestamp

Cathedral_autogen/timestamp: /opt/anaconda3/bin/moc
Cathedral_autogen/timestamp: /opt/anaconda3/bin/uic
Cathedral_autogen/timestamp: CMakeFiles/Cathedral_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/nolanlarison/cathedral/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target Cathedral"
	/opt/homebrew/bin/cmake -E cmake_autogen /Users/nolanlarison/cathedral/build/CMakeFiles/Cathedral_autogen.dir/AutogenInfo.json ""
	/opt/homebrew/bin/cmake -E touch /Users/nolanlarison/cathedral/build/Cathedral_autogen/timestamp

CMakeFiles/Cathedral_autogen.dir/codegen:
.PHONY : CMakeFiles/Cathedral_autogen.dir/codegen

Cathedral_autogen: CMakeFiles/Cathedral_autogen
Cathedral_autogen: Cathedral_autogen/timestamp
Cathedral_autogen: CMakeFiles/Cathedral_autogen.dir/build.make
.PHONY : Cathedral_autogen

# Rule to build all files generated by this target.
CMakeFiles/Cathedral_autogen.dir/build: Cathedral_autogen
.PHONY : CMakeFiles/Cathedral_autogen.dir/build

CMakeFiles/Cathedral_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Cathedral_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Cathedral_autogen.dir/clean

CMakeFiles/Cathedral_autogen.dir/depend:
	cd /Users/nolanlarison/cathedral/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/nolanlarison/cathedral /Users/nolanlarison/cathedral /Users/nolanlarison/cathedral/build /Users/nolanlarison/cathedral/build /Users/nolanlarison/cathedral/build/CMakeFiles/Cathedral_autogen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Cathedral_autogen.dir/depend

