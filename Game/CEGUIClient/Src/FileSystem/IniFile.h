#ifndef __H_INI_FILE_H__
#define __H_INI_FILE_H__
#include "Common.h"
#include "ConstDef.h"
#include "StringCVTools.h"



class IniFile
{
private:
	typedef struct tagLine
	{
		char	section[COMMON_CHAR_BUF_64];
		bool	validLine;
		int		lineNum;
	} Line;
public:
	IniFile();
	IniFile( const char* szFileName );
	~IniFile();
public:
	bool		LoadFile( const char* szFileName );

private:
	bool		IsNextLine( const char c );
	bool		IsLeftBarce( const char c );
	bool		IsRigthBarce( const char c );
	bool		IsEndOfFile();

	void		ParseLine( std::string strLine, int lineNum );
private:
	char		m_szFileName[COMMON_CHAR_BUF_64];
	FILE*		m_pFile;
};




//class IniFile
//{
//public:
//	IniFile(const string & fileName);
//public:
//	virtual ~IniFile(void);
//	
//	const string & getFileName() const;
//
//	const string &getSection() const;
//	void setSection(const string &section);
//	
//	bool write(const string &key, const string & value) const ;
//	bool write(const string &key, int value) const ;
//
//	string readStr(const string &key,const string &default_value) const;
//	int readInt(const string &key, int default_value) const;
//
//public:
//	static int read_profile_string( const char *section, const char *key,char *value, 
//		int size, const char *default_value, const char *file);
//	static int read_profile_int( const char *section, const char *key,int default_value, 
//		const char *file);
//	static int write_profile_string(const char *section, const char *key,
//		const char *value, const char *file);
//
//private:
//	static int load_ini_file(const char *file, char *buf,int *file_size);
//	static int newline(char c);
//	static int end_of_string(char c);
//	static int left_barce(char c);
//	static int right_brace(char c );
//	static int parse_file(const char *section, const char *key, const char *buf,int *sec_s,int *sec_e,
//		int *key_s,int *key_e, int *value_s, int *value_e);
//
//private:
//	string m_fileName;
//	string m_section;
//};

#endif // __H_INI_FILE_H__

