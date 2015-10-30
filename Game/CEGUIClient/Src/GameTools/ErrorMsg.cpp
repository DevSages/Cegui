#include "ErrorMsg.h"
#include "ConstDef.h"
#include "Common.h"
#include <stdarg.h>
#include <Windows.h>


void printfWnd( const char* szMsg, ... )
{
	static char szDest[COMMON_CHAR_BUF_512] = {0};
	va_list args; 
	va_start( args, szMsg );
	_vsnprintf_s( szDest, COMMON_CHAR_BUF_512, szMsg, args ); 
	va_end(args);

	MessageBoxA( NULL, "", szDest, MB_OK );
}