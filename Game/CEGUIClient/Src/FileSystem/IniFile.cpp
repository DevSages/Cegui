#include "IniFile.h"
#include <assert.h>
#include "LogSystem.h"

string IniFile::m_InvalidSection = "INVALID_SECTION";

SectionHandler::SectionHandler() :
	m_pHeadSection( NULL ),
	m_pTailSection( NULL )
{

}
SectionHandler::~SectionHandler()
{

}

void SectionHandler::Insert( const char* szSec, const char* szContent )
{
	if( m_pHeadSection == NULL )
	{
		m_pHeadSection = new Section( szSec );
		if( m_pHeadSection != NULL )
		{
			m_pHeadSection->m_Lines.push_back( szContent );
			m_pTailSection = m_pHeadSection;
		}
		return;
	}

	bool	bFinded = false;
	Section* pSection = m_pHeadSection;
	Section* pPrevSection = NULL;
	while( pSection )
	{
		if( strcmp( pSection->m_Section.c_str(), szSec ) == 0 )
		{
			pSection->m_Lines.push_back( szContent );
			bFinded = true;
			break;
		}
		pPrevSection = pSection;
		pSection = pSection->m_NextSection;
	}

	if( !bFinded && pPrevSection != NULL )
	{
		pPrevSection->m_NextSection = new Section( szSec );
		if( pPrevSection->m_NextSection != NULL )
		{
			pPrevSection->m_NextSection->m_Lines.push_back( szContent );
			m_pTailSection = pPrevSection->m_NextSection;
		}
	}
}

void SectionHandler::Clear()
{
	Section* pSection = m_pHeadSection;
	Section* pDelete = NULL;
	while( pSection )
	{
		pDelete = pSection;
		pSection = pSection->m_NextSection;
		SAFE_DELETE( pDelete );
	}
}

SectionHandler::Section* SectionHandler::GetSection( const char* szSec )
{
	Section* pSection = m_pHeadSection;
	while( pSection )
	{
		if( strcmp( pSection->m_Section.c_str(), szSec ) == 0 )
		{
			return pSection;
		}
		pSection = pSection->m_NextSection;
	}
	return NULL;
}

bool SectionHandler::IsTailSection( const char* szSec )
{
	if( m_pTailSection != NULL )
	{
		if( strcmp( m_pTailSection->m_Section.c_str(), szSec ) == 0 )
			return true;
	}
	return false;
}

IniFile::IniFile():
	m_bReady( false )
{
	memset( m_FileName, 0, PATH_MAX );
}


IniFile::~IniFile()
{
	m_Sections.clear();
	m_SectionHandler.Clear();
	m_bReady = false;
}


bool IniFile::LoadFile( const char* szFileName )
{
	FileHandler fileHander;
	m_bReady =  fileHander.OpenFile( szFileName );

	if( !m_bReady )
	{
		Log_error( "LoadFile Error: %s", szFileName );
		return false;
	}

	strncpy_s( m_FileName, szFileName, PATH_MAX );

	LINES vLines;
	if( !fileHander.GetLineContent( vLines ) )
	{
		Log_error( "Invalid File Size!" );
		return false;
	}

	for( size_t i = 0; i < vLines.size(); i++ )
	{
		ParseLine( vLines[i], i );
	}

	fileHander.Close();
	return true;
}

void IniFile::SaveFile()
{
	FileHandler fileHander;
	if( !fileHander.OpenFile( m_FileName, "wb" ) )
	{
		Log_error( "LoadFile Error: %s", m_FileName );
		return;
	}

	std::string strContens;
	SectionHandler::Section* pSection = m_SectionHandler.GetHeadSection();
	while( pSection )
	{
		LINES lines = pSection->m_Lines;
		size_t lineSize = lines.size();
		for( size_t i = 0; i < lineSize; i++ )
		{
			strContens += lines[i];
		}
		pSection = pSection->m_NextSection;
	}

	fileHander.WriteFile( strContens.c_str(), strContens.size() );
	fileHander.Close();
}

