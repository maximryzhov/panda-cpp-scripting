#pragma once
#include <dtool_config.h>
#undef DO_MEMORY_USAGE

#ifdef BUILDING_SCRIPTING
	#define EXPCL_SCRIPTING EXPORT_CLASS
	#define EXPTP_SCRIPTING EXPORT_TEMPL
#else
	#define EXPCL_SCRIPTING IMPORT_CLASS
	#define EXPTP_SCRIPTING IMPORT_TEMPL
#endif // BUILDING_SCRIPTING