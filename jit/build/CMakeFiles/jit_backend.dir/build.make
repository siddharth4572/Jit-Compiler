# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/Dhananjay/Desktop/guijit.2./jit

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/Dhananjay/Desktop/guijit.2./jit/build

# Include any dependencies generated for this target.
include CMakeFiles/jit_backend.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/jit_backend.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/jit_backend.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/jit_backend.dir/flags.make

parser.cpp: /home/Dhananjay/Desktop/guijit.2./jit/src/parser.y
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/Dhananjay/Desktop/guijit.2./jit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "[BISON][Parser] Building parser with bison 3.8.2"
	cd /home/Dhananjay/Desktop/guijit.2./jit && /usr/bin/bison --defines=/home/Dhananjay/Desktop/guijit.2./jit/build/parser.hpp -o /home/Dhananjay/Desktop/guijit.2./jit/build/parser.cpp src/parser.y

parser.hpp: parser.cpp
	@$(CMAKE_COMMAND) -E touch_nocreate parser.hpp

lexer.cpp: /home/Dhananjay/Desktop/guijit.2./jit/src/lexer.l
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/Dhananjay/Desktop/guijit.2./jit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "[FLEX][Lexer] Building scanner with flex 2.6.4"
	cd /home/Dhananjay/Desktop/guijit.2./jit && /usr/bin/flex -o/home/Dhananjay/Desktop/guijit.2./jit/build/lexer.cpp src/lexer.l

CMakeFiles/jit_backend.dir/src/codegen.cpp.o: CMakeFiles/jit_backend.dir/flags.make
CMakeFiles/jit_backend.dir/src/codegen.cpp.o: /home/Dhananjay/Desktop/guijit.2./jit/src/codegen.cpp
CMakeFiles/jit_backend.dir/src/codegen.cpp.o: CMakeFiles/jit_backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/Dhananjay/Desktop/guijit.2./jit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/jit_backend.dir/src/codegen.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jit_backend.dir/src/codegen.cpp.o -MF CMakeFiles/jit_backend.dir/src/codegen.cpp.o.d -o CMakeFiles/jit_backend.dir/src/codegen.cpp.o -c /home/Dhananjay/Desktop/guijit.2./jit/src/codegen.cpp

CMakeFiles/jit_backend.dir/src/codegen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jit_backend.dir/src/codegen.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Dhananjay/Desktop/guijit.2./jit/src/codegen.cpp > CMakeFiles/jit_backend.dir/src/codegen.cpp.i

CMakeFiles/jit_backend.dir/src/codegen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jit_backend.dir/src/codegen.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Dhananjay/Desktop/guijit.2./jit/src/codegen.cpp -o CMakeFiles/jit_backend.dir/src/codegen.cpp.s

CMakeFiles/jit_backend.dir/src/helper.cpp.o: CMakeFiles/jit_backend.dir/flags.make
CMakeFiles/jit_backend.dir/src/helper.cpp.o: /home/Dhananjay/Desktop/guijit.2./jit/src/helper.cpp
CMakeFiles/jit_backend.dir/src/helper.cpp.o: CMakeFiles/jit_backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/Dhananjay/Desktop/guijit.2./jit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/jit_backend.dir/src/helper.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jit_backend.dir/src/helper.cpp.o -MF CMakeFiles/jit_backend.dir/src/helper.cpp.o.d -o CMakeFiles/jit_backend.dir/src/helper.cpp.o -c /home/Dhananjay/Desktop/guijit.2./jit/src/helper.cpp

CMakeFiles/jit_backend.dir/src/helper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jit_backend.dir/src/helper.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Dhananjay/Desktop/guijit.2./jit/src/helper.cpp > CMakeFiles/jit_backend.dir/src/helper.cpp.i

CMakeFiles/jit_backend.dir/src/helper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jit_backend.dir/src/helper.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Dhananjay/Desktop/guijit.2./jit/src/helper.cpp -o CMakeFiles/jit_backend.dir/src/helper.cpp.s

CMakeFiles/jit_backend.dir/src/jit_interface.cpp.o: CMakeFiles/jit_backend.dir/flags.make
CMakeFiles/jit_backend.dir/src/jit_interface.cpp.o: /home/Dhananjay/Desktop/guijit.2./jit/src/jit_interface.cpp
CMakeFiles/jit_backend.dir/src/jit_interface.cpp.o: CMakeFiles/jit_backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/Dhananjay/Desktop/guijit.2./jit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/jit_backend.dir/src/jit_interface.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jit_backend.dir/src/jit_interface.cpp.o -MF CMakeFiles/jit_backend.dir/src/jit_interface.cpp.o.d -o CMakeFiles/jit_backend.dir/src/jit_interface.cpp.o -c /home/Dhananjay/Desktop/guijit.2./jit/src/jit_interface.cpp

CMakeFiles/jit_backend.dir/src/jit_interface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jit_backend.dir/src/jit_interface.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Dhananjay/Desktop/guijit.2./jit/src/jit_interface.cpp > CMakeFiles/jit_backend.dir/src/jit_interface.cpp.i

