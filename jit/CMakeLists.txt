cmake_minimum_required(VERSION 3.13)
project(jit LANGUAGES C CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Include LLVM headers and definitions
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Map required LLVM components (adjust components if needed)


llvm_map_components_to_libnames(llvm_libs
    core
    irreader
    mcjit          # ✅ Add this for MCJIT
    native
    nativecodegen
    support
    executionengine
    passes
    transformutils
)


# Find Flex and Bison
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(Parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp)
FLEX_TARGET(Lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

# Include directories
include_directories(
    include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Backend library for core compiler + frontend interface
add_library(jit_backend STATIC
    src/codegen.cpp
    src/helper.cpp
    src/jit_interface.cpp
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
)

target_include_directories(jit_backend PUBLIC
    include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(jit_backend
    ${llvm_libs}
)

# Main CLI executable
add_executable(jit
    src/main.cpp
    src/codegen.cpp
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
    # other sources for jit
)

add_executable(jit_gui
    src/jit_gui.cpp
    src/gui_backend.cpp
    src/jit_interface.cpp
    # other sources for jit_gui
)

target_link_libraries(jit
    jit_backend
    ${llvm_libs}
)

# -------------------- FLTK GUI Support --------------------
find_package(FLTK REQUIRED)


target_include_directories(jit_gui PRIVATE
    include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(jit_gui
    jit_backend
    ${FLTK_LIBRARIES}
    ${llvm_libs}
)

