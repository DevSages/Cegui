#ifndef __H_STRINGTOOLS_H__
#define __H_STRINGTOOLS_H__
#include <string>
#include <stdarg.h>
using std::string;



class StringTools
{
public:
	static void		RemoveSpace( string& strString );

	static void		RemoveRN( string& strString );

	static bool		Format( string& strString, const char* szFormat, ... );
};






#endif // __H_STRINGTOOLS_H__