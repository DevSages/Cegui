#ifndef __H_LOGSYSTEM_H__
#define __H_LOGSYSTEM_H__
#include <stdarg.h>
#include "Thread.h"
#include "FileHandler.h"


class LOG
{
public:
	enum LOG_TYPE
	{
		eLOG_TYPE_COMMON = 0,
		eLOG_TYPE_WARNING,
		eLOG_TYPE_ERROR,
	};
public:
	LOG();

	void			WriteLog( int logType, const char* szDebugFileName, unsigned LineNum, const char* szFormat, ... );

private:
	Mutex			m_MutexTime;
	Mutex			m_MutexFile;
	static char		m_FileDirectory[PATH_MAX];
};

extern LOG		g_Log;

#define Log_common( format, ... ) \
	g_Log.WriteLog( LOG::eLOG_TYPE_COMMON, __FILE__, __LINE__, format, ##__VA_ARGS__ )

#define Log_error( format, ... ) \
	g_Log.WriteLog( LOG::eLOG_TYPE_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__ )

#define Log_warning( format, ... ) \
	g_Log.WriteLog( LOG::eLOG_TYPE_WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__ )




#endif // __H_LOGSYSTEM_H__