cmake_minimum_required(VERSION 3.23)

project(liblena)
project(liblena_tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(CMAKE_C_COMPILER="${CMAKE_C_COMPILER}")
message(CMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}")

# For Visulization
find_package(opengl_system)
find_package(stb)
find_package(glfw3)

# liblena
add_library(liblena INTERFACE)
target_compile_features(liblena INTERFACE cxx_std_20)
target_include_directories(liblena INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/Input
    ${CMAKE_CURRENT_SOURCE_DIR}/Types
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/JPEG
    ${CMAKE_CURRENT_SOURCE_DIR}/Visualization
)

target_link_libraries(liblena INTERFACE
    # Visulization libs
    opengl::opengl
    glfw
    stb::stb
)

# liblena_tests

# For Tests
find_package(GTest)
include(GoogleTest)
enable_testing()

add_executable(liblena_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests/PPM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests/YCbCr.cpp
)
target_link_libraries(liblena_tests
    liblena
    # Tests libs
    gtest::gtest
)

# For Tests
target_compile_definitions(liblena_tests PRIVATE
    RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/TestsData"
)
gtest_discover_tests(liblena_tests)