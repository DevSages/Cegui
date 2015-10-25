#ifndef __H_COMMON_H__
#define __H_COMMON_H__

#include <stdio.h>

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


const int COMMON_CHAR_BUF_64	=	64;
const int COMMON_CHAR_BUF_128	=	128;
const int COMMON_CHAR_BUF_256	=	256;
const int COMMON_CHAR_BUF_512	=	512;









#endif // __H_COMMON_H__