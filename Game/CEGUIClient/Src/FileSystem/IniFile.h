#ifndef __H_INI_FILE_H__
#define __H_INI_FILE_H__
#include "Common.h"
#include "ConstDef.h"
#include "StringCVTools.h"
#include <map>
#include <vector>




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
		int				lineIndex;
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

	typedef std::map< std::string, KeyValues > Sections;
	typedef Sections::iterator SectionIter;
public:
	bool		LoadFile( const char* szFileName );

	void		WriteInteger( const char* szSec, const char* szKey, int value );

	void		WriteFloat( const char* szSec, const char* szKey, float value );

	void		WriteString( const char* szSec, const char* szKey, const char* value );

	int			GetInteger( const char* szSec, const char* szKey );

	float		GetFloat( const char* szSec, const char* szKey );

	const char* GetString( const char* szSec, const char* szKey );

	bool		Save();

private:
	void		ParseLine( std::string strLine, int iLineIndex );

	void		Reset();

	void		WriteValue( const char* szSec, const char* szKey, const char* value );

	int			GetValidLineIndex( const char* szSec, int& iNoFindValue ); // return -1;时iNoFindValue是当前section的最后一个keyValue所在行

	bool		IsNextLine( const char c );

	bool		IsEndOfFile();
private:
	FILE*		m_pFile;
	Lines		m_Lines;
	Sections	m_Sections;
	char		m_szFileName[COMMON_CHAR_BUF_128];
};


#endif // __H_INI_FILE_H__

