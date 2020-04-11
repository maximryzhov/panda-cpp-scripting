include_directories(${PROJECT_SOURCE_DIR}/src/scripting ${PANDA_DIR}/include ${PYTHON_DIR}/include)
link_directories( ${PYTHON_DIR}/libs ${PANDA_DIR}/lib)

# Build C++ DLL
add_library(scripting SHARED config_scripting.cpp pyobjectWrapper.cpp scriptManager.cpp )
target_link_libraries(scripting py_panda libpanda libp3framework libpandaexpress libp3dtool libp3dtoolconfig libp3interrogatedb)
target_compile_definitions(scripting PRIVATE -DBUILDING_SCRIPTING)
add_custom_command(
TARGET scripting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different scripting.dll ${CMAKE_BINARY_DIR}/src/example/scripting.dll
)

# Generate Python code bindings with interrogate
add_custom_command(
    OUTPUT scripting_igate.cxx scripting_module.cxx
    COMMAND ${PANDA_DIR}/bin/interrogate -D__inline -DCPPPARSER -DP3_INTERROGATE=1 -D__cplusplus -fnames -string -refcount -assert
    -S${PANDA_DIR}/include/parser-inc -S${PANDA_DIR}/include -I${PANDA_DIR}/include
    -oc scripting_igate.cxx -od scripting.in -python-native ${PROJECT_SOURCE_DIR}/src/scripting/*.h -module scripting -library scripting
    
    COMMAND ${PANDA_DIR}/bin/interrogate_module -python-native -module scripting -library scripting -oc scripting_module.cxx scripting.in
)

# Build Python extension module to use in Python (not necessary)
# add_library (py_scripting SHARED config_scripting.cpp scriptManager.cpp scripting_igate.cxx scripting_module.cxx)
# target_compile_definitions(py_scripting PRIVATE -DBUILDING_SCRIPTING)
# target_link_libraries(py_scripting libpanda libp3framework libpandaexpress libp3dtool libp3dtoolconfig libp3interrogatedb libp3pystub libp3direct)

# Copy resources to build folder
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/res/ DESTINATION ${CMAKE_BINARY_DIR}/src/example/res
)

# Copy config file
install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/etc/ DESTINATION ${CMAKE_BINARY_DIR}/src/example/etc
)
