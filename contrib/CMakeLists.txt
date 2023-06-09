cmake_minimum_required (VERSION 3.0)
project (SoLoud)

set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

include (OptionDependentOnPackage)
include (PrintOptionStatus)

option (SOLOUD_DYNAMIC "Set to ON to build dynamic SoLoud" OFF)
print_option_status (SOLOUD_DYNAMIC "Build dynamic library")

option (SOLOUD_STATIC "Set to ON to build static SoLoud" ON)
print_option_status (SOLOUD_STATIC "Build static library")

# TODO:
option (SOLOUD_BUILD_DEMOS "Set to ON for building demos" OFF)
print_option_status (SOLOUD_BUILD_DEMOS "Build demos")

option (SOLOUD_ENABLE_BACKEND_NULL "Set to ON for building NULL backend" ON)
print_option_status (SOLOUD_ENABLE_BACKEND_NULL "NULL backend")

option (SOLOUD_BACKEND_SDL2 "Set to ON for building SDL2 backend" OFF)
print_option_status (SOLOUD_BACKEND_SDL2 "SDL2 backend")

option (SOLOUD_BACKEND_COREAUDIO "Set to ON for building CoreAudio backend" OFF)
print_option_status (SOLOUD_BACKEND_COREAUDIO "CoreAudio backend")

option (SOLOUD_BACKEND_OPENSLES "Set to ON for building OpenSLES backend" OFF)
print_option_status (SOLOUD_BACKEND_OPENSLES "OpenSLES backend")

option (SOLOUD_BACKEND_XAUDIO2 "Set to ON for building XAudio2 backend" OFF)
print_option_status (SOLOUD_BACKEND_XAUDIO2 "XAudio2 backend")

option (SOLOUD_BACKEND_WINMM "Set to ON for building WINMM backend" OFF)
print_option_status (SOLOUD_BACKEND_WINMM "WINMM backend")

option (SOLOUD_BACKEND_WASAPI "Set to ON for building WASAPI backend" OFF)
print_option_status (SOLOUD_BACKEND_WASAPI "WASAPI backend")

#Establish some basic mappings to filesystem directories
get_filename_component(SOLOUD_ROOT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

#INCLUDE (InstallIncludes.cmake)
#INCLUDE (InstallStaticData.cmake)

if (UNIX AND NOT WIN32 AND NOT APPLE)
	if (CMAKE_SIZEOF_VOID_P MATCHES "8")
		set (LIB_POSTFIX "64" CACHE STRING "suffix for 32/64 dir placement")
		mark_as_advanced (LIB_POSTFIX)
	endif ()
endif ()
if (NOT DEFINED LIB_POSTFIX)
	set (LIB_POSTFIX "")
endif ()

include (src.cmake)

# Demos
IF (SOLOUD_BUILD_DEMOS)
	include (demos.cmake)
endif ()

