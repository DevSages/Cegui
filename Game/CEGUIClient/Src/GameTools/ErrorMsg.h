#ifndef __H_ERRORMSG_H__
#define __H_ERRORMSG_H__
#include <stdio.h>


//#define ErrorPrintf( szMsg, ... ) printf( "File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__ )

extern void printfWnd( const char* szMsg, ... );




#ifdef WIN32
	#if defined( _CONSOLE )
		#define ErrorMsg	printf
	#else
		#define ErrorMsg	printfWnd
	#endif
#endif








#endif // __H_ERRORMSG_H__