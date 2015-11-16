#include "IniFileHandler.h"
#include <assert.h>

IniFiles::IniFiles():
	m_bReady( false )
{

}

IniFiles::IniFiles( const char* szFileName ):
	m_bReady( false )
{
	if( !LoadFile( szFileName ) )
		RETURN_EMPTY;
}


bool IniFiles::LoadFile( const char* szFileName )
{
	m_bReady =  m_File.OpenFile( szFileName );
	if( !m_bReady )
		RETURN_FALSE;

	if( !m_File.GetLineContent( m_Lines ) )
		RETURN_FALSE;

	for( size_t i = 0; i < m_Lines.size(); i++ )
	{
		ParseLine( m_Lines[i], i );
	}
	RETURN_TRUE;
}

void IniFiles::ParseLine( string& strLine, int iLineNum )
{
	if( strLine.empty() )
		return;

	static string strCurSection;
	string strLineTmp( strLine );
	StringTools::RemoveSpace( strLineTmp );
	StringTools::RemoveRN( strLineTmp );

	if( strLineTmp.empty() )
	{
		if( !strCurSection.empty() )
		{
			UseableLinesIter iter = m_UseableLines.find( strCurSection );
			Useable u;
			u.m_LineNum = iLineNum;
			if( iter != m_UseableLines.end() )
			{
				UseableLine& useable = iter->second;
				useable.push_back( u );
			}
			else
			{
				UseableLine useable;
				useable.push_back( u );
				m_UseableLines.insert( make_pair( strCurSection, useable ) );
			}
		}
		return;
	}

	if( strLineTmp[0] == '[' )
	{
		string::size_type rightBarce = strLineTmp.find_first_of( ']' );
		if( rightBarce == std::string::npos )
		{
			return;
		}
		if( !strCurSection.empty() )
		{
			Useable u;
			u.m_LineNum = iLineNum;
			u.m_bEndLineNum = true;
			m_UseableLines[strCurSection].push_back( u );
		}
		strCurSection = strLineTmp.substr( 1, rightBarce-1 );
		KeyValue keyValue;
		keyValue.insert( make_pair( strCurSection, "" ) );
		m_Section.insert( make_pair( strCurSection, keyValue ) );
	}
	else
	{
		string::size_type equalPos = strLineTmp.find_first_of( '=' );
		if( equalPos == std::string::npos )
		{
			return;
		}

		string strkey, strValue;
		strkey = strLineTmp.substr( 0, equalPos );
		strValue = strLineTmp.substr( equalPos + 1 );

		SectionIter secIter = m_Section.find( strCurSection );
		if( secIter != m_Section.end() )
		{
			KeyValue& keyValue = secIter->second;
			keyValue.insert( make_pair( strkey, strValue ) );
		}
		else
		{
			KeyValue keyValue;
			keyValue.insert( make_pair( strkey, strValue ) );
			m_Section.insert( make_pair( strCurSection, keyValue ) );
		}
	}
}

void IniFiles::WriteInteger( const char* szSec, const char* szKey, int value )
{
	if( szSec == NULL || szKey == NULL || value == NULL )
		return;

	char szBuf[COMMON_CHAR_BUF_64] = {0};
	sprintf_s( szBuf, "%d", value );
	if( strlen( szBuf ) > 0 )
	{
		WriteValue( szSec, szKey, szBuf );
	}
}

void IniFiles::WriteFloat( const char* szSec, const char* szKey, float value )
{
	if( szSec == NULL || szKey == NULL || value == NULL )
		return;

	char szBuf[COMMON_CHAR_BUF_64] = {0};
	sprintf_s( szBuf, "%f", value );
	if( strlen( szBuf ) > 0  )
	{
		WriteValue( szSec, szKey, szBuf );
	}
}

void IniFiles::WriteString( const char* szSec, const char* szKey, const char* value )
{
	if( szSec == NULL || szKey == NULL || value == NULL )
		return;

	WriteValue( szSec, szKey, value );
}

bool IniFiles::GetBool( const char* szSec, const char* szKey, bool defValue )
{
	const char* szValue = GetValue( szSec, szKey );
	if( szValue == NULL )
	{
		return defValue;
	}

	int value = atoi( szValue );

	return value == 0 ? false : true;
}

int IniFiles::GetInteger( const char* szSec, const char* szKey, int defValue )
{
	const char* szValue = GetValue( szSec, szKey );
	if( szValue == NULL )
	{
		return defValue;
	}

	return atoi( szValue );
}

float IniFiles::GetFloat( const char* szSec, const char* szKey, float defValue )
{
	const char* szValue = GetValue( szSec, szKey );
	if( szValue == NULL )
	{
		return defValue;
	}

	return (float)atof( szValue );
}

const char* IniFiles::GetString( const char* szSec, const char* szKey, std::string defValue )
{
	const char* szValue = GetValue( szSec, szKey );
	if( szValue == NULL )
	{
		return defValue.c_str();
	}

	return szValue;
}



void IniFiles::WriteValue( const char* szSec, const char* szKey, const char* value )
{
	if( szSec == NULL || szKey == NULL || value == NULL )
		return;

	SectionIter iter = m_Section.find( szSec );

	if( iter != m_Section.end() )
	{
		KeyValue& keyValue = iter->second;
		KeyValueIter it = keyValue.find( szKey );
		if( it != keyValue.end() )
		{
			const string& key = it->first;
			if( strcmp( key.c_str(), szSec ) == 0 )
			{
				UseableLinesIter usableIter = m_UseableLines.find( szSec );
				if( usableIter != m_UseableLines.end() )
				{

				}
				else
				{
					string strConten( szKey );
					strConten += "=";
					strConten += value;
					strConten += "\r\n";
					m_Lines.push_back( strConten );
				}
			}
		}
		else
		{

		}
	}
	else
	{

	}
}

const char* IniFiles::GetValue( const char* szSec, const char* szKey )
{
	SectionIter it = m_Section.find( szSec );
	if( it == m_Section.end() )
	{
		return NULL;
	}

	KeyValue& keyValue = it->second;
	KeyValueIter keyIter = keyValue.find( szKey );

	if( keyIter == keyValue.end() )
	{
		return NULL;
	}

	string& value = keyIter->second;

	return value.c_str();
}