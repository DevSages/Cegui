#ifndef __H_COMMON_H__
#define __H_COMMON_H__

#include <stdio.h>
#include <stdlib.h>

#ifndef PATH_MAX
#   include <stdlib.h>
#   ifndef PATH_MAX
#       include <limits.h>
#   endif
#   ifndef PATH_MAX
#      ifdef _MAX_PATH
#           define PATH_MAX _MAX_PATH
#       else
#           define PATH_MAX 260
#       endif
#   endif
#endif



#if (defined( __WIN32__ ) || defined( _WIN32 )) && defined( _DEBUG )
	#ifdef _CONSOLE
	#endif
#else
	#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif



#define SAFE_DELETE( p ) { if( p != NULL ) delete p; p = NULL; }





#endif // __H_COMMON_H__