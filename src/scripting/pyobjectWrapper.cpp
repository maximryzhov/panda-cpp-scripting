#include "pyobjectWrapper.h"

PyInstance PyClass::makeInstance(std::string name) {
	PyObject* init_args = PyTuple_New(0);
	PyObject* py_instance = PyObject_CallObject(_pyref, init_args);
	Py_DECREF(init_args);
	if (!py_instance) {
		scripting_cat.error() 
			<< "Error creating instance of " << _name << endl;
		PyErr_Print();
		return PyInstance();
	}
	PyInstance instance = PyInstance(name, _name, py_instance);
	return instance;
}

PyObject* PyModule::getVar(std::string name) {
	PyObject* py_module_dict = PyModule_GetDict(_pyref);
	PyObject* py_obj = PyDict_GetItemString(py_module_dict, name.c_str());
	Py_DECREF(py_module_dict);
	if (!py_obj) {
		scripting_cat.error()
			<< "Module " << _name << "doesn't have variable " << name << endl;
		PyErr_Print();
		return nullptr;
	}
	return py_obj;
}

PyClass PyModule::getClass() {
	PyObject* py_class = getVar(_name);
	if (!py_class) {
		scripting_cat.error()
			<< "Script must contain a class with the same name as the file name: " << _name << endl;
		return PyClass();
	}

	// Check if object is a class (works only for new-style classes)
	if (PyType_Check(py_class) != 1) {
		scripting_cat.error() << _name << " is not a class";
		Py_DECREF(py_class);
		return PyClass();
	}

	PyClass class_ = PyClass(_name, py_class);
	return class_;
}

PyModule PyCode::makeModule() {
	PyObject* py_module = PyImport_ExecCodeModule(_name.c_str(), _pyref);
	if (!py_module) {
		scripting_cat.error() << "Failed to import module " << _name << endl;
		PyErr_Print();
		return PyModule();
	}
	PyModule module_ = PyModule(_name, py_module);
	return module_;
}