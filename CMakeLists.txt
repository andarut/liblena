cmake_minimum_required(VERSION 3.23)

project(liblena)
project(liblena_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# liblena
add_library(liblena STATIC
    src/ycbcr.cpp
)
target_compile_features(liblena PRIVATE cxx_std_17)
target_include_directories(liblena PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# liblena_tests
add_executable(liblena_tests 
    tests/ycbcr_tests.cpp
)

find_package(GTest)

target_link_libraries(liblena_tests PRIVATE
    liblena
    gtest::gtest    
)

add_test(NAME liblena_tests COMMAND liblena_tests)