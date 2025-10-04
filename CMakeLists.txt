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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/YCbCrConverter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DCT.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Quantizator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HuffmanEncoder.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/include/libjpeg/Zigzag.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libjpeg/DPCM.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libjpeg/RLC.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libjpeg/HuffmanTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libjpeg/PPM.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QuantizationTable.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Compressor.cpp    

    # types
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageCh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Image.cpp
)
target_compile_features(liblena PRIVATE cxx_std_20)
target_include_directories(liblena PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libjpeg
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
    # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/test_PPMReader.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/test_YCbCrConverter.cpp
#     # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/Downsampling.cpp
#     # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/MCUs.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/test_DCT.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/test_Quantizator.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/test_Zigzag.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/test_RLC.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/Tests/test_HuffmanEncoder.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/Tests/test_Compressor.cpp
)
target_include_directories(liblena_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libjpeg/
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
gtest_discover_tests(liblena_tests
DISCOVERY_MODE PRE_TEST
    PROPERTIES
        EXCLUDE_FROM_DEFAULT_BUILD 1)