### So loud CMakeLists.txt
project(SoLoud)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)

# Add the SoLoud source files
file(GLOB_RECURSE SOURCES "src/*.c" "src/*.cpp" "src/*.h" "src/*.hpp")

# Add the SoLoud include directory
include_directories("include")

# Create the SoLoud library
add_library(SoLoud ${SOURCES})

# Add the SoLoud library to the include directories
target_include_directories(SoLoud PUBLIC "include")

# Add the SoLoud library to the link directories
target_link_directories(SoLoud PUBLIC "lib")
