cmake_minimum_required(VERSION 3.10)
project(DynamicJITCompiler)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

include_directories(include)

BISON_TARGET(Parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/generated/parser.cpp)
FLEX_TARGET(Lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/generated/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)

add_executable(jit
    src/main.cpp
    src/helper.cpp
    ${BISON_Parser_OUTPUTS}
    ${FLEX_Lexer_OUTPUTS}
)

set(CMAKE_CXX_STANDARD 17)
