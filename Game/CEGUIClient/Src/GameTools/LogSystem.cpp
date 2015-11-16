#include "LogSystem.h"



void LOG::Write( const char* szFileName, eLog_Lv lv )
{
	if( lv & LOG_LV_WRITE )
	{
		FileHandler file;
		if( file.OpenFile( szFileName ) )
		{

		}
	}

	if( lv & LOG_LV_CONSOLE )
	{

	}




}