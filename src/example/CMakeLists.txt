# Example use of scripting module
include_directories(
    ${PROJECT_SOURCE_DIR}/src/scripting
    ${PROJECT_SOURCE_DIR}/src/dtool 
    ${PANDA_DIR}/include 
    ${PYTHON_DIR}/include
)
link_directories(${PYTHON_DIR}/libs ${PANDA_DIR}/lib)
add_executable(example main.cpp)
target_link_libraries(
    example py_panda scripting
    libpanda libp3framework libpandaexpress libp3dtool libp3dtoolconfig libp3interrogatedb
)
set_target_properties(example PROPERTIES OUTPUT_NAME "example")

# Collect panda DLLs
file(GLOB PANDA_DLLS
    ${PANDA_DIR}/bin/libp*.dll
)

# Copy required DLLs from Panda installation
foreach( file_i ${PANDA_DLLS})
    add_custom_command(
    TARGET example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file_i} ${CMAKE_BINARY_DIR}/src/example
    )
endforeach( file_i )
