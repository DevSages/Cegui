#ifndef __H_LOGSYSTEM_H__
#define __H_LOGSYSTEM_H__
#include <stdarg.h>
#include "FileHandler.h"

enum eLog_Lv
{
	LOG_LV_COMMON = 0,
	LOG_LV_CONSOLE = 1,
	LOG_LV_WRITE = 2,
	LOG_LV_ALL = 3
};

class LOG
{
public:
	static void	Write( const char* szFileName, eLog_Lv lv = LOG_LV_COMMON );
};




#endif // __H_LOGSYSTEM_H__