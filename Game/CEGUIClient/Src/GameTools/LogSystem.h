#ifndef __H_LOGSYSTEM_H__
#define __H_LOGSYSTEM_H__
#include <stdarg.h>
#include "Thread.h"
#include "FileHandler.h"


class LOG
{
public:
	enum LogType
	{
		LOG_TYPE_FILE,
		LOG_TYPE_CONSOLE,
	};

	enum ErrorLv
	{
		ERROR_LV_COMMON,
		ERROR_LV_WARNING,
		ERROR_LV_ERROR,
	};
public:
	LOG( const char* szFileName )
	{
		strncpy_s( m_LogFileName, szFileName, PATH_MAX );
	}

	virtual void WriteLog( const char* szFormat, ... ) = 0;

private:
	Mutex		m_Mutex;
	char		m_LogFileName[PATH_MAX];
};




#endif // __H_LOGSYSTEM_H__