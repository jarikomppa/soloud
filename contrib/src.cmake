set(SOLOUD_PUBLIC_HEADERS_DIR ${SOLOUD_ROOT_DIRECTORY}/include)
set(SOLOUD_SOURCE_PATH_DIR ${SOLOUD_ROOT_DIRECTORY}/src)

#Stub the existence of the libraries
add_library(soloud STATIC)
add_library(soloud-dynamic SHARED)

function(apply_to_soloud_libraries FUNCTION_NAME)
	cmake_language(CALL ${FUNCTION_NAME} soloud ${ARGN})
	cmake_language(CALL ${FUNCTION_NAME} soloud-dynamic ${ARGN})
endfunction()

set(LINK_LIBRARIES)
set(SOLOUD_SOURCES)
######################################
#Core
######################################
set(SOLOUD_CORE_SOURCES
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_audiosource.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_audiosource.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_bassboostfilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_biquadresonantfilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_bus.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_bus.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_c.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_core_3d.cpp
	${SOLOUD_SOURCE_PATH_DIR}/soloud_core_basicops.cpp
	${SOLOUD_SOURCE_PATH_DIR}/soloud_core_faderops.cpp
	${SOLOUD_SOURCE_PATH_DIR}/soloud_core_filterops.cpp
	${SOLOUD_SOURCE_PATH_DIR}/soloud_core_getters.cpp
	${SOLOUD_SOURCE_PATH_DIR}/soloud_core_setters.cpp
	${SOLOUD_SOURCE_PATH_DIR}/soloud_core_voicegroup.cpp
	${SOLOUD_SOURCE_PATH_DIR}/soloud_core_voiceops.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_dcremovalfilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_echofilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_error.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_fader.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_fader.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_fft.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_fft.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_fftfilter.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_fft_lut.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_file.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_file.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_file_hack_off.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_file_hack_on.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_filter.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_filter.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_flangerfilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_internal.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_lofifilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_monotone.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_openmpt.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_queue.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_queue.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_robotizefilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_sfxr.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_speech.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_tedsid.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_thread.h
	${SOLOUD_SOURCE_PATH_DIR}/soloud_thread.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_vic.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_vizsn.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_wav.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_waveshaperfilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_wavstream.h
)
source_group ("Core" FILES ${SOLOUD_CORE_SOURCES})
list(APPEND SOLOUD_SOURCES ${SOLOUD_CORE_SOURCES})

######################################
#Audiosources
######################################
set(SOLOUD_AUDIOSOURCES_DIR ${SOLOUD_SOURCE_PATH_DIR}/audiosource)
set(SOLOUD_AUDIOSOURCES_SOURCES
	${SOLOUD_AUDIOSOURCES_DIR}/monotone/soloud_monotone.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/openmpt/soloud_openmpt.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/openmpt/soloud_openmpt_dll.c
	${SOLOUD_AUDIOSOURCES_DIR}/sfxr/soloud_sfxr.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/speech/darray.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/speech/klatt.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/speech/resonator.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/speech/soloud_speech.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/speech/tts.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/tedsid/sid.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/tedsid/soloud_tedsid.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/tedsid/ted.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/vic/soloud_vic.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/vizsn/soloud_vizsn.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/wav/dr_impl.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/wav/soloud_wav.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/wav/soloud_wavstream.cpp
	${SOLOUD_AUDIOSOURCES_DIR}/wav/stb_vorbis.c
)
source_group ("AudioSources" FILES ${SOLOUD_AUDIOSOURCES_SOURCES})
list(APPEND SOLOUD_SOURCES ${SOLOUD_AUDIOSOURCES_SOURCES})

######################################
#Filters
######################################
set (SOLOUD_FILTERS_PATH_DIR ${SOURCE_PATH}/filter)
set (SOLOUD_FILTERS_SOURCES
	${SOLOUD_FILTERS_PATH_DIR}/soloud_bassboostfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_biquadresonantfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_dcremovalfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_echofilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_fftfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_flangerfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_lofifilter.cpp
)
source_group ("Filters" FILES ${SOLOUD_FILTERS_SOURCES})
list(APPEND SOLOUD_SOURCES ${SOLOUD_FILTERS_SOURCES})

############################################
#Backends
############################################
set(SOLOUD_BACKENDS_SOURCE_DIR ${SOLOUD_SOURCE_PATH_DIR}/backend)
set(SOLOUD_BACKENDS_SOURCES)

if(SOLOUD_ENABLE_BACKEND_NULL)

	add_definitions(-DWITH_NULL)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/null/soloud_null.cpp)
endif()


if (SOLOUD_BACKEND_COREAUDIO)
	if (NOT APPLE)
		message (FATAL_ERROR "CoreAudio backend can be enabled only on Apple!")
	endif ()

	add_definitions (-DWITH_COREAUDIO)

	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/coreaudio/soloud_coreaudio.cpp)

	find_library (AUDIOTOOLBOX_FRAMEWORK AudioToolbox)
	set (LINK_LIBRARIES
		${LINK_LIBRARIES}
		${AUDIOTOOLBOX_FRAMEWORK}
	)
endif()


if (SOLOUD_BACKEND_OPENSLES)
	add_definitions (-DWITH_OPENSLES)

	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/opensles/soloud_opensles.cpp)

	find_library (OPENSLES_LIBRARY OpenSLES)
	set (LINK_LIBRARIES
		${LINK_LIBRARIES}
		${OPENSLES_LIBRARY}
	)
endif()


if (SOLOUD_BACKEND_XAUDIO2)
	add_definitions (-DWITH_XAUDIO2)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/xaudio2/soloud_xaudio2.cpp)
endif()

if (SOLOUD_BACKEND_WINMM)
	add_definitions (-DWITH_WINMM)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/winmm/soloud_winmm.cpp)
endif()

if (SOLOUD_BACKEND_WASAPI)
	add_definitions (-DWITH_WASAPI)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/wasapi/soloud_wasapi.cpp)
endif()


if (SOLOUD_BACKEND_SDL2)
	find_package (SDL2 REQUIRED)
	# target_include_directories(${TARGET_NAME} PUBLIC ${SDL2_INCLUDE_DIR})
	# target_compile_definitions(${TARGET_NAME} PUBLIC WITH_SDL2_STATIC=1)
	# target_link_libraries(${TARGET_NAME} PUBLIC ${SDL2_LIBRARY})
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/sdl2_static/soloud_sdl2_static.cpp)
endif()


source_group ("Backends" FILES ${SOLOUD_BACKENDS_SOURCES})
list(APPEND SOLOUD_SOURCES ${SOLOUD_BACKENDS_SOURCES})
apply_to_soloud_libraries(target_sources ${SOLOUD_SOURCES})


target_link_libraries (${TARGET_NAME} PUBLIC ${LINK_LIBRARIES})
target_include_directories(${TARGET_NAME} PUBLIC ${SOLOUD_ROOT_DIRECTORY}/include)

