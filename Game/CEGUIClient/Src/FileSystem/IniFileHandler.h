#ifndef __H_INIFILEHANDLER_H__
#define __H_INIFILEHANDLER_H__
#include <map>
#include "FileHandler.h"
using std::map;



class IniFiles
{
private:
	struct Useable 
	{
		int		m_LineNum;
		bool	m_bEndLineNum;

		Useable() { memset( this, 0, sizeof(Useable) ); }
	};
	typedef vector< Useable >		UseableLine;
	typedef UseableLine::iterator	UseableLineIter;

	typedef map< string, UseableLine > UseableLines;
	typedef UseableLines::iterator	UseableLinesIter;

	typedef map< string, string >	KeyValue;
	typedef KeyValue::iterator		KeyValueIter;

	typedef map< string, KeyValue >	Section;
	typedef Section::iterator		SectionIter;

public:
	IniFiles();
	IniFiles( const char* szFileName );

	bool				LoadFile( const char* szFileName );

	void				WriteInteger( const char* szSec, const char* szKey, int value );

	void				WriteFloat( const char* szSec, const char* szKey, float value );

	void				WriteString( const char* szSec, const char* szKey, const char* value );

	bool				GetBool( const char* szSec, const char* szKey, bool defValue = false );

	int					GetInteger( const char* szSec, const char* szKey, int defValue = -1 );

	float				GetFloat( const char* szSec, const char* szKey, float defValue = 0.f );

	const char*			GetString( const char* szSec, const char* szKey, std::string defValue = "" );

private:
	void				ParseLine( string& strLine, int iLineNum );

	void				WriteValue( const char* szSec, const char* szKey, const char* value );

	const char*			GetValue( const char* szSec, const char* szKey );

private:
	bool				m_bReady;
	FileHandler			m_File;
	UseableLines		m_UseableLines;
	Section				m_Section;
	LINES				m_Lines;
};





#endif // __H_INIFILEHANDLER_H__