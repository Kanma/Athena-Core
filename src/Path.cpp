/**	@file	Path.h
	@author	Philip Abbet

	Implementation of the class 'Athena::Utils::Path'
*/

#include <Athena-Core/Path.h>

#if ATHENA_CORE_PLATFORM == ATHENA_CORE_PLATFORM_WIN32
	#include <direct.h>
	#include <stdlib.h>
#else
    #include <unistd.h>
#endif


using namespace Athena::Utils;
using namespace std;


std::string Path::getCurrentWorkingDirectory()
{
	std::string currentWorkingDirectory = "";

#if ATHENA_CORE_PLATFORM == ATHENA_CORE_PLATFORM_WIN32
	char currentPath[_MAX_PATH];
	getcwd(currentPath, _MAX_PATH);
#else
	char currentPath[256];
	getcwd(currentPath, 256);
#endif

	currentWorkingDirectory = currentPath;

	return currentWorkingDirectory;
}