CMakeFiles/jit_backend.dir/src/jit_interface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jit_backend.dir/src/jit_interface.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Dhananjay/Desktop/guijit.2./jit/src/jit_interface.cpp -o CMakeFiles/jit_backend.dir/src/jit_interface.cpp.s

CMakeFiles/jit_backend.dir/parser.cpp.o: CMakeFiles/jit_backend.dir/flags.make
CMakeFiles/jit_backend.dir/parser.cpp.o: parser.cpp
CMakeFiles/jit_backend.dir/parser.cpp.o: CMakeFiles/jit_backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/Dhananjay/Desktop/guijit.2./jit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/jit_backend.dir/parser.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jit_backend.dir/parser.cpp.o -MF CMakeFiles/jit_backend.dir/parser.cpp.o.d -o CMakeFiles/jit_backend.dir/parser.cpp.o -c /home/Dhananjay/Desktop/guijit.2./jit/build/parser.cpp

CMakeFiles/jit_backend.dir/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jit_backend.dir/parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Dhananjay/Desktop/guijit.2./jit/build/parser.cpp > CMakeFiles/jit_backend.dir/parser.cpp.i

CMakeFiles/jit_backend.dir/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jit_backend.dir/parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Dhananjay/Desktop/guijit.2./jit/build/parser.cpp -o CMakeFiles/jit_backend.dir/parser.cpp.s

CMakeFiles/jit_backend.dir/lexer.cpp.o: CMakeFiles/jit_backend.dir/flags.make
CMakeFiles/jit_backend.dir/lexer.cpp.o: lexer.cpp
CMakeFiles/jit_backend.dir/lexer.cpp.o: parser.hpp
CMakeFiles/jit_backend.dir/lexer.cpp.o: CMakeFiles/jit_backend.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/Dhananjay/Desktop/guijit.2./jit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/jit_backend.dir/lexer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/jit_backend.dir/lexer.cpp.o -MF CMakeFiles/jit_backend.dir/lexer.cpp.o.d -o CMakeFiles/jit_backend.dir/lexer.cpp.o -c /home/Dhananjay/Desktop/guijit.2./jit/build/lexer.cpp

CMakeFiles/jit_backend.dir/lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/jit_backend.dir/lexer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Dhananjay/Desktop/guijit.2./jit/build/lexer.cpp > CMakeFiles/jit_backend.dir/lexer.cpp.i

CMakeFiles/jit_backend.dir/lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/jit_backend.dir/lexer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Dhananjay/Desktop/guijit.2./jit/build/lexer.cpp -o CMakeFiles/jit_backend.dir/lexer.cpp.s

# Object files for target jit_backend
jit_backend_OBJECTS = \
"CMakeFiles/jit_backend.dir/src/codegen.cpp.o" \
"CMakeFiles/jit_backend.dir/src/helper.cpp.o" \
"CMakeFiles/jit_backend.dir/src/jit_interface.cpp.o" \
"CMakeFiles/jit_backend.dir/parser.cpp.o" \
"CMakeFiles/jit_backend.dir/lexer.cpp.o"

# External object files for target jit_backend
jit_backend_EXTERNAL_OBJECTS =

libjit_backend.a: CMakeFiles/jit_backend.dir/src/codegen.cpp.o
libjit_backend.a: CMakeFiles/jit_backend.dir/src/helper.cpp.o
libjit_backend.a: CMakeFiles/jit_backend.dir/src/jit_interface.cpp.o
libjit_backend.a: CMakeFiles/jit_backend.dir/parser.cpp.o
libjit_backend.a: CMakeFiles/jit_backend.dir/lexer.cpp.o
libjit_backend.a: CMakeFiles/jit_backend.dir/build.make
libjit_backend.a: CMakeFiles/jit_backend.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/Dhananjay/Desktop/guijit.2./jit/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library libjit_backend.a"
	$(CMAKE_COMMAND) -P CMakeFiles/jit_backend.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jit_backend.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/jit_backend.dir/build: libjit_backend.a
.PHONY : CMakeFiles/jit_backend.dir/build

CMakeFiles/jit_backend.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/jit_backend.dir/cmake_clean.cmake
.PHONY : CMakeFiles/jit_backend.dir/clean

CMakeFiles/jit_backend.dir/depend: lexer.cpp
CMakeFiles/jit_backend.dir/depend: parser.cpp
CMakeFiles/jit_backend.dir/depend: parser.hpp
	cd /home/Dhananjay/Desktop/guijit.2./jit/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/Dhananjay/Desktop/guijit.2./jit /home/Dhananjay/Desktop/guijit.2./jit /home/Dhananjay/Desktop/guijit.2./jit/build /home/Dhananjay/Desktop/guijit.2./jit/build /home/Dhananjay/Desktop/guijit.2./jit/build/CMakeFiles/jit_backend.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/jit_backend.dir/depend

