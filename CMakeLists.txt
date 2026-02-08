cmake_minimum_required(VERSION 3.10)

project(IIITD_interprator 
  VERSION     0.0.1
  DESCRIPTION "A custom programming language interprator")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# - - - C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)

# - - - Build mode (RELEASE / DEBUG)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

# - - - Binaries in bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(COMMON_FLAGS "-Wall -Werror -DPROFILE_ENABLED")

string(TOUPPER "${CMAKE_BUILD_TYPE}" TYPE_UPPER)
if(TYPE_UPPER STREQUAL "DEBUG")
  set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -O0 -DDEBUG -g -fsanitize=address")
else()
  set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -O3 -DNDEBUG")
endif()

# - - - Library
# Core interpreter logic (NO main.cpp here)
add_library(interprator 
  src/virtual_machine/value.cpp
  src/virtual_machine/vm.cpp
  src/virtual_machine/disassembler.cpp
  src/virtual_machine/instructions.cpp
  src/virtual_machine/chunk.cpp
  src/library.cpp
  src/utils/profiler.cpp
  src/utils/testManager.cpp)

target_include_directories(interprator PUBLIC include)

# - - - Executable
# Entry point to check if interpreter is working
add_executable(interprator_cli
  src/virtual_machine/main.cpp)

target_link_libraries(interprator_cli interprator)

# - - - Examples
add_custom_target(build_examples)

file(GLOB EX_FILES "examples/*.cpp")
foreach(FILE ${EX_FILES})
  get_filename_component(NAME ${FILE} NAME_WE)
  add_executable(${NAME} ${FILE})
  target_link_libraries(${NAME} interprator)
  add_dependencies(build_examples ${NAME})
  set_target_properties(${NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/examples")
endforeach()

# - - - Tests
add_custom_target(build_tests)

file(GLOB TST_FILES "tests/*.cpp")
foreach(FILE ${TST_FILES})
  get_filename_component(NAME ${FILE} NAME_WE)
  add_executable(${NAME} ${FILE})
  target_link_libraries(${NAME} interprator)
  add_dependencies(build_tests ${NAME})
  set_target_properties(${NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/tests")
endforeach()

# - - - Doxygen Documentation
find_package(Doxygen)
if (DOXYGEN_FOUND)
  add_custom_target(build_docs ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation"
    VERBATIM)
endif()

# - - - Clean build + bin directories
add_custom_target(clean_all
  COMMAND ${CMAKE_COMMAND} -E rm -rf
          ${CMAKE_BINARY_DIR}
          ${CMAKE_SOURCE_DIR}/bin
  COMMENT "Removing build and bin directories"
)
