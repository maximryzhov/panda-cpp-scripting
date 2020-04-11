#pragma once
#include <string>

#include "scriptingSymbols.h"
#include "py_panda.h"
#include "pyobjectWrapper.h"


class EXPCL_SCRIPTING ScriptManager
{
private:
	ScriptManager() {};
	ScriptManager(ScriptManager const&);
	void operator=(ScriptManager const&);
	
PUBLISHED:
	// TODO: Use Panda3D-style singletons
	INLINE static ScriptManager* getGlobalPtr() {
		static ScriptManager* _instance;
		if (!_instance) {
			_instance = new ScriptManager();
		}
		return _instance;
	}
	~ScriptManager();
	
	// Initializes Python interpreter
	bool start(std::string pythonHome);

	// Reads script file and returns PyCode object
	PyCode loadScript(const std::string path);
};