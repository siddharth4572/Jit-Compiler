cmake_minimum_required(VERSION 3.13)
project(jit LANGUAGES C CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# LLVM setup
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Map required LLVM components (extended)
llvm_map_components_to_libnames(llvm_libs
    core
    irreader
    nativecodegen
    native
    orcjit
    support
    executionengine
    passes
    transformutils
)

# Flex and Bison
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(Parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
             DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.hpp)
FLEX_TARGET(Lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

# Include dirs
include_directories(
    include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Build target
add_executable(jit
    src/main.cpp
    src/codegen.cpp
    src/helper.cpp
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
)

# Link LLVM
target_link_libraries(jit ${llvm_libs})

