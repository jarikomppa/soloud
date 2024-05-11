### So loud CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(SoLoud)
set(CMAKE_CXX_STANDARD 17)

set(USE_PORTAUDIO 0)
set(USE_MINIAUDIO 1)

# Add the SoLoud source files
file(GLOB_RECURSE SOURCES 
    "src/audiosource/*.c" "src/audiosource/*.cpp" "src/audiosource/*.h" "src/audiosource/*.hpp"
    "src/filter/*.c" "src/filter/*.cpp" "src/filter/*.h" "src/filter/*.hpp"
    "src/core/*.c" "src/core/*.cpp" "src/core/*.h" "src/core/*.hpp"
    "src/c_api/*.c" "src/c_api/*.cpp" "src/c_api/*.h" "src/c_api/*.hpp"
    # "src/tools/*.c" "src/tools/*.cpp" "src/tools/*.h" "src/tools/*.hpp"
    ## We use the portaudio backend
    # "src/backend/portaudio/*.c" "src/backend/portaudio/*.cpp" "src/backend/portaudio/*.h" "src/backend/portaudio/*.hpp"
    "src/backend/miniaudio/*.c" "src/backend/miniaudio/*.cpp" "src/backend/miniaudio/*.h" "src/backend/miniaudio/*.hpp"
)

if (USE_PORTAUDIO)
    add_compile_definitions(WITH_PORTAUDIO)
endif()

if (USE_MINIAUDIO)
    add_compile_definitions(WITH_MINIAUDIO)
endif()

# Add the SoLoud include directory
include_directories("include")

# Create the SoLoud library
add_library(SoLoud ${SOURCES})

if (USE_PORTAUDIO)
    add_subdirectory(vendor/portaudio)
    include_directories(vendor/portaudio/include)
endif()

# Add the SoLoud library to the include directories
target_include_directories(SoLoud PUBLIC "include")

# Add the SoLoud library to the link directories
target_link_directories(SoLoud PUBLIC "lib")
