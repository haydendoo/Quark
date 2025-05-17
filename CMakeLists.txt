cmake_minimum_required(VERSION 3.2)

project(Quark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_executable(quark ${SOURCES})

target_include_directories(quark PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${LLVM_INCLUDE_DIRS}
)

add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(quark ${llvm_libs})

target_compile_options(quark PRIVATE
    -isystem ${LLVM_INCLUDE_DIRS}
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
    >
)
