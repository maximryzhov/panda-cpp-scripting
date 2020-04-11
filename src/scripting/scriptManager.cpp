#include <Python.h>

#include <filename.h>
#include <stringStream.h>

#include "scriptManager.h"


bool ScriptManager::start(const std::string pythonHome) {
	Filename pyHomeFN = Filename(pythonHome);
	scripting_cat.info() << "Setting PYTHONHOME to " << pythonHome << endl;

	if (!pyHomeFN.exists()) {
		scripting_cat.error() << "Path doesn't exist: " << pythonHome << endl;
		return FALSE;
	}

	if (!pyHomeFN.is_directory()) {
		scripting_cat.error() << "Not a directory: " << pythonHome << endl;
		return FALSE;
	}

	std::wstring pythonHome_w = pyHomeFN.get_fullpath_w();

	// It is impossible to catch any Python C API fatal errors
	// If this call fails (e.g. Python directory is not found) the program will crash
	// TODO: try running interpreter from another process
	Py_SetPythonHome(pythonHome_w.c_str());

	if (Py_IsInitialized()) {
		scripting_cat.warning()
			<< "Python interpreter already started, shutting down\n";
		Py_FinalizeEx();
	}
	Py_InitializeEx(0);

	scripting_cat.info()
		<< "Started Python interpreter\n"
		<< "Python version: " << Py_GetVersion() << endl;

	return TRUE;
};


ScriptManager::~ScriptManager() {
	Py_FinalizeEx();
}

PyCode ScriptManager::loadScript(std::string path)
{
	// Return empty object in case of failure
	PyCode nocode = PyCode();

  // Perform some checks before loading the file

	Filename fn = Filename(path);

	if (!fn.exists()) {
		scripting_cat.error() << "File doesn't exist: " << path << endl;
		return nocode;
	}

	FileStream fs(path.c_str());
	if (fs.fail()) {
		scripting_cat.error() << "Can't access file: " << path << endl;
		return nocode;
	}

	if (fn.get_extension() != "py") {
		scripting_cat.error() 
			<< "File has wrong extension: " << fn.get_extension() << endl;
		return nocode;
	}

	// Load file contents
	StringStream ss;
	ss << fs.rdbuf();
	std::string source = ss.get_data();
	scripting_cat.debug() << "Loaded script: " << path << endl;

	// Create python compiled code
	PyObject* py_code = Py_CompileString(source.c_str(), "", Py_file_input);
	if (!py_code) {
		scripting_cat.error() << "Failed to compile python code\n";
		PyErr_Print();
		return nocode;
	}

	std::string moduleName = fn.get_basename_wo_extension();
	PyCode code = PyCode(moduleName, py_code);
	Py_INCREF(py_code);
	return code;
}