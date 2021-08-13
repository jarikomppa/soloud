
# We need python to run the glue-code generators
find_package (Python3 COMPONENTS Interpreter REQUIRED)

# Create the executable that generates soloud_codegen.py
add_executable (codegen ../src/tools/codegen/main.cpp)

# Add a command to run the executable to generate the python file
add_custom_command (OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/soloud_codegen.py"
        COMMAND codegen ARGS go
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Now we can run the actual Python code-generators, BUT we need to add a dependency on
# soloud_codegen.py, or otherwise codegen.exe will not be run beforehand

###############################################################################
# C# API
###############################################################################
add_custom_command (OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/../glue/soloud.cs"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/gen_cs.py"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/soloud_codegen.py"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/gen_cs.py"
        )
add_custom_target (generate_glue_cs ALL DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../glue/soloud.cs")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../glue/soloud.cs" DESTINATION glue)

###############################################################################
# Python API
###############################################################################
add_custom_command (OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/../glue/soloud.rb"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/gen_ruby.py"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/soloud_codegen.py"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/gen_ruby.py"
        )
add_custom_target (generate_glue_ruby ALL DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../glue/soloud.rb")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../glue/soloud.rb" DESTINATION glue)

###############################################################################
# Ruby API
###############################################################################
add_custom_command (OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/../glue/soloud.py"
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/gen_python.py"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/soloud_codegen.py"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../scripts/gen_python.py"
        )
add_custom_target(generate_glue_python ALL DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/../glue/soloud.py")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../glue/soloud.py" DESTINATION glue)
