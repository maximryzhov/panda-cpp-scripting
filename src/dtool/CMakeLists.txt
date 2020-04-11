include_directories(
	${PROJECT_SOURCE_DIR}/src/scripting 
	${PROJECT_SOURCE_DIR}/src/dtool 
	${PANDA_DIR}/include 
	${PYTHON_DIR}/include
)
link_directories( ${PYTHON_DIR}/libs)
add_library(py_panda py_panda.cxx py_wrappers.cxx dtool_super_base.cxx py_compat.cxx )