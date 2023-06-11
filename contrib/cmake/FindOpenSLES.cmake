# - Find OpenSLES
# Find the OpenSLES includes and libraries
#
#  OPENSLES_INCLUDE_DIR - where to find dsound.h
#  OPENSLES_LIBRARIES   - List of libraries when using dsound.
#  OPENSLES_FOUND       - True if dsound found.

get_property(_FIND_LIBRARY_USE_LIB64_PATHS GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS 1)

find_path(OpenSLES_INCLUDE_DIR SLES/OpenSLES.h)
find_library(OpenSLES_LIBRARY NAMES OpenSLES)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenSLES DEFAULT_MSG OpenSLES_INCLUDE_DIR OpenSLES_LIBRARY)

if(OpenSLES_FOUND)
	set(OpenSLES_INCLUDE_DIRS ${OPENSLES_INCLUDE_DIR})
	set(OpenSLES_LIBRARIES ${OPENSLES_LIBRARY})
else(OpenSLES_FOUND)
	if (OpenSLES_FIND_REQUIRED)
		message(FATAL_ERROR "Could NOT find OPENSLES")
	endif()
endif(OpenSLES_FOUND)

mark_as_advanced(OpenSLES_INCLUDE_DIR OpenSLES_LIBRARY)

set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ${_FIND_LIBRARY_USE_LIB64_PATHS})
