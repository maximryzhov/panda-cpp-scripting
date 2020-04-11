cmake_minimum_required (VERSION 3.15.1)

project ("panda-scripting")
SET(PANDA_DIR "E:/Projects/Dev/panda-source/built_x64" CACHE FILEPATH "Path to full Panda3D SDK")
SET(PYTHON_DIR "${PANDA_DIR}/python" CACHE FILEPATH "Path to Panda3D Python directory")

add_subdirectory ("src/dtool")
add_subdirectory ("src/scripting")
add_subdirectory ("src/example")