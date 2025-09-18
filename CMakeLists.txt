cmake_minimum_required(VERSION 3.23)

project(liblena)
project(liblena_tests)

find_package(imgui)
find_package(glfw3)
find_package(opengl_system)
find_package(GTest)
find_package(glad)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(CMAKE_C_COMPILER="${CMAKE_C_COMPILER}")
message(CMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}")

# liblena
add_library(liblena
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/src/ycbcr.cpp
)
target_compile_features(liblena PRIVATE cxx_std_20)
target_include_directories(liblena PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/include/
)

target_link_libraries(liblena PRIVATE
    # Visulization libs
    opengl::opengl
    glfw
    imgui::imgui
    glad::glad
)

# liblena_tests

# For Tests
# find_package(GTest)
include(GoogleTest)
enable_testing()

add_executable(liblena_tests
#     # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/PPM.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tests/test_ycbcr.cpp
#     # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/Downsampling.cpp
#     # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/MCUs.cpp
#     # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/DCT.cpp
#     # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/Quantization.cpp
#     # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/EntropyCoding.cpp
#     ${CMAKE_CURRENT_SOURCE_DIR}/Tests/JPEG.cpp
)
target_include_directories(liblena_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/include/
)

target_link_libraries(liblena_tests
    liblena
#     # Tests libs
    gtest::gtest
)

# # # For Tests
target_compile_definitions(liblena_tests PRIVATE
    RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/TestsData"
)
gtest_discover_tests(liblena_tests)