bool IniFile::ParseLine( string& strLine, int iLineNum )
{
	if( strLine.empty() )
		return false;

	static string strCurSection = m_InvalidSection;

	string strLineTmp( strLine );
	StringTools::RemoveSpace( strLineTmp );
	StringTools::RemoveRN( strLineTmp );

	if( strLineTmp.empty() )
	{
		m_SectionHandler.Insert( strCurSection.c_str(), strLine.c_str() );
		return false;
	}

	if( strLineTmp[0] == '[' )
	{
		string::size_type rightBarce = strLineTmp.find_first_of( ']' );
		if( rightBarce == std::string::npos )
		{
			return false;
		}
		strCurSection = strLineTmp.substr( 1, rightBarce-1 );
		KeyValue keyValue;
		m_Sections.insert( make_pair( strCurSection, keyValue ) );
		m_SectionHandler.Insert( strCurSection.c_str(), strLine.c_str() );
	}
	else
	{
		string::size_type equalPos = strLineTmp.find_first_of( '=' );
		if( equalPos == std::string::npos )
		{
			m_SectionHandler.Insert( strCurSection.c_str(), strLine.c_str() );
			return false;
		}

		string strkey, strValue;
		strkey = strLineTmp.substr( 0, equalPos );
		strValue = strLineTmp.substr( equalPos + 1 );

		SectionIter secIter = m_Sections.find( strCurSection );
		if( secIter != m_Sections.end() )
		{
			KeyValue& keyValue = secIter->second;
			keyValue.insert( make_pair( strkey, strValue ) );
		}
		else
		{
			KeyValue keyValue;
			keyValue.insert( make_pair( strkey, strValue ) );
			m_Sections.insert( make_pair( strCurSection, keyValue ) );
		}
		m_SectionHandler.Insert( strCurSection.c_str(), strLine.c_str() );
	}
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

void IniFile::WriteValue( const char* szSec, const char* szKey, const char* szValue )
{
	if( szSec == NULL || szKey == NULL || szValue == NULL || !m_bReady )
	{
		Log_error( "bReady = false!" );
		return;
	}

	SectionIter sectionIter = m_Sections.find( szSec );

	if( sectionIter != m_Sections.end() )
	{
		KeyValue& keyValue = sectionIter->second;
		KeyValueIter keyIter = keyValue.find( szKey );

		if( keyIter != keyValue.end() )
		{
			string& oldValue = keyIter->second;
			keyIter->second = szValue;
			ReplaceValueForLine( szSec, szKey, szValue );
		}
		else
		{
			SectionHandler::Section* pSection = m_SectionHandler.GetSection( szSec );
			if( pSection != NULL )
			{
				keyValue.insert( std::make_pair( szKey, szValue ) );
				string strContent;
				StringTools::Format( strContent, "%s=%s", szKey, szValue );

				LINES& lines = pSection->m_Lines;

				size_t lineNum = -1;
				bool bHaveEmptyLine = false;
				size_t lineSize = lines.size();

				for( size_t i = 0; i < lineSize; i++ )
				{
					const string& lineContent = lines.at( i );
					string strTmp( lineContent );
					StringTools::RemoveRN( strTmp );

					if( strTmp.empty() )
					{
						strContent += "\r\n";
						lines[i] = strContent;
						bHaveEmptyLine = true;
						break;
					}
					else
					{
						lineNum = i;
					}
				}
				if( !bHaveEmptyLine && lineNum != -1 && lineNum < lineSize )
				{
					if( m_SectionHandler.IsTailSection( szSec ) && lineNum == (lineSize - 1) )
					{
						lines[lineNum] += "\r\n";
						lines.push_back( strContent );
					}
					else
					{
						strContent += "\r\n";
						lines.insert( lines.begin()+lineNum + 1, strContent );
					}
				}
			}
		}
	}
	else
	{
		SectionHandler::Section* pSection = m_SectionHandler.GetTailSection();
		if( pSection != NULL )
		{
			LINES& lines = pSection->m_Lines;
			string& strContent = lines.back();
			strContent += "\r\n";
		}

		KeyValue keyValue;
		keyValue.insert( std::make_pair( szKey, szValue ) );
		m_Sections.insert( std::make_pair( szSec, keyValue ) );

		
		string strContent;
		StringTools::Format( strContent, "[%s]\r\n", szSec );
		m_SectionHandler.Insert( szSec, strContent.c_str() );

		StringTools::Format( strContent, "%s=%s", szKey, szValue );
		m_SectionHandler.Insert( szSec, strContent.c_str() );
	}
}

const char* IniFile::GetValue( const char* szSec, const char* szKey )
{
	if( szSec == NULL || szKey == NULL && !m_bReady )
	{
		Log_error( "bReady = false!" );
		return NULL;
	}

	SectionIter it = m_Sections.find( szSec );
	if( it == m_Sections.end() )
	{
		return NULL;
	}

	KeyValue& keyValue = it->second;
	KeyValueIter keyIter = keyValue.find( szKey );

	if( keyIter == keyValue.end() )
	{
		return NULL;
	}

	const string& value = keyIter->second;

	return value.c_str();
}

void IniFile::ReplaceValueForLine(  const char* szSec, const char* szKey, const char* szNewValue )
{
	SectionHandler::Section* pSection = m_SectionHandler.GetSection( szSec );
	if( pSection == NULL )
		return;

	LINES& lines = pSection->m_Lines;
	if( lines.empty() )
		return;

	size_t lineSize = lines.size();
	for( size_t i = 0; i < lineSize; i++ )
	{
		string& strConten = lines[i];
		string::size_type equalPos = strConten.find_first_of( '=' );
		if( equalPos == std::string::npos )
		{
			continue;
		}

		if( strcmp( szKey, strConten.substr( 0, equalPos ).c_str() ) == 0 )
		{
			string::size_type rnPos  = strConten.find( "\r\n" );
			strConten.erase( equalPos + 1 );
			if( rnPos != string::npos )
			{
				strConten += szNewValue;
				strConten += "\r\n";
			}
			else
			{
				strConten += szNewValue;
			}
			break;
		}
	}
}