CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT (SoLoud)

SET (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
#INCLUDE (StandartIncludes)

INCLUDE_DIRECTORIES (include)
INCLUDE (Configure.cmake)
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

add_subdirectory (src)

# Demos
IF (SOLOUD_BUILD_DEMOS)
	add_subdirectory (demos)
endif ()

include (InstallExport)
