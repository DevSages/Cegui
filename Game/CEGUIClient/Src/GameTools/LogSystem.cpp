#include "LogSystem.h"
#include <time.h>
#include "StringTools.h"
#include "PathMngr.h"
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")


LOG		g_Log;

char LOG::m_FileDirectory[PATH_MAX] = {0};


LOG::LOG()
{
	string strFindFile;
	StringTools::Format( strFindFile, "%s/GameLog", PathMngr::GetSingleton().GetExePath() );
	if( !PathIsDirectory( strFindFile.c_str() ) )
	{
		::CreateDirectory( strFindFile.c_str(), NULL );
	}
	strncpy_s( m_FileDirectory, strFindFile.c_str(), PATH_MAX );
}

void LOG::WriteLog( int logType, const char* szDebugFileName, unsigned LineNum, const char* szFormat, ... )
{
	tm _tm;
	time_t timeInfo = time( NULL );
	errno_t error = localtime_s( &_tm, &timeInfo );

	if( 0 != error )
		return;

	static int _day = -1;

	static string strTime;

	m_MutexTime.Lock();
	if( _day != _tm.tm_mday )
	{
		StringTools::Format( strTime, "%d_%d_%d", _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday );
		_day = _tm.tm_mday;
	}
	m_MutexTime.Unlock();

	va_list argptr = NULL;
	va_start(argptr, szFormat);
	int bufsize = _vscprintf(szFormat, argptr) + 2;

	char* szBuf = new char[bufsize];
	vsprintf_s( szBuf, bufsize, szFormat, argptr);
	va_end(argptr);

	string strFileName;
	string strContent;
	if( logType != eLOG_TYPE_COMMON )
	{
		string subFileName( szDebugFileName );
		string::size_type equalPos = subFileName.find_last_of( '\\' );
		if( equalPos != string::npos )
		{
			subFileName.erase( 0, equalPos + 1 );
		}
		StringTools::Format( strContent, "[file : %s][line : %u] : %s", subFileName.c_str(), LineNum, szBuf );
		if( logType == eLOG_TYPE_WARNING )
		{
			StringTools::Format( strFileName, "%s/%s%s%s", m_FileDirectory, "warning_", strTime.c_str(), ".log" );
		}
		else if( logType == eLOG_TYPE_ERROR )
		{
			StringTools::Format( strFileName, "%s/%s%s%s", m_FileDirectory, "error_", strTime.c_str(), ".log" );
		}
	}
	else
	{
		StringTools::Format( strFileName, "%s/%s%s%s", m_FileDirectory, "common_", strTime.c_str(), ".log" );
		strContent = szBuf;
	}

	static FileHandler fileHandler;
	m_MutexFile.Lock();
	fileHandler.OpenFile( strFileName.c_str(), "ab+" );
	strContent += "\r\n";
	fileHandler.WriteFile( strContent.c_str(), strContent.size() );
	fileHandler.Close();
	m_MutexFile.Unlock();

#if (defined( __WIN32__ ) || defined( _WIN32 )) && defined( _DEBUG )
	#ifdef _CONSOLE
	printf( strContent.c_str() );
	#endif
#endif

	SAFE_DELETE_ARRAY( szBuf );
}