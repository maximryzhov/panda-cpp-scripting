#include "pandaFramework.h"
#include "pandaSystem.h"
#include "configVariable.h"

#include "scriptManager.h"


int main()
{
	// Open a new window framework
	PandaFramework framework;
	framework.open_framework();

	// Set the window title and open the window
	framework.set_window_title("Panda3D example: Run Python from C++");
	WindowFramework* window = framework.open_window();

	// Create script manager
	ScriptManager* scrMgr = ScriptManager::getGlobalPtr();
	std::string  pythonHome = ConfigVariableString("python-home");
	if (!scrMgr->start(pythonHome))
		exit(-1);

	// Get a handle to the scenegraph root
	NodePath render = window->get_render();
	
	// Load script
	PyCode pyCode = scrMgr->loadScript("res/TestScript.py");
	if (!pyCode)
		exit(-1);

	// Import script as module
	PyModule pyModule = pyCode.makeModule();
	if (!pyModule)
		exit(-1);

	// Pass the reference to the "render" NodePath in the Python module dictionary
	pyModule.setPandaVar<NodePath>(&render, "render");

	// Get the class from the module
	PyClass pyClass = pyModule.getClass();
	if (!pyClass)
		exit(-1);

	// Create an instance of a class
	PyInstance pyInstance = pyClass.makeInstance("new instance");
	if (!pyInstance)
		exit(-1);
	
	framework.main_loop();
	framework.close_framework();


	return 0;
}

