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