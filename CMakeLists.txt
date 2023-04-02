cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    my-thread-pool
    VERSION 2023.4
    LANGUAGES C CXX
)

# clang tools
set(CLANG_TOOLS_PATH "/usr/local/bin" "/usr/bin")
find_program(
    CLANG_FORMAT_BIN
    NAMES clang-format clang-format-14
    HINTS ${CLANG_TOOLS_PATH}
)

if ("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
    message(WARNING "clang-format not found.")
else ()
    message(STATUS "Found clang-format at ${CLANG_FORMAT_BIN}")
endif ()

find_program(
    CLANG_TIDY_BIN
    NAMES clang-tidy clang-tidy-14
    HINTS ${CLANG_TOOLS_PATH}
)

if ("${CLANG_TIDY_BIN}" STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
    message(WARNING "clang-tidy not found.")
else ()
    message(STATUS "Found clang-tidy at ${CLANG_TIDY_BIN}")
endif ()

# CTest
enable_testing()

# Compiler flags.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-unused-parameter -Wno-attributes")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

# files
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_SRC_DIR ${CMAKE_SOURCE_DIR}/test)
set(SRC_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)
set(THIRD_PARTY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/fmt/include)
file(GLOB_RECURSE SRC_LIST
     "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE TEST_SRC_LIST
     "${CMAKE_SOURCE_DIR}/test/*.cpp")
file(GLOB_RECURSE INCLUDE_LIST
     "${CMAKE_SOURCE_DIR}/src/*.h")

# include
include_directories(${SRC_INCLUDE_DIR} ${THIRD_PARTY_INCLUDE_DIR})

# subdirectory
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(third_party)
add_subdirectory(build_support)

# custom_target
add_custom_target(
    format
    ${CLANG_FORMAT_BIN} -i -style=file ${INCLUDE_LIST} ${SRC_LIST} ${TEST_SRC_LIST}
)

add_custom_target(
    tidy
    # ${CMAKE_SOURCE_DIR}/build_support/run_clang_tidy ${CLANG_TIDY_BIN} ${CMAKE_BINARY_DIR}
    bash ${CMAKE_SOURCE_DIR}/build_support/run_clang_tidy.sh ${CLANG_TIDY_BIN} ${CMAKE_BINARY_DIR}
)