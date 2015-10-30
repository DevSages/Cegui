#include "PathMngr.h"
#include "ErrorMsg.h"
#include <Windows.h>


TCHAR PathMngr::m_szResourceFolderName[COMMON_CHAR_BUF_64] = TEXT( "Resources" );
TCHAR PathMngr::m_szTableFolderName[COMMON_CHAR_BUF_64] = TEXT( "Tables" );
TCHAR PathMngr::m_szScriptFolderName[COMMON_CHAR_BUF_64] = TEXT( "Scripts" );


PathMngr::PathMngr()
{
	memset( m_szExePath, 0, PATH_MAX );
}

PathMngr& PathMngr::GetSingleton()
{
	static PathMngr _singleton;
	return _singleton;
}


const TCHAR* PathMngr::GetExePath()
{
	if( _tcslen( m_szExePath ) != 0 )
		return m_szExePath;

	DWORD dwLen = GetModuleFileName( NULL, m_szExePath, PATH_MAX );
	if( 0 == dwLen )
	{
		ErrorMsg( "GetExePath()Failed!-ErrorCode = %d", GetLastError() );
		return NULL;
	}
	TCHAR* szFirstAddr = NULL;
	szFirstAddr = _tcsstr( m_szExePath, _TEXT( "CEGUIClient.exe" ) );
	if( NULL != szFirstAddr )
	{
		*(--szFirstAddr) = 0;
	}
	else
	{
		return NULL;
	}
	return m_szExePath;
}

const TCHAR* PathMngr::GetResourcePath()
{
	const TCHAR* szExePath = GetExePath();
	if( NULL != szExePath )
	{
		static TCHAR szResPath[PATH_MAX] = {0};
		_stprintf_s( szResPath, _TEXT("%s/%s"), szExePath, m_szResourceFolderName );
		return szResPath;
	}

	return NULL;
}

const TCHAR* PathMngr::GetTablePath()
{
	const TCHAR* szExePath = GetExePath();
	if( NULL != szExePath )
	{
		static TCHAR szTablePath[PATH_MAX] = {0};
		_stprintf_s( szTablePath, _TEXT("%s/%s"), szExePath, m_szTableFolderName );
		return szTablePath;
	}

	return NULL;
}


const TCHAR* PathMngr::GetScriptPath()
{
	const TCHAR* szExePath = GetExePath();
	if( NULL != szExePath )
	{
		static TCHAR szScriptPath[PATH_MAX] = {0};
		_stprintf_s( szScriptPath, _TEXT("%s/%s"), szExePath, m_szScriptFolderName );
		return szScriptPath;
	}

	return NULL;
}