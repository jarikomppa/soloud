### So loud CMakeLists.txt
project(SoLoud)
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)

# Add the SoLoud source files
file(GLOB_RECURSE SOURCES 
    "src/audiosource/*.c" "src/audiosource/*.cpp" "src/audiosource/*.h" "src/audiosource/*.hpp"
    "src/filter/*.c" "src/filter/*.cpp" "src/filter/*.h" "src/filter/*.hpp"
    "src/core/*.c" "src/core/*.cpp" "src/core/*.h" "src/core/*.hpp"
    # "src/tools/*.c" "src/tools/*.cpp" "src/tools/*.h" "src/tools/*.hpp"
    ## We use the portaudio backend
    "src/backend/portaudio/*.c" "src/backend/portaudio/*.cpp" "src/backend/portaudio/*.h" "src/backend/portaudio/*.hpp"
)

# Add the SoLoud include directory
include_directories("include")

# Create the SoLoud library
add_library(SoLoud ${SOURCES})

# PortAudio is required for SoLoud
add_subdirectory(vendor/portaudio)
include_directories(vendor/portaudio/include)

# Add the SoLoud library to the include directories
target_include_directories(SoLoud PUBLIC "include")

# Add the SoLoud library to the link directories
target_link_directories(SoLoud PUBLIC "lib")
