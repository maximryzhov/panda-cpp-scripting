#pragma once
#include <python.h>

#include "config_scripting.h"

using namespace std;

/*
	A wrapper class around generic PyObject
	Decreases references on destruction
	TODO: use Panda3D TypedObject ?
*/
class EXPCL_SCRIPTING PyObj {
public:
	PyObj() {
		_pyref = nullptr;
		scripting_cat.spam() << "Creating empty PyObj\n";
	};
	PyObj(std::string name, PyObject* pyObject) {
		scripting_cat.spam() << "Creating PyObj " << name << "\n";
		_pyref = pyObject;
		_name = name;
	};
	~PyObj() {
		if (_pyref != nullptr) {
			scripting_cat.spam() << "DECREF PyObject " << _name << "\n";
			Py_DECREF(_pyref);
		}
	};
	bool operator!() {
		return _pyref == nullptr;
	};
	std::string getName() {
		return _name;
	}
protected:
	std::string _name; // string to identify
	PyObject* _pyref; // a reference to the PyObject in memory
};

class EXPCL_SCRIPTING PyInstance : public PyObj {
private:
	std::string _class_name;
public:
	PyInstance() : PyObj() {};
	PyInstance(std::string name, std::string class_name, PyObject* code) : PyObj(name, code) {
		_class_name = class_name;
	};
	std::string PyInstance::getClassName() {
		return _class_name;
	}
};


class EXPCL_SCRIPTING PyClass : public PyObj {
public:
	PyClass() : PyObj() {};
	PyClass(std::string name, PyObject* code) : PyObj(name, code) {};

	// Try to create a class instance
	// TODO: construct init args
	PyInstance makeInstance(std::string name);
};


class EXPCL_SCRIPTING PyModule : public PyObj {
public:
	PyModule() : PyObj() {};

	PyModule(std::string name, PyObject* code) : PyObj(name, code) {};

	// Get a raw PyObject pointer to module's global var
	PyObject* getVar(std::string name);

	// Try to get the class object from the Python code
	// Class name should be the same as the file name
	PyClass getClass();


	// Use this function to inject a reference to a typed Panda3D object
	// from the C++ code into the Python module as a global module variable
	template <typename T>
	void setPandaVar(void* pandaObject, std::string name) {
		T* typedPandaObject = (T*)pandaObject;
		PyObject* py_panda_object = DTool_CreatePyInstance(typedPandaObject, true);

		PyObject* module_dict = PyModule_GetDict(_pyref);
		int result = PyDict_SetItemString(module_dict, name.c_str(), py_panda_object);
		if (result < 0) {
			PyErr_Print();
			scripting_cat.error() << "Error setting Panda variable " << name << endl;
		}
	}
};


class EXPCL_SCRIPTING PyCode : public PyObj {
private:
public:
	PyCode() : PyObj() {};
	PyCode(std::string name, PyObject* code) : PyObj(name, code) {};

	// Loads module into memory and evaluates the code
	PyModule makeModule();
};
