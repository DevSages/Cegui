#ifndef __H_INI_FILE_H__
#define __H_INI_FILE_H__
#include "Common.h"
#include "ConstDef.h"
#include <vector>
#include <map>




class IniFile
{
public:
	IniFile();
	IniFile( const char* szFileName );
	~IniFile();
private:
	typedef struct tagLine
	{
		std::string		content;
		bool			writeadle;
		std::string		section;
		unsigned int	lineIndex;

		tagLine():
			content( "" ),
			writeadle( false ),
			section( "" ),
			lineIndex( 0)
		{

		}

		tagLine( std::string _content, bool _writeable, std::string _section, unsigned int _lineIndex ):
			content( _content ),
			writeadle( _writeable ),
			section( _section ),
			lineIndex( _lineIndex )
		{

		}

	} Line;

	typedef struct tagValue
	{
		int				lineIndex;
		std::string		value;
	} Value;

	typedef std::vector< Line >	Lines;
	typedef Lines::iterator LinesIter;
	typedef Lines::const_iterator CLinesIter;

	typedef std::map< std::string, Value > KeyValues;
	typedef KeyValues::iterator KeyValueIter;
	typedef KeyValues::const_iterator CKeyValueIter;

	typedef std::map< std::string, KeyValues > Sections;
	typedef Sections::iterator SectionIter;
	
public:
	bool		OpenFile( const char* szFileName );

	bool		LoadFile( const char* szFileName );

	bool		Save();

	void		WriteInteger( const char* szSec, const char* szKey, int value );

	void		WriteFloat( const char* szSec, const char* szKey, float value );

	void		WriteString( const char* szSec, const char* szKey, const char* value );

	bool		GetBool( const char* szSec, const char* szKey, bool defValue = false );

	int			GetInteger( const char* szSec, const char* szKey, int defValue = -1 );

	float		GetFloat( const char* szSec, const char* szKey, float defValue = 0.f );

	const char* GetString( const char* szSec, const char* szKey, std::string defValue = "" );

private:
	void		ParseLine( std::string strLine, int iLineIndex );

	void		Reset();

	void		WriteValue( const char* szSec, const char* szKey, const char* value );

	const char* GetValue( const char* szSec, const char* szKey );

	int			GetValidLineIndex( const char* szSec, int& iNoFindValue ); // return -1;时iNoFindValue是当前section的最后一个keyValue所在行

	bool		IsNextLine( const char c );

	bool		IsEndOfFile( FILE* fp );
private:
	Lines		m_Lines;
	Sections	m_Sections;
	char		m_szFileName[COMMON_CHAR_BUF_128];
};


#endif // __H_INI_FILE_H__

