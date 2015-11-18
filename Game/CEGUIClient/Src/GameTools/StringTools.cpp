#include "StringTools.h"




void StringTools::RemoveSpace( string& strString )
{
	string::size_type pos = strString.find_last_not_of( " " );
	if( pos != string::npos )
		strString.erase( pos + 1 );
	pos = strString.find_first_not_of( " " );
	if( pos != string::npos )
		strString.erase( 0, pos );
}


void StringTools::RemoveRN( string& strString )
{
	string::size_type pos = strString.find_first_of( '\r' );
	if( pos != string::npos )
	{
		strString.erase( pos );
	}
	else
	{
		pos = strString.find_first_of( '\n' );
		if( pos != string::npos )
		{
			strString.erase( pos );
		}
	}
}

bool StringTools::Format( string& strString, const char* szFormat, ... )
{
	if( !strString.empty() )
		strString.clear();

	va_list argptr = NULL;
	va_start(argptr, szFormat);
	int bufsize = _vscprintf(szFormat, argptr) + 2;
	char* szBuf = new char[bufsize];
	vsprintf_s( szBuf, bufsize, szFormat, argptr);
	strString = szBuf;
	va_end(argptr);

	delete[] szBuf;

	if( strString.empty() )
		return false;

	return true;
}