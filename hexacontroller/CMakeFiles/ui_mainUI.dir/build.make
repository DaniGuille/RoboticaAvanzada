# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/salabeta/RoboticaAvanzada/hexacontroller

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/salabeta/RoboticaAvanzada/hexacontroller

# Utility rule file for ui_mainUI.

# Include the progress variables for this target.
include CMakeFiles/ui_mainUI.dir/progress.make

CMakeFiles/ui_mainUI: src/ui_mainUI.py

src/ui_mainUI.py: src/mainUI.ui
	$(CMAKE_COMMAND) -E cmake_progress_report /home/salabeta/RoboticaAvanzada/hexacontroller/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating src/ui_mainUI.py from src/mainUI.ui"
	pyside-uic src/mainUI.ui -o src/ui_mainUI.py

ui_mainUI: CMakeFiles/ui_mainUI
ui_mainUI: src/ui_mainUI.py
ui_mainUI: CMakeFiles/ui_mainUI.dir/build.make
.PHONY : ui_mainUI

# Rule to build all files generated by this target.
CMakeFiles/ui_mainUI.dir/build: ui_mainUI
.PHONY : CMakeFiles/ui_mainUI.dir/build

CMakeFiles/ui_mainUI.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ui_mainUI.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ui_mainUI.dir/clean

CMakeFiles/ui_mainUI.dir/depend:
	cd /home/salabeta/RoboticaAvanzada/hexacontroller && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/salabeta/RoboticaAvanzada/hexacontroller /home/salabeta/RoboticaAvanzada/hexacontroller /home/salabeta/RoboticaAvanzada/hexacontroller /home/salabeta/RoboticaAvanzada/hexacontroller /home/salabeta/RoboticaAvanzada/hexacontroller/CMakeFiles/ui_mainUI.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ui_mainUI.dir/depend

