set (TARGET_NAME soloud)

set (LINK_LIBRARIES)

# Headers
set (TARGET_HEADERS
	../include/soloud.h
	../include/soloud_audiosource.h
	../include/soloud_bassboostfilter.h
	../include/soloud_biquadresonantfilter.h
	../include/soloud_bus.h
	../include/soloud_c.h
	../include/soloud_dcremovalfilter.h
	../include/soloud_echofilter.h
	../include/soloud_error.h
	../include/soloud_fader.h
	../include/soloud_fft.h
	../include/soloud_fftfilter.h
	../include/soloud_file.h
	../include/soloud_file_hack_off.h
	../include/soloud_file_hack_on.h
	../include/soloud_filter.h
	../include/soloud_flangerfilter.h
	../include/soloud_internal.h
	../include/soloud_lofifilter.h
	../include/soloud_monotone.h
	../include/soloud_openmpt.h
	../include/soloud_sfxr.h
	../include/soloud_speech.h
	../include/soloud_tedsid.h
	../include/soloud_thread.h
	../include/soloud_vic.h
	../include/soloud_wav.h
	../include/soloud_wavstream.h
)


# Core
set (CORE_SOURCES
	core/soloud.cpp
	core/soloud_audiosource.cpp
	core/soloud_bus.cpp
	core/soloud_core_3d.cpp
	core/soloud_core_basicops.cpp
	core/soloud_core_faderops.cpp
	core/soloud_core_filterops.cpp
	core/soloud_core_getters.cpp
	core/soloud_core_setters.cpp
	core/soloud_core_voicegroup.cpp
	core/soloud_core_voiceops.cpp
	core/soloud_fader.cpp
	core/soloud_fft.cpp
	core/soloud_fft_lut.cpp
	core/soloud_file.cpp
	core/soloud_filter.cpp
	core/soloud_thread.cpp
)


# Audiosources
set (AUDIOSOURCES_SOURCES
	audiosource/monotone/soloud_monotone.cpp
	audiosource/openmpt/soloud_openmpt.cpp
	audiosource/openmpt/soloud_openmpt_dll.c
	audiosource/sfxr/soloud_sfxr.cpp
	audiosource/speech/darray.cpp
	audiosource/speech/klatt.cpp
	audiosource/speech/resonator.cpp
	audiosource/speech/soloud_speech.cpp
	audiosource/speech/tts.cpp
	audiosource/tedsid/sid.cpp
	audiosource/tedsid/soloud_tedsid.cpp
	audiosource/tedsid/ted.cpp
	audiosource/vic/soloud_vic.cpp
	audiosource/wav/soloud_wav.cpp
	audiosource/wav/soloud_wavstream.cpp
	audiosource/wav/stb_vorbis.c
)


# Backends
set (BACKENDS_SOURCES)

if (SOLOUD_BACKEND_NULL)
	set (BACKENDS_SOURCES
		${BACKENDS_SOURCES}
		backend/null/soloud_null.cpp
	)
	add_definitions(-DWITH_NULL)
endif()

if (SOLOUD_BACKEND_SDL2)
	find_package (SDL2 REQUIRED)
	include_directories (${SDL2_INCLUDE_DIR})
	add_definitions (-DWITH_SDL2_STATIC)

	set (BACKENDS_SOURCES
		${BACKENDS_SOURCES}
		backend/sdl2_static/soloud_sdl2_static.cpp
	)

	set (LINK_LIBRARIES
		${LINK_LIBRARIES}
		${SDL2_LIBRARY}
	)

endif()

if (SOLOUD_BACKEND_COREAUDIO)
	if (NOT APPLE)
		message (FATAL_ERROR "CoreAudio backend can be enabled only on Apple!")
	endif ()

	add_definitions (-DWITH_COREAUDIO)

	set (BACKENDS_SOURCES
		${BACKENDS_SOURCES}
		backend/coreaudio/soloud_coreaudio.cpp
	)

	find_library (AUDIOTOOLBOX_FRAMEWORK AudioToolbox)
	set (LINK_LIBRARIES
		${LINK_LIBRARIES}
		${AUDIOTOOLBOX_FRAMEWORK}
	)
endif()


if (SOLOUD_BACKEND_OPENSLES)
	add_definitions (-DWITH_OPENSLES)

	set (BACKENDS_SOURCES
		${BACKENDS_SOURCES}
		backend/opensles/soloud_opensles.cpp
	)

	find_library (OPENSLES_LIBRARY OpenSLES)
	set (LINK_LIBRARIES
		${LINK_LIBRARIES}
		${OPENSLES_LIBRARY}
	)
endif()


# Filters
set (FILTERS_SOURCES
	filter/soloud_bassboostfilter.cpp
	filter/soloud_biquadresonantfilter.cpp
	filter/soloud_dcremovalfilter.cpp
	filter/soloud_echofilter.cpp
	filter/soloud_fftfilter.cpp
	filter/soloud_flangerfilter.cpp
	filter/soloud_lofifilter.cpp
)


# All together
source_group ("Includes"		FILES ${TARGET_HEADERS})
source_group ("Core"			FILES ${CORE_SOURCES})
source_group ("Audiosources"	FILES ${AUDIOSOURCES_SOURCES})
source_group ("Backends"		FILES ${BACKENDS_SOURCES})
source_group ("Filters"			FILES ${FILTERS_SOURCES})

set (TARGET_SOURCES
	${CORE_SOURCES}
	${AUDIOSOURCES_SOURCES}
	${BACKENDS_SOURCES}
	${FILTERS_SOURCES}
)

if (SOLOUD_DYNAMIC)
	add_library(${TARGET_NAME} ${TARGET_SOURCES})
endif ()

if (SOLOUD_STATIC)
	add_library(${TARGET_NAME} STATIC ${TARGET_SOURCES})
endif()

target_link_libraries (${TARGET_NAME} ${LINK_LIBRARIES})

include (Install)
INSTALL(FILES ${TARGET_HEADERS} DESTINATION include/${TARGET_NAME})
