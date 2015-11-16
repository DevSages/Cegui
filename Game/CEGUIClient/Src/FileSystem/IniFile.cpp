#include "IniFile.h"
#include "PathMngr.h"
#include <assert.h>
#include "FileHandler.h"
#include "IniFileHandler.h"



IniFile::IniFile()
{
	Reset();
}

IniFile::IniFile( const char* szFileName )
{
	
	//assert( LoadFile( szFileName ) );
	std::string strExePath = PathMngr::GetSingleton().GetExePath();
	sprintf_s( m_szFileName, "%s/%s", strExePath.c_str(), szFileName );

	IniFiles ini( m_szFileName );
	const char* a  = ini.GetString( "root", "lihuihui" );
	int b=  1;
	ini.WriteFloat( "hello", "hello", 3.5 );
	int asf= 1;
}

IniFile::~IniFile()
{
	Save();
}

bool IniFile::OpenFile( const char* szFileName )
{
	return LoadFile( szFileName );
}

bool IniFile::LoadFile( const char* szFileName )
{
	Reset();

	std::string strExePath = PathMngr::GetSingleton().GetExePath();
	if( strExePath.empty() )
	{
		return false;
	}
	sprintf_s( m_szFileName, "%s/%s", strExePath.c_str(), szFileName );

	FILE* fp;
	errno_t noerr = fopen_s( &fp, m_szFileName, "rb" );
	if( fp == NULL || noerr != 0 )
	{
		return false;
	}

	fseek( fp, ZERO, SEEK_END );
	int size = ftell( fp );
	fseek( fp, ZERO, SEEK_SET );
	if( size == ZERO )
	{
		fclose( fp );
		return true;
	}

	m_Lines.clear();
	int iLineIndex = 0;
	char szLineBuf[COMMON_CHAR_BUF_128];

	while( !IsEndOfFile( fp ) )
	{
		char* szContent = NULL;
		std::string strLine;
		do
		{
			szContent = fgets( szLineBuf, COMMON_CHAR_BUF_128, fp );
			if( szContent != NULL )
			{
				strLine += szContent;
			}
		}while( !IsNextLine( szLineBuf[strlen(szLineBuf)-1] ) && !IsEndOfFile( fp ) && szContent != NULL );

		ParseLine( strLine.c_str(), iLineIndex++ );
	}

	fclose( fp );
	return true;
}

bool IniFile::Save()
{
	if( m_Lines.empty() )
		return false;

	FILE* fp;
	errno_t noerr = fopen_s( &fp, m_szFileName, "wb+" );
	if( fp == NULL || noerr != 0 )
	{
		return false;
	}

	std::string strText = "";
	for( LinesIter it = m_Lines.begin(); it != m_Lines.end(); ++it )
	{
		strText += it->content;
	}
	fwrite( strText.c_str(), strText.length(), 1, fp );
	fflush( fp );
	fclose( fp );

	return true;
}


void IniFile::WriteInteger( const char* szSec, const char* szKey, int value )
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

void IniFile::WriteFloat( const char* szSec, const char* szKey, float value )
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

void IniFile::WriteString( const char* szSec, const char* szKey, const char* value )
{
	if( szSec == NULL || szKey == NULL || value == NULL )
		return;

	WriteValue( szSec, szKey, value );
}

bool IniFile::GetBool( const char* szSec, const char* szKey, bool defValue )
{
	const char* szValue = GetValue( szSec, szKey );
	if( szValue == NULL )
	{
		return defValue;
	}

	int value = atoi( szValue );

	return value == 0 ? false : true;
}

int IniFile::GetInteger( const char* szSec, const char* szKey, int defValue )
{
	const char* szValue = GetValue( szSec, szKey );
	if( szValue == NULL )
	{
		return defValue;
	}

	return atoi( szValue );
}

float IniFile::GetFloat( const char* szSec, const char* szKey, float defValue )
{
	const char* szValue = GetValue( szSec, szKey );
	if( szValue == NULL )
	{
		return defValue;
	}

	return (float)atof( szValue );
}

const char* IniFile::GetString( const char* szSec, const char* szKey, std::string defValue )
{
	const char* szValue = GetValue( szSec, szKey );
	if( szValue == NULL )
	{
		return defValue.c_str();
	}

	return szValue;
}

bool IniFile::IsNextLine( const char c )
{
	return ( c == '\n' || c == '\r' );
}

bool IniFile::IsEndOfFile( FILE* fp )
{
	if( fp != NULL )
	{
		return ( feof(fp) != ZERO || ferror(fp) != ZERO );
	}
	return true;
}

