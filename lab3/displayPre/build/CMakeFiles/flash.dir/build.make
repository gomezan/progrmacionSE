# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = C:/Users/aulasingenieria/.espressif/tools/cmake/3.24.0/bin/cmake.exe

# The command to remove a file.
RM = C:/Users/aulasingenieria/.espressif/tools/cmake/3.24.0/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre/build

# Utility rule file for flash.

# Include any custom commands dependencies for this target.
include CMakeFiles/flash.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/flash.dir/progress.make

CMakeFiles/flash:
	cd C:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esptool_py && C:/Users/aulasingenieria/.espressif/tools/cmake/3.24.0/bin/cmake.exe -D IDF_PATH=C:/Users/aulasingenieria/esp/v5.2.1/esp-idf -D "SERIAL_TOOL=C:/Users/aulasingenieria/.espressif/python_env/idf5.2_py3.11_env/Scripts/python.exe;;C:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esptool_py/esptool/esptool.py;--chip;esp32" -D "SERIAL_TOOL_ARGS=--before=default_reset;--after=hard_reset;write_flash;@flash_args" -D WORKING_DIRECTORY=C:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre/build -P C:/Users/aulasingenieria/esp/v5.2.1/esp-idf/components/esptool_py/run_serial_tool.cmake

flash: CMakeFiles/flash
flash: CMakeFiles/flash.dir/build.make
.PHONY : flash

# Rule to build all files generated by this target.
CMakeFiles/flash.dir/build: flash
.PHONY : CMakeFiles/flash.dir/build

CMakeFiles/flash.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/flash.dir/cmake_clean.cmake
.PHONY : CMakeFiles/flash.dir/clean

CMakeFiles/flash.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre C:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre C:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre/build C:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre/build C:/Users/aulasingenieria/Documents/GitHub/progrmacionSE/lab3/displayPre/build/CMakeFiles/flash.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/flash.dir/depend

