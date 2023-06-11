set(SOLOUD_DEMO_DIRECTORY ${SOLOUD_ROOT_DIRECTORY}/demos)


##############################
#Backing libraries for Demo
##############################
set(SOLOUD_DEMO_COMMON ${SOLOUD_DEMO_DIRECTORY}/common)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL)

set(SOLOUD_DEMO_GLEW_DIR ${SOLOUD_DEMO_COMMON}/glew/GL)
set(SOLOUD_DEMO_GLEW_SOURCES
	${SOLOUD_DEMO_GLEW_DIR}/glew.c
	${SOLOUD_DEMO_GLEW_DIR}/glew.h
	${SOLOUD_DEMO_GLEW_DIR}/glxew.h
	${SOLOUD_DEMO_GLEW_DIR}/wglew.h
)
add_library(soloud-demo-glew STATIC ${SOLOUD_DEMO_GLEW_SOURCES})
target_include_directories(soloud-demo-glew PUBLIC ${SOLOUD_DEMO_COMMON}/glew)

set(SOLOUD_DEMO_IMGUI_DIR ${SOLOUD_DEMO_COMMON}/imgui)
set(SOLOUD_IMGUI_SOURCES
	${SOLOUD_DEMO_IMGUI_DIR}/imconfig.h
	${SOLOUD_DEMO_IMGUI_DIR}/imgui.cpp
	${SOLOUD_DEMO_IMGUI_DIR}/imgui_demo.cpp
	${SOLOUD_DEMO_IMGUI_DIR}/imgui_draw.cpp
	${SOLOUD_DEMO_IMGUI_DIR}/imgui.h
	${SOLOUD_DEMO_IMGUI_DIR}/imgui_internal.h
	${SOLOUD_DEMO_IMGUI_DIR}/stb_rect_pack.h
	${SOLOUD_DEMO_IMGUI_DIR}/stb_textedit.h
	${SOLOUD_DEMO_IMGUI_DIR}/stb_truetype.h
	)
add_library(soloud-imgui STATIC ${SOLOUD_IMGUI_SOURCES})
target_include_directories(soloud-imgui PUBLIC ${SOLOUD_DEMO_COMMON}/imgui)


set(SOLOUD_DEMO_FRAMEWORK_SOURCES
	${SOLOUD_DEMO_COMMON}/soloud_demo_framework.cpp
	${SOLOUD_DEMO_COMMON}/soloud_demo_framework.h
	${SOLOUD_DEMO_COMMON}/stb_image.h
	)
add_library(soloud-demo-framework STATIC ${SOLOUD_DEMO_FRAMEWORK_SOURCES})
target_include_directories(soloud-demo-framework PUBLIC ${SOLOUD_DEMO_COMMON})
target_link_libraries(soloud-demo-framework PUBLIC soloud-imgui soloud-demo-glew OpenGL::GL)

##############################
#Demos
##############################

#Simplest
add_executable(simplest ${SOLOUD_DEMO_DIRECTORY}/simplest/main.cpp)
target_link_libraries(simplest PUBLIC soloud)

#C API test
add_executable(ctest ${SOLOUD_DEMO_DIRECTORY}/c_test/main.c)
target_link_libraries(ctest PUBLIC soloud)

#Welcome
add_executable(welcome ${SOLOUD_DEMO_DIRECTORY}/welcome/main.cpp)
target_link_libraries(welcome PUBLIC soloud)

#env
add_executable(env ${SOLOUD_DEMO_DIRECTORY}/env/main.cpp)
target_link_libraries(env PUBLIC soloud soloud-demo-framework)

#null
add_executable(null ${SOLOUD_DEMO_DIRECTORY}/null/main.cpp)
target_link_libraries(null PUBLIC soloud)

#piana
set(SOLOUD_DEMO_PIANO_SOURCE_DIR ${SOLOUD_DEMO_DIRECTORY}/piano)
set(SOLOUD_DEMO_PIANO_SOURCES
	${SOLOUD_DEMO_PIANO_SOURCE_DIR}/main.cpp
	${SOLOUD_DEMO_PIANO_SOURCE_DIR}/RtMidi.cpp
	${SOLOUD_DEMO_PIANO_SOURCE_DIR}/RtMidi.h
	${SOLOUD_DEMO_PIANO_SOURCE_DIR}/soloud_basicwave.cpp
	${SOLOUD_DEMO_PIANO_SOURCE_DIR}/soloud_basicwave.h
	${SOLOUD_DEMO_PIANO_SOURCE_DIR}/soloud_padsynth.cpp
	${SOLOUD_DEMO_PIANO_SOURCE_DIR}/soloud_padsynth.h
	)
add_executable(piano ${SOLOUD_DEMO_PIANO_SOURCES})
target_link_libraries(piano PUBLIC soloud soloud-demo-framework)

#MegaDemo
set(SOLOUD_DEMO_MEGADEMO_SOURCE_DIR ${SOLOUD_DEMO_DIRECTORY}/megademo)
set(SOLOUD_DEMO_MEGADEMO_SOURCES
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/3dtest.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/main.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/mixbusses.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/monotone.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/multimusic.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/pewpew.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/radiogaga.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/space.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/speechfilter.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/tedsid.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/virtualvoices.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/wavformats.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/speakers.cpp
		${SOLOUD_DEMO_MEGADEMO_SOURCE_DIR}/thebutton.cpp
)
add_executable(megademo ${SOLOUD_DEMO_MEGADEMO_SOURCES})
target_link_libraries(megademo PUBLIC soloud soloud-demo-framework)
