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


/*
*	����3����ֻ�����жϺ�����Ӧ��ʧ�ܵ������ʹ�ã�������Ӧ��ʹ���⼸���ꡣ
*/
#if (defined( __WIN32__ ) || defined( _WIN32 )) && defined( _DEBUG )
	#ifdef _CONSOLE
		#define RETURN_FALSE	{ printf( "file: %s\nline: %d\nfunction: %s\nEND!\n", __FILE__, __LINE__, __FUNCTION__ ); system("pause"); return false; }
		#define RETURN_TRUE		{ return true; }
		#define RETURN_EMPTY	{ printf( "file: %s\nline: %d\nfunction: %s\nEND!\n", __FILE__, __LINE__, __FUNCTION__ ); system("pause");				}
	#endif
#else
	#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
	#define RETURN_FALSE	{ return false;	}
	#define RETURN_TRUE		{ return true;	}
	#define RETURN_EMPTY	{				}
#endif



#define SAFE_DELETE( p ) { if( p != NULL ) delete p; p = NULL; }





#endif // __H_COMMON_H__