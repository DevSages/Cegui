#ifndef __H_INIFILEHANDLER_H__
#define __H_INIFILEHANDLER_H__
#include <map>
#include <list>
#include "FileHandler.h"
using std::map;
using std::list;

class SectionHandler
{
public:
	struct Section
	{
		string			m_Section;
		LINES			m_Lines;
		Section*		m_NextSection;

		Section( const char* szSec ) : m_Section( szSec ), m_NextSection( NULL ) { }
	};
public:
	SectionHandler();
	~SectionHandler();
public:
	void				Insert( const char* szSec, const char* szContent );

	void				Clear();

	Section*			GetSection( const char* szSec );

	Section*			GetHeadSection() { return m_pHeadSection; }

	Section*			GetTailSection() { return m_pTailSection; }

	bool				IsTailSection( const char* szSec );

private:
	Section*			m_pHeadSection;
	Section*			m_pTailSection;
};

class IniFile
{
public:
	IniFile();
	~IniFile();
protected:
	typedef map< string, string >	KeyValue;
	typedef KeyValue::iterator		KeyValueIter;

	typedef map< string, KeyValue >	Section;
	typedef Section::iterator		SectionIter;
public:
	bool				LoadFile( const char* szFileName );

	void				SaveFile();

	void				WriteInteger( const char* szSec, const char* szKey, int value );

	void				WriteFloat( const char* szSec, const char* szKey, float value );

	void				WriteString( const char* szSec, const char* szKey, const char* value );

	bool				GetBool( const char* szSec, const char* szKey, bool defValue = false );

	int					GetInteger( const char* szSec, const char* szKey, int defValue = -1 );

	float				GetFloat( const char* szSec, const char* szKey, float defValue = 0.f );

	const char*			GetString( const char* szSec, const char* szKey, std::string defValue = "" );

private:
	bool				ParseLine( string& strLine, int iLineNum );

	void				WriteValue( const char* szSec, const char* szKey, const char* szValue );

	const char*			GetValue( const char* szSec, const char* szKey );

	void				ReplaceValueForLine( const char* szSec, const char* szKey, const char* szNewValue );

private:
	Section				m_Sections;
	SectionHandler		m_SectionHandler;
	bool				m_bReady;
	char				m_FileName[PATH_MAX];
private:
	static string		m_InvalidSection;
};





#endif // __H_INIFILEHANDLER_H__