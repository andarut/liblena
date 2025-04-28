cmake_minimum_required(VERSION 3.23)

project(liblena)
project(liblena_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(CMAKE_C_COMPILER="${CMAKE_C_COMPILER}")
message(CMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}")

# liblena
add_library(liblena STATIC
    src/ppm_image.cpp
)
target_compile_features(liblena PRIVATE cxx_std_17)
target_include_directories(liblena PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# liblena visualization
find_package(opengl_system)
find_package(stb)
find_package(glfw3)

add_executable(liblena_visualization
    visualization/visualization.cpp
)
target_include_directories(liblena_visualization PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/visualization/include)

target_link_libraries(liblena_visualization PRIVATE
    opengl::opengl
    glfw
    stb::stb
)


# liblena_tests
find_package(GTest)
include(GoogleTest)
enable_testing()

add_executable(liblena_tests 
    
    # unit tests
    tests/ppm_image_tests.cpp
    tests/ycbcr_tests.cpp
    tests/subsampling_tests.cpp
    
    # intergration tests
    tests/jpeg_tests.cpp
)

target_compile_definitions(liblena_tests PRIVATE
    RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/tests_data"
)

target_link_libraries(liblena_tests PRIVATE
    liblena
    gtest::gtest    
)

gtest_discover_tests(liblena_tests)