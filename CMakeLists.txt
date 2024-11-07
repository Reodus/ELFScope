cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME ELF_PARSER)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)

include_directories(
    "include"
)

set(INC_FILES
    "include/elf_parser.h"
    "include/eheader_parser.h"
    "include/pheader_parser.h"
)

set(SRC_FILES
    "src/main.cpp"
    "src/elf_parser.cpp"
    "src/eheader_parser.cpp"
    "src/pheader_parser.cpp"
    
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES})
