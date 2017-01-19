set (TARGET_NAME SoLoud_welcome)

add_executable (${TARGET_NAME} main.cpp)

target_link_libraries (${TARGET_NAME}
	soloud
)
