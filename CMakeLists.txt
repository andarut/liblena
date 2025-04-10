cmake_minimum_required(VERSION 3.23)

project(liblena)
project(liblena_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# liblena
add_library(liblena STATIC
    src/ppm_image.cpp
)
target_compile_features(liblena PRIVATE cxx_std_17)
target_include_directories(liblena PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


# liblena_tests
find_package(GTest)
include(GoogleTest)
enable_testing()

add_executable(liblena_tests 
    tests/ppm_image_tests.cpp
)

target_compile_definitions(liblena_tests PRIVATE
    RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/tests_data"
)

target_link_libraries(liblena_tests PRIVATE
    liblena
    gtest::gtest    
)

gtest_discover_tests(liblena_tests)