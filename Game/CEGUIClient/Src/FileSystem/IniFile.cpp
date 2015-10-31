#include "IniFile.h"
#include "PathMngr.h"
#include "ErrorMsg.h"
#include <assert.h>


#define Npos std::string::npos
#define Size_Type std::string::size_type


IniFile::IniFile():
	m_pFile( NULL )
{
	Reset();
}

IniFile::IniFile( const char* szFileName ):
	m_pFile( NULL )
{
	assert( LoadFile( szFileName ) );
}

IniFile::~IniFile()
{
	Save();
}

bool IniFile::LoadFile( const char* szFileName )
{
	Reset();

	std::string strExePath = UnicodeToANSI( PathMngr::GetSingleton().GetExePath() );
	if( strExePath.empty() )
	{
		ErrorMsg( "GetExePath() = NULL!" );
		return false;
	}
	sprintf_s( m_szFileName, "%s/%s", strExePath.c_str(), szFileName );

	errno_t noerr = fopen_s( &m_pFile, m_szFileName, "r" );
	if( m_pFile == NULL || noerr != 0 )
	{
		ErrorMsg( "m_pFile = NULL!" );
		return false;
	}

	fseek( m_pFile, 0, SEEK_END );
	long size = ftell( m_pFile );
	fseek( m_pFile, 0, SEEK_SET );
	if( size == ZERO )
	{
		ErrorMsg( "fileSize = ZERO!!" );
		return false;
	}

	m_Lines.clear();
	int iLineIndex = 0;
	char szLineBuf[5];

	while( !IsEndOfFile() )
	{
		char* szContent = NULL;
		std::string strLine;
		do
		{
			szContent = fgets( szLineBuf, 5, m_pFile );
			if( szContent != NULL )
			{
				strLine += szContent;
			}
		}while( !IsNextLine( szLineBuf[strlen(szLineBuf)-1] ) && !IsEndOfFile() && szContent != NULL );

		iLineIndex++;
		ParseLine( strLine.c_str(), iLineIndex );
	}
	return true;
}


void IniFile::WriteInteger( const char* szSec, const char* szKey, int value )
{
	if( szSec == NULL || szKey == NULL || value == NULL )
		return;

	char szBuf[COMMON_CHAR_BUF_64] = {0};
	sprintf_s( szBuf, "%d", value );
	if( strlen( szBuf ) > 0  )
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

int IniFile::GetInteger( const char* szSec, const char* szKey )
{
	return 0;
}

float IniFile::GetFloat( const char* szSec, const char* szKey )
{
	return 0.f;
}

const char* IniFile::GetString( const char* szSec, const char* szKey )
{
	return NULL;
}

bool IniFile::Save()
{
	if( m_pFile == NULL || m_Lines.empty() )
		return false;

	fclose( m_pFile );
	m_pFile = NULL;

	errno_t noerr = fopen_s( &m_pFile, m_szFileName, "w+" );
	if( m_pFile == NULL || noerr != 0 )
	{
		ErrorMsg( "m_pFile = NULL!" );
		return false;
	}

	std::string strText = "";
	for( LinesIter it = m_Lines.begin(); it != m_Lines.end(); ++it )
	{
		strText += it->content;
	}
	fwrite( strText.c_str(), strText.length(), 1, m_pFile );
	fclose( m_pFile );

	return true;
}

bool IniFile::IsNextLine( const char c )
{
	return ( c == '\n' || c == '\r' );
}

bool IniFile::IsEndOfFile()
{
	if( m_pFile != NULL )
	{
		return ( feof(m_pFile) != ZERO || ferror( m_pFile ) != ZERO );
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

	Size_Type flagPos = strLine.find_first_of( '\n' );
	if( flagPos != Npos )
	{
		strLine.erase( flagPos );
	}

	if( strLine.empty() )
	{
		line.writeadle = true;
		m_Lines.push_back( line );
		return;
	}

	Size_Type leftBarce = strLine.find_first_of( '[' );
	if( leftBarce != Npos )
	{
		Size_Type rightBarce = strLine.find_first_of( ']' );
		if( rightBarce == Npos )
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
		if( flagPos == Npos )
		{
			m_Lines.push_back( line );
			return;
		}

		strLine.erase( strLine.find_last_not_of( ' ' ) + 1 );
		strLine.erase( ZERO, strLine.find_first_not_of( ' ' ) );
		flagPos = strLine.find_first_of( '=' );

		// 可以有注释每行 #为注释标示
		std::string key;
		Value _value;
		_value.lineIndex = iLineIndex;

		key		= strLine.substr( ZERO, flagPos );
		Size_Type notePos =  strLine.find_first_of( '#' );
		notePos != Npos ? _value.value = strLine.substr( flagPos + 1, notePos - flagPos - 1 ) : _value.value = strLine.substr( flagPos + 1 );

		SectionIter it = m_Sections.find( strSection );
		if( it != m_Sections.end() )
		{
			KeyValues& keyValuse = it->second;
			std::pair< std::map< std::string,Value >::iterator,bool > ret = keyValuse.insert( std::make_pair( key, _value ) );
			if( !ret.second )
			{
				ErrorMsg( "Insert Key Failed, Key = %s", key.c_str() );
			}
		}
		else
		{
			KeyValues keyValues;
			std::pair< std::map< std::string,Value >::iterator,bool > ret = keyValues.insert( std::make_pair( key, _value ) );
			if( !ret.second )
			{
				ErrorMsg( "Insert Key Failed, Key = %s", key.c_str() );
			}
			std::pair< std::map< std::string, KeyValues >::iterator,bool > ret2 = m_Sections.insert( std::make_pair( strSection, keyValues ) );
			if( !ret2.second )
			{
				ErrorMsg( "Insert section Failed, section = %s", strSection.c_str() );
			}
		}
	}
	m_Lines.push_back( line );
}

void IniFile::Reset()
{
	m_pFile = NULL;
	m_Lines.clear();
	m_Sections.clear();
	memset( m_szFileName, 0, COMMON_CHAR_BUF_128 );
}


void IniFile::WriteValue( const char* szSec, const char* szKey, const char* value )
{
	if( szSec == NULL || szKey == NULL || value == NULL )
		return;

	Line line;
	line.content;
	line.content += szKey;
	line.content += '=';
	line.content += value;
	line.content += '\n';
	line.section = szSec;
	line.writeadle= false;

	SectionIter it = m_Sections.find( szSec );
	if( it != m_Sections.end() )
	{
		KeyValues& key = it->second;
		KeyValueIter keyIter = key.find( szKey );
		if( keyIter != key.end() )
		{
			const Value& _value = keyIter->second;
			if( strcmp( _value.value.c_str(), value ) == ZERO )
			{
				return;
			}
			line.lineIndex = _value.lineIndex;
			m_Lines[_value.lineIndex] = line;
		}
		else
		{
			int iNoFindValue = 0;
			int iValidLineIndex = GetValidLineIndex( szSec, iNoFindValue );
			if( iValidLineIndex == INVALID_VALUE )
			{
				line.lineIndex = iNoFindValue;
				m_Lines.insert( m_Lines.begin()+iNoFindValue, line );
			}
			else
			{
				line.lineIndex = iValidLineIndex;
				m_Lines[iValidLineIndex] = line;
			}
		}
	}
	else
	{
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