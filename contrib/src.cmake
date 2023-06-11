set(SOLOUD_PUBLIC_HEADERS_DIR ${SOLOUD_ROOT_DIRECTORY}/include)
set(SOLOUD_SOURCE_PATH_DIR ${SOLOUD_ROOT_DIRECTORY}/src)

#Stub the existence of the libraries
if(SOLOUD_STATIC)
	add_library(soloud STATIC)
endif()
if(SOLOUD_DYNAMIC)
	add_library(soloud-dynamic SHARED)
	set_target_properties(soloud-dynamic
						PROPERTIES
						POSITION_INDEPENDENT_CODE ON)
endif()

function(apply_to_soloud_libraries FUNCTION_NAME)
	if(SOLOUD_STATIC)
		cmake_language(CALL ${FUNCTION_NAME} soloud ${ARGN})
	endif()
	if(SOLOUD_DYNAMIC)
		cmake_language(CALL ${FUNCTION_NAME} soloud-dynamic ${ARGN})
	endif()
endfunction()

set(SOLOUD_SOURCES)
######################################
#Core
######################################
set(SOLOUD_CORE_SOURCE_DIR ${SOLOUD_SOURCE_PATH_DIR}/core)
set(SOLOUD_CORE_SOURCES
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_audiosource.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_audiosource.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_bassboostfilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_biquadresonantfilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_bus.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_bus.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_c.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_core_3d.cpp
	${SOLOUD_CORE_SOURCE_DIR}/soloud_core_basicops.cpp
	${SOLOUD_CORE_SOURCE_DIR}/soloud_core_faderops.cpp
	${SOLOUD_CORE_SOURCE_DIR}/soloud_core_filterops.cpp
	${SOLOUD_CORE_SOURCE_DIR}/soloud_core_getters.cpp
	${SOLOUD_CORE_SOURCE_DIR}/soloud_core_setters.cpp
	${SOLOUD_CORE_SOURCE_DIR}/soloud_core_voicegroup.cpp
	${SOLOUD_CORE_SOURCE_DIR}/soloud_core_voiceops.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_dcremovalfilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_echofilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_error.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_fader.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_fader.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_fft.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_fft.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_fftfilter.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_fft_lut.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_file.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_file.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_file_hack_off.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_file_hack_on.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_filter.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_filter.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_flangerfilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_internal.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_lofifilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_monotone.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_openmpt.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_queue.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_queue.cpp
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_robotizefilter.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_sfxr.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_speech.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_tedsid.h
	${SOLOUD_PUBLIC_HEADERS_DIR}/soloud_thread.h
	${SOLOUD_CORE_SOURCE_DIR}/soloud_thread.cpp
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
set (SOLOUD_FILTERS_PATH_DIR ${SOLOUD_SOURCE_PATH_DIR}/filter)
set (SOLOUD_FILTERS_SOURCES
	${SOLOUD_FILTERS_PATH_DIR}/soloud_bassboostfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_biquadresonantfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_dcremovalfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_echofilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_fftfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_flangerfilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_lofifilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_robotizefilter.cpp
	${SOLOUD_FILTERS_PATH_DIR}/soloud_waveshaperfilter.cpp
)
source_group ("Filters" FILES ${SOLOUD_FILTERS_SOURCES})
list(APPEND SOLOUD_SOURCES ${SOLOUD_FILTERS_SOURCES})

############################################
#C API
############################################
set(SOLOUD_C_API_SOURCE_DIR ${SOLOUD_SOURCE_PATH_DIR}/c_api)
set(SOLOUD_C_API_SOURCES
	${SOLOUD_C_API_SOURCE_DIR}/soloud_c.cpp
	)
source_group("C API" FILES ${SOLOUD_C_API_SOURCES})
list(APPEND SOLOUD_SOURCES ${SOLOUD_C_API_SOURCES})

############################################
#Backends
############################################
set(SOLOUD_BACKENDS_SOURCE_DIR ${SOLOUD_SOURCE_PATH_DIR}/backend)
set(SOLOUD_BACKENDS_SOURCES)

if(SOLOUD_ENABLE_BACKEND_NULL)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/null/soloud_null.cpp)
	apply_to_soloud_libraries(target_compile_definitions PUBLIC WITH_NULL=1)
endif()

if(SOLOUD_ENABLE_BACKEND_SDL2)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/sdl2_static/soloud_sdl2_static.cpp)

	find_package (SDL2 REQUIRED)
	apply_to_soloud_libraries(target_include_directories PUBLIC ${SDL2_INCLUDE_DIR})
	apply_to_soloud_libraries(target_link_libraries PUBLIC ${SDL2_LIBRARY})
	apply_to_soloud_libraries(target_compile_definitions PUBLIC WITH_SDL2_STATIC=1)
endif()

if(SOLOUD_ENABLE_BACKEND_ALSA)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/alsa/soloud_alsa.cpp)

	find_package(ALSA REQUIRED)
	apply_to_soloud_libraries(target_include_directories PUBLIC ${ALSA_INCLUDE_DIRS})
	apply_to_soloud_libraries(target_link_libraries PUBLIC ${ALSA_LIBRARIES})
	apply_to_soloud_libraries(target_compile_definitions PUBLIC WITH_ALSA=1)
endif()

if (SOLOUD_ENABLE_BACKEND_COREAUDIO)
	if(NOT APPLE)
		message(FATAL_ERROR "CoreAudio backend can be enabled only on Apple!")
	endif()

	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/coreaudio/soloud_coreaudio.cpp)

	find_library (AUDIOTOOLBOX_FRAMEWORK AudioToolbox)
	apply_to_soloud_libraries(target_link_libraries PUBLIC ${AUDIOTOOLBOX_FRAMEWORK})
	apply_to_soloud_libraries(target_compile_definitions PUBLIC WITH_COREAUDIO=1)
endif()

if (SOLOUD_ENABLE_BACKEND_OPENSLES)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/opensles/soloud_opensles.cpp)

	find_library (OPENSLES_LIBRARY OpenSLES)
	apply_to_soloud_libraries(target_link_libraries PUBLIC ${OPENSLES_LIBRARY})
	apply_to_soloud_libraries(target_compile_definitions PUBLIC WITH_OPENSLES=1)
endif()

if (SOLOUD_ENABLE_BACKEND_XAUDIO2)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/xaudio2/soloud_xaudio2.cpp)
	apply_to_soloud_libraries(target_compile_definitions PUBLIC WITH_XAUDIO2=1)
endif()

if (SOLOUD_ENABLE_BACKEND_WINMM)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/winmm/soloud_winmm.cpp)
	apply_to_soloud_libraries(target_compile_definitions PUBLIC WITH_WINMM=1)
endif()

if (SOLOUD_ENABLE_BACKEND_WASAPI)
	list(APPEND SOLOUD_BACKENDS_SOURCES ${SOLOUD_BACKENDS_SOURCE_DIR}/wasapi/soloud_wasapi.cpp)
	apply_to_soloud_libraries(target_compile_definitions PUBLIC WITH_WASAPI=1)
endif()

source_group ("Backends" FILES ${SOLOUD_BACKENDS_SOURCES})
list(APPEND SOLOUD_SOURCES ${SOLOUD_BACKENDS_SOURCES})

#Apply the source files
apply_to_soloud_libraries(target_sources PUBLIC ${SOLOUD_SOURCES})
apply_to_soloud_libraries(target_include_directories PUBLIC ${SOLOUD_ROOT_DIRECTORY}/include)
