# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\lserazin\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\183.5429.37\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\lserazin\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\183.5429.37\bin\cmake\win\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\Algo.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\Algo.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\Algo.dir\flags.make

CMakeFiles\Algo.dir\main.cpp.obj: CMakeFiles\Algo.dir\flags.make
CMakeFiles\Algo.dir\main.cpp.obj: ..\main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Algo.dir/main.cpp.obj"
	C:\PROGRA~2\MICROS~1.0\VC\bin\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoCMakeFiles\Algo.dir\main.cpp.obj /FdCMakeFiles\Algo.dir\ /FS -c C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\main.cpp
<<

CMakeFiles\Algo.dir\main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Algo.dir/main.cpp.i"
	C:\PROGRA~2\MICROS~1.0\VC\bin\cl.exe > CMakeFiles\Algo.dir\main.cpp.i @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\main.cpp
<<

CMakeFiles\Algo.dir\main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Algo.dir/main.cpp.s"
	C:\PROGRA~2\MICROS~1.0\VC\bin\cl.exe @<<
 /nologo /TP $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) /FoNUL /FAs /FaCMakeFiles\Algo.dir\main.cpp.s /c C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\main.cpp
<<

# Object files for target Algo
Algo_OBJECTS = \
"CMakeFiles\Algo.dir\main.cpp.obj"

# External object files for target Algo
Algo_EXTERNAL_OBJECTS =

Algo.exe: CMakeFiles\Algo.dir\main.cpp.obj
Algo.exe: CMakeFiles\Algo.dir\build.make
Algo.exe: CMakeFiles\Algo.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Algo.exe"
	C:\Users\lserazin\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\183.5429.37\bin\cmake\win\bin\cmake.exe -E vs_link_exe --intdir=CMakeFiles\Algo.dir --manifests  -- C:\PROGRA~2\MICROS~1.0\VC\bin\link.exe /nologo @CMakeFiles\Algo.dir\objects1.rsp @<<
 /out:Algo.exe /implib:Algo.lib /pdb:C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\cmake-build-debug\Algo.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\Algo.dir\build: Algo.exe

.PHONY : CMakeFiles\Algo.dir\build

CMakeFiles\Algo.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Algo.dir\cmake_clean.cmake
.PHONY : CMakeFiles\Algo.dir\clean

CMakeFiles\Algo.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\cmake-build-debug C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\cmake-build-debug C:\Users\lserazin\AppData\Local\gitkraken\app-5.0.2\Algo\cmake-build-debug\CMakeFiles\Algo.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\Algo.dir\depend
