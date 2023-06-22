cmake_minimum_required (VERSION 3.0)
project(SoLoud)

list(APPEND CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake)

include(OptionDependentOnPackage)
include(PrintOptionStatus)

option(SOLOUD_DYNAMIC "Set to ON to build dynamic SoLoud" OFF)
print_option_status (SOLOUD_DYNAMIC "Build dynamic library")

option(SOLOUD_STATIC "Set to ON to build static SoLoud" ON)
print_option_status (SOLOUD_STATIC "Build static library")

option(SOLOUD_BUILD_DEMOS "Set to ON for building demos" OFF)
option(SOLOUD_ENABLE_BACKEND_NULL "Set to ON for building NULL backend" ON)
option(SOLOUD_ENABLE_BACKEND_SDL2 "Set to ON for building SDL2 backend" OFF)
option(SOLOUD_ENABLE_BACKEND_ALSA "Set to ON for building the ALSA backend" OFF)
option(SOLOUD_ENABLE_BACKEND_PIPEWIRE "Set to ON for building PIPEWIRE backend" OFF)
option(SOLOUD_ENABLE_BACKEND_COREAUDIO "Set to ON for building CoreAudio backend" OFF)
option(SOLOUD_ENABLE_BACKEND_OPENSLES "Set to ON for building OpenSLES backend" OFF)
option(SOLOUD_ENABLE_BACKEND_XAUDIO2 "Set to ON for building XAudio2 backend" OFF)
option(SOLOUD_ENABLE_BACKEND_WINMM "Set to ON for building WINMM backend" OFF)
option(SOLOUD_ENABLE_BACKEND_WASAPI "Set to ON for building WASAPI backend" OFF)
option(SOLOUD_AUTO_DETECT_BACKENDS "Attempt to enable backends based on what is available" ON)

if(SOLOUD_AUTO_DETECT_BACKENDS)
	find_package(SDL2)
	set(SOLOUD_ENABLE_BACKEND_SDL2 ${SDL2_FOUND})
	find_package(ALSA)
	set(SOLOUD_ENABLE_BACKEND_ALSA ${ALSA_FOUND})
	find_package(PipeWire)
	set(SOLOUD_ENABLE_BACKEND_PIPEWIRE ${PipeWire_FOUND})

	if(APPLE)
		set(SOLOUD_BACKEND_COREAUDIO ON)
	endif()

	find_package(OpenSLES)
	set(SOLOUD_ENABLE_BACKEND_OPENSLES ${OPENSLES_FOUND})
endif()

if(SOLOUD_BUILD_DEMOS)
	set(SOLOUD_STATIC ON)
endif()

print_option_status (SOLOUD_BUILD_DEMOS "Build demos")
print_option_status (SOLOUD_AUTO_DETECT_BACKENDS "Auto Detect Backends")
print_option_status (SOLOUD_ENABLE_BACKEND_NULL "NULL backend")
print_option_status (SOLOUD_BACKEND_SDL2 "SDL2 backend")
print_option_status (SOLOUD_ENABLE_BACKEND_ALSA "ALSA backend")
print_option_status (SOLOUD_ENABLE_BACKEND_PIPEWIRE "PipeWire backend")
print_option_status (SOLOUD_BACKEND_COREAUDIO "CoreAudio backend")
print_option_status (SOLOUD_BACKEND_OPENSLES "OpenSLES backend")
print_option_status (SOLOUD_BACKEND_XAUDIO2 "XAudio2 backend")
print_option_status (SOLOUD_BACKEND_WINMM "WINMM backend")
print_option_status (SOLOUD_BACKEND_WASAPI "WASAPI backend")

#Establish some basic mappings to filesystem directories
get_filename_component(SOLOUD_ROOT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

include (src.cmake)

# Demos
if(SOLOUD_BUILD_DEMOS)
	include (demos.cmake)
endif ()