void IniFile::ParseLine( std::string strLine, int iLineIndex )
{
	static std::string strSection = "";

	Line line;
	line.content = strLine;
	line.writeadle = false;
	line.section = strSection;
	line.lineIndex = iLineIndex;

	std::string::size_type flagPos = strLine.find_first_of( '\r' );
	if( flagPos != std::string::npos )
	{
		strLine.erase( flagPos );
	}
	else
	{
		flagPos = strLine.find_first_of( '\n' );
		if( flagPos != std::string::npos )
		{
			strLine.erase( flagPos );
		}
	}

	if( strLine.empty() )
	{
		line.writeadle = true;
		m_Lines.push_back( line );
		return;
	}

	std::string::size_type leftBarce = strLine.find_first_of( '[' );
	if( leftBarce != std::string::npos )
	{
		std::string::size_type rightBarce = strLine.find_first_of( ']' );
		if( rightBarce == std::string::npos )
		{
			m_Lines.push_back( line );
			return;
		}
		strSection = strLine.substr( leftBarce+1, rightBarce - leftBarce -1 );
		line.section = "";
	}
	else
	{
		line.section = strSection;
		flagPos = strLine.find_first_of( '=' );
		if( flagPos == std::string::npos )
		{
			m_Lines.push_back( line );
			return;
		}

		strLine.erase( strLine.find_last_not_of( ' ' ) + 1 );
		strLine.erase( ZERO, strLine.find_first_not_of( ' ' ) );
		flagPos = strLine.find_first_of( '=' );

		std::string key;
		Value _value;
		_value.lineIndex = iLineIndex;

		key		= strLine.substr( ZERO, flagPos );
		std::string::size_type notePos =  strLine.find_first_of( '#' );
		notePos != std::string::npos ? _value.value = strLine.substr( flagPos + 1, notePos - flagPos - 1 ) : _value.value = strLine.substr( flagPos + 1 );

		SectionIter it = m_Sections.find( strSection );
		if( it != m_Sections.end() )
		{
			KeyValues& keyValuse = it->second;
			std::pair< std::map< std::string,Value >::iterator,bool > ret = keyValuse.insert( std::make_pair( key, _value ) );
			if( !ret.second )
			{
			}
		}
		else
		{
			KeyValues keyValues;
			std::pair< std::map< std::string,Value >::iterator,bool > ret = keyValues.insert( std::make_pair( key, _value ) );
			if( !ret.second )
			{
			}
			std::pair< std::map< std::string, KeyValues >::iterator,bool > ret2 = m_Sections.insert( std::make_pair( strSection, keyValues ) );
			if( !ret2.second )
			{
			}
		}
	}
	m_Lines.push_back( line );
}

void IniFile::Reset()
{
	m_Lines.clear();
	m_Sections.clear();
	memset( m_szFileName, 0, COMMON_CHAR_BUF_128 );
}


void IniFile::WriteValue( const char* szSec, const char* szKey, const char* value )
{
	if( szSec == NULL || szKey == NULL || value == NULL )
		return;

	Line line;
	line.content += szKey;
	line.content += '=';
	line.content += value;
	line.section = szSec;
	line.writeadle= false;
	size_t lineSize = m_Lines.size();

	SectionIter it = m_Sections.find( szSec );
	if( it != m_Sections.end() )
	{
		KeyValues& keyValue = it->second;
		KeyValueIter keyValueIter = keyValue.find( szKey );
		if( keyValueIter != keyValue.end() )
		{
			Value& _value = keyValueIter->second;
			if( strcmp( _value.value.c_str(), value ) == ZERO )
				return;

			_value.value = value;
			line.lineIndex = _value.lineIndex;
			line.content += "\r\n";
			if( line.lineIndex >= lineSize )
			{
				return;
			}
			m_Lines[line.lineIndex] = line;
		}
		else
		{
			Value _value;
			_value.value = value;
			int iNoFindValue = 0;
			int iValidLineIndex = GetValidLineIndex( szSec, iNoFindValue );
			if( iValidLineIndex == INVALID_VALUE )
			{
				line.lineIndex = iNoFindValue;
				if( line.lineIndex != lineSize )
				{
					line.content += "\r\n";
				}
				m_Lines.insert( m_Lines.begin()+iNoFindValue, line );
				_value.lineIndex = iNoFindValue;
			}
			else
			{
				line.lineIndex = iValidLineIndex;
				line.content += "\r\n";

				if( line.lineIndex >= lineSize )
				{
					return;
				}
				m_Lines[line.lineIndex] = line;
				_value.lineIndex = iValidLineIndex;
			}
			keyValue[szKey] = _value;
		}
		m_Sections[szSec] = keyValue;
	}
	else
	{
		if( lineSize > ZERO )
		{
			m_Lines[lineSize-1].content += '\n';
		}

		Line lineSection;
		lineSection.content;
		lineSection.content += '[';
		lineSection.content += szSec;
		lineSection.content += ']';
		lineSection.content += '\n';
		lineSection.section = "";
		lineSection.writeadle= false;
		lineSection.lineIndex = m_Lines.size();
		m_Lines.push_back( lineSection );

		line.lineIndex = lineSection.lineIndex+1;
		m_Lines.push_back( line );
	}
}

const char* IniFile::GetValue( const char* szSec, const char* szKey )
{
	SectionIter it = m_Sections.find( szSec );
	if( it == m_Sections.end() )
	{
		return NULL;
	}

	const KeyValues& keyValue = it->second;
	CKeyValueIter keyIter = keyValue.find( szKey );

	if( keyIter == keyValue.end() )
	{
		return NULL;
	}

	const Value& _value = keyIter->second;

	return _value.value.c_str();
}


int IniFile::GetValidLineIndex( const char* szSec, int& iNoFindValue )
{
	iNoFindValue = 0;
	for( CLinesIter it = m_Lines.begin(); it != m_Lines.end(); it++ )
	{
		if( strcmp( it->section.c_str(), szSec ) == ZERO )
		{
			if( it->writeadle )
				return it->lineIndex;
			else
				iNoFindValue = it->lineIndex;
		}
	}
	return INVALID_VALUE;
}