#ifndef __H_FILEHANDLER_H__
#define __H_FILEHANDLER_H__
#include "Common.h"
#include "StringTools.h"
#include "ConstDef.h"
#include <vector>
using std::vector;

typedef vector<string> LINES;
typedef LINES::const_iterator LINES_CITER;
typedef LINES::iterator LINES_ITER;

class FileHandler
{
public:
	FileHandler();
	~FileHandler();
public:
	bool			OpenFile( const char* szFileName, const char* szOpenFlag = "rb");

	void			Close();

	void			WriteFile( const char* szStrem, size_t size );

	long			GetFileSize();

	void			MoveBegin();

	void			MoveEnd();

	bool			GetLineContent( LINES& vContents );

	string			GetLineContent( size_t uLineIndex );

	bool			GetContent( string& content );

	bool			ValidFileSize( size_t& uFileSize  );

private:
	bool			IsNextLine( const char c );

	bool			IsEndOfFile();

private:
	FILE*			m_pFile;
};








#endif // __H_FILEHANDLER_H__