#ifndef __H_ERRORMSG_H__
#define __H_ERRORMSG_H__
#include "Common.h"
#include <stdarg.h>

//#define ErrorPrintf( szMsg, ... ) printf( "File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__ )

extern void printfWnd( const char* szMsg, ... );




#ifdef WIN32
	#if defined( _CONSOLE )
		#define ErrorMsg	printf
	#else
		#define ErrorMsg	printfWnd
	#endif
#endif


#ifdef _DEBUG
//void MyLog(char* format, ...)
//{
//	//char szBuffer[10240] = {0};
//	//FILE* fLog = fopen("MyLog.log", "a+");
//	//if ( fLog )
//	//{
//	//	va_list args;
//	//	int     len;
//	//	va_start( args, format );
//	//	len = _vscprintf( format, args ) + 1;
//	//	if (len > 800)
//	//	{
//	//		args[800] = '\0';
//	//	}
//	//	vsprintf( szBuffer, format, args ); 
//
//	//	fseek(fLog, 0, SEEK_END);
//	//	fwrite(szBuffer, 1, strlen(szBuffer), fLog);
//	//	fclose(fLog);
//	//	fLog = NULL;
//	//}
//}
#endif








#endif // __H_ERRORMSG_H__