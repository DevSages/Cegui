#ifndef __H_PATHMNGR_H__
#define __H_PATHMNGR_H__
#include "Common.h"
#include "ConstDef.h"
#include <tchar.h>



class PathMngr
{
public:
	PathMngr();
public:
	static PathMngr&	GetSingleton();
public:
	const TCHAR*		GetExePath();

	const TCHAR*		GetResourcePath();

	const TCHAR*		GetTablePath();

	const TCHAR*		GetScriptPath();

private:
	TCHAR				m_szExePath[PATH_MAX];
	static TCHAR		m_szResourceFolderName[COMMON_CHAR_BUF_64];
	static TCHAR		m_szTableFolderName[COMMON_CHAR_BUF_64];
	static TCHAR		m_szScriptFolderName[COMMON_CHAR_BUF_64];
};






#endif // __H_PATHMNGR_H__