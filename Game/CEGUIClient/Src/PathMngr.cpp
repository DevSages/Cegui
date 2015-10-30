#include "PathMngr.h"
#include "Common.h"
#include <Windows.h>

const char* GetResourcePath()
{

}

const char* GetExePath()
{
	TCHAR szPath[PATH_MAX];
	GetModuleFileName( NULL, szPath, PATH_MAX );
}

const char* GetTablePath()
{

}